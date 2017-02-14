#include <Adafruit_NeoPixel.h>

#include <Adafruit_NeoMatrix.h>
#include <gamma.h>

#include <Adafruit_GFX.h>
#include <gfxfont.h>

#define trigPin A0
#define echoPin A1

#define trigPin2 A4
#define echoPin2 A5

/*
  Button_to_NeoMatrix
  This code takes button input and decide which color to light the neopixel matrix to.
*/

// setup the first neopixel grid as an object called matrixm oriented width wise
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 4, 6,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

// color definition
const uint16_t colors[] = {
matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };

// the setup routine runs once when you press reset:
int InpPin = 10; // A0 for light, 10 for infrared, 9 for switch
int InpVal = 0;
int LEDPin = 5;


void setup() {
  // initialize serial communication at 9600 bits per second:
  pinMode(LEDPin, OUTPUT);
  pinMode(InpPin, INPUT);
  pinMode(trigPin, OUTPUT); // this triggers only if tall people walk through
  pinMode(echoPin, INPUT);
  pinMode(trigPin2, OUTPUT); // this triggers if any person walks through
  pinMode(echoPin2, INPUT);
//  Serial.begin(9600);
  matrix.begin();
  matrix.setBrightness(40);
}

int colorChoice = 0;
long duration, distance;
long duration2, distance2;

// the loop routine runs over and over again forever:
void loop() {
  matrix.fillScreen(0);

  digitalWrite(trigPin2, LOW);
  delay(2);
  digitalWrite(trigPin2, HIGH);
  delay(2);

  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = (duration2/2) / 29.1;

  if(distance2 <= 50) {
    digitalWrite(trigPin, LOW);
    delay(2);
    digitalWrite(trigPin, HIGH);
    delay(2);
  
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration/2) / 29.1;
    
    if(distance <= 50){
      digitalWrite(LEDPin, HIGH);
      colorChoice = 0;
    }    
    else
    {
      digitalWrite(LEDPin, LOW);
      colorChoice = 1;
    }
    matrix.drawRect(1,0,6,4,colors[colorChoice]);
    matrix.show();
    delay(300);
    }

  matrix.show();
//  Serial.print("distance 1 is");
//  Serial.println(distance);
//  Serial.print("distance 2 is");
//  Serial.println(distance2);
  delay(10);
  
}
