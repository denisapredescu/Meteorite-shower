# Meteorite shower

## Introduction 
--
The project is made for Introduction to Reobotics course, 3rd year at the Faculty of Matematics and Computer Science, University of Bucharest.
It is composed by a menu and a game called Meteorite shower, an imitation of Space Invasion recreated on a 8X8 led matrix.

## Task Requirements
1. Menu Requirements: 
  - Intro message "Meteorite shower" (game name) that is shown for 4s. In this time, on matrix is display a "HI" message.
  - After waiting 4s, the main menu is displayed:
    a. Start game
    b. Leaderboard: if the joystick button is pressed, a list of at most 5 player name - player score appears. If no one played before/the leaderboard was reseted, instead of the list, appers a message "Nothing here...". To return to the menu, the player must press the joystick button. 
    c. Settings: displays a new menu
      - Enter name: the player has the option to insert his name from menu and play multiple games with that chosen name, without been ask to set his name repeatedly.
      - Difficulty (3 types: low, medium, high)
      The difficulty setting is very important. By default, the difficulty is set as LOW.
      Below, it is explained how difficulty influenced the game:
      
     | Difficulty | LOW | |  MEDIUM || HIGH  ||
     | :---: | :---:  | :---: |     :---: | :---:     |   :---:     |   :---: |
     |Step | Step 0 |  Step 1 |  Step 0 |  Step 1 | Step 0 | Step 1 | 
     |Points for destroy a meteorite | 1 | 1| 2 | 2| 3 |3 |
     |Interval of occurrence of meteorites| 4s | 3s | 3s | 2s| 2s|2s|
     |Number of meteorites per occurrence| 1| 1| 2| 2 | 3 |3 |
     |Number of lives of a meteorite | 1 |1 | 1 |1 |1 |2
     |Laser shot | - | -| - |- | - | one after killing 6 meteorites|
     
      - LCD contrast control: can take 6 values: 25, 50, 75, 100, 125, 150. The chosen value is saved to EEPROM.
      - LCD brightness contol: can take 6 values: 0, 51, 102, 153, 204, 255. The chosen value is saved to EEPROM.
      - Matrix brightness control: can take 6 values: 0, 3, 6, 9, 12, 15. The chosen value is saved to EEPROM.
      - Sound: ON/OFF
        In menu, sound ON means that it is heard a sound when the joystick is moved up-down-left-right and another sound for moving the joystick in a direction thta can not be moved.
      - Reset hscore: the list of scores from leaderboard from main menu is deleted (if it is clicked on Leaderboard it will display the message "Nothing here...")
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
