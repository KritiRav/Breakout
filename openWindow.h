#ifndef XINFO_H
#define XINFO_H

using namespace std;

struct XInfo {
    Display*    display;
    int      screen;
    Window   window;
};

struct BrickPoint {
    int X;
    int Y;
    bool isHit;
};

BrickPoint CreateBrick(int x, int y, int isHit);

vector<vector<BrickPoint> > resetBricks(); 

void error( string str );

void initSplash (int argc, char* argv[], XInfo& xinfo);

void initX(int argc, char* argv[], XInfo& xinfo);

#endif