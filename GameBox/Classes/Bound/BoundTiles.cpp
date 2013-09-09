/**
 *  @file   BoundTiles.cpp
 *  @author Ben Cortina
 *  @date   8/30/13.
 */


#include "BoundTiles.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
/*=======================================      BTile      =======================================*/

BTile::BTile(const Color3B c, Coords position): color(c), loc(position)
{
    on = false;
    updateVisibility();
}

BTile* BTile::createWithFileColorLoc(const char* image, const Color3B c, const Coords position)
{
    BTile *pobBTile = new BTile(c, position);
    if (pobBTile && pobBTile->initWithFile(image))
    {
        pobBTile->autorelease();
        return pobBTile;
    }
    CC_SAFE_DELETE(pobBTile);
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*====================================      BFloorTile      =====================================*/
BFloorTile* BFloorTile::createWithFileColorLoc(const char* image, const Color3B c, const Coords position)
{
    BFloorTile* tile = (BFloorTile*)BTile::createWithFileColorLoc(image, c, position);
    if (tile)
        tile->setOn(true);
    return tile;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
/*=====================================      BWallTile      =====================================*/
BWallTile* BWallTile::createWithFileColorLoc(const char* image, const Color3B c, const Coords position)
{
    BWallTile* tile = (BWallTile*)BTile::createWithFileColorLoc(image, c, position);
    if (tile)
        tile->setOn(true);
    return tile;
}

