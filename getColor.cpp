#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "getColor.h"

XColor getColor (Colormap cmap, int r, int g, int b, Display * d) {
    XColor pix;
    pix.red = r; pix.green = g; pix.blue = b;
	pix.flags = DoRed | DoGreen | DoBlue;
    XAllocColor(d, cmap, &pix);
    return pix;
}
