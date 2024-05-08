#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>
#define MINPRESSURE 200
#define MAXPRESSURE 1000

const int XP = 7, XM = A1, YP = A2, YM = 6; // 240x320 ID=0x9341
const int TS_LEFT = 918, TS_RT = 201, TS_TOP = 932, TS_BOT = 223;
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define BLACK 0x0000
#define BLUE 0x000F
#define RED 0xF800
#define GREEN 0x03E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define BUTTON_X 30
#define BUTTON_Y 123
#define BUTTON_W 51
#define BUTTON_H 50
#define BUTTON_SPACING_X 07
#define BUTTON_SPACING_Y 07
#define BUTTON_TEXTSIZE 2

char buttonlabels[16][5] = {"1", "2", "3", "+", "4", "5", "6", "-", "7", "8", "9", "/", "=", "0", "DEL", "x"};
uint16_t buttoncolors[16] = {BLACK, BLACK, BLACK, BLUE, BLACK, BLACK, BLACK, BLUE, BLACK, BLACK, BLACK, BLUE, GREEN, BLACK, RED, BLUE};
Adafruit_GFX_Button buttons[16];
int col = 0, row = 0, z = 0, i, j;
#define TEXT_X 5
#define TEXT_Y 25
#define TEXT_W (tft.width() - 10)
#define TEXT_H 70
#define TEXT_TSIZE 3
#define TEXT_TCOLOR BLUE
char q;
int presskey;
int pass;
float num1, num2;

int operator_type = 0;
#define TEXT_LEN 10
char textfield[TEXT_LEN + 1] = "";
uint8_t textfield_i = 0;
float result = 0;

TSPoint tp;
boolean n = false;
int nc;
float fn; // final number
#define TS_MINX 138
#define TS_MINY 236
#define TS_MAXX 772
#define TS_MAXY 869

void setup()
{
    Serial.begin(9600);
    tft.reset();
    uint16_t ID = tft.readID();
    Serial.print("TFT ID = 0x");
    Serial.println(ID, HEX);
    Serial.println("Calibrate for your Touch Panel");
    if (ID == 0xD3D3)
        ID = 0x9486; // write-only shield
    tft.begin(ID);
    tft.setRotation(0); // PORTRAIT
    tft.fillScreen(WHITE);

    // welcome screen
    uint16_t width = tft.width() - 1;
    uint16_t height = tft.height() - 1;
    uint8_t border = 10;
    tft.fillScreen(WHITE);
    tft.fillRect(border, border, (width - border * 2), (height - border * 2), WHITE);
    tft.setTextSize(3);
    tft.setTextColor(RED);
    tft.setCursor((tft.width() / 2) - 85, 40);
    tft.println("CALCULATOR");
    tft.setCursor((tft.width() / 2) - 85, 110);
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
    tft.println("Developed by: \n\n\tRifa (2102101) \n\n\tMohima (2102102) \n\n\tAbid (2102103)");
    tft.setCursor((tft.width() / 2) - 95, 280);
    tft.println("Touch to Proceed");

    do
    {
        tp = ts.getPoint();
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
    } while ((tp.z < MINPRESSURE) || (tp.z > MAXPRESSURE));

    tp.x = map(tp.x, TS_LEFT, TS_RT, 0, tft.width());
    tp.y = map(tp.y, TS_TOP, TS_BOT, 0, tft.height());

    tft.fillScreen(WHITE);
    tft.setCursor((tft.width() / 2) - 60, 5);
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
    tft.println("CALCULATOR");
    tft.drawRoundRect(TEXT_X, TEXT_Y, TEXT_W, TEXT_H, 10, BLACK);

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            row = i;
            col = j;

            buttons[z].initButton(&tft, BUTTON_X + col * (BUTTON_W + BUTTON_SPACING_X),
                                  BUTTON_Y + row * (BUTTON_H + BUTTON_SPACING_Y),
                                  BUTTON_W, BUTTON_H, BLACK, buttoncolors[z], WHITE,
                                  buttonlabels[z], BUTTON_TEXTSIZE);
            buttons[z].drawButton();
            z++;
        }
    }
}

