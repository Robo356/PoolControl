#include "Arduino.h"
#include "Hardware.h"
#include "MoreDateTime.h"
#include "PumpMode.h"
#include "TimerMode.h"

class Menu {
private:
  enum MenuState : uint8_t
  {
    State,
    PumpMode,
    PumpModeSelect,
    Timer,
    TimerSelect,
    Time,
    TimeSelectYear,
    TimeSelectMonth,
    TimeSelectDay,
    TimeSelectHour,
    TimeSelectMinute
  };

  enum StatusDisplayState : uint8_t
  {
    StatusDisplayStateMode,
    StatusDisplayStateTemperature,
    StatusDisplayStateTime,
    StatusDisplayStateAnalog
  };

#ifdef DISPLAY_ANALOG_VALUES
  const StatusDisplayState StatusDisplayStateEnd = StatusDisplayStateAnalog;
#else
  const StatusDisplayState StatusDisplayStateEnd = StatusDisplayStateTime;
#endif

  const unsigned long StatusDisplayStateDelay = 3000;
  const unsigned long GearIconDelay = 500;
  const unsigned long MessageDelay = 1000;
  
  Hardware* _Hardware;
  MenuState _MenuState;
  // DateTime for adjustment
  MoreDateTime _DateTime;
  PumpModes _PumpMode;
  TimerModes _TimerMode;
  char _StringBuffer[15];
  StatusDisplayState _StatusDisplayState;
  unsigned long _StatusDisplayStateTimer;
  uint8_t _GearIcon;
  unsigned long _GearIconTimer;
  unsigned long _MessageTimer;
  float _PoolTemp;
  float _HeaterTemp;

  void DisplayMessage(const char* message)
  {
    _MessageTimer = millis();
    _Hardware->Lcd.clear();
    _Hardware->Lcd.setCursor(0, 0);
    _Hardware->Lcd.print(message);      
  }

