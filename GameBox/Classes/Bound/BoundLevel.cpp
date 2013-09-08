/**
 *  @file   BoundLevel.cpp
 *  @author Ben Cortina
 *  @date   8/30/13.
 */

#include "BoundLevel.h"
//for file parsing
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

bool RectIntersectsRect(const Rect r1, const Rect r2)
{
    //if they intersect
    if ( (r1.getMinX() <= r2.getMaxX() &&
          r2.getMinX() <= r1.getMaxX() )  &&
        (r1.getMinY() <= r2.getMaxY() &&
         r2.getMinY() <= r1.getMaxY() )  )
        return true;
    return false;
}

BLevel::BLevel(const char* file)
{
    parseFile(file);
}

void BLevel::parseFile(const char* file)
{
    std::ifstream bdlfile;
    
    bdlfile.open(file);
    
    if(!bdlfile.is_open())
    {
        //ERROR opening file
        return;
    }
    
    char floorChar;
    char firstChar;
    char wallChar;
    Color3B floorColor1;
    Color3B floorColor2;
    Color3B wallColor;
    width = 0;
    height = 0;
    int intIn;
    
    std::string line;
    std::streampos prevLine;
    
    ////////////////////////////////////////////////////////
    //Name line
    do {
        if (bdlfile.eof()) {
            //ERROR: wrong format, no content
            return;
        }
        
        //get line
        prevLine = bdlfile.tellg();
        bdlfile >> firstChar;
        bdlfile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        //while its a comment line
    } while (firstChar == '#');
    
    //do nothing, we dont need the name
    
    ////////////////////////////////////////////////////////
    //Floor line
    do {
        if (bdlfile.eof()) {
            //ERROR: wrong format, no content
            return;
        }
        
        //get line
        prevLine = bdlfile.tellg();
        bdlfile >> firstChar;
        bdlfile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
    //while its a comment line
    } while (firstChar == '#');
    
    //back up one line
    bdlfile.seekg(prevLine);
    bdlfile >> floorChar;
    
    //------------------floor color 1
    if (!(bdlfile >> intIn) || intIn > 255 || intIn < 0)
    {
        //ERROR reading floorColor1r
        return;
    }
    floorColor1.r = intIn;
    
    if (!(bdlfile >> intIn) || intIn > 255 || intIn < 0)
    {
        //ERROR reading floorColor1g
        return;
    }
    floorColor1.g = intIn;
    
    if (!(bdlfile >> intIn) || intIn > 255 || intIn < 0)
    {
        //ERROR reading floorColor1b
        return;
    }
    floorColor1.b = intIn;
    
    //-------------------floor color 2
    if (!(bdlfile >> intIn) || intIn > 255 || intIn < 0)
    {
        //ERROR reading floorColor2r
        return;
    }
    floorColor2.r = intIn;
    
    if (!(bdlfile >> intIn) || intIn > 255 || intIn < 0)
    {
        //ERROR reading floorColor2g
        return;
    }
    floorColor2.g = intIn;
    
    if (!(bdlfile >> intIn) || intIn > 255 || intIn < 0)
    {
        //ERROR reading floorColor2b
        return;
    }
    floorColor2.b = intIn;
    
    //next line
    bdlfile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    ////////////////////////////////////////////////////////
    //Wall line
    do {
        if (bdlfile.eof()) {
            //ERROR: wrong format, no wall
            return;
        }
        
        //get line
        prevLine = bdlfile.tellg();
        bdlfile >> firstChar;
        bdlfile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        //while its a comment line
    } while (firstChar == '#');

    //back up one line
    bdlfile.seekg(prevLine);
    bdlfile >> wallChar;
    
    //------------------wall color 
    if (!(bdlfile >> intIn) || intIn > 255 || intIn < 0)
    {
        //ERROR reading wallColorr
        return;
    }
    wallColor.r = intIn;
    
    if (!(bdlfile >> intIn) || intIn > 255 || intIn < 0)
    {
        //ERROR reading wallColorg
        return;
    }
    wallColor.g = intIn;
    
    if (!(bdlfile >> intIn) || intIn > 255 || intIn < 0)
    {
        //ERROR reading wallColorb
        return;
    }
    wallColor.b = intIn;
    
    ////////////////////////////////////////////////////////
    //Level
    do {
        if (bdlfile.eof()) {
            //ERROR: no level
            return;
        }
        
        //get line
        prevLine = bdlfile.tellg();
        bdlfile >> firstChar;
        bdlfile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        //while its a comment line
    } while (firstChar == '#');
    
    if (firstChar != '-') {
        //ERROR level not found (the line before and after the level must start with '-'
        return;
    }
    
    //this is the beggining of the level
    prevLine = bdlfile.tellg();
    std::vector<std::string> levelLines;
    //lets find out how big the level is
    do {
        if (bdlfile.eof()) {
            //ERROR: no level end, the line after the level end must start with '-'
            return;
        }
        //get line
        getline(bdlfile, line);
        line = line.substr(0,line.find_first_of('#'));
        width = MAX(width, line.size());
        levelLines.push_back(line);
        height++;
        
    //while its not the end of the level
    } while (line[0] != '-');
    
    //ignore the '-' line
    height--;
    levelLines.pop_back();
    
    //now that we know how many tiles we will create them
    
    bdlfile.close();
}

