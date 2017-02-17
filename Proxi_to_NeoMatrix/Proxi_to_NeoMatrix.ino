#include <Adafruit_NeoPixel.h>

#include <Adafruit_NeoMatrix.h>
#include <gamma.h>

#include <Adafruit_GFX.h>
#include <gfxfont.h>

#include <SPI.h>
#include <SD.h>
#include <Adafruit_VS1053.h>

// These are the pins used
#define VS1053_RESET   -1     // VS1053 reset pin (not used!)

#define trigPin2 A0
#define echoPin2 A1

#define trigPin1 A4
#define echoPin1 A5

// Sound ports
#define VS1053_CS       6     // VS1053 chip select pin (output)
#define VS1053_DCS     10     // VS1053 Data/command select pin (output)
#define CARDCS          5     // Card chip select pin
// DREQ should be an Int pin *if possible* (not possible on 32u4)
#define VS1053_DREQ     9     // VS1053 Data request, ideally an Interrupt 

Adafruit_VS1053_FilePlayer musicPlayer = 
  Adafruit_VS1053_FilePlayer(VS1053_RESET, VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS);
  
/*
  Proxi_to_NeoMatrix
  This code takes proxi sensor input and decide which color to light the neopixel matrix to.
  Once the first sensor is triggered (trigpin1), it checks the second 
*/

// setup the first neopixel grid as an object called matrix oriented width wise
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 4, 11,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

// color definition
const uint16_t colors[] = {
matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };

// the setup routine runs once when you press reset:

void setup() {
  // initialize serial communication at 9600 bits per second:
  pinMode(trigPin1, OUTPUT); // this triggers if any person walks through
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT); // this triggers only if tall people walk through
  pinMode(echoPin2, INPUT);

  Serial.begin(9600);
  matrix.begin();
  matrix.setBrightness(25);

  // Sound code
  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }

  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }
  
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(1,1);
  
#if defined(__AVR_ATmega32U4__) 
  // Timer interrupts are not suggested, better to use DREQ interrupt!
  // but we don't have them on the 32u4 feather...
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int
#elif defined(ESP32)
  // no IRQ! doesn't work yet :/
#else
  // If DREQ is on an interrupt pin we can do background
  // audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
#endif
  
}

int colorChoice = 0;
long duration1, distance1;
long duration2, distance2;

// the loop routine runs over and over again forever:
void loop() {
  matrix.fillScreen(0);

  digitalWrite(trigPin1, LOW);
  delay(2);
  digitalWrite(trigPin1, HIGH);
  delay(2);

  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = (duration1/2) / 29.1;
  
  digitalWrite(trigPin2, LOW);
  delay(2);
  digitalWrite(trigPin2, HIGH);
  delay(2);

  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = (duration2/2) / 29.1;
  
  if(distance1 <= 50) {

    if(distance2 <= 50){
      colorChoice = 0;
//      digitalWrite(soundTrigger, HIGH); // send out high if tall person detected -- NOT WELCOME!!!
    }    
    else
    {
      colorChoice = 1;
    }
    matrix.drawRect(1,0,6,4,colors[colorChoice]);
    matrix.show();
    if (colorChoice == 1) {
      musicPlayer.playFullFile("welcome.mp3");
    }
    else {
      musicPlayer.playFullFile("error.mp3");
    }
    delay(100);
    }

    Serial.print("lower distance is ");
    Serial.println(distance1);
    Serial.print("upper distance is ");
    Serial.println(distance2);
    
  matrix.show();
  delay(10);
  
}
