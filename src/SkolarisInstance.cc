#include "SkolarisInstance.h"
#include "event_handler.h"
#include "algorithm_builder.h"
#include "controller_builder.h"
#include "gascheduler/src/timetable/constraint_holder.h"
#include "gascheduler/src/interface/icontroller.h"
#include "gascheduler/src/storage/store.h"
#include <algorithm/isolution.h>
#include <ctoolhu/random/engine.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>
#include <string>

using namespace std;
using namespace boost::property_tree;

/// The Instance class.  One of these exists for each instance of your NaCl
/// module on the web page.  The browser will ask the Module object to create
/// a new Instance for each occurrence of the <embed> tag that has these
/// attributes:
///     src="hello_tutorial.nmf"
///     type="application/x-pnacl"
/// To communicate with the browser, you must override HandleMessage() to
/// receive messages from the browser, and use PostMessage() to send messages
/// back to the browser.  Note that this interface is asynchronous.
  
/// The constructor creates the plugin-side instance.
  /// @param[in] instance the handle to the browser-side plugin instance.
SkolarisInstance::SkolarisInstance(PP_Instance instance)
:
	pp::Instance(instance)
{
	m_EventHandler = make_unique<PluginEventHandler>(this);
}

SkolarisInstance::~SkolarisInstance()
{
}

string SkolarisInstance::StringifySolution(const shared_ptr<Algorithm::ISolution> &solution)
{
	if (!solution)
		return string();

	try {
		ptree data;
		solution->Save(data);
		stringstream s;
		json_parser::write_json(s, data);
		return s.str();
	}
	catch (exception &e) {
		post_error(string("Unable to stringify solution: ") + e.what());
		return string();
	}
	catch(...) {
		post_error(string("Unable to stringify solution: unknown exception"));
		return string();
	}
}

string SkolarisInstance::StringifyMessages() const
{
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
void SkolarisInstance::LoadData(const string &jsonData)
{
	m_Errors.clear();
	m_CheckFails.clear();
	int maxTime = 5; //has no effect at the moment
	PluginControllerBuilder b { jsonData, maxTime, m_Errors };
	m_Controller = b.BuildController(m_Store, m_ConstraintHolder, m_CheckFails);
}

void SkolarisInstance::LoadSchedules(const string &jsonSchedules)
{
	if (Controller()->IsRunning()) {
		post_error("Cannot load schedules while search is running");
		return;
	}
	ptree schedules;
	try {
		stringstream s(jsonSchedules);
		json_parser::read_json(s, schedules);
	}
	catch(...) {
		post_error("Unable to parse schedules");
		return;
	}
	auto currentSolutionPtr = Store()->GetCurrentSolution();
	auto backup = currentSolutionPtr->Clone();
	try {
		currentSolutionPtr->Load(schedules);
	}
	catch(...) {
		backup->CopyTo(currentSolutionPtr.get());
		post_error("Unable to load schedules");
		return;
	}
	if (currentSolutionPtr->GetFitness() < Store()->GetBestSolution()->GetFitness())
		Store()->SetBestSolution();

	if (currentSolutionPtr->IsFeasible()) {
		auto storedFeasibleSolution = Store()->GetFeasibleSolution();
		if (!storedFeasibleSolution || currentSolutionPtr->GetFitness() < storedFeasibleSolution->GetFitness()) {
			Store()->SetFeasibleSolution();
			post_feasiblesolutionfound();
		}
	}
}

void SkolarisInstance::LoadConstraints(const string &jsonConstraints)
{
	if (Controller()->IsRunning()) {
		post_error("Cannot load constraints while search is running");
		return;
	}
	ptree constraints;
	try {
		stringstream s(jsonConstraints);
		json_parser::read_json(s, constraints);
	}
	catch(...) {
		post_error("Unable to parse constraints");
		return;
	}
	try {
		m_ConstraintHolder->LoadConstraints(constraints);
	}
	catch(...) {
		post_error("Unable to load constraints");
		return;
	}
	Store()->GetCurrentSolution()->MarkDirty();
	Store()->GetBestSolution()->MarkDirty();
	if (Store()->GetFeasibleSolution())
		Store()->GetFeasibleSolution()->MarkDirty();
}

IController *SkolarisInstance::Controller()
{
	if (!m_Controller) {
		post_error("Skolaris plugin controller has not been initialized. Open the errors/warnings console for details.");
		return nullptr;
	}
	return m_Controller.get();
}

Ctoolhu::Thread::LockingProxy<Storage::Store> SkolarisInstance::Store() const
{
	return Storage::LockStore(m_Store.get());
}

void SkolarisInstance::Start(const string &jsonAlgorithm, bool benchmark)
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
		if (Controller()->Start(move(algorithm)))
			post_started();
	}
	catch(...) {
		post_error("Unable to start search");
	}
}
