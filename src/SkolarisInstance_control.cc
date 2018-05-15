#include "SkolarisInstance.h"
#include "gascheduler/src/interface/icontroller.h"
#include "gascheduler/src/storage/store.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <string>

using namespace std;
using namespace boost::property_tree;

//algorithm control
void SkolarisInstance::start(int requestId)
{
	if (start())
		post_started(requestId);
	else
		post_error(requestId, "Unable to start search");
}

void SkolarisInstance::pause()
{
	controller()->pauseAsync();
}

void SkolarisInstance::resume()
{
	controller()->resume();
}

void SkolarisInstance::stop()
{
	controller()->stopAsync();
}
