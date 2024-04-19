#include <emscripten.h>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for time()
#include <cmath>
#include <iostream>
#include <vector>
#include <complex>

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
            if($0 < 100) console.log("p: " + $0 + " " + $1 + " " + $4);
            var canvas = document.getElementById('myCanvas');
            var ctx = canvas.getContext('2d');
            ctx.fillStyle = UTF8ToString($4);
            ctx.fillRect($0, $1, $2, $3);
        }, x, y, width, height, color);
    }
    EMSCRIPTEN_KEEPALIVE void drawRectangleOnCanvas2(int x, int y, int width, int height, int r, int g, int b, float clocktime) {
        EM_ASM_({
            if($7 > 0.0) alert("time elapsed: " + $7);
            // if($0 < 100) console.log("p: " + $0 + " " + $1 + " " + $4);
            var canvas = document.getElementById('myCanvas');
            var ctx = canvas.getContext('2d');

            // var rgbValue = Math.floor(255 * $4);
            ctx.fillStyle = 'rgb(' + $4 + ',' + $5 + ',' + $6 + ')';
            // if($0 < 100) console.log("p: " + $0 + " " + $1 + " " + ctx.fillStyle);
            ctx.fillRect($0, $1, $2, $3);
        }, x, y, width, height, r, g, b, clocktime);
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

    EMSCRIPTEN_KEEPALIVE int mandelbrot(float x, float y, int maxIterations) {
        float real = x;
        float imag = y;
        for (int i = 0; i < maxIterations; i++) {
            const float real2 = real * real;
            const float imag2 = imag * imag;
            if (real2 + imag2 > 4) {
                return i;
            }
            imag = 2 * real * imag + y;
            real = real2 - imag2 + x;
        }
        return maxIterations;
    }

    EMSCRIPTEN_KEEPALIVE void escapeTimeToRGB(int escapeTime, int maxIterations, int result [3]) {
    if (escapeTime == maxIterations) {
        // Point is inside the Mandelbrot set, color it black
        result[0] = 0;
        result[1] = 0;
        result[2] = 0;
        return;
    } else {
        // return {255, 255, 255};
        // Map escape time to a color gradient
        float t = static_cast<double>(escapeTime) / static_cast<double>(maxIterations);
        
        result[0] = static_cast<int>(255 * (1.0 - t));
        result[1] = static_cast<int>(255 * (1.0 - t));
        result[2] = static_cast<int>(255 * t);
    }
}

    EMSCRIPTEN_KEEPALIVE void drawMandelbrot(int canvasWidth, int canvasHeight, int maxIterations) {
        const int rows = canvasHeight;
        const int cols = canvasWidth;
        // static int mandelbrotSet[1600][1200];
        std::vector<std::vector<int>> mandelbrotSet(rows, std::vector<int>(cols));
        unsigned int t0 = clock();
        for (int y = 0; y < canvasHeight; y++) {
            for (int x = 0; x < canvasWidth; x++) {
                const float real = ((float)x - canvasWidth / 2) * 4 / canvasWidth;
                const float imag = ((float)y - canvasHeight / 2) * 4 / canvasWidth;
                mandelbrotSet[y][x] = mandelbrot(real, imag, maxIterations);
            }
        }
        unsigned int t1 = clock();
        float clockTime = (t1 - t0)*1000.0 / CLOCKS_PER_SEC;
        for (int i = 0; i < canvasHeight; i++) {
            for (int j = 0; j < canvasWidth; j++) {
                int color [3] = {0,0,0};
                escapeTimeToRGB(mandelbrotSet[i][j], maxIterations, color);
                
                const int r = color[0];
                const int g = color[1];
                const int b = color[2];

                drawRectangleOnCanvas2(j, i, 1, 1, r, g, b, clockTime);
                clockTime = 0.0;
            }
        }
    }
}
