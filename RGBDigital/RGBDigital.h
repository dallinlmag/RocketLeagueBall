#pragma once

enum COMMON_TYPE{ ANODE, CATHODE};
constexpr const int MAX_VAL = 255 * 4; // Approx 1023 which is the pwm max value, this will make the color math easier.
constexpr const int SCALE_FACTOR = 4; //This takes us from the 255 scale up to the PWM scale
constexpr const int ALL = 255; // Based on the RGB 255 color code chart
constexpr const int NONE = 0; // Based on the RGB 255 color code chart

class RGBDigital {
public:
    RGBDigital(int red, int green, int blue, COMMON_TYPE type= COMMON_TYPE::ANODE);

    void setColor(int red, int green, int blue);
    void off();

private:
    int red_pin, green_pin, blue_pin;
    COMMON_TYPE commonType_;


};