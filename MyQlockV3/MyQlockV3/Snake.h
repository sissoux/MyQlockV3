#ifndef Snake_h
#define Snake_h
#include "Arduino.h"
#include "FastLED.h"
#include "define.h"


#define N_ROW 10
#define N_COLUMN 11
#define MAX_SIZE N_ROW*N_COLUMN

// Game Matrix is build this way: 0----------->
//                                 ----------->
//                                 ----------->
//                                 -----------> N_ROW*N_COLUMN

typedef enum {
  Up,
  Down,
  Right,
  Left
} Direction;


class Snake
{
  public:
    uint8_t Lenght = 0;
    uint8_t Body[MAX_SIZE]; //Define Body max size as matrix size. Head is Cell 0
    uint8_t Apple = 0;
    bool AppleCaught = false;
    CHSV AppleColor;
    CHSV HeadColor;
    CHSV BodyColor;

    Snake();
    int8_t move(Direction Dir);
    void resetBody();
    void resetApple();
    void endGame();
    void drawBoard(CHSV Buffer[][COLUMN_COUNT]);
};
#endif

