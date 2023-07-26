#include "Arduino.h"
// LiquidCrystal I2C by Frank de Brabander
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include "TempSensor.h"
#include "PushButton.h"

class Hardware
{
private:
  // Special characters for LCD
  
  const uint8_t gear_1[8] = {
    B00000,
    B11111,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000
  };

  const uint8_t gear_2[8] = {
    B00000,
    B00001,
    B00001,
    B00001,
    B00001,
    B00001,
    B00000,
    B00000
  };

  const uint8_t gear_3[8] = {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B11111,
    B00000,
    B00000
  };

  const uint8_t gear_4[8] = {
    B00000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B00000,
    B00000
  };

  const uint8_t icon_sun[8] = {
    B00000,
    B00100,
    B01110,
    B11111,
    B01110,
    B00100,
    B00000,
    B00000
  };

  const uint8_t PumpRelayPort = 3;
  const uint8_t LdrPort = A2;
  const uint8_t TempSensorPoolPort = A0;
  const uint8_t TempSensorHeaterPort = A1;
  const uint8_t ButtonUpPort = PB0;
  const uint8_t ButtonDownPort = PB1;
  const uint8_t ButtonSelectPort = PB2;

  // Tresholds
  const unsigned int LdrTreshold = 900;
public:
  const char LcdGearIcons[4] = {0, 1, 2, 3};
  const uint8_t LcdGearIconsMax = 4;
  const char LcdSunIcon = 4;
  const char LcdSpaceChar = 32;

  LiquidCrystal_I2C Lcd;
  RTC_DS1307 RTC;
  PushButton ButtonUp;
  PushButton ButtonDown;
  PushButton ButtonSelect;
  TempSensor_MJSTS_103 TempSensorPool;
  TempSensor_MJSTS_103 TempSensorHeater;

  Hardware() :
    Lcd(0x27, 16, 2),
    TempSensorPool(TempSensorPoolPort),
    TempSensorHeater(TempSensorHeaterPort),
    ButtonUp(ButtonUpPort),
    ButtonDown(ButtonDownPort),
    ButtonSelect(ButtonSelectPort)
  {
  }

  void Init()
  {
  
    // Initialize the LCD
    Lcd.init();
    Lcd.backlight();
    Lcd.createChar(LcdGearIcons[0], gear_1);
    Lcd.createChar(LcdGearIcons[1], gear_2);
    Lcd.createChar(LcdGearIcons[2], gear_3);
    Lcd.createChar(LcdGearIcons[3], gear_4);
    Lcd.createChar(LcdSunIcon, icon_sun);
    // Initialize the RTC
    RTC.begin();
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // Buttons
    ButtonUp.Init();
    ButtonDown.Init();
    ButtonSelect.Init();
    
    TempSensorPool.Init();
    TempSensorHeater.Init();
    pinMode(PumpRelayPort, OUTPUT);
    pinMode(LdrPort, INPUT);
  }

  void Loop()
  {
    // Buttons
    ButtonUp.Loop();
    ButtonDown.Loop();
    ButtonSelect.Loop();
  }

  bool LdrIsOn() const
  {
    return analogRead(LdrPort) >= LdrTreshold;
  }

  int LdrRawValue() const
  {
    return analogRead(LdrPort);
  }

  bool PumpIsOn() const
  {
    return digitalRead(PumpRelayPort) == HIGH;
  }

  void SetPumpOn()
  {
#ifdef _SERIAL_DEBUG
    Serial.println("Pump switched on");
#endif
    digitalWrite(PumpRelayPort, HIGH);
  }

  void SetPumpOff()
  {
#ifdef _SERIAL_DEBUG
    Serial.println("Pump switched off");
#endif
    digitalWrite(PumpRelayPort, LOW);
  }
};