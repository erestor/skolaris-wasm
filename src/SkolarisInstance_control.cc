#include "SkolarisInstance.h"
#include "gascheduler/src/plugin/algorithm_builder.h"
#include <ctoolhu/random/engine.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>

using namespace std;
using namespace boost::property_tree;

//algorithm control

static void initRand(bool benchmarkMode)
{
	auto &rand = Ctoolhu::Random::Private::SingleRandomEngine::Instance();
	if (benchmarkMode)
		rand.seed(0);
	else {
		random_device rd;
		rand.seed(rd());
	}
}

void SkolarisInstance::start()
{
	if (controller()->isRunning())
		post_message("error", "SkolarisInstance::start: Unable to start search, because it is already running");

	initRand(_benchmarkMode);
	controller()->start(PluginAlgorithmBuilder::BuildAlgorithm(_jsonAlgorithm));
		//this blocks the current thread until the algorithm finishes by itself
}

void SkolarisInstance::startAsync(int requestId)
{
	if (controller()->isRunning())
		post_error(requestId, "SkolarisInstance::startAsync: Unable to start search, because it is already running");

	try {
		initRand(_benchmarkMode);
		controller()->startAsync(PluginAlgorithmBuilder::BuildAlgorithm(_jsonAlgorithm));
		post_started(requestId);
	}
	catch (const exception &e) {
		post_error(requestId, "SkolarisInstance::startAsync: Unable to start search: "s + e.what());
	}
}

void SkolarisInstance::pauseAsync()
{
	controller()->pauseAsync();
}

void SkolarisInstance::resumeAsync()
{
	controller()->resumeAsync();
}

void SkolarisInstance::stopAsync()
{
	controller()->stopAsync();
}