void loop()
{
    do
    {
        tp = ts.getPoint();
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
    } while ((tp.z < MINPRESSURE) || (tp.z > MAXPRESSURE));

    tp.x = map(tp.x, TS_LEFT, TS_RT, 0, tft.width());
    tp.y = map(tp.y, TS_TOP, TS_BOT, 0, tft.height());

    if (n == true)
    {
        tft.setCursor(TEXT_X + 2, TEXT_Y + 40);
        tft.setTextColor(TEXT_TCOLOR, WHITE);
        tft.setTextSize(TEXT_TSIZE);
        tft.print(" ");
        tft.setCursor(TEXT_X + 2, TEXT_Y + 10);
        tft.print(" ");
        n = false;
    }

    for (uint8_t b = 0; b < 16; b++)
    {
        if (buttons[b].contains(tp.x, tp.y))
        {
            buttons[b].press(true);
            q = b;
        }
        else
        {
            buttons[b].press(false);
        }
    }
    Serial.print("value of q: ");
    Serial.println(q);

    if (q == 3 || q == 7 || q == 11 || q == 12 || q == 14 || q == 15)
    {
        switch (q)
        {
        case 3:
            operator_type = 1;
            pressed_button();
            animate();
            display_text();
            break;
        case 7:
            operator_type = 2;
            pressed_button();
            animate();
            display_text();
            break;
        case 11:
            operator_type = 3;
            pressed_button();
            animate();
            display_text();
            break;
        case 12:
            pressed_button();
            animate();
            break;
        case 14:
            pressed_button();
            animate();
            clr_button();
            break;
        case 15:
            operator_type = 4;
            pressed_button();
            animate();
            display_text();
            break;
        }
    }
    else
    {
        switch (q)
        {
        case 0:
            nc = 1;
            pressed_button();
            animate();
            display_text();
            break;
        case 1:
            nc = 2;
            pressed_button();
            animate();
            display_text();
            break;
        case 2:
            nc = 3;
            pressed_button();
            animate();
            display_text();
            break;
        case 4:
            nc = 4;
            pressed_button();
            animate();
            display_text();
            break;
        case 5:
            nc = 5;
            pressed_button();
            animate();
            display_text();
            break;
        case 6:
            nc = 6;
            pressed_button();
            animate();
            display_text();
            break;
        case 8:
            nc = 7;
            pressed_button();
            animate();
            display_text();
            break;
        case 9:
            nc = 8;
            pressed_button();
            animate();
            display_text();
            break;
        case 10:
            nc = 9;
            pressed_button();
            animate();
            display_text();
            break;
        case 13:
            nc = 0;
            pressed_button();
            animate();
            display_text();
            break;
        }
    }

    if (nc > 0 || q == 13)
    {
        switch (presskey)
        {
        case 0:
            fn = nc;
            break;
        case 1:
            fn = fn * 10 + nc;
            break;
        case 2:
            fn = fn * 10 + nc;
            break;
        case 3:
            fn = fn * 10 + nc;
            break;
        }
        Serial.print("presskey: ");
        Serial.println(presskey);
        nc = 0;
        presskey++;
        delay(200);
    }
    if (q == 3 || q == 7 || q == 11 || q == 12 || q == 15)
    {
        if (pass == 0)
        {
            presskey = 0;
            num1 = fn;
            pass++;
        }
        else if (pass == 1)
        {
            presskey = 0;
            num2 = fn;
        }
    }

    if (q == 12)
    {
        switch (operator_type)
        {
        case 1:
            result = num1 + num2;
            break;
        case 2:
            result = num1 - num2;
            break;
        case 3:
            result = num1 / num2;
            break;
        case 4:
            result = num1 * num2;
            break;
        }
        tft.setCursor(TEXT_X + 2, TEXT_Y + 40);
        tft.setTextColor(TEXT_TCOLOR, WHITE);
        tft.setTextSize(TEXT_TSIZE);
        tft.print('=');
        tft.print(result);

        q = 0;
        num1 = 0;
        num2 = 0;
        pass = 0;
        nc = 0;
        fn = 0;
        result = 0;
        operator_type = 0;
        char textfield[TEXT_LEN + 1] = "";
        textfield_i = 0;
        n = true;
    }
    Serial.print("pass: ");
    Serial.println(pass);
    Serial.print("fn: ");
    Serial.println(fn);
    Serial.print("num1: ");
    Serial.println(num1);
    Serial.print("num2: ");
    Serial.println(num2);
    Serial.print("operator: ");
    Serial.println(operator_type);
    Serial.print("result: ");
    Serial.println(result);
    delay(100);
}

void pressed_button()
{
    textfield[textfield_i] = buttonlabels[q][0];
    textfield_i++;
    textfield[textfield_i] = 0;
}

void animate()
{
    if (buttons[q].justPressed())
    {
        buttons[q].drawButton(true);
        delay(300);
        buttons[q].drawButton();
    }
}

void display_text()
{
    tft.setCursor(TEXT_X + 2, TEXT_Y + 10);
    tft.setTextColor(TEXT_TCOLOR, WHITE);
    tft.setTextSize(TEXT_TSIZE);
    tft.print(textfield);
}

void clr_button()
{
    q = 0;
    num1 = 0;
    num2 = 0;
    nc = 0;
    fn = 0;
    pass = 0;
    result = 0;
    char textfield[TEXT_LEN + 1] = "";
    textfield_i = 0;
    operator_type = 0;
    n = true;
}