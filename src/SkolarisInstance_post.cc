#include "SkolarisInstance.h"
#include "gascheduler/src/storage/store.h"
#include <string>

using namespace std;

const string SKOLARIS_VERSION("2.7.2"
#ifdef DEBUG
	"debug"
#endif
);

const string SKOLARIS_VERSION_MAJOR("2");
const string SKOLARIS_VERSION_MINOR("7");
const string SKOLARIS_VERSION_PATCH("2");

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
//	post_text("Getting current solution\n");
	if (StringifySolution(s, Store()->GetCurrentSolution()))
		post_message(requestId, "currentsolution", pp::Var(s));
	else
		post_error(requestId, s);

//	post_text("Finished getting current solution\n");
}

void SkolarisInstance::post_bestsolution(int requestId)
{
	string s;
	if (StringifySolution(s, Store()->GetBestSolution()))
		post_message(requestId, "bestsolution", pp::Var(s));
	else
		post_error(requestId, s);
}

void SkolarisInstance::post_feasiblesolution(int requestId)
{
	string s;
	if (StringifySolution(s, Store()->GetFeasibleSolution()))
		post_message(requestId, "feasiblesolution", pp::Var(s));
	else
		post_error(requestId, s);
}

void SkolarisInstance::post_bestsolutionfound(Algorithm::Fitness fitness)
{
	post_message("bestsolutionfound", pp::Var(static_cast<int>(fitness)));
}

void SkolarisInstance::post_feasiblesolutionfound(Algorithm::Fitness fitness)
{
	post_message("feasiblesolutionfound", pp::Var(static_cast<int>(fitness)));
}


void SkolarisInstance::post_messages()
{
	post_message("messages", pp::Var(StringifyMessages()));
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

void SkolarisInstance::post_stopped(int h, int m, int s, int ms, const std::string &formatted)
{
	pp::VarDictionary timeDict;
	timeDict.Set("h", h);
	timeDict.Set("m", m);
	timeDict.Set("s", s);
	timeDict.Set("ms", ms);
	timeDict.Set("formatted", formatted);
	post_message("stopped", timeDict);
}
