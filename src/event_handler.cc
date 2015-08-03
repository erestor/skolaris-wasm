//----------------------------------------------------------------------------
// Author:		Martin Klemsa
//----------------------------------------------------------------------------
#include "event_handler.h"
#include "gascheduler/src/timetable/constraint.h"
#include "SkolarisInstance.h"
#include <algorithm/isolution.h>
#include <sstream>

using namespace std;

PluginEventHandler::PluginEventHandler(const boost::shared_ptr<SkolarisInstance> &browserAPI)
	: _browserAPI(browserAPI)
{
}

void PluginEventHandler::on(Algorithm::Events::Started *ev)
{
	if (auto api = _browserAPI.lock()) {
		stringstream s;
		s << endl << "Executing " << ev->algorithmName << "..." << endl
				<< "Initial fitness: " << ev->currentSolutionPtr->GetFitness() << (ev->currentSolutionPtr->IsFeasible() ? "F " : "") << endl;
		api->post_text(s.str());
	}
}

void PluginEventHandler::on(Algorithm::Events::Finished *)
{
	if (auto api = _browserAPI.lock())
		api->post_text("\n");
}

void PluginEventHandler::on(Algorithm::Events::BestSolutionFound *ev)
{
	if (auto api = _browserAPI.lock()) {
		api->post_bestsolutionfound();
		stringstream s;
		s << " " << ev->solutionPtr->GetFitness() << (ev->solutionPtr->IsFeasible() ? "F " : " ");
		api->post_text(s.str());
	}
}

void PluginEventHandler::on(Algorithm::Events::FeasibleSolutionFound *ev)
{
	if (auto api = _browserAPI.lock())
		api->post_feasiblesolutionfound();
}

void PluginEventHandler::on(Algorithm::RNA::Events::Tick *)
{
	if (auto api = _browserAPI.lock())
		api->post_text("*");
}

void PluginEventHandler::on(Algorithm::TabuSearch::Events::AfterTabuSearchStep *ev)
{
	if (auto api = _browserAPI.lock()) {
		stringstream s;
		s << ev->stepDescription.data() << " (" << ev->currentSolutionPtr->GetFitness() << ")" << endl;
		api->post_text(s.str());
	}
}

void PluginEventHandler::on(Control::Events::ThreadFinished *ev)
{
	using namespace std::chrono;
	auto milli = ev->elapsedTime;
	int h = duration_cast<hours>(milli).count();
	int m = duration_cast<minutes>(milli).count() % 60;
	int s = duration_cast<seconds>(milli).count() % 60;
	int ms = milli.count() % 1000;
	char formatted[13];
	sprintf(formatted, "%02d:%02d:%02d.%03d", h, m, s, ms);
	if (auto api = _browserAPI.lock())
		api->post_stopped(h, m, s, ms, formatted);
}

void PluginEventHandler::on(Control::Events::Paused *)
{
	if (auto api = _browserAPI.lock())
		api->post_paused();
}

void PluginEventHandler::on(Control::Events::Resumed *)
{
	if (auto api = _browserAPI.lock())
		api->post_resumed();
}

void PluginEventHandler::on(Timetabling::Constraints::Events::AlphaChanged *ev)
{
	if (auto api = _browserAPI.lock()) {
		char formatted[256];
		snprintf(formatted, 255, "Constraint: %s, alpha: %0.2f -> %0.2f\n", ev->constraintPtr->GetName().data(), ev->oldAlpha, ev->newAlpha);
		api->post_text(formatted);
	}
}
