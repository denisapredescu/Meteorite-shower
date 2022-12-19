# Meteorite shower

## Introduction 
The project is made for Introduction to Reobotics course, 3rd year at the Faculty of Matematics and Computer Science, University of Bucharest.
It is composed by a menu and a game called `Meteorite shower`, similar to Space Invader recreated on a 8X8 led matrix.

## Task Requirements
1. <i>Game menu</i>
  - Intro message "Meteorite shower" (game name) that is shown for 4s. In this time, on matrix is displayed a "HI" message.
  - Main menu (every section of this menu has a different image on matrix):
    - **Start game**
    - **Leaderboard**: </br>
    If the joystick button is pressed, a list of at most 5 player - score pairs appears. </br>
    If no one played before/the leaderboard was reseted, instead of the list, appears the message "Nothing here...". </br>
    To return to the menu, the player must press the joystick button. 
    - **Setting**: displays a new menu
      - <b>Enter name</b>: the player has the option to insert his name from menu and play multiple games with that chosen name, without been ask to set his name repeatedly.
      - <b>Difficulty</b>: the player can choose between low, medium and high. </br>
        By default, the difficulty is set as LOW. </br>
      - <b>LCD contrast control</b>: can take 6 values: 25, 50, 75, 100, 125, 150. The chosen value is saved to EEPROM.
      - <b>LCD brightness contol</b>: can take 6 values: 0, 51, 102, 153, 204, 255. The chosen value is saved to EEPROM.
      - <b>Matrix brightness control</b>: can take 6 values: 0, 3, 6, 9, 12, 15. The chosen value is saved to EEPROM.
      - <b>Sound</b>: ON/OFF (the chosen value is saved to EEPROM.) </br>
        In menu, sound ON means that it is heard a sound when the joystick is moved up-down-left-right and another sound for moving the joystick in a direction that can not be moved.
      - <b>Reset hscore</b>: the list of scores from leaderboard from main menu is reset (now if it is clicked on Leaderboard it will display the message "Nothing here...")
      - Press to exit: by pressing the joystick, we return to the main menu 
    - **About**: displays the below details:
       > Game: Meteorite shower </br>
       Author: Denisa Predescu </br>
       GitHub: https://github.com/denisapredescu/Meteorite-shower (link to this repo) </br>
       Project for Introduction to Robotics, Unibuc </br>
       Prees to exit - button that returns to main manu
    - **How to play**: displays a mini description about the game </br>
      >The earth is under a meteorite shower and we are the last hope for the humanity! </br>
       Destroy the meteorites before they hit earth! </br>
       The difficulty increases as more enemies are killed. </br>
       You have 3 lives. If you die, the game restarts at the first step of the current difficulty.
  - Extra: added a potentiometer to decrease/increase the volume of the sound
  - While playing, on LCD is displayed the player name (the one he chosed in settings or the dummy name `PLAYR`), the difficulty, the number of lives left and the score. Additionally, in the last step of the game, when the player has the ability to laser shot, appears an 'L' to indicate that the next bullet is a laser. 
  - After the game ends, it's displayed the blinking message "GAME OVER" and the acumulated score for a few seconds. If the score is not higher then one from the leaderboard, the message `Your score is not good enought` is shown to inform the player. If the score is top 5, firstly the player has to insert a player name in case if he did not do this in settings, secondly if is number 1, he will see the message `You made it! Highest score!` and if is just on top 5 - `You made it! Top 5! :)`. </br>

2. <i>Game</i> 
  Meteorite shower is a very intuitive and fun to play game: the player need to destroy as many meteorites as he can in order to prevent the meteorites from killing him. </br>
  </br>
  Playing is simple: press the button to shoot and move the joystick left-right to move the destroyer. </br>
  </br>
  The difficulty can be seen as a level, so every difficulty end with a boss fight which is harder to beat as difficulty increases (at the first boss, the player has time to shoot it before it comes closer; at the second, the time is less and the third boss - the one which comes repeatedly - is hardest because every piece of it needs 2 bullets to destroy). </br>
  </br>
  Below it is explained how difficulty influenced the game: </br>
  </br>
 | Difficulty | LOW ||  MEDIUM || HIGH  ||
 | :---: | :---:  | :---: | :---: | :---: | :---: | :---: |
 |<b>Step</b> | Step 0 |  Step 1 |  Step 0 |  Step 1 | Step 0 | Step 1 | 
 |<b>Points for destroy a meteorite</b> | 1 | 1| 2 | 2| 3 |3 |
 |<b>Interval of occurrence of meteorites</b>| 4s | 3s | 3s | 2s| 2s|2s|
 |<b>Number of meteorites per occurrence</b>| 1| 1| 2| 2 | 3 |3 |
 |<b>Number of lives of a meteorite</b> | 1 |1 | 1 |1 |1 |2|
 |<b>Number of killed meteorites in order to increase the difficulty</b>| 10 | 10 | 20 | 20 | 30 | - |
 |<b>Laser shot</b> | - | -| - |- | - | one shot after killing 6 meteorites|
 |<b>Boss fight</b>| - | - | just if the player ends the difficulty low | - | just if the player ends the difficulty medium | for every 30+ killed meteorites |  
 </br>
 </br>
 The game have basic sounds when hit a meteorite, when dying, when the difficulty is increased, when the game is over.
 </br>
 
## Used components
- Arduino Uno board
- LCD 
- 8X8 matrix display
- MAX7219 Driver for matrix
- joystick
- buzzer
- potentiometer for volume
- button
- 2 resistors
- cables
- 2 breadboards


## Pictures of the setup

## Pictures of the matrix images

## Link to video showcasting functionality

