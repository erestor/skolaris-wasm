#include "SkolarisInstance.h"
#include "gascheduler/src/interface/icontroller.h"
#include "gascheduler/src/storage/store.h"
#include <string>

using namespace std;

const string SKOLARIS_VERSION("1.5.1" //keep in sync with 'currentVersion' in SkolarisUI/SkolarisUI.Web/App/config/plugin.js
#ifdef DEBUG
	"debug"
#endif
);

const string SKOLARIS_VERSION_MAJOR("1");
const string SKOLARIS_VERSION_MINOR("5");
const string SKOLARIS_VERSION_PATCH("1");

/// Handler for messages coming in from the browser via postMessage().  The
/// @a var_message can contain be any pp:Var type; for example int, string
/// Array or Dictinary. Please see the pp:Var documentation for more details.
/// @param[in] var_message The message posted by the browser.
void SkolarisInstance::HandleMessage(const pp::Var &var_message)
{
	if (!var_message.is_dictionary())
		return;

	pp::VarDictionary dictionary(var_message);
	auto type = dictionary.Get(pp::Var("type")).AsString();
	if (type == "get")
		handleGetMessage(dictionary);
	else if (type == "set")
		handleSetMessage(dictionary);
	else if (type == "control")
		handleControlMessage(dictionary);
	else
		post_error("unknown message");
}

void SkolarisInstance::handleSetMessage(const pp::VarDictionary &dict)
{
	auto target = dict.Get(pp::Var("target"));
	auto payload = dict.Get(pp::Var("payload"));
	auto t = target.AsString();
	auto p = payload.AsString();
	if (t == "jsonData")
		set_jsonData(p);
	else if (t == "jsonSchedules")
		set_jsonSchedules(p);
	else if (t == "jsonConstraints")
		set_jsonConstraints(p);
	else if (t == "jsonAlgorithm")
		set_jsonAlgorithm(p);
	else if (t == "benchmarkMode")
		set_benchmarkMode(payload.AsBool());
}

void SkolarisInstance::handleGetMessage(const pp::VarDictionary &dict)
{
	auto target = dict.Get(pp::Var("target"));
	auto t = target.AsString();
	if (t == "version")
		post_version();
	else if (t == "majorversion")
		post_majorversion();
	else if (t == "minorversion")
		post_minorversion();
	else if (t == "patchversion")
		post_patchversion();
	else if (t == "currentsolution")
		post_currentsolution();
	else if (t == "bestsolution")
		post_bestsolution();
	else if (t == "feasiblesolution")
		post_feasiblesolution();
	else if (t == "messages")
		post_messages();
	else if (t == "isRunning")
		post_isrunning();
}

void SkolarisInstance::handleControlMessage(const pp::VarDictionary &dict)
{
	auto target = dict.Get(pp::Var("target"));
	auto t = target.AsString();
	if (t == "start")
		start();
	else if (t == "pause")
		pause();
	else if (t == "resume")
		resume();
	else if (t == "stop")
		stop();
}

void SkolarisInstance::post_message(const std::string &type)
{
	pp::VarDictionary dictionary;
	dictionary.Set(pp::Var("type"), type);
	PostMessage(dictionary);
}

void SkolarisInstance::post_message(const std::string &type, const pp::Var &payload)
{
	pp::VarDictionary dictionary;
	dictionary.Set(pp::Var("type"), type);
	dictionary.Set(pp::Var("payload"), payload);
	PostMessage(dictionary);
}

void SkolarisInstance::post_error(const std::string &what)
{
	post_message("error", pp::Var(what));
}

void SkolarisInstance::post_text(const std::string &what)
{
	post_message("text", pp::Var(what));
}


void SkolarisInstance::post_version()
{
    post_message("version", pp::Var(SKOLARIS_VERSION));
}

void SkolarisInstance::post_majorversion()
{
    post_message("majorversion", pp::Var(SKOLARIS_VERSION_MAJOR));
}

void SkolarisInstance::post_minorversion()
{
    post_message("minorversion", pp::Var(SKOLARIS_VERSION_MINOR));
}

void SkolarisInstance::post_patchversion()
{
    post_message("patchversion", pp::Var(SKOLARIS_VERSION_PATCH));
}


void SkolarisInstance::post_currentsolution()
{
	post_message("currentsolution", pp::Var(StringifySolution(Store()->GetCurrentSolution())));
}

void SkolarisInstance::post_bestsolution()
{
	post_message("bestsolution", pp::Var(StringifySolution(Store()->GetBestSolution())));
}

void SkolarisInstance::post_feasiblesolution()
{
	post_message("feasiblesolution", pp::Var(StringifySolution(Store()->GetFeasibleSolution())));
}

void SkolarisInstance::post_bestsolutionfound()
{
	post_message("bestsolutionfound", pp::Var(StringifySolution(Store()->GetBestSolution())));
}

void SkolarisInstance::post_feasiblesolutionfound()
{
	post_message("feasiblesolutionfound", pp::Var(StringifySolution(Store()->GetFeasibleSolution())));
}


void SkolarisInstance::post_messages()
{
	post_message("messages", pp::Var(StringifyMessages()));
}


void SkolarisInstance::post_started()
{
	post_message("started");
}

void SkolarisInstance::post_isrunning()
{
	post_message("isrunning", Controller()->IsRunning());
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
	timeDict.Set(pp::Var("h"), h);
	timeDict.Set(pp::Var("m"), m);
	timeDict.Set(pp::Var("s"), s);
	timeDict.Set(pp::Var("ms"), ms);
	timeDict.Set(pp::Var("formatted"), formatted);
	post_message("stopped", timeDict);
}


void SkolarisInstance::set_jsonData(const string &val)
{
    m_jsonData = val;
	LoadData(m_jsonData);
}

void SkolarisInstance::set_jsonSchedules(const string &val)
{
    m_jsonSchedules = val;
	LoadSchedules(m_jsonSchedules);
}

void SkolarisInstance::set_jsonConstraints(const string &val)
{
    m_jsonConstraints = val;
	LoadConstraints(m_jsonConstraints);
}

void SkolarisInstance::set_jsonAlgorithm(const string &val)
{
	m_jsonAlgorithm = val;
}

void SkolarisInstance::set_benchmarkMode(bool val)
{
    m_benchmarkMode = val;
}

//algorithm control
void SkolarisInstance::start()
{
	Start(m_jsonAlgorithm, m_benchmarkMode);
}

void SkolarisInstance::pause()
{
	Controller()->Pause();
}

void SkolarisInstance::resume()
{
	Controller()->Resume();
}

void SkolarisInstance::stop()
{
	Controller()->Stop();
}
