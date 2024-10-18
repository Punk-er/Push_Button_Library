class Button {
public:
  unsigned long int firstTime = 0, lastTime = 0;
  bool stat = false, firstpress = true, stillOnHold = false, Rising = true;
  int tapcounter = 0, holdTime = 800, tapTime = 20, tapGap = 400, pin;
  void (*callback_hold)(int), (*callback_tap)(int);
  Button(int pin, bool Rising) {
    this->pin = pin;
    this->Rising = Rising;
    if (Rising){
      stat = false;
    }else
    {
      stat = true;

    }
  }
  // Hold Event(Hold time)  Tap Event(Tap count)
  void SetEvent(void (*callback_hold1)(int), void (*callback_tap1)(int)) {
    callback_hold = callback_hold1;
    callback_tap = callback_tap1;
  }
  // should repeat in a loop all the time in the main loop
  void Listen() {
    if (Rising) {
      if (stat != digitalRead(pin)) {
        stat = digitalRead(pin);
        if (stat)
          firstTime = millis();
        else {
          if (millis() - firstTime > tapTime && millis() - firstTime < holdTime) {
            tapcounter++;
            lastTime = millis();
          }
          if (stillOnHold && millis() - firstTime >= holdTime) {
            stillOnHold = false;
          }
        }
      } else {
        if (!stillOnHold && stat && millis() - firstTime >= holdTime) {
          callback_hold(millis() - firstTime);
          stillOnHold = true;
        }
        if (tapcounter != 0 and millis() - lastTime > tapGap) {
          callback_tap(tapcounter);
          tapcounter = 0;
        }
      }
    } else {
      if (stat != digitalRead(pin)) {
        stat = digitalRead(pin);
        if (!stat)
          firstTime = millis();
        else {
          if (millis() - firstTime > tapTime && millis() - firstTime < holdTime) {
            tapcounter++;
            lastTime = millis();
          }
          if (stillOnHold && millis() - firstTime >= holdTime) {
            stillOnHold = false;
          }
        }
      } else {
        if (!stillOnHold && !stat && millis() - firstTime >= holdTime) {
          callback_hold(millis() - firstTime);
          stillOnHold = true;
        }
        if (tapcounter != 0 and millis() - lastTime > tapGap) {
          callback_tap(tapcounter);
          tapcounter = 0;
        }
      }
    }
  }
};