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
    on = true;
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
/*==================================      BExplosionTile      ===================================*/
BExplosionTile::BExplosionTile(const Color3B c, Coords position)
{
    color = c;
    loc = position;
    on = false;
    exploded = false;
    duration = 0;
    interval = 1;
    updateVisibility();
}

BExplosionTile* BExplosionTile::createWithFileColorLoc(const char* image, const Color3B c, const Coords position)
{
    BExplosionTile *pobBTile = new BExplosionTile(c, position);
    if (pobBTile && pobBTile->initWithFile(image))
    {
        if (!pobBTile->hasExploded())
            std::cout<<"faild"<<std::endl;
        pobBTile->autorelease();
        return pobBTile;
    }
    CC_SAFE_DELETE(pobBTile);
    return NULL;
}

void BExplosionTile::scheduleFirstExplode(float st)
{
    //schedule first explosion
    scheduleOnce(schedule_selector(BExplosionTile::firstExplode), st);
    //schedule first fade
    scheduleOnce(schedule_selector(BExplosionTile::firstFade), st+duration);
}

void BExplosionTile::firstExplode(float dt)
{
    explode(dt);
    schedule(schedule_selector(BExplosionTile::explode), interval);
}

void BExplosionTile::firstFade(float dt)
{
    fade(dt);
    schedule(schedule_selector(BExplosionTile::fade), interval);
}

void BExplosionTile::explode(float dt)
{
    exploded = true;
    setOn(true);
}

void BExplosionTile::fade(float dt)
{
    setOn(false);
}
