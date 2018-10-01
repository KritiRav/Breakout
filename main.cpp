// CS 349 Fall 2018
// A1: Breakout code sample
// You may use any or all of this code in your assignment!
// See makefile for compiling instructions

#include <cstdlib>
#include <iostream>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <sys/time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include "openWindow.h"
#include "getColor.h"


using namespace std;

 
int bricksDestroyed=0;
int timesFailed=0;
// get current time
unsigned long now() {
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

// entry point
int main( int argc, char *argv[] ) {
	stringstream strValue;
	strValue << argv[1];
	int speed;
	int FPS;
	strValue >> FPS;
	stringstream str2Value;
	str2Value << argv[2];
	str2Value >> speed;
	if (FPS<10||FPS>60) {
		cout<<"invalid FPS value. FPS must be between 10 and 60"<<endl;
		exit(0);
	} if (speed<1 || speed >10) {
		cout<<"invalid ball speed value. ballspeed must be between 10 and 60"<<endl;
		exit(0);
	}
	speed=speed/1.5;
	// create window
	XInfo xinfo;
	 
    initX(argc, argv, xinfo);

	usleep(10 * 1000);
	//create gc for main window
	Colormap cmap3= XDefaultColormap(xinfo.display,DefaultScreen(xinfo.display));
	XColor xbackground = getColor (cmap3, 25500,23000,25500, xinfo.display); 
	GC gc = XCreateGC(xinfo.display, xinfo.window, 0, 0);
  	XSetBackground(xinfo.display, gc, xbackground.pixel);
	XSetForeground(xinfo.display,gc,BlackPixel(xinfo.display,xinfo.screen));
	XFontStruct * font;
	font = XLoadQueryFont (xinfo.display, "fixed");
	XSetFont (xinfo.display, gc, font->fid);
	int start=1;

	// DOUBLE BUFFER
	// create bimap (pximap) to us a other buffer
	int depth = DefaultDepth(xinfo.display, DefaultScreen(xinfo.display));
	Pixmap  buffer = XCreatePixmap(xinfo.display, xinfo.window, 1280, 800, depth);
	Pixmap pixmap; 
	pixmap = xinfo.window;
	XSetForeground(xinfo.display, gc, WhitePixel(xinfo.display, DefaultScreen(xinfo.display)));
	XFillRectangle(xinfo.display, pixmap, gc,
						0, 0, 1280, 800);
	

	// ball postition, size, and velocity
	XPoint ballPos;
	ballPos.x = 400;
	ballPos.y = 360;
	int ballSize = 20;

	XPoint ballDir;
	ballDir.x = speed;
	ballDir.y = speed;

	// block position, size
	XPoint rectPos;
	rectPos.x =640;
	rectPos.y = 600;

	XWindowAttributes w;
	XGetWindowAttributes(xinfo.display, xinfo.window, &w);
	// save time of last window paint
	unsigned long lastRepaint = 0;

	// event handle for current event
	XEvent event;

	//setting up different colors for blocks
	Colormap cmap= XDefaultColormap(xinfo.display,DefaultScreen(xinfo.display));
	Colormap cmap2= XDefaultColormap(xinfo.display,DefaultScreen(xinfo.display));
	//Colormap cmap3= XDefaultColormap(xinfo.display,DefaultScreen(xinfo.display));
	XColor xred = getColor(cmap, 55000,0,0, xinfo.display);
	XColor xblack = getColor(cmap2, 0,55000,0, xinfo.display);
	XColor xblue = getColor(cmap3,0,0,55000, xinfo.display);
	XColor xrand = getColor (cmap2, 50000,65000,45000, xinfo.display);
	XColor xcol3 = getColor (cmap3, 14000,14000,35000, xinfo.display);
	XColor xcol4 = getColor (cmap3, 65000,35000,65000, xinfo.display);
	//XColor xbackground = getColor (cmap3, 255,230,255, xinfo.display);

	vector<vector<BrickPoint> > vecBrick (5, vector<BrickPoint> (15, CreateBrick(0,0,0)));
	//fill blocks
	for (int i=0; i<5; ++i) {
		 XSetForeground(xinfo.display, gc, xred.pixel);
		for (int y=0; y<15;++y) {
		XSetForeground(xinfo.display, gc, WhitePixel(xinfo.display, xinfo.screen));
		XFillRectangle(xinfo.display, pixmap, gc, y*75+76, i*45 +50, 75,45);
		XSetForeground(xinfo.display, gc, BlackPixel(xinfo.display, xinfo.screen));
		XDrawRectangle(xinfo.display, pixmap, gc, y*75 +76, i*45 +50, 75,45);
		vecBrick[i][y] = CreateBrick((y*75) + 50, (i*45)+76, 0);
		}
	}

	//text 
	string text("Game Over. Press q to quit or any other key to restart!");
	string text1 = string("Lives Left: ") + to_string(3-timesFailed);
	string text2 = string("Score: ") + to_string(bricksDestroyed);
	int numHit = 0; // number of bricks hit 

	// event loop
	while ( true ) {
			//check if start of game
		if (start) {
			string startGame1("userid: k6ravind      Name: Kriti Ravindran");
			string startGame2("How to play: ");
			string startGame3("Press any key/ click anywhere to start. The object of the game is to hit all the bricks. Press 'a' to move the paddle to the left. Press 'd' to move the paddle to the right");
			string startGame4("Each brick you hit will count as 1 point. You have 3 lives to finish the game");
			XClearWindow (xinfo.display, xinfo.window);
			XSetForeground(xinfo.display, gc, BlackPixel(xinfo.display, xinfo.screen));
			XDrawString( xinfo.display, xinfo.window, gc,
								450, 375, startGame1.c_str(), startGame1.length());
			XDrawString( xinfo.display, xinfo.window, gc,
								150, 415, startGame2.c_str(), startGame2.length());
			XDrawString( xinfo.display, xinfo.window, gc,
								150, 430, startGame3.c_str(), startGame3.length());
			XDrawString( xinfo.display, xinfo.window, gc,
								450, 470, startGame4.c_str(), startGame4.length());
			XEvent event0; 
	
			XNextEvent( xinfo.display, &event0 ); 

		switch ( event0.type ) {

		// mouse button press
		case ButtonPress:
			break;

		case KeyPress: // any keypress
			break;
		}
			start=0; 
		}
		// process if we have any events
		if (XPending(xinfo.display) > 0) { 
			XNextEvent( xinfo.display, &event ); 

			switch ( event.type ) {

				// mouse button press
				case ButtonPress:
					break;

				case KeyPress: // any keypress
					KeySym key;
					char text[10];
					int i = XLookupString( (XKeyEvent*)&event, text, 10, &key, 0 );

					// move right
					if ( i == 1 && text[0] == 'd' ) {
						rectPos.x += 15;
					}

					// move left
					if ( i == 1 && text[0] == 'a' ) {
						rectPos.x -= 15;
					}

					// quit game
					if ( i == 1 && text[0] == 'q' ) {
						XCloseDisplay(xinfo.display);
						exit(0);
					}
					break;
				} 
		}

		unsigned long end = now();	// get current time in microsecond

		if (end - lastRepaint > 1000000 / FPS) { 

	// clear background
	XClearWindow(xinfo.display, pixmap);

	//check if lost all lives or hit all bricks, then put splash screen saying game over
	if (timesFailed==3 || numHit==150) { 
	numHit=0;
	bricksDestroyed=0;
	if (ballDir.x<0) { ballDir.x = -ballDir.x; }
	XDrawString( xinfo.display, xinfo.window, gc,
						500, 400, text.c_str(), text.length());
	XEvent eventGameOver;
	timesFailed=0;
		XNextEvent( xinfo.display, &eventGameOver ); 
		vecBrick = resetBricks();
		switch (eventGameOver.type ) {
			
			// mouse button press
			case ButtonPress:
				break;

			case KeyPress: // any keypress
				KeySym key;
				char text[10];
				int i = XLookupString( (XKeyEvent*)&eventGameOver, text, 10, &key, 0 );
				// quit game
				if ( i == 1 && text[0] == 'q' ) {
					XCloseDisplay(xinfo.display);
					exit(0);
				}
				break;
			} 
		}

			//text 
			text1 = string("Lives Left: ") + to_string(3-timesFailed);
			text2 = string("Score: ") + to_string(bricksDestroyed);
			XDrawString( xinfo.display, xinfo.window, gc,
						10, 15, text1.c_str(), text1.length());
			XDrawString( xinfo.display, xinfo.window, gc,
						10, 30, text2.c_str(), text2.length());
			
			// draw rectangle
			XDrawRectangle(xinfo.display, pixmap, gc, rectPos.x, rectPos.y, 150, 25);


			// bounce ball
			if (ballPos.x + ballSize/2 > w.width ||
				ballPos.x - ballSize/2 < 0)
				ballDir.x = -ballDir.x;
			if (ballPos.y + ballSize/2 > w.height ||
				ballPos.y - ballSize/2 < 0)
				ballDir.y = -ballDir.y;
				//check if ball hit paddle, if so change direction
			if ((ballPos.x>=rectPos.x && ballPos.x<=rectPos.x+150)&&(ballPos.y>=590&&ballPos.y<=615)) {
				ballDir.y= -ballDir.y;
			}
			//check if ball hit brick, if so mark brick as destroyed
			for (int y=0; y<5; ++y) {
				for (int x=0; x<15; ++x) {
					if (!vecBrick[y][x].isHit) {
						if((ballPos.x > vecBrick[y][x].X - ballSize/2) && (ballPos.x < vecBrick[y][x].X + 75) &&
            (ballPos.y > vecBrick[y][x].Y - ballSize/2) && (ballPos.y < vecBrick[y][x].Y + 45)) {
							if((ballPos.x > vecBrick[y][x].X -ballSize) && (ballPos.x <= vecBrick[y][x].X + 75)
							&& (!(ballPos.y+ballSize > vecBrick[y][x].Y - ballSize)) && (!(ballPos.y< vecBrick[y][x].Y + 45))) {
								ballDir.x = -ballDir.x;
								vecBrick[y][x].isHit = 1; 
								++numHit;
								++bricksDestroyed;
							} else if ((ballPos.y > vecBrick[y][x].Y - ballSize) && (ballPos.y< vecBrick[y][x].Y + 45)) {
								ballDir.y = -ballDir.y;
								vecBrick[y][x].isHit = 1; 
								++numHit;
								++bricksDestroyed;
							}
							else if ((ballPos.x > vecBrick[y][x].X -75/2) && (ballPos.x <= vecBrick[y][x].X + 75/2)
							&& (!(ballPos.y+ballSize/2 > vecBrick[y][x].Y)) && (!(ballPos.y-ballSize/2< vecBrick[y][x].Y + 45)))
							{
								ballDir.x = -ballDir.x;
								vecBrick[y][x].isHit = 1; 
								++numHit;
								++bricksDestroyed;
							} 
							else if (1) {} 
						}
					}
				}
			}
			

			//draw bricks
			for (int y=0; y<5; y=y+1) {
				for (int x=0; x<15; x=x+1) {
					if (vecBrick[y][x].isHit==0) {
					if (y==0) { XSetForeground(xinfo.display, gc, xred.pixel); }
					else if (y==1) { XSetForeground(xinfo.display, gc, xblue.pixel); }
					else if (y==2) { XSetForeground(xinfo.display, gc, xrand.pixel);}
					else if (y==3) { XSetForeground(xinfo.display, gc, xcol3.pixel);}
					else { XSetForeground(xinfo.display, gc, xcol4.pixel); }
					//	cout<<"in loop, should make brick at: "<<vecBrick[y][x].X<<", "<<vecBrick[y][x].Y<<endl;
						XFillRectangle(xinfo.display, pixmap, gc, x*75 +76, y*45 +50, 75,45);
						XSetForeground(xinfo.display, gc, BlackPixel(xinfo.display, xinfo.screen));
						XDrawRectangle(xinfo.display, pixmap, gc, x*75 +76, y*45 +50, 75,45);
						}
					}
					XFlush(xinfo.display);
				}
			//XAllocColor(xinfo.display, cmap, &xblack);
			//cout<<"drew bricks"<<endl;

			// draw ball from centre
			XFillArc(xinfo.display, pixmap, gc, 
				ballPos.x - ballSize/2, 
				ballPos.y - ballSize/2, 
				ballSize, ballSize,
				0, 360*64);

			// update ball position
			ballPos.x += (ballDir.x*35)/FPS;
			ballPos.y += (ballDir.y*35)/FPS;

			//check if ball is on the bottom, reset 
			if (ballPos.y >= 760) {
				//ballPos.y = 200;
				ballPos.y = 360;
				ballPos.x = 400;
				if (ballDir.x<0) { ballDir.x=-ballDir.x;}
				++timesFailed;
			}
			// XCopyArea(xinfo.display, pixmap, xinfo.window, gc,
            //                               0, 0, 1280, 800,  // region of pixmap to copy
            //                               0, 0); // position to put top left corner of pixmap in window

			XFlush( xinfo.display );

			lastRepaint = now(); // remember when the paint happened
		}

		// IMPORTANT: sleep for a bit to let other processes work
		if (XPending(xinfo.display) == 0) {
			usleep(1000000 / FPS - (now() - lastRepaint));
		}
		//cout<<"exited loop"<<endl;
	}
	XCloseDisplay(xinfo.display);
}
