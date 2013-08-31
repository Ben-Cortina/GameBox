//
//  BoundLevel.h
//  GameBox
//
//  Created by Blaeeap on 8/30/13.
//
//

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
    BWallTile** wTile;
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
    BLevel* creatWithFile(char* level);
    
    
};

#endif /* defined(__GameBox__BoundLevel__) */
