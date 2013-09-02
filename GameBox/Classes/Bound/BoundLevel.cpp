/**
 *  @file   BoundLevel.cpp
 *  @author Ben Cortina
 *  @date   8/30/13.
 */

#include "BoundLevel.h"
#include <fstream>


bool BLevel::isWall(Coords loc)
{
    for (int i = 0; i < wTileCount; i++)
    {
        //This is where I would check if on
        //Check to see if this is the tile in that spot
        if(wTiles[i]->getLocation() == loc)
            return true;
    }
    return false;
}

bool BLevel::isExplosion(const Coords loc)
{
    for (int i = 0; i < wTileCount; i++)
    {
        //check if it is active and in that spot
        if(eTiles[i]->isOn() && eTiles[i]->getLocation() == loc)
            return true;
    }
    return false;
}