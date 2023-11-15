#include <functions.h>
#include <pong.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <user_syscalls.h>


#define ESC 27
#define X 0
#define Y 1
#define WIDTH 2
#define HEIGHT 3
#define RADIO 2

char numberMap[] = {
    // 0
    'S',
    'S',
    'S',
    'S',
    '_',
    'S',
    'S',
    '_',
    'S',
    'S',
    '_',
    'S',
    'S',
    'S',
    'S',
    // 1
    'S',
    'S',
    '_',
    '_',
    'S',
    '_',
    '_',
    'S',
    '_',
    '_',
    'S',
    '_',
    'S',
    'S',
    'S',
    // 2
    'S',
    'S',
    'S',
    '_',
    '_',
    'S',
    'S',
    'S',
    'S',
    'S',
    '_',
    '_',
    'S',
    'S',
    'S',
    // 3
    'S',
    'S',
    'S',
    '_',
    '_',
    'S',
    'S',
    'S',
    'S',
    '_',
    '_',
    'S',
    'S',
    'S',
    'S',
    // 4
    'S',
    '_',
    'S',
    'S',
    '_',
    'S',
    'S',
    'S',
    'S',
    '_',
    '_',
    'S',
    '_',
    '_',
    'S',
    // 5
    'S',
    'S',
    'S',
    'S',
    '_',
    '_',
    'S',
    'S',
    'S',
    '_',
    '_',
    'S',
    'S',
    'S',
    'S',
    // 6
    'S',
    'S',
    'S',
    'S',
    '_',
    '_',
    'S',
    'S',
    'S',
    'S',
    '_',
    'S',
    'S',
    'S',
    'S',
    // 7
    'S',
    'S',
    'S',
    '_',
    '_',
    'S',
    '_',
    '_',
    'S',
    '_',
    '_',
    'S',
    '_',
    '_',
    'S',
    // 8
    'S',
    'S',
    'S',
    'S',
    '_',
    'S',
    'S',
    'S',
    'S',
    'S',
    '_',
    'S',
    'S',
    'S',
    'S',
    // 9
    'S',
    'S',
    'S',
    'S',
    '_',
    'S',
    'S',
    'S',
    'S',
    '_',
    '_',
    'S',
    '_',
    '_',
    'S',
};

int widthScreen;
int height;
int width;
int top;
int bottom;
int left;
int right;
int square = 10;

void drawRectangle(int x, int y, int sizex, int sizey, int color) {
  for (int i = 0; i < sizex; i++) {
    for (int j = 0; j < sizey; j++) {
      sys_put_pixel(color, i + x, j + y);
    }
  }
}

char *getNumber(int number) { return (numberMap + number * 3 * 5); }

void printNumber(int number, int x, int y) {
  char *numMap = getNumber(number);
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 3; j++) {
      if (numMap[j + i * 3] == 'S') {
        drawRectangle(x + j * square, y + i * square, square, square, WHITE);
      }
    }
  }
}

void drawScreenBorders(int starty, int endy, int startx, int endx) {
  for (int x = startx; x <= endx; x++) {
    sys_put_pixel(WHITE, x, starty);
    sys_put_pixel(WHITE, x, endy);
  }

  for (int y = starty; y <= endy; y++) {
    sys_put_pixel(WHITE, startx, y);
    sys_put_pixel(WHITE, endx, y);
  }
}

void drawDottedLine(int x, int y, int length, int size) {
  for (int i = 0; i < length; i += size * 2) {
    drawRectangle(x - size / 2, i + y, size, size, WHITE);
  }
}

void drawBar(int *bar) {
  drawRectangle(bar[X], bar[Y], bar[WIDTH], bar[HEIGHT], WHITE);
}

void moveUpBar(int *bar) {
  int move = 0;
  for (move = 0; bar[Y] - move > top + 1 && move <= 15; move++) {
  }

  drawRectangle(bar[X], bar[Y] + bar[HEIGHT] - move, bar[WIDTH], move, BLACK);
  bar[Y] = bar[Y] - move;
  drawBar(bar);
}

void moveDownBar(int *bar) {
  int move = 0;
  for (move = 0; bar[Y] + bar[HEIGHT] + move < bottom && move <= 15; move++) {
  }
  drawRectangle(bar[X], bar[Y], bar[WIDTH], move, BLACK);
  bar[Y] = bar[Y] + move;
  drawBar(bar);
}

