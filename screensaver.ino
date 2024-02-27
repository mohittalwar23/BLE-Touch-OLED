// requires libraries and drivers , not published here 
#include <Arduino.h>
#include "LCD_Test.h"
#include <math.h>

// Define custom color for darker yellow
#define YELLOW         0xFFE0  // Yellow color
#define DARK_YELLOW    0xFFA500  // Darker shade of yellow
#define BLACK          0x0000  // Black color

UWORD Imagesize = LCD_1IN28_HEIGHT * LCD_1IN28_WIDTH * 2;
UWORD *BlackImage = NULL;

void drawSmileyFace(int x, int y) {
    // Draw the face (hollow circle)
    Paint_DrawCircle(x, y, 50, YELLOW, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(x, y, 50, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);

    // Draw the eyes (filled circles)
    Paint_DrawCircle(x - 20, y - 20, 5, DARK_YELLOW, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(x + 20, y - 20, 5, DARK_YELLOW, DOT_PIXEL_1X1, DRAW_FILL_FULL);

    // Draw the mouth (arc)
    for (int angle = 50; angle <= 130; angle++) { // Changed angle range for the mouth
        int mouth_x = x + 40 * cos(angle * PI / 180);
        int mouth_y = y + 40 * sin(angle * PI / 180)-20; // Adjusted y-coordinate for the mouth
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
    static int smiley_x = 60; // Initial x-coordinate of the smiley face
    static int smiley_y = 60; // Initial y-coordinate of the smiley face
    static float direction_angle = PI/4; // Initial direction angle of movement

    // Clear the screen
    Paint_Clear(CYAN);

    // Draw the smiley face at its current position
    drawSmileyFace(smiley_x, smiley_y);

    // Calculate the movement components
    float dx = 2 * cos(direction_angle);
    float dy = 2 * sin(direction_angle);

    // Update the smiley face position
    smiley_x += dx;
    smiley_y += dy;

    // Handle collisions with the edges of the screen
    if (smiley_x >= LCD_1IN28_WIDTH - 50 || smiley_x <= 50 || smiley_y >= LCD_1IN28_HEIGHT - 50 || smiley_y <= 50) {
        // Generate a random direction angle
        direction_angle = random(0, 360) * PI / 180.0; // Convert degrees to radians

        // Adjust the smiley face position to prevent immediate collision
        if (smiley_x >= LCD_1IN28_WIDTH - 50) smiley_x = LCD_1IN28_WIDTH - 51;
        else if (smiley_x <= 50) smiley_x = 51;
        if (smiley_y >= LCD_1IN28_HEIGHT - 50) smiley_y = LCD_1IN28_HEIGHT - 51;
        else if (smiley_y <= 50) smiley_y = 51;
    }

    // Display the updated image
    LCD_1IN28_Display(BlackImage);

    // Delay to control the animation speed
    delay(10);
}
