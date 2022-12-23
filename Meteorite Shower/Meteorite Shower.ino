// values position in EEPROM:
// position 0: lcdBrightness (value between 0 and 255)
// position 1: matrixBrightness (value between 0 and 15)
// position 2: sound (0 or 1)
// position 3: lcdContrast (value between 25 and 150)
// position 9: numberOfHighscores (max 5)
// then leaderboard:
//      positions 10 - 14  name 1
//      positions 16 - 18  score 1
//      positions 20 - 24  nume 2
//      positions 26 - 28  score 2
//      ...

#include <LiquidCrystal.h>  // library for display
#include "LedControl.h"     // library for matrix
#include <EEPROM.h>         // library for eeprom: read/write data
#include "pitches.h"

const byte dinPin = 12;
const byte clockPin = 11;
const byte loadPin = 10;
const byte matrixSize = 8;

const byte RS = 13;
const byte enable = 8;
const byte d4 = 7;
const byte d5 = 3;
const byte d6 = 5;
const byte d7 = 4;
const byte lcdBrightnessControlPin = 6;
const byte lcdContrastControlPin = 9;

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

// joystick setup
const byte pinSW = 2;
const byte pinX = A0;
const byte pinY = A1;
const byte buzzer = A2;
const byte buttonPin = A3;
byte swState = HIGH;
byte lastSwState = HIGH;
byte buttonState = HIGH;
byte lastButtonState = HIGH;
int xValue = 0;
int yValue = 0;
bool xJoystickState = false;
bool yJoystickState = false;
const int minThreshold = 400;
const int maxThreshold = 600;

byte readingPinSW = 1;
byte readingButtonPin = 1;
unsigned long lastDebounceTimeForButton = 0;
unsigned long lastDebounceTime = 0;
const byte debounceDelay = 50;  // for both buttons
unsigned long startGreetingTime = 0;
const unsigned int greetingDuration = 4000;
const unsigned int endingMessageDuration = 5000;
unsigned long startEndingMessage = 0;

byte indexText = 1;

// menu state: represent the menu that is displayed
// 0. main menu
// 1. start game
// 2. view leaderboard
// 3. settings (list of different settings)
// 4. about (info about the game and the creator of it)
// 5. how to play (info about the game)
byte menuState;

// the place where we are in the main menu
// the place where the symbol ">" is displays  (e.g. ">Start game" means mainMenuState = 0)
byte mainMenuState;

// like the menu above, we need to know where we are in a menu/list of texts
byte infoMenuState;
byte settingsMenuState;
byte howToPlayMenuState;
byte leaderboardMenuState;

bool nameAddedFromSettings = false;
bool nameAddedAfterPlaying;
String playerName = "AAAAA";  // global name, created/updated in settings menu
String temporaryName;         // is necessary for creating a temporary name for a new game played
                              // by a person that not inserted a name in settings
byte yJoystickPosition = 0;   // for changing letters from player name
byte difficulty = 0;
byte currentGameDifficulty;
bool sound;
bool gameOverSoundWasPlayed = false;

// values for brightness and contrast
byte lcdBrightness;
byte matrixBrightness;
byte lcdContrast;

// brightnessItem has 2 values:
// 0 if we are on the lcd brightness element
// 1 if we are on the matrix brightness element
byte brightnessItem;

byte numberOfHighscores;
String leaderboardNames[5] = {};
int highscores[5] = {};
bool checkHighscore = false;
byte leaderboardPosition;

// game setup
byte destroyerPosition;
byte lastDestroyerPosition;
bool buttonPressed = false;

// 0 means that there is no meteorite / bullet
// 1 that it takes 1 bullet to break the meteorite
// 2 that it takes 2 bullets to break the meteorite
byte meteorites[8][8] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};
struct {
  byte x, y;
} bullets[20], boss[20];
byte numberOfBullets = 0;
byte lenghtBoss;

byte wait;
byte numberOfMeteoritePerGeneration = 1;
byte numberOfMeteoritesLives;

unsigned long generatingMeteoriteTime = 5000;
unsigned long lastGeneratingMeteoriteTime = 0;
const byte bulletSpeed = 100;
unsigned long lastBulletMovement = 0;

bool playing;
long score;
byte lives;
byte step;
bool isBoss;
byte killedMeteorites;

// blinking interval
const int blinkTime = 400;
unsigned long lastBlink = 0;

byte arrowDown[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b10001,
  0b01010,
  0b00100
};

byte speaker[8] = {
  0b00000,
  0b00010,
  0b00110,
  0b01110,
  0b01110,
  0b00110,
  0b00010,
  0b00000
};

byte soundOff[8] = {
  0b00000,
  0b00000,
  0b10010,
  0b01100,
  0b01100,
  0b10010,
  0b00000,
  0b00000
};

byte soundOn[8] = {
  0b11000,
  0b00100,
  0b10010,
  0b01010,
  0b01010,
  0b10010,
  0b00100,
  0b11000
};

byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

byte displayImageOnMatrix;
byte lastDisplayImageOnMatrix;
const uint8_t matrixImages[][8] = {
  { 0b00000000,
    0b01001010,
    0b01001000,
    0b01001010,
    0b01111010,
    0b01001010,
    0b01001010,
    0b01001010 },  // HI
  { 0b00000000,
    0b00000000,
    0b00100100,
    0b00100100,
    0b00000000,
    0b00111100,
    0b01000010,
    0b00000000 },  // sad face, position 1
  { 0b00000000,
    0b00000000,
    0b00100100,
    0b00100100,
    0b00000000,
    0b01000010,
    0b00111100,
    0b00000000 },  // happy face, position 2
  { 0b00000000,
    0b00111100,
    0b01111110,
    0b11011011,
    0b11111111,
    0b11111111,
    0b10100101,
    0b00000000 },  // monster, position 3
  { 0b01111110,
    0b01111110,
    0b00111100,
    0b00111100,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00111100 },  // trophy, position 4
  { 0b00001100,
    0b00011000,
    0b00011001,
    0b00011111,
    0b00111110,
    0b01110000,
    0b11100000,
    0b11000000 },  // settings image, position 5
  { 0b00000000,
    0b00011000,
    0b00011000,
    0b00000000,
    0b00111000,
    0b00011000,
    0b00011000,
    0b00011100 },  // about/info section, position 6
  { 0b00111100,
    0b01100110,
    0b01100110,
    0b00001100,
    0b00011000,
    0b00011000,
    0b00000000,
    0b00011000 }  // how to play, position 7
};

void setup() {
  lcd.begin(16, 2);
  pinMode(pinSW, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);

  // setupEEPROM();  // run only once to insert default values in eeprom
  readDataFromEEPROM();

  // set the value for lcd and lc from eeprom
  pinMode(lcdBrightnessControlPin, OUTPUT);
  analogWrite(lcdBrightnessControlPin, lcdBrightness);
  pinMode(lcdContrastControlPin, OUTPUT);
  analogWrite(lcdContrastControlPin, lcdContrast);

  lc.shutdown(0, false);                 // turn off power saving, enables display
  lc.setIntensity(0, matrixBrightness);  // sets brightness (0~15 possible values)

  startGreetingTime = millis();
  menuState = 0;
  mainMenuState = 0;

  lcd.createChar(7, arrowDown);

  changeMatrixImage(0);  // welcome image for matrix
  displayImageOnMatrix = 0;
  lastDisplayImageOnMatrix = 0;
}

