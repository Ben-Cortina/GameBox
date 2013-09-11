//
//  CustomTileGrid.h
//  GameBox
//
//  Created by Blaeeap on 9/11/13.
//
//

#ifndef __GameBox__CustomTileGrid__
#define __GameBox__CustomTileGrid__

#include <iostream>

struct Coords
{
    int x;
    int y;
    Coords(){};
    Coords(int px, int py):x(px),y(py){};
    
    inline bool operator== (const Coords r) {return (x == r.x && y == r.y);};
    inline Coords& operator= (Coords r) {x = r.x; y = r.y; return *this;};
    inline Coords& operator+= (const Coords r) {(*this) = (*this) + r; return *this;};
    Coords operator+ (Coords r) {r.x = x + r.x; r.y = y + r.y; return r;};
    inline Coords& operator-= (const Coords r) {(*this) = (*this) - r; return *this;};
    Coords operator- (Coords r) {r.x = x - r.x; r.y = y - r.y; return r;};
};

#endif /* defined(__GameBox__CustomTileGrid__) */
