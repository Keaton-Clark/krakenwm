#include "bar.h"

static Window bar;
static unsigned int rootw, rooth;
static int scrn = 0;
static Display *dpy;
static GC gc;

void drawkraken(int x, int y);
void drawtabs(int m);

void bar_printf(int location, char *format, ...) {
	va_list valist;
	static int i = 0;
	char str[50];
	va_start(valist, format);
	vsprintf(str, format, valist);
	va_end(valist);
	XClearWindow(dpy, bar);
	//XDrawString(dpy, bar, gc, i, 15, str, strlen(str));
	
	i += 50;
	drawtabs(7);
	drawkraken(0, 0);
}

void drawtabs(int m) {
	for (int n = 0; n < 9; n++) {
		if (m == n + 1)
			XFillRectangle(dpy, bar, gc, 100 + (n * 15), 10, 5, 5);
		XDrawRectangle(dpy, bar, gc, 100 + (n * 15), 10, 5, 5);
	}
}

void drawkraken(int x, int y) {
	for (int i = 0; i < 27; i++) {
		for (int n = 0; n < 19; n++) {
			if (kraken[i][n] == 1)
				XDrawPoint(dpy, bar, gc, n + y, i + x);
		}
	}
}

void bar_init(Display *dpy1, Window root) {
	dpy = dpy1;
	XGCValues vals;
	vals.foreground = 0xCB4B16;
	//vals.foreground = 0x586e75;
	//gc = DefaultGC(dpy, scrn);
	bar = XCreateSimpleWindow(dpy, root, RESOLUTION_X - BAR_WIDTH - 1, -1, BAR_WIDTH, RESOLUTION_Y, 1, 0x586e75, 0x002b36);
	gc = XCreateGC(dpy, bar, GCForeground, &vals);
	//XSelectInput(dpy, bar, ExposureMask|KeyPressMask);
	XMapWindow(dpy, bar);
}
