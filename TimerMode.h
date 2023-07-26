
enum TimerModes : uint8_t
{
  Timer2h = 0,
  Timer4h,
  Timer8h,
  Timer12h
};

#define TimerMode_Min TimerModes::Timer2h
#define TimerMode_Max TimerModes::Timer12h