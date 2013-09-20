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
    if ( (r1.getMinX() < r2.getMaxX() &&
          r2.getMinX() < r1.getMaxX() )  &&
         (r1.getMinY() < r2.getMaxY() &&
          r2.getMinY() < r1.getMaxY() )  )
        return true;
    return false;
}

BLevel::BLevel(const char* filepath)
{
    init();
    std::string pathKey = filepath;
    pathKey = FileUtils::getInstance()->fullPathForFilename(pathKey.c_str());
    
    valid = parseFile(pathKey.c_str());
    
    if (valid)
        setupTiles();
}

void BLevel::setupTiles()
{
    //I could either center the layer or center the tiles... I prefer centering the tiles
    Size winSize = Director::getInstance()->getVisibleSize();
    Size margin = Size((winSize.width - tileSize * width)/ 2,
                       (winSize.height - tileSize * height)/ 2);
    
    for (int i = 0; i < fTileCount; i++)
    {
        fTiles[i]->setScale(tileSize);
        fTiles[i]->setPosition(Point(tileSize*fTiles[i]->getLocation().x + tileSize / 2 + margin.width,
                                     tileSize*fTiles[i]->getLocation().y + tileSize / 2 + margin.height));
    }
    for (int i = 0; i < wTileCount; i++)
    {
        wTiles[i]->setScale(tileSize);
        wTiles[i]->setPosition(Point(tileSize*wTiles[i]->getLocation().x + tileSize / 2 + margin.width,
                                     tileSize*wTiles[i]->getLocation().y + tileSize / 2 + margin.height));
    }
    for (int i = 0; i < eTileCount; i++)
    {
        eTiles[i]->setScale(tileSize);
        eTiles[i]->setPosition(Point(tileSize*eTiles[i]->getLocation().x + tileSize / 2 + margin.width,
                                     tileSize*eTiles[i]->getLocation().y + tileSize / 2 + margin.height));
    }
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
    bb.origin = bb.origin - getPosition();
    
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
    bb.origin = bb.origin - getPosition();
    
    //check every one. Because Y.O.L.O.
    for ( int i = 0; i < wTileCount; i++)
    {
        tileBB = wTiles[i]->getBoundingBox();
        //would check On here
        if (RectIntersectsRect(bb, tileBB))
            return true;
    }
    return false;
}

