// 
// Created by: Steven Smethurst (funvill) 
// Created on: September 16, 2015 
// Created for: https://goo.gl/YlIgWO
// Last updated: September 16, 2015 
// 
// 

#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

// Define the pin that we are going to be using to control the LEDs. 
// This could be any digitial pin, 6 was choosen at random. 
#define LED_PIN 6

// Defines how many milli seconds there are in one second. 
#define MS_PER_SECOND (1000 )

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, LED_PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

// This is a global variable used to store the current time. Generally, you 
// should use "uint32_t" for variables that hold time. The value will
// quickly become too large for an int to store. 
// 
// The Arduino Pro Mini uses a ATmega328 chip. This chip is a 8 bit AVR. 
// Because of this the size of a unsigned long is 4 BYTES or 32 bits. 
uint32_t gTime ; 

uint32_t gSecondColorOn, gSecondColorOff; 

// The set up function is run once at the start of the program. 
void setup() {

  // Set up the serial port to be used for debug messages. 
  Serial.begin(9600);
    
  // Set up the LED strip 
  strip.begin();
  strip.setBrightness( 255 / 4 ); // Set the brightness of the LEDs to 1/4 of their normal brightness. Don't blind me bro 
  strip.show(); // Initialize all pixels to 'off'

  // Test the LED strip to ensure that all the LEDs work. 
  testLEDStrip();
  
  // Configure the color for the pixels 
  gSecondColorOn  = strip.Color(0, 0, 255);
  gSecondColorOff = strip.Color(0, 0, 0);
  
  // Reset the global time variable to a known value. 
  // 1442646883 = GMT: Sat, 19 Sep 2015 07:14:43 GMT || 9/19/2015, 12:14:43 AM GMT-7:00 DST
  // 1442874600 = GMT: Mon, 21 Sep 2015 22:30:00 GMT || 9/21/2015, 03:30:00 PM GMT-7:00 DST
  gTime = 1442874600 ; 
}

// This function will print the value as a decimal number and in binary 
// This can be useful for debuging what is happening in your application. 
void debugPrintBinary( uint32_t value ) {
  Serial.print("Value=[");
  Serial.print( value, DEC );
  Serial.print("] Binary=[");
  Serial.print( value, BIN );
  Serial.println("]");  
}

// Set all the LEDS to the same color. 
void setAllLEDs( uint32_t c ){
  // Loop thought all the LEDs and set the color of the LED to the same color. 
  for( unsigned char pixelOffset = 0 ; pixelOffset < strip.numPixels() ; pixelOffset++ ) {
    strip.setPixelColor(pixelOffset, c );
  }
}

// This function is used to make sure all the LEDs in the strip are functioning correctly
void testLEDStrip() {    
  // Rainbow 
  // Cycle thought the rainbow 5 times quickly. 
  for( int rainbowOffset = 0 ; rainbowOffset < 256*5 ; rainbowOffset++ ) {
    for( int pixelOffset = 0 ; pixelOffset < strip.numPixels() ; pixelOffset++ ) {
      strip.setPixelColor(pixelOffset, Wheel(((pixelOffset * 256 / strip.numPixels()) + rainbowOffset) & 255));
    }
    strip.show(); 
    delay(1); 
  }
  setAllLEDs( gSecondColorOff ); 
}

// This function will take a value and extract the bits from the value to be shown as pixels on the 
// LED strip. 
void UpdateLEDsFromValue( uint32_t value ) {
  debugPrintBinary( value ); 
  
  // Loop thought the value extracting the bits. If a bit is 1 then turn the pixel to the on color 
  // If the bit is off then turn the pixel to the off color. 
  
  // sizeof() 
  // The sizeof operator returns the number of bytes in a variable type, or the number of bytes 
  // occupied by an array.
  // Reference: https://www.arduino.cc/en/Reference/sizeof
  
  for( unsigned char pixelOffset = 0 ; pixelOffset <= (sizeof( uint32_t ) * 8) ; pixelOffset++ ) {
    // bitRead()
    // Reads a bit of a number. 
    // Reference: https://www.arduino.cc/en/Reference/bitRead
    
    if( bitRead(value, pixelOffset) == 1 ) {
      strip.setPixelColor(pixelOffset, gSecondColorOn ); // On 
    } else {
      strip.setPixelColor(pixelOffset, gSecondColorOff ); // Off
    }
  }
  
  // Show the updated LEDS.
  strip.show();

}

// This is the simplest form of a count up clock. 
// When the program starts the value of gTime will be set to zero. Every time this function is run the 
// value of gTime will be incurmented by one. At the end of this function the program is delayed by one second. 
// 
// This function has a major flaw with it, can you guess what the flaw is? 
void simpleCountUpClock(){
  gTime++ ; // Each loop, incurment the timer. 
  
  // Display the gTime on the LEDs 
  UpdateLEDsFromValue( gTime ); 
  
  // Delay() 
  // Pauses the program for the amount of time (in miliseconds) 
  // https://www.arduino.cc/en/Reference/Delay 
  delay( MS_PER_SECOND ) ; // Delay the application for one second 
}

