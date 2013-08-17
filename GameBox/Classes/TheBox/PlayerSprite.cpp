//
//  Player.cpp
//  GameBox
//
//  Created by Ben Cortina on 8/6/13.
//
//

#include "PlayerSprite.h"

PlayerSprite* PlayerSprite::create(const char *pszFileName)
{
    PlayerSprite *pobSprite = new PlayerSprite();
    if (pobSprite && pobSprite->initWithFile(pszFileName))
    {
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}

bool PlayerSprite::initWithFile(const char *pszFilename)
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

/* Currently Unused
void PlayerSprite::applyForce(float x, float y, float dt)
{
    //V=F*t/m
    _vel.x += x*dt/_weight;
    _vel.y += y*dt/_weight;
}*/

void PlayerSprite::applyVelocityX(float vel_x, float dt)
{
    Point currentPos = this->_position;
    currentPos.x += _vel.x * dt;
    this->_position = currentPos;
}

void PlayerSprite::applyVelocityY(float vel_y, float dt)
{
    Point currentPos = this->_position;
    currentPos.y += _vel.y * dt;
    this->_position = currentPos;
}

/* Currently Unused
void PlayerSprite::updatePos(float dt)
{
    Point currentPos = this->_position;
    currentPos.x += _vel.x * dt;
    currentPos.y += _vel.y * dt;
    this->_position = currentPos;
}*/