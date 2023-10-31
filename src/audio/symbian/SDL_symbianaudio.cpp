#include "SDL_symbianaudio.h"

void SDL_PrivateAudioData::RequestSound() {
  static TUint8 buffer[iDevice->spec.size];
  iDevice->callbackspec.callback(iDevice->callbackspec.userdata, buffer, iDevice->spec.size);

  if (iOutputStatus == ESetVolume) {
    iOutputStatus = EOpen;
    if (iVolume < 0)
      iVolume = 0;
    if (iVolume > iOutputStream->MaxVolume())
      iVolume = iOutputStream->MaxVolume();
    //LPRINTF("Set volume to %d", iVolume);
    iOutputStream->SetVolume(iVolume);
  }

  iOutputStream->WriteL(iSoundBufferPtr);
}

~SDL_PrivateAudioData() {
  delete[] iSoundBuffer;
}

SDL_PrivateAudioData::SDL_PrivateAudioData(_THIS)
  : iDevice{_this} {
  TUint32 format;
  for (test_format = SDL_FirstAudioFormat(_this->spec.format); test_format; test_format = SDL_NextAudioFormat()) {
    switch (test_format) {
    case AUDIO_S8:
      format = KMMFFourCCCodePCM8;
      break;

    case AUDIO_U8:
      format = KMMFFourCCCodePCMU8;
      break;

    case AUDIO_S16LSB:
      format = KMMFFourCCCodePCM16;
      break;

    case AUDIO_S16MSB:
      format = KMMFFourCCCodePCM16B;
      break;

    case AUDIO_S32LSB:
      format = KMMFFourCCCodePCM32;
      break;

    case AUDIO_S32MSB:
      format = KMMFFourCCCodePCM32B;
      break;

    default:
      continue;
    }
  }
  iSoundBuffer = new TUint8[_this->spec.size];
  iSoundBufferPtr.Set(iSoundBuffer, _this->spec.size);

  switch (_this->spec.format) {
  case 8000:
    iStreamSettings.iSampleRate = TMdaAudioDataSettings::ESampleRate8000Hz;
    break;
  case 11025:
    iStreamSettings.iSampleRate = TMdaAudioDataSettings::ESampleRate11025Hz;
    break;
  case 16000:
    iStreamSettings.iSampleRate = TMdaAudioDataSettings::ESampleRate16000Hz;
    break;
  case 22050:
    iStreamSettings.iSampleRate = TMdaAudioDataSettings::ESampleRate22050Hz;
    break;
  case 24000:
    iStreamSettings.iSampleRate = TMdaAudioDataSettings::ESampleRate24000Hz;
    break;
  case 32000:
    iStreamSettings.iSampleRate = TMdaAudioDataSettings::ESampleRate32000Hz;
    break;
  case 44100:
    iStreamSettings.iSampleRate = TMdaAudioDataSettings::ESampleRate44100Hz;
    break;
  case 48000:
    iStreamSettings.iSampleRate = TMdaAudioDataSettings::ESampleRate48000Hz;
    break;
  case 96000:
    iStreamSettings.iSampleRate = TMdaAudioDataSettings::ESampleRate96000Hz;
    break;
  case 64000:
    iStreamSettings.iSampleRate = TMdaAudioDataSettings::ESampleRate64000Hz;
    break;
  default:
    return SDL_SetError("%s: Unsupported audio sample rate", "mda");
  }
  iStreamSettings.iChannels = _this->spec.channels == 1 ? TMdaAudioDataSettings::EChannelsMono : TMdaAudioDataSettings::EChannelsStereo;
  iStreamSettings.iCaps = TMdaAudioDataSettings::ERealTime | TMdaAudioDataSettings::ESampleRateFixed;

  iOutputStream = CMdaAudioOutputStream::NewL(*this);
  iOutputStream->SetDataTypeL({ format });
  iOutputStream->Open(&iStreamSettings);
}

namespace {
  
int SYMBIANAUDIO_OpenDevice(_THIS, const char *devname) {
  auto p = _this->hidden = new SDL_PrivateAudioData(_this);
}
}


extern "C" {

SDL_bool HAIKUAUDIO_Init(SDL_AudioDriverImpl * impl) {
  impl->OpenDevice = SYMBIANAUDIO_OpenDevice;
}

AudioBootStrap SYMBIANAUDIO_bootstrap = {
    "mda", "Symbian MDA Audio Interface", SYMBIANAUDIO_Init, SDL_FALSE
};
}
