#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"

/// The Module class.  The browser calls the CreateInstance() method to create
/// an instance of your NaCl module on the web page.  The browser creates a new
/// instance for each <embed> tag with type="application/x-pnacl".
class SkolarisModule : public pp::Module {
 public:
  SkolarisModule();
  virtual ~SkolarisModule() {}

  /// Create and return a SkolarisInstance object.
  /// @param[in] instance The browser-side instance.
  /// @return the plugin-side instance.
  virtual pp::Instance* CreateInstance(PP_Instance instance);
};