void loop() {
  if (displayImageOnMatrix != lastDisplayImageOnMatrix) {
    changeMatrixImage(displayImageOnMatrix);
    lastDisplayImageOnMatrix = displayImageOnMatrix;
  }

  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  readingPinSW = digitalRead(pinSW);
  if (readingPinSW != lastSwState) {
    lastDebounceTime = millis();
  }

  readingButtonPin = digitalRead(buttonPin);
  if (readingButtonPin != lastButtonState) {
    lastDebounceTimeForButton = millis();
  }

  if (millis() - startGreetingTime < greetingDuration) {  // greeting message/image displayed for a few moments
    lcd.setCursor(3, 0);
    lcd.print("Meteorite");
    lcd.setCursor(5, 1);
    lcd.print("shower");
  } else {
    if (millis() - lastDebounceTime > debounceDelay) {
      if (readingPinSW != swState) {
        swState = readingPinSW;
      }
    }
    lastSwState = readingPinSW;

    if (millis() - lastDebounceTimeForButton > debounceDelay) {
      if (readingButtonPin != buttonState) {
        buttonState = readingButtonPin;
      }
    }
    lastButtonState = readingButtonPin;

    // check what menu is displayed
    if (menuState == 0) {  // main menu
      delay(30);
      switch (mainMenuState) {
        case 0:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(">Start Game");
          lcd.setCursor(1, 1);
          lcd.print("Leaderboard");

          lcd.setCursor(15, 1);  // down arrow
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW) {  // button pressed => game starts
            playing = true;
            setupForGame();  // set variables to default/start values
            lcd.createChar(0, heart);

            menuState = 1;
            swState = HIGH;
            lastSwState = HIGH;
          }

          displayImageOnMatrix = 3;
          mainMenuState = setMenuState(mainMenuState, 1, mainMenuState);
          break;

        case 1:
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Start Game");
          lcd.setCursor(0, 1);
          lcd.print(">Leaderboard");

          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW) {
            leaderboardMenuState = 0;
            menuState = 2;
            swState = HIGH;
            lastSwState = HIGH;
          }

          displayImageOnMatrix = 4;
          mainMenuState = setMenuState(mainMenuState, 2, 0);
          break;

        case 2:
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Leaderboard");
          lcd.setCursor(0, 1);
          lcd.print(">Settings");

          lcd.setCursor(15, 0);  // up arrow
          lcd.print("^");
          lcd.setCursor(15, 1);  // down arroe
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW) {
            settingsMenuState = 0;
            menuState = 3;
            swState = HIGH;
            lastSwState = HIGH;
          }

          displayImageOnMatrix = 5;
          mainMenuState = setMenuState(mainMenuState, 4, 3);
          break;

        case 3:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(">Leaderboard");
          lcd.setCursor(1, 1);
          lcd.print("Settings");

          lcd.setCursor(15, 0);
          lcd.print("^");
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW) {
            leaderboardMenuState = 0;
            menuState = 2;
            swState = HIGH;
            lastSwState = HIGH;
          }

          displayImageOnMatrix = 4;
          mainMenuState = setMenuState(mainMenuState, 2, 0);
          break;

        case 4:
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Settings");
          lcd.setCursor(0, 1);
          lcd.print(">About");

          lcd.setCursor(15, 0);
          lcd.print("^");
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW) {
            infoMenuState = 0;
            menuState = 4;
            swState = HIGH;
            lastSwState = HIGH;
          }

          displayImageOnMatrix = 6;
          mainMenuState = setMenuState(mainMenuState, 6, 5);
          break;

        case 5:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(">Settings");
          lcd.setCursor(1, 1);
          lcd.print("About");

          lcd.setCursor(15, 0);
          lcd.print("^");
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW) {
            settingsMenuState = 0;
            menuState = 3;
            swState = HIGH;
            lastSwState = HIGH;
          }

          displayImageOnMatrix = 5;
          mainMenuState = setMenuState(mainMenuState, 4, 3);
          break;

        case 6:
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("About");
          lcd.setCursor(0, 1);
          lcd.print(">How to play");

          lcd.setCursor(15, 0);
          lcd.print("^");
          delay(50);

          if (swState == LOW) {
            howToPlayMenuState = 0;
            menuState = 5;
            swState = HIGH;
            lastSwState = HIGH;
          }

          displayImageOnMatrix = 7;
          mainMenuState = setMenuState(mainMenuState, mainMenuState, 7);
          break;

        case 7:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(">About");
          lcd.setCursor(1, 1);
          lcd.print("How to play");

          lcd.setCursor(15, 0);
          lcd.print("^");
          delay(50);

          if (swState == LOW) {
            infoMenuState = 0;
            menuState = 4;
            swState = HIGH;
            lastSwState = HIGH;
          }

          displayImageOnMatrix = 6;
          mainMenuState = setMenuState(mainMenuState, 6, 5);
          break;
      }
    } else if (menuState == 1) {  // start game
      if (playing) {
        displayDestroyer();
        destroyerMovement();
        meteoritesMovement();
        gameOverCases();
        progressDifficulty();

        if (buttonState == LOW && !buttonPressed) {  // one bullet is fired when the button is pressed, so the player pressing time should not make any difference.
          bullets[numberOfBullets].x = 6;            // fire a bullet
          bullets[numberOfBullets].y = destroyerPosition;
          numberOfBullets++;
          buttonPressed = true;
        } else if (buttonState == HIGH) {
          buttonPressed = false;
        }

        if (millis() - lastBulletMovement > bulletSpeed) {
          bulletsMovement();
        }

        displayInGameStats();  // display relevant info: name, difficulty, lives, score

        if (!playing) {                            // game over
          displayImageOnMatrix = 1;                // sad face
          if (sound && !gameOverSoundWasPlayed) {  // game over sound
            gameOverSoundWasPlayed = true;
            gameOverSound();
          }
          turnOffOnMatrix();
          startEndingMessage = millis();
        }
      } else {
        gameEnded();
        if (checkHighscore) {  // display messages for the player status
          lcd.clear();
          if (leaderboardPosition < 5) {
            displayImageOnMatrix = 2;  // happy face
            lcd.setCursor(2, 0);
            lcd.print("You made it!");
            if (leaderboardPosition == 0) {
              lcd.setCursor(1, 1);
              lcd.print("Highest score!");
            } else {
              lcd.setCursor(3, 1);
              lcd.print("Top 5! :)");
            }
          } else {
            lcd.setCursor(1, 0);
            lcd.print("Your score is");
            lcd.setCursor(0, 1);
            lcd.print("not good enought");
          }
          delay(50);

          if (swState == LOW) {
            checkHighscore = false;
            menuState = 0;
            swState = HIGH;
            lastSwState = HIGH;
          }
        }
      }
    } else if (menuState == 2) {  // view leaderboard; first 5 hightscores
      if (numberOfHighscores == 0) {
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("Leaderboard");
        lcd.setCursor(0, 1);
        lcd.print("Nothing here...");
        delay(50);
      } else {
        switch (leaderboardMenuState) {
          case 0:
            lcd.clear();
            lcd.setCursor(3, 0);
            lcd.print("Leaderboard");

            lcd.setCursor(0, 1);
            lcd.print("1." + leaderboardNames[0]);
            lcd.setCursor(11, 1);
            lcd.print(highscores[0]);

            delay(50);
            leaderboardMenuState = setMenuState(leaderboardMenuState, 1, leaderboardMenuState);
            break;

          case 1:
            lcd.clear();

            lcd.setCursor(0, 0);
            lcd.print("1." + leaderboardNames[0]);
            lcd.setCursor(11, 0);
            lcd.print(highscores[0]);

            lcd.setCursor(0, 1);
            if (numberOfHighscores > 1) {
              lcd.print("2." + leaderboardNames[1]);
              lcd.setCursor(11, 1);
              lcd.print(highscores[1]);
            } else {
              lcd.print("2.");
            }

            delay(50);
            leaderboardMenuState = setMenuState(leaderboardMenuState, 2, 0);
            break;

          case 2:
            lcd.clear();

            lcd.setCursor(0, 0);
            if (numberOfHighscores > 1) {
              lcd.print("2." + leaderboardNames[1]);
              lcd.setCursor(11, 0);
              lcd.print(highscores[1]);
            } else {
              lcd.print("2.");
            }

            lcd.setCursor(0, 1);
            if (numberOfHighscores > 2) {
              lcd.print("3." + leaderboardNames[2]);
              lcd.setCursor(11, 1);
              lcd.print(highscores[2]);
            } else {
              lcd.print("3.");
            }

            delay(50);
            leaderboardMenuState = setMenuState(leaderboardMenuState, 3, 1);
            break;

          case 3:
            lcd.clear();

            lcd.setCursor(0, 0);
            if (numberOfHighscores > 2) {
              lcd.print("3." + leaderboardNames[2]);
              lcd.setCursor(11, 0);
              lcd.print(highscores[2]);
            } else {
              lcd.print("3.");
            }

            lcd.setCursor(0, 1);
            if (numberOfHighscores > 3) {
              lcd.print("4." + leaderboardNames[3]);
              lcd.setCursor(11, 1);
              lcd.print(highscores[3]);
            } else {
              lcd.print("4.");
            }

            delay(50);
            leaderboardMenuState = setMenuState(leaderboardMenuState, 4, 2);
            break;

          case 4:
            lcd.clear();

            lcd.setCursor(0, 0);
            if (numberOfHighscores > 3) {
              lcd.print("4." + leaderboardNames[3]);
              lcd.setCursor(11, 0);
              lcd.print(highscores[3]);
            } else {
              lcd.print("4.");
            }

            lcd.setCursor(0, 1);
            if (numberOfHighscores > 4) {
              lcd.print("5." + leaderboardNames[4]);
              lcd.setCursor(11, 1);
              lcd.print(highscores[4]);
            } else {
              lcd.print("5.");
            }

            delay(50);
            leaderboardMenuState = setMenuState(leaderboardMenuState, 5, 3);
            break;

          case 5:
            lcd.clear();

            lcd.setCursor(0, 0);
            if (numberOfHighscores > 4) {
              lcd.print("5." + leaderboardNames[4]);
              lcd.setCursor(11, 0);
              lcd.print(highscores[4]);
            } else {
              lcd.print("5.");
            }

            lcd.setCursor(0, 1);
            lcd.print("Press to exit");

            delay(50);
            leaderboardMenuState = setMenuState(leaderboardMenuState, leaderboardMenuState, 4);
            break;
        }
      }
      if (swState == LOW) {  // in this case, whenever the player wants to exit leaderborad list, he can press the button and go back
        menuState = 0;
        swState = HIGH;
        lastSwState = HIGH;
      }
    } else if (menuState == 3) {  // clicked on setting, show settings menu
      switch (settingsMenuState) {
        case 0:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(">Enter name");
          lcd.setCursor(1, 1);
          lcd.print("Difficulty");

          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW) {
            yJoystickPosition = 0;
            settingsMenuState = 12;  // go to enter name section
            swState = HIGH;
            lastSwState = HIGH;
          }

          settingsMenuState = setMenuState(settingsMenuState, 1, settingsMenuState);
          break;

        case 1:
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Enter name");
          lcd.setCursor(0, 1);
          lcd.print(">Difficulty");

          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW) {
            settingsMenuState = 13;  // go to set difficulty section
            swState = HIGH;
            lastSwState = HIGH;
          }

          settingsMenuState = setMenuState(settingsMenuState, 2, 0);
          break;

        case 2:
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Difficulty");
          lcd.setCursor(0, 1);
          lcd.print(">LCD contrast");

          lcd.setCursor(15, 0);
          lcd.print("^");
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW) {
            settingsMenuState = 14;  // go to set lcd contrast
            swState = HIGH;
            lastSwState = HIGH;
          }

          settingsMenuState = setMenuState(settingsMenuState, 5, 3);
          break;

        case 3:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(">Difficulty");
          lcd.setCursor(1, 1);
          lcd.print("LCD contrast");

          lcd.setCursor(15, 0);
          lcd.print("^");
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW) {
            settingsMenuState = 13;  // go to set difficulty section
            swState = HIGH;
            lastSwState = HIGH;
          }

          settingsMenuState = setMenuState(settingsMenuState, 2, 0);
          break;

        case 4:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(">LCD contrast");
          lcd.setCursor(1, 1);
          lcd.print("Brightness");

          lcd.setCursor(15, 0);
          lcd.print("^");
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW) {
            settingsMenuState = 14;  // go to set lcd contrast
            swState = HIGH;
            lastSwState = HIGH;
          }

          settingsMenuState = setMenuState(settingsMenuState, 5, 3);
          break;

        case 5:
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("LCD contrast");
          lcd.setCursor(0, 1);
          lcd.print(">Brightness");

          lcd.setCursor(15, 0);
          lcd.print("^");
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW) {
            brightnessItem = 0;
            settingsMenuState = 15;  // go to set brightness for lcd and matrix section
            swState = HIGH;
            lastSwState = HIGH;
          }

          settingsMenuState = setMenuState(settingsMenuState, 6, 4);
          break;

        case 6:
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Brightness");
          lcd.setCursor(0, 1);
          lcd.print(">Sound");

          lcd.setCursor(15, 0);
          lcd.print("^");
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW) {
            settingsMenuState = 16;  // go to set sound section
            swState = HIGH;
            lastSwState = HIGH;
          }

          settingsMenuState = setMenuState(settingsMenuState, 8, 7);
          break;

        case 7:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(">Brightness");
          lcd.setCursor(1, 1);
          lcd.print("Sound");

          lcd.setCursor(15, 0);
          lcd.print("^");
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW) {
            brightnessItem = 0;
            settingsMenuState = 15;  // go to set brightness for lcd and matrix section
            swState = HIGH;
            lastSwState = HIGH;
          }

          settingsMenuState = setMenuState(settingsMenuState, 6, 4);
          break;

        case 8:
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Sound");
          lcd.setCursor(0, 1);
          lcd.print(">Reset hscore");

          lcd.setCursor(15, 0);
          lcd.print("^");
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW) {
            settingsMenuState = 17;
            swState = HIGH;
            lastSwState = HIGH;
          }

          settingsMenuState = setMenuState(settingsMenuState, 10, 9);
          break;

        case 9:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(">Sound");
          lcd.setCursor(1, 1);
          lcd.print("Reset hscore");

          lcd.setCursor(15, 0);
          lcd.print("^");
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW) {
            settingsMenuState = 16;
            swState = HIGH;
            lastSwState = HIGH;
          }

          settingsMenuState = setMenuState(settingsMenuState, 8, 7);
          break;

        case 10:
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Reset hscore");
          lcd.setCursor(0, 1);
          lcd.print(">Press to exit");  // exit "button"

          lcd.setCursor(15, 0);
          lcd.print("^");
          delay(50);

          if (swState == LOW) {
            menuState = 0;  // go back to main menu
            swState = HIGH;
            lastSwState = HIGH;
          }

          settingsMenuState = setMenuState(settingsMenuState, settingsMenuState, 11);
          break;

        case 11:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print(">Reset hscore");  // reset leaderboard
          lcd.setCursor(1, 1);
          lcd.print("Press to exit");

          lcd.setCursor(15, 0);
          lcd.print("^");
          delay(50);

          if (swState == LOW) {
            settingsMenuState = 17;  // go to set sound section
            swState = HIGH;
            lastSwState = HIGH;
          }

          settingsMenuState = setMenuState(settingsMenuState, 10, 9);
          break;

        case 12:                         // enter/edit name (be pressing the joystick from case 0 - enter name)
          nameAddedFromSettings = true;  // if we are here means that the player choose to add his name
          nameAddedAfterPlaying = false;

          playerName = setName(playerName);
          if (swState == LOW && yJoystickPosition == 5) {  // yJoystickPosition == 5 if the player wants to save the current his name
            settingsMenuState = 0;                         // go back to settings menu, display 0
            swState = HIGH;
            lastSwState = HIGH;
          }
          break;

        case 13:  // set difficulty
          setDifficulty();
          if (swState == LOW) {
            settingsMenuState = 3;  // go back to settings menu
            swState = HIGH;
            lastSwState = HIGH;
          }
          break;

        case 14:  // lcd contrast control
          setContrast();
          if (swState == LOW) {  // go back to settings menu to "LCD Contrast" line and save the edited info
            EEPROM.put(3, lcdContrast);
            settingsMenuState = 4;
            swState = HIGH;
            lastSwState = HIGH;
          }
          break;

        case 15:  // brightness for lcd and matrix settings menu
          setBrightness();
          if (swState == LOW) {  // go back to settings menu to "Brightness" line and save the edited info
            EEPROM.put(0, lcdBrightness);
            EEPROM.put(1, matrixBrightness);
            settingsMenuState = 7;
            swState = HIGH;
            lastSwState = HIGH;
          }
          break;

        case 16:  // set sound on/off
          lcd.createChar(0, speaker);
          lcd.createChar(1, soundOn);
          lcd.createChar(2, soundOff);

          setSound();
          if (swState == LOW) {
            EEPROM.put(2, sound);  // save the sound setting
            settingsMenuState = 9;
            swState = HIGH;
            lastSwState = HIGH;
          }
          break;

        case 17:  // reset leaderboard
          lcd.clear();
          lcd.setCursor(2, 0);
          lcd.print("Highscore");
          lcd.setCursor(5, 1);
          lcd.print("reseted!");
          delay(50);

          numberOfHighscores = 0;
          EEPROM.put(19, numberOfHighscores);

          if (swState == LOW) {  // go back to settings menu to "Reset hscore"
            settingsMenuState = 11;
            swState = HIGH;
            lastSwState = HIGH;
          }
          break;
      }
    } else if (menuState == 4) {  // about section
      switch (infoMenuState) {
        case 0:
          lcd.clear();
          displayText("Author: Denisa Predescu", 1, 1, false);
          displayText("Game: Meteorite shower", 1, 0, true);
          delay(50);

          infoMenuState = setMenuState(infoMenuState, 1, infoMenuState);
          break;

        case 1:
          lcd.clear();
          displayText("Game: Meteorite shower", 1, 0, false);
          displayText("Author: Denisa Predescu", 1, 1, true);
          delay(50);

          infoMenuState = setMenuState(infoMenuState, 2, 0);
          break;

        case 2:
          lcd.clear();
          displayText("Author: Denisa Predescu", 1, 0, false);
          displayText("GitHub: https://github.com/denisapredescu/Meteorite-shower", 1, 1, true);
          delay(50);

          infoMenuState = setMenuState(infoMenuState, 4, 3);
          break;

        case 3:
          lcd.clear();
          displayText("GitHub: https://github.com/denisapredescu/Meteorite-shower", 1, 1, false);
          displayText("Author: Denisa Predescu", 1, 0, true);
          delay(50);

          infoMenuState = setMenuState(infoMenuState, 2, 0);
          break;

        case 4:
          lcd.clear();
          displayText("GitHub: https://github.com/denisapredescu", 1, 0, false);
          displayText("Project for Introduction to Robotics, Unibuc", 1, 1, true);
          delay(50);

          infoMenuState = setMenuState(infoMenuState, 6, 5);
          break;

        case 5:
          lcd.clear();
          displayText("Project for Introduction to Robotics, Unibuc", 1, 1, false);
          displayText("GitHub: https://github.com/denisapredescu", 1, 0, true);
          delay(50);

          infoMenuState = setMenuState(infoMenuState, 4, 3);
          break;

        case 6:
          lcd.clear();
          displayText("Project for Introduction to Robotics, Unibuc", 1, 0, false);
          displayText("Press to exit", 1, 1, true);
          delay(50);

          if (swState == LOW) {
            menuState = 0;
            swState = HIGH;
            lastSwState = HIGH;
          }
          infoMenuState = setMenuState(infoMenuState, infoMenuState, 7);
          break;

        case 7:
          lcd.clear();
          displayText("Press to exit", 1, 1, false);
          displayText("Project for Introduction to Robotics, Unibuc", 1, 0, true);
          delay(50);
          infoMenuState = setMenuState(infoMenuState, 6, 5);
          break;
      }
    } else if (menuState == 5) {  // how to play section
      // displayed text:
      // The earth is under a meteorite shower and we are the last hope for the humanity!
      // Destroy the meteorites before they hit earth.
      // The  difficulty increases as more enemies are killed.
      // You have 3 lives. If you die, the game restarts at the first step of the current difficulty.
      switch (howToPlayMenuState) {
        case 0:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("The earth is");
          lcd.setCursor(0, 1);
          lcd.print("under a");

          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          howToPlayMenuState = setMenuState(howToPlayMenuState, 1, howToPlayMenuState);
          break;

        case 1:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("meteorite ");
          lcd.setCursor(0, 1);
          lcd.print("shower and we");

          lcd.setCursor(15, 0);
          lcd.print("^");
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          howToPlayMenuState = setMenuState(howToPlayMenuState, 2, 0);
          break;

        case 2:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("are the last");
          lcd.setCursor(0, 1);
          lcd.print("hope for the");

          lcd.setCursor(15, 0);
          lcd.print("^");
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          howToPlayMenuState = setMenuState(howToPlayMenuState, 3, 1);
          break;

        case 3:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("humanity!");
          lcd.setCursor(0, 1);
          lcd.print("Destroy the");

          lcd.setCursor(15, 0);
          lcd.print("^");
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          howToPlayMenuState = setMenuState(howToPlayMenuState, 4, 2);
          break;

        case 4:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("meteorites");
          lcd.setCursor(0, 1);
          lcd.print("before they hit");

          lcd.setCursor(15, 0);
          lcd.print("^");
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          howToPlayMenuState = setMenuState(howToPlayMenuState, 5, 3);
          break;

        case 5:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("earth! The");

          lcd.setCursor(0, 1);
          lcd.print("difficulty ");

          lcd.setCursor(15, 0);
          lcd.print("^");
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          howToPlayMenuState = setMenuState(howToPlayMenuState, 6, 4);
          break;

        case 6:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("increases as");

          lcd.setCursor(0, 1);
          lcd.print("more enemies");

          lcd.setCursor(15, 0);
          lcd.print("^");
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          howToPlayMenuState = setMenuState(howToPlayMenuState, 7, 5);
          break;

        case 7:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("are killed. You");

          lcd.setCursor(0, 1);
          lcd.print("have 3 lives.If");

          lcd.setCursor(15, 0);
          lcd.print("^");
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          howToPlayMenuState = setMenuState(howToPlayMenuState, 8, 6);
          break;

        case 8:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("you die, the ");

          lcd.setCursor(0, 1);
          lcd.print("game restarts");

          lcd.setCursor(15, 0);
          lcd.print("^");
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          howToPlayMenuState = setMenuState(howToPlayMenuState, 9, 7);
          break;

        case 9:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("at the first");

          lcd.setCursor(0, 1);
          lcd.print("step of the");

          lcd.setCursor(15, 0);
          lcd.print("^");
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          howToPlayMenuState = setMenuState(howToPlayMenuState, 10, 8);
          break;

        case 10:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("current");

          lcd.setCursor(0, 1);
          lcd.print("difficulty.");

          lcd.setCursor(15, 0);
          lcd.print("^");
          delay(50);

          howToPlayMenuState = setMenuState(howToPlayMenuState, howToPlayMenuState, 9);
          break;
      }
      if (swState == LOW) {
        menuState = 0;
        swState = HIGH;
        lastSwState = HIGH;
      }
    }
  }
}

