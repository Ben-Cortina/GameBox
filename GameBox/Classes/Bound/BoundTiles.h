/**-------------------------------------------------------------*
 * @file    Bound.h                                             *
 * @author  Ben Cortina                                         *
 * @date    8/30/13.                                            *
 *                                                              *
 * This holds the various types of Tiles that will be used in   *
 * The Bound Game.                                              *
 *                                                              *
 * Currently: Wall, Floor, Explosion Tiles                      *
 *--------------------------------------------------------------*/

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
    inline Coords& operator= (Coords r) {x = r.x; y = r.y; return *this;};
    inline Coords& operator+= (const Coords r) {(*this) = (*this) + r; return *this;};
    Coords operator+ (Coords r) {r.x = x + r.x; r.y = y + r.y; return r;};
    inline Coords& operator-= (const Coords r) {(*this) = (*this) - r; return *this;};
    Coords operator- (Coords r) {r.x = x - r.x; r.y = y - r.y; return r;};
};

///////////////////////////////////////////////////////////////////////////////////////////////////
/*=======================================      BTile      =======================================*/

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
    Color3B color;
    
    /**
     *  @brief  Updates the current color based on the state of 'on'
     */
    virtual void updateVisibility() { setVisible(on); };
    
    /**
     *  @brief  Updates the current position of the tile
     */
    void updatePosition() { setPosition( Point(loc.x * tileSize + tileSize/2, loc.y * tileSize + tileSize/2) ); };
    
public:
    BTile(const Color3B c, const Coords position);
    
    static BTile* createWithFileColorLoc(const char* image, const Color3B c, const Coords position);
    
    /** 
     *  @brief  Sets the state of the Tile
     *  @param  state   True will set this to "On"
     */
    void setOn(bool state) { on = state; updateVisibility(); };
    
    /**
     *  @brief  Reutrns the on start
     */
    bool isOn() {return on;};
    
    /**
     *  @brief  Sets the location of the tile
     *  @param  location    the x y coordinate of the tile
     */
    void setLocation(Coords location) { loc = location; updatePosition(); };
    Coords getLocation() {return loc;};
    
};

///////////////////////////////////////////////////////////////////////////////////////////////////
/*=====================================      BFloorTile      ====================================*/

/**
 *  @class  BFloorTile
 *  @brief  This is a floor tile, the player can move throught this freely
 */
class BFloorTile : public BTile
{
public:
    static BFloorTile* createWithFileColorLoc(const char* image, const Color3B c, const Coords position);
};

///////////////////////////////////////////////////////////////////////////////////////////////////
/*=====================================      BWallTile      =====================================*/

/**
 *  @class  BWallTile
 *  @brief  This is a wall tile, the player can not pass through this.
 */
class BWallTile : public BTile
{
public:
    static BWallTile* createWithFileColorLoc(const char* image, const Color3B c, const Coords position);
};

///////////////////////////////////////////////////////////////////////////////////////////////////
/*===================================      BExplosionTile      ==================================*/

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
    
    static BExplosionTile* createWithFileColorLoc(const char* image, const Color3B c, const Coords position)
                            { return (BExplosionTile*)BTile::createWithFileColorLoc(image, c, position); };

    /**
     *  @brief  Special updateColor will fade to off rather than just instantly turn off
     */
    void updateVisibility() { if(on) setOpacity(150);
                         else runAction( FadeOut::create(interval/2) );};

};

#endif /* defined(__GameBox__BoundTiles__) */
