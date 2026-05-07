#include "barndoor.h"
//  ------------------------------------------
// Main Constructor
//  ------------------------------------------
Barndoor::Barndoor() 
  :myStepper(motorInterfaceType, stepPin, dirPin) {
    counter=0;
    modo=0;
    correctionFactor=1.0;
    this->myStepper.setMaxSpeed(6400);
    pinMode(endofPin, INPUT_PULLUP);
    }
//  ------------------------------------------
// Set speed correction factor
//  ------------------------------------------
void Barndoor::setCorrectionFactor(float c){
  this->correctionFactor=c;    
  }
//  ------------------------------------------
// Calculate the needed speed in a specific point
//  ------------------------------------------    
float Barndoor::calcSpeed(long position){
  float pos=(float)position/3200*8+X0;
  float a=sq(pos/(2.0*L));
  float b=sqrt(1-a);
  float c=SIDERAL*this->correctionFactor*L*b;
  float d=c*N/alfa;
  Serial.print(position);
  Serial.print(" ");
  Serial.print(pos);
  Serial.print(" ");
  Serial.print(a,8);
  Serial.print(" ");
  Serial.print(b,8);
  Serial.print(" ");
  Serial.print(c,8);
  Serial.print(" ");
  Serial.println(d,8);
  return(d);
  }
//  ------------------------------------------
// Set motion direction
//  ------------------------------------------
void Barndoor::setDirection(int dire){
  this->direction=dire;
  }
//  ------------------------------------------
// Set motion speed
//  ------------------------------------------
void Barndoor::setSpeed(int speed){
  this->myStepper.setSpeed(speed);
  }
//  ------------------------------------------
// Stop
//  ------------------------------------------ 
void Barndoor::stop(){
  this->direction=0;
  this->myStepper.stop();
  }
//  ------------------------------------------ 
// Get current step
//  ------------------------------------------ 
long Barndoor::getCurrentStep(){
      return(this->myStepper.currentPosition());
    }
//  ------------------------------------------ 
// Reset position to close
//  ------------------------------------------   
void Barndoor::goToZero(){
      this->myStepper.moveTo(-10000);
      this->setSpeed(-2000);
      Serial.println("Inizia a chiudere");
      while(digitalRead(endofPin)){
        this->myStepper.runSpeedToPosition();
        //Serial.println(this->myStepper.currentPosition());
      }
      Serial.println("Chiuso a zero");
      this->setSpeed(0);
      this->myStepper.stop();
      this->direction=0;
      this->myStepper.setCurrentPosition(0);
    }
//  ------------------------------------------ 
// Go fast to a position
//  ------------------------------------------ 
void Barndoor::fastGoto(long target){            
      float sped;
      if (this->myStepper.currentPosition() > target){
        sped=-6400;
        Serial.println("chiude");
      }
      else {
        sped=6400;
        Serial.println("apre");
      }
      this->myStepper.moveTo(target);
      this->myStepper.setSpeed(sped);
      this->modo=2;
      }
//  ------------------------------------------ 
// Update
//  ------------------------------------------
void Barndoor::update(){
  switch(modo){
    case 0:
      //if ((this->direction != 0 ) && (this->currentStep >= 0) && digitalRead(7)) {
      //this->myStepper.step(this->direction);
      //this->currentStep=this->currentStep+this->direction;
      //if ((this->currentStep>MAXSTEPS) || (this->currentStep==0) || !digitalRead(7)) {
      //  this->stop();
      //((}
      //}
      break;
    case 1:             // Track
      if (this->counter==10){
        counter=0;
        float rr=calcSpeed(this->myStepper.currentPosition());
        this->myStepper.setSpeed(rr);
        this->myStepper.runSpeed();
        //Serial.print(millis());
        //Serial.print(" ");
        //Serial.print(this->myStepper.currentPosition());
        //Serial.print(" ");
        //Serial.print(rr,8);
        //Serial.print(" ");
        //Serial.println(this->myStepper.speed(),8);
        }
      else {
        counter++;
        }
      this->myStepper.runSpeed();
      break;
    case 2: {           // Move with speed 
      this->myStepper.runSpeedToPosition();
      //Serial.println(this->myStepper.distanceToGo());
      if (this->myStepper.distanceToGo() == 0){
        modo=0;
        this->myStepper.stop();
      }
      break;
    }
  }
}  
//  ------------------------------------------ 
//  Start Tracking
//  ------------------------------------------
void Barndoor::startTrack(){
  this->modo=1;
  this->myStepper.setSpeed(calcSpeed(this->myStepper.currentPosition()));
  this->myStepper.runSpeed();
  Serial.println(this->myStepper.speed());
  time0=millis();
  this->direction=-1;
  }
//  ------------------------------------------ 
//  Stop Tracking
//  ------------------------------------------
void Barndoor::stopTracking(){
  modo=0;
  this->direction=0;
  }






    