// function that moves the display up, down or stay still
int setMenuState(int stay, int goDown, int goUp) {
  if (xValue < minThreshold && !xJoystickState) {
    if (goDown == stay) {
      canNotMoveJoystickSound();
    } else {
      moveJoystickSound();
    }
    indexText = 1;  // for scrollable text that can be found in list (e.g. about section)
    xJoystickState = true;
    return goDown;
  }
  if (xValue > maxThreshold && !xJoystickState) {
    if (goUp == stay) {
      canNotMoveJoystickSound();
    } else {
      moveJoystickSound();
    }
    indexText = 1;
    xJoystickState = true;
    return goUp;
  }
  if (xValue < maxThreshold && xValue > minThreshold) {
    xJoystickState = false;
    return stay;
  }
}

String setName(String name) {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print(name);
  lcd.setCursor(12, 0);
  lcd.print("save");

  if (yJoystickPosition < 5) {
    lcd.setCursor(1 + yJoystickPosition, 1);
    lcd.print("^");
  } else {
    lcd.setCursor(11, 0);
    lcd.print(">");
  }
  delay(50);

  movingBetweenPositions();  // joystick movement
  if (yJoystickPosition < 5) {
    name = movingBetweenLetters(name);  // A - Z
  }
  return name;
}

String movingBetweenLetters(String name) {
  if (xValue < minThreshold && yValue > minThreshold && yValue < maxThreshold && !xJoystickState) {  //DOWN
    if (name[yJoystickPosition] != 'A') {
      name[yJoystickPosition] = (char)(name[yJoystickPosition] - 1);
    } else {
      name[yJoystickPosition] = 'Z';
    }
    xJoystickState = true;
    moveJoystickSound();
  }
  if (xValue > maxThreshold && yValue > minThreshold && yValue < maxThreshold && !xJoystickState) {  //UP
    if (name[yJoystickPosition] != 'Z') {
      name[yJoystickPosition] = (char)(name[yJoystickPosition] + 1);
    } else {
      name[yJoystickPosition] = 'A';
    }
    xJoystickState = true;
    moveJoystickSound();
  }
  if (xValue < maxThreshold && xValue > minThreshold) {
    xJoystickState = false;
  }
  return name;
}

