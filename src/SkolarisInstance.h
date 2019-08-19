#include "event_handler.h"
#include <algorithm/isolution.h>
#include <ctoolhu/thread/proxy.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <loki/Singleton.h>
#include <memory>
#include <string>
#include <vector>

namespace Storage { class Store; }
namespace Timetabling { class ConstraintHolder; }

class IController;
class PluginEventHandler;

class SkolarisInstance {

	typedef std::unique_ptr<IController> controller_ptr_type;

  public:

	SkolarisInstance();
	virtual ~SkolarisInstance(); 

	//handler for messages coming in from the browser
	void handleMessage(const std::string &);

	void post_text(const std::string &) const;
    void post_paused() const;
    void post_resumed() const;
	void post_stopped(int h, int m, int s, int ms) const;
    void post_currentsolutionchanged(Algorithm::ISolution *) const;
    void post_bestsolutionfound(Algorithm::ISolution *) const;
    void post_feasiblesolutionfound(Algorithm::ISolution *) const;

  private:

	//posts message to the browser
	void postMessage(const char *) const;
	void postMessage(const boost::property_tree::ptree &) const;

	void handleSetMessage(const boost::property_tree::ptree &);
	void handleGetMessage(const boost::property_tree::ptree &);
	void handleControlMessage(const boost::property_tree::ptree &);

	void post_message(const std::string &type) const;
	void post_message(const std::string &type, const std::string &payload) const;
	void post_message(const std::string &type, const boost::property_tree::ptree &) const;

	void post_message(int requestId, const std::string &type) const;
	void post_message(int requestId, const std::string &type, const std::string &payload) const;
	void post_message(int requestId, const std::string &type, const boost::property_tree::ptree &) const;

	void post_complete(int requestId) const;
	void post_error(int requestId, const std::string &what) const;
	void post_version(int requestId) const;
    void post_currentsolution(int requestId) const;
    void post_bestsolution(int requestId) const;
    void post_feasiblesolution(int requestId) const;
    void post_bestoverallsolution(int requestId) const;
    void post_feasibleoverallsolution(int requestId) const;
    void post_warnings();

	void post_started(int requestId) const;

    bool set_jsonData(const std::string &, int requestId);
    bool set_jsonSchedules(const std::string &, int requestId);
    bool set_jsonConstraints(const std::string &, int requestId);
    void set_algorithm(const boost::property_tree::ptree &);

    //algorithm control
    void start(int requestId);
    void stop();
    void pause();
    void resume();

	bool stringifyFitnessSummary(std::string &result, Algorithm::ISolution *) const;
	bool stringifySolution(std::string &result, const std::shared_ptr<Algorithm::ISolution> &) const;
	std::string stringifyMessages(const std::vector<boost::property_tree::ptree> &warnings) const; //puts errors and check fails into JSON string
	IController *controller();
	Ctoolhu::Thread::LockingProxy<Storage::Store> store() const; //do NOT call this twice in one expression, it will cause an exception. It is an object auto-locking call!
	bool start();

	controller_ptr_type m_Controller;
	std::shared_ptr<Storage::Store> m_Store;
	std::shared_ptr<Timetabling::ConstraintHolder> m_ConstraintHolder;
	std::unique_ptr<PluginEventHandler> m_EventHandler;
	std::vector<std::string> m_Errors;

	std::string m_jsonAlgorithm;
	bool m_benchmarkMode;
};

typedef Loki::SingletonHolder<SkolarisInstance> SingleSkolarisInstance;
