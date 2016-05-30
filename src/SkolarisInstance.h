#include "event_handler.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/var.h"
#include "ppapi/cpp/var_dictionary.h"
#include <algorithm/isolution.h>
#include <ctoolhu/thread/proxy.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <memory>
#include <string>
#include <vector>

namespace Storage { class Store; }
namespace Timetabling { class ConstraintHolderPT; }

class IController;
class PluginEventHandler;

/// The Instance class.  One of these exists for each instance of your NaCl
/// module on the web page.  The browser will ask the Module object to create
/// a new Instance for each occurrence of the <embed> tag that has these
/// attributes:
///     src="hello_tutorial.nmf"
///     type="application/x-pnacl"
/// To communicate with the browser, you must override HandleMessage() to
/// receive messages from the browser, and use PostMessage() to send messages
/// back to the browser.  Note that this interface is asynchronous.
class SkolarisInstance : public pp::Instance {

	typedef std::unique_ptr<IController> controller_ptr_type;

  public:

	/// The constructor creates the plugin-side instance.
	/// @param[in] instance the handle to the browser-side plugin instance.
	explicit SkolarisInstance(PP_Instance instance); 
	virtual ~SkolarisInstance(); 

	/// Handler for messages coming in from the browser via postMessage().  The
	/// @a var_message can contain be any pp:Var type; for example int, string
	/// Array or Dictinary. Please see the pp:Var documentation for more details.
	/// @param[in] var_message The message posted by the browser.
	virtual void HandleMessage(const pp::Var&);

	void post_text(const std::string &);
    void post_paused();
    void post_resumed();
	void post_stopped(int h, int m, int s, int ms, const std::string &formatted);
    void post_bestsolutionfound(Algorithm::Fitness fitness);
    void post_feasiblesolutionfound(Algorithm::Fitness fitness);

  private:

	void handleSetMessage(const pp::VarDictionary &);
	void handleGetMessage(const pp::VarDictionary &);
	void handleControlMessage(const pp::VarDictionary &);

	void post_message(const std::string &type);
	void post_message(const std::string &type, const pp::Var &payload);

	void post_message(int requestId, const std::string &type);
	void post_message(int requestId, const std::string &type, const pp::Var &payload);

	void post_complete(int requestId);
	void post_error(int requestId, const std::string &what);
	void post_version(int requestId);
    void post_currentsolution(int requestId);
    void post_bestsolution(int requestId);
    void post_feasiblesolution(int requestId);
    void post_messages();

	void post_started(int requestId);

    bool set_jsonData(const std::string &, int requestId);
    bool set_jsonSchedules(const std::string &, int requestId);
    bool set_jsonConstraints(const std::string &, int requestId);
    void set_algorithm(const pp::Var &);

    //algorithm control
    void start(int requestId);
    void stop();
    void pause();
    void resume();

	bool StringifySolution(std::string &result, const std::shared_ptr<Algorithm::ISolution> &);
	std::string StringifyMessages() const; //puts errors and check fails into JSON string
	IController *Controller();
	Ctoolhu::Thread::LockingProxy<Storage::Store> Store() const; //do NOT call this twice in one expression, it will cause an exception. It is an object auto-locking call!
	bool Start();

	controller_ptr_type m_Controller;
	std::shared_ptr<Storage::Store> m_Store;
	std::shared_ptr<Timetabling::ConstraintHolderPT> m_ConstraintHolder;
	std::unique_ptr<PluginEventHandler> m_EventHandler;
	std::vector<std::string> m_Errors;
	std::vector<boost::property_tree::ptree> m_CheckFails;

	std::string m_jsonAlgorithm;
	bool m_benchmarkMode;
};