void movingBetweenPositions() {
  if (yValue < minThreshold && xValue > minThreshold && xValue < maxThreshold && !yJoystickState) {
    yJoystickState = true;
    if (yJoystickPosition > 0) {
      yJoystickPosition -= 1;
      moveJoystickSound();
    } else {
      canNotMoveJoystickSound();
    }
    // yJoystickPosition = yJoystickPosition > 0 ? yJoystickPosition - 1 : 0;
  }
  if (yValue > maxThreshold && xValue > minThreshold && xValue < maxThreshold && !yJoystickState) {
    yJoystickState = true;
    if (yJoystickPosition < 5) {
      yJoystickPosition += 1;
      moveJoystickSound();
    } else {
      canNotMoveJoystickSound();
    }
    // yJoystickPosition = yJoystickPosition < 5 ? yJoystickPosition + 1 : 5;
  }
  if (yValue < maxThreshold && yValue > minThreshold) {
    yJoystickState = false;
  }
}

void setDifficulty() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("low medium high");

  if (difficulty == 0) {
    lcd.setCursor(1, 1);
  } else if (difficulty == 1) {
    lcd.setCursor(6, 1);
  } else if (difficulty == 2) {
    lcd.setCursor(13, 1);
  }
  lcd.print("^");
  delay(50);

  movingSelectedDifficulty();
}

