
enum TimerMode : uint8_t
{
  Timer2h = 0,
  Timer4h,
  Timer8h,
  Timer12h
};

#define TimerMode_Max TimerMode::Timer12h