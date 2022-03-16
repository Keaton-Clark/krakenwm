#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "bar.h"
#include "handler.h"

void init();
void loop();
Display* dpy;
int scrn;
void tile();

void (*handler[])(XEvent *ev) = {
	[KeyPress] = keypress,
	[MapRequest] = maprequest,
	[ConfigureRequest] = configurerequest,
	[ButtonPress] = buttonpress,
	[Expose] = expose,
};


typedef struct Client Client;
struct Client {
	Window window;
	Client* next;
};
typedef struct Root Root;
struct Root {
	unsigned int w, h;
	int x, y;
	Window win;
	Window bar;
	GC gc;
};
Root* root;


static Client* head;
void pop(Client* c);
void push(Client* c);
Client* getclient(Window w);
Client* newclient(Window w);

bool sendevent(Window win, Atom proto) {
    int n;
    Atom *protocols;
    bool exists = false;
    XEvent ev;

    if (XGetWMProtocols(dpy, win, &protocols, &n)) {
        while (!exists && n--) exists = protocols[n] == proto;
        XFree(protocols);
    }

    if (exists) {
        ev.type = ClientMessage;
        ev.xclient.window = win;
        ev.xclient.message_type = XInternAtom(dpy, "WM_PROTOCOLS", True);
        ev.xclient.format = 32;
        ev.xclient.data.l[0] = proto;
        ev.xclient.data.l[1] = CurrentTime;
        XSendEvent(dpy, win, False, NoEventMask, &ev);
    }
    return exists;
}

void maprequest(XEvent* ev) {
	XMapRequestEvent* mrev = &ev->xmaprequest;
	Client* c = newclient(mrev->window);
	push(c);
	tile();
	XMapWindow(dpy, mrev->window);
}

void keypress(XEvent* ev) {
	sendevent(ev->xbutton.subwindow, XInternAtom(dpy, "WM_DELETE_WIDOW", True));
	XDestroyWindow(dpy, ev->xbutton.subwindow);
	pop(getclient(ev->xbutton.subwindow));
	tile();
}

void configurerequest(XEvent* ev) {
	Client* c;
	XWindowChanges wc;
	wc.x = ev->xconfigurerequest.x;
	wc.y = ev->xconfigurerequest.y;
	wc.width = ev->xconfigurerequest.width;
	wc.height = ev->xconfigurerequest.height;
	wc.border_width = ev->xconfigurerequest.border_width;
	wc.sibling = ev->xconfigurerequest.above;
	wc.stack_mode = ev->xconfigurerequest.detail;
	XConfigureWindow(dpy, ev->xconfigurerequest.window, ev->xconfigurerequest.value_mask, &wc);

}

void expose(XEvent *ev) {
}

void buttonpress(XEvent* ev) {
}


void push(Client* c) {
	if (!head) {head = c; return;}
	Client *i = head;
	while(i->next) {i = i->next;}
	i->next = c;
}

Client* newclient(Window w) {
	Client* c;
	if (!(c = (Client*)malloc(sizeof(Client)))) exit(EXIT_FAILURE);
	c->window = w;
	c->next = NULL;
}

Client* getclient(Window w) {
	Client* c = head;
	do { 
		if (c->window == w) return c;
	} while (c = c->next);
	return NULL;
}

void pop(Client* c) {
	if (c == head) {head = head->next; return;}
	Client* i = head;
	while (i->next != c) i = i->next;
	i->next = c->next;
}

void tile() {
	if (!head) return;
	int i = 1;
	Client* c = head;
	while (c = c->next) i++;
	if (i == 1)
		XMoveResizeWindow(dpy, head->window, 0, 0, XDisplayWidth(dpy, scrn), XDisplayHeight(dpy, scrn)-28);
	else if (i == 2) {
		XMoveResizeWindow(dpy, head->window, 0, 0, XDisplayWidth(dpy, scrn)/2, XDisplayHeight(dpy, scrn)-28);
		XMoveResizeWindow(dpy, head->next->window, XDisplayWidth(dpy, scrn)/2, 0, XDisplayWidth(dpy, scrn)/2, XDisplayHeight(dpy, scrn)-20);
	}
}

void init() {
	XInitThreads();
	if(!(dpy = XOpenDisplay(NULL))) exit(EXIT_FAILURE);
	scrn = DefaultScreen(dpy);
	root = (Root*)malloc(sizeof(Root));
	root->win = DefaultRootWindow(dpy);
	root->x = 0; root->y = 0;
	root->w = XDisplayWidth(dpy, scrn);
	root->h = XDisplayHeight(dpy, scrn) - 20;
	bar_init(dpy, root->win);
	XSelectInput(dpy, root->win, SubstructureRedirectMask);
	XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("c")), ShiftMask|Mod1Mask, root->win, True, GrabModeAsync, GrabModeAsync);
}

void loop() {
	XEvent ev;
	while(!XNextEvent(dpy, &ev)) {
		bar_printf(0, "%d", ev.type);
		if (handler[ev.type]) handler[ev.type](&ev);
	}
}

int main() {
	init();	
	loop();
}