//I may replace this with NotificationCenter eventually...
bool BLevel::checkExplosions(Rect bb)
{
    bool hit = false;
    
    //I have to check each of them to ensure I reset "bool exploded"
    for ( int i = 0; i < eTileCount; i++)
    {
        //if it has exploded and the player was touching it...
        if (eTiles[i]->hasExploded() && RectIntersectsRect(bb, eTiles[i]->getBoundingBox()))
            hit = true;
    }
    
    return hit;
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

Rect BLevel::getTileBB( Coords loc )
{
    Size winSize = Director::getInstance()->getVisibleSize();
    Size margin = Size((winSize.width - tileSize * width)/ 2,
                       (winSize.height - tileSize * height)/ 2);
    return Rect(tileSize * loc.x + margin.width,
                tileSize * loc.y + margin.height,
                tileSize,
                tileSize);
};

////////////////////////////////////////////////////////////////////////////////////////////////
/*-------------  Ugly File Parsing Method. Abandon hope all ye who enter here  ---------------*/
// This is only in place until I get around to making a level editor so I can write a class/struct
// to a file and extract it easily

bool skipComments(std::ifstream& file)
{
    std::streampos prevLine;
    char firstChar;
    do {
        //get line
        prevLine = file.tellg();
        file >> firstChar;
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (file.eof())
            return false;
        //while its a comment line
    } while (firstChar == '#');
    
    //back one line
    file.seekg(prevLine);
    return true;
}

bool BLevel::parseFile(const char* file)
{
    std::ifstream bdlfile;
    
    std::cout << "Reading level from " << file << std::endl;
    
    bdlfile.open(file);
    
    if(!bdlfile)
    {
        std::cout << "ERROR opening file" <<std::endl;
        return false;
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
    std::streampos prevPoint;
    
    ////////////////////////////////////////////////////////
    //Floor line
    if (!(skipComments(bdlfile)))
    {
        std::cout << "ERROR eof before floor" <<std::endl;
        bdlfile.close();
        return false;
    }
    
    bdlfile >> floorChar;
    
    //------------------floor color 1
    if (!(bdlfile >> intIn) || intIn > 255 || intIn < 0)
    {
        std::cout << floorChar << " " << "ERROR reading floorColor1r" <<std::endl;
        bdlfile.close();
        return false;
    }
    floorColor1.r = intIn;
    
    if (!(bdlfile >> intIn) || intIn > 255 || intIn < 0)
    {
        std::cout << "ERROR reading floorColor1g" <<std::endl;
        bdlfile.close();
        return false;
    }
    floorColor1.g = intIn;
    
    if (!(bdlfile >> intIn) || intIn > 255 || intIn < 0)
    {
        std::cout << "ERROR reading floorColor1b" <<std::endl;
        bdlfile.close();
        return false;
    }
    floorColor1.b = intIn;
    
    //-------------------floor color 2
    if (!(bdlfile >> intIn) || intIn > 255 || intIn < 0)
    {
        std::cout << "ERROR reading floorColor2r" <<std::endl;
        bdlfile.close();
        return false;
    }
    floorColor2.r = intIn;
    
    if (!(bdlfile >> intIn) || intIn > 255 || intIn < 0)
    {
        std::cout << "ERROR reading floorColor2g" <<std::endl;
        bdlfile.close();
        return false;
    }
    floorColor2.g = intIn;
    
    if (!(bdlfile >> intIn) || intIn > 255 || intIn < 0)
    {
        std::cout << "ERROR reading floorColor2b" <<std::endl;
        bdlfile.close();
        return false;
    }
    floorColor2.b = intIn;
    
    //next line
    bdlfile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    ////////////////////////////////////////////////////////
    //Wall line
    if (!(skipComments(bdlfile)))
    {
        std::cout << "ERROR eof before wall" <<std::endl;
        bdlfile.close();
        return false;
    }
    
    bdlfile >> wallChar;
    
    //------------------wall color
    if (!(bdlfile >> intIn) || intIn > 255 || intIn < 0)
    {
        std::cout << "ERROR reading wallColorr" <<std::endl;
        bdlfile.close();
        return false;
    }
    wallColor.r = intIn;
    
    if (!(bdlfile >> intIn) || intIn > 255 || intIn < 0)
    {
        std::cout << "ERROR reading wallColorg" <<std::endl;
        bdlfile.close();
        return false;
    }
    wallColor.g = intIn;
    
    if (!(bdlfile >> intIn) || intIn > 255 || intIn < 0)
    {
        std::cout << "ERROR reading wallColorb" <<std::endl;
        bdlfile.close();
        return false;
    }
    wallColor.b = intIn;
    
    //next line
    bdlfile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    
    if (floorChar == 'E' || wallChar == 'E')
    {
        std::cout << "ERROR E is reserved for End" <<std::endl;
        bdlfile.close();
        return false;
    }
    
    if (floorChar == 'S' || wallChar == 'S')
    {
        std::cout << "ERROR S is reserved for Start" <<std::endl;
        bdlfile.close();
        return false;
    }
    
    ////////////////////////////////////////////////////////
    //Level
    if (!(skipComments(bdlfile)))
    {
        std::cout << "ERROR eof before level" <<std::endl;
        bdlfile.close();
        return false;
    }
    
    bdlfile >> firstChar;
    
    if (firstChar != '-') {
        std::cout << "ERROR level not found (the line before and after the level must start with '-'" <<std::endl;
        bdlfile.close();
        return false;
    }
    
    //next line
    bdlfile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    std::vector<std::string> levelLines;
    
    //lets find out how big the level is
    do {
        if (!(skipComments(bdlfile)))
        {
            std::cout << "ERROR eof before end of level (The line after the level must start with '-')" <<std::endl;
            bdlfile.close();
            return false;
        }
        //get line
        getline(bdlfile, line);
        line = line.substr(0,line.find_first_of('#'));
        width = MAX(width, line.size());
        levelLines.push_back(line);
        
        std::cout << line <<std::endl;
        height++;
        
        //while its not the end of the level
    } while (line[0] != '-');
    
    //ignore the '-' line
    height--;
    levelLines.pop_back();
    
    //set tileSize
    Size winSize = Director::getInstance()->getVisibleSize();
    tileSize = MIN(winSize.height / (float)height, winSize.width / (float)width);
    
    fTileCount = 0;
    wTileCount = 0;
    endCount = 0;
    Coords startloc = Coords(-1,-1);
    
    //now that we know how many tiles, we will count them
    for (int i = 0; i < levelLines.size(); i++)
    {
        for (int j = 0; j < levelLines[i].size(); j++)
        {
            if (levelLines[i][j] == floorChar)
                fTileCount++;
            if (levelLines[i][j] == wallChar)
                wTileCount++;
            if (levelLines[i][j] == 'E')
            {
                endCount++;
                fTileCount++;
            }
            if (levelLines[i][j] == 'S')
            {
                if (startloc == Coords(-1,-1))
                {
                    startloc = Coords(j, levelLines.size() - i - 1);
                } else
                {
                    std::cout << "ERROR multiple starts declared" <<std::endl;
                    bdlfile.close();
                    return false;
                }
                fTileCount++;
            }
        }
    }
    if (fTileCount == 0)
    {
        std::cout << "ERROR no floor tiles" <<std::endl;
        bdlfile.close();
        return false;
    }
    if (endCount == 0)
    {
        std::cout << "ERROR no end tile" <<std::endl;
        bdlfile.close();
        return false;
    }
    if (startloc == Coords(-1,-1))
    {
        std::cout << "ERROR no start tile" <<std::endl;
        bdlfile.close();
        return false;
    }
    //create tile arrays
    wTiles = new BWallTile*[wTileCount];
    fTiles = new BFloorTile*[fTileCount];
    
    //get the start point
    Rect startbb = getTileBB(startloc);
    start.x = startbb.getMidX();
    start.y = startbb.getMidY();
    
    
    //create the tiles
    int wItt = 0;
    int fItt = 0;
    for (int i = 0; i < levelLines.size(); i++)
    {
        for (int j = 0; j < levelLines[i].size(); j++)
        {
            if (levelLines[i][j] == floorChar || levelLines[i][j] == 'E' || levelLines[i][j] == 'S')
            {
                if ( levelLines[i][j] == 'E')
                    fTiles[fItt] = BFloorTile::createWithFileColorLoc("Pixel.png",
                                                                      Color3B(255, 250, 210),
                                                                      Coords(j, levelLines.size() - i - 1));
                else
                    fTiles[fItt] = BFloorTile::createWithFileColorLoc("Pixel.png",
                                                                      (((i+j)%2 == 0) ? floorColor1 : floorColor2),
                                                                      Coords(j, levelLines.size() - i - 1));
                addChild(fTiles[fItt]);
                fItt++;
            }
            if (levelLines[i][j] == wallChar)
            {
                wTiles[wItt] = BWallTile::createWithFileColorLoc("Pixel.png",
                                                                 wallColor,
                                                                 Coords(j,levelLines.size() - i - 1));
                addChild(wTiles[wItt]);
                wItt++;
            }
        }
    }
    
    
    ////////////////////////////////////////////////////////
    //Explosions
    int ex_x;
    int ex_y;
    Color3B ex_color;
    bool outside = true;
    
    eTileCount = 0;
    //beggining of the explosions
    std::streampos eStart = bdlfile.tellg();
    
    //count how many explosions there will be
    while (!bdlfile.eof()) {
        
        while (outside) {
            if (!(skipComments(bdlfile)))
                break;
            
            prevPoint = bdlfile.tellg();
            bdlfile >> firstChar;
            
            if (firstChar == '{')
            {
                outside = false;
                break;
            }
            
            //back up one line
            bdlfile.seekg(prevPoint);
            
            //get coords
            if ( !(bdlfile >> ex_x) || !(bdlfile >> ex_y))
            {
                std::cout << "ERROR reading explosion coords;" <<std::endl;
                bdlfile.close();
                return false;
            } else
            {
                eTileCount++;
            }
        }
        
        while (!outside) {
            if (bdlfile.eof())
            {
                std::cout << "ERROR explosion definition does not end." <<std::endl;
                bdlfile.close();
                return false;
            }
            
            bdlfile >> firstChar;
            
            
            if (firstChar == '#')
                bdlfile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            if (firstChar == '}')
            {
                outside = true;
            }
        }
    }
    
    //return to start of explosions
    bdlfile.clear();
    bdlfile.seekg(eStart);
    
    //create explosions
    eTiles = new BExplosionTile*[eTileCount];
    std::vector<Coords> eCoords;
    int eItt = 0;
    float eFirst;
    float eInterval;
    float eDuration;
    Color3B eColor;
    
    //populate the explosions
    while (!bdlfile.eof())
    {

        while (outside) {
            if (!(skipComments(bdlfile)))
                break;
            
            prevPoint = bdlfile.tellg();
            bdlfile >> firstChar;
            
            if (firstChar == '{')
            {
                outside = false;
                break;
            }
            
            //back up one line
            bdlfile.seekg(prevPoint);
            
            //get coords
            bdlfile >> ex_x;
            bdlfile >> ex_y;
            eCoords.push_back(Coords(ex_x-1, ex_y-1));
            
        }
        
        while (!outside && !bdlfile.eof())
        {
            //get first explosion time
            skipComments(bdlfile);
            if ( !(bdlfile >> eFirst) )
            {
                std::cout << "ERROR could not read first explosion time" <<std::endl;
                bdlfile.close();
                return false;
            }
            
            //get explosion interval
            skipComments(bdlfile);
            if ( !(bdlfile >> eInterval) )
            {
                std::cout << "ERROR could not read explosion interval" <<std::endl;
                bdlfile.close();
                return false;
            }
            
            //get explosion duration
            skipComments(bdlfile);
            if ( !(bdlfile >> eDuration) )
            {
                std::cout << "ERROR could not read explosion duration" <<std::endl;
                bdlfile.close();
                return false;
            }
            
            //get color
            skipComments(bdlfile);
            if (!(bdlfile >> intIn) || intIn > 255 || intIn < 0)
            {
                std::cout << "ERROR reading explosion color r" <<std::endl;
                bdlfile.close();
                return false;
            }
            eColor.r = intIn;
            
            if (!(bdlfile >> intIn) || intIn > 255 || intIn < 0)
            {
                std::cout << "ERROR reading explosion color g" <<std::endl;
                bdlfile.close();
                return false;
            }
            eColor.g = intIn;
            
            if (!(bdlfile >> intIn) || intIn > 255 || intIn < 0)
            {
                std::cout << "ERROR reading explosion color b" <<std::endl;
                bdlfile.close();
                return false;
            }
            eColor.b = intIn;
            
            // next should be the end of the explosion definition
            skipComments(bdlfile);
            bdlfile >> firstChar;
            
            if (firstChar == '}')
            {
                //nextline
                bdlfile.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                outside = true;
            } else
            {
                std::cout << "ERROR Invalid explosion definition" <<std::endl;
                bdlfile.close();
                return false;
            }
        }
        
        if (eFirst < 0 || eInterval < 0 || eDuration < 0) {
            std::cout << "ERROR negative values are not allowed in the exlosion declaration" <<std::endl;
            bdlfile.close();
            return false;
        }

        for (int i = 0; i < eCoords.size(); i++)
        {
            eTiles[eItt] = BExplosionTile::createWithFileColorLoc("Pixel.png",
                                                                  eColor,
                                                                  eCoords[i]);
            eTiles[eItt]->setInterval(eInterval);
            eTiles[eItt]->setDuration(eDuration);
            eTiles[eItt]->scheduleFirstExplode(eFirst);
            addChild(eTiles[eItt]);
            eItt++;
        }
        
        
        eCoords.clear();
    }
    
    std::cout<< "we did it, we made it to the eof without any errors! Well done everybody. " <<std::endl;
    
    bdlfile.close();
    return true;
}