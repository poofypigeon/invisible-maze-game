#ifndef MACRO_H
#define MACRO_H

// 2D array coords to 1D index
#define cto1d(x, y, w) ((y) * (w) + (x))

// get 2D x coord from 1D index
#define ito2dx(i, w) ((i) % (w))

// get 2D y coord from 1D index
#define ito2dy(i, w) ((i) / (w))

#endif // MACRO_H