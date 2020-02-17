#include "SkolarisInstance.h"
#include "gascheduler/src/timetable/timetable_events.h"
#include <ctoolhu/event/firer.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <string>

using namespace boost::property_tree;
using namespace std;

const string SKOLARIS_VERSION("9.13.0"
#ifdef DEBUG
	"debug"
#endif
);

const string SKOLARIS_VERSION_MAJOR("9");
const string SKOLARIS_VERSION_MINOR("13");
const string SKOLARIS_VERSION_PATCH("0");

void SkolarisInstance::postMessage(const ptree &data) const
{
	std::stringstream s;
	json_parser::write_json(s, data);
	auto result = s.str();
	postMessage(result.c_str());
}

void SkolarisInstance::post_complete(int requestId) const
{
	ptree dictionary;
	dictionary.put("reqId", requestId);
	postMessage(dictionary);
}

void SkolarisInstance::post_message(const std::string &type) const
{
	ptree dictionary;
	dictionary.put("type", type);
	postMessage(dictionary);
}

void SkolarisInstance::post_message(const std::string &type, const string &payload) const
{
	ptree dictionary;
	dictionary.put("type", type);
	dictionary.put("payload", payload);
	postMessage(dictionary);
}

void SkolarisInstance::post_message(const std::string &type, const ptree &payload) const
{
	ptree dictionary;
	dictionary.put("type", type);
	dictionary.put_child("payload", payload);
	postMessage(dictionary);
}

void SkolarisInstance::post_message(int requestId, const std::string &type) const
{
	ptree dictionary;
	dictionary.put("reqId", requestId);
	dictionary.put("type", type);
	postMessage(dictionary);
}

void SkolarisInstance::post_message(int requestId, const std::string &type, const string &payload) const
{
	ptree dictionary;
	dictionary.put("reqId", requestId);
	dictionary.put("type", type);
	dictionary.put("payload", payload);
	postMessage(dictionary);
}

void SkolarisInstance::post_message(int requestId, const std::string &type, const ptree &payload) const
{
	ptree dictionary;
	dictionary.put("reqId", requestId);
	dictionary.put("type", type);
	dictionary.put_child("payload", payload);
	postMessage(dictionary);
}

void SkolarisInstance::post_error(int requestId, const std::string &what) const
{
	post_message(requestId, "error", what);
}

void SkolarisInstance::post_text(const std::string &what) const
{
	post_message("text", what);
}

void SkolarisInstance::post_version(int requestId) const
{
	ptree dict;
	dict.put("version", SKOLARIS_VERSION);
	dict.put("major", SKOLARIS_VERSION_MAJOR);
	dict.put("minor", SKOLARIS_VERSION_MINOR);
	dict.put("patch", SKOLARIS_VERSION_PATCH);
	post_message(requestId, "version", dict);
}


void SkolarisInstance::post_currentsolution(int requestId) const
{
	string s;
	if (stringifySolution(s, lockStore()->getCurrentSolution()))
		post_message(requestId, "currentsolution", s);
	else
		post_error(requestId, s);
}

void SkolarisInstance::post_bestsolution(int requestId) const
{
	string s;
	if (stringifySolution(s, lockStore()->getBestSolution()))
		post_message(requestId, "bestsolution", s);
	else
		post_error(requestId, s);
}

void SkolarisInstance::post_feasiblesolution(int requestId) const
{
	string s;
	if (stringifySolution(s, lockStore()->getFeasibleSolution()))
		post_message(requestId, "feasiblesolution", s);
	else
		post_error(requestId, s);
}

void SkolarisInstance::post_bestoverallsolution(int requestId) const
{
    string s;
    if (stringifySolution(s, lockStore()->getBestOverallSolution()))
        post_message(requestId, "bestoverallsolution", s);
    else
        post_error(requestId, s);
}

void SkolarisInstance::post_feasibleoverallsolution(int requestId) const
{
    string s;
    if (stringifySolution(s, lockStore()->getFeasibleOverallSolution()))
        post_message(requestId, "feasibleoverallsolution", s);
    else
        post_error(requestId, s);
}

void SkolarisInstance::post_currentsolutionchanged(Algorithm::ISolution *solutionPtr) const
{
	string s;
	if (stringifyFitnessSummary(s, solutionPtr))
		post_message("currentsolutionchanged", s);
	else
		post_error(0, s);
}

void SkolarisInstance::post_bestsolutionfound(Algorithm::ISolution *solutionPtr) const
{
	string s;
	if (stringifyFitnessSummary(s, solutionPtr))
		post_message("bestsolutionfound", s);
	else
		post_error(0, s);
}

void SkolarisInstance::post_feasiblesolutionfound(Algorithm::ISolution *solutionPtr) const
{
	string s;
	if (stringifyFitnessSummary(s, solutionPtr))
		post_message("feasiblesolutionfound", s);
	else
		post_error(0, s);
}

void SkolarisInstance::post_started(int requestId) const
{
	post_message(requestId, "started");
}

void SkolarisInstance::post_paused() const
{
	post_message("paused");
}

void SkolarisInstance::post_resumed() const
{
	post_message("resumed");
}

void SkolarisInstance::post_stopped(int h, int m, int s, int ms) const
{
	ptree timeDict;
	timeDict.put("h", h);
	timeDict.put("m", m);
	timeDict.put("s", s);
	timeDict.put("ms", ms);
	post_message("stopped", timeDict);
}