// This function uses the timer inside of the arduino to get the relitive time since the program started up. 
// Then if the current time is greater then the intervale (one sec) it will trigger the program to update 
// the LEDs with the new time value. 
void countUpBasedOnTimer(){  
  static uint32_t previousMillis = millis(); ;  

  // millis()
  // Returns the number of milliseconds since the Arduino board began running 
  // the current program. This number will overflow (go back to zero), after 
  // approximately 50 days.
  // Reference: https://www.arduino.cc/en/Reference/millis 
  uint32_t currentMillis = millis();
  
  // Check to see if the interval has elapsed 
  if( currentMillis - previousMillis > MS_PER_SECOND ) {
    previousMillis = currentMillis ; // Update the previouse time 
    
    // Update the LEDs 
    UpdateLEDsFromValue( gTime + ( currentMillis / MS_PER_SECOND ) ); 
  }
}

// This function is advanced. Come talk to me about it.

void fancyCountUpClock() {
  static uint32_t previousMillis = millis(); ;  
  uint32_t currentMillis = millis() ; 
  
  // Check to see if the interval has elapsed 
  if( currentMillis - previousMillis > MS_PER_SECOND/strip.numPixels() ) {
    previousMillis = currentMillis ; // Update the previous time 
    
    // Show the Millis lead led. 
    static int pixelMillisOffset = 0 ;
    pixelMillisOffset++; 
    if( pixelMillisOffset >= strip.numPixels() ) {
      // Reset 
      setAllLEDs( gSecondColorOff ); 
      pixelMillisOffset = 0 ; 
    }
        
    // Update the background Millis pixel. 
    // Make the leading Millis pixel bright blue.
    strip.setPixelColor(pixelMillisOffset, gSecondColorOn ); 
    // Turn off the previouse pixel that might have been tunred on. 
    if( pixelMillisOffset > 0 ) { 
      strip.setPixelColor(pixelMillisOffset-1, gSecondColorOff ); 
    }

    
    // Update the seconed LEDs 
    uint32_t currentTime = gTime + ( currentMillis / MS_PER_SECOND ) ; 
    for( int pixelOffset = 0 ; pixelOffset < (sizeof( uint32_t ) * 8) ; pixelOffset++ ) {
      if( bitRead( currentTime, pixelOffset) == 1 ) {
        strip.setPixelColor(pixelOffset, Wheel( (255 / strip.numPixels()) * pixelOffset ) ); // On 
      } 
    }
    
    
    // Show the updated LEDS.
    strip.show();
  }   
}

// Gives the user a warning flash then starts a count down. 
void CountDownTimer( uint32_t timeInSeconds ) {
  
  // Set to Red. 
  setAllLEDs( strip.Color(0, 0, 0) );
  strip.show(); // Show the updated LEDS.
  for( int pixelOffset = strip.numPixels() ; pixelOffset >= 0 ; pixelOffset-- ) {
    strip.setPixelColor(pixelOffset, strip.Color(255, 0, 0) );
    strip.show();
    delay(MS_PER_SECOND/strip.numPixels()); 
  }
  
  // Count down the red to yellow. 
  for( int pixelOffset = strip.numPixels() ; pixelOffset >= 0 ; pixelOffset-- ) {
    strip.setPixelColor(pixelOffset, strip.Color(255, 255, 0) );
    strip.show();
    delay(MS_PER_SECOND/strip.numPixels()); 
  }

  uint32_t previousMillis = millis();       
  // Show count down. 
  while( timeInSeconds > 0 ) {
    uint32_t currentMillis = millis();   
    // Check to see if the interval has elapsed 
    if( currentMillis - previousMillis > MS_PER_SECOND ) {
      previousMillis = currentMillis ; // Update the previouse time 
      timeInSeconds--; 
      
      UpdateLEDsFromValue( timeInSeconds ); 
    }
  }
  
  // Done, flash a bunch of times. 
  for( int flashCount = 0 ; flashCount < 10 ; flashCount++ ) {
    setAllLEDs( strip.Color(255, 0, 0) );
    strip.show(); // Show the updated LEDS.
    delay(MS_PER_SECOND); 
    setAllLEDs( strip.Color(0, 0, 0) );
    strip.show(); // Show the updated LEDS.
    delay(MS_PER_SECOND); 
  }
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

void loop() {
 
  // simpleCountUpClock(); 
  // countUpBasedOnTimer(); 
  fancyCountUpClock(); 
  // CountDownTimer( 120 ); 
}
