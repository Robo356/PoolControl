//#define _SERIAL_DEBUG
//#define DISPLAY_ANALOG_VALUES

#include "Arduino.h"
#include "Menu.h"

class PoolPumpController
{
private:
  Hardware _Hardware;
  Menu _Menu;

  const uint8_t _MidDay = 14;
  const unsigned long _LdrDelayTreshold = 10000;
  unsigned long _LdrDelayTimer;
  bool _SunIsShining;
  unsigned int _LdrSampleCount;
  unsigned int _LdrSamples;
  const unsigned int _LdrSampleTreshold = 50; // Sun is on 50 % of the time

  void CheckSunlight()
  {
    if (_LdrDelayTimer == 0 && _SunIsShining != _Hardware.LdrIsOn()) // Start timer
    {
      _LdrDelayTimer = millis();
      _LdrSampleCount = 0;
      _LdrSamples = 0;
    }
    if (_LdrDelayTimer > 0) // Timing LDR
    {
      // Sample
      _LdrSampleCount ++;
      if (_Hardware.LdrIsOn())
      {
        _LdrSamples ++;
      }
      if (millis() - _LdrDelayTimer > _LdrDelayTreshold)
      {
        _SunIsShining =  ((_LdrSamples * 100) / _LdrSampleCount) > _LdrSampleTreshold;
        // Reset timer
        _LdrDelayTimer = 0;
      }
    }
  }

  uint8_t GetTimerHourSpan()
  {
    switch (_Menu.GetTimerMode()) {
      case TimerModes::Timer2h:
        return 2;
      case TimerModes::Timer4h:
        return 4;
      case TimerModes::Timer8h:
        return 8;
      case TimerModes::Timer12h:
        return 12;
      }
    return 2;
  }

  void DoPumpModeTimer(bool withLight)
  {
    uint8_t hourSpan = GetTimerHourSpan();
    uint8_t hour = _Hardware.RTC.now().hour();
    if ((_SunIsShining || !withLight) &&
    (hour >= (_MidDay - (hourSpan / 2)) && hour < (_MidDay + (hourSpan / 2))))
    {
      if (!_Hardware.PumpIsOn())
      {
        _Hardware.SetPumpOn();
      }
    }
    else
    {
      if (_Hardware.PumpIsOn())
      {
        _Hardware.SetPumpOff();
      }
    }
  }

  void DoPumpModeLight()
  {
    if (_SunIsShining)
    {
      if (!_Hardware.PumpIsOn())
      {
        _Hardware.SetPumpOn();
      }
    }
    else
    {
      if (_Hardware.PumpIsOn())
      {
        _Hardware.SetPumpOff();
      }
    }
  }

  void DoPumpModeContinuous()
  {
    if (!_Hardware.PumpIsOn())
    {
      _Hardware.SetPumpOn();
    }
  }

  void DoPumpModeOff()
  {
    if (_Hardware.PumpIsOn())
    {
      _Hardware.SetPumpOff();
    }
  }
public:
  PoolPumpController()
  {
    _LdrDelayTimer = 0;
    _SunIsShining = false;
  }

  void Init()
  {
#ifdef _SERIAL_DEBUG
    // Serial
    Serial.begin(9600);
#endif
    _Hardware.Init();
    _Menu.Init(&_Hardware);
  }

  void Loop()
  {
    _Hardware.Loop();
    _Menu.Loop();
    CheckSunlight();
    if (_Menu.IsInIdleMode())
    {
      switch (_Menu.GetPumpMode()) {
        case PumpModes::Timer:
          DoPumpModeTimer(false);
          break;
        case PumpModes::TimerLight:
          DoPumpModeTimer(true);
          break;
        case PumpModes::Light:
          DoPumpModeLight();
          break;
        case PumpModes::Continuous:
          DoPumpModeContinuous();
          break;
        case PumpModes::Off:
          DoPumpModeOff();
          break;
      }
    }
  }
};
