#include "Renderer.h"
#include "includes.h"

static void GLClearError();
static bool GLLogCall(const char *function, const char *file, int line);