void movingSelectedDifficulty() {  // moving left right
  if (yValue < minThreshold && xValue > minThreshold && xValue < maxThreshold && !yJoystickState) {
    yJoystickState = true;
    difficulty = difficulty - 1;
    if (difficulty == 255) {
      difficulty = 2;
    }
    moveJoystickSound();
  }
  if (yValue > maxThreshold && xValue > minThreshold && xValue < maxThreshold && !yJoystickState) {
    yJoystickState = true;
    difficulty = (difficulty + 1) % 3;
    moveJoystickSound();
  }
  if (yValue < maxThreshold && yValue > minThreshold) {
    yJoystickState = false;
  }
}

void setSound() {
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.write((byte)0);  // speaker
  lcd.write((byte)1);  // on

  lcd.setCursor(4, 1);
  lcd.write((byte)0);  // speaker
  lcd.write((byte)2);  // off

  if (sound) {
    lcd.setCursor(2, 0);
  } else {
    lcd.setCursor(2, 1);
  }
  lcd.print(">");
  delay(50);

  movingOnOff();
}

void movingOnOff() {
  if (xValue < minThreshold && !xJoystickState) {
    xJoystickState = true;
    moveJoystickSound();
    sound = false;
  }
  if (xValue > maxThreshold && !xJoystickState) {
    xJoystickState = true;
    canNotMoveJoystickSound();
    sound = true;
  }
  if (xValue < maxThreshold && xValue > minThreshold) {
    xJoystickState = false;
  }
}

void setContrast() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LCD contrast");

  lcd.setCursor(8, 1);
  lcd.print("<");
  lcd.setCursor(14, 1);
  lcd.print(">");

  analogWrite(lcdContrastControlPin, lcdContrast);

  setContrastDisplay();
  delay(50);
  changingValueOfContrast();
}

void setContrastDisplay() {
  byte contrastLevel;

  if (lcdContrast == 30) {
    contrastLevel = 1;
  } else if (lcdContrast == 50) {
    contrastLevel = 2;
  } else if (lcdContrast == 70) {
    contrastLevel = 3;
  } else if (lcdContrast == 90) {
    contrastLevel = 4;
  } else if (lcdContrast == 110) {
    contrastLevel = 5;
  }

  for (int i = 0; i < contrastLevel; i++) {
    lcd.setCursor(9 + i, 1);
    lcd.print("I");
  }
}

// we move with 51 step for lcd contrast
void changingValueOfContrast() {
  if (yValue < minThreshold && xValue > minThreshold && xValue < maxThreshold && !yJoystickState) {  // move to left
    yJoystickState = true;
    if (lcdContrast != 30) {
      lcdContrast -= 20;
      moveJoystickSound();
    } else {
      canNotMoveJoystickSound();
    }
  }
  if (yValue > maxThreshold && xValue > minThreshold && xValue < maxThreshold && !yJoystickState) {  // move to right
    yJoystickState = true;
    if (lcdContrast != 110) {
      lcdContrast += 20;
      moveJoystickSound();
    } else {
      canNotMoveJoystickSound();
    }
  }
  if (yValue < maxThreshold && yValue > minThreshold) {
    yJoystickState = false;
  }
}

void setBrightness() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Brightness for:");

  lcd.setCursor(8, 1);
  lcd.print("<");
  lcd.setCursor(14, 1);
  lcd.print(">");

  lcd.setCursor(1, 1);
  if (brightnessItem == 0) {
    lcd.print("LCD");
    analogWrite(lcdBrightnessControlPin, lcdBrightness);
    lcd.setCursor(15, 1);
    lcd.write((byte)7);
  } else {
    lcd.print("Matrix");
    lc.setIntensity(0, matrixBrightness);
    lcd.setCursor(15, 0);
    lcd.print("^");
  }
  setBrightnessDisplay();
  delay(50);
  changingValueOfBrightness();
  movingBetweenBrightnessElements();
}

