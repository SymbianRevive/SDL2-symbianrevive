#include "../../SDL_internal.h"

#if SDL_VIDEO_DRIVER_SYMBIAN
#include <stdlib.h>
#include <string.h>

#include "SDL_error.h"
#include "SDL_log.h"
#include "../SDL_egl_c.h"

void SYMBIAN_GLES_DefaultProfileConfig(_THIS, int *mask, int *major, int *minor)
{
    *mask = SDL_GL_CONTEXT_PROFILE_ES;
    *major = 2;
    *minor = 0;
}

int SYMBIAN_GLES_LoadLibrary(_THIS, const char *path)
{
    return SDL_EGL_LoadLibrary(_this, path, EGL_DEFAULT_DISPLAY, 0);
}

SDL_EGL_SwapWindow_impl(SYMBIAN)
SDL_EGL_CreateContext_impl(SYMBIAN)
SDL_EGL_MakeCurrent_impl(SYMBIAN)

#endif