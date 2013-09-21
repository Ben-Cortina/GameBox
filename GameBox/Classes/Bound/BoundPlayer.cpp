/**
 *  @file   BoundPlayer.cpp
 *  @author Ben Cortina
 *  @date   9/2/13.
 */

#include "BoundPlayer.h"

BPlayer::BPlayer(const char* image, std::function<void(Object*)> winF)
{
    winFunc = winF;
    force.x = 0;
    force.y = 0;
    velocity.x = 0;
    velocity.y = 0;
    maxSpeed = 0;
    keyState.up = keyState.down = keyState.left = keyState.right = false;
    isDying = false;
    player = Sprite::create(image);
    playerColor = Color3B(100, 200, 125);
    addChild(player);
}

void BPlayer::setLevel(BLevel* newLevel)
{
    level = newLevel;
    maxSpeed = level->getTileSize() * 2;
    player->setScale((int)(level->getTileSize() / 10));
    spawn(0);
    scheduleUpdate();
    setKeyboardEnabled(true);
};

void BPlayer::spawn(float dt)
{
    isDying = false;
    player->setPosition( level->getStart() );
    player->setScale((int)(level->getTileSize()/10));
    player->setColor(playerColor);
    player->setOpacity(255);
}

void BPlayer::fall()
{
    std::cout << "fell" << std::endl;
    isDying = true;
    force.x = 0;
    force.y = 0;
    scheduleOnce( schedule_selector(BPlayer::spawn), 1.5f );
    //fade out
    player->runAction( ScaleTo::create(1.0f, 0.0001f) );
}

void BPlayer::die()
{
    std::cout << "died" << std::endl;
    isDying = true;
    player->setColor(Color3B(0,0,0));
    force.x = 0;
    force.y = 0;
    scheduleOnce( schedule_selector(BPlayer::spawn), 1.5f );
    //fade to black
    player->runAction( FadeOut::create(1.0f) );
    
}

void BPlayer::win()
{
    setKeyboardEnabled(false);
    force.x = 0;
    force.y = 0;
    velocity.x = 0;
    velocity.y = 0;
    maxSpeed = 0;
    keyState.up = keyState.down = keyState.left = keyState.right = false;
    winFunc(this);
}

void BPlayer::updateVelocity()
{
    if (force.x != 0 || force.y !=0)
    {
        const float normal = MAX(fabs(force.y), fabs(force.x));
        const float ny = force.y/normal;
        const float nx = force.x/normal;
        
        float direction = atanf(ny / nx);
        if (nx < 0) direction += M_PI;
        
        velocity.x = maxSpeed * cosf(direction);
        velocity.y = maxSpeed * sinf(direction);
        
        //damn floating point numbers *grumble grumble grumble*
        if (fabs(velocity.x) < 0.00001)
            velocity.x = 0;
        if (fabs(velocity.y) < 0.00001)
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
    if (level->checkExplosions(player->getBoundingBox()) && !isDying)
        die();
    
    handleCollisions(velocity.x * dt, velocity.y * dt);
    
    if (level->inExit(player->getBoundingBox()) && !isDying)
        win();
    
}

//this update is purely for percision. It allows me to update the player the instant the button press is received
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
    if ( (x == 0 && y == 0) || isDying)
        return;
    
    Rect bb = player->getBoundingBox();
    Rect bbMoved = bb;
    bbMoved.origin.x += x;
    bbMoved.origin.y += y;
    Rect tileBB;
    
    float new_x = x;
    float new_y = y;
    
    if( level->isWallCollision(bbMoved, tileBB) )
    {
        //compensate for the level position
        tileBB.origin = tileBB.origin - level->getPosition();
        
        //handle movement with wall collision
        
        //find where it would hit first
        Point newPos = player->getPosition();
        Point tileMid;
        tileMid.x = tileBB.getMidX();
        tileMid.y = tileBB.getMidY();
        
        //if it would hit on the left or right side
        if( fabs( player->getPosition().x - tileMid.x ) > fabs( player->getPosition().y - tileMid.y ))
        {
            //put player on the edge of tile
            newPos.x = ( x > 0 )? tileBB.getMinX() - bb.size.width/2 : tileBB.getMaxX() + (bb.size.width/2 + 0.1);
            
            // adjust x
            x -= player->getPosition().x - newPos.x;
                                
            //no x movement
            new_x = 0;
            // continue to either past the edge of the tile or the remaining y
            new_y = (( y < 0 )? -1 : 1) * MIN( fabs(y), fabs( player->getPosition().y -
                                ( ( y < 0 )? tileBB.getMinY() - bb.size.height/2 : tileBB.getMaxY() + bb.size.height/2 )));
        } else //hit on top or bottom
        {
            //put player on the edge of tile
            newPos.y = ( y > 0 )? tileBB.getMinY() - bb.size.height/2 : tileBB.getMaxY() + (bb.size.height/2 + 0.1);
            
            // adjust y
            y -= player->getPosition().y - newPos.y;
            
            // continue to either past the edge of the tile or the remaining x
            new_x = (( x < 0 )? -1 : 1) *  MIN( fabs(x), fabs( player->getPosition().x -
                                ( ( x < 0 )? tileBB.getMinX() - bb.size.width/2 : tileBB.getMaxX() + bb.size.width/2 )));
            //no y movement
            new_y = 0;
        }
        
        //adjust player position to be against the wall
        player->setPosition(newPos);
        
        //handle new mvement if there is any
        if (new_x != 0 || new_y != 0)
        {
            //run this again with adjusted movement
            handleCollisions(new_x, new_y);
            
            //check if it died after that move.
            if (isDying)
                return;
            
            //and again for any remaining movement
            float percent_remaining = 1 - ( (new_y != 0 && y != 0)? new_y / y : new_x / x );
            
            handleCollisions( new_x * percent_remaining, new_y * percent_remaining );
            
            //if it died, stop all movement.
            if (isDying)
                return;
        }
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
        std::cout << pos.x << " " << pos.y << std::endl;
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
    
    //the change as a result of the keypress
    int x = 0;
    int y = 0;
    
    switch(keyCode)
    {
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
    
    //do nothing if it is paused
    if (Director::getInstance()->isPaused())
        return;
    
    if( x != 0 )
        setForceX(x);
    if( y != 0)
        setForceY(y);
}

void BPlayer::keyReleased(int keyCode)
{
    
    switch(keyCode)
    {
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
    
    //do nothing if it is paused
    if (Director::getInstance()->isPaused())
        return;
    
    if( !keyState.left && !keyState.right )
        setForceX(0);
    if( !keyState.up && !keyState.down )
        setForceY(0);
}