#include "SkolarisModule.h"
#include "SkolarisInstance.h"

SkolarisModule::SkolarisModule()
{
}

std::unique_ptr<SkolarisInstance> SkolarisModule::CreateInstance()
{
	return std::make_unique<SkolarisInstance>();
}
