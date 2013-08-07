//
//  Player.cpp
//  GameBox
//
//  Created by Ben Cortina on 8/6/13.
//
//

#include "Player.h"

Player* Player::create(const char *pszFileName)
{
    Player *pobSprite = new Player();
    if (pobSprite && pobSprite->initWithFile(pszFileName))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

bool Player::initWithFile(const char *pszFilename)
{
    CCASSERT(pszFilename != NULL, "Invalid filename for sprite");
    
    Texture2D *pTexture = TextureCache::getInstance()->addImage(pszFilename);
    if (pTexture)
    {
        Rect rect = Rect::ZERO;
        rect.size = pTexture->getContentSize();
        return initWithTexture(pTexture, rect);
    }
    
    // don't release here.
    // when load texture failed, it's better to get a "transparent" sprite then a crashed program
    // this->release();
    return false;
}

void Player::applyForce(float x, float y, float dt)
{
    //V=F*t/m
    _vel.x += x*dt/_weight;
    _vel.y += y*dt/_weight;
}