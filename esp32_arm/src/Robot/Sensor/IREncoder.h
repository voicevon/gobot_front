#pragma once


#include "SimpleFOC/common/base_classes/Sensor.h"
#include "SimpleFOC/common/time_utils.h"
#include "stdint.h"
#include "SensorBase.h"

/**
 *  Quadrature mode configuration structure
 */
// enum Quadrature{
//   ON, //!<  Enable quadrature mode CPR = 4xPPR
//   OFF //!<  Disable quadrature mode / CPR = PPR
// };


class IrEncoder :public Sensor, public SensorBase{
    public:
        IrEncoder(uint8_t pinA, uint8_t pinB , uint16_t ppr, uint8_t pinIndex=0);

    void init();
    void enableInterrupts(void(*doB)() = nullptr, void(*doIndex)() = nullptr);
    
    void handleA();
    void handleB();
    void handleIndex();
    
    
    uint8_t pinA; //!< encoder hardware pin A
    uint8_t pinB; //!< encoder hardware pin B
    uint8_t pinIndex;

    Pullup pullup; //!< Configuration parameter internal or external pullups
    // Quadrature quadrature;//!< Configuration parameter enable or disable quadrature mode
    uint16_t cpr;//!< encoder cpr number

    /** get current angle (rad) */
    float getAngle() override;
    /**  get current angular velocity (rad/s) */
    float getVelocity() override;
    /**
     * returns 0 if it does need search for absolute zero
     * 0 - encoder without index 
     * 1 - ecoder with index
     */
    int needsSearch() override;

  private:
    int hasIndex(); //!< function returning 1 if encoder has index pin and 0 if not.

    volatile long pulse_counter;//!< current pulse counter
    volatile long pulse_timestamp;//!< last impulse timestamp in us
    // volatile int A_active; //!< current active states of A channel
    // volatile int B_active; //!< current active states of B channel
    // volatile int I_active; //!< current active states of Index channel
    volatile bool index_found = false; //!< flag stating that the index has been found

    // velocity calculation variables
    float prev_Th, pulse_per_second;
    volatile long prev_pulse_counter, prev_timestamp_us;
};

