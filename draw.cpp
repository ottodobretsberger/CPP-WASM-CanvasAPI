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
    EMSCRIPTEN_KEEPALIVE void drawRectangleOnCanvas2(int x, int y, int width, int height, int r, int g, int b) {
        EM_ASM_({
            // if($0 < 100) console.log("p: " + $0 + " " + $1 + " " + $4);
            var canvas = document.getElementById('myCanvas');
            var ctx = canvas.getContext('2d');

            // var rgbValue = Math.floor(255 * $4);
            ctx.fillStyle = 'rgb(' + $4 + ',' + $5 + ',' + $6 + ')';
            // if($0 < 100) console.log("p: " + $0 + " " + $1 + " " + ctx.fillStyle);
            ctx.fillRect($0, $1, $2, $3);
        }, x, y, width, height, r, g, b);
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

    EMSCRIPTEN_KEEPALIVE int mandelbrot(const std::complex<double>& c, int maxIterations) {
        std::complex<double> z = 0;
        int n = 0;
        while (abs(z) <= 2 && n < maxIterations) {
            z = z * z + c;
            n++;
        }
        return n;
    }

    EMSCRIPTEN_KEEPALIVE std::vector<int> escapeTimeToRGB(int escapeTime, int maxIterations) {
    if (escapeTime == maxIterations) {
        // Point is inside the Mandelbrot set, color it black
        return {0, 0, 0};
    } else {
        // Map escape time to a color gradient
        double t = static_cast<double>(escapeTime) / maxIterations;
        
        int red = static_cast<int>(255 * (1 - t));
        int green = static_cast<int>(255 * (1 - t));
        int blue = static_cast<int>(255 * t);
        return {red, green, blue};
    }
}

    EMSCRIPTEN_KEEPALIVE void drawMandelbrot(int canvasWidth, int canvasHeight, int maxIterations) {
        const double scaleX = 3.5 / canvasWidth;
        const double scaleY = 2.0 / canvasHeight;
        const double centerX = -0.7;
        const double centerY = 0;
        std::vector<std::vector<int>> mandelbrotSet(canvasHeight, std::vector<int>(canvasWidth));

        for (int y = 0; y < canvasHeight; ++y) {
            for (int x = 0; x < canvasWidth; ++x) {
                std::complex<double> c(centerX + (x - canvasWidth / 2) * scaleX, centerY + (y - canvasHeight / 2) * scaleY);
                mandelbrotSet[y][x] = mandelbrot(c,maxIterations);
            }
        }

        for(int i = 0; i < canvasHeight; i++)
        {
            for(int j = 0; j < canvasWidth; j++)
            {
                 std::vector<int> color = escapeTimeToRGB(mandelbrotSet[j][i], maxIterations);
                 int r = color[0];
                 int g = color[1];
                 int b = color[2];

                drawRectangleOnCanvas2(j,i,1,1,r,g,b);
            }
        }

       
    }
}
