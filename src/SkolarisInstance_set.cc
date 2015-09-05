#include "SkolarisInstance.h"
#include "controller_builder.h"
#include "gascheduler/src/interface/icontroller.h"
#include "gascheduler/src/storage/store.h"
#include "gascheduler/src/timetable/constraint_holder.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>

using namespace std;
using namespace boost::property_tree;

void SkolarisInstance::set_jsonData(const string &jsonData)
{
	m_Errors.clear();
	m_CheckFails.clear();
	int maxTime = 5; //has no effect at the moment
	PluginControllerBuilder b { jsonData, maxTime, m_Errors };
	m_Controller = b.BuildController(m_Store, m_ConstraintHolder, m_CheckFails);
	post_messages();
}

bool SkolarisInstance::set_jsonSchedules(const string &jsonSchedules, int requestId)
{
	if (Controller()->IsRunning()) {
		post_error(requestId, "Cannot load schedules while search is running");
		return false;
	}
	ptree schedules;
	try {
		stringstream s(jsonSchedules);
		json_parser::read_json(s, schedules);
	}
	catch (const exception &e) {
		post_error(requestId, string("Unable to parse schedules: ") + e.what());
		return false;
	}
	catch(...) {
		post_error(requestId, "Unable to parse schedules");
		return false;
	}
	auto currentSolutionPtr = Store()->GetCurrentSolution();
	auto backup = currentSolutionPtr->Clone();
	try {
		currentSolutionPtr->Load(schedules);
	}
	catch (const exception &e) {
		backup->CopyTo(currentSolutionPtr.get());
		post_error(requestId, string("Unable to load schedules: ") + e.what());
		return false;
	}
	catch(...) {
		backup->CopyTo(currentSolutionPtr.get());
		post_error(requestId, "Unable to load schedules");
		return false;
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
	return true;
}

bool SkolarisInstance::set_jsonConstraints(const string &jsonConstraints, int requestId)
{
	if (Controller()->IsRunning()) {
		post_error(requestId, "Cannot load constraints while search is running");
		return false;
	}
	ptree constraints;
	try {
		stringstream s(jsonConstraints);
		json_parser::read_json(s, constraints);
	}
	catch(...) {
		post_error(requestId, "Unable to parse constraints");
		return false;
	}
	try {
		m_ConstraintHolder->LoadConstraints(constraints);
	}
	catch(...) {
		post_error(requestId, "Unable to load constraints");
		return false;
	}
	Store()->GetCurrentSolution()->MarkDirty();
	Store()->GetBestSolution()->MarkDirty();
	if (Store()->GetFeasibleSolution())
		Store()->GetFeasibleSolution()->MarkDirty();

	return true;
}

void SkolarisInstance::set_algorithm(const pp::Var &payload)
{
	pp::VarDictionary dict(payload);
	m_jsonAlgorithm = dict.Get("algorithm").AsString();
    m_benchmarkMode = dict.Get("benchmark").AsBool();
}
