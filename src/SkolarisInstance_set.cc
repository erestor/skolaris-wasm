#include "SkolarisInstance.h"
#include "gascheduler/src/controller_builder.h"
#include "gascheduler/src/interface/icontroller.h"
#include "gascheduler/src/storage/store.h"
#include "gascheduler/src/timetable/plugin/constraint_holder_pt.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>

using namespace std;
using namespace boost::property_tree;

bool SkolarisInstance::set_jsonData(const string &jsonData, int requestId)
{
	m_Errors.clear();
	m_CheckFails.clear();
	int maxTime = 0; //=forever
	ControllerBuilder b { jsonData, maxTime, m_Errors };
	m_Controller = b.BuildController(m_Store, m_ConstraintHolder, m_CheckFails);
	post_messages();
	if (!m_Controller) {
		post_error(requestId, "Unable to load timetable data");
		return false;
	}
	return true;
}

bool SkolarisInstance::set_jsonSchedules(const string &jsonSchedules, int requestId)
{
	if (controller()->IsRunning()) {
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
	auto currentSolutionPtr = store()->GetCurrentSolution();
	unique_ptr<Algorithm::ISolution> backup{currentSolutionPtr->Clone()};
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
	auto fitness = currentSolutionPtr->GetFitness();
	if (fitness < store()->GetBestSolution()->GetFitness()) {
		store()->SetBestSolution();
		post_bestsolutionfound(currentSolutionPtr.get());
	}
	if (currentSolutionPtr->IsFeasible()) {
		auto storedFeasibleSolution = store()->GetFeasibleSolution();
		if (!storedFeasibleSolution || fitness < storedFeasibleSolution->GetFitness()) {
			store()->SetFeasibleSolution();
			post_feasiblesolutionfound(currentSolutionPtr.get());
		}
	}
	return true;
}

bool SkolarisInstance::set_jsonConstraints(const string &jsonConstraints, int requestId)
{
	if (controller()->IsRunning()) {
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
	store()->GetCurrentSolution()->MarkDirty();
	store()->GetBestSolution()->MarkDirty();
	auto storedFeasibleSolution = store()->GetFeasibleSolution();
	if (storedFeasibleSolution) {
		storedFeasibleSolution->MarkDirty();
		if (!storedFeasibleSolution->IsFeasible())
			store()->ResetFeasibleSolution();
	}
	return true;
}

void SkolarisInstance::set_algorithm(const pp::Var &payload)
{
	pp::VarDictionary dict(payload);
	m_jsonAlgorithm = dict.Get("algorithm").AsString();
    m_benchmarkMode = dict.Get("benchmark").AsBool();
}