bool BLevel::isWall(const Coords loc)
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
    for (int i = 0; i < eTileCount; i++)
    {
        //check if it is active and in that spot
        if(eTiles[i]->isOn() && eTiles[i]->getLocation() == loc)
            return true;
    }
    return false;
}

bool BLevel::isOnFloor(Rect bb)
{
    //adjust for BLevel position
    bb.origin = bb.origin + getPosition();
    
    //check every one. Because Y.O.L.O.
    for ( int i = 0; i < fTileCount; i++)
        //would check On here
        if (RectIntersectsRect(bb, fTiles[i]->getBoundingBox()))
            return true;
    return false;
}

bool BLevel::isWallCollision(Rect bb, Rect& tileBB)
{
    //adjust for BLevel position
    bb.origin = bb.origin + getPosition();
    
    //check every one. Because Y.O.L.O.
    for ( int i = 0; i < wTileCount; i++)
    {
        tileBB = wTiles[i]->getBoundingBox();
        //would check On here
        if (RectIntersectsRect(bb, tileBB))
            return true;
    }
    return false;
    
//
//    Coords loc = getTile(bb.origin);
//    //check all nine surrounding tiles
//    //0,0
//    tileBB = getTileBB(loc);
//    if ( RectIntersectsRect(bb, tileBB) && isWall(loc) )
//        return true;
//    
//    //+1,0
//    loc.x++;
//    tileBB = getTileBB(loc);
//    if ( RectIntersectsRect(bb, tileBB) && isWall(loc) )
//        return true;
//    
//    //+1,+1
//    loc.y++;
//    tileBB = getTileBB(loc);
//    if ( RectIntersectsRect(bb, tileBB) && isWall(loc) )
//        return true;
//    
//    //0,+1
//    loc.x--;
//    tileBB = getTileBB(loc);
//    if ( RectIntersectsRect(bb, tileBB) && isWall(loc) )
//        return true;
//    
//    //-1,+1
//    loc.x--;
//    tileBB = getTileBB(loc);
//    if ( RectIntersectsRect(bb, tileBB) && isWall(loc) )
//        return true;
//    
//    //-1,0
//    loc.y--;
//    tileBB = getTileBB(loc);
//    if ( RectIntersectsRect(bb, tileBB) && isWall(loc) )
//        return true;
//    
//    //-1,-1
//    loc.y--;
//    tileBB = getTileBB(loc);
//    if ( RectIntersectsRect(bb, tileBB) && isWall(loc) )
//        return true;
//    
//    //0,-1
//    loc.x++;
//    tileBB = getTileBB(loc);
//    if ( RectIntersectsRect(bb, tileBB) && isWall(loc) )
//        return true;
//    
//    //+1,-1
//    loc.x++;
//    tileBB = getTileBB(loc);
//    if ( RectIntersectsRect(bb, tileBB) && isWall(loc) )
//        return true;
//    
//    return false;
}

bool BLevel::isExplosionCollision(Rect bb)
{
    //adjust for BLevel position
    bb.origin = bb.origin + getPosition();
    
    //check every one. Because Y.O.L.O.
    for ( int i = 0; i < eTileCount; i++)
        if (eTiles[i]->isOn() && RectIntersectsRect(bb, eTiles[i]->getBoundingBox()))
            return true;
    return false;
}