class TempSensor
{
private:
  const float ThermistorA = 1;
  const float ThermistorB = 1;
  const float ThermistorC = 1;
  int _Port;
public:
  TempSensor(int port)
  {
    _Port = port;
  }

  void Init()
  {
    pinMode(_Port, INPUT);
  }

  float GetTemperature()
  {
    int sensorValue = analogRead(_Port);  // 0 -> 1023 value
    float resistance = (float)(1023 - sensorValue) * 10000 / sensorValue;
    // Steinhart and Hart Equation
    return (1 / (ThermistorA + ThermistorB * log(resistance) + ThermistorC * pow(log(resistance), 3))) - 273.15;  // Kelvin -> C°
  }
};