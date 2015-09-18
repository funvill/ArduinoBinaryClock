#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

#define MS_PER_SECOND 1000 

unsigned long gTime ; 

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

void setup() {
  
  // Set up the debug serial port 
  Serial.begin(9600);
  
  // Set up the LED strip 
  strip.begin();
  strip.setBrightness(64); // Set the brightness of the LEDs to 1/4 of their normal brightness. Don't blind me bro 
  strip.show(); // Initialize all pixels to 'off'
  
  testLEDStrip();
   
  gTime = 0 ; 
}

void debugPrintBinary( unsigned long value ) {
  Serial.print("Value=[");
  Serial.print( value, DEC );
  Serial.print("] Binary=[");
  Serial.print( value, BIN );
  Serial.println("]");
  
}

void setAllToBlack(){
  // Set to black. 
  for( unsigned char pixelOffset = 0 ; pixelOffset < strip.numPixels() ; pixelOffset++ ) {
    strip.setPixelColor(pixelOffset, strip.Color(0, 0, 0) ); // Red 
  }  
}

void testLEDStrip() {
  
  // Set to black. 
  setAllToBlack();
  
  // Set each led to red one at a time. 
  for( unsigned char pixelOffset = 0 ; pixelOffset < strip.numPixels() ; pixelOffset++ ) {
    strip.setPixelColor(pixelOffset, strip.Color(0, 255, 0) ); // Green
    strip.show();
    delay(20); 
  }  
}

void simpleCountUpClock(){
  gTime++ ; // Each loop, incurment the time. 
  debugPrintBinary( gTime ); 
  
  // Loop thought the pixels in the LED strip turning on or off the LEDs if the bit in the time variable is on. 
  for( unsigned char pixelOffset = 0 ; pixelOffset < (sizeof( gTime ) * 8)   ; pixelOffset++ ) {    
    if( bitRead(gTime, pixelOffset) == 1 ) {
      strip.setPixelColor(pixelOffset, strip.Color(255, 0, 0) ); // Red 
    } else {
      strip.setPixelColor(pixelOffset, strip.Color(0, 0, 0) ); // Black 
    }
  }  
 
  // Show the results.  
  strip.show();
  delay( MS_PER_SECOND ); 
}


void countUpBasedOnTimer(){
  unsigned long currentMillis = millis();
  if( currentMillis - previousMillis > MS_PER_SECOND ) {
    debugPrintBinary( currentMillis / MS_PER_SECOND ); 
    
    previousMillis = currentMillis ; 
    
    // Loop thought the pixels in the LED strip turning on or off the LEDs if the bit in the time variable is on. 
    for( unsigned char pixelOffset = 0 ; pixelOffset < (sizeof( previousMillis ) * 8)   ; pixelOffset++ ) {    
      if( bitRead(currentMillis/MS_PER_SECOND, pixelOffset) == 1 ) {
        strip.setPixelColor(pixelOffset, strip.Color(255, 0, 0) ); // Red 
      } else {
        strip.setPixelColor(pixelOffset, strip.Color(0, 0, 0) ); // Black 
      }
    }
    
    // Show the results.  
    strip.show();
  }
}


void loop() {
//  simpleCountUpClock(); 
  countUpBasedOnTimer(); 
  // delay(2); 

}
