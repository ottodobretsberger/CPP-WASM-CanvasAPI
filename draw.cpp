#include <emscripten.h>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <cstdio>  // for snprintf()

// Function to convert integer to hexadecimal string
void intToHex(int value, char* result) {
    static const char* digits = "0123456789ABCDEF";
    result[0] = digits[(value >> 20) & 0xF];
    result[1] = digits[(value >> 16) & 0xF];
    result[2] = digits[(value >> 12) & 0xF];
    result[3] = digits[(value >> 8) & 0xF];
    result[4] = digits[(value >> 4) & 0xF];
    result[5] = digits[value & 0xF];
    result[6] = '\0';
}

// Exported function to draw random rectangles
extern "C" {
    EMSCRIPTEN_KEEPALIVE void drawRectangleOnCanvas(int x, int y, int width, int height, const char* color) {
        EM_ASM_({
            var canvas = document.getElementById('myCanvas');
            var ctx = canvas.getContext('2d');
            ctx.fillStyle = UTF8ToString($4);
            ctx.fillRect($0, $1, $2, $3);
        }, x, y, width, height, color);
    }

    EMSCRIPTEN_KEEPALIVE void drawRandomRectangles(int numRectangles, int canvasWidth, int canvasHeight) {
        srand(time(NULL)); // Seed the random number generator

        for (int i = 0; i < numRectangles; ++i) {
            int x = rand() % canvasWidth;     // Random x-coordinate
            int y = rand() % canvasHeight;    // Random y-coordinate
            int width = rand() % 50 + 20;     // Random width between 20 and 70
            int height = rand() % 50 + 20;    // Random height between 20 and 70

            // Generate a random color
            char color[8]; // 7 characters for RGB hex code + null terminator
            intToHex(rand() % 0xFFFFFF, color);

            drawRectangleOnCanvas(x, y, width, height, color);
        }
    }
}
