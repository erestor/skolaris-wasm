#include "SkolarisInstance.h"
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <string>
#include <stdio.h>

using namespace boost::property_tree;
using namespace std;

/// Handler for messages coming in from the browser via postMessage().  The
/// @a var_message can contain be any pp:Var type; for example int, string
/// Array or Dictinary. Please see the pp:Var documentation for more details.
/// @param[in] var_message The message posted by the browser.
void SkolarisInstance::handleMessage(const string &var_message)
{
	ptree message;
	stringstream s(var_message);
	json_parser::read_json(s, message);

	auto type = message.get<string>("type");
	auto target = message.get<string>("target");
	printf("SkolarisInstance::HandleMessage type = %s, target = %s\n", type.c_str(), target.c_str());
	if (type == "get")
		handleGetMessage(message);
	else if (type == "set")
		handleSetMessage(message);
	else if (type == "control")
		handleControlMessage(message);
	else
		post_error(0, "unknown message");
}

void SkolarisInstance::handleGetMessage(const ptree &msg)
{
	auto t = msg.get<string>("target");
	auto requestId = msg.get("reqId", 0);
	if (t == "version")
		post_version(requestId);
	else if (t == "currentsolution")
		post_currentsolution(requestId);
	else if (t == "bestsolution")
		post_bestsolution(requestId);
	else if (t == "feasiblesolution")
		post_feasiblesolution(requestId);
	else if (t == "bestoverallsolution")
		post_bestoverallsolution(requestId);
	else if (t == "feasibleoverallsolution")
		post_feasibleoverallsolution(requestId);
}

void SkolarisInstance::handleSetMessage(const ptree &msg)
{
	auto t = msg.get<string>("target");
	auto p = msg.get<string>("payload");
	auto requestId = msg.get("reqId", 0);
	if (t == "jsonData") {
		if (!set_jsonData(p, requestId))
			return;
	}
	else if (t == "jsonSchedules") {
		if (!set_jsonSchedules(p, requestId))
			return;
	}
	else if (t == "jsonConstraints") {
		if (!set_jsonConstraints(p, requestId))
			return;
	}
	else if (t == "algorithm") {
		ptree payload;
		stringstream s(p);
		json_parser::read_json(s, payload);
		set_algorithm(payload);
	}
	post_complete(requestId);
}

void SkolarisInstance::handleControlMessage(const ptree &msg)
{
	auto t = msg.get<string>("target");
	auto requestId = msg.get("reqId", 0);
	if (t == "start")
		start(requestId);
	else if (t == "pause")
		pause();
	else if (t == "resume")
		resume();
	else if (t == "stop")
		stop();
}
