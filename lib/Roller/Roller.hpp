#ifndef Roller_hpp
#define Roller_hpp

#include <SimpleStepper.h>
#include <Ticker.h>

class Roller {
  public:
    Roller(unsigned int currPos, unsigned int maxPos, bool direction);
    ~Roller() {};
	
	  void setup();

    void setZero(){_currentPos = 0;};
    void setMaxPosition(){_maxPos =_currentPos;};
    void setMaxPosition(unsigned int max) {_maxPos = max;};

    void moveTo(unsigned int percentage);
    void moveUp();
    void moveDown();
    void moveTop();
    void moveBottom();
    void stop();
    bool isMoving();

    unsigned int getMaxPosition() {return _maxPos;};
    unsigned int getCurrentPosition() {return _currentPos;};
	  unsigned int getCurrentPercentage() {return 100 * _currentPos/_maxPos;};

  private:
    unsigned int _currentPos;
    unsigned int _maxPos;
    unsigned int _targetPos;

    SimpleStepper _stepper;
    Ticker _stepperISR;

    static const uint8_t DELTA_MOVE = 2;
    static const uint8_t MOTOR_SPEED_MS = 2;
};

#endif