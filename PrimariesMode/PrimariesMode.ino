//Daniel and Kathleen- Primaries Mode
#include <LiquidCrystal.h>
#include <LCDKeypad.h>

 
LiquidCrystal lcd(8, 13, 9, 4, 5, 6, 7); //set up LCD screen

String states[51] = {"AL","AK","AZ","AR","CA","CO","CT","DE","FL","GA","HI","ID","IL","IN","IA","KS","KY","LA","ME","MD","MA","MI","MN","MS","MO","MT","NE","NV","NH","NJ","NM","NY","NC","ND","OH","OK","OR","PA","RI","SC","SD","TN","TX","UT","VT","VA","WA","WV","WI","WY","DC"};
int electoralVotes[51] = {9,3,11,6,55,9,7,3,29,16,4,4,20,11,6,6,8,8,4,10,11,16,10,6,10,3,5,6,4,14,5,29,15,3,18,7,7,20,4,9,3,11,38,6,3,13,12,5,10,3,3};
int repDelegates[51] = {27,51,29,44,172,28,34,36,19,71,16,28,95,42,28,40,58,9,66,9,72,52,69,99,19,9,29,9,40,59,32,19,23,23,46,40,37,49,155,58,43,38,42,76,40,50,30,50,23,30,57};
int demDelegates[51] = {60,20,85,37,546,79,70,31,246,116,34,27,182,92,52,37,60,59,30,118,116,147,93,41,84,27,30,43,32,142,4,291,121,23,159,42,74,210,33,59,25,76,252,37,26,110,118,37,96,18,45};
int delegates[51];
String state;

int sensorPin = A4;
int sensorValue = 0; 
int outPin = 2; 
int redPin = 12;
int bluePin = 11;       

bool upButton;
bool downButton;
bool selectButton;