  void DisplayStatus()
  {
    // Message
    if (_MessageTimer > 0)
    {
      if (millis() - _MessageTimer > MessageDelay)
        _MessageTimer = 0;
      else
        return;
    }
    // Set state
    if (millis() - _StatusDisplayStateTimer > StatusDisplayStateDelay)
    {
      _StatusDisplayState = _StatusDisplayState + 1;
      if (_StatusDisplayState > StatusDisplayStateEnd)
        _StatusDisplayState = StatusDisplayState::StatusDisplayStateMode;
      _StatusDisplayStateTimer = millis();

      if (_StatusDisplayState == StatusDisplayState::StatusDisplayStateTemperature)
      {
        _PoolTemp = _Hardware->TempSensorPool.GetTemperature();
        _HeaterTemp = _Hardware->TempSensorHeater.GetTemperature();
      }
      _Hardware->Lcd.clear();
    }
    if (_StatusDisplayState == StatusDisplayState::StatusDisplayStateMode)
    {
      _Hardware->Lcd.setCursor(0, 0);
      switch (_PumpMode) {
        case PumpModes::Timer:
          _Hardware->Lcd.print("Timer");
          _Hardware->Lcd.setCursor(0, 1);
          switch (_TimerMode) {
            case TimerModes::Timer2h:
              _Hardware->Lcd.print("2 hours");
              break;
            case TimerModes::Timer4h:
              _Hardware->Lcd.print("4 hours");
              break;
            case TimerModes::Timer8h:
              _Hardware->Lcd.print("8 hours");
              break;
            case TimerModes::Timer12h:
              _Hardware->Lcd.print("12 hours");
              break;
          }
          break;
        case PumpModes::TimerLight:
          _Hardware->Lcd.print("Timer/Sunlight");
          _Hardware->Lcd.setCursor(0, 1);
          switch (_TimerMode) {
            case TimerModes::Timer2h:
              _Hardware->Lcd.print("2 hours");
              break;
            case TimerModes::Timer4h:
              _Hardware->Lcd.print("4 hours");
              break;
            case TimerModes::Timer8h:
              _Hardware->Lcd.print("8 hours");
              break;
            case TimerModes::Timer12h:
              _Hardware->Lcd.print("12 hours");
              break;
          }
          break;
        case PumpModes::Light:
          _Hardware->Lcd.print("Sunlight");
          break;
        case PumpModes::Continuous:
          _Hardware->Lcd.print("Continous");
          break;
        case PumpModes::Off:
          _Hardware->Lcd.print("Pump Off");
          break;
      }
    }
    else if (_StatusDisplayState == StatusDisplayState::StatusDisplayStateTemperature)
    {
      _Hardware->Lcd.setCursor(0, 0);
      _Hardware->Lcd.print("Pool   ");
      _Hardware->Lcd.print(_PoolTemp, 1);
      _Hardware->Lcd.setCursor(0, 1);
      _Hardware->Lcd.print("Heater ");
      _Hardware->Lcd.print(_HeaterTemp, 1);
    }
    else if (_StatusDisplayState == StatusDisplayState::StatusDisplayStateTime)
    {
      DateTime dateTime = _Hardware->RTC.now();
      _Hardware->Lcd.setCursor(0, 0);
      strcpy(_StringBuffer, "DD/MM/YYYY");
      _Hardware->Lcd.print(dateTime.toString(_StringBuffer));
      _Hardware->Lcd.setCursor(0, 1);
      strcpy(_StringBuffer, "hh:mm:ss");
      _Hardware->Lcd.print(dateTime.toString(_StringBuffer));
    }
    else if (_StatusDisplayState == StatusDisplayState::StatusDisplayStateAnalog)
    {
      _Hardware->Lcd.setCursor(0, 0);
      _Hardware->Lcd.print(_Hardware->TempSensorPool.GetAnalogValue());
      _Hardware->Lcd.print(" / ");
      _Hardware->Lcd.print(_Hardware->TempSensorHeater.GetAnalogValue());
      _Hardware->Lcd.setCursor(0, 1);
      _Hardware->Lcd.print(_Hardware->LdrRawValue());
    }
    // Status icons
    _Hardware->Lcd.setCursor(15, 0);
    if (_Hardware->PumpIsOn())
    {
        _Hardware->Lcd.write(_Hardware->LcdGearIcons[_GearIcon]);
        if (millis() - _GearIconTimer > GearIconDelay)
        {
          _GearIcon ++;
          if (_GearIcon >= _Hardware->LcdGearIconsMax)
          _GearIcon = 0;
        _GearIconTimer = millis();
        }
    }
    else
    {
      _Hardware->Lcd.write(_Hardware->LcdSpaceChar);
    }
    _Hardware->Lcd.setCursor(15, 1);
    if (_Hardware->LdrIsOn())
    {
        _Hardware->Lcd.write(_Hardware->LcdSunIcon);         
    }
    else
    {
      _Hardware->Lcd.write(_Hardware->LcdSpaceChar);
    }
  }

  void DisplayPumpMode()
  {
    _Hardware->Lcd.setCursor(0, 0);
    _Hardware->Lcd.print("Pump mode");
  }

  void DisplayPumpModeSelect()
  {
    _Hardware->Lcd.setCursor(0, 0);
    switch (_PumpMode) {
      case PumpModes::Timer:
        _Hardware->Lcd.print("Timer");
        break;
      case PumpModes::TimerLight:
        _Hardware->Lcd.print("Timer/Sunlight");
        break;
      case PumpModes::Light:
        _Hardware->Lcd.print("Light");
        break;
      case PumpModes::Continuous:
        _Hardware->Lcd.print("Continous");
        break;
      case PumpModes::Off:
        _Hardware->Lcd.print("Off");
        break;
    }
  }

  void DisplayTimer()
  {
    _Hardware->Lcd.setCursor(0, 0);
    _Hardware->Lcd.print("Timer mode");
  }

  void DisplayTimerSelect()
  {
    _Hardware->Lcd.setCursor(0, 0);
    switch (_TimerMode) {
      case TimerModes::Timer2h:
        _Hardware->Lcd.print("2 hours");
        break;
      case TimerModes::Timer4h:
        _Hardware->Lcd.print("4 hours");
        break;
      case TimerModes::Timer8h:
        _Hardware->Lcd.print("8 hours");
        break;
      case TimerModes::Timer12h:
        _Hardware->Lcd.print("12 hours");
        break;
    }
  }

