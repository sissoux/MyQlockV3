#include "Snake.h"
#include "define.h"

#define ROW_COUNT 12
#define COLUMN_COUNT 13

Snake::Snake()
{
  this->resetBody();
  this->resetApple();
}


int8_t Snake::move(Direction Dir)
{
  //define new Head position
  uint8_t Head = this->Body[0];
  switch (Dir)
  {
    case Up:
      if (Head / N_COLUMN == 0) //We are on upper edge
      {
        Head = Head + (N_COLUMN * (N_ROW - 1));
      }
      else Head = Head - N_COLUMN;
      break;

    case Down:
      if (Head % N_COLUMN == 0) //We are on lower edge
      {
        Head = Head - (N_COLUMN * (N_ROW - 1));
      }
      else Head = Head + N_COLUMN;
      break;

    case Left:
      if (Head % N_COLUMN == 0) //We are on left edge
      {
        Head = Head + (N_COLUMN - 1);
      }
      else Head = Head - 1;
      break;

    case Right:
      if (Head % N_COLUMN == 3) //We are on right edge
      {
        Head = Head - (N_COLUMN + 1);
      }
      else Head = Head + 1;
      break;

    default:
      break;
  }
  // SI Apple ==> Lenght++ / Reset Apple

  uint8_t LenghtMinusOne = this->Lenght - 1;
  for (uint8_t i = 0; i < LenghtMinusOne; i++)
  {
    this->Body[LenghtMinusOne - i] = this->Body[LenghtMinusOne - i - 1];
    if (Head == Body[LenghtMinusOne - i])
    {
      endGame();
      return -1;
    }
  }
  this->Body[0] = Head;
}

void Snake::resetBody()
{

  //Clear full body
  //Define head position
  //Define body position relative to the head
  this->Lenght = 2;

}

void Snake::resetApple()
{
  //Generate New Apple, which cannot be in snake body
}

void Snake::endGame()
{
  //Generate New Apple, which cannot be in snake body
}

void Snake::drawBoard()
{

  for (uint8_t x = 0; x < COLUMN_COUNT; x++)    //We check each pixel, if it's supposed to be ON : Set corresponding LED ON, else turn it off
  {
    for (uint8_t y = 0; y < ROW_COUNT; y++)
    {
      //uint8_t LED_Number = Mapping[y][x];
      //if (LED_Number < StripLenght) this->leds[LED_Number] = //AlternativeBuffer[y][x];
    }
  }

}

