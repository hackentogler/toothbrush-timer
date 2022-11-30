///////////////////////////////////////////////////////////////////////////////
// Circuit Playground Basic Hourglass
//
// Author: Carter Nelson
// MIT License (https://opensource.org/licenses/MIT)
// 
// 2020-07-25: Adapted for tooth brushing
// Jeff Hougentogler

#include <Adafruit_CircuitPlayground.h>

//#define DEBUG 1
#define NUM_LEDS 10
// NUM_LEDS should be evenly divisible by SIDES_OF_TEETH
#define NUM_LEDS_USED 8
#define SIDES_OF_TEETH 4
// Total time in milliseconds
#define COUNT_TIME 120000
// Miliseconds delay on flashing
#define FLASH_RATE 50

///////////////////////////////////////////////////////////////////////////////
void setup() {
  // Initialize the Circuit Playground
  CircuitPlayground.begin();
  
  #ifdef DEBUG
  Serial.begin(9600);
  #endif
}

///////////////////////////////////////////////////////////////////////////////
void loop() {
  // After this interval, change the side of your teeth you are brushing
  unsigned long changeSideTime = COUNT_TIME / SIDES_OF_TEETH;
  unsigned long sideCounter = 0;

  #ifdef DEBUG
  CircuitPlayground.playTone(1000,1000,false);
  #endif
  
  // Turn ON all the NeoPixels
  for (int p = 0; p < NUM_LEDS; p++) {
    if (p != 2 && p != 7) {
      CircuitPlayground.setPixelColor(p, 255, 255, 255);
    }
  }

  // Compute DT
  unsigned long DT = COUNT_TIME / NUM_LEDS_USED;
  
  // Turn OFF the NeoPixels one at a time, waiting DT each time
  for (int p = 0; p < NUM_LEDS; p++) {
    // Skip over 2 and 7 because they aren't lit
    if (p == 2 || p == 7) {
      continue;
    }
    
    delay(DT);
    
    sideCounter += DT;
    
    #ifdef DEBUG
    Serial.print("sideCounter = ");
    Serial.println(sideCounter);
    #endif
    
    // Has this exceeded the time needed to brush on one side?
    // Currently this is every 12 seconds, need to adjust to 30 second boundaries
    if (sideCounter >= changeSideTime) {
      CircuitPlayground.playTone(1000,1000,false);
      // Flash LED and reset counter
      for (int q = 0; q < 5; q++) {
        CircuitPlayground.setPixelColor(p, 255, 0, 0);
        delay(FLASH_RATE);
        CircuitPlayground.setPixelColor(p, 0, 255, 0);
        delay(FLASH_RATE);
        CircuitPlayground.setPixelColor(p, 0, 0, 255);
        delay(FLASH_RATE);
        CircuitPlayground.setPixelColor(p, 0, 0, 0);
        delay(FLASH_RATE);
      }
      
      //CircuitPlayground.setPixelColor(p, 255, 255, 255);
      sideCounter = 0;
      
    }
    
    CircuitPlayground.setPixelColor(p, 0, 0, 0);
  }
  
  // Wait for Circuit Playground to be flipped over (face down)
  while (CircuitPlayground.motionZ() > 0) {};

  // A little debounce
  delay(500);

  // Wait for Circuit Playground to be flipped back over (face up)
  while (CircuitPlayground.motionZ() < 0) {};
}
