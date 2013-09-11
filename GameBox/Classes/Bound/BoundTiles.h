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
#include "CustomTileGrid.h"

USING_NS_CC;

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
    BTile(){};
    
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
    static BFloorTile* createWithFileColorLoc(const char* image, const Color3B c, const Coords position)
                            {return (BFloorTile*)BTile::createWithFileColorLoc(image, c, position); };
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
    static BWallTile* createWithFileColorLoc(const char* image, const Color3B c, const Coords position)
                            {return (BWallTile*)BTile::createWithFileColorLoc(image, c, position); };
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
    float interval;
    float duration;
    bool exploded;
    
public:
    
    BExplosionTile(const Color3B c, Coords position);
    
    static BExplosionTile* createWithFileColorLoc(const char* image, const Color3B c, const Coords position);
    
    /** @brief set method for interval */
    void setInterval(const float f) { interval = f; };
    /** @brief Get method for interval */
    float getInterval() { return interval; };
    
    /** @brief set method for duration */
    void setDuration(const float f) { duration = f; };
    /** @brief Get method for duration */
    float getDuration() { return duration; };
    
    /**
     *  @brief  holds the exploded state until it has been checked
     */
    bool hasExploded()
    { if(exploded)
        {
            exploded = false;
            return true;
        } else
            return false;
    };

    /**
     *  @brief  Special updateColor will fade to off rather than just instantly turn off
     */
    void updateVisibility() { if(on) setOpacity(150);
                         else runAction( FadeOut::create(interval/2) );};
    
    /**
     *  @brief  schedules firstFade and firstExplode once
     */
    void scheduleFirstExplode(float st);
    
    /**
     *  @brief  fades then schedules fade
     */
    void firstFade(float dt);
    
    /**
     *  @brief  explodes then schedules explode
     */
    void firstExplode(float dt);
    
    /**
     *  @brief  sets exploded then sets the tile on
     */
    void explode(float dt);
    
    /**
     *  @brief  sets the tile off
     */
    void fade(float dt);

};

#endif /* defined(__GameBox__BoundTiles__) */
