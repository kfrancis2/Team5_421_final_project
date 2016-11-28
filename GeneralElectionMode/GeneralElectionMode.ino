//Daniel and Kathleen- General Election Mode
#include <LiquidCrystal.h>
#include <LCDKeypad.h>

 
LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7); //set up LCD screen

String players[2] = {"Republican", "Democrat"};
String states[51] = {"AL","AK","AZ","AR","CA","CO","CT","DE","FL","GA","HI","ID","IL","IN","IA","KS","KY","LA","ME","MD","MA","MI","MN","MS","MO","MT","NE","NV","NH","NJ","NM","NY","NC","ND","OH","OK","OR","PA","RI","SC","SD","TN","TX","UT","VT","VA","WA","WV","WI","WY","DC"};
int electoralVotes[51] = {9,3,11,6,55,9,7,3,29,16,4,4,20,11,6,6,8,8,4,10,11,16,10,6,10,3,5,6,4,14,5,29,15,3,18,7,7,20,4,9,3,11,38,6,3,13,12,5,10,3,3};

int sensorPin = A4;
int sensorValue = 0; 
int outPin = 2; 
int redPin = 12;
int bluePin = 11; 
      
bool upButton;
bool downButton;
bool selectButton;

void setup() {
  lcd.begin(16, 2); //set up our 16x2 LCD screen
  lcd.clear();
  lcd.setCursor(0,0);
  
  randomSeed(analogRead(0));
  pinMode(outPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  Serial.begin(9600);
  
  lcd.print("New Game!");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
}

void loop() {
  upButton = false;
  downButton = false;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Press UP for Gen'l Election (2 players)   ");
  delay(500);
  lcd.setCursor(0,0);
  for (char k=0;k<23;k++){
    if (upButton || downButton) {
      break;
    }
    lcd.scrollDisplayLeft();
    for (int a=0; a<2000; a++) {
      upButton = analogRead(0) == 931; //UP button being pushed is indicated by a reading of ~931 on analogRead(0);
      downButton = analogRead(0) == 903; //DOWN button being pushed is indicated by a reading of ~903 on analogRead(0);
    }
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Press DOWN for Primaries (>2 players)    ");
  lcd.setCursor(0,0);
  for (char k=0;k<23;k++){
    if (upButton || downButton) {
      break;
    }
    lcd.scrollDisplayLeft();
    for (int a=0; a<2000; a++) {
      upButton = analogRead(0) == 931; //UP button being pushed is indicated by a reading of ~931 on analogRead(0);
      downButton = analogRead(0) == 903; //DOWN button being pushed is indicated by a reading of ~903 on analogRead(0);
    }
  }
  staticPrintToLCD("UP = 2 Players");
  lcd.setCursor(0,1);
  lcd.print("DOWN = >2 Players");
  while(!upButton && !downButton) { //wait for player to press button to select game mode
    upButton = analogRead(0) == 931; //UP button being pushed is indicated by a reading of ~931 on analogRead(0);
    downButton = analogRead(0) == 903; //DOWN button being pushed is indicated by a reading of ~903 on analogRead(0);
  }
  if (upButton) {
    //GENERAL ELECTION MODE
    scrollPrintToLCD("You chose General Election Mode!");
    scrollPrintToLCD("One player will be Democratic candidate.");
    scrollPrintToLCD("Other will be Republican candidate.");
    
    int score[2] = {0,0};
    int electoralScore[2] = {0,0};
    boolean first = random(2); //randomly choose one player to go first
    int order[2] = {first, !first}; //make array containing order of players
    for(int i=0; i<51; i++){ //loop through all the states + DC
      String state = states[i]; //this is the state for this turn
      scrollPrintToLCD("This turn, you'll get votes in " + state);
      
      int turnScore[2] = {5000, 5000}; //each player starts with a score of 5000 in each state
      for(int j=0; j<2; j++) {
        scrollPrintToLCD(players[order[j]] + "'s turn is now!");
        scrollPrintToLCD("Press SELECT when you remove ballot");
        delay(500);
        
        selectButton = false; 
        int iter = 0;
        while (!selectButton && iter < 120){ //waits for the SELECT button to be pressed or for it to time out
          selectButton = analogRead(0) == 613; //checks if SELECT button pressed
          sensorValue = analogRead(sensorPin);
          turnScore[j] -= sensorValue; //subtract from players score if they hit the edge of the ballot hole
          staticPrintToLCD("VOTES: " + String(turnScore[j])); //display score
          for (int b=0; b<2000; b++) { //this spends about a half second checking the sensor; this makes the buzzer very responsive, and means that if you only briefly touch the edge, you might not lose points
            if(analogRead(sensorPin)>400){
              digitalWrite(outPin, HIGH); //tell CircuitPlayground to buzz and light up if they hit the edge of the ballot hole
            }
            digitalWrite(outPin, LOW);
          }
          iter++; //keep track of how long they're taking
        }
        turnScore[j] -= iter; //subtract votes for how long they're taking
        if(selectButton){
          turnScore[j] += 1000; // +1000 votes if successfully remove ballot
        } else {
          turnScore[j] = 0; //0 votes if they didn't remove ballot (timed out)
        }
        score[j] += turnScore[j]; //update total score

        scrollPrintToLCD("You earned " + String(turnScore[j]) + " votes in " + state);
        scrollPrintToLCD("You now have " + String(score[j]) + " total votes.");
        delay(2000); //delay before next player's turn
      }
      if(turnScore[0] > turnScore[1]) { //check who got more votes in that state
        electoralScore[0] += electoralVotes[i]; //give winner the appropriate number of electoral votes for that state
        staticPrintToLCD(players[order[0]].substring(0,3) + " won " + state);
        if (players[order[0]] == "Democrat") {
          digitalWrite(bluePin, HIGH);
          delay(500);
          digitalWrite(bluePin, LOW);  
        } else {
          digitalWrite(redPin, HIGH);
          delay(500);
          digitalWrite(redPin, LOW);  
        }
        delay(1000);
      } else {
        electoralScore[1] += electoralVotes[i];
        staticPrintToLCD(players[order[1]].substring(0,3) + " won " + state);
        if (players[order[1]] == "Democrat") {
          digitalWrite(bluePin, HIGH);
          delay(500);
          digitalWrite(bluePin, LOW);    
        } else {
          digitalWrite(redPin, HIGH);
          delay(500);
          digitalWrite(redPin, LOW);  
        }
        delay(1000);
      }
      for (int m=0; m<2; m++) {
        staticPrintToLCD(players[order[m]] + ":");
        delay(1000);
        staticPrintToLCD("Electoral: " + String(electoralScore[m]));
        lcd.setCursor(0,1);
        lcd.print("Popular: " + String(score[m]));
        delay(2000);
      }
    }
    staticPrintToLCD("Final Pop. Vote");
    staticPrintToLCD(players[order[0]].substring(0,3) + ": " + String(score[0])); 
    lcd.setCursor(0,1);
    lcd.print(players[order[1]].substring(0,3) + ": " + String(score[1]));

    staticPrintToLCD("Final Elec. Vote");
    staticPrintToLCD(players[order[0]].substring(0,3) + ": " + String(electoralScore[0]));
    lcd.setCursor(0,1);
    lcd.print(players[order[1]].substring(0,3) + ": " + String(electoralScore[1]));
    if(score[0] > score[1]){
      if (players[order[0]] == "Democrat") {
        digitalWrite(bluePin, HIGH); //send voltage to correct pin on CircuitPlayground to light up the correct color and play Hail to the Chief
        delay(500);
        digitalWrite(bluePin, LOW);   
      } else {
        digitalWrite(redPin, HIGH);
        delay(500);
        digitalWrite(redPin, LOW);  
      }
      scrollPrintToLCD(players[order[0]] + " won the popular vote.");
    } else {
      if (players[order[1]] == "Democrat") {
        digitalWrite(bluePin, HIGH);
        delay(500);
        digitalWrite(bluePin, LOW);    
      } else {
        digitalWrite(redPin, HIGH);
        delay(500);
        digitalWrite(redPin, LOW);  
      }
      scrollPrintToLCD(players[order[1]] + " won the popular vote.");
    }
    delay(1000);
    if(electoralScore[0] > electoralScore[1]){
      if (players[order[0]] == "Democrat") {
        digitalWrite(bluePin, HIGH);
        delay(500);
        digitalWrite(bluePin, LOW);    
      } else {
        digitalWrite(redPin, HIGH);
        delay(500);
        digitalWrite(redPin, LOW);  
      }
      scrollPrintToLCD(players[order[0]] + " won the electoral vote.");
    } else {
      if (players[order[1]] == "Democrat") {
        digitalWrite(bluePin, HIGH);
        delay(500);
        digitalWrite(bluePin, LOW);    
      } else {
        digitalWrite(redPin, HIGH);
        delay(500);
        digitalWrite(redPin, LOW);  
      }
      scrollPrintToLCD(players[order[1]] + " won the electoral vote.");
    }
  } else if (downButton) {
    //PRIMARY MODE --------------------------------------------------------------------------
    scrollPrintToLCD("You chose Primaries Mode!"); // can only scroll through 40 characters at a time; so messages are split up into short phrases
    //The arduino got SUPER buggy when we tried to put both modes in one sketch, so we separated them into two sketches.
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Press RESET button to start new game.");
  while(true) {
    lcd.setCursor(0,0);
    delay(1000);
    for (char k=0;k<23;k++){
      lcd.scrollDisplayLeft();
      delay(400);
    }
  }
}

void scrollPrintToLCD(String stringToPrint) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(stringToPrint + "         "); // can only scroll through 40 characters at a time; so messages are split up into short phrases
  delay(1000);
  selectButton = false;
  for (char k=0;k<23;k++){
    if (selectButton) { //if you press SELECT, it will skip past the instructions
      break;
    }
    lcd.scrollDisplayLeft();
    for (int a=0; a<2000; a++) {
      selectButton = analogRead(0) == 613; //SELECT button being pushed is indicated by a reading of ~613 on analogRead(0);
    }
  }  
}

void staticPrintToLCD(String stringToPrint) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(stringToPrint);
}


