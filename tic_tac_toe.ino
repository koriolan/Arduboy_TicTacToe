#include <Arduboy2.h>
#include "bitmaps.h"


Arduboy2 arduboy;
constexpr uint8_t frameRate = 30;

struct CELL {
  uint8_t cnt : 3;
  uint8_t XO : 1;
  uint8_t isEmpty : 1;
  uint8_t reserved : 3;
} field[3][3];

struct CURSOR {
  uint8_t X : 3;
  uint8_t Y : 3;
  uint8_t XO : 1;
  uint8_t reserved : 1;
} cursor;
uint8_t isWin = 0;
const uint8_t width = 18;
const uint8_t height = 18;

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(frameRate);
  newGame();
}

void loop() {
  if (!(arduboy.nextFrame()))
    return;

  arduboy.pollButtons();
  if (arduboy.justPressed(RIGHT_BUTTON) && cursor.X < 2) {
    cursor.X++;
  }
  if (arduboy.justPressed(LEFT_BUTTON) && cursor.X > 0) {
    cursor.X--;
  }
  if (arduboy.justPressed(DOWN_BUTTON) && cursor.Y < 2) {
    cursor.Y++;
  }
  if (arduboy.justPressed(UP_BUTTON) && cursor.Y > 0) {
    cursor.Y--;
  }

  if (arduboy.justPressed(A_BUTTON) && !isWin) {
    if (field[cursor.X][cursor.Y].isEmpty) {
      for (uint8_t x = 0; x < 3; x++) {
        for (uint8_t y = 0; y < 3; y++) {
          if (!field[x][y].isEmpty && field[x][y].XO == cursor.XO) {
            field[x][y].cnt++;
            if (field[x][y].cnt > 2) {
              clearCell(x, y);
            }
          }
        }
      }

      field[cursor.X][cursor.Y].cnt = 0;
      field[cursor.X][cursor.Y].isEmpty = 0;
      field[cursor.X][cursor.Y].XO = cursor.XO;
      cursor.XO ^= 1;
    }
  }
  if (arduboy.justPressed(B_BUTTON)) {
    newGame();
  }
  chekWin();
  arduboy.clear();
  drawGameField();
  for (uint8_t x = 0; x < 3; x++) {
    for (uint8_t y = 0; y < 3; y++) {
      if (!field[x][y].isEmpty) {
        field[x][y].XO ? drawO(x, y) : drawX(x, y);
      }
    }
  }
  chekWin();
  if(!isWin){
    drawCursor(cursor.X, cursor.Y);
    cursor.XO ? arduboy.setRGBled(0xC8,0x00,0x00) : arduboy.setRGBled(0x00,0x00,0xC8);
  }else
  {
    arduboy.setRGBled(0x00,0xC8,0x00);
  }
  arduboy.display();
}

void newGame() {
  cursor.X = 0;
  cursor.Y = 0;
  cursor.XO = 0;
  isWin = 0;
  arduboy.setRGBled(0xC8,0x00,0x00);

  for (uint8_t x = 0; x < 3; x++) {
    for (uint8_t y = 0; y < 3; y++) {
      clearCell(x, y);
    }
  }
}

void clearCell(uint8_t x, uint8_t y) {
  field[x][y].cnt = 0;
  field[x][y].isEmpty = 1;
  field[x][y].XO = 0;
}

void drawGameField() {
  arduboy.drawFastVLine(20, 0, 64, WHITE);
  arduboy.drawFastVLine(21, 0, 64, WHITE);
  arduboy.drawFastVLine(42, 0, 64, WHITE);
  arduboy.drawFastVLine(43, 0, 64, WHITE);

  arduboy.drawFastHLine(0, 20, 64, WHITE);
  arduboy.drawFastHLine(0, 21, 64, WHITE);
  arduboy.drawFastHLine(0, 42, 64, WHITE);
  arduboy.drawFastHLine(0, 43, 64, WHITE);
  arduboy.drawSlowXYBitmap(67, 18, bitmapControl, 58, 29, WHITE);
}

void chekWin()
{
  for(uint8_t y=0;y<3;y++)
  {
    if(!field[0][y].isEmpty&& !field[1][y].isEmpty && !field[2][y].isEmpty && field[0][y].XO==field[1][y].XO && field[1][y].XO==field[2][y].XO){
      arduboy.drawFastHLine(2, 9+22*y, 60, WHITE);
      arduboy.drawFastHLine(2, 10+22*y, 60, WHITE);
    isWin=1;
    return;
    }
  }

  for(uint8_t x=0;x<3;x++)
  {
    if(!field[x][0].isEmpty&& !field[x][1].isEmpty && !field[x][2].isEmpty && field[x][0].XO==field[x][1].XO && field[x][1].XO==field[x][2].XO){
      arduboy.drawFastVLine(9+22*x, 2, 60, WHITE);
      arduboy.drawFastVLine(10+22*x, 2, 60, WHITE);
    isWin= 1;
    return;
    }
  }

  if(!field[0][0].isEmpty&& !field[1][1].isEmpty && !field[2][2].isEmpty && field[0][0].XO==field[1][1].XO && field[1][1].XO==field[2][2].XO){
    arduboy.drawLine(3, 2, 62, 61, WHITE);
    arduboy.drawLine(3, 3, 61, 61, WHITE);
    arduboy.drawLine(2, 3, 61, 62, WHITE);
    isWin= 1;
    return;
  }

  if(!field[2][0].isEmpty&& !field[1][1].isEmpty && !field[0][2].isEmpty && field[2][0].XO==field[1][1].XO && field[1][1].XO==field[0][2].XO){
    arduboy.drawLine(1, 61, 62, 1, WHITE);
    arduboy.drawLine(2, 61, 61, 2, WHITE);
    arduboy.drawLine(2, 62, 62, 2, WHITE);

    isWin= 1;
    return;
  }
  
}

uint8_t cordToPix(uint8_t c) {
  return c * 22 + 1;
}

void drawO(uint8_t x, uint8_t y) {
  arduboy.drawSlowXYBitmap(cordToPix(x), cordToPix(y), bitmapO, width, height, WHITE);
}

void drawX(uint8_t x, uint8_t y) {
  arduboy.drawSlowXYBitmap(cordToPix(x), cordToPix(y), bitmapX, width, height, WHITE);
}

void drawCursor(uint8_t x, uint8_t y) {
  arduboy.drawSlowXYBitmap(cordToPix(x) + 8, cordToPix(y), bitmapCursor, 8, 8, WHITE);
}
