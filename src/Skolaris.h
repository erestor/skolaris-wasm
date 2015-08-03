/**********************************************************\

  This file contains the auto-generated main plugin object
  implementation for the Skolaris project

\**********************************************************/
#ifndef H_SkolarisPLUGIN
#define H_SkolarisPLUGIN

#include "PluginWindow.h"
#include "PluginEvents/MouseEvents.h"
#include "PluginEvents/AttachedEvent.h"
#include "PluginCore.h"

#include <ctoolhu/thread/proxy.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <string>
#include <vector>

namespace Algorithm { class ISolution; }
namespace Storage { class Store; }
namespace Timetabling { class ConstraintHolder; }

class IController;
class PluginEventHandler;

class Skolaris : public FB::PluginCore {

  public:

    Skolaris();
    virtual ~Skolaris();

	static void StaticInitialize();
    static void StaticDeinitialize();

    void onPluginReady();
    virtual FB::JSAPIPtr createJSAPI();
    // If you want your plugin to always be windowless, set this to true
    // If you want your plugin to be optionally windowless based on the
    // value of the "windowless" param tag, remove this method or return
    // FB::PluginCore::isWindowless()
    virtual bool isWindowless() { return true; }

    BEGIN_PLUGIN_EVENT_MAP()
        EVENTTYPE_CASE(FB::AttachedEvent, onWindowAttached, FB::PluginWindow)
        EVENTTYPE_CASE(FB::DetachedEvent, onWindowDetached, FB::PluginWindow)
    END_PLUGIN_EVENT_MAP()

    /** BEGIN EVENTDEF -- DON'T CHANGE THIS LINE **/
    virtual bool onWindowAttached(FB::AttachedEvent *evt, FB::PluginWindow *);
    virtual bool onWindowDetached(FB::DetachedEvent *evt, FB::PluginWindow *);
    /** END EVENTDEF -- DON'T CHANGE THIS LINE **/

	//added stuff:

	typedef std::unique_ptr<IController> controller_ptr_type;

	static std::string StringifySolution(const std::shared_ptr<Algorithm::ISolution> &);
	std::string StringifyMessages() const; //puts errors and check fails into JSON string

	const controller_ptr_type &Controller();
	Ctoolhu::Thread::LockingProxy<Storage::Store> Store() const; //do NOT call this twice in one expression, it will cause an exception. It is an object auto-locking call!

	void LoadData(const std::string &);
	void LoadSchedules(const std::string &);
	void LoadConstraints(const std::string &);
	bool Start(const std::string &jsonAlgorithm, bool benchmark);

  private:

	controller_ptr_type m_Controller;
	std::shared_ptr<Storage::Store> m_Store;
	std::shared_ptr<Timetabling::ConstraintHolder> m_ConstraintHolder;
	std::unique_ptr<PluginEventHandler> m_EventHandler;

	std::vector<std::string> m_Errors;
	std::vector<boost::property_tree::ptree> m_CheckFails;
};

#endif //H_SkolarisPLUGIN
