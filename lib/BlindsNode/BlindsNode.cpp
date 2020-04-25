#include "BlindsNode.hpp"

HomieSetting<long> maxPosition("maxPosition","Bottom steps number");
HomieSetting<bool> clockwise("clockwise","Direction of rotation");

BlindsNode::BlindsNode(const char* id, const char* name, const char* type) : HomieNode(id,name,type), _blinds(0,0,false) {
  maxPosition.setDefaultValue(4076).setValidator([](long value){return value > 0;});
  clockwise.setDefaultValue(false);
  _executingCommand = false;
  _blinds.setup();
}

void 
BlindsNode::setup() {
    Homie.getLogger() << F("Calling Node Setup...") << endl;
    Homie.getLogger() << F("HomieSettings...") << endl;
    Homie.getLogger() << F("  ◦ maxPosition: ") << maxPosition.get() << endl;
    Homie.getLogger() << F("  ◦ clockwise: ") << (clockwise.get()? "clockwise" : "counterclockwise") << endl;
    
    _blinds.setMaxPosition((unsigned int) maxPosition.get());
    _blinds.setDirection(clockwise.get());

    setRunLoopDisconnected(true);
    advertise("command").setName("Commands")
                   .setDatatype("string")
                   .setFormat("top,up,stop,down,bottom")
                   .settable([this](const HomieRange& range, const String& value) {
                              return blindsCommandHandler(value);
                             });
    advertise("percentage").setName("Percentage")
                   .setDatatype("integer")
                   .setFormat("0-100")
                   .settable([this](const HomieRange& range, const String& value) {
                              return blindsPercentageHandler(value.toInt());
                             });
    advertise("step").setName("Step")
                     .setDatatype("integer");
    advertise("calibration").setName("Calibration")
                   .setDatatype("string")
                   .setFormat("top,stop,bottom")
                   .settable([this](const HomieRange& range, const String& value) {
                              return blindsCalibrationHandler(value);
                             });
}

void 
BlindsNode::loop() {
    if(_executingCommand == true ) {
        if(_blinds.isMoving()) { // If Moving Executing Command
            if(millis() >= 2000 + _lastUpdate) {
                Homie.getLogger() << F(" ⏳ Executing command.") << endl;
                setProperty("percentage").overwriteSetter(false).send((String) _blinds.getCurrentPercentage());
                setProperty("step").send((String) _blinds.getCurrentPosition());
               _lastUpdate = millis();
            }
        } else { // If Not Moving Command Finished Execution
            Homie.getLogger() << F(" ✋ Command Finished Execution.") << endl;
            _blinds.stop();
            _executingCommand = false;
            setProperty("percentage").overwriteSetter(false).send((String) _blinds.getCurrentPercentage());
            setProperty("step").send((String) _blinds.getCurrentPosition());
            setProperty("command").overwriteSetter(false).send("stop");
            setProperty("calibration").overwriteSetter(false).send("stop");
        }
    }
}

void 
BlindsNode::onReadyToOperate() {
    Homie.getLogger() << F("Calling Ready To Operate... ") << endl;
    Homie.getLogger() << F("  ◦ Node Name: ") << getName() << endl;
    Homie.getLogger() << F("  ◦ Current Position: ") << _blinds.getCurrentPosition() << endl;
    Homie.getLogger() << F("  ◦ Max Position: ") << _blinds.getMaxPosition() << endl;
    Homie.getLogger() << F("  ◦ Current Percentage: ") << _blinds.getCurrentPercentage() << endl;
    
    setProperty("percentage").overwriteSetter(false).send((String) _blinds.getCurrentPercentage());
    setProperty("step").send((String) _blinds.getCurrentPosition());
    setProperty("command").overwriteSetter(false).send("stop");
    setProperty("calibration").overwriteSetter(false).send("stop");
}

bool 
BlindsNode::handleInput(const HomieRange& range, const String& property, const String& value) {
    Homie.getLogger() << F("Calling Node Handle Input...") << endl;
    if(property != "command" && property != "percentage" && property != "calibration") {
        Homie.getLogger() << F("  ◦ Error: property not handle: ") << property << endl; 
        return true;
    }
    if(property == "command" && value != "up" && value != "down" && 
       value != "top" && value != "bottom" && value != "stop") {
        Homie.getLogger() << F("  ◦ Error: wrong value for command property: ") << value << endl; 
        return true;
    }
    if(property == "calibration" && value != "top" && value != "bottom") {
        Homie.getLogger() << F("  ◦ Error: wrong value for calibration property: ") << value << endl; 
        return true;
    }
    if(property == "percentage" && value.toInt() < 0 && value.toInt() > 100) {
        Homie.getLogger() << F("  ◦ Error: wrong value for percentage property: ") << value << endl; 
        return true;
    }
    Homie.getLogger() << F("  ◦ Receive Property/Value: ") << property  << F(" ━► ") << value << endl;
    return false;
}

bool 
BlindsNode::blindsCommandHandler(const String& value) {
    Homie.getLogger() << F("  ◦ Command Handler Recieved ━► ") << value << endl;
    
    if(_blinds.isMoving())
        _blinds.stop();

    for(int i = 0; i < 5; i++) {
        if(commands[i].command == value) {
            ((_blinds).*(commands[i].stepperCommand))();
            setProperty("command").overwriteSetter(false).send(value);
            _executingCommand = true;
            return true;
        }
    }

    return false;
}

bool 
BlindsNode::blindsPercentageHandler(const int value) {
    Homie.getLogger() << F("  ◦ Percentage Handler Recieved ━► ") << value << endl;
    
    if(_blinds.isMoving())
        _blinds.stop();
    
    _blinds.moveTo(value);
    _executingCommand = true;
    setProperty("percentage").overwriteSetter(false).send((String) value);
    return true;
}

bool
BlindsNode::blindsCalibrationHandler(const String& value) {
    Homie.getLogger() << F("  ◦ Calibration Handler Recieved ━► ") << value << endl;

    if(_blinds.isMoving())
        _blinds.stop();

    for(int i = 0; i < 3; i++) {
        if(calibrationCommands[i].command == value) {
           ((_blinds).*(calibrationCommands[i].stepperCommand))();
            setProperty("calibration").overwriteSetter(false).send(value);
            _executingCommand = true;
            return true;
        }
    }

    return true;
}