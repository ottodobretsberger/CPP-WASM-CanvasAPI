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
    }

    EMSCRIPTEN_KEEPALIVE int mandelbrot(float x, float y, int maxIterations) {
        float real = x;
        float imag = y;
        for (int i = 0; i < maxIterations; i++) {
            float real2 = real * real;
            float imag2 = imag * imag;
            if (real2 + imag2 > 4.0f) {
                return i;
            }
            imag = 2.0f * real * imag + y;
            real = real2 - imag2 + x;
        }
        return maxIterations;
    }

    EMSCRIPTEN_KEEPALIVE void escapeTimeToRGB(int escapeTime, int maxIterations, uint8_t* data, int offset) {
        if (escapeTime == maxIterations) {
            data[offset] = data[offset + 1] = data[offset + 2] = 0;
            data[offset + 3] = 255;
        } else {
            float t = (float)escapeTime / (float)maxIterations;
            data[offset]     = (uint8_t)(255 * (1.0f - t));
            data[offset + 1] = (uint8_t)(255 * (1.0f - t));
            data[offset + 2] = (uint8_t)(255 * t);
            data[offset + 3] = 255;
        }
    }

    EMSCRIPTEN_KEEPALIVE void drawMandelbrot(int canvasWidth, int canvasHeight, int maxIterations) {
        int size = canvasWidth * canvasHeight * 4;
        uint8_t* buffer = (uint8_t*)malloc(size);

        float scale = 4.0f / canvasWidth;
        float xOffset = -canvasWidth / 2.0f;
        float yOffset = -canvasHeight / 2.0f;

        clock_t t0 = clock();

        int idx = 0;
        for (int y = 0; y < canvasHeight; y++) {
            float imag = (y + yOffset) * scale;
            for (int x = 0; x < canvasWidth; x++) {
                float real = (x + xOffset) * scale;
                int escape = mandelbrot(real, imag, maxIterations);
                escapeTimeToRGB(escape, maxIterations, buffer, idx);
                idx += 4;
            }
        }

        clock_t t1 = clock();
        float elapsed = (t1 - t0) * 1000.0 / CLOCKS_PER_SEC;
        EM_ASM_({
            const canvas = document.getElementById('myCanvas');
            const ctx = canvas.getContext('2d');
            const imgData = ctx.createImageData($0, $1);
            const buffer = HEAPU8.subarray($2, $2 + $0 * $1 * 4);
            imgData.data.set(buffer);
            ctx.putImageData(imgData, 0, 0);
            alert("WASM Time: " + $3);
        }, canvasWidth, canvasHeight, buffer, elapsed);

        free(buffer);
    }
}
