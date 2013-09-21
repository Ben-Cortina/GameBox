/**
 *  @file   BoundLevel.h
 *  @author Ben Cortina
 *  @date   8/30/13.
 */



#ifndef __GameBox__BoundLevel__
#define __GameBox__BoundLevel__

#include "BoundTiles.h"
#include <vector>

USING_NS_CC;

/**
 *  @struct ExplosionSchedule
 *  @brief  This holds the information needed for representing one explosion
 */
struct ExplosionSchedule
{
    float start;
    float interval;
    float duration;
    Color3B Color;
};

/**
 *  @class  BLevel
 *  @brief  This will hold and draw the data for a level.
 */
class BLevel : public Layer
{
private:
    BFloorTile** fTiles;
    BWallTile** wTiles;
    BExplosionTile** eTiles;
    float exlosionTimes;
    Sprite* playerSprite;
    int width;
    int height;
    float tileSize;
    int fTileCount;
    int wTileCount;
    int eTileCount;
    
    Point start;
    Coords* end;
    int endCount;
    bool valid;
    
    /**
     *  @brief  This will parse a .bdl file and populate BLevel
     */
    bool parseFile(const char* file);
    
    /**
     *  @brief  Size and position all of the tiles
     */
    void setupTiles();
    
public:
    
    /** @brief  This will load in a level. Level formatting can be found in LevelMaking.bdl */
    BLevel(const char* file);
    
    /** @brief returns the starting location */
    Point getStart() { return start; };
    
    /** @brief returns the tileSize */
    float getTileSize() { return tileSize; };
    
    /** @brief  Checks if the player is fully inside an exit */
    bool inExit(const Rect bb);
    
    /** @brief  Did the level load */
    bool isValid(){ return valid; };
    
    /**
     *  @brief  Checks if a rectangle collides with any Wall tiles
     *  @return 0 if none, 1 if wall, 2 if explosion
     */
    bool isWallCollision(Rect bb, Rect& tileBB);
    
    /**
     *  @brief  Checks if a rectangle collides with any Active Explosions tiles. If the player does, it dies.
     */
    bool isExplosionCollision(Rect bb);
    
    /**
     *  @brief  This checks if any explosions have gone off on top of the player.
     */
    bool checkExplosions(Rect bb);
    
    /**
     *  @brief  Checks if the passed rectangle touches any floor.
     *          If the player does not intersect any of the floor tiles he isnt touching any floor
     *          thus, he falls.
     */
    bool isOnFloor(Rect bb);
    
    Coords getTile(Point pt) { pt = pt - getPosition(); return Coords((int)(pt.x / tileSize),
                                                                      (int)(pt.y / tileSize)); };
    
    Rect getTileBB(Coords loc);
    
    
};

#endif /* defined(__GameBox__BoundLevel__) */
