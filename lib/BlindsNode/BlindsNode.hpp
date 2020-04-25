#ifndef BlindsNode_hpp
#define BlindsNode_hpp

#include <Homie.hpp>
#include "Roller.hpp"

class BlindsNode : public HomieNode {
    public:
        BlindsNode(const char* id, const char* name, const char* type);

    protected:
        virtual void setup() override;
        virtual void loop() override;
        virtual void onReadyToOperate() override;
        virtual bool handleInput(const HomieRange& range, const String& property, const String& value);

    private:
        Roller _blinds;
        unsigned long _lastUpdate;
        bool _executingCommand;
        bool blindsCommandHandler(const String& value);
        bool blindsPercentageHandler(const int value);
        bool blindsCalibrationHandler(const String& value);
        typedef struct {
            String command;
            void (Roller::*stepperCommand)(void);
        } StepperCommands;
        StepperCommands commands[5] = { { "top",    &Roller::moveTop    },
                                        { "bottom", &Roller::moveBottom },
                                        { "up",     &Roller::moveUp     },
                                        { "down",   &Roller::moveDown   },
                                        { "stop",   &Roller::stop       }
                                      };
        StepperCommands calibrationCommands[3] = { { "top",    &Roller::setZero        },
                                                   { "bottom", &Roller::setMaxPosition },
                                                   { "stop",   &Roller::stop           }
                                                 };};
#endif //BlindsNode_hpp