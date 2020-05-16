#pragma once

enum COMMON_TYPE{ ANODE, CATHODE};
constexpr const int MAX_VAL = 255 * 4; // Approx 1023 which is the pwm max value, this will make the color math easier.
constexpr const int ALL = 255; // Based on the RGB 255 color code chart
constexpr const int NONE = 0; // Based on the RGB 255 color code chart
constexpr const int FADE_DELAY = 50; 
// Brightness allows us to manipulate the power of the lights to determine brightness
enum Brightness {OFF_B, LOW_B, MEDIUM_B, HIGH_B, MAX_B};

typedef struct Color {
    int r;
    int g;
    int b;
};

constexpr Color colors[8] {{255,0,0}, {255,127,0}, {255,255,0}, {0,255,0}, {0,255,255}, {0,0,255}, {255,0,255}, {255,255,255}};
enum color {RED, ORANGE, YELLOW, GREEN, CYAN, BLUE, MAGENTA, WHITE};

// constexpr Color RED {255,0,0};
// constexpr Color ORANGE {255,127,0};
// constexpr Color YELLOW {255,255,0};
// constexpr Color GREEN {0,255,0};
// constexpr Color CYAN {0,255,255};
// constexpr Color BLUE {0,0,255};
// constexpr Color MAGENTA {255,0,255};
// constexpr Color WHITE {255,255,255};


class RgbLed {
public:
    RgbLed(int red, int green, int blue, COMMON_TYPE type= COMMON_TYPE::ANODE);

    void setColor(int red, int green, int blue, int power = MAX_B);
    void setColor (Color color);
    void off();
    void fade(Color from, Color to, int speedInMS);

private:
    int red_pin, green_pin, blue_pin;
    COMMON_TYPE commonType_;


};

void setLEDs(RgbLed leds[], Color color, int numLEDs = 1, int brightness = 4 );
void fadeLEDs(RgbLed leds[], Color from, Color to, int speed, int numLEDs = 1, int brightness = 4  );