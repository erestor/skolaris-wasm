#include "SkolarisInstance.h"
#include "event_handler.h"
#include "gascheduler/src/interface/icontroller.h"
#include "gascheduler/src/plugin/algorithm_builder.h"
#include "gascheduler/src/storage/store.h"
#include <algorithm/isolution.h>
#include <ctoolhu/random/engine.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;
using namespace boost::property_tree;

/// The Instance class.  One of these exists for each instance of your NaCl
/// module on the web page.  The browser will ask the Module object to create
/// a new Instance for each occurrence of the <embed> tag that has the needed
/// attributes.
SkolarisInstance::SkolarisInstance(PP_Instance instance)
:
	pp::Instance(instance)
{
	m_EventHandler = make_unique<PluginEventHandler>(this);
}

SkolarisInstance::~SkolarisInstance()
{
}

bool SkolarisInstance::stringifyFitnessSummary(string &result, Algorithm::ISolution *solutionPtr) const
{
	if (!solutionPtr) {
		result = "Unable to stringify fitness summary: input is null";
		return false;
	}
	try {
		ptree data;
		data.put("fitness", solutionPtr->getFitness());
		data.put("feasible", solutionPtr->isFeasible());
		stringstream s;
		json_parser::write_json(s, data);
		result = s.str();
		return true;
	}
	catch (const exception &e) {
		result = string("Unable to stringify fitness summary: ") + e.what();
	}
	return false;
}

bool SkolarisInstance::stringifySolution(string &result, const shared_ptr<Algorithm::ISolution> &solutionPtr) const
{
	if (!solutionPtr) {
		result = "Unable to stringify solution: input is null";
		return false;
	}
	try {
		ptree data;
		solutionPtr->save(data);
		stringstream s;
		json_parser::write_json(s, data);
		result = s.str();
		return true;
	}
	catch (const exception &e) {
		result = string("Unable to stringify solution: ") + e.what();
	}
	return false;
}

string SkolarisInstance::stringifyMessages() const
{
	ptree errors;
	for (auto const &e : m_Errors) {
		ptree child;
		child.put("", e);
		errors.push_back(std::make_pair("", child));
	}
	ptree checkFails;
	for (auto const &cf : m_CheckFails) {
		checkFails.push_back(std::make_pair("", cf));
	}
	ptree messages;
	messages.add_child("errors", errors);
	messages.add_child("checkFails", checkFails);
	stringstream s;
	json_parser::write_json(s, messages);
	return s.str();
}

//====================================================================================================================
IController *SkolarisInstance::controller()
{
	if (!m_Controller) {
		post_error(0, "Skolaris plugin controller has not been initialized. Open the errors/warnings console for details.");
		return nullptr;
	}
	return m_Controller.get();
}

Ctoolhu::Thread::LockingProxy<Storage::Store> SkolarisInstance::store() const
{
	return Storage::lockStore(m_Store.get());
}

bool SkolarisInstance::start()
{
	try {
		auto &engine = Ctoolhu::Random::Private::SingleRandomEngine::Instance();
		if (m_benchmarkMode)
			engine.seed(0);
		else {
			random_device rd;
			engine.seed(rd());
		}
		auto algorithm = PluginAlgorithmBuilder::BuildAlgorithm(m_jsonAlgorithm);
		return controller()->startAsync(move(algorithm));
	}
	catch(...) {
		return false;
	}
}
