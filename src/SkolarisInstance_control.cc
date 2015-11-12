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
	if (Start())
		post_started(requestId);
	else
		post_error(requestId, "Unable to start search");
}

void SkolarisInstance::pause()
{
	Controller()->PauseAsync();
}

void SkolarisInstance::resume()
{
	Controller()->Resume();
}

void SkolarisInstance::stop()
{
	Controller()->StopAsync();
}
