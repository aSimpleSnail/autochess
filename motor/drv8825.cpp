#include "drv8825.h"
#include <chrono>
#include <thread>


Drv8825::Drv8825(gpio_setFunc dirPin, gpio_setFunc stepPin, gpio_setFunc m0Pin, gpio_setFunc m1Pin, gpio_setFunc m2Pin, gpio_setFunc sleepPin)
    : setDirPin(dirPin), setStepPin(stepPin), setM0Pin(m0Pin), setM1Pin(m1Pin), setM2Pin(m2Pin), setSleepPin(sleepPin)
{
    init_drv();
    enable_drv();
}
void Drv8825::init_drv(){
    setMode(DrvStep::step_full);
    setDirection(DrvDir::dir_0);
}
void Drv8825::enable_drv(){
    setSleepPin(true);
}
void Drv8825::disable_drv(){
    setSleepPin(false);    
}
void Drv8825::step(DrvStep stepSize, DrvDir direction){
    bool waitOnSetup = false;
    if(stepSize != currentStepSize){
        setMode(stepSize);
        waitOnSetup = true;
    }
    if(direction != currentDir){
        setDirection(direction);
        waitOnSetup = true;
    }
    if(waitOnSetup){
        std::this_thread::sleep_for(std::chrono::microseconds(setupTime));
    }

    setStepPin(1);
    std::this_thread::sleep_for(std::chrono::microseconds(stepDuration));
    setStepPin(0);
    std::this_thread::sleep_for(std::chrono::microseconds(stepDuration));
       
} 
void Drv8825::setMode(DrvStep stepSize){
    switch (stepSize){
        case 0 :
        setM0Pin(0);
        setM1Pin(0);
        setM2Pin(0);        
        break;
        case 1 :
        setM0Pin(1);
        setM1Pin(0);
        setM2Pin(0);
        break; 
        case 2 :
        setM0Pin(0);
        setM1Pin(1);
        setM2Pin(0);
        break;
        case 3 :
        setM0Pin(1);
        setM1Pin(1);
        setM2Pin(0);
        break;
        case 4 :
        setM0Pin(0);
        setM1Pin(0);
        setM2Pin(1);
        break;
        case 5 :
        setM0Pin(1);
        setM1Pin(1);
        setM2Pin(1);
        break;
        default:
        return;
    }
    currentStepSize = stepSize;
}
void Drv8825::setDirection(DrvDir direction){
    setDirPin(direction);
    currentDir = direction;
}