#include "Roller.hpp"

Roller::Roller(int currPos, int maxPos, bool direction) : _currentPos(currPos), _maxPos(maxPos), _stepper(SimpleStepper::FULLSTEP, direction, D5, D6, D7, D8) { 
}

void Roller::setup() {
  //_stepperISR.attach_ms(MOTOR_SPEED_MS,std::bind(&SimpleStepper::step, &_stepper));
  _stepperISR.attach_ms(MOTOR_SPEED_MS,[this](){
                                                 _stepper.step(); 
                                                 _currentPos = _targetPos - _stepper.getStep();
                                               });
}

void 
Roller::moveTo(int percentage) {  
  if(percentage < 0 || percentage > 100)
    return;

  if(_stepper.ready()) {
	_targetPos = _maxPos * percentage/100;
    _stepper.setStep(_targetPos - _currentPos);
  }
}

void 
Roller::moveUp(){
  if(_stepper.ready()) {
   // if(_currentPos <= (unsigned int) DELTA_MOVE*_stepper.getFullRotationSteps()) {
   //   _stepper.setStep(-_currentPos);
   //   _targetPos = 0;
   // } else {
      _stepper.setStep(-DELTA_MOVE*_stepper.getFullRotationSteps());
      _targetPos = _currentPos - DELTA_MOVE*_stepper.getFullRotationSteps();
   // }
  }
}

void 
Roller::moveDown(){
  if(_stepper.ready()) {
    _stepper.setStep(DELTA_MOVE*_stepper.getFullRotationSteps());
    _targetPos = _currentPos + DELTA_MOVE*_stepper.getFullRotationSteps();
  }
}

void 
Roller::moveTop() {
  if(_stepper.ready()) {
    _stepper.setStep(- _currentPos);
    _targetPos = 0;
  }
}

void 
Roller::moveBottom() {
  if(_stepper.ready()) {
    _stepper.setStep(_maxPos - _currentPos);
    _targetPos = _maxPos;
  }
}

void 
Roller::stop(){
  if(!_stepper.ready()) { 
    _currentPos = _targetPos - _stepper.getStep();
	  _stepper.setStep(0);
    _targetPos = _currentPos;
  }	
  _stepper.offCoils();
}

bool 
Roller::isMoving() {
  return !_stepper.ready();
}