#include "event_handler.h"
#include "gascheduler/src/controller.hpp"
#include "gascheduler/src/timetable/schedule.h"
#include <localsearch/store.hpp>
#include <localsearch/interface/isolution.h>
#include <ctoolhu/singleton/holder.hpp>
#include <ctoolhu/thread/proxy.hpp>
#include <boost/property_tree/ptree_fwd.hpp>
#include <memory>
#include <string>
#include <vector>

namespace Timetabling {
	class ConstraintHolder;
}

class PluginEventHandler;

class SkolarisInstance {

  public:

	SkolarisInstance();
	~SkolarisInstance(); //necessary because of the unique ptrs in the data

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
	bool stringifySolution(std::string &result, const Algorithm::ISolution *) const;
	std::string stringifyMessages(const std::vector<boost::property_tree::ptree> &warnings) const;
		//puts errors and check fails into JSON string

	Controller<Timetabling::Schedule> *controller();

	Algorithm::Storage::Store<Timetabling::Schedule>::locked_t lockStore() const;
		//do NOT call this twice in one expression, it will cause an exception. It is an object auto-locking call!

	std::unique_ptr<Controller<Timetabling::Schedule>> _controller;
	std::shared_ptr<Algorithm::Storage::Store<Timetabling::Schedule>> _store;
	std::shared_ptr<Timetabling::ConstraintHolder> _constraintHolder;
	std::shared_ptr<Timetabling::Timetable> _timetable;
	std::unique_ptr<PluginEventHandler> _eventHandler;
	std::vector<std::string> _errors;

	std::string _jsonAlgorithm;
	bool _benchmarkMode;
};

using SingleSkolarisInstance = Ctoolhu::Singleton::Holder<SkolarisInstance>;
