// MIT License
//
// Copyright (c) 2023 Julia Nelz
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "SDL_main.h"

#include "SDL_symbianappui.h"
#include "SDL_symbiancontainer.h"

#include <AknDef.h>
#include <eikdoc.h>
#include <aknappui.h>
#include <coecntrl.h>
#include <coemain.h>
#include <w32std.h>
#include <e32event.h>
#include <algorithm>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>

#if !defined(EKeyLeftUpArrow) && \
    !defined(EKeyRightUpArrow) && \
    !defined(EKeyRightDownArrow) && \
    !defined(EKeyLeftDownArrow)
#define EKeyLeftUpArrow      EKeyDevice10  //! Diagonal arrow event
#define EKeyRightUpArrow     EKeyDevice11  //! Diagonal arrow event
#define EKeyRightDownArrow   EKeyDevice12  //! Diagonal arrow event
#define EKeyLeftDownArrow    EKeyDevice13  //! Diagonal arrow event
#endif

#pragma mark - CSdlContainer constants

extern "C" const char KReMapping[256];
extern "C" const char KReMappingF[256];
extern "C" const char KReMappingS[256];

// clang-format on

#pragma mark - CSdlContainer impl

CSdlContainer::CSdlContainer() : iSoundBufferPtr{KNullDesC8} {}

void CSdlContainer::ConstructL(const TRect &aRect, CAknAppUi *aAppUi) {
  if (kInstance) {
    _LIT(KSdl, "SDL");
    User::Panic(KSdl, 17);
  }
  kInstance = this;

  iAppUi = aAppUi;

  CreateWindowL();
  SetExtentToWholeScreen();
  ActivateL();

  HandleResourceChange(KEikDynamicLayoutVariantSwitch);

  iPeriodic = CPeriodic::NewL(CActive::EPriorityIdle);
  iPeriodic->Start(10000, 10000, TCallBack(CGodotContainer::ResetInactivityTimeCallBack, this));

  Window().PointerFilter(EPointerFilterDrag, 0);
  iWsEventReceiver = CWsEventReceiver::NewL(*this, &CCoeEnv::Static()->WsSession());

  iEngineThread.Create(_L("SdlEngine"), RunEngineThread, 0x400000, nullptr, this);
}

CSdlContainer *CSdlContainer::Instance() { return kInstance; }

static CSdlContainer *CSdlContainer::kInstance{};

TInt CSdlContainer::RunEngineThread(TAny *aInstance) {
  User::SetCritical(User::TCritical::EProcessPermanent);

  return SDL_main(0, NULL);
}

TInt CSdlContainer::ResetInactivityTimeCallBack(TAny *aInstance) {
  CSdlContainer *instance = static_cast<CSdlContainer *>(aInstance);

  User::ResetInactivityTime();

  User::After(10000);

  return 0;
}

bool CSdlContainer::IsScanCodeNonModifier(TInt aScanCode) {
  return !(aScanCode == EStdKeyLeftFunc ||
           aScanCode == EStdKeyRightFunc ||
           aScanCode == EStdKeyLeftCtrl ||
           aScanCode == EStdKeyRightCtrl ||
           aScanCode == EStdKeyLeftShift ||
           aScanCode == EStdKeyRightShift);
}

