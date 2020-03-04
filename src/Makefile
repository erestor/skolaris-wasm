CXX = emcc
TARGET = skolaris
BUILD_DIR = build

MKDIR_P = @mkdir -p

INCBOOST = /usr/local/include
INCCTOOLHU = /usr/local/include/ctoolhu
INCLOCALSEARCH = /usr/local/include/localsearch
INCDIRS = -I${INCBOOST} -I${INCCTOOLHU} -I${INCLOCALSEARCH}

CFLAGS = -Wall -std=c++17 -fno-rtti ${INCDIRS} -DBOOST_SYSTEM_NO_DEPRECATED -DBOOST_NO_RTTI -DBOOST_NO_TYPEID -MMD -MP
CFLAGSDEBUG = -g4
CFLAGSRELEASE = -DNDEBUG -O2

LDFLAGS = -s TOTAL_MEMORY=384Mb -s USE_PTHREADS=1 -s PTHREAD_POOL_SIZE=6 --shell-file html_template/shell_minimal.html
LDFLAGSDEBUG = -s EXTRA_EXPORTED_RUNTIME_METHODS='["calledRun","cwrap"]' -s ASSERTIONS=1 -s DEMANGLE_SUPPORT=1 -s DISABLE_EXCEPTION_CATCHING=0 --source-map-base http://localhost/SkolarisUI.Web/Plugin/src/ -g4
LDFLAGSRELEASE = -s EXTRA_EXPORTED_RUNTIME_METHODS='["cwrap"]' -s MODULARIZE=1 -s EXPORT_NAME=SkolarisModule -O2
LDFLAGSUNUSED = -s VERBOSE=1 -s TOTAL_MEMORY=16Mb -s ALLOW_MEMORY_GROWTH=1 -s WASM_MEM_MAX=512Mb -s USE_PTHREADS=1 -s PTHREAD_POOL_SIZE=6

SOURCES_CC = $(wildcard *.cc)
SOURCES_CPP = \
	$(wildcard gascheduler/src/*.cpp) \
	$(wildcard gascheduler/src/plugin/algorithm_builder.cpp) \
	$(wildcard gascheduler/src/timetable/*.cpp) \
	$(wildcard gascheduler/src/timetable/algorithm/*.cpp) \
	$(wildcard gascheduler/src/timetable/analysis/*.cpp) \
	$(wildcard gascheduler/src/timetable/constraints/*.cpp) \
	$(wildcard gascheduler/src/timetable/model/*.cpp)

SOURCES = $(SOURCES_CC) $(SOURCES_CPP)
OBJS = $(SOURCES:%=$(BUILD_DIR)/%.o)
#OBJS = $(SOURCES_CC:.cc=.o) $(SOURCES_CPP:.cpp=.o)
DEPS = $(OBJS:.o=.d)

$(BUILD_DIR)/%.cc.o: %.cc
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CFLAGS) $(CFLAGSRELEASE) -c $< -o $@

$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CFLAGS) $(CFLAGSRELEASE) -c $< -o $@

skolaris: $(OBJS)
	$(CXX) $(OBJS) -o Release/skolaris.html $(LDFLAGS) $(LDFLAGSRELEASE)

debug: $(OBJS)
	$(CXX) $(OBJS) -o Debug/skolaris.html $(LDFLAGS) $(LDFLAGSDEBUG)

-include $(DEPS)

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: wipe
wipe:
	rm -rf ~/.emscripten_cache; find . -name "*.o" -type f -delete

.PHONY: install
install:
	cp Release/skolaris.* ~/Public/skolaris_wasm/release; cp Debug/skolaris.* ~/Public/skolaris_wasm/debug

