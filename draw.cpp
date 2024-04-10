#include <emscripten.h>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()

// Function to convert integer to string
void intToString(int value, char* result) {
    // Handle negative values
    if (value < 0) {
        *result++ = '-';
        value = -value;
    }
    
    // Convert digits to characters in reverse order
    char buffer[20]; // Buffer to store the digits
    int index = 0;
    do {
        buffer[index++] = value % 10 + '0';
        value /= 10;
    } while (value > 0);
    
    // Copy the digits in reverse order to the result string
    while (index > 0) {
        *result++ = buffer[--index];
    }
    *result = '\0'; // Null-terminate the string
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
        
        for(int i = 0; i < canvasWidth; i+=4)
        {
            for(int j = 0; j < canvasHeight; j+=4)
            {
                int width = 3;
                int height = 3;
                char color[20];
                 color[0] = 'r'; color[1] = 'g'; color[2] = 'b'; color[3] = '(';
                 color[4] = '0'; color[5] = ','; color[6] = '0'; color[7] = ',';
                 color[8] = '0'; color[9] = ')'; color[10] = '\0';

                 drawRectangleOnCanvas(i, j, width, height, color);
            }
        }


        // for(int i = 0; i < 30; i++)
        // {
        //     for (int i = 0; i < numRectangles; ++i) {
        //             int x = rand() % canvasWidth;     // Random x-coordinate
        //             int y = rand() % canvasHeight;    // Random y-coordinate
        //             int width = rand() % 50 + 10 ;     // Random width between 10 and 70
        //             int height = rand() % 50 + 10;    // Random height between 10 and 70

        //             // Generate a random color in RGB format
        //             char color[20]; // Sufficient size for the color string
        //             color[0] = 'r'; color[1] = 'g'; color[2] = 'b'; color[3] = '(';
        //             intToString(rand() % 256, color + 4); // Red component
        //             color[6] = ',';
        //             intToString(rand() % 256, color + 7); // Green component
        //             color[9] = ',';
        //             intToString(rand() % 256, color + 10); // Blue component
        //             color[12] = ')';
        //             color[13] = '\0';
                    
        //             drawRectangleOnCanvas(x, y, width, height, color);
        //     }
        // }
    }
}
