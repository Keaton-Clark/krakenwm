#ifndef HANDLER_H
#define HANDLER_H
#include <X11/Xlib.h>
#include <stdbool.h>
bool sendevent(Window win, Atom proto);

void	maprequest		(XEvent* ev);
void	configurerequest	(XEvent* ev);
void	buttonpress		(XEvent* ev);
void	keypress		(XEvent* ev);
void	expose			(XEvent* ev);

#endif //HANDLER_H
