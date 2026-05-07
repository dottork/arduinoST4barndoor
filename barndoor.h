#ifndef barndoor_h
#define barndoor_h

#include <Arduino.h>
#include "AccelStepper.h" // To interface with stepper

// Define stepper motor connections and motor interface type. 
// Motor interface type must be set to 1 when using a driver
#define motorInterfaceType 1
#define dirPin 12
#define stepPin 14

// Define Pin for end of travel, (when is fully closed)
#define endofPin 13

// Barndoor geometrical data
#define N 3200.0
#define X0 13.5
#define alfa 8.0
#define L 153.45
//Tracking speed
#define SIDERAL 3.646E-5
#define RAP 1.25*3.646E-8
#define RAM 0.75*3.646E-8
#define ANG0 8.809E-2


class Barndoor {
  private:
    long currentStep;
    long targetStep;
    float correctionFactor;
    unsigned long time0;
    int counter;
    AccelStepper myStepper;
    int direction;
    byte modo;
  public:
    Barndoor();
    void setCorrectionFactor(float c);
    float calcSpeed(long position);
    void setDirection(int dire);
    void setSpeed(int speed);
    void stop();
    long getCurrentStep();
    void goToZero();
    void fastGoto(long target);
    void update();
    void startTrack();
    void stopTracking();
};

#endif
