//https://www.pololu.com/product/2132

#ifndef DRV8825.H
#define DRV8825.H

class Drv8825{
    typedef void (*gpio_setFunc)(bool);

    public:
    enum DrvDir{
        dir_0 = 0,
        dir_1 = 1
    };
    enum DrvStep{
        step_full = 0,
        step_half = 1,
        step_4 = 2,
        step_8 = 3,
        step_16 = 4,
        step_32 = 5
    };

    Drv8825(gpio_setFunc dirPin, gpio_setFunc stepPin, gpio_setFunc m0Pin, gpio_setFunc m1Pin, gpio_setFunc m2Pin, gpio_setFunc sleepPin);
    Drv8825(const Drv8825&) = delete;
    void enable_drv();
    void disable_drv();
    void step(DrvStep stepSize, DrvDir direction);

    private:
    void init_drv();
    gpio_setFunc setDirPin;
    gpio_setFunc setStepPin;
    gpio_setFunc setM0Pin;
    gpio_setFunc setM1Pin;
    gpio_setFunc setM2Pin;
    gpio_setFunc setSleepPin;
    void setMode(DrvStep stepSize);
    void setDirection(DrvDir direction);

    DrvStep currentStepSize;
    DrvDir currentDir;
    static const uint8_t stepDuration = 2;
    static const uint8_t setupTime = 1;
};

#endif