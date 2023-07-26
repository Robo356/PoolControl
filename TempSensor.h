#include "Arduino.h"
class TempSensor
{
private:
  uint8_t _Port;
protected:
  virtual float GetThermistorA() const = 0;
  virtual float GetThermistorB() const = 0;
  virtual float GetThermistorC() const = 0;
  virtual float GetR2() const = 0;
public:
  TempSensor(uint8_t port)
  {
    _Port = port;
  }

  void Init()
  {
    pinMode(_Port, INPUT);
  }

  int GetAnalogValue() const
  {
    return analogRead(_Port);
  }

  float GetTemperature() const
  {
    int sensorValue = analogRead(_Port);
    float resistance = (float)(1023 - sensorValue) * GetR2() / sensorValue;
    // Steinhart and Hart Equation
    return (1 / (GetThermistorA() + GetThermistorB() * log(resistance) + GetThermistorC() * pow(log(resistance), 3))) - 273.15;  // Kelvin -> C°
  }

  int GetRawValue() const
  {
    return analogRead(_Port);
  }
};

class TempSensor_MJSTS_103 : public TempSensor
{
private:
  // TRU COMPONENTS MJSTS-103-3950-1-600-3D
  // Steinhart-Hart Equation with values at 0°, 20° and 40°
  const float ThermistorA = 0.0010884816053190823;
  const float ThermistorB = 0.00024128349722150576;
  const float ThermistorC = 0.000000055189600461676735;
  // Black / Brown / Orange
  const float R2 = 10000;
protected:
  virtual float GetThermistorA() const
  {
    return ThermistorA;
  }

  virtual float GetThermistorB() const
  {
    return ThermistorB;
  }

  virtual float GetThermistorC() const
  {
    return ThermistorC;
  }

  virtual float GetR2() const
  {
    return R2;
  }
public:
  TempSensor_MJSTS_103(uint8_t port) :
    TempSensor(port)
  {
  }

};

class TempSensor_HYG_NTC_203 : public TempSensor
{
private:
  // HYG NTC 203
  const float ThermistorA = 0.0010128721995810298;
  const float ThermistorB = 0.00022534436829268063;
  const float ThermistorC = 0.00000011266180268721773;
  // Red / Red / Orange
  const float R2 = 22000;
protected:
  virtual float GetThermistorA() const
  {
    return ThermistorA;
  }

  virtual float GetThermistorB() const
  {
    return ThermistorB;
  }

  virtual float GetThermistorC() const
  {
    return ThermistorC;
  }

  virtual float GetR2() const
  {
    return R2;
  }
public:
  TempSensor_HYG_NTC_203(uint8_t port) :
    TempSensor(port)
  {
  }

};