void updateScores(int scores[]) {
  if (scores[1] / 10 > 0) {
    printNumber(scores[1] / 10, widthScreen / 2 + 20, 50);
    printNumber(scores[1] % 10, widthScreen / 2 + 30 + 3 * square, 50);
  }
  if (scores[0] / 10 > 0) {
    printNumber(scores[0] / 10, widthScreen / 2 - 30 - 6 * square, 50);
    printNumber(scores[0] % 10, widthScreen / 2 - 20 - 3 * square, 50);
  } else {
    printNumber(scores[1], widthScreen / 2 + 20, 50);
    printNumber(scores[0], widthScreen / 2 - 20 - 3 * square, 50);
  }
}

void drawBall(int *circle, int color) {
  int r = circle[RADIO];

  for (int i = -r; i <= r; i++) {
    for (int j = -r; j <= r; j++) {
      if (i * i + j * j <= r * r) {
        sys_put_pixel(color, circle[X] + i, circle[Y] + j);
      }
    }
  }
}

void moveBall(int *ball, int x, int y, int scores[]) {
  drawBall(ball, BLACK);
  ball[X] = ball[X] + x;
  ball[Y] = ball[Y] + y;
  drawBall(ball, WHITE);
  drawDottedLine(widthScreen / 2, top, bottom - top, 4);
  updateScores(scores);
}

// left is false, right is true
void nextMoveBall(int ball[], int lastDir[], int bar[], bool side) {
  int section = bar[HEIGHT] / 10;
  int j = 1;
  if (side) {
    j = -1;
  }
  if (ball[Y] < bar[Y] + section) {
    lastDir[X] = j * 17;
    lastDir[Y] = -15;
  } else if (ball[Y] < bar[Y] + 2 * section) {
    lastDir[X] = j * 17;
    lastDir[Y] = -12;
  } else if (ball[Y] < bar[Y] + 3 * section) {
    lastDir[X] = j * 17;
    lastDir[Y] = -10;
  } else if (ball[Y] < bar[Y] + 4 * section) {
    lastDir[X] = j * 17;
    lastDir[Y] = -5;
  } else if (ball[Y] < bar[Y] + 5 * section) {
    lastDir[X] = j * 17;
    lastDir[Y] = -3;
  } else if (ball[Y] < bar[Y] + 6 * section) {
    lastDir[X] = j * 17;
    lastDir[Y] = 3;
  } else if (ball[Y] < bar[Y] + 7 * section) {
    lastDir[X] = j * 17;
    lastDir[Y] = 5;
  } else if (ball[Y] < bar[Y] + 9 * section) {
    lastDir[X] = j * 17;
    lastDir[Y] = 10;
  } else if (ball[Y] < bar[Y] + 9 * section) {
    lastDir[X] = j * 7;
    lastDir[Y] = 12;
  } else {
    lastDir[X] = j * 17;
    lastDir[Y] = 15;
  }
}

void drawScores(int scores[]) {
  drawRectangle(widthScreen / 2 + 20, 50, 3 * square, 5 * square, BLACK);
  drawRectangle(widthScreen / 2 - 20 - 3 * square, 50, 3 * square, 5 * square,
                BLACK);
  if (scores[1] / 10 > 0) {
    drawRectangle(widthScreen / 2 + 30 + 3 * square, 50, 3 * square, 5 * square,
                  BLACK);
    printNumber(scores[1] / 10, widthScreen / 2 + 20, 50);
    printNumber(scores[1] % 10, widthScreen / 2 + 30 + 3 * square, 50);
  } else {
    printNumber(scores[1], widthScreen / 2 + 20, 50);
  }
  if (scores[0] / 10 > 0) {
    drawRectangle(widthScreen / 2 - 30 - 6 * square, 50, 3 * square, 5 * square,
                  BLACK);
    printNumber(scores[0] / 10, widthScreen / 2 - 30 - 6 * square, 50);
    printNumber(scores[0] % 10, widthScreen / 2 - 20 - 3 * square, 50);
  } else {
    printNumber(scores[0], widthScreen / 2 - 20 - 3 * square, 50);
  }
}

