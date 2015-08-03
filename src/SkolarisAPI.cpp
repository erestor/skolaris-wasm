/**********************************************************\

  Auto-generated SkolarisAPI.cpp

\**********************************************************/
#include "SkolarisAPI.h"
#include "Skolaris.h"
#include "../GAScheduler/src/interface/icontroller.h"
#include "../GAScheduler/src/storage/store.h"
#include "../../../firebreath-build/projects/Skolaris/gen/global/config.h"

using namespace std;

const string SKOLARIS_VERSION(FBSTRING_PLUGIN_VERSION //keep in sync with 'currentVersion' in SkolarisUI/SkolarisUI.Web/App/config/plugin.js
#ifdef DEBUG
	"debug"
#endif
);

///////////////////////////////////////////////////////////////////////////////
/// @fn SkolarisAPI::SkolarisAPI(const SkolarisPtr& plugin, const FB::BrowserHostPtr host)
///
/// @brief  Constructor for your JSAPI object.  You should register your methods, properties, and events
///         that should be accessible to Javascript from here.
///
/// @see FB::JSAPIAuto::registerMethod
/// @see FB::JSAPIAuto::registerProperty
/// @see FB::JSAPIAuto::registerEvent
///////////////////////////////////////////////////////////////////////////////
SkolarisAPI::SkolarisAPI(const SkolarisPtr& plugin, const FB::BrowserHostPtr& host)
:
	m_plugin(plugin),
	m_host(host),
	m_benchmarkMode{false}
{
	registerMethod("startSearch",		make_method(this, &SkolarisAPI::startSearch));
	registerMethod("pauseSearch",		make_method(this, &SkolarisAPI::pauseSearch));
	registerMethod("resumeSearch",		make_method(this, &SkolarisAPI::resumeSearch));
	registerMethod("stopSearch",		make_method(this, &SkolarisAPI::stopSearch));
	registerMethod("isSearchRunning",	make_method(this, &SkolarisAPI::isSearchRunning));

    registerProperty("jsonData", make_property(this,
                        &SkolarisAPI::get_jsonData,
                        &SkolarisAPI::set_jsonData));

	registerProperty("jsonSchedules", make_property(this,
                        &SkolarisAPI::get_jsonSchedules,
                        &SkolarisAPI::set_jsonSchedules));

	registerProperty("jsonConstraints", make_property(this,
                        &SkolarisAPI::get_jsonConstraints,
                        &SkolarisAPI::set_jsonConstraints));
	
	registerProperty("jsonAlgorithm", make_property(this,
                        &SkolarisAPI::get_jsonAlgorithm,
                        &SkolarisAPI::set_jsonAlgorithm));

	registerProperty("benchmarkMode", make_property(this,
                        &SkolarisAPI::get_benchmarkMode,
                        &SkolarisAPI::set_benchmarkMode));

    // Read-only properties
    registerProperty("version",			make_property(this, &SkolarisAPI::get_version));
    registerProperty("majorversion",	make_property(this, &SkolarisAPI::get_majorversion));
    registerProperty("minorversion",	make_property(this, &SkolarisAPI::get_minorversion));
    registerProperty("patchversion",	make_property(this, &SkolarisAPI::get_patchversion));

    registerProperty("currentSolution",	make_property(this, &SkolarisAPI::get_currentSolution));
    registerProperty("bestSolution",	make_property(this, &SkolarisAPI::get_bestSolution));
    registerProperty("feasibleSolution", make_property(this, &SkolarisAPI::get_feasibleSolution));
    registerProperty("messages",		make_property(this, &SkolarisAPI::get_messages));
}

///////////////////////////////////////////////////////////////////////////////
/// @fn SkolarisAPI::~SkolarisAPI()
///
/// @brief  Destructor.  Remember that this object will not be released until
///         the browser is done with it; this will almost definitely be after
///         the plugin is released.
///////////////////////////////////////////////////////////////////////////////
SkolarisAPI::~SkolarisAPI()
{
}

///////////////////////////////////////////////////////////////////////////////
/// @fn SkolarisPtr SkolarisAPI::getPlugin()
///
/// @brief  Gets a reference to the plugin that was passed in when the object
///         was created.  If the plugin has already been released then this
///         will throw a FB::script_error that will be translated into a
///         javascript exception in the page.
///////////////////////////////////////////////////////////////////////////////
SkolarisPtr SkolarisAPI::getPlugin()
{
    SkolarisPtr plugin(m_plugin.lock());
    if (!plugin)
        throw FB::script_error("The plugin is invalid");

    return plugin;
}

const string &SkolarisAPI::get_jsonData() const
{
    return m_jsonData;
}
void SkolarisAPI::set_jsonData(const string &val)
{
    m_jsonData = val;
	getPlugin()->LoadData(m_jsonData);
}

const string &SkolarisAPI::get_jsonSchedules() const
{
    return m_jsonSchedules;
}
void SkolarisAPI::set_jsonSchedules(const string &val)
{
    m_jsonSchedules = val;
	getPlugin()->LoadSchedules(m_jsonSchedules);
}

const string &SkolarisAPI::get_jsonConstraints() const
{
    return m_jsonConstraints;
}
void SkolarisAPI::set_jsonConstraints(const string &val)
{
    m_jsonConstraints = val;
	getPlugin()->LoadConstraints(m_jsonConstraints);
}

const std::string &SkolarisAPI::get_jsonAlgorithm() const
{
	return m_jsonAlgorithm;
}
void SkolarisAPI::set_jsonAlgorithm(const string &val)
{
	m_jsonAlgorithm = val;
}

bool SkolarisAPI::get_benchmarkMode() const
{
    return m_benchmarkMode;
}
void SkolarisAPI::set_benchmarkMode(bool val)
{
    m_benchmarkMode = val;
}

// Read-only property version
const string &SkolarisAPI::get_version() const
{
    return SKOLARIS_VERSION;
}
int SkolarisAPI::get_majorversion() const
{
    return FBSTRING_VERSION_MAJOR;
}
int SkolarisAPI::get_minorversion() const
{
    return FBSTRING_VERSION_MINOR;
}
int SkolarisAPI::get_patchversion() const
{
    return FBSTRING_VERSION_PATCH;
}

string SkolarisAPI::get_currentSolution()
{
	return Skolaris::StringifySolution(getPlugin()->Store()->GetCurrentSolution());
}

string SkolarisAPI::get_bestSolution()
{
	return Skolaris::StringifySolution(getPlugin()->Store()->GetBestSolution());
}

string SkolarisAPI::get_feasibleSolution()
{
	return Skolaris::StringifySolution(getPlugin()->Store()->GetFeasibleSolution());
}

string SkolarisAPI::get_messages()
{
	return getPlugin()->StringifyMessages();
}

//algorithm control
bool SkolarisAPI::startSearch()
{
	return getPlugin()->Start(m_jsonAlgorithm, m_benchmarkMode);
}

void SkolarisAPI::pauseSearch()
{
	getPlugin()->Controller()->Pause();
}

void SkolarisAPI::resumeSearch()
{
	getPlugin()->Controller()->Resume();
}

void SkolarisAPI::stopSearch()
{
	getPlugin()->Controller()->Stop();
}

bool SkolarisAPI::isSearchRunning()
{
	return getPlugin()->Controller()->IsRunning();
}
