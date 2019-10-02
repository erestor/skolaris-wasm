//----------------------------------------------------------------------------
// Author:		Martin Klemsa
//----------------------------------------------------------------------------
#ifndef _event_handler_included_
#define _event_handler_included_

#include "gascheduler/src/timetable/timetable_events.h"
#include "gascheduler/src/timetable/schedule_events.h"
#include "gascheduler/src/timetable/constraint_events.h"
#include "gascheduler/src/control_events.h"
#include <localsearch/events/events.h>
#include <localsearch/events/rna_search_events.h>
#include <localsearch/events/tabu_search_events.h>
#include <ctoolhu/event/subscriber.hpp>
#include <ctoolhu/event/events.h>
#include <memory>

class SkolarisInstance;

class PluginEventHandler : 
	Ctoolhu::Event::Subscriber<
		Ctoolhu::Event::Message,
		Timetabling::TimetableEvents::BeforeInit,
		Timetabling::TimetableEvents::AfterInit,
		Algorithm::Events::Started,
		Algorithm::Events::Finished,
		Algorithm::Events::CurrentSolutionChanged,
		Algorithm::Events::BestSolutionFound,
		Algorithm::Events::FeasibleSolutionFound,
		Algorithm::RNA::Events::Tick,
		Algorithm::TabuSearch::Events::StepExecuted,
		Control::Events::Exception,
		Control::Events::ThreadFinished,
		Control::Events::Paused,
		Control::Events::Resumed,
		Timetabling::ConstraintEvents::AlphaChanged> {

  public:

	PluginEventHandler(SkolarisInstance *);

	void on(Ctoolhu::Event::Message *);
	void on(Timetabling::TimetableEvents::BeforeInit *);
	void on(Timetabling::TimetableEvents::AfterInit *);
	void on(Algorithm::Events::Started *);
	void on(Algorithm::Events::Finished *);
	void on(Algorithm::Events::CurrentSolutionChanged *);
	void on(Algorithm::Events::BestSolutionFound *);
	void on(Algorithm::Events::FeasibleSolutionFound *);
	void on(Algorithm::RNA::Events::Tick *);
	void on(Algorithm::TabuSearch::Events::StepExecuted *);
	void on(Control::Events::Exception *);
	void on(Control::Events::ThreadFinished *);
	void on(Control::Events::Paused *);
	void on(Control::Events::Resumed *);
	void on(Timetabling::ConstraintEvents::AlphaChanged *ev);

  private:

	SkolarisInstance *_api;
};

#endif //file guard

