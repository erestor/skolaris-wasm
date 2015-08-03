#include "JSAPIAuto.h"
#include "BrowserHost.h"
#include "Skolaris_fwd.h"

#ifndef H_SkolarisAPI
#define H_SkolarisAPI

//javascript API of the plugin - methods callable from JS are camelCase
class SkolarisAPI : public FB::JSAPIAuto {

  public:

    SkolarisAPI(const SkolarisPtr &plugin, const FB::BrowserHostPtr &host);
	~SkolarisAPI();

    SkolarisPtr getPlugin();

	FB_JSAPI_EVENT(bestsolutionfound, 0, ());
	FB_JSAPI_EVENT(feasiblesolutionfound, 0, ());
	FB_JSAPI_EVENT(searchfinished, 5, (int, int, int, int, const std::string &));
	FB_JSAPI_EVENT(paused, 0, ());
	FB_JSAPI_EVENT(resumed, 0, ());
	FB_JSAPI_EVENT(textoutput, 1, (const std::string &));

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
	std::string get_currentSolution();
	std::string get_bestSolution();
	std::string get_feasibleSolution();
	std::string get_messages();

    const std::string &get_version() const;
    int get_majorversion() const;
    int get_minorversion() const;
    int get_patchversion() const;

	//algorithm control
	bool startSearch();
	void stopSearch();
	void pauseSearch();
	void resumeSearch();
	bool isSearchRunning();

  private:

	SkolarisWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;

	std::string m_jsonData;
	std::string m_jsonSchedules;
	std::string m_jsonConstraints;
	std::string m_jsonAlgorithm;
	bool m_benchmarkMode;
};

#endif // H_SkolarisAPI
