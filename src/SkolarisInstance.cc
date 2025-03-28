#include "SkolarisInstance.h"
#include "event_handler.h"
#include "gascheduler/src/plugin/algorithm_builder.h"
#include <ctoolhu/random/engine.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace boost::property_tree;

SkolarisInstance::~SkolarisInstance() = default;

SkolarisInstance::SkolarisInstance()
{
	_eventHandler = std::make_unique<PluginEventHandler>(this);
}

bool SkolarisInstance::stringifyFitnessSummary(std::string &result, Algorithm::ISolution *solutionPtr) const
{
	if (!solutionPtr) {
		result = "Unable to stringify fitness summary: input is null";
		return false;
	}
	try {
		ptree data;
		data.put("fitness", solutionPtr->getFitness());
		data.put("feasible", solutionPtr->isFeasible());
		std::ostringstream s;
		json_parser::write_json(s, data);
		result = s.str();
		return true;
	}
	catch (const std::exception &e) {
		result = std::string{"Unable to stringify fitness summary: "} + e.what();
	}
	return false;
}

bool SkolarisInstance::stringifySolution(std::string &result, const Algorithm::ISolution *solutionPtr) const
{
	if (!solutionPtr) {
		result = "Unable to stringify solution: input is null";
		return false;
	}
	try {
		ptree data;
		solutionPtr->save(data);
		std::ostringstream s;
		json_parser::write_json(s, data);
		result = s.str();
		return true;
	}
	catch (const std::exception &e) {
		result = std::string{"Unable to stringify solution: "} + e.what();
	}
	return false;
}

std::string SkolarisInstance::stringifyMessages(const std::vector<boost::property_tree::ptree> &warnings) const
{
	ptree errors;
	for (auto const &e : _errors) {
		ptree child;
		child.put("", e);
		errors.push_back({"", child});
	}
	ptree checkFails;
	for (auto const &cf : warnings)
		checkFails.push_back({"", cf});

	ptree messages;
	messages.add_child("errors", errors);
	messages.add_child("checkFails", checkFails);
	std::ostringstream s;
	json_parser::write_json(s, messages);
	return s.str();
}

Control::Controller<Timetabling::Schedule> *SkolarisInstance::controller()
{
	if (!_controller) {
		post_error(0, "Skolaris plugin controller has not been initialized. Open the errors/warnings console for details.");
		return nullptr;
	}
	return _controller.get();
}

Algorithm::Storage::Store<Timetabling::Schedule>::locked_t SkolarisInstance::lockStore() const
{
	return Algorithm::Storage::lockStore(*_store);
}
