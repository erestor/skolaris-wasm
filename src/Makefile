# Copyright (c) 2013 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

# GNU Makefile based on shared rules provided by the Native Client SDK.
# See README.Makefiles for more details.

CXX = emcc
TARGET = skolaris

THREADS = -s USE_PTHREADS=1 -s PTHREAD_POOL_SIZE=6
INCBOOST = /usr/local/include
INCCTOOLHU = /usr/local/include/ctoolhu
INCLOCALSEARCH = /usr/local/include/localsearch
INCDIRS = -I${INCBOOST} -I${INCCTOOLHU} -I${INCLOCALSEARCH}

CFLAGS = -O2 -Wall -fno-rtti -std=c++17 ${INCDIRS} -DBOOST_SYSTEM_NO_DEPRECATED

LDFLAGS = -O2 -s MODULARIZE=1 -s EXPORT_NAME=SkolarisModule --shell-file html_template/shell_minimal.html -s EXTRA_EXPORTED_RUNTIME_METHODS='["cwrap"]' ${THREADS} -s ALLOW_MEMORY_GROWTH=1 -s WASM_MEM_MAX=512Mb

UNUSEDLDFLAGS = -s VERBOSE=1 -s TOTAL_MEMORY=16Mb

SOURCES_CC = $(wildcard *.cc)
SOURCES_CPP = \
	$(wildcard gascheduler/src/*.cpp) \
	$(wildcard gascheduler/src/plugin/algorithm_builder.cpp) \
	$(wildcard gascheduler/src/storage/*.cpp) \
	$(wildcard gascheduler/src/timetable/algorithm/*.cpp) \
	$(wildcard gascheduler/src/timetable/analysis/*.cpp) \
	$(wildcard gascheduler/src/timetable/model/*.cpp) \
	$(wildcard gascheduler/src/timetable/*.cpp)

SOURCES = $(SOURCES_CC) $(SOURCES_CPP)
OBJECTS = $(SOURCES_CC:.cc=.o) $(SOURCES_CPP:.cpp=.o)

skolaris: $(OBJECTS)
	$(CXX) -o Release/skolaris.html $^ $(LDFLAGS)

%.o: %.cc
	$(CXX) -c $(CFLAGS) -o $@ $<

%.o: %.cpp
	$(CXX) -c $(CFLAGS) -o $@ $<

clean:
	rm -rf ~/.emscripten_cache; find . -name "*.o" -type f -delete

copy:
	cp Release/skolaris.* ~/Public/skolaris_wasm

