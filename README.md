# Meteorite shower

The matrix project is made for Introduction to Robotics course, 3rd year at the Faculty of Matematics and Computer Science, University of Bucharest.

## Backstory
The game i made is called `Meteorite shower` and it's inspired from Space Invader. The main idea of the game is to hit the meteorites that appears from the upper side of the matrix. </br>
I chose this game because I was very excited when I saw it and I thought it was interesting to try to create it from scratch and make it as fun as possible to play. </br>
I build it in steps: first it was important to have a moving destroyer (moved by the joystick) and meteorites that appear random at an interval of time. From there it was easier to implement the ideas. The difficulties are basically changing the number of meteorites that come it once, the duration of an interval, the number of meteorites life and the points gained from destroying a meteorites.


## How to navigate through the menu
At first it is displayed a intro message "Meteorite shower" (game name) that is shown for 4s. In this time, on matrix is displayed a "HI" message. After those 4s, the main menu appears and every section of this menu has a different image on matrix):
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

## Game description
Meteorite shower is a very intuitive and fun to play game: the player need to destroy as many meteorites as he can in order to prevent the meteorites from killing him. </br>
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
 |<b>Laser shot</b> | - | -| - |- | - | one shot after killing 6 meteorites |
 |<b>Boss fight</b>| - | - | just if the player ends the difficulty low | - | just if the player ends the difficulty medium | for every 30+ killed meteorites |  
 </br>
The difficulty can be seen as a level, so every difficulty end with a boss fight which is harder to beat as difficulty increases (at the first boss, the player has time to shoot it before it comes closer; at the second, the time is less and the third boss - the one which comes repeatedly - is hardest because every piece of it needs 2 bullets to destroy). </br>
 </br>
 While playing, on LCD is displayed the player name (the one he chosed in settings or the dummy name `PLAYR`), the difficulty, the number of lives left and the score. Additionally, in the last step of the game, when the player has the ability to laser shot, appears an 'L' to indicate that the next bullet is a laser. 
   </br>
   </br>
After the game ends, it's displayed the blinking message "GAME OVER" and the acumulated score for a few seconds and the player sees a sad face on the matrix. If the score is not higher then one from the leaderboard, the message `Your score is not good enought` is shown to inform the player. If the score is top 5 appears a happy face on matrix and firstly the player has to insert a player name in case if he did not do this in settings, secondly if is number 1, he will see the message `You made it! Highest score!` and if is just on top 5 - `You made it! Top 5! :)`. </br>
 </br>
 The game have basic sounds: when hit a meteorite, when dying, when the difficulty is increased, when the game is over.


## How to play
 Playing is simple: press the button to shoot and move the joystick left-right to move the destroyer. </br>
 The game forces the player to move faster as the difficulty increases and to play smart when the boss scene is on.  </br>
 At the final step of difficulty HIGH, the player needs to pay attention to the display to see when the "L" appears (meaning that the next bullet is a laser shot)   </br>
 
 <details><summary>See how the LCD looks like while playing</summary>
  
 - normal display
  ![fara L](https://user-images.githubusercontent.com/86727047/208517026-77be93a9-f4b0-4920-ad36-198062da5aad.jpeg)

 - display when the laser shot is active
  ![cu L](https://user-images.githubusercontent.com/86727047/208517016-030a334b-3e52-4f32-a16e-ad4e31120f5b.jpeg)

</details>
  
  
 <details><summary><b>Used components</b></summary>
  
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

</details>


<details><summary><b>Picture of the setup</b></summary>
  
 ![WhatsApp Image 2022-12-19 at 21 03 48 (1)](https://user-images.githubusercontent.com/86727047/208517545-a2bccbd4-e617-44d8-a451-99e10891f85b.jpeg)

  
 ![WhatsApp Image 2022-12-19 at 21 03 47 (1)](https://user-images.githubusercontent.com/86727047/208517304-cd52482d-f297-4d94-a46c-07852343c9cf.jpeg)


![WhatsApp Image 2022-12-19 at 21 03 47](https://user-images.githubusercontent.com/86727047/208517183-3e1a0495-74b0-4640-8d11-a87666c5642d.jpeg)

  
</details>


<details><summary><b>Pictures of the matrix images</b></summary>
  
- Greeting message
  ![3](https://user-images.githubusercontent.com/86727047/208514716-ef03b4a9-e1fe-4a99-9596-0920936ebc0c.jpeg)
  
- Start game 
  ![monster](https://user-images.githubusercontent.com/86727047/208516245-5355f00f-f330-44ec-b799-992268006871.jpeg)

- Leaderboard - image of a trophy
  ![WhatsApp Image 2022-12-19 at 21 03 52 (1)](https://user-images.githubusercontent.com/86727047/208515043-ebb7bda9-1939-446a-8209-8a3ad75fc441.jpeg)
  
- Settings - image of a wrench
  ![WhatsApp Image 2022-12-19 at 21 03 52](https://user-images.githubusercontent.com/86727047/208515975-13d84380-31fd-4abe-94c7-bc622b0452ca.jpeg)

- About - image of an "i" from information
  ![WhatsApp Image 2022-12-19 at 21 03 51](https://user-images.githubusercontent.com/86727047/208515141-7656f4cd-72db-421b-8f25-e609acba6895.jpeg)
  
- How to play - image of an question mark
  ![WhatsApp Image 2022-12-19 at 21 03 49 (1)](https://user-images.githubusercontent.com/86727047/208515304-54bbcffd-1ca1-4804-905e-34a556bc74ae.jpeg)

  
  


  
  
  
</details>


## Link to video showcasting functionality

