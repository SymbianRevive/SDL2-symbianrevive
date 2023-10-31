#include "../../SDL_internal.h"

#ifndef SDL_POWER_DISABLED
#if SDL_POWER_SYMBIAN

#include "SDL_power.h"
#include <hwrmpower.h>

extern "C" SDL_bool SDL_GetPowerInfo_SYMBIAN(SDL_PowerState *state, int *seconds, int *percent)
{
    CHWRMPower *power = CHWRMPower::NewLC();
    TBatteryChargingStatus chargingStatus;
    power->GetBatteryChargingStatus(chargingStatus);
    TRequestStatus status;
    TBatteryConsumptionData batteryData;
    power->GetBatteryInfo(request, batteryData);
    User::WaitForRequest(status);
    User::LeaveIfError(status.Int());

    *seconds = -1;
    *percent = batteryData.iRemainingPercentageCapacity;
    switch (chargingStatus) {
        case EChargingStatusError:
            *state = SDL_POWERSTATE_UNKNOWN;
        case EChargingStatusIllegalChargerError:
        case EChargingStatusChargerUnderVoltageError:
        case EChargingStatusChargerOverVoltageError:
        case EChargingStatusTempTooHotError:
        case EChargingStatusTempTooColdError:
        case EChargingStatusCharging:
        case EChargingStatusNotCharging:
        case EChargingStatusAlmostComplete:
        case EChargingStatusChargingComplete:
        case EChargingStatusChargingContinued:
            *state = SDL_POWERSTATE_CHARGING;
            break;
        case EChargingStatusNotConnected:
            *state = SDL_POWERSTATE_ON_BATTERY;
            break;
    }

    return SDL_TRUE;
}

#endif /* SDL_POWER_SYMBIAN */
#endif /* SDL_POWER_DISABLED */

/* vi: set ts=4 sw=4 expandtab: */
