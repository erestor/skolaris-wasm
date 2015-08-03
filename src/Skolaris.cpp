/**********************************************************\

  Auto-generated Skolaris.cpp

  This file contains the auto-generated main plugin object
  implementation for the Skolaris project

\**********************************************************/

#include "Skolaris.h"
#include "SkolarisAPI.h"
#include "../GAScheduler/src/interface/icontroller.h" //used by unique_ptr destructor
#include "../GAScheduler/src/plugin/event_handler.h" //used by unique_ptr destructor
#include <boost/property_tree/ptree.hpp>

///////////////////////////////////////////////////////////////////////////////
/// @fn Skolaris::StaticInitialize()
///
/// @brief  Called from PluginFactory::globalPluginInitialize()
///
/// @see FB::FactoryBase::globalPluginInitialize
///////////////////////////////////////////////////////////////////////////////
void Skolaris::StaticInitialize()
{
    // Place one-time initialization stuff here; As of FireBreath 1.4 this should only
    // be called once per process
}

///////////////////////////////////////////////////////////////////////////////
/// @fn Skolaris::StaticInitialize()
///
/// @brief  Called from PluginFactory::globalPluginDeinitialize()
///
/// @see FB::FactoryBase::globalPluginDeinitialize
///////////////////////////////////////////////////////////////////////////////
void Skolaris::StaticDeinitialize()
{
    // Place one-time deinitialization stuff here. As of FireBreath 1.4 this should
    // always be called just before the plugin library is unloaded
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  Skolaris constructor.  Note that your API is not available
///         at this point, nor the window.  For best results wait to use
///         the JSAPI object until the onPluginReady method is called
///////////////////////////////////////////////////////////////////////////////
Skolaris::Skolaris()
{
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  Skolaris destructor.
///////////////////////////////////////////////////////////////////////////////
Skolaris::~Skolaris()
{
    // This is optional, but if you reset m_api (the shared_ptr to your JSAPI
    // root object) and tell the host to free the retained JSAPI objects then
    // unless you are holding another shared_ptr reference to your JSAPI object
    // they will be released here.
    releaseRootJSAPI();
    m_host->freeRetainedObjects();
}

void Skolaris::onPluginReady()
{
    // When this is called, the BrowserHost is attached, the JSAPI object is
    // created, and we are ready to interact with the page and such.  The
    // PluginWindow may or may not have already fire the AttachedEvent at
    // this point.
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  Creates an instance of the JSAPI object that provides your main
///         Javascript interface.
///
/// Note that m_host is your BrowserHost and shared_ptr returns a
/// FB::PluginCorePtr, which can be used to provide a
/// boost::weak_ptr<Skolaris> for your JSAPI class.
///
/// Be very careful where you hold a shared_ptr to your plugin class from,
/// as it could prevent your plugin class from getting destroyed properly.
///////////////////////////////////////////////////////////////////////////////
FB::JSAPIPtr Skolaris::createJSAPI()
{
    // m_host is the BrowserHost
    return boost::make_shared<SkolarisAPI>(FB::ptr_cast<Skolaris>(shared_from_this()), m_host);
}

bool Skolaris::onWindowAttached(FB::AttachedEvent *evt, FB::PluginWindow *)
{
    // The window is attached; act appropriately
    return false;
}

bool Skolaris::onWindowDetached(FB::DetachedEvent *evt, FB::PluginWindow *)
{
    // The window is about to be detached; act appropriately
    return false;
}
