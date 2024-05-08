#include <Adafruit_GFX.h> // Core graphics library
#include <MCUFRIEND_kbv.h>
#include "TouchScreen.h"
MCUFRIEND_kbv tft;

#define YP A1 // must be an analog pin,   use "An" notation!
#define XM A2 // must be an analog pin, use "An" notation!
#define YM 7  // can be a digital pin
#define XP 6  // can be a digital pin
#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
#define LCD_RESET A4

#define BLACK 0x0000
#define BROWN 0x7980
#define RED 0xF800
#define ORANGE 0xFBE0
#define YELLOW 0xFFE0
#define GREEN 0x07E0
#define BLUE 0x001F
#define VIOLET 0xA81F
#define GREY 0x7BEF
#define WHITE 0xFFFF
#define CYAN 0x07FF
#define MAGENTA 0xF81F

#define MINPRESSURE 10
#define MAXPRESSURE 1200

#define TS_MINX 932 // left
#define TS_MINY 201 // t09
#define TS_MAXX 223 // RT
#define TS_MAXY 918 // Bot

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
String symbol[3][4] = {
    {"0", "1", "2", "3"},
    {"4", "5", "6", "7"},
    {"8", "9", "C", "K"}};
String colors[1][10] =
    {"White", "Sand Bed", "Cyan", "Orange", "Blue", "Red", "Yellow", "Green", "Grey", "Black"};
int X, Y;
int ring, ring1, ring2, ring3, ring4 = 0;
long Number1;
char C;
char K;
long resistance = 0;
void setup()
{
  Serial.begin(9600);
  tft.reset();
  tft.begin(0x9341);
  tft.setRotation(2);
  tft.invertDisplay(1);
  IntroScreen();
  draw_BoxNButtons();
}

void loop()
{

  TSPoint p = waitTouch();
  X = p.y;
  Y = p.x;
  Serial.print(X);
  Serial.print(',');
  Serial.println(Y);

  DetectButtons();

  Ring();
}

TSPoint waitTouch()
{

  TSPoint p;
  do
  {
    p = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
  } while ((p.z < MINPRESSURE) || (p.z > MAXPRESSURE));
  p.x = map(p.x, TS_MAXX, TS_MINX, 0, 320);
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, 240);
  ;
  return p;
}

void DetectButtons()
{

  if (X > 135)
  {
    if (Y > 0 && Y < 60)
    {
      Serial.println("Button 8");
      Number1 = 8;
    }

    if (Y > 60 && Y < 120)
    {
      Serial.println("Button 4");
      Number1 = 4;
    }

    if (Y > 120 && Y < 180)
    {
      Serial.println("Button 0");

      Number1 = 0;
    }
  }

  if (X < 135 && X > 90)
  {
    if (Y > 0 && Y < 60)
    {
      Serial.println("Button 9");

      Number1 = 9;
    }

    if (Y > 60 && Y < 120)
    {
      Serial.println("Button 5");
      Number1 = 5;
    }

    if (Y > 120 && Y < 180)
    {
      Serial.println("Button 1");
      Number1 = 1;
    }
  }

  if (X < 90 && X > 40)
  {
    if (Y > 0 && Y < 60)
    {
      Serial.println("Button C");

      ring = 0;
      ring1 = 0;
      ring2 = 0;
      ring3 = 0;
      ring4 = 0;

      draw_BoxNButtons();
      resistance = 0;
      Number1 = 0;

      delay(500);

      return;
    }

    if (Y > 60 && Y < 120)
    {
      Serial.println("Button 6");
      Number1 = 6;
    }

    if (Y > 120 && Y < 180)
    {
      Serial.println("Button 2");
      Number1 = 2;
    }
  }

  if (X < 48)
  {
    if (Y > 0 && Y < 60)
    {
      Serial.println("Button K");

      if (ring > 3)
        calculation();
      else
        draw_BoxNButtons();

      ring = -1;
      ring1 = 0;
      ring2 = 0;
      ring3 = 0;
      ring4 = 0;
      delay(700);
    }

    if (Y > 60 && Y < 120)
    {
      Serial.println("Button 7");
      Number1 = 7;
    }

    if (Y > 120 && Y < 180)
    {
      Serial.println("Button 3");
      Number1 = 3;
    }
  }
  if (!(ring > 3 || ring == -1))
  {
    tft.setCursor(55, 28 + (20 * ring));
    tft.setTextColor(RED);
    tft.setTextSize(2);
    tft.println(colors[0][Number1]);
    delay(300);
    ring++;
  }
  else
    ring++;
}

