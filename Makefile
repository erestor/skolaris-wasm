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

LDFLAGS = -s TOTAL_MEMORY=384Mb -s USE_PTHREADS=1 -s PTHREAD_POOL_SIZE=6 --shell-file src/html_template/shell_minimal.html
LDFLAGSDEBUG = -s EXTRA_EXPORTED_RUNTIME_METHODS='["calledRun","cwrap"]' -s ASSERTIONS=1 -s DEMANGLE_SUPPORT=1 -s DISABLE_EXCEPTION_CATCHING=0 --source-map-base http://localhost/SkolarisUI.Web/Plugin/src/ -g4
LDFLAGSRELEASE = -s EXTRA_EXPORTED_RUNTIME_METHODS='["cwrap"]' -s MODULARIZE=1 -s EXPORT_NAME=SkolarisModule -O2
LDFLAGSUNUSED = -s VERBOSE=1 -s TOTAL_MEMORY=16Mb -s ALLOW_MEMORY_GROWTH=1 -s WASM_MEM_MAX=512Mb -s USE_PTHREADS=1 -s PTHREAD_POOL_SIZE=6

SOURCES_CC = $(wildcard src/*.cc)
SOURCES_CPP = \
	$(wildcard src/gascheduler/src/*.cpp) \
	$(wildcard src/gascheduler/src/plugin/algorithm_builder.cpp) \
	$(wildcard src/gascheduler/src/timetable/*.cpp) \
	$(wildcard src/gascheduler/src/timetable/algorithm/*.cpp) \
	$(wildcard src/gascheduler/src/timetable/analysis/*.cpp) \
	$(wildcard src/gascheduler/src/timetable/constraints/*.cpp) \
	$(wildcard src/gascheduler/src/timetable/model/*.cpp)

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
	$(MKDIR_P) release
	$(CXX) $(OBJS) -o release/skolaris.html $(LDFLAGS) $(LDFLAGSRELEASE)

debug: $(OBJS)
	$(MKDIR_P) debug
	$(CXX) $(OBJS) -o debug/skolaris.html $(LDFLAGS) $(LDFLAGSDEBUG)

-include $(DEPS)

.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR)

.PHONY: wipe
wipe:
	rm -rf ~/.emscripten_cache; rm -rf $(BUILD_DIR)

.PHONY: install
install:
	cp release/skolaris.* ~/Public/skolaris_wasm/release; cp debug/skolaris.* ~/Public/skolaris_wasm/debug
