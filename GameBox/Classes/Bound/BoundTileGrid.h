//
//  BoundTileGrid.h
//  GameBox
//
//  Created by Blaeeap on 8/29/13.
//
//

#ifndef __GameBox__BoundTileGrid__
#define __GameBox__BoundTileGrid__

#include <iostream>
#include "cocos2d.h"

//this is namespace cocos2d
USING_NS_CC;

struct Coords
{
    int x;
    int y;
    Coords(){};
    Coords(int px, int py):x(px),y(py){};
    
    inline bool operator== (const Coords r) {return (x == r.x && y == r.y);};
    inline Coords& operator= (const Coords r) {x = r.x; y = r.y; return *this;};
    inline Coords& operator+= (const Coords r) {(*this) = (*this) + r; return *this;};
    Coords operator+ (Coords r) {r.x = x + r.x; r.y = y + r.y; return r;};
    inline Coords& operator-= (const Coords r) {(*this) = (*this) - r; return *this;};
    Coords operator- (Coords r) {r.x = x - r.x; r.y = y - r.y; return r;};
};

/**
 *  @class  BTile
 *  @brief  This represents a tile that can be toggled on or off.
 */
class BTile : public Sprite
{
private:
    inline void updateColor() { (on) ? setColor(onColor) : setColor(offColor); };
    
protected:
    bool on;
    Color3B onColor;
    Color3B offColor;
    
public:
    BTile();
    BTile(const Color3B onC, const Color3B offC);
    
    BTile* createWithFileColors(const char* image, const Color3B onC, const Color3B offC);
    
    /**
     *  @brief  Change the "On" color of the tile
     *  @param  color   The color to set as the onColor
     */
    void setOnColor(const Color3B color) {onColor = color; updateColor(); };
    
    /**
     *  @brief  Change the "Off" color of the tile
     *  @param  color   The color to set as the offColor
     */
    void setOffColor(const Color3B color) {onColor = color; updateColor(); };
    
    /**
     *  @brief  Sets the state of the Tile
     *  @param  state   True will set this to "On"
     */
    void setOn(bool state) { on = state; updateColor(); };
    
};

/**
 *  @class  BTileGrid
 *  @brief  This holds an array of BTiles that act as a grid.
 */
class BTileGrid
{
protected:
    BTile** pTiles; //an array containing all the tiles
    int width;
    int height;
    
public:
    
    BTileGrid(int w, int h);
    
    /**
     *  @brief  This will load in a level. Level formatting can be found in LevelMaking.bdl
     */
    BTileGrid* creatWithLevel(char* level, char* image);
    
};

#endif /* defined(__GameBox__BoundTile__) */
