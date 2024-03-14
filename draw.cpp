#include <emscripten.h>

extern "C" {
    void drawRectangleOnCanvas(int x, int y, int width, int height) {
        EM_ASM_({
            var canvas = document.getElementById('myCanvas');
            var ctx = canvas.getContext('2d');
            ctx.fillStyle = 'red';
            ctx.fillRect($0, $1, $2, $3);
        }, x, y, width, height);
    }
}
