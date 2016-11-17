//Daniel and Kathleen- Final Project3
#include <LiquidCrystal.h>
#include <LCDKeypad.h>

 
LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7); //set up LCD screen

String players[2] = {"Republican", "Democrat"};
String states[51] = {"AL","AK","AZ","AR","CA","CO","CT","DE","FL","GA","HI","ID","IL","IN","IA","KS","KY","LA","ME","MD","MA","MI","MN","MS","MO","MT","NE","NV","NH","NJ","NM","NY","NC","ND","OH","OK","OR","PA","RI","SC","SD","TN","TX","UT","VT","VA","WA","WV","WI","WY","DC"};
int electoralVotes[51] = {9,3,11,6,55,9,7,3,29,16,4,4,20,11,6,6,8,8,4,10,11,16,10,6,10,3,5,6,4,14,5,29,15,3,18,7,7,20,4,9,3,11,38,6,3,13,12,5,10,3,3};
int repDelegates[51] = {27,51,29,44,172,28,34,36,19,71,16,28,95,42,28,40,58,9,66,9,72,52,69,99,19,9,29,9,40,59,32,19,23,23,46,40,37,49,155,58,43,38,42,76,40,50,30,50,23,30,57};
int demDelegates[51] = {60,20,85,37,546,79,70,31,246,116,34,27,182,92,52,37,60,59,30,118,116,147,93,41,84,27,30,43,32,142,4,291,121,23,159,42,74,210,33,59,25,76,252,37,26,110,118,37,96,18,45};
int delegates [51];

int sensorPin = A4;
int sensorValue = 0; 
int outPin = 2; 
int redPin = 12;
int bluePin = 11;       

bool upButton;
bool downButton;
bool selectButton;
bool buttonPressed;

int numPlayers;

