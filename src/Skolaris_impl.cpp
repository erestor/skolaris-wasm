#include "Skolaris.h"
#include "SkolarisAPI.h"
#include "../GAScheduler/src/interface/icontroller.h"
#include "../GAScheduler/src/plugin/algorithm_builder.h"
#include "../GAScheduler/src/plugin/controller_builder.h"
#include "../GAScheduler/src/plugin/event_handler.h"
#include "../GAScheduler/src/storage/store.h"
#include "../GAScheduler/src/timetable/constraint_holder.h"
#include <algorithm/isolution.h>
#include <ctoolhu/random/engine.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>

using namespace std;
using namespace boost::property_tree;

string Skolaris::StringifySolution(const shared_ptr<Algorithm::ISolution> &solution)
{
	using namespace boost::property_tree;

	if (!solution)
		return string();

	ptree data;
	try {
		solution->Save(data);
	}
	catch (exception &e) {
		throw FB::script_error(string("Error occurred while serializing solution: ") + e.what());
	}
	try {
		stringstream s;
		json_parser::write_json(s, data);
		return s.str();
	}
	catch(...) {
		throw FB::script_error("Unable to stringify solution");
	}
}

string Skolaris::StringifyMessages() const
{
	using namespace boost::property_tree;

	ptree errors;
	for (auto const &e : m_Errors) {
		ptree child;
		child.put("", e);
		errors.push_back(std::make_pair("", child));
	}
	ptree checkFails;
	for (auto const &cf : m_CheckFails) {
		checkFails.push_back(std::make_pair("", cf));
	}
	ptree messages;
	messages.add_child("errors", errors);
	messages.add_child("checkFails", checkFails);
	stringstream s;
	json_parser::write_json(s, messages);
	return s.str();
}

//====================================================================================================================
void Skolaris::LoadData(const string &jsonData)
{
	m_Errors.clear();
	m_CheckFails.clear();
	int maxTime = 5; //has no effect at the moment
	PluginControllerBuilder b { jsonData, maxTime, m_Errors };
	m_Controller = b.BuildController(m_Store, m_ConstraintHolder, m_CheckFails);
}

void Skolaris::LoadSchedules(const string &jsonSchedules)
{
	if (Controller()->IsRunning())
		throw FB::script_error("Cannot load schedules while search is running");

	ptree schedules;
	try {
		stringstream s(jsonSchedules);
		json_parser::read_json(s, schedules);
	}
	catch(...) {
		throw FB::script_error("Unable to parse schedules");
	}
	auto currentSolutionPtr = Store()->GetCurrentSolution();
	auto backup = currentSolutionPtr->Clone();
	try {
		currentSolutionPtr->Load(schedules);
	}
	catch(...) {
		backup->CopyTo(currentSolutionPtr.get());
		throw FB::script_error("Unable to load schedules");
	}
	if (currentSolutionPtr->GetFitness() < Store()->GetBestSolution()->GetFitness())
		Store()->SetBestSolution();

	if (currentSolutionPtr->IsFeasible()) {
		auto storedFeasibleSolution = Store()->GetFeasibleSolution();
		if (!storedFeasibleSolution || currentSolutionPtr->GetFitness() < storedFeasibleSolution->GetFitness()) {
			Store()->SetFeasibleSolution();
			auto const &apiPtr = boost::static_pointer_cast<SkolarisAPI>(getRootJSAPI());
			apiPtr->fire_feasiblesolutionfound();
		}
	}
}

void Skolaris::LoadConstraints(const string &jsonConstraints)
{
	if (Controller()->IsRunning())
		throw FB::script_error("Cannot load constraints while search is running");

	ptree constraints;
	try {
		stringstream s(jsonConstraints);
		json_parser::read_json(s, constraints);
	}
	catch(...) {
		throw FB::script_error("Unable to parse constraints");
	}
	try {
		m_ConstraintHolder->LoadConstraints(constraints);
	}
	catch(...) {
		throw FB::script_error("Unable to load constraints");
	}
	Store()->GetCurrentSolution()->MarkDirty();
	Store()->GetBestSolution()->MarkDirty();
	if (Store()->GetFeasibleSolution())
		Store()->GetFeasibleSolution()->MarkDirty();
}

const Skolaris::controller_ptr_type &Skolaris::Controller()
{
	if (!m_Controller)
		throw FB::script_error("Skolaris plugin controller has not been initialized. Open the errors/warnings console for details.");

	//make sure event handler is ready when controller is first requested
	if (!m_EventHandler) {
		auto const &apiPtr = boost::static_pointer_cast<SkolarisAPI>(getRootJSAPI());
		m_EventHandler = make_unique<PluginEventHandler>(apiPtr);
	}
	return m_Controller;
}

Ctoolhu::Thread::LockingProxy<Storage::Store> Skolaris::Store() const
{
	if (!m_Store)
		throw FB::script_error("Skolaris plugin solution store has not been initialized. Open the errors/warnings console for details.");

	return Storage::LockStore(m_Store.get());
}

bool Skolaris::Start(const string &jsonAlgorithm, bool benchmark)
{
	try {
		auto &engine = Ctoolhu::Random::Private::SingleRandomEngine::Instance();
		if (benchmark)
			engine.seed(0);
		else {
			random_device rd;
			engine.seed(rd());
		}
		auto algorithm = PluginAlgorithmBuilder::BuildAlgorithm(jsonAlgorithm);
		return Controller()->Start(move(algorithm));
	}
	catch(...) {
		throw FB::script_error("Unable to start search");
	}
}
