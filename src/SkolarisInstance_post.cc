#include "SkolarisInstance.h"
#include "gascheduler/src/storage/store.h"
#include <string>

using namespace std;

const string SKOLARIS_VERSION("9.7.0"
#ifdef DEBUG
	"debug"
#endif
);

const string SKOLARIS_VERSION_MAJOR("9");
const string SKOLARIS_VERSION_MINOR("7");
const string SKOLARIS_VERSION_PATCH("0");

void SkolarisInstance::post_complete(int requestId)
{
	pp::VarDictionary dictionary;
	dictionary.Set("reqId", requestId);
	PostMessage(dictionary);
}

void SkolarisInstance::post_message(const std::string &type)
{
	pp::VarDictionary dictionary;
	dictionary.Set("type", type);
	PostMessage(dictionary);
}

void SkolarisInstance::post_message(const std::string &type, const pp::Var &payload)
{
	pp::VarDictionary dictionary;
	dictionary.Set("type", type);
	dictionary.Set("payload", payload);
	PostMessage(dictionary);
}

void SkolarisInstance::post_message(int requestId, const std::string &type)
{
	pp::VarDictionary dictionary;
	dictionary.Set("reqId", requestId);
	dictionary.Set("type", type);
	PostMessage(dictionary);
}

void SkolarisInstance::post_message(int requestId, const std::string &type, const pp::Var &payload)
{
	pp::VarDictionary dictionary;
	dictionary.Set("reqId", requestId);
	dictionary.Set("type", type);
	dictionary.Set("payload", payload);
	PostMessage(dictionary);
}

void SkolarisInstance::post_error(int requestId, const std::string &what)
{
	post_message(requestId, "error", pp::Var(what));
}

void SkolarisInstance::post_text(const std::string &what)
{
	post_message("text", pp::Var(what));
}

void SkolarisInstance::post_version(int requestId)
{
	pp::VarDictionary dict;
	dict.Set("version", SKOLARIS_VERSION);
	dict.Set("major", SKOLARIS_VERSION_MAJOR);
	dict.Set("minor", SKOLARIS_VERSION_MINOR);
	dict.Set("patch", SKOLARIS_VERSION_PATCH);
	post_message(requestId, "version", dict);
}


void SkolarisInstance::post_currentsolution(int requestId)
{
	string s;
	if (stringifySolution(s, store()->getCurrentSolution()))
		post_message(requestId, "currentsolution", pp::Var(s));
	else
		post_error(requestId, s);
}

void SkolarisInstance::post_bestsolution(int requestId)
{
	string s;
	if (stringifySolution(s, store()->getBestSolution()))
		post_message(requestId, "bestsolution", pp::Var(s));
	else
		post_error(requestId, s);
}

void SkolarisInstance::post_feasiblesolution(int requestId)
{
	string s;
	if (stringifySolution(s, store()->getFeasibleSolution()))
		post_message(requestId, "feasiblesolution", pp::Var(s));
	else
		post_error(requestId, s);
}

void SkolarisInstance::post_bestoverallsolution(int requestId)
{
    string s;
    if (stringifySolution(s, store()->getBestOverallSolution()))
        post_message(requestId, "bestoverallsolution", pp::Var(s));
    else
        post_error(requestId, s);
}

void SkolarisInstance::post_feasibleoverallsolution(int requestId)
{
    string s;
    if (stringifySolution(s, store()->getFeasibleOverallSolution()))
        post_message(requestId, "feasibleoverallsolution", pp::Var(s));
    else
        post_error(requestId, s);
}

void SkolarisInstance::post_currentsolutionchanged(Algorithm::ISolution *solutionPtr)
{
	string s;
	if (stringifyFitnessSummary(s, solutionPtr))
		post_message("currentsolutionchanged", pp::Var(s));
	else
		post_error(0, s);
}

void SkolarisInstance::post_bestsolutionfound(Algorithm::ISolution *solutionPtr)
{
	string s;
	if (stringifyFitnessSummary(s, solutionPtr))
		post_message("bestsolutionfound", pp::Var(s));
	else
		post_error(0, s);
}

void SkolarisInstance::post_feasiblesolutionfound(Algorithm::ISolution *solutionPtr)
{
	string s;
	if (stringifyFitnessSummary(s, solutionPtr))
		post_message("feasiblesolutionfound", pp::Var(s));
	else
		post_error(0, s);
}


void SkolarisInstance::post_messages()
{
	post_message("messages", pp::Var(stringifyMessages()));
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
	pp::VarDictionary timeDict;
	timeDict.Set("h", h);
	timeDict.Set("m", m);
	timeDict.Set("s", s);
	timeDict.Set("ms", ms);
	post_message("stopped", timeDict);
}
