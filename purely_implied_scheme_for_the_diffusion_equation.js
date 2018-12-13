const makeRange = (a, b, step) => {
  let array = [];
  let temp = a;

  for(let i = 0; temp < b; ++i) {
    temp = array[i] = a + i * step;
  }

  return array;
}

const progonka = (a, c, b, coef) => {
  let alpha = [0];
  let beta = [coef[0]];
  const n = c.length - 1;
  
  //forwand
  for(let i = 1; i < n; ++i) {
    const denom = c[i] - alpha[i-1] * a[i];
    alpha[i] =  b[i] / denom;
    beta[i] = (a[i] * beta[i-1] - coef[i]) / denom;
  }
  {
    const denom = c[n] - alpha[n-1] * a[n];
    beta[n] = (a[n] * beta[n-1] - coef[n]) / denom;
  }

  //backward calculating yi
  
  coef[n] = beta[n];

  for(let i = n - 1; i >= 0; --i) {
    coef[i] = alpha[i] * coef[i + 1] + beta[i];
  }

  return coef;
}

const forward = (h, tao, intervalX, intervalT, f, u0) => {
  const xs = makeRange(intervalX[0], intervalX[1], h);
  const ts = makeRange(intervalT[0], intervalT[1], tao);

  let y = Array(ts.length).fill().map(() => [0]);
  y[0].pop();
  
  for(let x of xs) {
    y[0].push(u0(x));
  }

  const gama = tao / (h * h);
  const n = xs.length - 1;

  for(let j = 1; j < ts.length; ++j) {
    for(let i = 1; i < n; ++i) {
      y[j][i] = y[j-1][i] + 
        gama * (y[j-1][i+1] - 2 * y[j-1][i] + y[j-1][i-1]) + tao * f(xs[i], ts[j - 1]);
    }

    y[j][n] = y[j-1][n] - 2 * gama * (y[j-1][n] - y [j-1][n-1]) + tao * f(xs[n], ts[j-1]);  
  }
  return y;
}

const backward = (h, tao, intervalX, intervalT, f, u0, l, r, r2) => {
  const xs = makeRange(intervalX[0], intervalX[1], h);
  const ts = makeRange(intervalT[0], intervalT[1], tao);
  const gama = tao / (h * h);
  const n = xs.length;

  let a = Array(n).fill(gama);
  a[0] = undefined;
  a[n-1] = 1/(2*h);

  let c = Array(n).fill(1 + 2*gama);
  c[0] = 1;
  c[n-1] = 1/(2*h) - h/(2*tao);

  let b = Array(n-1).fill(gama);
  b[0] = 0;
  
  let y = [[]];

  for(let x of xs) {
    y[0].push( u0(x) );
  }

  for(let j = 1; j < ts.length; ++j) {
    let fj = [l(ts[j])];

    for(let i = 1; i < n - 1; ++i) {
      fj[i] =  -y[j-1][i] - f(xs[i], ts[j]) * tao ;
    }
    fj[n-1] = h/(2*tao)*y[j-1][n-1] + r(intervalT[1]) + (h/2) * f(xs[n-1], ts[j]);
    
    y[j] = progonka(a, c, b, fj);
  }

  return y;
}

const main = (n = 10, tao = 0.5) => {
  const intervalX = [0, 1];
  const intervalT = [0, 2];
  const h = (intervalX[1] - intervalX[0]) / n;
  const f = (x, t) => - Math.exp(-t) * (0.5 * x * x - x + 1);
  const u = (x, t) => Math.exp(-t) * (0.5 * x * x - x);
  const u0 = (x) => u(x, intervalT[0]);
  const l = (t) => u(intervalX[0], t);
  const r = (t) => Math.exp(-t)*(intervalX[1] - 1);// du/dx(intervalX[1])
  const r2 = (t) => Math.exp(-t)
  const xs = makeRange(intervalX[0], intervalX[1], h);
  const ts = makeRange(intervalT[0], intervalT[1], tao);

  //const y = forward(h, tao, intervalX, intervalT, f, u0);
  const y = backward(h, tao, intervalX, intervalT, f, u0, l, r, r2);

  for(let j = 0; j < ts.length; ++j) {
    console.log("TIME = " + ts[j]);
    console.log("xi     , u(xi,t), y(xi,t)   , eps = u(xi, t) - y(xi, t)");
    
    for(let i = 0; i < xs.length; ++i) {
      
      const uout = u(xs[i], ts[j]);
      
      console.log(xs[i].toFixed(6) + " " + 
                  uout.toFixed(6) + " " + 
                  y[j][i].toFixed(6) + " " +
                  (uout - y[j][i]).toFixed(6)
      );
    }
  }
}

main(20,0.2);
