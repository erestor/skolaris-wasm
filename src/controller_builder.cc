#include "controller_builder.h"
#include "gascheduler/src/control.h"
#include "gascheduler/src/timetable/constraint_holder.h"
#include "gascheduler/src/timetable/plugin/timetable_pt.h"
#include "gascheduler/src/timetable/plugin/schedule_pt.h"
#include "gascheduler/src/timetable/booking_info.h"
#include "gascheduler/src/timetable/fitness_info.h"
#include "gascheduler/src/timetable/constraint_break_recorder.h"
#include "gascheduler/src/timetable/fast_break_recorder.h"
#include "gascheduler/src/storage/store.h"
#include <boost/property_tree/json_parser.hpp>

using namespace boost::property_tree;
using namespace std;

PluginControllerBuilder::PluginControllerBuilder(
	const string &jsonData,
	int maxTime,
	vector<string> &errors)
:
	jsonData_(jsonData),
	maxTime_(maxTime),
	errors_(errors)
{
}

void PluginControllerBuilder::LogException(const exception &e, const string &place)
{
	errors_.push_back("Caught exception while " + place + ": " + e.what());
}

unique_ptr<IController> PluginControllerBuilder::BuildController(
	shared_ptr<Storage::Store> &storePtr,
	shared_ptr<Timetabling::ConstraintHolder> &constraintHolderPtr,
	vector<ptree> &checkFails
)
{
	try {
		//timetable
		ptree dataPt;
		try {
			stringstream s(jsonData_);
			json_parser::read_json(s, dataPt);
		}
		catch(const exception &e) {
			LogException(e, "parsing timetable JSON");
			return nullptr;
		}

		shared_ptr<Timetabling::TimetablePT> timetable;
		try {
			timetable = make_shared<Timetabling::TimetablePT>(dataPt);
		}
		catch(const exception &e) {
			LogException(e, "constructing timetable");
			return nullptr;
		}
		auto isValid = timetable->IsValid();
		for (auto const &cf : timetable->GetCheckFails()) {
			ptree pt;
			cf->Save(pt);
			checkFails.push_back(pt);
		}

		if (!isValid) {
			errors_.push_back("Timetable data is invalid");
			return nullptr;
		}

		//constraint holder
		try {
			constraintHolderPtr = make_shared<Timetabling::ConstraintHolder>();
		}
		catch(const exception &e) {
			LogException(e, "constructing constraint holder");
			return nullptr;
		}

		//empty initial solution
		shared_ptr<Timetabling::SchedulePT> solution;
		try {
			solution = make_shared<Timetabling::SchedulePT>(timetable, constraintHolderPtr, ptree());
		}
		catch(const exception &e) {
			LogException(e, "constructing initial solution");
			return nullptr;
		}

		//store
		try {
			storePtr = make_shared<Storage::Store>(solution);
		}
		catch(const exception &e) {
			LogException(e, "constructing solution store");
			return nullptr;
		}

		//controller
		bool stopWhenFeasible = false;
		ptree controllerPt;
		controllerPt.put("stopWhenFeasible", stopWhenFeasible);
		controllerPt.put("maxTime", maxTime_);
		try {
			return make_unique<Control::Controller>(storePtr, controllerPt);
		}
		catch (const exception &e) {
			LogException(e, "constructing controller");
		}
	}
	catch (...) {
		errors_.emplace_back("Caught unclassified exception while building controller structures");
	}
	return nullptr;
}
