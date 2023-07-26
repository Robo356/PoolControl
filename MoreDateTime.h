class MoreDateTime : public DateTime
{
public:
  MoreDateTime() : DateTime()
  {

  }

  MoreDateTime(const DateTime &copy) : DateTime(copy)
  {

  }

  void IncYear()
  {
    this->yOff ++;
  }

  void DecYear()
  {
    this->yOff --;
  }

  void IncMonth()
  {
    if (this->m >= 12)
      this->m = 1;
    else
      this->m ++;
  }

  void DecMonth()
  {
    if (this->m <= 1)
      this->m = 12;
    else
      this->m --;
  }

  void IncDay()
  {
    if (this->d >= 31)
      this->d = 1;
    else
      this->d ++;
  }

  void DecDay()
  {
    if (this->d <= 1)
      this->d = 31;
    else
      this->d --;
  }

  void IncHour()
  {
    if (this->hh >= 23)
      this->hh = 0;
    else
      this->hh ++;
  }

  void DecHour()
  {
    if (this->hh == 0)
      this->hh = 23;
    else
      this->hh --;
  }

  void IncMinute()
  {
    if (this->mm >= 59)
      this->mm = 0;
    else
      this->mm ++;
  }

  void DecMinute()
  {
    if (this->mm == 0)
      this->mm = 59;
    else
      this->mm --;
  }
};