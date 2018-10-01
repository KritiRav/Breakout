/*
CS 349 Code Examples: X Windows and XLib

    openwindow     Opens a single blank window

- - - - - - - - - - - - - - - - - - - - - -

See associated makefile for compiling instructions

*/

#include <cstdlib>
#include <iostream>
#include <list>
#include <vector>
#include <unistd.h>
/*
 * Header files for X functions
 */
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include "openWindow.h"
#include "getColor.h"
using namespace std;


/*
 * Information to draw on the window.
 */


/*
 * Function to put out a message on error exits.
 */
void error( string str ) {
    cerr << str << endl;
    exit(0);
}

BrickPoint CreateBrick(int x, int y, int isHit) {
    BrickPoint a;
    a.X = x;
    a.Y = y;
    a.isHit = isHit;
    return a;
}

/*
 * Create a window
 */
void initSplash (int argc, char* argv[], XInfo& xinfo) {
      XSizeHints hints;

    hints.x = 450;
    hints.y = 250;
    hints.width = 400;
    hints.height = 300;
    hints.min_width=400;
    hints.max_width=400;
    hints.min_height=300;
    hints.max_height=300;
    hints.flags = PPosition | PSize | PMinSize | PMaxSize;

    /*
    * Display opening uses the DISPLAY  environment variable.
    * It can go wrong if DISPLAY isn't set, or you don't have permission.
    */
    xinfo.display = XOpenDisplay( "" );
    if ( !xinfo.display )   {
        error( "Can't open display." );
    }

    /*
    * Find out some things about the display you're using.
    */
    // DefaultScreen is as macro to get default screen index
    xinfo.screen = DefaultScreen( xinfo.display ); 

    unsigned long white, black;
    white = XWhitePixel( xinfo.display, xinfo.screen ); 
    black = XBlackPixel( xinfo.display, xinfo.screen );

    xinfo.window = XCreateSimpleWindow(
        xinfo.display,              // display where window appears
        DefaultRootWindow( xinfo.display ), // window's parent in window tree
        hints.x, hints.y,           // upper left corner location
        hints.width, hints.height,  // size of the window
        10,                         // width of window's border
        black,                      // window border colour
        white );                        // window background colour
     Atom type = XInternAtom(xinfo.display,"_NET_WM_WINDOW_TYPE", False);
     Atom value = XInternAtom(xinfo.display,"_NET_WM_WINDOW_TYPE_SPLASH", False);
     XChangeProperty(xinfo.display, xinfo.window, type, XA_ATOM, 32, PropModeReplace, reinterpret_cast<unsigned char*>(&value), 1);
    /* register interest in the delete window message */
     Atom wmDeleteMessage = XInternAtom(xinfo.display, "WM_DELETE_WINDOW", False);
     XSetWMProtocols(xinfo.display, xinfo.window, &wmDeleteMessage, 1);

    //extra window properties like a window title
    XSetStandardProperties(
        xinfo.display,      // display containing the window
        xinfo.window,       // window whose properties are set
        "x1_openWindow",    // window's title
        "OW",               // icon's title
        None,               // pixmap for the icon
        argv, argc,         // applications command line args
        &hints);            // size hints for the window

     XSetWMNormalHints(xinfo.display,xinfo.window,&hints);
	/*
     * Put the window on the screen.
     */
    XSelectInput(xinfo.display, xinfo.window, ButtonPressMask | KeyPressMask);
    XMapRaised( xinfo.display, xinfo.window );
    XFlush(xinfo.display);
}

 vector<vector<BrickPoint> > resetBricks() {
        vector<vector<BrickPoint> > newReset(5, vector<BrickPoint> (15, CreateBrick(0,0,0)));
        cout<<"enter resetBricks"<<endl; 
        //fill blocks
	for (int i=0; i<5; ++i) {
		// XSetForeground(xinfo.display, gc, xcolour.pixel);
		for (int y=0; y<15;++y) {
		newReset[i][y] = CreateBrick(y*75 + 56, i*45+50, 0);
		//cout<<"brick created at: "<< vecBrick[i][y].X<<", "<<vecBrick[i][y].Y<<endl;
		}
	}
    cout<<"exit resetBricks"<<endl;
    return newReset;
    }

void initX(int argc, char* argv[], XInfo& xinfo) {
    XSizeHints hints;

    hints.x = 0;
    hints.y = 0;
    hints.width = 1280;
    hints.height = 800;
    hints.min_width=1280;
    hints.max_width=1280;
    hints.min_height=800;
    hints.max_height=800;
    hints.flags = PPosition | PSize | PMinSize | PMaxSize;

    /*
    * Display opening uses the DISPLAY  environment variable.
    * It can go wrong if DISPLAY isn't set, or you don't have permission.
    */
    xinfo.display = XOpenDisplay( "" );
    if ( !xinfo.display )   {
        error( "Can't open display." );
    }

    /*
    * Find out some things about the display you're using.
    */
    // DefaultScreen is as macro to get default screen index
    xinfo.screen = DefaultScreen( xinfo.display ); 

    unsigned long black;
    Colormap cmap= XDefaultColormap(xinfo.display,DefaultScreen(xinfo.display));
	XColor xred = getColor(cmap, 50000,65000,55000, xinfo.display);
    black = XBlackPixel( xinfo.display, xinfo.screen );

    xinfo.window = XCreateSimpleWindow(
        xinfo.display,              // display where window appears
        DefaultRootWindow( xinfo.display ), // window's parent in window tree
        hints.x, hints.y,           // upper left corner location
        hints.width, hints.height,  // size of the window
        10,                         // width of window's border
        black,                      // window border colour
        xred.pixel );                        // window background colour
     
     XSetWMNormalHints(xinfo.display,xinfo.window,&hints);
	/*
     * Put the window on the screen.
     */
    XSelectInput(xinfo.display, xinfo.window, ButtonPressMask | KeyPressMask);
    XMapRaised( xinfo.display, xinfo.window );
    XFlush(xinfo.display);
}


// int main ( int argc, char* argv[] ) {

//     XInfo xinfo;

//     initX(argc, argv, xinfo);

//     // wait for user input to quit (a concole event for now)
//     cout << "press ENTER to exit";
//     cin.get();

//     XCloseDisplay(xinfo.display);
// }
