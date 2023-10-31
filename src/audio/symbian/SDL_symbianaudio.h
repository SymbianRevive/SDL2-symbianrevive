#pragma once

#include "../SDL_sysaudio.h"

#include <mdaaudiosampleeditor.h>
#include <mdaaudiooutputstream.h>
#include <mda/common/audio.h>
#include <mmf/common/mmfutilities.h>

/* Hidden "this" pointer for the audio functions */
#define _THIS SDL_AudioDevice *_this

struct SDL_PrivateAudioData : MMdaAudioOutputStreamCallback {
  SDL_AudioDevice *iDevice;

  TUint8 *iSoundBuffer;
  TPtrC8 iSoundBufferPtr;

  TInt iVolume;
  TUint iVolumeStep;

  TMdaAudioDataSettings iStreamSettings;
  CMdaAudioOutputStream* iOutputStream;
  TStatus iOutputStatus;

public:

  SDL_PrivateAudioData(_THIS);

  ~SDL_PrivateAudioData();

  void MaoscOpenComplete(TInt aError) override;

  void MaoscBufferCopied(TInt aError, const TDesC8& aBuffer) override;
  
  void MaoscPlayComplete(TInt aError) override;
  
private:
  void RequestSound();
};