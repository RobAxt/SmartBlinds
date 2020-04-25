#ifndef Roller_hpp
#define Roller_hpp

#include <SimpleStepper.h>
#include <Ticker.h>

class Roller {
  public:
    Roller(int currPos,  int maxPos, bool direction);
    ~Roller() {};
	
	  void setup();
    void setZero() { _currentPos = _targetPos = 0; };
    void setZero( int currPos ){ _currentPos = currPos; };
    void setMaxPosition() { _maxPos = _currentPos != 0 ? _currentPos : _maxPos; };
    void setMaxPosition( int max) { _maxPos = max; };
    void setDirection(bool direction) { _stepper.setDirection(direction); };

    void moveTo(int percentage);
    void moveUp();
    void moveDown();
    void moveTop();
    void moveBottom();
    void stop();
    bool isMoving();

    int getMaxPosition() { return _maxPos; };
    int getCurrentPosition() { return _currentPos; };
	  int getCurrentPercentage() { return 100 * _currentPos/_maxPos; };

  private:
    int _currentPos;
    int _maxPos;
    int _targetPos;

    SimpleStepper _stepper;
    Ticker _stepperISR;

    static const uint8_t DELTA_MOVE = 2;
    static const uint8_t MOTOR_SPEED_MS = 3;
};

#endif //Roller_hpp