// LCD brightness take values between 0 and 255; the player can choose from 5 values: 51, 102, 153, 204, 255
// Matrix brightness take values between 0 and 15; the player can choose from 5 values: 3, 6, 9, 12, 15
void setBrightnessDisplay() {
  byte brightnessLevel;

  if (brightnessItem == 0) {
    brightnessLevel = lcdBrightness / 51;
  } else {
    brightnessLevel = matrixBrightness / 3;
  }
  for (int i = 0; i < brightnessLevel; i++) {
    lcd.setCursor(9 + i, 1);
    lcd.print("I");
  }
  for (int i = brightnessLevel; i < 5; i++) {
    lcd.setCursor(9 + i, 1);
    lcd.print(" ");
  }
}

// we move with 51 step for matrix brightness and 3 step for lcd brightness
void changingValueOfBrightness() {
  if (yValue < minThreshold && xValue > minThreshold && xValue < maxThreshold && !yJoystickState) {  // move to left
    yJoystickState = true;
    if (brightnessItem == 0) {
      if (lcdBrightness != 51) {
        lcdBrightness -= 51;
        moveJoystickSound();
      } else {
        canNotMoveJoystickSound();
      }
    } else {
      if (matrixBrightness != 3) {
        matrixBrightness -= 3;
        moveJoystickSound();
      } else {
        canNotMoveJoystickSound();
      }
    }
  }
  if (yValue > maxThreshold && xValue > minThreshold && xValue < maxThreshold && !yJoystickState) {  // move to right
    yJoystickState = true;
    if (brightnessItem == 0) {
      if (lcdBrightness != 255) {
        lcdBrightness += 51;
        moveJoystickSound();
      } else {
        canNotMoveJoystickSound();
      }
    } else {
      if (matrixBrightness != 15) {
        matrixBrightness += 3;
        moveJoystickSound();
      } else {
        canNotMoveJoystickSound();
      }
    }
  }
  if (yValue < maxThreshold && yValue > minThreshold) {
    yJoystickState = false;
  }
}

void movingBetweenBrightnessElements() {  // move between lcd brightness setup and matrix brightness setup
  if (xValue < minThreshold && yValue > minThreshold && yValue < maxThreshold && !xJoystickState) {
    xJoystickState = true;
    if (brightnessItem == 0) {
      brightnessItem = 1;
      moveJoystickSound();
    } else {
      canNotMoveJoystickSound();
    }
  }
  if (xValue > maxThreshold && yValue > minThreshold && yValue < maxThreshold && !xJoystickState) {
    xJoystickState = true;
    if (brightnessItem == 1) {
      brightnessItem = 0;
      moveJoystickSound();
    } else {
      canNotMoveJoystickSound();
    }
  }
  if (xValue < maxThreshold && xValue > minThreshold) {
    xJoystickState = false;
  }
}

void displayText(String text, byte column, byte row, bool scrollable) {
  if (!scrollable) {
    lcd.setCursor(column, row);
    lcd.print(text.substring(0, 15));
  } else {  // if a text is scrollable, the symbol ">" is on it
    lcd.setCursor(column - 1, row);
    lcd.print(">");
    lcd.setCursor(column, row);
    if (text.length() <= 15) {
      lcd.print(text);
    } else {
      String newText = text + "  ";
      if (indexText <= newText.length()) {
        scrollText(indexText, newText);
        indexText++;
      } else {
        indexText = 1;
      }
    }
  }
}

void scrollText(int startLetter, String message) {  // logic for scrolling a text
  if (message.length() - startLetter > 15) {
    lcd.print(message.substring(startLetter, startLetter + 15));
  } else {
    lcd.print(message.substring(startLetter, message.length()) + message.substring(0, 15 - message.length() + startLetter));
  }
  delay(250);
}

// run only once to insert first data in EEPROM
void setupEEPROM() {
  lcdBrightness = 102;
  matrixBrightness = 15;
  lcdContrast = 110;
  sound = true;
  numberOfHighscores = 0;

  EEPROM.put(0, lcdBrightness);
  EEPROM.put(1, matrixBrightness);
  EEPROM.put(2, sound);
  EEPROM.put(3, lcdContrast);
  EEPROM.put(19, numberOfHighscores);
}

// read all data from EEPROM
void readDataFromEEPROM() {
  lcdBrightness = EEPROM.read(0);
  matrixBrightness = EEPROM.read(1);
  sound = EEPROM.read(2);
  lcdContrast = EEPROM.read(3);
  numberOfHighscores = EEPROM.read(19);

  for (int i = 1; i <= numberOfHighscores; i++) {
    String player = "";
    long playerScore = 0;

    for (int j = 0; j < 5; j++) {
      player += (char)EEPROM.read(i * 10 + j);
    }

    byte firstByte = EEPROM.read(i * 10 + 6);
    byte secondByte = EEPROM.read(i * 10 + 7);
    playerScore = (firstByte * 256) + secondByte;

    leaderboardNames[i - 1] = player;
    highscores[i - 1] = playerScore;
  }
}

void destroyerMovement() {
  if (yValue < minThreshold && !yJoystickState) {  // go left
    yJoystickState = true;
    if (destroyerPosition != 0) {
      destroyerPosition -= 1;
    }
  }
  if (yValue > maxThreshold && !yJoystickState) {  // go right
    yJoystickState = true;
    if (destroyerPosition != 7) {  // the destroyer has lenght 3 so if the left of the destroyer is on column position 5, the rightmost part is on 7
      destroyerPosition += 1;
    }
  }
  if (yValue < maxThreshold && yValue > minThreshold) {
    yJoystickState = false;
  }
}

// playr   Dif:Medium
// iii          xxxxx

// Display while playing the game:
//     lives
//     scores
//     difficulty
//     player name  => if the name is set from settings menu
//                  => otherwise is shown a default name "PLAYR"
void displayInGameStats() {
  lcd.clear();
  lcd.setCursor(0, 0);
  if (nameAddedFromSettings) {  // the player has set a name for himself in settings menu
    lcd.print(playerName);
  } else {
    lcd.print("PLAYR");
    nameAddedAfterPlaying = true;
  }

  lcd.setCursor(6, 0);
  lcd.print("Dif:");

  lcd.setCursor(10, 0);
  if (currentGameDifficulty == 0) {
    lcd.print(" LOW ");
  } else if (currentGameDifficulty == 1) {
    lcd.print("MEDIUM");
  } else if (currentGameDifficulty == 2) {
    lcd.print(" HIGH ");
  }

  for (int i = 0; i < lives; i++) {
    lcd.setCursor(i, 1);
    lcd.write((byte)0);
  }

  if (killedMeteorites % 7 == 0 && killedMeteorites != 0 && step == 1 && currentGameDifficulty == 2) {
    lcd.setCursor(4, 1);
    lcd.print("L");  // L from "laser" => appears on display just when is active the laser shoot on difficulty high, step 2
  }

  lcd.setCursor(6, 1);
  lcd.print("Score:");
  lcd.setCursor(12, 1);
  lcd.print(score);

  delay(50);
}