void pong() {
  char key;
  sys_clear_screen();
  sys_get_screen_width(&widthScreen);
  height = 500;
  width = 900;
  top = 5;
  bottom = top + height;
  left = (widthScreen - width) / 2;
  right = widthScreen - (widthScreen - width) / 2;

  print("Press ENTER to play");
  enter();
  print("Press ESC to leave the game at any moment");
  enter();
  print("Instructions to play pong are: ");
  enter();
  print("The white ball will get automatically thrown onto the screen.");
  print("To align with the incoming white ball,");
  print("you will need to move the");
  enter();
  print("white paddles with: ");
  enter();
  sys_write("'W,S' if you are player1 and 'O,L' if you are player 2", RED);
  enter();
  print(" Finally, there is a scoreboard with each players points displayed "
        "above their side of the screen.");

  while (1) {
    key = getKey();
    if (key == ESC) {
      sys_clear_screen();
      return;
    }
    if (key == '\n') {
      sys_clear_screen();
      drawScreenBorders(top, bottom, left, right);
      drawDottedLine(widthScreen / 2, top, bottom - top, 4);

      int barL[4];
      int barR[4];
      int ball[3];
      int lastDir[2];
      int scores[2];

      barL[X] = left + 20;
      barL[Y] = top + 100;
      barL[WIDTH] = 10;
      barL[HEIGHT] = 100;

      barR[X] = right - 30;
      barR[Y] = top + 100;
      barR[WIDTH] = 10;
      barR[HEIGHT] = 100;

      ball[X] = widthScreen / 2;
      ball[Y] = (bottom - top) / 2;
      ball[RADIO] = 10;

      lastDir[X] = 20;
      lastDir[Y] = 0;

      scores[0] = 0;
      scores[1] = 0;

      drawScores(scores);
      drawBar(barL);
      drawBar(barR);
      drawBall(ball, WHITE);
      while (1) {
        sys_sleep(1);
        key = getKey();
        if (key == 'W') {
          moveUpBar(barL);
          sys_get_clean_buffer();
        }
        if (key == 'S') {
          moveDownBar(barL);
          sys_get_clean_buffer();
        }
        if (key == 'O') {
          moveUpBar(barR);
          sys_get_clean_buffer();
        }
        if (key == 'L') {
          moveDownBar(barR);
          sys_get_clean_buffer();
        }
        if (ball[X] + ball[RADIO] + lastDir[X] > barR[X]) {
          if (ball[Y] + ball[RADIO] > barR[Y] &&
              ball[Y] - ball[RADIO] < barR[Y] + barR[HEIGHT]) {
            nextMoveBall(ball, lastDir, barR, true);
            // hits right bar
          } else if (scores[0] == 99) {
            key = ESC;
            scores[0]++;
          } else {
            scores[0]++;
            drawScores(scores);
            lastDir[X] = -lastDir[X];
            lastDir[Y] = -lastDir[Y];
            drawBall(ball, BLACK);
            ball[X] = widthScreen / 2;
            ball[Y] = (bottom - top) / 2;
            sys_beep(196, 5);
            sys_beep(220, 5);
            sys_sleep(50);
          }
        }
        if (ball[X] - ball[RADIO] + lastDir[X] < barL[X] + barL[WIDTH]) {
          if (ball[Y] + ball[RADIO] > barL[Y] &&
              ball[Y] - ball[RADIO] < barL[Y] + barL[HEIGHT]) {
            nextMoveBall(ball, lastDir, barL, false);
            // hits left bar
          } else if (scores[1] == 99) {
            key = ESC;
            scores[1]++;
          } else {
            scores[1]++;
            drawScores(scores);
            lastDir[X] = -lastDir[X];
            lastDir[Y] = -lastDir[Y];
            drawBall(ball, BLACK);
            ball[X] = widthScreen / 2;
            ball[Y] = (bottom - top) / 2;
            sys_beep(196, 5);
            sys_beep(220, 5);

            sys_sleep(50);
          }
        }
        if (ball[Y] - ball[RADIO] + lastDir[Y] < top + 1) {
          lastDir[Y] = -lastDir[Y];
        }

        if (ball[Y] + ball[RADIO] + lastDir[Y] > bottom) {
          lastDir[Y] = -lastDir[Y];
        }

        if (key == ESC) {
          sys_clear_screen();
          if (scores[0] > scores[1]) {
            print("Player 1 is the winner with ");
            sys_write_dec(scores[0], WHITE);
            print(" goals");
          } else if (scores[0] < scores[1]) {
            print("Player 2 is the winner with ");
            sys_write_dec(scores[1], WHITE);
            print(" goals");
          } else {
            print("Its a tie");
          }
          sys_sleep(1000);
          sys_clear_screen();
          return;
        }

        moveBall(ball, lastDir[X], lastDir[Y], scores);
      }
    }
  }
}
