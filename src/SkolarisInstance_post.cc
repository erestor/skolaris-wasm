#include "SkolarisInstance.h"
#include "gascheduler/src/storage/store.h"
#include "gascheduler/src/timetable/timetable_events.h"
#include <ctoolhu/event/firer.hpp>
#include <boost/property_tree/ptree.hpp>
#include <string>

using namespace boost::property_tree;
using namespace std;

const string SKOLARIS_VERSION("9.8.1"
#ifdef DEBUG
	"debug"
#endif
);

const string SKOLARIS_VERSION_MAJOR("9");
const string SKOLARIS_VERSION_MINOR("8");
const string SKOLARIS_VERSION_PATCH("1");

void SkolarisInstance::post_complete(int requestId)
{
	ptree dictionary;
	dictionary.put("reqId", requestId);
	PostMessage(dictionary);
}

void SkolarisInstance::post_message(const std::string &type)
{
	ptree dictionary;
	dictionary.put("type", type);
	PostMessage(dictionary);
}

void SkolarisInstance::post_message(const std::string &type, const string &payload)
{
	ptree dictionary;
	dictionary.put("type", type);
	dictionary.put("payload", payload);
	PostMessage(dictionary);
}

void SkolarisInstance::post_message(const std::string &type, const ptree &payload)
{
	ptree dictionary;
	dictionary.put("type", type);
	dictionary.put_child("payload", payload);
	PostMessage(dictionary);
}

void SkolarisInstance::post_message(int requestId, const std::string &type)
{
	ptree dictionary;
	dictionary.put("reqId", requestId);
	dictionary.put("type", type);
	PostMessage(dictionary);
}

void SkolarisInstance::post_message(int requestId, const std::string &type, const string &payload)
{
	ptree dictionary;
	dictionary.put("reqId", requestId);
	dictionary.put("type", type);
	dictionary.put("payload", payload);
	PostMessage(dictionary);
}

void SkolarisInstance::post_message(int requestId, const std::string &type, const ptree &payload)
{
	ptree dictionary;
	dictionary.put("reqId", requestId);
	dictionary.put("type", type);
	dictionary.put_child("payload", payload);
	PostMessage(dictionary);
}

void SkolarisInstance::post_error(int requestId, const std::string &what)
{
	post_message(requestId, "error", what);
}

void SkolarisInstance::post_text(const std::string &what)
{
	post_message("text", what);
}

void SkolarisInstance::post_version(int requestId)
{
	ptree dict;
	dict.put("version", SKOLARIS_VERSION);
	dict.put("major", SKOLARIS_VERSION_MAJOR);
	dict.put("minor", SKOLARIS_VERSION_MINOR);
	dict.put("patch", SKOLARIS_VERSION_PATCH);
	post_message(requestId, "version", dict);
}


void SkolarisInstance::post_currentsolution(int requestId)
{
	string s;
	if (stringifySolution(s, store()->getCurrentSolution()))
		post_message(requestId, "currentsolution", s);
	else
		post_error(requestId, s);
}

void SkolarisInstance::post_bestsolution(int requestId)
{
	string s;
	if (stringifySolution(s, store()->getBestSolution()))
		post_message(requestId, "bestsolution", s);
	else
		post_error(requestId, s);
}

void SkolarisInstance::post_feasiblesolution(int requestId)
{
	string s;
	if (stringifySolution(s, store()->getFeasibleSolution()))
		post_message(requestId, "feasiblesolution", s);
	else
		post_error(requestId, s);
}

void SkolarisInstance::post_bestoverallsolution(int requestId)
{
    string s;
    if (stringifySolution(s, store()->getBestOverallSolution()))
        post_message(requestId, "bestoverallsolution", s);
    else
        post_error(requestId, s);
}

void SkolarisInstance::post_feasibleoverallsolution(int requestId)
{
    string s;
    if (stringifySolution(s, store()->getFeasibleOverallSolution()))
        post_message(requestId, "feasibleoverallsolution", s);
    else
        post_error(requestId, s);
}

void SkolarisInstance::post_currentsolutionchanged(Algorithm::ISolution *solutionPtr)
{
	string s;
	if (stringifyFitnessSummary(s, solutionPtr))
		post_message("currentsolutionchanged", s);
	else
		post_error(0, s);
}

void SkolarisInstance::post_bestsolutionfound(Algorithm::ISolution *solutionPtr)
{
	string s;
	if (stringifyFitnessSummary(s, solutionPtr))
		post_message("bestsolutionfound", s);
	else
		post_error(0, s);
}

void SkolarisInstance::post_feasiblesolutionfound(Algorithm::ISolution *solutionPtr)
{
	string s;
	if (stringifyFitnessSummary(s, solutionPtr))
		post_message("feasiblesolutionfound", s);
	else
		post_error(0, s);
}


void SkolarisInstance::post_warnings()
{
	auto event = Timetabling::TimetableEvents::Check();
	Ctoolhu::Event::Fire(event);
	if (!event.valid)
		m_Errors.push_back("Timetable data is invalid");

	post_message("messages", stringifyMessages(event.messages));
}


void SkolarisInstance::post_started(int requestId)
{
	post_message(requestId, "started");
}

void SkolarisInstance::post_paused()
{
	post_message("paused");
}

void SkolarisInstance::post_resumed()
{
	post_message("resumed");
}

void SkolarisInstance::post_stopped(int h, int m, int s, int ms)
{
	ptree timeDict;
	timeDict.put("h", h);
	timeDict.put("m", m);
	timeDict.put("s", s);
	timeDict.put("ms", ms);
	post_message("stopped", timeDict);
}