void gameEnded() {
  lcd.clear();

  if (millis() - startEndingMessage < endingMessageDuration) {  // screen 1 is seen for a few seconds
    lcd.setCursor(3, 0);
    if (millis() - lastBlink > blinkTime) {
      lcd.print("GAME OVER");
    }
    if (millis() - lastBlink > 2 * blinkTime) {
      lcd.clear();
      lastBlink = millis();
    }

    lcd.setCursor(3, 1);
    lcd.print("Score:" + (String)score);
    delay(50);

    temporaryName = playerName;  // "name" will be the name entered in settings or "AAAAA" (default value)
                                 // if case of default, the player will have to set a "temporary" name
  } else {                       // screen 2
    if (!checkHighscore) {


      if (score == 0) {
        leaderboardPosition = 10;  // doesn't matter what position, just not one from 0 to 4
                                   // if the player start a game and he does not play (aka score = 0), he should not be in leaderboard even if the leaderboard is empty
      } else {
        leaderboardPosition = 0;
        while (leaderboardPosition < numberOfHighscores && score <= highscores[leaderboardPosition]) {  // search for the suitable position in leaderboard
          leaderboardPosition++;
        }
      }

      if (leaderboardPosition < 5) {
        displayImageOnMatrix = 2;     // happy face
        if (nameAddedAfterPlaying) {  // if player not inserted a name already, he has to do it now
          temporaryName = setName(temporaryName);

          if (swState == LOW && yJoystickPosition == 5) {  // the player sets his name and now he wants to exit
            nameAddedAfterPlaying = false;
            swState = HIGH;
            lastSwState = HIGH;
          }
        } else {  // the name is set, it can be saved to leaderboard
          if (numberOfHighscores < 5) {
            numberOfHighscores = numberOfHighscores + 1;
          }
          for (int i = numberOfHighscores - 1; i > leaderboardPosition; i--) {
            highscores[i] = highscores[i - 1];
            leaderboardNames[i] = leaderboardNames[i - 1];
          }
          highscores[leaderboardPosition] = score;
          leaderboardNames[leaderboardPosition] = temporaryName;

          writeLeaderboardToEEPROM();
          checkHighscore = true;
        }
      } else {
        checkHighscore = true;
      }
    }
  }
}

// save/update leaderboard to EEPROM from leaderboardNames and highscores vectors
void writeLeaderboardToEEPROM() {
  EEPROM.put(19, numberOfHighscores);

  for (int i = 1; i <= numberOfHighscores; i++) {
    for (int j = 0; j < 5; j++) {
      EEPROM.put(i * 10 + j, leaderboardNames[i - 1][j]);
      EEPROM.put(i * 10 + 7, 0);
    }

    int hix = highByte(highscores[i - 1]);
    int lox = lowByte(highscores[i - 1]);
    EEPROM.put(i * 10 + 6, hix);
    EEPROM.put(i * 10 + 7, lox);
  }
}

// function that is call when the game is over to reset the matrix
void turnOffOnMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, true);  // turns on LED at col, row
      delay(20);
    }
  }
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, false);  // turns off on LED
      delay(20);
    }
  }
}

void resetMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, false);  // turn off on LED
      meteorites[row][col] = 0;       // reset bullets matrix and meteorites matrix
    }
  }
}

//function that change the displayed image of the matrix
void changeMatrixImage(int index) {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, false);  // turns off the matrix
    }
  }
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, bitRead(matrixImages[index][row], 7 - col));
    }
  }
}

void displayDestroyer() {
  if (lastDestroyerPosition != destroyerPosition) {
    // turn off last position
    lc.setLed(0, 7, lastDestroyerPosition - 1, false);
    lc.setLed(0, 7, lastDestroyerPosition, false);
    lc.setLed(0, 7, lastDestroyerPosition + 1, false);
    lc.setLed(0, 6, lastDestroyerPosition, false);
    lastDestroyerPosition = destroyerPosition;
  }
  // turn on current position
  lc.setLed(0, 6, destroyerPosition, true);
  lc.setLed(0, 7, destroyerPosition, true);

  if (destroyerPosition == 0) {
    lc.setLed(0, 7, destroyerPosition + 1, true);
  } else if (destroyerPosition == 7) {
    lc.setLed(0, 7, destroyerPosition - 1, true);
  } else {
    lc.setLed(0, 7, destroyerPosition + 1, true);
    lc.setLed(0, 7, destroyerPosition - 1, true);
  }
}

void meteoritesMovement() {
  if (millis() - lastGeneratingMeteoriteTime > generatingMeteoriteTime) {
    if (!isBoss) {
      for (int nr = 0; nr < numberOfMeteoritePerGeneration; nr++) {  // if the game is on and the boss is off, means that it needs to be generated new meteorites
        byte newMeteoritePosition = random(matrixSize);

        for (int i = 0; i < matrixSize; i++) {
          if (meteorites[i][newMeteoritePosition] == 0) {
            lc.setLed(0, i, newMeteoritePosition, true);
            meteorites[i][newMeteoritePosition] = numberOfMeteoritesLives;
            break;
          }
        }
      }
    } else {
      if (wait > 0) {  // waits for the player to start shooting before the boss starts to move down
        wait--;
      } else {
        for (int i = 0; i < lenghtBoss; i++) {
          if (boss[i].x != 255) {
            lc.setLed(0, boss[i].x, boss[i].y, false);

            if (meteorites[boss[i].x][boss[i].y] == 0) {
              boss[i].x = -1;  // it was hit
            } else {
              lc.setLed(0, boss[i].x + 1, boss[i].y, true);
              meteorites[boss[i].x + 1][boss[i].y] = meteorites[boss[i].x][boss[i].y];
              meteorites[boss[i].x][boss[i].y] = 0;
              boss[i].x += 1;  // the boss is moving
            }
          }
        }

        for (int i = 0; i < lenghtBoss; i++) {
          if (boss[i].x == 255) {  // need to be removed; verify if is equal with 255 because is type byte => -1 = 255
            for (int j = i; j < lenghtBoss - 1; j++) {
              boss[j] = boss[j + 1];
            }
            lenghtBoss--;
            i--;
          }
        }
      }
    }
    lastGeneratingMeteoriteTime = millis();
  }
}

void bulletsMovement() {
  for (int i = 0; i < numberOfBullets; i++) {
    lc.setLed(0, bullets[i].x, bullets[i].y, false);
    bullets[i].x -= 1;  // bullet movement

    if (bullets[i].x >= 0 && bullets[i].x <= 7) {
      lc.setLed(0, bullets[i].x, bullets[i].y, true);

      if (meteorites[bullets[i].x][bullets[i].y] >= 1) {  // the bullet is on a meteorite
        hitMeteoriteSound();

        if (killedMeteorites % 7 == 0 && killedMeteorites != 0 && step == 1 && currentGameDifficulty == 2) {  // last difficulty, last step
          // the destroyes has a laser shoot (just one then he turns to normal bullets)
          if (!isBoss) {
            for (int j = bullets[i].x; j >= 0; j--) {
              if (meteorites[j][bullets[i].y] >= 1) {
                meteorites[j][bullets[i].y] = 0;
                lc.setLed(0, j, bullets[i].y, false);
                score += 3;
                killedMeteorites++;
              }
            }
          } else {
            for (int j = bullets[i].x; j >= 0; j--) {
              if (meteorites[j][bullets[i].y] >= 1) {
                meteorites[j][bullets[i].y] = 0;
                lc.setLed(0, j, bullets[i].y, false);
                score += 3;
                killedMeteorites++;
              }
            }
          }
        } else {  // the game is not on difficulty high, step 2 => the normal functionality
          meteorites[bullets[i].x][bullets[i].y] -= 1;
          if (meteorites[bullets[i].x][bullets[i].y] == 0) {
            lc.setLed(0, bullets[i].x, bullets[i].y, false);
            killedMeteorites++;

            if (currentGameDifficulty == 0) {  // the score increases just if the destroyer kills the meteorite
              score += 1;
            } else if (currentGameDifficulty == 1) {
              score += 2;
            } else if (currentGameDifficulty == 2) {
              score += 3;
            }
          }
        }
        bullets[i].x = -1;  // the bullets does not exist anymore
      }
    }
  }

  for (int i = 0; i < numberOfBullets; i++) {
    if (bullets[i].x == 255) {  // need to be removed; verify if is equal with 255 because is type byte => -1 = 255
      for (int j = i; j < numberOfBullets - 1; j++) {
        bullets[j] = bullets[j + 1];
      }
      numberOfBullets--;
      i--;
    }
  }
}