int capsense[51] = {0,1,2,3,12,6,9,10,0,1,2,3,12,6,9,10,0,1,2,3,12,6,9,10,0,1,2,3,12,6,9,10,0,1,2,3,12,6,9,10,0,1,2,3,12,6,9,10,0,1,2};
#define BUZZER_1       400



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
  //PRIMARY MODE --------------------------------------------------------------------------
  scrollPrintToLCD("You chose Primaries Mode!"); // can only scroll through 40 characters at a time; so messages are split up into short phrases
  scrollPrintToLCD("Use UP/DOWN to select # players."); 
  scrollPrintToLCD("Press SELECT when done."); 
  numPlayers = 3;
  selectButton = false;
  staticPrintToLCD("# of Players: " + String(numPlayers));
  delay(500);
  while (!selectButton) {
    upButton = analogRead(0) == 931; //UP button being pushed is indicated by a reading of ~931 on analogRead(0);
    downButton = analogRead(0) == 902; //DOWN button being pushed is indicated by a reading of ~902 on analogRead(0);
    if (upButton && numPlayers<20) {
      numPlayers++;
      staticPrintToLCD("# of Players: " + String(numPlayers));
    }
    if (downButton && numPlayers>3) {
      numPlayers--;
      staticPrintToLCD("# of Players: " + String(numPlayers));
    }
    upButton = false;
    downButton = false;
    delay(200);
    selectButton = analogRead(0) == 613;
  }
  scrollPrintToLCD("You chose " + String(numPlayers) + " players.");
  
  upButton = false;
  downButton = false;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Press UP for Republican.     ");
  delay(500);
  lcd.setCursor(0,0);
  for (char k=0;k<23;k++){
    if (upButton || downButton) {
      break;
    }
    lcd.scrollDisplayLeft();
    for (int a=0; a<2000; a++) {
      upButton = analogRead(0) == 931; //UP button being pushed is indicated by a reading of ~931 on analogRead(0);
      downButton = analogRead(0) == 902; //DOWN button being pushed is indicated by a reading of ~902 on analogRead(0);
    }
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Press DOWN for Democrat.     ");
  lcd.setCursor(0,0);
  for (char k=0;k<23;k++){
    if (upButton || downButton) {
      break;
    }
    lcd.scrollDisplayLeft();
    for (int a=0; a<2000; a++) {
      upButton = analogRead(0) == 931; //UP button being pushed is indicated by a reading of ~931 on analogRead(0);
      downButton = analogRead(0) == 902; //DOWN button being pushed is indicated by a reading of ~902 on analogRead(0);
    }
  }
  staticPrintToLCD("UP = Republican");
  lcd.setCursor(0,1);
  lcd.print("DOWN = Democrat");
  while(!upButton && !downButton) { //wait for player to press button to select game mode
    upButton = analogRead(0) == 931; //UP button being pushed is indicated by a reading of ~931 on analogRead(0);
    downButton = analogRead(0) == 902; //DOWN button being pushed is indicated by a reading of ~902 on analogRead(0);
  }
  if (upButton) {
    memcpy(delegates, repDelegates, 51);
    scrollPrintToLCD("You chose Republican!");
    digitalWrite(redPin, HIGH);
    delay(500);
    digitalWrite(redPin, LOW);  
  } else if (downButton){
    int *delegates = demDelegates;
    scrollPrintToLCD("You chose Democrat!");
    digitalWrite(bluePin, HIGH);
    delay(500);
    digitalWrite(bluePin, LOW);  
  }
  for (int l=0; l<sizeof(delegates); l++) {
    Serial.println(delegates[l]);
  }
  int score[numPlayers];
  for (int c=0; c<numPlayers; c++) {
    score[c] = 0;
  }
  int delegateCount[numPlayers];    

  for(int i=0; i<2; i++){ //loop through all the states + DC
    String state = states[i]; //this is the state for this turn
    scrollPrintToLCD("This turn, you'll get votes in " + state);
    
    int turnScore[numPlayers];
    for(int j=0; j<numPlayers; j++) {
      turnScore[j] = 5000;
      scrollPrintToLCD("Player " + String(j+1) + "'s turn is now!"); //display which player's turn it is at the beginning of this turn
      scrollPrintToLCD("Press SELECT when you remove ballot");
      delay(500);
        
      buttonPressed = false; 
      int iter = 0;
      while (!buttonPressed && iter < 120){ //waits for the SELECT button to be pressed or for it to time out
        buttonPressed = analogRead(0) == 613; //checks if SELECT button pressed
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
      if(buttonPressed){
        turnScore[j] += 1000; // +1000 votes if successfully remove ballot
      } else {
        turnScore[j] = 0; //0 votes if they didn't remove ballot (timed out)
      }
      if (turnScore[j]<0) {
        turnScore[j] = 0;
      }
      score[j] += turnScore[j]; //update total score

      scrollPrintToLCD("You earned " + String(turnScore[j]) + " votes in " + state);
      scrollPrintToLCD("You now have " + String(score[j]) + " total votes.");
      delay(2000); //delay before next player's turn
    }       
    for (int m=0; m<numPlayers; m++) {
      if (upButton && i==2 || i==7 || i==25 || i==26 || i==29 || i==34 || i==40) { //if REPUBLICAN and in states where republicans have winner-take-all primaries
        if (turnScore[m] == maxArray(turnScore)) { //if player m got the max score
          delegateCount[m] += delegates[i]; //player m get's all the delegates
          scrollPrintToLCD("Player " + String(m+1) + " won " + state);
        }
      } else {
        delegateCount[m] += turnScore[m] / sum(turnScore) * delegates[i]; //else, split up the delegates proportionally
        Serial.println("--");
        Serial.println(turnScore[m]);
        int sumscore = sum(turnScore);
        Serial.println(sumscore);
        Serial.println(delegates[i]);
        
        Serial.println(turnScore[m] / sum(turnScore) * delegates[i]);
      }
      scrollPrintToLCD("Player " + String(m+1) + ": ");
      scrollPrintToLCD("Delegate Count: " + String(delegateCount[m]) + ", Votes: " + String(score[m]));
    }
  }
  staticPrintToLCD("FINAL SCORE:");
  lcd.setCursor(0,1);
  lcd.print("Popular Vote: ");
  for (int m=0; m<numPlayers; m++) {
    staticPrintToLCD("P" + String(m+1) + " Votes:" + String(score[m]));
    lcd.setCursor(0,1);
    lcd.print("P" + String(m+1) + " Delegates:" + String(delegateCount[m]));
  }
  Serial.println("max score");
  Serial.println(maxArray(score));
  for (int m=0; m<numPlayers; m++) {
    if (score[m] == maxArray(score)) {
      if (upButton) {
        digitalWrite(redPin, HIGH);
        delay(500);
        digitalWrite(redPin, LOW);  
      } else {
        digitalWrite(bluePin, HIGH);
        delay(500);
        digitalWrite(bluePin, LOW);  
      }
      scrollPrintToLCD("Player " + String(m+1) + " won the most votes!");
    }
    delay(1000);
    if (delegateCount[m] = maxArray(delegateCount)) {
      if (upButton) {
        digitalWrite(redPin, HIGH);
        delay(500);
        digitalWrite(redPin, LOW);  
      } else {
        digitalWrite(bluePin, HIGH);
        delay(500);
        digitalWrite(bluePin, LOW);  
      }
      scrollPrintToLCD("Player " + String(m+1) + " won the most delegates!");
    }
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

int maxArray(int arrayToMax[]) {
  int maxInt = arrayToMax[0];
  for (int i=0; i<sizeof(arrayToMax); i++) {
    maxInt = max(maxInt, arrayToMax[i]);
  }
}

int sum(int arrayToSum[]) {
  int arraySum = 0;
  for (int i=0; i<sizeof(arrayToSum); i++) {
    arraySum += arrayToSum[i];
  }
}

