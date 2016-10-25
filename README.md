# Team5_421_final_project
Bioe 421 Operation Game

# Operation: Election Game - Project Abstract
We will build a microcontroller-controlled game, based on a Presidential Election theme and loosely based on the concept of the existing board game Operation. The board game will consist of a map of the United States, and each state will have a hole in it, surrounded by capacitive sensors, from which the player must extract a “ballot” using alligator clips. At the beginning of the game, one player chooses to be the Democratic candidate, while the other will be the Republican candidate. The sensing mechanism will be connected to the microcontroller, which will use the sensing data to determine how “well” the player extracted the ballot (without touching the sides of the hole). For each state, each of the two players will try to extract the ballot, and the state’s electoral votes will go to the player who extracts the ballot better (according to the microcontroller’s scoring mechanism). If both players are either unsuccessful or successful, the microcontroller will give the votes to the party that typically carries that state (i.e. red vs. blue state); for swing states, we will randomly assign the votes to one player. The microcontroller will have a small LED screen attached, which can show the score and whose turn it is. As we develop the game, we will try to add complexity by incorporating data about different election years, so that the players can choose and election year at the beginning of the game, which will affect who gets which states.


# Brainstorming Project Ideas
- Using Circuit Playground
  - Use accelerometers on Circuit Playground to make our own (simple) Wii-like games
  - Bop-it type game?
  - Operation type game
  - Using the speech pattern to make an alarm clock/stop watch?
  - Using camera
 - Facial Recognition -- make it automatically take a picture when a person is in view, then email picture to that person, add more features maybe?
   - Respond to voice/clap to take picture
   - You can see the image on the screen, position yourself, then clap and it’ll take a picture after a couple seconds
   - Maybe options like clap once to save photo/twice to delete and try again?
   - Clap multiple times for multiple pictures?
   - Put the camera on a little turntable with motor and have it scan the room and take pictures when it senses motion or sees a face?
 - Using Twitter/internet?
   - Light up/buzz/alert you when you get an email, and then press a button to send an automatic reply
   - Posting happy birthday onto a person’s wall when it is their birthday
   - Getting top news stories of interest and compiling them together- or alerts when news stories come out?


- Problems:
  - Not knowing when the metro is coming
  - Training for arthrogryposis stretching


# Further Idea Development 
“Operation” Game Idea:
- We might be able to make our own capacitive sensor to be whatever size/shape we want: https://square-the-circle.com/2013/12/08/arduino-capacitative-proximity-sensor/ 
- Automatically keeping score:
  - Tell it at the beginning how many players there are
  - Add a little LED Screen attached to Pi or Arduino to show the score.
  - Maybe press a button when the turn is over, keeps track of whose turn it is
  - During turn, microcontroller senses how close you are, calculates score based on how close and how long it took
  - At end of turn, either:
     - camera takes a picture and uses OpenCV to determine if piece was successfully removed
     - OR there’s a button next to each hole that you can press when you successfully remove it (based on  which button and which player’s turn it is, the computer can assign points)




- Election Theme:
     - Map of the US with all 50 States
       - Print out on OEDK or Mudd plotter
     - Each state you have to pick out a “ballot” or some little shape that represents the state
     - One person is democrat/the other republican, and the game board could have an LED that lights up with the appropriate color after someone gets the piece out
     - You get a “number of votes” (score) based on how close you get to the edge
     - We could incorporate something so that “red” states are harder for the democrat to get/”blue” states are harder for republican
     - Something special for “swing” states?
     - Going along with that, we could have the option to choose an election year, and then we have the data for which candidate won which state that year and incorporate that?


- Rice Themed:
     - Map of Rice campus, you have to “steal” something from each building (or each college?)
     - Each player chooses a residential college, and the LEDs light up with the college colors when they get the point
