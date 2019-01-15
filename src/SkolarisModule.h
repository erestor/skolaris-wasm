#include <memory>

class SkolarisInstance;

class SkolarisModule {

  public:

	SkolarisModule();
	virtual ~SkolarisModule() {}

	std::unique_ptr<SkolarisInstance> CreateInstance();
};
