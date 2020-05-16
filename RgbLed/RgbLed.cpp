#include "RgbLed.h"
#include "Arduino.h"



RgbLed::RgbLed(int red, int green, int blue, COMMON_TYPE type)
: red_pin {red}, green_pin {green}, blue_pin {blue}, commonType_ {type} {
    off();
}

void RgbLed::off(){
    auto writeType = (commonType_ == COMMON_TYPE::ANODE ? HIGH : LOW);
    setColor(0,0,0);
}

void RgbLed::setColor(int red, int green, int blue, int brightness){
    // The PWM value goes up to 1024, MAX_VAL in our case. 
    // the brightness levels reflect the change in actual power
    red *= brightness;
    green *= brightness;
    blue *= brightness;

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

void RgbLed::setColor (Color color){
    setColor(color.r, color.g, color.b);
}

void RgbLed::fade(Color from, Color to, int speedInMS)
{
    //if speed is 0, then we just want to switch the color straight away
    if (speedInMS == 0)
    {
        setColor(to.r, to.g, to.b);
        return;
    }
    float fadeIterations = speedInMS / FADE_DELAY;
    int redRate = round((to.r - from.r) / fadeIterations);
    int greenRate = round((to.g - from.g) / fadeIterations);
    int blueRate = round((to.b - from.b) / fadeIterations);
    Color currentColor {from.r, from.g, from.b};
    for (auto i = 0; i < fadeIterations; i++)
    {
        currentColor.r += redRate;
        currentColor.g += greenRate;
        currentColor.b += blueRate;
        setColor(currentColor);
        delay(FADE_DELAY);
    }
    setColor(to);
}

void setLEDs(RgbLed leds[], Color color, int numLEDs, int brightness )
{
    for (auto i = 0; i < numLEDs; i++)
    {
        leds[i].setColor(color.r, color.g, color.b, brightness);
    }
}

void fadeLEDs(RgbLed leds[], Color from, Color to, int speed, int numLEDs, int brightness )
{
    //if speed is 0, then we just want to switch the color straight away
    if (speed == 0)
    {
        setLEDs(leds, to, numLEDs);
        return;
    }
    float fadeIterations = speed / FADE_DELAY;
    int redRate = round((to.r - from.r) / fadeIterations);
    int greenRate = round((to.g - from.g) / fadeIterations);
    int blueRate = round((to.b - from.b) / fadeIterations);
    Color currentColor {from.r, from.g, from.b};
    for (auto i = 0; i < fadeIterations; i++)
    {
        currentColor.r += redRate;
        currentColor.g += greenRate;
        currentColor.b += blueRate;
        setLEDs(leds, currentColor, numLEDs, brightness);
        delay(FADE_DELAY);
    }
    setLEDs(leds, to, numLEDs);
}