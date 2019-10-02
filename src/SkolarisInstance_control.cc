#include "SkolarisInstance.h"
#include "gascheduler/src/plugin/algorithm_builder.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>

using namespace std;
using namespace boost::property_tree;

//algorithm control
void SkolarisInstance::start(int requestId)
{
	if (controller()->isRunning())
		post_error(requestId, "SkolarisInstance::start: Unable to start search, because it is already running");

	try {
		auto &rand = Ctoolhu::Random::Private::SingleRandomEngine::Instance();
		if (_benchmarkMode)
			rand.seed(0);
		else {
			random_device rd;
			rand.seed(rd());
		}
		controller()->startAsync(PluginAlgorithmBuilder::BuildAlgorithm(_jsonAlgorithm));
		post_started(requestId);
	}
	catch (const exception &e) {
		post_error(requestId, "SkolarisInstance::start: Unable to start search: "s + e.what());
	}
}

void SkolarisInstance::pause()
{
	controller()->pauseAsync();
}

void SkolarisInstance::resume()
{
	controller()->resumeAsync();
}

void SkolarisInstance::stop()
{
	controller()->stopAsync();
}
