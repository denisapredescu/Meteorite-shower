# Meteorite shower

## Introduction 
---
The project is made for Introduction to Reobotics course, 3rd year at the Faculty of Matematics and Computer Science, University of Bucharest.
It is composed by a menu and a game called Meteorite shower, an imitation of Space Invasion recreated on a 8X8 led matrix.

## Task Requirements
---
1. <i>Menu Requirements</i>: 
  - Intro message "Meteorite shower" (game name) that is shown for 4s. In this time, on matrix is display a "HI" message.
  - After waiting 4s, the main menu is displayed:
    a. <b>Start game</b>
    b. <b>Leaderboard</b>: </br>
    If the joystick button is pressed, a list of at most 5 player name - player score appears. If no one played before/the leaderboard was reseted, instead of the list, appers a message "Nothing here...". To return to the menu, the player must press the joystick button. 
    c. <b>Settings</b>: displays a new menu
      - <b>Enter name</b>: the player has the option to insert his name from menu and play multiple games with that chosen name, without been ask to set his name repeatedly.
      - <b>Difficulty</b> (3 types: low, medium, high) </br>
      The difficulty setting is very important. By default, the difficulty is set as LOW. </br>
      Below, it is explained how difficulty influenced the game: </br>
      
     | Difficulty | LOW | |  MEDIUM || HIGH  ||
     | :---: | :---:  | :---: |     :---: | :---:     |   :---:     |   :---: |
     |<<b>Step</b> | Step 0 |  Step 1 |  Step 0 |  Step 1 | Step 0 | Step 1 | 
     |<b>Points for destroy a meteorite</b> | 1 | 1| 2 | 2| 3 |3 |
     |<b>Interval of occurrence of meteorites</b>| 4s | 3s | 3s | 2s| 2s|2s|
     |<b>Number of meteorites per occurrence</b>| 1| 1| 2| 2 | 3 |3 |
     |<b>Number of lives of a meteorite</b> | 1 |1 | 1 |1 |1 |2
     |<b>Laser shot</b> | - | -| - |- | - | one after killing 6 meteorites|
     
      - <b>LCD contrast control</b>: can take 6 values: 25, 50, 75, 100, 125, 150. The chosen value is saved to EEPROM.
      - <b>LCD brightness contol</b>: can take 6 values: 0, 51, 102, 153, 204, 255. The chosen value is saved to EEPROM.
      - <b>Matrix brightness control</b>: can take 6 values: 0, 3, 6, 9, 12, 15. The chosen value is saved to EEPROM.
      - <b>Sound</b>: ON/OFF </br>
        In menu, sound ON means that it is heard a sound when the joystick is moved up-down-left-right and another sound for moving the joystick in a direction thta can not be moved.
      - <b>Reset hscore</b>: the list of scores from leaderboard from main menu is deleted (if it is clicked on Leaderboard it will display the message "Nothing here...")
      - Press to exit: by pressing the joystick, we return to the main menu 
    d. About: displays details:
      -  `Game: Meteorite shower`
      -  `Author: Denisa Predescu`
      -  `GitHub: https://github.com/denisapredescu/Meteorite-shower` (link to this repo)
      -  `Project for Introduction to Robotics, Unibuc`
      -  Prees to exit - button
    e. How to play: displays a mini description about the game
      `The earth is under a meteorite shower and we are the last hope for the humanity! 
       Destroy the meteorites before they hit earth. 
       The  difficulty increases as more enemies are killed. 
       You have 3 lives. If you die, the game restarts at the first step of the current difficulty.`
  - Extra: added a potentiometer to decrease/increase the volum of the sound
  - While playing, on LCD presentes the player name (the one he chosed in settings or the dummy name `PLAYR`), the difficulty, the number of lives left and the score. Additionally, in the last step of the game, when the player has the ability to laser shot, it appearce an L to indicate that the next bullet is a laser. 
## Used components
---
