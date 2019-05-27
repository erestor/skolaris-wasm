#include "SkolarisInstance.h"
#include "gascheduler/src/controller_builder.h"
#include "gascheduler/src/interface/icontroller.h"
#include "gascheduler/src/storage/store.h"
#include "gascheduler/src/timetable/constraint_holder.h"
#include <algorithm/events.h>
#include <ctoolhu/event/firer.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>

using namespace std;
using namespace boost::property_tree;

bool SkolarisInstance::set_jsonData(const string &jsonData, int requestId)
{
	m_Errors.clear();
	int maxTime = 0; //=forever
	ControllerBuilder b { jsonData, maxTime, m_Errors };
	m_Controller = b.BuildController(m_Store, m_ConstraintHolder);
	post_warnings();
	if (!m_Controller) {
		post_error(requestId, "Unable to load timetable data");
		return false;
	}
	return true;
}

bool SkolarisInstance::set_jsonSchedules(const string &jsonSchedules, int requestId)
{
	if (controller()->isRunning()) {
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
	auto currentSolutionPtr = store()->getCurrentSolution();
	unique_ptr<Algorithm::ISolution> backup{currentSolutionPtr->clone()};
	try {
		currentSolutionPtr->load(schedules);
	}
	catch (const exception &e) {
		backup->copyTo(currentSolutionPtr.get());
		post_error(requestId, string("Unable to load schedules: ") + e.what());
		return false;
	}
	return true;
}

bool SkolarisInstance::set_jsonConstraints(const string &jsonConstraints, int requestId)
{
	m_Errors.clear();
	if (controller()->isRunning()) {
		post_error(requestId, "Cannot load constraints while search is running");
		return false;
	}
	ptree constraints;
	try {
		stringstream s(jsonConstraints);
		json_parser::read_json(s, constraints);
	}
	catch(const exception &e) {
		post_error(requestId, string("Unable to parse constraints: ") + e.what());
		return false;
	}
	try {
		m_ConstraintHolder->loadConstraints(constraints);
	}
	catch(const exception &e) {
		post_error(requestId, string("Unable to load constraints: ") + e.what());
		return false;
	}
	post_warnings();
	return true;
}

void SkolarisInstance::set_algorithm(const pp::Var &payload)
{
	pp::VarDictionary dict(payload);
	m_jsonAlgorithm = dict.Get("algorithm").AsString();
    m_benchmarkMode = dict.Get("benchmark").AsBool();
}
