//stillvrequires drivers and other files

#include <Arduino.h>
#include "LCD_Test.h"
#include <math.h>

#define YELLOW         0xFFE0  
#define DARK_YELLOW    0xFFA500  
#define BLACK          0x0000  

UWORD Imagesize = LCD_1IN28_HEIGHT * LCD_1IN28_WIDTH * 2;
UWORD *BlackImage = NULL;

void drawSmileyFace(int x, int y) {
    Paint_DrawCircle(x, y, 50, YELLOW, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(x, y, 50, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(x - 20, y - 20, 5, DARK_YELLOW, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(x + 20, y - 20, 5, DARK_YELLOW, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    // Draw the mouth (arc)
    for (int angle = 50; angle <= 130; angle++) { 
        int mouth_x = x + 40 * cos(angle * PI / 180);
        int mouth_y = y + 40 * sin(angle * PI / 180)-20; 
        Paint_DrawPoint(mouth_x, mouth_y, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    }
}

void setup() {
    Serial.begin(115200);

    if (psramInit()) {
        Serial.println("\nPSRAM is correctly initialized");
    } else {
        Serial.println("PSRAM not available");
    }
    if ((BlackImage = (UWORD *)ps_malloc(Imagesize)) == NULL) {
        Serial.println("Failed to apply for black memory...");
        exit(0);
    }

    if (DEV_Module_Init() != 0)
        Serial.println("GPIO Init Fail!");
    else
        Serial.println("GPIO Init successful!");

    LCD_1IN28_Init(HORIZONTAL);
    LCD_1IN28_Clear(WHITE);
    Paint_NewImage((UBYTE *)BlackImage, LCD_1IN28.WIDTH, LCD_1IN28.HEIGHT, 0, CYAN);
    Paint_SetScale(65);
    Paint_SetRotate(ROTATE_0);
    Paint_Clear(CYAN);

    LCD_1IN28_Display(BlackImage);
}

void loop() {
    static int smiley_x = LCD_1IN28_WIDTH / 2; 
    static int smiley_y = LCD_1IN28_HEIGHT / 2; 
    static float direction_angle = random(0, 360) * PI / 180.0; // Initial random direction angle

    Paint_Clear(CYAN);

    drawSmileyFace(smiley_x, smiley_y);

    float dx = 2 * cos(direction_angle);
    float dy = 2 * sin(direction_angle);

    smiley_x += dx;
    smiley_y += dy;

    // Circular constraints
    int radius = 50;
    int lcd_center_x = LCD_1IN28_WIDTH / 2;
    int lcd_center_y = LCD_1IN28_HEIGHT / 2;
    int distance_from_center = sqrt(pow(smiley_x - lcd_center_x, 2) + pow(smiley_y - lcd_center_y, 2));

    if (distance_from_center >= LCD_1IN28_HEIGHT / 2 - radius) {
        float angle_to_center = atan2(smiley_y - lcd_center_y, smiley_x - lcd_center_x);
        smiley_x = lcd_center_x + (LCD_1IN28_HEIGHT / 2 - radius) * cos(angle_to_center);
        smiley_y = lcd_center_y + (LCD_1IN28_HEIGHT / 2 - radius) * sin(angle_to_center);
        direction_angle = random(0, 360) * PI / 180.0; // Generate a new random direction angle
    }

    LCD_1IN28_Display(BlackImage);
    delay(5);
}
