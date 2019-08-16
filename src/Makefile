# Copyright (c) 2013 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

# GNU Makefile based on shared rules provided by the Native Client SDK.
# See README.Makefiles for more details.

CXX = emcc
TARGET = skolaris

THREADS = -s USE_PTHREADS=1 -s PTHREAD_POOL_SIZE=5
INCBOOST = /usr/local/include
INCCTOOLHU = /usr/local/include/ctoolhu
INCLOCALSEARCH = /usr/local/include/localsearch/include
INCDIRS = -I${INCBOOST} -I${INCCTOOLHU} -I${INCLOCALSEARCH}

NOERROR1 = no-error=unused-local-typedef
NOLOCALTYPEDEF = no-unused-local-typedef
NOUNUSEDCAPTURE = no-unused-lambda-capture

CFLAGS = -Wall -W${NOLOCALTYPEDEF} -W$(NOUNUSEDCAPTURE) -std=c++17 ${INCDIRS} -DBOOST_SYSTEM_NO_DEPRECATED -g4

LDFLAGS = -s WASM=1 --shell-file html_template/shell_minimal.html -s NO_EXIT_RUNTIME=1 -s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' -s ASSERTIONS=1 -s VERBOSE=1 -s NO_FILESYSTEM=1 -g4 --source-map-base http://localhost/SkolarisUI.Web/Plugin/src/ ${THREADS}

SOURCES_CC = $(wildcard *.cc)
SOURCES_CPP = \
	$(wildcard gascheduler/src/*.cpp) \
	$(wildcard gascheduler/src/plugin/algorithm_builder.cpp) \
	$(wildcard gascheduler/src/storage/*.cpp) \
	$(wildcard gascheduler/src/timetable/algorithm/*.cpp) \
	$(wildcard gascheduler/src/timetable/analysis/*.cpp) \
	$(wildcard gascheduler/src/timetable/model/*.cpp) \
	$(wildcard gascheduler/src/timetable/*.cpp) \
	$(wildcard ~/dev/localsearch/**/*.cpp)

SOURCES = $(SOURCES_CC) $(SOURCES_CPP)
OBJECTS = $(SOURCES_CC:.cc=.o) $(SOURCES_CPP:.cpp=.o)

skolaris: $(OBJECTS)
	EMCC_DEBUG=1 $(CXX) -o Release/skolaris.html $^ $(LDFLAGS)

%.o: %.cc
	$(CXX) -c $(CFLAGS) -o $@ $<

%.o: %.cpp
	$(CXX) -c $(CFLAGS) -o $@ $<

clean:
	find . ~/dev/localsearch -name "*.o" -type f -delete

copy:
	cp Release/skolaris.* Release/pthread-main.js ~/Public/skolaris_wasm

