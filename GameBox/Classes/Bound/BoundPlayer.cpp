/**
 *  @file   BoundPlayer.cpp
 *  @author Ben Cortina
 *  @date   9/2/13.
 */

#include "BoundPlayer.h"


BPlayer::BPlayer()
{
    maxSpeed = 0;
    keyState.up = keyState.down = keyState.left = keyState.right = false;
    isDying = false;
    playerColor = Color3B(100, 200, 125);
}

BPlayer::BPlayer(const char* image)
{
    maxSpeed = 0;
    keyState.up = keyState.down = keyState.left = keyState.right = false;
    isDying = false;
    player = Sprite::create(image);
    playerColor = Color3B(100, 200, 125);
    addChild(player);
    setKeyboardEnabled(true);
}

void BPlayer::setLevel(BLevel* newLevel)
{
    level = newLevel;
    maxSpeed = level->getTileSize();
    player->setScale(level->getTileSize()/10);
    spawn(0);
    scheduleUpdate();
};

void BPlayer::spawn(float dt)
{
    isDying = false;
    player->setPosition( level->getStart() );
    player->setColor(playerColor);
    player->setOpacity(255);
}

void BPlayer::fall()
{
    isDying = true;
    force.x = 0;
    force.y = 0;
    scheduleOnce( schedule_selector(BPlayer::spawn), 1.0f );
    //fade out
    player->runAction( ScaleTo::create(1.0f, 0.0001f) );
}

void BPlayer::die()
{
    isDying = true;
    force.x = 0;
    force.y = 0;
    scheduleOnce( schedule_selector(BPlayer::spawn), 1.0f );
    //fade to black
    runAction( TintTo::create(1.0f,0,0,0) );
    
}

void BPlayer::updateVelocity()
{
    if (force.x != 0 || force.y !=0)
    {
        const float normal = MAX(abs(force.y), abs(force.x));
        const float ny = force.y/normal;
        const float nx = force.x/normal;
        
        float direction = atanf(ny / nx);
        if (nx < 0) direction += M_PI;
        
        velocity.x = maxSpeed * cosf(direction);
        velocity.y = maxSpeed * sinf(direction);
        
        //damn floating point numbers *grumble grumble grumble*
        if (abs(velocity.x) < 0.00001)
            velocity.x = 0;
        if (abs(velocity.y) < 0.00001)
            velocity.y = 0;
    } else
    {
        velocity.x = 0;
        velocity.y = 0;
    }
    update();
}

void BPlayer::update(float dt)
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
    
    //check explosions incase one went off on player
    if (level->checkExplosions(getBoundingBox()) && !isDying)
        die();
    
    handleCollisions(velocity.x * dt, velocity.y * dt);
    
}

void BPlayer::update()
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

void BPlayer::handleCollisions(float x, float y)
{
    //if no movement, check nothing
    if ( x == 0 && y == 0 )
        return;
    
    Rect bb = player->getBoundingBox();
    Rect bbMoved = bb;
    bbMoved.origin.x += x;
    bbMoved.origin.x += y;
    Rect tileBB;
    
    float new_x = x;
    float new_y = y;
    
    if( level->isWallCollision(bbMoved, tileBB) )
    {
        //compensate for the level position
        tileBB.origin = tileBB.origin - level->getPosition();
        
        //handle movement with wall collision
        
        //find where it would hit first
        Point playerMid = getPosition();
        Point tileMid;
        tileMid.x = tileBB.getMidX();
        tileMid.y = tileBB.getMidY();
        
        //if it hit on the left or right side
        if( abs( playerMid.x - tileMid.x ) / ( tileBB.size.width + bb.size.width ) )
        {
            //no x movement
            new_x = 0;
            // continue to either past the edge of the tile or the remaining y
            new_y = MIN( abs(y),
                         abs( playerMid.y - ( (y < 0 )? tileBB.getMinY() - bb.size.height/2 :
                                                        tileBB.getMaxY() + bb.size.height/2 )));
        } else //hit on top or bottom
        {
            // continue to either past the edge of the tile or the remaining x
            new_x = MIN( abs(x),
                         abs( playerMid.x - ( (y < 0 )? tileBB.getMinX() - bb.size.width/2 :
                                                        tileBB.getMaxX() + bb.size.width/2 )));
            //no y movement
            new_y = 0;
        }
        
        //run this again with adjusted movement
        handleCollisions(new_x, new_y);
        if (isDying)
            return;
        //and again for any remaining movement
        float percent_remaining = 1 - ( (new_y == 0 && y != 0)? new_y / y : new_x / x );
        handleCollisions( new_x * percent_remaining, new_y * percent_remaining);
        if (isDying)
            return;
        
        //if it has reached here all movement has occured
        x = 0;
        y = 0;
    }
    //now we move it and check death collisions
    Point pos = player->getPosition();
    pos.x += x;
    pos.y += y;
    player->setPosition(pos);
    bbMoved = player->getBoundingBox();
    if ( !level->isOnFloor(bbMoved) )
    {
        //fall
        fall();
        
    }
    if ( level->isExplosionCollision(bbMoved) )
    {
        //die
        die();
    }
}

void BPlayer::keyPressed(int keyCode)
{
    //do nothing if it is paused
    if (Director::getInstance()->isPaused())
        return;
    
    //the change as a result of the keypress
    int x = 0;
    int y = 0;
    
    switch(keyCode)
    {
            // exitLoc
        case 53: //Esc
            //openMenu();
            break;
            
            // Player Up
        case 126: // Up Arrow
        case 13:  // W
            if (!keyState.up)
            {
                keyState.up = true;
                y = 1;
            }
            break;
            
            // Player Left
        case 123: // Left Arrow
        case 0:  // A
            if (!keyState.left)
            {
                keyState.left = true;
                x = -1;
            }
            break;
            
            // Player Down
        case 125: // Down Arrow
        case 1: // S
            if (!keyState.down)
            {
                keyState.down = true;
                y = -1;
            }
            break;
            
            // Player Right
        case 124: // Right Arrow
        case 2: // D
            if (!keyState.right)
            {
                keyState.right = true;
                x = 1;
            }
            break;
            
        default:
            break;
    }
    
    if( x != 0 )
        setForceX(x);
    if( y != 0)
        setForceY(y);
}

void BPlayer::keyReleased(int keyCode)
{
    //do nothing if it is paused
    if (Director::getInstance()->isPaused())
        return;
    
    switch(keyCode)
    {
            // exitLoc
        case 53: //Esc
            //openMenu();
            break;
            
            // Player Up
        case 126: // Up Arrow
        case 13:  // W
            keyState.up = false;
            break;
            
            // Player Left
        case 123: // Left Arrow
        case 0:  // A
            keyState.left = false;
            break;
            
            // Player Down
        case 125: // Down Arrow
        case 1: // S
            keyState.down = false;
            break;
            
            // Player Right
        case 124: // Right Arrow
        case 2: // D
            keyState.right = false;
            break;
            
        default:
            break;
    }
    if( !keyState.left && !keyState.right )
        setForceX(0);
    if( !keyState.up && !keyState.down )
        setForceY(0);
}