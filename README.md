# Meteorite shower

## Introduction 
--
The project is made for Introduction to Reobotics course, 3rd year at the Faculty of Matematics and Computer Science, University of Bucharest.
It is composed by a menu and a game called Meteorite shower, an imitation of Space Invasion recreated on a 8X8 led matrix.

## Task Requirements
- Menu Requirements: 
  - Intro message "Meteorite shower" (game name) that is shown for 4s. In this time, on matrix is display a "HI" message.
  - After waiting 4s, the menu is displayed: 
    - Start game
    - Leaderboard: if the joystick button is pressed, a list of at most 5 player name - player score appears. If no one played before/the leaderboard was reseted, instead of the list, appers a message "Nothing here...". To return to the menu, the player must press the joystick button. 
    - Settings: displays a new menu
      - Enter name: the player has the opportunity to set his name from menu and there for to play multiple games with that chosen name, without been ask to set his name repeatedly.
      - Difficulty (3 types: low, medium, high)
      The difficulty setting is very important. Below, it is explained how difficulty influenced the game:
      
     | | LOW | |  MEDIUM || HIGH  ||
     | | :---:  | :---: |     :---: | :---:     |   :---:     |   :---: |
     |Step | Step 0 |  Step 1 |  Step 0 |  Step 1 | Step 0 | Step 1 | 
     |Points for destroy a meteorite | 1 | 1| 2 | 2| 3 |3 |
     |Interval of occurrence of meteorites| 4s | 3s | 3s | 2s| 2s|2s|
     |Number of meteorites per occurrence| 1| 1| 2| 2 | 3 |3 |
     |Number of lives of a meteorite | 1 |1 | 1 |1 |1 |2
     |Laser shoot | - | -| - |- | - | one after killing 6 meteorites|
      
## Used components
---
