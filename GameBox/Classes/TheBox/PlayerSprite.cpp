//
//  Player.cpp
//  GameBox
//
//  Created by Ben Cortina on 8/6/13.
//
//

#include "PlayerSprite.h"

float getTime()
{
    struct timeval now;
    float time;
    
    if (gettimeofday(&now, NULL) != 0)
    {
        CCLOG("error in gettimeofday");
        time = 0;
    }
    else
    {
        time = (now.tv_sec) + (now.tv_usec) / 1000000.0f;
        time = MAX(0, time);
    }
    return time;
};

PlayerSprite::PlayerSprite()
{
    dtCalculated = 0;
    if (gettimeofday(&lastUpdate, NULL) != 0)
    {
        CCLOG("error in gettimeofday");
        lastUpdate.tv_sec = 0;
        lastUpdate.tv_usec = 0;
    }
    
    _maxSpeed = 0;
    _vel.x = _vel.y = 0;
    _force.x = _force.y = 0;
}

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
        scheduleUpdate();
        return initWithTexture(pTexture, rect);
    }
    
    return false;
}

/* Currently Unused
void PlayerSprite::applyForce(float x, float y, float dt)
{
    //V=F*t/m
    _vel.x += x*dt/_weight;
    _vel.y += y*dt/_weight;
}*/

void PlayerSprite::updateVelocity()
{
    if (_force.x != 0 || _force.y !=0)
    {
        float normal = MAX(abs(_force.y), abs(_force.x));
        float ny = _force.y/normal;
        float nx = _force.x/normal;
        float direction = atanf(ny / nx);
        if (nx < 0) direction += M_PI;
        _vel.x = _maxSpeed * cosf(direction);
        _vel.y = _maxSpeed * sinf(direction);
    } else
    {
        _vel.x = 0;
        _vel.y = 0;
    }
}

void PlayerSprite::update(float dt)
{
    if (gettimeofday(&lastUpdate, NULL) != 0)
    {
        CCLOG("error in gettimeofday");
        lastUpdate.tv_sec = 0;
        lastUpdate.tv_usec = 0;
    }
    //adjust for how much has already been calcualted
    dt -= dtCalculated;
    dtCalculated = 0;
    

    Point currentPos = _position;
    currentPos.x += _vel.x * dt;
    currentPos.y += _vel.y * dt;
    setPosition(currentPos);
    
}


void PlayerSprite::update()
{
    //get current time
    timeval now;
    if (gettimeofday(&now, NULL) != 0)
    {
        CCLOG("error in gettimeofday");
        return;
    }
    //calcualte the time difference from now and the last update
    float dt = (now.tv_sec - lastUpdate.tv_sec) + (now.tv_usec - lastUpdate.tv_usec) / 1000000.0f;
    
    //incase this has been called twice before one update has occured
    float dc = dt;
    if (dtCalculated != 0)
        dc = dtCalculated + dt;
    dtCalculated = 0;
    
    update(dt);
    
    //record how much time has already been accounted for
    dtCalculated = dc;
}