void IntroScreen()
{
  tft.fillScreen(WHITE);
  tft.setCursor(55, 50);
  tft.setTextSize(3);
  tft.setTextColor(RED);
  tft.println("ARDUINO");
  tft.setTextColor(RED);
  tft.setCursor(60, 140);
  tft.setTextSize(2);
  tft.println("Resistor");
  tft.setCursor(10, 170);
  tft.println("Value Calculator");
  tft.setCursor(10, 230);
  tft.setTextColor(BLACK);
  tft.println("Rifa, Mohima, Abid");
  delay(4000);
}

void draw_BoxNButtons()
{
  tft.fillRect(0, 0, 240, 140, BLUE);
  tft.fillRect(20, 25, 20, 85, WHITE);
  for (int a = 0; a < 3; a++)
  {
    tft.drawLine(29 + a, 25, 29 + a, 15, WHITE);
    tft.drawLine(29 + a, 80, 29 + a, 120, WHITE);
  }
  tft.fillRect(20, 35, 20, 3, BLACK);
  tft.fillRect(20, 55, 20, 3, BLACK);

  tft.fillRect(20, 75, 20, 3, BLACK);
  tft.fillRect(20, 100, 20, 3, BLACK);

  tft.fillRect(0, 260, 60, 60, GREY);
  tft.fillRect(0, 200, 60, 60, YELLOW);
  tft.fillRect(0, 140, 60, 60, BLACK);

  tft.fillRect(60, 260, 60, 60, WHITE);
  tft.fillRect(60, 200, 60, 60, GREEN);
  tft.fillRect(60, 140, 60, 60, BROWN);

  tft.fillRect(120, 260, 60, 60, BLACK);
  tft.fillRect(120, 200, 60, 60, BLUE);
  tft.fillRect(120, 140, 60, 60, RED);

  tft.fillRect(180, 260, 60, 60, BLACK);
  tft.fillRect(180, 200, 60, 60, MAGENTA);
  tft.fillRect(180, 140, 60, 60, ORANGE);

  for (int h = 140; h <= 320; h += 60)
    tft.drawFastHLine(0, h, 240, WHITE);

  for (int v = 0; v <= 240; v += 60)
    tft.drawFastVLine(v, 140, 240, WHITE);

  for (int j = 0; j < 3; j++)
  {
    for (int i = 0; i < 4; i++)
    {
      tft.setCursor(22 + (60 * i), 100 + (60 * (j + 1)));
      tft.setTextSize(3);
      tft.setTextColor(WHITE);
      tft.println(symbol[j][i]);
    }
  }

  tft.setTextSize(3);
  tft.setTextColor(BLACK);
  tft.setCursor(22, 220);
  tft.println("4");
  tft.setCursor(82, 280);
  tft.println("9");
  tft.setTextColor(RED);
  tft.setCursor(22, 280);
  tft.println("8");
  tft.fillRect(181, 261, 60, 60, BLACK);

  tft.setTextSize(3);
  tft.setCursor(195, 280);
  tft.println("OK");
}

void Ring()
{
  if (ring == 1)
    ring1 = Number1;
  if (ring == 2)
    ring2 = Number1;
  if (ring == 3)
    ring3 = Number1;
  if (ring == 4)
    ring4 = Number1;
}

void calculation()
{

  long test;
  float finalvalue1;
  int mega = 0;

  if (ring3 == 0)
    test = 1;
  if (ring3 == 1)
    test = 10;
  if (ring3 == 2)
    test = 100;
  if (ring3 == 3)
    test = 1000;
  if (ring3 == 4)
    test = 10000;
  if (ring3 == 5)
  {
    test = 1;
    mega = 1;
  };
  if (ring3 == 6)
  {
    test = 10;
    mega = 1;
  }
  if (ring3 == 7)
  {
    test = 100;
    mega = 1;
  }
  if (ring3 == 8)
  {
    test = 1000;
    mega = 1;
  }
  if (ring3 == 9)
  {
    test = 10000;
    mega = 1;
  }
  resistance = (ring1 * 10 + ring2) * test;
  Serial.println(resistance);
  Serial.println(ring1);
  Serial.println(ring2);
  Serial.println(ring3);

  tft.setTextSize(3);
  tft.setTextColor(WHITE);
  tft.setCursor(145, 20);
  if (resistance < 1000 && mega == 0)
  {
    tft.println(resistance);
    tft.setCursor(145, 50);
    tft.print("Ohms");
  }
  else if (resistance > 999 && mega == 0)
  {

    finalvalue1 = (float)resistance / 1000;
    tft.print(finalvalue1);
    tft.setCursor(145, 60);
    tft.print("K.Ohm");
  }
  else
  {

    finalvalue1 = (float)resistance / 10;
    tft.print(finalvalue1);
    tft.setCursor(145, 60);
    tft.print("M.Ohm");
  }
  ring = 0;
}