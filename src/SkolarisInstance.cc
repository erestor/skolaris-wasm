#include "SkolarisInstance.h"
#include "event_handler.h"
#include "algorithm_builder.h"
#include "gascheduler/src/timetable/constraint_holder.h"
#include "gascheduler/src/interface/icontroller.h"
#include "gascheduler/src/storage/store.h"
#include <algorithm/isolution.h>
#include <ctoolhu/random/engine.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <sstream>
#include <string>

using namespace std;
using namespace boost::property_tree;

/// The Instance class.  One of these exists for each instance of your NaCl
/// module on the web page.  The browser will ask the Module object to create
/// a new Instance for each occurrence of the <embed> tag that has these
/// attributes:
///     src="hello_tutorial.nmf"
///     type="application/x-pnacl"
/// To communicate with the browser, you must override HandleMessage() to
/// receive messages from the browser, and use PostMessage() to send messages
/// back to the browser.  Note that this interface is asynchronous.
  
/// The constructor creates the plugin-side instance.
  /// @param[in] instance the handle to the browser-side plugin instance.
SkolarisInstance::SkolarisInstance(PP_Instance instance)
:
	pp::Instance(instance)
{
	m_EventHandler = make_unique<PluginEventHandler>(this);
}

SkolarisInstance::~SkolarisInstance()
{
}

bool SkolarisInstance::StringifySolution(string &result, const shared_ptr<Algorithm::ISolution> &solution)
{
	if (!solution) {
		result = "Unable to stringify solution: input is null";
		return false;
	}
	try {
		ptree data;
		solution->Save(data);
		stringstream s;
		json_parser::write_json(s, data);
		result = s.str();
		return true;
	}
	catch (exception &e) {
		result = "Unable to stringify solution: " + e.what();
	}
	catch(...) {
		result = "Unable to stringify solution: unknown exception";
	}
	return false;
}

string SkolarisInstance::StringifyMessages() const
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
IController *SkolarisInstance::Controller()
{
	if (!m_Controller) {
		post_error(0, "Skolaris plugin controller has not been initialized. Open the errors/warnings console for details.");
		return nullptr;
	}
	return m_Controller.get();
}

Ctoolhu::Thread::LockingProxy<Storage::Store> SkolarisInstance::Store() const
{
	return Storage::LockStore(m_Store.get());
}

bool SkolarisInstance::Start()
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
		return Controller()->Start(move(algorithm));
	}
	catch(...) {
		return false;
	}
}
