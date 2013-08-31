//
//  BoundTiles.cpp
//  GameBox
//
//  Created by Blaeeap on 8/30/13.
//
//

#include "BoundTiles.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
/*=======================================      BTile      =======================================*/
BTile::BTile()
{
    BTile(Color3B(255,255,255),Color3B(0,0,0));
}

BTile::BTile(const Color3B onC, const Color3B offC): onColor(onC), offColor(offC)
{
    on = false;
    updateColor();
}

BTile* BTile::createWithFileColors(const char* image, const Color3B onC, const Color3B offC)
{
    BTile *pobBTile = new BTile(onC, offC);
    if (pobBTile && pobBTile->initWithFile(image))
    {
        pobBTile->autorelease();
        return pobBTile;
    }
    CC_SAFE_DELETE(pobBTile);
    return NULL;
}