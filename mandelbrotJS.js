let width = 0;
let height = 0;

let maxIterations = 0;

function mandelbrot(x, y, maxIterations) {
    let real = x;
    let imag = y;
    for (let i = 0; i < maxIterations; i++) {
        const real2 = real * real;
        const imag2 = imag * imag;
        if (real2 + imag2 > 4.0) {
            return i;
        }
        imag = 2 * real * imag + y;
        real = real2 - imag2 + x;
    }
    return maxIterations;
}

function escapeTimeToRGB(escapeTime, maxIterations, data, offset) {
    if (escapeTime === maxIterations) {
        data[offset] = 0;
        data[offset + 1] = 0;
        data[offset + 2] = 0;
        data[offset + 3] = 255;
    } else {
        const t = escapeTime / maxIterations;
        data[offset] = Math.round(255 * (1 - t));
        data[offset + 1] = Math.round(255 * (1 - t));
        data[offset + 2] = Math.round(255 * t);
        data[offset + 3] = 255;
    }
}

function drawMandelbrot(canvaswidth, canvasheight, iterations) {
    const canvas = document.getElementById('myCanvas');
    const ctx = canvas.getContext('2d');
    const imageData = ctx.createImageData(canvaswidth, canvasheight);
    const data = imageData.data;

    const scale = 4.0 / canvaswidth;
    const yOffset = -canvasheight / 2;
    const xOffset = -canvaswidth / 2;

    const startTime = performance.now();

    let idx = 0;
    for (let y = 0; y < canvasheight; y++) {
        const imag = (y + yOffset) * scale;
        for (let x = 0; x < canvaswidth; x++) {
            const real = (x + xOffset) * scale;
            const escape = mandelbrot(real, imag, iterations);
            escapeTimeToRGB(escape, iterations, data, idx);
            idx += 4;
        }
    }

    const elapsedTime = performance.now() - startTime;
    alert("JS Time: " + elapsedTime);

    ctx.putImageData(imageData, 0, 0);
}


function drawRectangleOnCanvas2(x, y, width, height, r, g, b) {
        var canvas = document.getElementById('myCanvas');
        var ctx = canvas.getContext('2d');
        ctx.fillStyle = 'rgb(' + r + ',' + g + ',' + b + ')';
        ctx.fillRect(x, y, width, height);
}
