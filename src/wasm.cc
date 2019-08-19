//#include "SkolarisModule.h"
#include "SkolarisInstance.h"
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <emscripten/emscripten.h>
#include <memory>
#include <sstream>
#include <stdio.h>

using namespace boost::property_tree;

//SkolarisModule module;
//std::unique_ptr<SkolarisInstance> instancePtr;

int main(int argc, char **argv)
{
//	instancePtr = module.CreateInstance();
	EM_ASM(SkolarisModule.wasmLoaded(true));
}

void SkolarisInstance::postMessage(const ptree &data)
{
	std::stringstream s;
	json_parser::write_json(s, data);
	auto result = s.str();
	EM_ASM({
		postMessage(UTF8ToString($0));
	}, result.c_str());
}

#ifdef __cplusplus
extern "C" {
#endif

void EMSCRIPTEN_KEEPALIVE handleMessage(const char *msg)
{
	printf("handleMessage called with %s\n", msg);
	SingleSkolarisInstance::Instance().handleMessage(msg);
}

#ifdef __cplusplus
}
#endif