bool CSdlContainer::HandleWsEvent(TWsEvent &aEvent) {
  TInt scanCode;
  switch (aEvent.Type()) {
  case EEventKeyDown:
    scanCode = aEvent.Key()->iScanCode;
    switch (aEvent.Key()->iCode) {
    case EKeyIncVolume:
      iVolume += iVolumeStep;
      iOutputStatus = ESetVolume;
      return true;
    case EKeyDecVolume:
      iVolume -= iVolumeStep;
      iOutputStatus = ESetVolume;
      return true;
    default:
      if (!std::count(iPressedKeys.begin(), iPressedKeys.end(), scanCode)) {
        iPressedKeys.push_back(scanCode);
        if (CSdlContainer::IsScanCodeNonModifier(scanCode)) {
          iModifierUsed = true;
        }
      }
      return true;
    }
    break;
  case EEventKeyUp:
    scanCode = aEvent.Key()->iScanCode;
    if (CSdlContainer::IsScanCodeNonModifier(scanCode)) {
      iPressedKeys.remove(scanCode);
      if (!std::count_if(iPressedKeys.begin(), iPressedKeys.end(), CSdlContainer::IsScanCodeNonModifier)) {
        for (auto modifier : iDepressedModifiers) {
          iPressedKeys.remove(modifier);
        }
        iDepressedModifiers.clear();
      }
    } else if (iModifierUsed) {
      if (!std::count_if(iPressedKeys.begin(), iPressedKeys.end(), CSdlContainer::IsScanCodeNonModifier)) {
        iPressedKeys.remove(scanCode);
        iDepressedModifiers.erase(scanCode);
      } else {
        iPressedKeys.clear();
      }
    } else {
      iDepressedModifiers.insert(scanCode);
    }
    if (!std::count_if(iPressedKeys.begin(), iPressedKeys.end(), [](auto scanCode) {
        return !CSdlContainer::IsScanCodeNonModifier(scanCode);
      })) {
      iModifierUsed = false;
    }
    if (iPressedKeys.empty()) {
      iDepressedModifiers.clear();
    }
    return true;
  case EEventPointer:
    switch (aEvent.Pointer()->iType) {
    case TPointerEvent::EButton1Down: 
      iPosition = aEvent.Pointer()->iPosition;
      iButton1DownNextFrame = true;
      break;
    case TPointerEvent::EButton1Up:
      iPosition = aEvent.Pointer()->iPosition;
      iButton1DownNextFrame = false;
      break;
    case TPointerEvent::EDrag:
    case TPointerEvent::EMove:
      iPosition = aEvent.Pointer()->iPosition;
      break;
    }
    return true;
  default:
    break;
  }
  return false;
}

void CSdlContainer::Draw(const TRect &aRect) const {}

void CSdlContainer::SizeChanged() {
  eglSwapBuffers(iEglDisplay, iEglSurface);
}

void CSdlContainer::HandleResourceChange(TInt aType) {
  switch (aType) {
  case KEikDynamicLayoutVariantSwitch:
    SetExtentToWholeScreen();
    eglSwapBuffers(iEglDisplay, iEglSurface);
    break;
  }
}

TInt CSdlContainer::CountComponentControls() const { return 0; }

CCoeControl *CSdlContainer::ComponentControl(TInt aIndex) const {
  return nullptr;
}

CSdlContainer::~CSdlContainer() {
  delete iPeriodic;

  if (iOutputStatus != ENotReady)
    iOutputStream->Stop();
  delete iOutputStream;

  eglMakeCurrent(iEglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
  eglDestroySurface(iEglDisplay, iEglSurface);
  eglDestroyContext(iEglDisplay, iEglContext);
  eglTerminate(iEglDisplay);
#ifdef SYMBIAN_GLES2_ENABLED
  eglReleaseThread();
#endif
}

#pragma mark - CWsEventReceiver impl

CWsEventReceiver::CWsEventReceiver()
    : CActive(CActive::EPriorityHigh), iParent(NULL) {}

CWsEventReceiver::~CWsEventReceiver() { Cancel(); }

CWsEventReceiver *CWsEventReceiver::NewL(CSdlContainer &aParent,
                                         RWsSession *aWsSession) {
  CWsEventReceiver *self = new (ELeave) CWsEventReceiver;

  CleanupStack::PushL(self);

  self->ConstructL(aParent, aWsSession);

  CleanupStack::Pop(self);

  return self;
}

void CWsEventReceiver::ConstructL(CSdlContainer &aParent, RWsSession *aWsSession) {
  iParent = &aParent;
  iWsSession = aWsSession;
  iWsSession->EventReady(&iStatus);

  CActiveScheduler::Add(this);

  SetActive();
}

void CWsEventReceiver::RunL() {
  TWsEvent wsEvent;
  iWsSession->GetEvent(wsEvent);

  if (!iParent->HandleWsEvent(wsEvent)) {
    static_cast<CSdlAppUi *>(iParent->iAppUi)->HandleEventL(wsEvent);
  }

  iWsSession->EventReady(&iStatus);

  SetActive();
}

void CWsEventReceiver::DoCancel() {
  iWsSession->EventReadyCancel();
}
