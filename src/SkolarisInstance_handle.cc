#include "SkolarisInstance.h"
#include <string>

using namespace std;

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
		post_error(0, "unknown message");
}

void SkolarisInstance::handleGetMessage(const pp::VarDictionary &dict)
{
	auto target = dict.Get(pp::Var("target"));
	auto t = target.AsString();
	auto requestId = dict.Get(pp::Var("reqId")).AsInt();
	if (t == "version")
		post_version(requestId);
	else if (t == "currentsolution")
		post_currentsolution(requestId);
	else if (t == "bestsolution")
		post_bestsolution(requestId);
	else if (t == "feasiblesolution")
		post_feasiblesolution(requestId);
}

void SkolarisInstance::handleSetMessage(const pp::VarDictionary &dict)
{
	auto target = dict.Get(pp::Var("target"));
	auto payload = dict.Get(pp::Var("payload"));
	auto t = target.AsString();
	auto p = payload.AsString();
	auto requestId = dict.Get(pp::Var("reqId")).AsInt();
	if (t == "jsonData")
		set_jsonData(p);
	else if (t == "jsonSchedules")
		set_jsonSchedules(p, requestId);
	else if (t == "jsonConstraints")
		set_jsonConstraints(p, requestId);
	else if (t == "algorithm")
		set_algorithm(payload);

	post_complete(requestId);
}

void SkolarisInstance::handleControlMessage(const pp::VarDictionary &dict)
{
	auto target = dict.Get(pp::Var("target"));
	auto t = target.AsString();
	auto requestId = dict.Get(pp::Var("reqId")).AsInt();
	if (t == "start")
		start(requestId);
	else if (t == "pause")
		pause();
	else if (t == "resume")
		resume();
	else if (t == "stop")
		stop();
}