  void DisplayTime()
  {
    _Hardware->Lcd.setCursor(0, 0);
    _Hardware->Lcd.print("Date/Time");
  }

  void DisplayTimeSelectYear()
  {
    _Hardware->Lcd.setCursor(0, 0);
    _Hardware->Lcd.print("Year ");
    strcpy(_StringBuffer, "YYYY");
    _Hardware->Lcd.print( _DateTime.toString(_StringBuffer));
  }

  void DisplayTimeSelectMonth()
  {
    _Hardware->Lcd.setCursor(0, 0);
    _Hardware->Lcd.print("Month ");
    sprintf(_StringBuffer, "%02d", _DateTime.month());
    _Hardware->Lcd.print(_StringBuffer);
  }

  void DisplayTimeSelectDay()
  {
    _Hardware->Lcd.setCursor(0, 0);
    _Hardware->Lcd.print("Day ");
    sprintf(_StringBuffer, "%02d", _DateTime.day());
    _Hardware->Lcd.print(_StringBuffer);
  }

  void DisplayTimeSelectHour()
  {
    _Hardware->Lcd.setCursor(0, 0);
    _Hardware->Lcd.print("Hour ");
    sprintf(_StringBuffer, "%02d", _DateTime.hour());
    _Hardware->Lcd.print(_StringBuffer);
  }

  void DisplayTimeSelectMinute()
  {
    _Hardware->Lcd.setCursor(0, 0);
    _Hardware->Lcd.print("Minute ");
    sprintf(_StringBuffer, "%02d", _DateTime.minute());
    _Hardware->Lcd.print(_StringBuffer);
  }

public:
  void ButtonUpPushed()
  {
#ifdef _SERIAL_DEBUG
    Serial.println("up");
#endif
    _Hardware->Lcd.clear();
    switch (_MenuState) {
      // Main menu
      case MenuState::State:
        _MenuState = MenuState::Time;
        break;
      case MenuState::PumpMode:
        _MenuState = MenuState::State;
        break;
      case MenuState::Timer:
        _MenuState = MenuState::PumpMode;
        break;
      case MenuState::Time:
        _MenuState = MenuState::Timer;
        break;
      // Select menu
      case MenuState::PumpModeSelect:
        if (_PumpMode == PumpMode_Min) _PumpMode = PumpMode_Max;
        else _PumpMode = _PumpMode-1;
        break;
      case MenuState::TimerSelect:
        if (_TimerMode == TimerMode_Min) _TimerMode = TimerMode_Max;
        else _TimerMode = _TimerMode-1;
        break;
      case MenuState::TimeSelectYear:
        _DateTime.IncYear();
        break;
      case MenuState::TimeSelectMonth:
        _DateTime.IncMonth();
        break;
      case MenuState::TimeSelectDay:
        _DateTime.IncDay();
        break;
      case MenuState::TimeSelectHour:
        _DateTime.IncHour();
        break;
      case MenuState::TimeSelectMinute:
        _DateTime.IncMinute();
        break;
    }
  }

  void ButtonDownPushed()
  {
#ifdef _SERIAL_DEBUG
    Serial.println("down");
#endif
    _Hardware->Lcd.clear();
    switch (_MenuState) {
      // Main menu
      case MenuState::State:
        _MenuState = MenuState::PumpMode;
        break;
      case MenuState::PumpMode:
        _MenuState = MenuState::Timer;
        break;
      case MenuState::Timer:
        _MenuState = MenuState::Time;
        break;
      case MenuState::Time:
        _MenuState = MenuState::State;
        break;
      // Select menu
      case MenuState::PumpModeSelect:
        if (_PumpMode == PumpMode_Max) _PumpMode = PumpMode_Min;
        else _PumpMode = _PumpMode+1;
        break;
      case MenuState::TimerSelect:
        if (_TimerMode == TimerMode_Max) _TimerMode = TimerMode_Min;
        else _TimerMode = _TimerMode+1;
        break;
      case MenuState::TimeSelectYear:
        _DateTime.DecYear();
        break;
      case MenuState::TimeSelectMonth:
        _DateTime.DecMonth();
        break;
      case MenuState::TimeSelectDay:
        _DateTime.DecDay();
        break;
      case MenuState::TimeSelectHour:
        _DateTime.DecHour();
        break;
      case MenuState::TimeSelectMinute:
        _DateTime.DecMinute();
        break;
    }
  }

