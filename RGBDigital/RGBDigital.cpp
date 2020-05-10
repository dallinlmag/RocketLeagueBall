#include "rgbDigital.h"
#include "Arduino.h"



RGBDigital::RGBDigital(int red, int green, int blue, COMMON_TYPE type)
: red_pin {red}, green_pin {green}, blue_pin {blue}, commonType_ {type} {
    off();
}

void RGBDigital::off(){
    auto writeType = (commonType_ == COMMON_TYPE::ANODE ? HIGH : LOW);
    setColor(0,0,0);
}

void RGBDigital::setColor(int red, int green, int blue){
    // The PWM value goes up to 1024, MAX_VAL in our case. 
    red *= SCALE_FACTOR;
    green *= SCALE_FACTOR;
    blue *= SCALE_FACTOR;

    // For commmon anode, we need to turn the pins low to produce the signal, so we need to take the difference of our value from the MAX_VAL
    if (commonType_ == COMMON_TYPE::ANODE){
        red = MAX_VAL - red;
        green = MAX_VAL - green;
        blue = MAX_VAL - blue;
    }

    //Now we have the right values we can write to the LED
    analogWrite(red_pin, red);
    analogWrite(green_pin, green);
    analogWrite(blue_pin, blue);

}

