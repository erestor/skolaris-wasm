#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/var.h"

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
 public:
  /// The constructor creates the plugin-side instance.
  /// @param[in] instance the handle to the browser-side plugin instance.
  explicit SkolarisInstance(PP_Instance instance); 
  virtual ~SkolarisInstance() {}

  /// Handler for messages coming in from the browser via postMessage().  The
  /// @a var_message can contain be any pp:Var type; for example int, string
  /// Array or Dictinary. Please see the pp:Var documentation for more details.
  /// @param[in] var_message The message posted by the browser.
  virtual void HandleMessage(const pp::Var&);

    void fire_bestsolutionfound();
    void fire_feasiblesolutionfound();
    void fire_searchfinished(int, int, int, int, const std::string &);
    void fire_paused();
    void fire_resumed();
    void fire_textoutput(const std::string &);

    // Read/Write property ${PROPERTY.ident}
    const std::string &get_jsonData() const;
    void set_jsonData(const std::string &);

    const std::string &get_jsonSchedules() const;
    void set_jsonSchedules(const std::string &);

    const std::string &get_jsonConstraints() const;
    void set_jsonConstraints(const std::string &);

    const std::string &get_jsonAlgorithm() const;
    void set_jsonAlgorithm(const std::string &);

    bool get_benchmarkMode() const;
    void set_benchmarkMode(bool);

    // Read-only property ${PROPERTY.ident}
    void fire_get_currentSolution();
    void fire_get_bestSolution();
    void fire_get_feasibleSolution();
    void fire_get_messages();

    void fire_get_version() const;
    void fire_get_majorversion() const;
    void fire_get_minorversion() const;
    void fire_get_patchversion() const;

    //algorithm control
    bool startSearch();
    void stopSearch();
    void pauseSearch();
    void resumeSearch();
    bool isSearchRunning();
};

