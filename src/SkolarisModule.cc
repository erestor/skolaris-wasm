#include "SkolarisModule.h"
#include "SkolarisInstance.h"

SkolarisModule::SkolarisModule()
:
	pp::Module()
{
}

/// Create and return a SkolarisInstance object.
/// @param[in] instance The browser-side instance.
/// @return the plugin-side instance.
pp::Instance* SkolarisModule::CreateInstance(PP_Instance instance)
{
	return new SkolarisInstance(instance);
}

namespace pp {
/// Factory function called by the browser when the module is first loaded.
/// The browser keeps a singleton of this module.  It calls the
/// CreateInstance() method on the object you return to make instances.  There
/// is one instance per <embed> tag on the page.  This is the main binding
/// point for your NaCl module with the browser.
Module* CreateModule()
{
	return new SkolarisModule();
}

}  // namespace pp
