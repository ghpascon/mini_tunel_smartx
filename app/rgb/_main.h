#include <FastLED.h>
#define NUM_LEDS 1
#define DATA_PIN 48
CRGB leds[NUM_LEDS];

class LED_RGB
{
public:
    void setup()
    {
        FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    }

    void state(byte r, byte g, byte b)
    {
        leds[0] = CRGB(r, g, b);
        FastLED.show();
    }
};
