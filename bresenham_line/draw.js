let firstPoint;
let interval;
const colors   = ["black", "red", "blue"];

const putPixel = (x, y, color) => {
    console.log("Color of pixel: (" + x + ", " + y + ") is: " + color);
		document.getElementsByClassName(x + "-" + y)[0].style.backgroundColor = color;
}

const initGrid = () => {
    firstPoint       = null;
    interval         = document.getElementById("speed").value;
    let grid         = document.getElementById("grid");
    const grid_size  = document.getElementById("grid_size").value;
    const pixel_size = document.getElementById("pixel_size").value;
    
    while (grid.firstChild) {
      grid.removeChild(grid.firstChild);
    }
    
    for (let x = 0; x < grid_size; ++x) {
        let row = grid.insertRow(x);

        for (let y = 0; y < grid_size; ++y) {
          let cell = row.insertCell(y);
          cell.classList.add(x + "-" + y);
          cell.style.height = cell.style.width = pixel_size + "px";
          cell.onclick      = () => {
  
            if(firstPoint) {
              bresenham(firstPoint[0], firstPoint[1], x, y);
              firstPoint = null;
            } else {
              firstPoint = [x, y];
              putPixel(x, y, colors[0]);
            }
          }
        }
    }
}

const bresenhamLine = (x1, y1, x2, y2, color) => {
  let dx = Math.abs(x1 - x2);
  let dy = Math.abs(y1 - y2);

  const reverse = dx < dy;

  if(reverse) {
    [x1, x2, dx, y1, y2, dy] = [y1, y2, dy, x1, x2, dx];
  }

  const put = reverse ?
    (x, y) => putPixel(y, x, color) :
    (x, y) => putPixel(x, y, color);

  const incUP = -2*dx + 2*dy;
  const incDN = 2*dy;
  const incX  = x1 <= x2 ? 1 : -1;
  const inxY  = y1 <= y2 ? 1 : -1;
  let d       = -dx + 2*dy;
  let x       = x1;
  let y       = y1;

  for(let i = 0; i < dx + 1; ++i) {
		setTimeout(((xx, yy) => () => put(xx, yy))(x, y), interval * i);
		
    x += incX;
    if (d > 0) {
      d += incUP;
      y += inxY;
    }
    else {
      d += incDN;
    }
  }
}

const bresenham = (x1, y1, x2, y2) => {
  bresenhamLine(x1, y1, x2, y2, colors[0]);

  const timeout = Math.max(Math.abs(x1 - x2), Math.abs(y1 - y2)) * interval + 5000;

  const f = (lhs, rhs, value) => {
    return lhs < rhs ? 
    [Math.floor(value), Math.ceil(value)] :
    [Math.ceil(value), Math.floor(value)];
  }

  const [mX1, mX2] = f(x1, x2, (x1 + x2) / 2);
  const [mY1, mY2] = f(y1, y2, (y1 + y2) / 2);

  setTimeout(() => bresenhamLine(x1, y1, mX1, mY1, colors[1]), timeout);
  setTimeout(() => bresenhamLine(x2, y2, mX2, mY2, colors[2]), timeout);
}