#include "Snake.h"
#include "define.h"

//#define ROW_COUNT 12
//#define COLUMN_COUNT 13

Snake::Snake()
{
  AppleColor = CHSV(96, 255, 255); // Green
  HeadColor = CHSV(32, 255, 255); // Orange
  BodyColor = CHSV(0, 255, 255); // Red

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
      else Head = Head - (N_COLUMN-1);
      break;

    case Down:
      if (Head / N_COLUMN == 0) //We are on lower edge
      {
        Head = Head - (N_COLUMN * (N_ROW - 1));
      }
      else Head = Head + (N_COLUMN-1);
      break;

    case Left:
      if ((Head+1) % N_COLUMN == 0) //We are on left edge
      {
        Head = Head + (N_COLUMN - 1);
      }
      else Head = Head - 1;
      break;

    case Right:
      if ((Head+1) % N_COLUMN == 0) //We are on right edge
      {
        Head = Head - (N_COLUMN-1);
      }
      else Head = Head + 1;
      break;

    default:
      break;
  }
  // SI Apple ==> Lenght++ / Reset Apple
  if (Head == Apple)
  {
    if (Lenght < MAX_SIZE) Lenght++;
    else endGame();
    AppleCaught = true;
  }

  uint8_t LenghtMinusOne = this->Lenght - 1;
  for (uint8_t i = 0; i < LenghtMinusOne; i++)
  {
    this->Body[LenghtMinusOne - i] = this->Body[LenghtMinusOne - i - 1];
    if (Head == Body[LenghtMinusOne - i]) //Check Collision with own body
    {
      endGame();
      return -1;
    }
  }
  this->Body[0] = Head;
  if (AppleCaught) resetApple();
  AppleCaught = false;
}

void Snake::resetBody()
{
  Lenght = 2;
  Body[0] = 1;
  Body[1] = 0;
}

void Snake::resetApple()
{
  bool retry = false;
  do {
    Apple = random(0, N_ROW * N_COLUMN);
    for (int i = 0; i < this->Lenght; i++)
    {
      if (Apple == Body[i]) retry = true;
    }
  } while (retry);
}

void Snake::endGame()
{
}

void Snake::drawBoard(CHSV Buffer[][COLUMN_COUNT])
{
  for (uint8_t x = 0; x < N_COLUMN; x++)    //We check each pixel, if it's supposed to be ON : Set corresponding LED ON, else turn it off
  {
    for (uint8_t y = 0; y < N_ROW; y++)
    {
      uint8_t CurrentPoint = y * N_ROW + x;
      CHSV CurrentColor = CHSV(0, 0, 0);
      if (CurrentPoint == Apple) CurrentColor = AppleColor;
      else if (CurrentPoint == Body[0]) CurrentColor = HeadColor;
      else
      {
        for (int i = 1; i < this->Lenght; i++)
        {
          if (CurrentPoint == Body[i]) CurrentColor = BodyColor;
        }
      }
      Buffer[y + 1][x + 1] = CurrentColor;
    }
  }
}


