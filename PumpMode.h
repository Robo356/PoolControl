
enum PumpModes : uint8_t
{
  Timer = 0,
  TimerLight,
  Light,
  Continuous,
  Off
};

#define PumpMode_Min PumpModes::Timer
#define PumpMode_Max PumpModes::Off