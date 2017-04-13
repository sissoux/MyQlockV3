#ifndef Snake_h
#define Snake_h
#include "Arduino.h"


#define N_ROW 11
#define N_COLUMN 12

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
    uint8_t Body[N_COLUMN * N_ROW]; //Define Body max size as matrix size. Head is Cell 0
    uint8_t Apple = 0;

    Snake();
    int8_t move(Direction Dir);
    void resetBody();
    void resetApple();
    void endGame();
    void drawBoard();
};
#endif

