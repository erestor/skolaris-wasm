#include "SkolarisInstance.h"
#include "gascheduler/src/controller_builder.hpp"
#include "gascheduler/src/timetable/constraint_holder.h"
#include <localsearch/events/events.h>
#include <ctoolhu/event/firer.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>

using namespace std;
using namespace boost::property_tree;

bool SkolarisInstance::set_jsonData(const string &jsonData, int requestId)
{
	_errors.clear();
	int maxTime = 0; //=forever
	ControllerBuilder<Timetabling::Schedule> b { jsonData, maxTime, _errors };
	_controller = b.BuildController(_store, _constraintHolder, _timetable);
	post_warnings();
	if (!_controller) {
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
	auto currentSolutionPtr = lockStore()->getCurrentSolution();
	Timetabling::Schedule backup(*currentSolutionPtr);
	try {
		currentSolutionPtr->load(schedules);
	}
	catch (const exception &e) {
		*currentSolutionPtr = backup;
		post_error(requestId, string("Unable to load schedules: ") + e.what());
		return false;
	}
	return true;
}

bool SkolarisInstance::set_jsonConstraints(const string &jsonConstraints, int requestId)
{
	_errors.clear();
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
		_constraintHolder->loadConstraints(constraints, *_timetable);
	}
	catch(const exception &e) {
		post_error(requestId, string("Unable to load constraints: ") + e.what());
		return false;
	}
	post_warnings();
	return true;
}

void SkolarisInstance::set_algorithm(const ptree &payload)
{
	_jsonAlgorithm = payload.get<string>("algorithm");
    _benchmarkMode = payload.get<bool>("benchmark");
}
