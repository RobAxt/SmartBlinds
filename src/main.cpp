#include <Roller.hpp>

Roller roller(0,4076,false);
bool flagDown = true;
bool flag50 = false;
bool flagTop = false;
bool flagBottom = false;
bool flagUp = false;
bool flagStop = false;

void setup() {
 Serial.begin(74880);
 Serial.println();
 roller.setup();
}

void loop() {
  if(flagDown) {
    Serial.print(F("Setting Zero: "));
    roller.setZero();
    Serial.println(roller.getCurrentPosition());
  
    roller.moveDown();
    Serial.println("Moving Down.");
    flagDown = false;
    flag50 = true ;   
  }

  if(flag50) {
    if(!roller.isMoving()){
      Serial.print("Current Position: ");
      Serial.println(roller.getCurrentPosition());

      roller.moveTo(50);
      Serial.println(F("Moving to 50%: "));

      flag50 = false;
      flagBottom = true;
    }
  }

  if(flagTop) {
    if(!roller.isMoving()) {
      Serial.print("Current Position: ");
      Serial.println(roller.getCurrentPosition());

      roller.moveTop();
      Serial.println(F("Moving Top:  "));

      flagTop = false;     
      flagBottom = true;
    }
  }
  
   if(flagBottom) {
    if(!roller.isMoving()) {
      Serial.print("Current Position: ");
      Serial.println(roller.getCurrentPosition());

      roller.moveBottom();
      Serial.println(F("Moving Bottom:  "));

      flagBottom = false;     
      flagUp = true;
    }
  }

   if(flagUp) {
    if(!roller.isMoving()) {
      Serial.print("Current Position: ");
      Serial.println(roller.getCurrentPosition());

      roller.moveUp();
      Serial.println(F("Moving Up:  "));

      flagUp = false;     
      flagStop = true;
    }
  }

  if(flagStop) {
    if(!roller.isMoving()){
      Serial.print(F("Stopped at: "));
      Serial.println(roller.getCurrentPosition());

      roller.stop();
      flagStop = false;
    }
  }
}