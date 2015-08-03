#include <algorithm/ialgorithm.h>
#include <memory>
#include <string>

class PluginAlgorithmBuilder {

  public:

	static std::unique_ptr<Algorithm::IAlgorithm> BuildAlgorithm(const std::string &jsonDefinition);

};
