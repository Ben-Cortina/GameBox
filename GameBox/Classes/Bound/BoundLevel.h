/**
 *  @file   BoundLevel.h
 *  @author Ben Cortina
 *  @date   8/30/13.
 */



#ifndef __GameBox__BoundLevel__
#define __GameBox__BoundLevel__

#include "BoundTiles.h"

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
    int width;
    int height;
    float tileSize;
    int fTileCount;
    int wTileCount;
    int eTileCount;
    
public:
    
    /**
     *  @brief  This will load in a level. Level formatting can be found in LevelMaking.bdl
     */
    BLevel(char* level);
    
    /**
     *  @brief  Checks if the provided location is a wall
     */
    bool isWall(const Coords loc);
    bool isWall(const int x, const int y) { return isWall( Coords(x, y) ); };
    
    /**
     *  @brief  Checks to see if there is an active explosion there
     */
    bool isExplosion(const Coords loc);
    bool isExplosion(const int x, const int y) { return isExplosion( Coords(x, y) ); };
    
    /**
     *  @brief  Checks if a rectangle collides with any tiles
     *  @return 0 if none, 1 if wall, 2 if explosion
     */
    int checkCollision(const Rect);
    
    Rect getTileBB(Point pt) { pt = pt - getPosition(); return Rect(tileSize * (int)(pt.x / tileSize),
                                                                    tileSize * (int)(pt.y / tileSize),
                                                                    tileSize, tileSize); };
    
};

#endif /* defined(__GameBox__BoundLevel__) */
