#include <Adafruit_CircuitPlayground.h>

#define C5  523.25
#define Db5 554.37
#define D5  587.33
#define Eb5 622.25
#define E5  659.26
#define F5  698.46
#define Gb5 739.99
#define G5  783.99
#define Ab5 830.61
#define LA5 880.00
#define Bb5 932.33
#define B5  987.77
#define C6  1046.50
#define Db6 1108.73
#define D6  1174.66
#define Eb6 1244.51
#define E6  1318.51
#define F6  1396.91
#define Gb6 1479.98
#define G6  1567.98
#define Ab6 1661.22
#define LA6 1760.00
#define Bb6 1864.66
#define B6  1975.53
// DURATION OF THE NOTES 

#define BPM 100 //determines speed of song
#define H 2*Q //half 2/4
#define Q 60000/BPM //quarter 1/4 
#define E Q/2   //eighth 1/8
#define S Q/4 // sixteenth 1/16
#define W 4*Q // whole 4/4

#define BUZZER_1       400 //tone for buzzer 


void setup() {
  Serial.begin(9600);
  pinMode(5, OUTPUT); 
  CircuitPlayground.begin();
}

void loop() {
  //The loop() of this code just continuously checks for voltage to be applied to the A11, A7, and A9 pins of the CircuitPlayground.
  //The Arduino code will send 5V to the appropriate pin to communicate something to the CircuitPlayground, and this code tells the CircuitPlayground what to do for each of the pins the Arduino communicates with.
  if (analogRead(A11) > 400) { //detect 5V applied to A11 pin (5V will be applied when player touches rim of ballot hole)
    for (int l=0; l<10; l++) {
      CircuitPlayground.setPixelColor(l, 255, 0, 0); //light up all NeoPixels red
    }
    CircuitPlayground.playTone(BUZZER_1, 250); //play buzzer noise
    delay(100); 
    CircuitPlayground.clearPixels(); //turn off neoPixels
  }
  if (analogRead(A7) > 500) { //detect if 5V applied to A7 pin (5V will be applied when Republican won a state or the game)
    for (int l=0; l<10; l++) {
     CircuitPlayground.setPixelColor(l, 255, 0, 0); //light up all NeoPixels red
    }
    //the following plays the first couple measures of "Hail to the Chief"
    tone(5,D6,Q); 
    delay(1+Q); //delay duration should always be 1 ms more than the note in order to separate them.
    tone(5,E6,E);
    delay(1+E);
    tone(5,Gb6,E);
    delay(1+E);
    tone(5,G6,Q);
    delay(1+Q);
    tone(5,Gb6,E);
    delay(1+E);
    tone(5,E6,E);
    delay(1+E);
    tone(5,D6,E+S); 
    delay(1+E+S);
    tone(5,E6,S); 
    delay(1+S);
    tone(5,D6,E); 
    delay(1+E);
    tone(5,B5,E); 
    delay(1+E);
    tone(5,LA5,Q); 
    delay(1+Q);
    tone(5,G5,Q); 
    delay(1+Q);
    
    delay(2000);
    CircuitPlayground.clearPixels(); //turn off neoPixels
  }
  
  if (analogRead(A9) > 500) { //detect if 5V applied to A9 pin (5V will be applied when Democrat won a state or the game)
    for (int l=0; l<10; l++) {
     CircuitPlayground.setPixelColor(l, 0, 0, 255); //light up all NeoPixels blue
    }
    //the following plays the first couple measures of "Hail to the Chief"
    tone(5,D6,Q); 
    delay(1+Q); //delay duration should always be 1 ms more than the note in order to separate them.
    tone(5,E6,E);
    delay(1+E);
    tone(5,Gb6,E);
    delay(1+E);
    tone(5,G6,Q);
    delay(1+Q);
    tone(5,Gb6,E);
    delay(1+E);
    tone(5,E6,E);
    delay(1+E);
    tone(5,D6,E+S); 
    delay(1+E+S);
    tone(5,E6,S); 
    delay(1+S);
    tone(5,D6,E); 
    delay(1+E);
    tone(5,B5,E); 
    delay(1+E);
    tone(5,LA5,Q); 
    delay(1+Q);
    tone(5,G5,Q); 
    delay(1+Q);

    delay(2000);
    CircuitPlayground.clearPixels(); //turn off NeoPixels
  }
}