int numPlayers;

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
  delay(1000);
  printTwoLines("Use UP/DOWN to  select # players."); //the scroll function was buggier with the arduino in this mode, so we decided to switch it to printing to two lines 
  delay(3000);
  printTwoLines("Press SELECT    when done."); 
  delay(3000);
  numPlayers = 3;
  selectButton = false;
  staticPrintToLCD("# of Players: " + String(numPlayers));
  delay(500);
  while (!selectButton) {
    upButton = analogRead(0) == 931; //UP button being pushed is indicated by a reading of ~931 on analogRead(0);
    downButton = analogRead(0) == 903; //DOWN button being pushed is indicated by a reading of ~903 on analogRead(0);
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
  printTwoLines("You chose " + String(numPlayers) + "     players.");
  delay(3000);
  
  upButton = false;
  downButton = false;
  staticPrintToLCD("UP = Republican");
  lcd.setCursor(0,1);
  lcd.print("DOWN = Democrat");
  while(!upButton && !downButton) { //wait for player to press button to select game mode
    upButton = analogRead(0) == 931; //UP button being pushed is indicated by a reading of ~931 on analogRead(0);
    downButton = analogRead(0) == 903; //DOWN button being pushed is indicated by a reading of ~903 on analogRead(0);
  }
  if (upButton) {
    memcpy(delegates, repDelegates, 51);
    printTwoLines("You chose       Republican!");
    digitalWrite(redPin, HIGH);
    delay(500);
    digitalWrite(redPin, LOW);  
  } else if (downButton){
    int *delegates = demDelegates;
    printTwoLines("You chose       Democrat!");
    digitalWrite(bluePin, HIGH);
    delay(500);
    digitalWrite(bluePin, LOW);  
  }
  delay(7000);
  int score[numPlayers];
  for (int c=0; c<numPlayers; c++) {
    score[c] = 0;
  }
  int delegateCount[numPlayers];    
  int turnScore[numPlayers];
  for(int i=0; i<2; i++){ //loop through all the states + DC
    state = String(states[i]); //this is the state for this turn
    staticPrintToLCD("This turn you'll");
    lcd.setCursor(0,1);
    lcd.print("get votes in ");
    lcd.setCursor(13,1);
    lcd.print(state); //we split up this printing because of bugs we were getting with the arduino
    delay(3000);  
    for(int j=0; j<numPlayers; j++) {
      turnScore[j] = 5000;
      staticPrintToLCD("Player ");
      lcd.setCursor(7,0);
      lcd.print(String(j+1));
      lcd.setCursor(0,1);
      lcd.print("Your turn is now"); //display which player's turn it is at the beginning of this turn
      delay(3000);
      printTwoLines("Press SELECT when ballot removed");
      delay(3000);
        
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
      if (turnScore[j]<0) {
        turnScore[j] = 0;
      }
      score[j] += turnScore[j]; //update total score
      staticPrintToLCD("You earned ");
      lcd.setCursor(11,0);
      lcd.print(String(turnScore[j]));
      lcd.setCursor(0,1);
      lcd.print("votes in ");
      lcd.setCursor(9,1);
      lcd.print(state);
      delay(3000);
      staticPrintToLCD("Your total score: ");
      lcd.setCursor(0,1);
      lcd.print(score[j]);
      lcd.setCursor(10,1);
      lcd.print("votes");
      delay(3000); //delay before next player's turn
    }       
    for (int m=0; m<numPlayers; m++) {
      if (upButton && i==2 || i==7 || i==25 || i==26 || i==29 || i==34 || i==40) { //if REPUBLICAN and in states where republicans have winner-take-all primaries
        if (turnScore[m] == maxArray(turnScore)) { //if player m got the max score
          delegateCount[m] += delegates[i]; //player m get's all the delegates
          staticPrintToLCD("Player " + String(m+1) + " won " + state);
        }
      } else {
        float delegatesAwarded = (float)turnScore[m] / (float)sum(turnScore); //else, split up the delegates proportionally; use float so that you can get fractions
        delegatesAwarded = delegatesAwarded * delegates[i]; 
        delegateCount[m] += (int)delegatesAwarded; //convert back to int because you can't get a fraction of a delegate
      }
      staticPrintToLCD("Player " + String(m+1) + ": ");
      delay(2000);
      staticPrintToLCD("Delegates: ");
      lcd.setCursor(11,0);
      lcd.print(String(delegateCount[m]));
      lcd.setCursor(0,1);
      lcd.print("Votes: ");
      lcd.setCursor(7,1);
      lcd.print(String(score[m]));
      delay(3000);
    }
  }
  staticPrintToLCD("FINAL SCORE:");
  lcd.setCursor(0,1);
  lcd.print("Popular Vote: ");
  delay(3000);
  for (int m=0; m<numPlayers; m++) {
    staticPrintToLCD("P" + String(m+1) + " Votes:" + String(score[m]));
    lcd.setCursor(0,1);
    lcd.print("P" + String(m+1) + " Delegates:" + String(delegateCount[m]));
    delay(3000);
  }
  for (int m=0; m<numPlayers; m++) {
    if (score[m] == maxArray(score)) { //find the vote winner and play song/light up
      if (upButton) {
        digitalWrite(redPin, HIGH);
        delay(500);
        digitalWrite(redPin, LOW);  
      } else {
        digitalWrite(bluePin, HIGH);
        delay(500);
        digitalWrite(bluePin, LOW);  
      }
      printTwoLines("Player " + String(m+1) + " won the most votes!");
      delay(3000);
    }
    delay(1000);
    if (delegateCount[m] == maxArray(delegateCount)) { //find the delegate winner and play song/light up
      if (upButton) {
        digitalWrite(redPin, HIGH);
        delay(500);
        digitalWrite(redPin, LOW);  
      } else {
        digitalWrite(bluePin, HIGH);
        delay(500);
        digitalWrite(bluePin, LOW);  
      }
      printTwoLines("Player " + String(m+1) + " won the most delegates!");
      delay(3000);
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

void printTwoLines(String stringToPrint) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(stringToPrint.substring(0,16));
  lcd.setCursor(0,1);
  lcd.print(stringToPrint.substring(16, stringToPrint.length()));
}

void staticPrintToLCD(String stringToPrint) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(stringToPrint);
}

int maxArray(int arrayToMax[]) { //function to find the max of elements in an int array
  int maxInt = arrayToMax[0];
  for (int i=0; i<=sizeof(arrayToMax); i++) {
    maxInt = max(maxInt, arrayToMax[i]);
  }
  return maxInt;
}

int sum(int arrayToSum[]) { //function to sum the elements in an int array
  int arraySum = 0;
  for (int i=0; i<=sizeof(arrayToSum); i++) {
    arraySum += arrayToSum[i];
  }
  Serial.println("sum = " + String(arraySum));
  return arraySum;
}

