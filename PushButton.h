class PushButton
{
private:
  uint8_t _State;
  uint8_t _Port;
  uint16_t _PushedCount;
public:
  PushButton(uint8_t port)
  {
    _Port = port;
    _PushedCount = 0;
    _State = LOW;
  }

  void Init()
  {
    pinMode(_Port, INPUT);
    _State = digitalRead(_Port);
  }

  void Loop()
  {
    uint8_t value = digitalRead(_Port);
    if (_State == HIGH && value == LOW)
      _PushedCount++;
    _State = value;
  }

  bool IsPushed()
  {
    bool result = _PushedCount > 0;
    if (_PushedCount > 0) _PushedCount--;
    return result;
  }

};