void gameOverCases() {
  if (meteorites[6][destroyerPosition] >= 1) {
    if (lives > 1) {
      loseALive();
    } else {
      playing = false;
    }
  }
  for (int j = 0; j < matrixSize - 1; j++) {
    if (meteorites[7][j] >= 1) {
      if (lives > 1) {
        loseALive();
      } else {
        playing = false;
      }
    }
  }
}

void loseALive() {
  loseALifeSound();

  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, false);  // turn off on LED
      meteorites[row][col] = 0;       // reset bullets and meteorites matrix
    }
  }

  lives--;
  numberOfBullets = 0;
  killedMeteorites = 0;

  if (!isBoss) {  // the player comes back to the start of the level just when he dies in the middle of it
    setupDifficulty();
  } else {  // if the boss is already present, the player needs to kill it in order to continue the game
    generateBoss();
  }
}

void setupForGame() {
  resetMatrix();

  destroyerPosition = 3;
  lastDestroyerPosition = 3;
  numberOfBullets = 0;
  score = 0;
  lives = 3;
  gameOverSoundWasPlayed = false;
  yJoystickPosition = 0;               // for setting the player name
  currentGameDifficulty = difficulty;  // the difficulty set up in settings menu is the start difficulty for the current game
  killedMeteorites = 0;                // the difficulty increase when the destroyer manage to kill
  isBoss = false;

  setupDifficulty();
}

void setupDifficulty() {
  step = 0;
  numberOfMeteoritesLives = 1;

  if (currentGameDifficulty == 0) {
    generatingMeteoriteTime = 4500;
  } else if (currentGameDifficulty == 1) {
    generatingMeteoriteTime = 3000;
    numberOfMeteoritePerGeneration = 2;
  } else if (currentGameDifficulty == 2) {
    generatingMeteoriteTime = 2000;
    numberOfMeteoritePerGeneration = 3;
  }
}

void progressDifficulty() {
  if (killedMeteorites == (currentGameDifficulty + 1) * 10 && !(currentGameDifficulty == 2 && step == 1) && !isBoss) {  // strategy if the game is normal (not boss)
    step = (step + 1) % 2;
    if (step == 0) {
      currentGameDifficulty++;
    }

    if (step == 1 && currentGameDifficulty == 0) {
      generatingMeteoriteTime = 3000;
    } else if (step == 0 && currentGameDifficulty == 1) {  // step 1 of difficulty MEDIUM
      increaseLevelSound();
      numberOfMeteoritePerGeneration = 2;
      isBoss = true;
      generateBoss();                                      // boss points 40
    } else if (step == 1 && currentGameDifficulty == 1) {  // step 2 of difficulty MEDIUM
      generatingMeteoriteTime = 2000;                      // from 3s to 2s
    } else if (step == 0 && currentGameDifficulty == 2) {  // step 1 difficulty high
      increaseLevelSound();
      numberOfMeteoritePerGeneration = 3;
      isBoss = true;
      generateBoss();  // boss points 60
    } else if (step == 1 && currentGameDifficulty == 2) {
      numberOfMeteoritesLives = 2;
    }

    killedMeteorites = 0;
  }

  //din cauza bombelor, poti lua mai multe puncte decat cele 30 necesare sa ajung la boss, deci in cazul aceasta aleg sa ajung la boss daca au fost omorati mai multi meteoriti decat 30
  if (!isBoss && currentGameDifficulty == 2 && step == 1 && killedMeteorites >= (currentGameDifficulty + 1) * 10) {
    isBoss = true;
    generateBoss();  // boss points 60
    killedMeteorites = 0;
  }

  if (killedMeteorites == 20 && isBoss) {  // a boss is killed if the player destroy every part of it (20)
    isBoss = false;
    killedMeteorites = 0;
  }
}

void generateBoss() {
  resetMatrix();

  boss[0].x = 3;  // "draw" the boss
  boss[0].y = 1;

  boss[1].x = 3;
  boss[1].y = 3;

  boss[2].x = 3;
  boss[2].y = 4;

  boss[3].x = 3;
  boss[3].y = 6;

  for (int i = 4; i < 10; i++) {
    boss[i].x = 2;
    boss[i].y = i - 3;
  }

  for (int i = 10; i < 16; i++) {
    boss[i].x = 1;
    boss[i].y = i - 9;
  }

  for (int i = 16; i < 20; i++) {
    boss[i].x = 0;
    boss[i].y = i - 14;
  }

  if (currentGameDifficulty == 2 && step == 1) {  // highest difficulty, the boss needs to stay "up" more time so the player has a chance to win
    wait = 5;
  } else if (currentGameDifficulty == 2 && step == 0) {  // the boss from the ending of difficulty MEDIUM (aka begining of dif HIGHT); the same boss as the one from the ending of difficulty LOW, but now it comes faster
    wait = 1;
  } else {
    wait = 3;
  }

  numberOfBullets = 0;
  lenghtBoss = 20;

  for (int i = 0; i < lenghtBoss; i++) {
    meteorites[boss[i].x][boss[i].y] = numberOfMeteoritesLives;
    lc.setLed(0, boss[i].x, boss[i].y, true);
  }
}

// sound for moving the joystick (up - down - left - right) in menu - if the sound setting is on
void moveJoystickSound() {
  if (sound) {
    tone(buzzer, 30, 40);
  }
}

// sound for moving the joystick in a direction that can not be moved - if the sound setting is on
void canNotMoveJoystickSound() {
  if (sound) {
    tone(buzzer, NOTE_B4, 100);
    delay(150);
    tone(buzzer, NOTE_A4, 100);
    delay(150);
  }
}

// sound for losing a life - if the sound setting is on
void loseALifeSound() {
  if (sound) {
    tone(buzzer, NOTE_E6, 850);  // cand pierzi o viata
    delay(800);
    noTone(buzzer);
  }
}

// sound for upgrading to a higher difficulty - if the sound setting is on
void increaseLevelSound() {
  if (sound) {
    tone(buzzer, NOTE_E6, 125);
    delay(130);
    tone(buzzer, NOTE_G6, 125);
    delay(130);
    tone(buzzer, NOTE_E7, 125);
    delay(130);
    tone(buzzer, NOTE_C7, 125);
    delay(130);
    tone(buzzer, NOTE_D7, 125);
    delay(130);
    tone(buzzer, NOTE_G7, 125);
    delay(130);
    noTone(buzzer);
  }
}

// sound for hit a meteorite - if the sound setting is on
void hitMeteoriteSound() {
  if (sound) {
    tone(buzzer, NOTE_G4, 35);
    delay(35);
    tone(buzzer, NOTE_G5, 35);
    delay(35);
    tone(buzzer, NOTE_G6, 35);
    delay(35);
    noTone(buzzer);
  }
}

// sound for game over - if the sound setting is on
void gameOverSound() {
  if (sound) {
    tone(buzzer, 500, 200);
    delay(200);
    tone(buzzer, 1200, 200);
    delay(200);
    tone(buzzer, 300, 200);
    delay(200);
    tone(buzzer, 1000, 200);
    delay(200);
    tone(buzzer, 400, 200);
    delay(200);
    tone(buzzer, 1100, 200);
    delay(200);
  }
}
