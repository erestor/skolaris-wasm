#ifndef _plugin_controller_builder_included_
#define _plugin_controller_builder_included_

#include <boost/property_tree/ptree_fwd.hpp>
#include <exception>
#include <memory>
#include <string>
#include <vector>

class IController;

namespace Storage { class Store; }
namespace Timetabling { class ConstraintHolder; }

class PluginControllerBuilder {

  public:

	PluginControllerBuilder(
		const std::string &jsonData,
		int maxTime,
		std::vector<std::string> &errors
	);

	std::unique_ptr<IController> BuildController(
		std::shared_ptr<Storage::Store> &,
		std::shared_ptr<Timetabling::ConstraintHolder> &,
		std::vector<boost::property_tree::ptree> &checkFails
	);

  private:

	void LogException(const std::exception &, const std::string &place);

	std::string jsonData_;
	int maxTime_;
	std::vector<std::string> &errors_;
};

#endif
