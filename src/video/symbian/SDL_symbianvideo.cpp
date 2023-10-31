#include "../../SDL_internal.h"

#if SDL_VIDEO_DRIVER_SYMBIAN

/* SDL internals */
#include "../SDL_sysvideo.h"
#include "SDL_version.h"
#include "SDL_syswm.h"
#include "SDL_loadso.h"
#include "SDL_events.h"
#include "../../events/SDL_mouse_c.h"
#include "../../events/SDL_keyboard_c.h"
#include "SDL_hints.h"

static SDL_VideoDevice *SYMBIAN_Create()
{
    SDL_VideoDevice *device;
    SDL_VideoData *phdata;

    /* Initialize SDL_VideoDevice structure */
    device = (SDL_VideoDevice *)SDL_calloc(1, sizeof(SDL_VideoDevice));
    if (device == NULL) {
        SDL_OutOfMemory();
        return NULL;
    }

    /* Initialize internal data */
    phdata = (SDL_VideoData *)SDL_calloc(1, sizeof(SDL_VideoData));
    if (phdata == NULL) {
        SDL_OutOfMemory();
        SDL_free(device);
        return NULL;
    }

    device->driverdata = phdata;

    /* Setup amount of available displays */
    device->num_displays = 0;

    /* Set device free function */
    device->free = SYMBIAN_Destroy;

    /* Setup all functions which we can handle */
    device->VideoInit = SYMBIAN_VideoInit;
    device->VideoQuit = SYMBIAN_VideoQuit;
    device->GetDisplayModes = SYMBIAN_GetDisplayModes;
    device->SetDisplayMode = SYMBIAN_SetDisplayMode;
    device->CreateSDLWindow = SYMBIAN_CreateWindow;
    device->CreateSDLWindowFrom = SYMBIAN_CreateWindowFrom;
    device->SetWindowTitle = SYMBIAN_SetWindowTitle;
    device->SetWindowIcon = SYMBIAN_SetWindowIcon;
    device->SetWindowPosition = SYMBIAN_SetWindowPosition;
    device->SetWindowSize = SYMBIAN_SetWindowSize;
    device->ShowWindow = SYMBIAN_ShowWindow;
    device->HideWindow = SYMBIAN_HideWindow;
    device->RaiseWindow = SYMBIAN_RaiseWindow;
    device->MaximizeWindow = SYMBIAN_MaximizeWindow;
    device->MinimizeWindow = SYMBIAN_MinimizeWindow;
    device->RestoreWindow = SYMBIAN_RestoreWindow;
    device->DestroyWindow = SYMBIAN_DestroyWindow;
#if 0
    device->GetWindowWMInfo = SYMBIAN_GetWindowWMInfo;
#endif
    device->GL_LoadLibrary = SYMBIAN_GLES_LoadLibrary;
    device->GL_GetProcAddress = SYMBIAN_GLES_GetProcAddress;
    device->GL_UnloadLibrary = SYMBIAN_GLES_UnloadLibrary;
    device->GL_CreateContext = SYMBIAN_GLES_CreateContext;
    device->GL_MakeCurrent = SYMBIAN_GLES_MakeCurrent;
    device->GL_SetSwapInterval = SYMBIAN_GLES_SetSwapInterval;
    device->GL_GetSwapInterval = SYMBIAN_GLES_GetSwapInterval;
    device->GL_SwapWindow = SYMBIAN_GLES_SwapWindow;
    device->GL_DeleteContext = SYMBIAN_GLES_DeleteContext;
    device->GL_DefaultProfileConfig = SYMBIAN_GLES_DefaultProfileConfig;

    device->PumpEvents = SYMBIAN_PumpEvents;

    return device;
}

VideoBootStrap SYMBIAN_bootstrap = {
    "symbian",
    "Symbian Video Driver",
    SYMBIAN_Create
};
#endif