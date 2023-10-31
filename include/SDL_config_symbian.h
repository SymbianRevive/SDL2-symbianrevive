/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2023 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#ifndef SDL_config_ngage_h_
#define SDL_config_ngage_h_
#define SDL_config_h_

#include "SDL_platform.h"

typedef signed char        int8_t;
typedef unsigned char      uint8_t;
typedef signed short       int16_t;
typedef unsigned short     uint16_t;
typedef signed int         int32_t;
typedef unsigned int       uint32_t;
typedef signed long long   int64_t;
typedef unsigned long long uint64_t;
typedef unsigned long      uintptr_t;

#define HAVE_STDARG_H    1
#define HAVE_STDDEF_H    1
#define HAVE_STDIO_H     1
#define HAVE_STDLIB_H    1
#define HAVE_MATH_H      1
#define HAVE_CEIL        1
#define HAVE_COPYSIGN    1
#define HAVE_COS         1
#define HAVE_EXP         1
#define HAVE_FABS        1
#define HAVE_FLOOR       1
#define HAVE_LOG         1
#define HAVE_LOG10       1
#define HAVE_SCALBN      1
#define HAVE_SIN         1
#define HAVE_SQRT        1
#define HAVE_TAN         1
#define HAVE_MALLOC      1
#define SDL_MAIN_NEEDED  1

#define SDL_THREAD_PTHREAD 1

#define SDL_TIMER_UNIX  1

#define SDL_VIDEO_DRIVER_SYMBIAN 1

#define SDL_AUDIO_DRIVER_SYMBIAN  1

#define SDL_JOYSTICK_DISABLED   1

#define SDL_HAPTIC_SYMBIAN 1

#define SDL_HIDAPI_DISABLED 1

#define SDL_SENSOR_SYMBIAN 1

#define SDL_LOADSO_DISABLED 1

#define SDL_FILESYSTEM_SYMBIAN 1

#endif /* SDL_config_ngage_h_ */
