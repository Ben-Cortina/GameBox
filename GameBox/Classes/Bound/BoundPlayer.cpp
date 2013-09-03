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
}

BPlayer::BPlayer(const char* image)
{
    BPlayer();
    player = Sprite::create(image);
    
    scheduleUpdate();
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
    
    
    Point currentPos = _position;
    
    Rect bb = getBoundingBox();
    
    

    //handleCollisions(velocity.x * dt, velocity.y * dt);
    
    setPosition(currentPos);
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

void BPlayer::handleMovement(const float x, const float y)
{
//    // if there is no movement no check needs to occur
//    if (x == 0 && y ==0)
//        return;
//    
//    //get Bounding box for collison detection
//    Rect bb = player->getBoundingBox();
//
//    //moved player
//    Rect bb_moved = bb;
//    bb_moved.origin.x += x;
//    bb_moved.origin.y += y;
//
//    //if its moving
//
//    int xside;
//    int yside;
//    float radius = 0;
//
//
//    //Find the tile the player is on.
//    const Rect tileBB = level->getTileBB(player->getPosition());
//
//    // I only calcualte the first lines we would cross here because that's all we need unless
//    // this runs on a computer so slow it only updates twice a second or less.
//
//    //Find the X side direction we are travelling.
//    if (x < 0)
//        xside = (int)(tileBB.getMinX());
//    else
//        xside = (int)(tileBB.getMaxX());
//
//    //Find the y side direction we are travelling.
//    if (y < 0)
//        yside = (int)(tileBB.getMinY());
//    else
//        yside = (int)(tileBB.getMaxY());
//
//    //find where the path intersects the grid
//    float vertisect = -1; // this is fraction of x traveled before the vertical line is hit
//    float horzisect = -1; // this is fraction of y traveled before  the horizontal line
//
//    // ydir and xdir represent the sign of the movement along each axis
//    const int xdir = (x<0)?-1:1;
//    const int ydir = (y<0)?-1:1;
//
//    //TODO: refine this to be a one time "vertisect = abs((pos.x - xsiderad) / x);"
//
//    /*=============================Check If/When a Grid line is Crossed=========================*/
//
//    //if we moved horizontally
//    if (x != 0)
//    {
//        // shift the xside to account for the radius
//        // this essentially finds when the closest x point on the player reaches xside
//        const float xsiderad = xside - ((radius) * xdir);
//
//        //If crossed the vertical line
//        if((pos.x - xsiderad) * ((pos.x + x) - xsiderad) <= 0)
//            vertisect = abs((pos.x - xsiderad) / x);
//    }
//
//    //if we moved vertically
//    if (y != 0)
//    {
//        // shift the yside to account for the radius
//        const float ysiderad = yside - ((radius) * ydir);
//
//        //If we get in range of the vertical line
//        if((pos.y - ysiderad) * ((pos.y + y) - ysiderad) <= 0)
//            horzisect = abs((pos.y - ysiderad) / y);
//    }
//
//    /*=============Check If There Were Any Collisions Based on The Grid Lines Crossed=============*/
//
//    // if it crosses both borders
//    // and both the xdir and ydir tiles are on, then the object is in the corner
//    if (vertisect != -1 &&
//        horzisect != -1 &&
//        sTiles[tI(tile_x, tile_y + ydir)]->isVisible() &&
//        sTiles[tI(tile_x + xdir, tile_y)]->isVisible())
//    {
//        pos.x = xside - (radius * xdir);
//        pos.y = yside - (radius * ydir);
//        return;
//    }
//
//    // variables for hit calculation
//    Point corner;
//    Point hitCorner;
//    float newx;
//    float newy;
//
//    /*            Horizontal Collision Detection
//     * Check a collision on the left or right side of a tile
//     * if one occurred, run collision detection again with the x movement as 0 to the edge of
//     * the offending tile then run this with any remaining x or y movement.
//     */
//
//    //if x intercepts
//    if (vertisect !=-1)
//    {
//        //if xdir, 0 is on
//        if (sTiles[tI(tile_x + xdir, tile_y)]->isVisible())
//        {
//            // set pos.x to be against the wall and cut x dir movement for a bit
//            pos.x = xside - (radius * xdir);
//            //revise x
//            //x = newPos.x - pos.x;
//            newx = 0;
//            newy = y;
//
//            if (horzisect !=-1)
//                newy = (y * (horzisect)) + ydir * 1;
//
//            //run check collision vertically to the end of the tile
//            handlePlayerCollisions(pos, newx, newy, radius);
//
//            //run what is left from this new position
//            if (horzisect != -1)
//                newx = (x * (horzisect));
//
//            handlePlayerCollisions(pos, newx, y - newy, radius);
//            return;
//        }
//    }
//
//    /*             Vertical Collision Detection
//     * Check a collision on the top or bottom side of a tile
//     * if one occurred, run collision detection again with the x movement as 0 to the edge of
//     * the offending tile then run this with any remaining x or y movement.
//     */
//    //if y intercepts
//    if (horzisect !=-1)
//    {
//        //if 0, ydir is on
//        if (sTiles[tI(tile_x, tile_y + ydir)]->isVisible())
//        {
//            // set pos.y to be against the wall and cut y dir movement for a bit
//            pos.y = yside - (radius * ydir);
//            //revise y
//            //y = newPos.y - pos.y;
//            newx = x;
//            newy = 0;
//
//            if (vertisect !=-1)
//                newx = (x * (vertisect)) + xdir * 1; // the distance to a little past the edge
//
//            //run check collision horizontally to a bit past the end of the tile
//            handlePlayerCollisions(pos, newx, newy, radius);
//
//            //run what is left from this new position
//            if (vertisect != -1)
//                newy = y * (1 - vertisect);
//
//            handlePlayerCollisions(pos, x - newx, newy, radius);
//            return;
//        }
//    }
//
//    /*             Corner Collision Detection
//     * For each direction check the two corners to the left and right or it in case
//     * we clipped it.
//     */
//
//    //if xdir,ydir is on
//    if (sTiles[tI(tile_x + xdir, tile_y + ydir)]->isVisible())
//    {
//        Point corner;
//        corner.x = xside;
//        corner.y = yside;
//
//        //if it hits the corner
//        // essentially, this checks the intersection of the ray made by the center of the object and
//        // a circle or radius 'radius' around the corner.
//        // This will tell me how far along the movement we first come in contact with the corner.
//        const float t1 = RayCircleIntersect(pos, newPos, corner, radius);
//        if (t1 != -1)
//        {
//            // we'll work on the corner movement later, for now just set position an kill movement
//            pos.x += x * t1;
//            pos.y += y * t1;
//            return;
//        }
//    }
//    //if -xdir,ydir is on
//    //horzisect != -1 &&
//    if (y != 0 &&
//        sTiles[tI(tile_x - xdir, tile_y + ydir)]->isVisible())
//    {
//        Point corner;
//        corner.x = xside - xdir * tileSize.width;
//        corner.y = yside;
//
//        //if it hits the corner
//        const float t1 = RayCircleIntersect(pos, newPos, corner, radius);
//        if (t1 != -1)
//        {
//            pos.x += x * t1;
//            pos.y += y * t1;
//            return;
//        }
//    }
//    //if xdir,-ydir is on
//    //vertisect != -1 &&
//    if (x != 0 &&
//        sTiles[tI(tile_x + xdir, tile_y - ydir)]->isVisible())
//    {
//        Point corner;
//        corner.x = xside;
//        corner.y = yside - ydir * tileSize.height;
//        
//        //if it hits the corner
//        const float t1 = RayCircleIntersect(pos, newPos, corner, radius);
//        if (t1 != -1)
//        {
//            pos.x += x * t1;
//            pos.y += y * t1;
//            return;
//        }
//    }
//    
//    // if it gets here then it didnt hit anything and we can move it freely
//    pos = newPos;
//    return;
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
            y = 1;
            break;
            
            // Player Left
        case 123: // Left Arrow
        case 0:  // A
            x = -1;
            break;
            
            // Player Down
        case 125: // Down Arrow
        case 1: // S
            y = -1;
            break;
            
            // Player Right
        case 124: // Right Arrow
        case 2: // D
            x = 1;
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
}