//
//  BoundTiles.h
//  GameBox
//
//  Created by Blaeeap on 8/30/13.
//
//

#ifndef __GameBox__BoundTiles__
#define __GameBox__BoundTiles__

#include "cocos2d.h"

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
 *  @brief  This is the base class for tiles in the bound game.
 */
class BTile : public Sprite
{
protected:
    float tileSize;
    Coords loc;
    bool on;
    Color3B onColor;
    Color3B offColor;
    
    /**
     *  @brief  Updates the current color based on the state of 'on'
     */
    void updateColor() { (on) ? setColor(onColor) : setColor(offColor); };
    
public:
    BTile();
    BTile(const Color3B onC, const Color3B offC);
    
    BTile* createWithFileColors(const char* image, const Color3B onC, const Color3B offC);
    
    /**
     *  @brief  Change the "On" color of the tile
     *  @param  color   The color to set as the onColor
     */
    void setOnColor(const Color3B color) { onColor = color; updateColor(); };
    
    /**
     *  @brief  Change the "Off" color of the tile
     *  @param  color   The color to set as the offColor
     */
    void setOffColor(const Color3B color) { onColor = color; updateColor(); };
    
    /**
     *  @brief  Sets the state of the Tile
     *  @param  state   True will set this to "On"
     */
    virtual void setOn(bool state) { on = state; updateColor(); };
    
    /**
     *  @brief  Reutrns the on start
     */
    bool isOn() {return on;};
    
    void updatePosition() {};
//    /**
//     *  @brief  Sets the location of the tile
//     *  @param  location    the x y coordinate of the tile
//     */
//    void setOn(bool location) { on = location; updatePosition(); };
//    bool isOn() {return on;};
    
};

/**
 *  @class  BFloorTile
 *  @brief  This is a floor tile, the player can move throught this freely
 */
class BFloorTile : public BTile
{

};

/**
 *  @class  BWallTile
 *  @brief  This is a wall tile, the player can not pass through this.
 */
class BWallTile : public BTile
{
    
};

/**
 *  @class  BExplosionTile
 *  @brief  This is an explosion tile, the player will die if they touch this while its on
 */
class BExplosionTile : public BTile
{
private:
    float start;
    float interval;
    float duration;
    
public:
    /**
     *  @brief  Special updateColor will fade to off rather than just instantly turn off
     */
    void updateColor() { if(on) setColor(onColor);
                        else runAction(Sequence::create(
                        TintTo::create(interval / 2, offColor.r, offColor.g, offColor.b), NULL));};

};

#endif /* defined(__GameBox__BoundTiles__) */
