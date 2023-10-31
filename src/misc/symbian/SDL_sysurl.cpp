#include "../SDL_sysurl.h"

extern "C" int SDL_SYS_OpenURL(const char *url)
{
    /* Not supported */
    return SDL_Unsupported();
}
