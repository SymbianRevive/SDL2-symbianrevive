#pragma once

#include "../../SDL_internal.h"

#if SDL_VIDEO_DRIVER_SYMBIAN && SDL_VIDEO_OPENGL_EGL

#include "../SDL_sysvideo.h"
#include "../SDL_egl_c.h"

/* OpenGLES functions */
#define RPI_GLES_GetAttribute SDL_EGL_GetAttribute
#define RPI_GLES_GetProcAddress SDL_EGL_GetProcAddress
#define RPI_GLES_UnloadLibrary SDL_EGL_UnloadLibrary
#define RPI_GLES_SetSwapInterval SDL_EGL_SetSwapInterval
#define RPI_GLES_GetSwapInterval SDL_EGL_GetSwapInterval
#define RPI_GLES_DeleteContext   SDL_EGL_DeleteContext

extern int RPI_GLES_LoadLibrary(_THIS, const char *path);
extern SDL_GLContext RPI_GLES_CreateContext(_THIS, SDL_Window *window);
extern int RPI_GLES_SwapWindow(_THIS, SDL_Window *window);
extern int RPI_GLES_MakeCurrent(_THIS, SDL_Window *window, SDL_GLContext context);
extern void RPI_GLES_DefaultProfileConfig(_THIS, int *mask, int *major, int *minor);

#endif
/* vi: set ts=4 sw=4 expandtab: */