#include "event_handler.h"
#include "SkolarisInstance.h"
#include <localsearch/interface/isolution.h>
#include <chrono>
#include <sstream>

using namespace std;

PluginEventHandler::PluginEventHandler(SkolarisInstance *api)
	: _api(api)
{
}

void PluginEventHandler::on(Ctoolhu::Event::Message *ev)
{
	_api->post_text(ev->msg + '\n');
}

void PluginEventHandler::on(Timetabling::TimetableEvents::BeforeInit *)
{
	_api->post_text("Initializing timetable...\n");
}

void PluginEventHandler::on(Timetabling::TimetableEvents::AfterInit *)
{
	_api->post_text("Timetable initialization complete\n");
}

void PluginEventHandler::on(Algorithm::Events::Started *ev)
{
	ostringstream s;
	s << endl << "Executing " << ev->algorithmName << "..." << endl
			<< "Initial fitness: " << ev->solutionPtr->getFitness() << (ev->solutionPtr->isFeasible() ? "F " : "") << endl;
	_api->post_text(s.str());
}

void PluginEventHandler::on(Algorithm::Events::Finished *)
{
	_api->post_text("\n");
}

void PluginEventHandler::on(Algorithm::Events::CurrentSolutionChanged *ev)
{
	_api->post_currentsolutionchanged(ev->solutionPtr);
}

void PluginEventHandler::on(Algorithm::Events::BestSolutionFound *ev)
{
	_api->post_bestsolutionfound(ev->solutionPtr);
	ostringstream s;
	s << " " << ev->solutionPtr->getFitness() << (ev->solutionPtr->isFeasible() ? "F " : " ");
	_api->post_text(s.str());
}

void PluginEventHandler::on(Algorithm::Events::FeasibleSolutionFound *ev)
{
	_api->post_feasiblesolutionfound(ev->solutionPtr);
}

void PluginEventHandler::on(Algorithm::RNA::Events::Tick *)
{
	_api->post_text("*");
}

void PluginEventHandler::on(Algorithm::TabuSearch::Events::StepExecuted *ev)
{
	ostringstream s;
	s << ev->stepDescription.data() << " (" << ev->currentSolutionPtr->getFitness() << ')' << endl;
	_api->post_text(s.str());
}

void PluginEventHandler::on(Control::Events::Exception *ev)
{
	_api->post_text(ev->what + '\n');
}

void PluginEventHandler::on(Control::Events::Finished *ev)
{
	using namespace std::chrono;
	auto milli = ev->elapsedTime;
	int h = duration_cast<hours>(milli).count();
	int m = duration_cast<minutes>(milli).count() % 60;
	int s = duration_cast<seconds>(milli).count() % 60;
	int ms = milli.count() % 1000;
	_api->post_stopped(h, m, s, ms);
}

void PluginEventHandler::on(Control::Events::Paused *)
{
	_api->post_paused();
}

void PluginEventHandler::on(Control::Events::Resumed *)
{
	_api->post_resumed();
}

void PluginEventHandler::on(Timetabling::ConstraintEvents::AlphaChanged *ev)
{
	ostringstream s;
	s << "Constraint: " << ev->name << ", alpha -> " << ev->newAlpha << '\n';
	_api->post_text(s.str());
}