  void ButtonSelectPushed()
  {
#ifdef _SERIAL_DEBUG
    Serial.println("select");
#endif
    _Hardware->Lcd.clear();
    switch (_MenuState) {
      case MenuState::PumpMode:
        _MenuState = MenuState::PumpModeSelect;
        break;
      case MenuState::PumpModeSelect:
        DisplayMessage("Mode set");
        _MenuState = MenuState::State;
        break;
      case MenuState::Timer:
        _MenuState = MenuState::TimerSelect;
        break;
      case MenuState::TimerSelect:
        DisplayMessage("Timer set");
        _MenuState = MenuState::State;
        break;
      case MenuState::Time:
        _DateTime = _Hardware->RTC.now();
        _MenuState = MenuState::TimeSelectYear;
        break;
      case MenuState::TimeSelectYear:
        _MenuState = MenuState::TimeSelectMonth;
        break;
      case MenuState::TimeSelectMonth:
        _MenuState = MenuState::TimeSelectDay;
        break;
      case MenuState::TimeSelectDay:
        _MenuState = MenuState::TimeSelectHour;
        break;
      case MenuState::TimeSelectHour:
        _MenuState = MenuState::TimeSelectMinute;
        break;
      case MenuState::TimeSelectMinute:
        if (_DateTime.isValid())
        {
          _Hardware->RTC.adjust(_DateTime);
          DisplayMessage("Date/Time set");
        }
        else
        {
          DisplayMessage("Invalid Date/Time");
        }
        _MenuState = MenuState::State;
        break;
    }
  }

public:
  Menu()
  {
    _MenuState = MenuState::State;
    _PumpMode = PumpModes::Off;
    _TimerMode = TimerModes::Timer2h;
    _StatusDisplayState = StatusDisplayState::StatusDisplayStateMode;
    _StatusDisplayStateTimer = 0;
    _GearIcon = 0;
    _GearIconTimer = 0;
    _MessageTimer = 0;
  }

  void Init(Hardware* hardware)
  {
    _Hardware = hardware;
    _StatusDisplayStateTimer = millis();
    _GearIconTimer = millis();
  }

  void Loop()
  {
#ifdef _SERIAL_DEBUG    
    if (Serial.available())
    {
      switch (Serial.read())
      {
        case 's':
          ButtonSelectPushed();
          break;
        case 'q':
          ButtonDownPushed();
          break;
        case 'd':
          ButtonUpPushed();
          break;
        default:
          Serial.println("unknown command");
          break;
      }
    }
#else
    if (_Hardware->ButtonUp.IsPushed())
    {
      ButtonUpPushed();
    }
    if (_Hardware->ButtonDown.IsPushed())
    {
      ButtonDownPushed();
    }
    if (_Hardware->ButtonSelect.IsPushed())
    {
      ButtonSelectPushed();
    }
#endif
    switch (_MenuState)
    {
      case MenuState::State:
        DisplayStatus();
        break;
      case MenuState::PumpMode:
        DisplayPumpMode();
        break;
      case MenuState::PumpModeSelect:
        DisplayPumpModeSelect();
        break;
      case MenuState::Timer:
        DisplayTimer();
        break;
      case MenuState::TimerSelect:
        DisplayTimerSelect();
        break;
      case MenuState::Time:
        DisplayTime();
        break;
      case MenuState::TimeSelectYear:
        DisplayTimeSelectYear();
        break;
      case MenuState::TimeSelectMonth:
        DisplayTimeSelectMonth();
        break;
      case MenuState::TimeSelectDay:
        DisplayTimeSelectDay();
        break;
      case MenuState::TimeSelectHour:
        DisplayTimeSelectHour();
        break;
      case MenuState::TimeSelectMinute:
        DisplayTimeSelectMinute();
        break;
    }
  }

  bool IsInIdleMode() const
  {
    return _MenuState == MenuState::State;    
  }

  PumpModes GetPumpMode() const
  {
    return _PumpMode;    
  }

  TimerModes GetTimerMode() const
  {
    return _TimerMode;
  }
};

