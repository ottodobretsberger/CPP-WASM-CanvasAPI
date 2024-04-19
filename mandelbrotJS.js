let width = 0;
let height = 0;

let maxIterations = 0;

function mandelbrot(x, y) {
    let real = x;
    let imag = y;
    for (let i = 0; i < maxIterations; i++) {
        const real2 = real * real;
        const imag2 = imag * imag;
        if (real2 + imag2 > 4) {
            return i;
        }
        imag = 2 * real * imag + y;
        real = real2 - imag2 + x;
    }
    return maxIterations;
}
function drawMandelbrot(canvaswidth, canvasheight, iterations) {
    width = canvaswidth;
    height = canvasheight;
    maxIterations = iterations;

    const rows = width;
    const cols = height;
    const mandelbrotSet = createEmptyArray(rows, cols);
    for (let x = 0; x < width; x++) {
        for (let y = 0; y < height; y++) {
            const real = (x - width / 2) * 4 / width;
            const imag = (y - height / 2) * 4 / width;
            mandelbrotSet[x][y] = mandelbrot(real, imag);
        }
    }
    for (let i = 0; i < height; i++) {
        for (let j = 0; j < width; j++) {
            const color = escapeTimeToRGB(mandelbrotSet[i][j], maxIterations);
            
            const r = color[0];
            const g = color[1];
            const b = color[2];

            drawRectangleOnCanvas2(j, i, 1, 1, r, g, b);
        }
    }
}

function createEmptyArray(rows, cols) {
    let emptyArray = [];
    for (let i = 0; i < rows; i++) {
        emptyArray[i] = [];
        for (let j = 0; j < cols; j++) {
            emptyArray[i][j] = null;
        }
    }
    return emptyArray;
}

function escapeTimeToRGB(escapeTime, maxIterations) {
    if (escapeTime === maxIterations) {
        // Point is inside the Mandelbrot set, color it black
        return [0, 0, 0];
    } else {
        // Map escape time to a color gradient
        let t = escapeTime / maxIterations;
        
        let red = Math.round(255 * (1 - t));
        let green = Math.round(255 * (1 - t));
        let blue = Math.round(255 * t);
        return [red, green, blue];
    }
}

function drawRectangleOnCanvas2(x, y, width, height, r, g, b) {
        var canvas = document.getElementById('myCanvas');
        var ctx = canvas.getContext('2d');
        ctx.fillStyle = 'rgb(' + r + ',' + g + ',' + b + ')';
        ctx.fillRect(x, y, width, height);
}
