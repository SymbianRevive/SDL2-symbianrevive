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
#include "../../SDL_internal.h"

/* An implementation of condes using the Symbian API. */

#include <e32std.h>

#include "SDL_thread.h"
#include "SDL_systhread_c.h"

struct SDL_mutex
{
    TInt handle;
};

struct SDL_cond
{
    TInt handle;
};

extern TInt CreateUnique(TInt (*aFunc)(const TDesC &aName, TAny *, TAny *), TAny *, TAny *);

static TInt NewCond(const TDesC &aName, TAny *aPtr1, TAny *)
{
    return ((RCondVar *)aPtr1)->CreateGlobal(aName);
}

/* Create a cond */
SDL_cond *SDL_CreateCond(void)
{
    RCondVar rcond;

    TInt status = CreateUnique(NewCond, &rcond, NULL);
    if (status != KErrNone) {
        SDL_SetError("Couldn't create cond.");
        return NULL;
    }
    SDL_cond *cond = new /*(ELeave)*/ SDL_cond;
    cond->handle = rcond.Handle();
    return cond;
}

/* Free the cond */
void SDL_DestroyCond(SDL_cond *cond)
{
    if (cond) {
        RCondVar rcond;
        rcond.SetHandle(cond->handle);
        rcond.Signal();
        rcond.Close();
        delete (cond);
        cond = NULL;
    }
}

int SDL_CondWait(SDL_cond *cond, SDL_mutex *mutex) SDL_NO_THREAD_SAFETY_ANALYSIS /* clang doesn't know about NULL condes */
{
    if (cond == NULL || mutex == NULL) {
        return 0;
    }

    RMutex rmutex;
    rmutex.SetHandle(mutex->handle);

    RCondVar rcond;
    rcond.SetHandle(cond->handle);
    rcond.Wait(rmutex);

    return 0;
}

int SDL_CondWaitTimeout(SDL_cond *cond, SDL_mutex *mutex, Uint32 ms) SDL_NO_THREAD_SAFETY_ANALYSIS /* clang doesn't know about NULL condes */
{
    if (cond == NULL || mutex == NULL) {
        return 0;
    }

    RMutex rmutex;
    rmutex.SetHandle(mutex->handle);

    RCondVar rcond;
    rcond.SetHandle(cond->handle);
    rcond.TimedWait(rmutex, ms * 1000);

    return 0;
}

int SDL_CondBroadcast(SDL_cond *cond)
{
    if (cond == NULL)
    {
        return 0;
    }

    RCondVar rcond;
    rcond.SetHandle(cond->handle);
    rcond.Broadcast();

    return 0;
}

int SDL_CondSignal(SDL_cond *cond)
{
    if (cond == NULL)
    {
        return 0;
    }

    RCondVar rcond;
    rcond.SetHandle(cond->handle);
    rcond.Signal();

    return 0;
}

/* vi: set ts=4 sw=4 expandtab: */
