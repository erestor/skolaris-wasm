CXX = emcc
TARGET = skolaris
BUILD_DIR = build

ifeq "${MAKECMDGOALS}" "skolarisDebug"
BUILD_DIR = build_debug
CFLAGSBUILD = -gsource-map -D_DEBUG -pthread -sDISABLE_EXCEPTION_CATCHING=0
LDFLAGSBUILD = -sDISABLE_EXCEPTION_CATCHING=0 --source-map-base https://skolaris.net/dev8/Plugin/src/ -gsource-map -sASSERTIONS=2 -sSAFE_HEAP
else
CFLAGSBUILD = -DNDEBUG -O3 -pthread -flto
LDFLAGSBUILD = -O3 -flto
endif

ifeq "${MAKECMDGOALS}" "skolarisOneThread"
BUILD_DIR = build_one_thread
CFLAGSBUILD = -DNDEBUG -O3 -DUSE_ONE_THREAD -flto
endif

ifeq "${MAKECMDGOALS}" "skolarisOneThreadDebug"
BUILD_DIR = build_one_thread_debug
CFLAGSBUILD = -gsource-map -D_DEBUG -DUSE_ONE_THREAD -sDISABLE_EXCEPTION_CATCHING=0
LDFLAGSBUILD = -sDISABLE_EXCEPTION_CATCHING=0 --source-map-base https://localhost/SkolarisUI.Web/Plugin/src/ -gsource-map -sASSERTIONS=2 -sSAFE_HEAP
endif

#$(info $$BUILD_DIR is [${BUILD_DIR}])

MKDIR_P = @mkdir -p

INCBOOST = /usr/local/include
INCCTOOLHU = /usr/local/include/ctoolhu
INCLOCALSEARCH = /usr/local/include/localsearch
INCDIRS = -I${INCBOOST} -I${INCCTOOLHU} -I${INCLOCALSEARCH}

CFLAGS = -Wall -std=c++20 -fexperimental-library -fno-rtti ${INCDIRS} -DBOOST_SYSTEM_NO_DEPRECATED -DBOOST_NO_RTTI -DBOOST_NO_TYPEID -MMD -MP -Wno-deprecated-builtins

LDFLAGS = -sALLOW_MEMORY_GROWTH -sINITIAL_MEMORY=64Mb -sMODULARIZE=1 -sTEXTDECODER=2 -sEXPORTED_FUNCTIONS='["_malloc", "_free"]' --shell-file src/html_template/shell_minimal.html
LDFLAGSONETHREAD = -sEXPORT_NAME=SkolarisOneThreadModule -sEXPORTED_RUNTIME_METHODS='["cwrap", "lengthBytesUTF8", "stringToUTF8"]'
LDFLAGSMULTITHREAD = -pthread -sPTHREAD_POOL_SIZE='navigator.hardwareConcurrency+2' -sEXPORT_NAME=SkolarisModule -sEXPORTED_RUNTIME_METHODS='["cwrap", "lengthBytesUTF8", "stringToUTF8", "PThread"]'
LDFLAGSUNUSED = -sVERBOSE=1

SOURCES_CC = $(wildcard src/*.cc)
SOURCES_CPP = \
	$(wildcard src/gascheduler/src/*.cpp) \
	$(wildcard src/gascheduler/src/plugin/algorithm_builder.cpp) \
	$(wildcard src/gascheduler/src/timetable/*.cpp) \
	$(wildcard src/gascheduler/src/timetable/algorithm/*.cpp) \
	$(wildcard src/gascheduler/src/timetable/analysis/*.cpp) \
	$(wildcard src/gascheduler/src/timetable/constraints/*.cpp) \
	$(wildcard src/gascheduler/src/timetable/fitness/*.cpp) \
	$(wildcard src/gascheduler/src/timetable/model/*.cpp) \
	$(wildcard src/gascheduler/src/timetable/solution/*.cpp)

SOURCES = $(SOURCES_CC) $(SOURCES_CPP)
OBJS = $(SOURCES:%=$(BUILD_DIR)/%.o)
DEPS = $(OBJS:.o=.d)

$(BUILD_DIR)/%.cc.o: %.cc
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CFLAGS) $(CFLAGSBUILD) -c $< -o $@

$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CFLAGS) $(CFLAGSBUILD) -c $< -o $@

skolaris: $(OBJS)
	$(MKDIR_P) release
	$(CXX) $(OBJS) -o release/skolaris.html $(LDFLAGS) $(LDFLAGSMULTITHREAD) $(LDFLAGSBUILD)

skolarisDebug: $(OBJS)
	$(MKDIR_P) debug
	$(CXX) $(OBJS) -o debug/skolaris.html $(LDFLAGS) $(LDFLAGSMULTITHREAD) $(LDFLAGSBUILD)

skolarisOneThread: $(OBJS)
	$(MKDIR_P) release
	$(CXX) $(OBJS) -o release/skolarisOneThread.html $(LDFLAGS) $(LDFLAGSONETHREAD) $(LDFLAGSBUILD)

skolarisOneThreadDebug: $(OBJS)
	$(MKDIR_P) debug
	$(CXX) $(OBJS) -o debug/skolarisOneThread.html $(LDFLAGS) $(LDFLAGSONETHREAD) $(LDFLAGSBUILD)

-include $(DEPS)

.PHONY: clean
clean:
	@rm -rf build build_debug build_one_thread build_one_thread_debug release debug

.PHONY: install
install:
	cp release/skolaris.* release/skolarisOneThread.* ~/Public/skolaris_wasm/release
	
.PHONY: install_debug
install_debug:
	cp debug/skolaris.* debug/skolarisOneThread.* ~/Public/skolaris_wasm/debug
