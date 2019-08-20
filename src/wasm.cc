#include "SkolarisInstance.h"
#include <emscripten/emscripten.h>
#include <memory>
#include <sstream>
#include <stdio.h>

void SkolarisInstance::postMessage(const char *msg) const
{
	EM_ASM({ postMessage(UTF8ToString($0)); }, msg);
}

#ifdef __cplusplus
extern "C" {
#endif

void EMSCRIPTEN_KEEPALIVE handleMessage(const char *msg)
{
	SingleSkolarisInstance::Instance().handleMessage(msg);
}

#ifdef __cplusplus
}
#endif
