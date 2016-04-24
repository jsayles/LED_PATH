#include <Adafruit_NeoPixel.h>

const int buttonPIN = 2;       // the pin of the pushbutton 
const int buttonLightPIN = 9;    // The ligth on the button
const int stripPIN = 6;

const int pulseInterval = 5;   // The pulse interval
const int pulseDelay = 60;    // The time between pulses

const int sequenceDelay = 20;

const int minButtonBrightness = 100;
const int maxButtonBrightness = 255;


Adafruit_NeoPixel strip = Adafruit_NeoPixel(100, stripPIN, NEO_GRB + NEO_KHZ400);

uint32_t base_color = strip.Color(0, 0, 255);

boolean sequence = false;

void setup() {
    pinMode(buttonLightPIN, OUTPUT);
    
    // Initialize all pixels to 'off'
    strip.begin();
    strip.show(); 

    // Set the interupt to start the sequence
    attachInterrupt(digitalPinToInterrupt(buttonPIN), start_led_sequence, LOW);
}

void loop() {
  if (sequence) {
    led_sequence();
    sequence = false;
  } else {
    colorWipe(strip.Color(0, 0, 0), sequenceDelay);
    button_pulse();
  }
}

void button_light(int fadeValue) {
  analogWrite(buttonLightPIN, fadeValue);
}

void button_pulse() {
  for (int fadeValue = minButtonBrightness ; fadeValue <= maxButtonBrightness; fadeValue += pulseInterval) {
    button_light(fadeValue);
    delay(pulseDelay);
    if (sequence) return;
  }
  for (int fadeValue = maxButtonBrightness ; fadeValue >= minButtonBrightness; fadeValue -= pulseInterval) {
    button_light(fadeValue);
    delay(pulseDelay);
    if (sequence) return;
  }
}

void start_led_sequence() {
  sequence = true;
}

void led_sequence() {
  // Light the button
  button_light(maxButtonBrightness);
  
  // Light all the lights
  //colorWipe(base_color, 40);
  //delay(5000);
  
  // Start the rainbow
  rainbowWipe(sequenceDelay * 2);
  rainbowCycle(sequenceDelay);
}

/********************************************
         From the Adafruit Tutorial
*********************************************/

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

// Makes the rainbow equally distributed throughout
void rainbowWipe(uint8_t wait) {
  uint16_t i;
  uint16_t j = 256*5;
  uint32_t wheel_color = 0;
  for(i=0; i<strip.numPixels(); i++) {
    wheel_color = Wheel(((i * 256 / strip.numPixels()) + j) & 255);
    strip.setPixelColor(i, wheel_color);
    strip.show();
    delay(wait);
  }
}

// Makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  uint32_t wheel_color = 0;
  for(j=256*5; j>0; j--) { // 5 cycles of all colors on wheel
    for(i=0; i<strip.numPixels(); i++) {
      wheel_color = Wheel(((i * 256 / strip.numPixels()) + j) & 255);
      strip.setPixelColor(i, wheel_color);
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
