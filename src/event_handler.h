//----------------------------------------------------------------------------
// Author:		Martin Klemsa
//----------------------------------------------------------------------------
#ifndef _event_handler_included_
#define _event_handler_included_

#include "gascheduler/src/timetable/timetable_events.h"
#include "gascheduler/src/timetable/schedule_events.h"
#include "gascheduler/src/timetable/constraint_events.h"
#include "gascheduler/src/control_events.h"
#include <algorithm/events.h>
#include <algorithm/rna_search_events.h>
#include <algorithm/tabu_search_events.h>
#include <ctoolhu/event/subscriber.hpp>
#include <memory>

class SkolarisInstance;

class PluginEventHandler : 
	Ctoolhu::Event::Subscriber<
		Algorithm::Events::Started,
		Algorithm::Events::Finished,
		Algorithm::Events::BestSolutionFound,
		Algorithm::Events::FeasibleSolutionFound,
		Algorithm::RNA::Events::Tick,
		Algorithm::TabuSearch::Events::AfterTabuSearchStep,
		Control::Events::ThreadFinished,
		Control::Events::Paused,
		Control::Events::Resumed,
		Timetabling::Constraints::Events::AlphaChanged> {

  public:

	PluginEventHandler(SkolarisInstance *);

	void on(Algorithm::Events::Started *);
	void on(Algorithm::Events::Finished *);
	void on(Algorithm::Events::BestSolutionFound *);
	void on(Algorithm::Events::FeasibleSolutionFound *);
	void on(Algorithm::RNA::Events::Tick *);
	void on(Algorithm::TabuSearch::Events::AfterTabuSearchStep *);
	void on(Control::Events::ThreadFinished *);
	void on(Control::Events::Paused *);
	void on(Control::Events::Resumed *);
	void on(Timetabling::Constraints::Events::AlphaChanged *ev);

  private:

	SkolarisInstance *_api;
};

#endif //file guard

