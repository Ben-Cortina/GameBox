//
//  TheBox.cpp
//  GameBox
//
//  Created by Ben Cortina on 7/25/13.
//
//

#include "TheBox.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

// returns the index position for the tile at x,y
int tI(int x, int y) {return y * THEBOX_MAX_WIDTH + x;}

float DistSquared (Point a, Point b){return (b.x-a.x)*(b.x-a.x)+(b.y-a.y)*(b.y-a.y);}
float Dist(Point a, Point b){return sqrtf(DistSquared(a,b));}
float Dot(Point a, Point b){return a.x*b.x+a.y*b.y;}

/**
 @brief     A modified version of bobobobo's "Circle line collision detection" answer on stackoverflow.com
 *          This calculates how far along the ray each intersection occurs.
 @return    How far along the line segment the first intersection with the circle occurs.
 *          0.2 is 20% from point v to point w. -1 = no intersect.
 */
float RayCircleIntersect(Point v, Point w, Point c, float r)
{
    Point a = w - v;
    Point b = v - c;
    const float d = Dot(a, a) ;
    const float e = 2 * Dot(b, a) ;
    const float f = Dot(b, b) - r * r ;
    
    float discriminant = e * e - 4 * d * f;
    
    if( discriminant < 0 )
    {
        // no intersection
        return -1;
    }
    else
    {
        // ray didn't totally miss sphere,
        // so there is a solution to
        // the equation.
        
        discriminant = sqrt(discriminant);
        
        // either solution may be on or off the ray so need to test both
        // t is always the first intersect, because BOTH discriminant and
        // a are nonnegative.
        float t1 = (-e - discriminant) / (2 * d);
        // bobobobo's solutions includes a second intersect as well. As
        // my problem will never need know the exit intersect, I will
        // only use t1
        //float t2 = (-e + discriminant) / (2 * d);
        
        
        // 3x HIT cases:
        //          -o->             --|-->  |            |  --|->
        // Impale(t1 hit,t2 hit), Poke(t1 hit,t2>1), ExitWound(t1<0, t2 hit),
        
        // 3x MISS cases:
        //       ->  o                     o ->              | -> |
        // FallShort (t1>1,t2>1), Past (t1<0,t2<0), CompletelyInside(t1<0, t2>1)
        
        if( t1 >= 0 && t1 <= 1 )
        {
            // t1 is an intersection, and if it hits,
            // it's closer than t2 would be
            // Impale, Poke
            
            //due to floating point inacuraccy, this would sometimes set the position slightly
            // further than the intersection, going in half steps should fix this
            return t1/2;
        }
        
        // no intn: FallShort, Past, CompletelyInside
        return -1;
    }
}

CommandState::CommandState()
{
    Up = Down = Right = Left = false;
}

TheBoxLayer::TheBoxLayer()
{
    player = PlayerSprite::create("Ball.png", this);
    timeLabel = LabelTTF::create("00.00", "Arial", 24);
    windowSize = Director::getInstance()->getVisibleSize();
    timePerLevel = 10;
    initTiles();
    setLayerSize(9, 9);
}

void TheBoxLayer::initTiles()
{
    //load the tile texture
    Texture2D* pixelTex = TextureCache::getInstance()->addImage("Pixel.png");
    
    for (int i = 0; i < THEBOX_MAX_HEIGHT * THEBOX_MAX_WIDTH; i++)
    {
        //create the sprite and add it
        sTiles[i] = Sprite::createWithTexture(pixelTex, Rect(0,0,1,1));
        addChild(sTiles[i], 0);
    }
}

void TheBoxLayer::initPlayer()
{
    
    // we want it to be about 1/20th of the grid
    float winSize = MIN(windowSize.width, windowSize.height);
    
    // winSize/20 will be 1/20th of the window in pixels. Divide this by the sprites size to
    // adjust the scale to match the image
    float scale = (winSize/40.0)/player->boundingBox().size.height;

    player->setScale(scale);
    player->setPosition(Point(windowSize.width/2,
                              windowSize.height/2));
    player->setColor(Color3B(100,200,0));
    
    exit.x = 0;
    exit.y = 0;
    
    addChild(player, 1);
}

// on "init" you need to initialize your instance
bool TheBoxLayer::init()
{    
    //init random seed
    srand (time(NULL));
    
    initPlayer();
    
    timeLabel->setPosition(Point(timeLabel->getContentSize().width / 2 + 10,
                                 windowSize.height - (timeLabel->getContentSize().height / 2 + 5)));
    timeLabel->setColor(Color3B(0,0,0));
    addChild(timeLabel);
    
    nextLevel();
    
    // add the sprite as a child to this layer
    scheduleUpdate();

    //turn off touch
    setTouchEnabled(true);
    setKeyboardEnabled(true);
    
    return true;
}

void TheBoxLayer::nextLevel()
{
    //increase time allowed per level
    timePerLevel += 10;
    nextChange = timePerLevel;
    nextSpawn = timePerLevel - TIME_PER_SPAWN;
    
    //reset player
    player->setPosition(Point(windowSize.width/2,
                              windowSize.height/2));
    
    //reset the timer
    timeRemaining = timePerLevel;
    setLayerSize(layerSize.width+1, layerSize.height+1);
    changeExit();
}

void TheBoxLayer::update(float dt)
{
    //Check for Victory or defeat
    checkConditions();
    
    //Update timer
    updateTimer(dt);
    
    // Check timer
    if (timeRemaining < nextChange)
        changeExit();
    if (timeRemaining < nextSpawn)
        spawnTile();

}

void TheBoxLayer::updateTimer(float dt)
{
    timeRemaining = timeRemaining - dt;
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << timeRemaining;
    timeLabel->setString((ss.str()).c_str());
}

void TheBoxLayer::spawnTile()
{
    //set the time for the next spawn
    nextSpawn -= TIME_PER_SPAWN;
    
    //get the tile the player is on
    Point pos = player->getPosition();
    Size margins = Size((windowSize.width - tileSize.width * layerSize.width)/2,
                        (windowSize.height - tileSize.height * layerSize.height)/2
                        );
    const int playerTile_x = (int)( (pos.x - margins.width) / tileSize.width );
    const int playerTile_y = (int)( (pos.y - margins.height) / tileSize.height );
    
    //choose a random tile that is greater than 1 away from the player's current tile
    int tile_x;
    int tile_y;
    do {
        tile_x = (rand() % (int)(layerSize.width - 2)) + 1;
        tile_y = (rand() % (int)(layerSize.height - 2)) + 1;
        std::cout << "r" << std::endl;
    } while ((abs(tile_x - playerTile_x) <= 1 &&
             abs(tile_y - playerTile_y) <= 1) ||
             sTiles[tI(tile_x, tile_y)]->isVisible() );
    
    std::cout << tile_x << " " << tile_y << std::endl;
    
    sTiles[tI(tile_x, tile_y)]->setVisible(true);
}

void TheBoxLayer::changeExit()
{
    //set the time for the next exit change
    nextChange -= TIME_PER_CHANGE;
    
    //choose the next exit
    const int tile = (rand() % (int)(layerSize.width - 2)) + 1;
    const int side = rand() % 4;
    
    //close the old exit
    sTiles[tI(exit.x, exit.y)]->setVisible(true);
    
    //choose the new exit
    switch (side) {
        case 0: // top
            exit.x = tile;
            exit.y = 0;
            break;
            
        case 1: // left
            exit.x = 0;
            exit.y = tile;
            break;
            
        case 2: // bot
            exit.x = tile;
            exit.y = layerSize.height - 1;
            break;
            
        case 3: // right
            exit.x = layerSize.width - 1;
            exit.y = tile;
            break;
            
        default:
            break;
    }
    
    //open the nex exit
    sTiles[tI(exit.x, exit.y)]->setVisible(false);
}

void TheBoxLayer::checkConditions()
{
    //check to see if the player has made it into the exit
    //This should be the case if the player is past the edge of the inner border
    
    //get the edges
    const Size margins = Size((windowSize.width - tileSize.width * layerSize.width)/2,
                        (windowSize.height - tileSize.height * layerSize.height)/2
                        );
    
    const int left = margins.width + tileSize.width;
    const int right = windowSize.width - (margins.width + tileSize.width);
    const int bot = margins.height + tileSize.height;
    const int top = windowSize.height - (margins.height + tileSize.height);
    
    //bounding box of the player
    Rect bb = player->getBoundingBox();
    
    //check to see if the player has won
    if (bb.getMinX() < left     ||
        bb.getMaxX() > right    ||
        bb.getMinY() < bot      ||
        bb.getMaxY() > top      )
    {
        //go to the next level
        nextLevel();
    }
    
}

void TheBoxLayer::updateTiles()
{
    const Size margins = Size((windowSize.width - tileSize.width * layerSize.width)/2,
                        (windowSize.height - tileSize.height * layerSize.height)/2
                        );
    
    for (int y = 0; y < THEBOX_MAX_HEIGHT; y++)
        for (int x = 0; x < THEBOX_MAX_WIDTH; x++)
        {
            if (y < layerSize.height && x < layerSize.width)
            {
                // update the center of the Sprite based on
                sTiles[tI(x, y)]->setPosition(Point(x * tileSize.width + tileSize.width/2 + margins.width,
                                                                    y * tileSize.height + tileSize.height/2 + margins.height
                                                                    )
                                                              );
                // update the size of the sprite
                sTiles[tI(x, y)]->setScale(tileSize.width);
            }
            sTiles[tI(x, y)]->setVisible(false);
        }
}

void TheBoxLayer::setLayerSize(int width, int height)
{
    //set layerSize
    layerSize = Size(width, height);

    //update TileSize
    float minSize = MIN(windowSize.width, windowSize.height);
    tileSize = Size(minSize / layerSize.width, minSize / layerSize.height);

    //set player speed
    player->setMaxSpeed(2* tileSize.height);
    
    //Update Tiles and borders
    updateTiles();
    updateBorder();
}

void TheBoxLayer::updateBorder()
{
    //turn on the left and right edges
    for (int x = 0; x < layerSize.width; x++)
    {
        sTiles[tI(x,0)]->setVisible(true);
        sTiles[tI(x,(int)(layerSize.height-1))]->setVisible(true);;
    }
    
    //turn on the top and bottom edges
    for (int y = 0; y < layerSize.height; y++)
    {
        sTiles[tI(0, y)]->setVisible(true);;
        sTiles[tI((int)(layerSize.width-1), y)]->setVisible(true);;
    }
}

void TheBoxLayer::keyPressed(int keyCode)
{

    // Because we have PlayerSprite handling its own update this make our job very easy
    // What we'll do here is call update to apply the current speed then change speed
    
    switch(keyCode)
    {
      // Exit 
      case 53: //Esc
        //returnHome();
        break;

      // Player Up
      case 126: // Up Arrow
      case 13:  // W
          if (!commandStates.Up)  // I check this incase the system sends multiple down presses
          {
              //set +y vel
              player->update(); //update the current vel
              player->setForceY(1); // set the new vel
              commandStates.Up = true;
          }
          break;

      // Player Left
      case 123: // Left Arrow
      case 0:  // A
          if (!commandStates.Left)
          {
              //set -x vel
              player->update();
              player->setForceX(-1);
              commandStates.Left = true;
          }
          break;

      // Player Down
      case 125: // Down Arrow
      case 1: // S
          if (!commandStates.Down)
          {
              //set -y vel
              player->update();
              player->setForceY(-1);
              commandStates.Down = true;
          }
          break;

      // Player Right
      case 124: // Right Arrow
      case 2: // D
          if (!commandStates.Right)
          {
              //set +x vel
              player->update();
              player->setForceX(1);
              commandStates.Right = true;
          }
          break;

      default:
          break;
    }
}

void TheBoxLayer::keyReleased(int keyCode)
{
    /*
     * now one thing we have to be aware of here is the interaction between opposing keys.
     * The two most common interactions are:
     * A: Opposing keys cancel out
     * B: The most recently pressed opposing key is used
     * I'm using B because if someone is quickly switching to reverse direction is feasibly
     * they may press the new key before they release the previous one and that slight pause
     * that would occur from A is likely unwanted
     */
    switch(keyCode)
    {
      // Exit 
      case 53: //Esc
        //returnHome();
        break;

      // Player Up
      case 126: // Up Arrow
      case 13:  // W
            player->update();
            if (!commandStates.Down) // if the opposing key is not pressed
                player->setForceY(0); //zero
            else
                player->setForceY(-1);
            commandStates.Up = false;
          break;

      // Player Left
      case 123: // Left Arrow
      case 0:  // A
            player->update();
            if (!commandStates.Right) // if the opposing key is not pressed
                player->setForceX(0); //zero
            else
                player->setForceX(1);
            commandStates.Left = false;
          break;

      // Player Down
      case 125: // Down Arrow
      case 1: // S
            player->update();
            if (!commandStates.Up) // if the opposing key is not pressed
                player->setForceY(0); //zero
            else
                player->setForceY(1);
            commandStates.Down = false;
          break;

      // Player Right
      case 124: // Right Arrow
      case 2: // D
            player->update();
            if (!commandStates.Left) // if the opposing key is not pressed
                player->setForceX(0); //zero
            else
                player->setForceX(-1);
            commandStates.Right = false;
          break;

      default:
          break;
    }

}

/* returns whether or not a cercle intersects a rectangle
 */
bool CircleIntersectTile(Point circleCent, float radius, Sprite* tile)
{
    // if the tile is off, we dont need to check for collision
    if (!tile->isVisible())
        return false;
    // we are going to do a simple circle to rectangle intersect check so well use the bounding box
    // of the tile
    
    Rect bb = tile->getBoundingBox();
    float distX;
    float distY;
    
    // find how far the circle's center is from the tile's center
    distX = abs(circleCent.x - bb.origin.x + bb.size.width / 2);
    distY = abs(circleCent.y - bb.origin.y + bb.size.height / 2);
    
    // We are now checking one dimension at a time, if the circles is out of range in either x or y then
    // there is no way there is a collision
    if (distX > (bb.size.width / 2 + radius) ||
        distY > (bb.size.height / 2 + radius))
        return false;
    
    // We know the circle is in range. This rules out the possibility that it is in a corner
    if (distX <= (bb.size.width / 2) ||
        distY <= (bb.size.height / 2))
        return true; 
    
    // We know its in the corner
    // This finds the distance from the center of the circle to the corner of the rectangle
    float distToCorner_sq = (distX - bb.size.width / 2) * (distX - bb.size.width / 2) +
                            (distY - bb.size.height / 2) * (distY - bb.size.height / 2);
    
    return (distToCorner_sq <= (radius * radius));
}

void TheBoxLayer::handlePlayerCollisions(Point &pos, float x, float y, float radius)
{
    /*
     * The most common collision detection is "hitbox" collision where the dev defines a rectangle
     * for each element (This is usually the image rectangle but can be any size). Then all that
     * needs to be checked for is if the two rectangles overlap. If your game sprites (like Terraria's)
     * dont have any majorly protruding features this is sufficient. Otherwise, collision will need to
     * be dont on a pixel by pixel basis. There are many many different methods for pixel collision.
     * essentially you iterate through the two offending image's pixels checking if two "solid" pixels
     * overlap.
     * This all gets more complicated as you factor in rotation and velocity. As you will check
     * incrementely, it is not uncommon for an object moving fast enough to go through another
     * object if it is moving fast enough. Therefore it is require to check the projection for
     * any intersect that may have occured.
     *
     * I am using a method specific to this game. It looks long but, in theory, it's a lot less
     * taxing than sending a checkCollision for each nearby tile.
     */
    
    // if there is no movement no check needs to occur
    if (x == 0 && y ==0)
        return;
    
    //if its moving
    
    int xside;
    int yside;
    
    
    //Find the tile the player is on and was on.
    const Size margins = Size((windowSize.width - tileSize.width * layerSize.width)/2,
                        (windowSize.height - tileSize.height * layerSize.height)/2
                        );
    const int tile_x = (int)( (pos.x - margins.width) / tileSize.width );
    const int tile_y = (int)( (pos.y - margins.height) / tileSize.height );
    
    //This would be the new position of the object if it was unObstructed
    Point newPos;
    newPos.x = pos.x + x;
    newPos.y = pos.y + y;
    
    const Rect bB = sTiles[tI(tile_x, tile_y)]->getBoundingBox();
    
    // I only calcualte the first lines we would cross here because that's all we need unless
    // this runs on a computer so slow it only updates twice a second or less.
    
    //Find the X side direction we are travelling.
    if (x < 0)
        xside = (int)(bB.getMinX());
    else
        xside = (int)(bB.getMaxX());
    
    //Find the y side direction we are travelling.
    if (y < 0)
        yside = (int)(bB.getMinY());
    else
        yside = (int)(bB.getMaxY());

    //find where the path intersects the grid
    float vertisect = -1; // this is fraction of x traveled before the vertical line is hit
    float horzisect = -1; // this is fraction of y traveled before  the horizontal line
    
    // ydir and xdir represent the sign of the movement along each axis
    const int xdir = (x<0)?-1:1;
    const int ydir = (y<0)?-1:1;
    
    //TODO: refine this to be a one time "vertisect = abs((pos.x - xsiderad) / x);"
    
    /*=============================Check If/When a Grid line is Crossed=========================*/
    
    //if we moved horizontally
    if (x != 0)
    {
        // shift the xside to account for the radius
        // this essentially finds when the closest x point on the player reaches xside
        const float xsiderad = xside - ((radius) * xdir);
        
        //If crossed the vertical line
        if((pos.x - xsiderad) * ((pos.x + x) - xsiderad) <= 0)
            vertisect = abs((pos.x - xsiderad) / x);
    }
    
    //if we moved vertically
    if (y != 0)
    {
        // shift the yside to account for the radius
        const float ysiderad = yside - ((radius) * ydir);
        
        //If we get in range of the vertical line
        if((pos.y - ysiderad) * ((pos.y + y) - ysiderad) <= 0)
            horzisect = abs((pos.y - ysiderad) / y);
    }
    
    /*=============Check If There Were Any Collisions Based on The Grid Lines Crossed=============*/
    
    // if it crosses both borders
    // and both the xdir and ydir tiles are on, then the object is in the corner
    if (vertisect != -1 &&
        horzisect != -1 &&
        sTiles[tI(tile_x, tile_y + ydir)]->isVisible() &&
        sTiles[tI(tile_x + xdir, tile_y)]->isVisible())
    {
            pos.x = xside - (radius * xdir);
            pos.y = yside - (radius * ydir);
            return;
    }
    
    // variables for hit calculation
    Point corner;
    Point hitCorner;
    float newx;
    float newy;
    
    /*            Horizontal Collision Detection
     * Check a collision on the left or right side of a tile
     * if one occurred, run collision detection again with the x movement as 0 to the edge of
     * the offending tile then run this with any remaining x or y movement.
     */
    
    //if x intercepts
    if (vertisect !=-1)
    {
        //if xdir, 0 is on
        if (sTiles[tI(tile_x + xdir, tile_y)]->isVisible())
        {
            // set pos.x to be against the wall and cut x dir movement for a bit
            pos.x = xside - (radius * xdir);
            //revise x
            //x = newPos.x - pos.x;
            newx = 0;
            newy = y;
        
            if (horzisect !=-1)
                newy = (y * (horzisect)) + ydir * 1;
        
            //run check collision vertically to the end of the tile
            handlePlayerCollisions(pos, newx, newy, radius);
            
            //run what is left from this new position
            if (horzisect != -1)
                newx = (x * (horzisect));

            handlePlayerCollisions(pos, newx, y - newy, radius);
            return;
        }
    }
    
    /*             Vertical Collision Detection
     * Check a collision on the top or bottom side of a tile
     * if one occurred, run collision detection again with the x movement as 0 to the edge of
     * the offending tile then run this with any remaining x or y movement.
     */
    //if y intercepts
    if (horzisect !=-1)
    {
        //if 0, ydir is on
        if (sTiles[tI(tile_x, tile_y + ydir)]->isVisible())
        {
            // set pos.y to be against the wall and cut y dir movement for a bit
            pos.y = yside - (radius * ydir);
            //revise y
            //y = newPos.y - pos.y;
            newx = x;
            newy = 0;
            
            if (vertisect !=-1)
                newx = (x * (vertisect)) + xdir * 1; // the distance to a little past the edge
            
            //run check collision horizontally to a bit past the end of the tile
            handlePlayerCollisions(pos, newx, newy, radius);
            
            //run what is left from this new position
            if (vertisect != -1)
                newy = y * (1 - vertisect);

            handlePlayerCollisions(pos, x - newx, newy, radius);
            return;
        }
    }
    
    /*             Corner Collision Detection
     * For each direction check the two corners to the left and right or it in case
     * we clipped it.
     */
    
    //if xdir,ydir is on
    if (sTiles[tI(tile_x + xdir, tile_y + ydir)]->isVisible())
    {
        Point corner;
        corner.x = xside;
        corner.y = yside;

        //if it hits the corner
        // essentially, this checks the intersection of the ray made by the center of the object and
        // a circle or radius 'radius' around the corner.
        // This will tell me how far along the movement we first come in contact with the corner.
        const float t1 = RayCircleIntersect(pos, newPos, corner, radius);
        if (t1 != -1)
        {
            // we'll work on the corner movement later, for now just set position an kill movement
            pos.x += x * t1;
            pos.y += y * t1;
            return;
        }
    }
    //if -xdir,ydir is on
    //horzisect != -1 &&
    if (y != 0 &&
        sTiles[tI(tile_x - xdir, tile_y + ydir)]->isVisible())
    {
        Point corner;
        corner.x = xside - xdir * tileSize.width;
        corner.y = yside;

        //if it hits the corner
        const float t1 = RayCircleIntersect(pos, newPos, corner, radius);
        if (t1 != -1)
        {
            pos.x += x * t1;
            pos.y += y * t1;
            return;
        }
    }
    //if xdir,-ydir is on
    //vertisect != -1 &&
    if (x != 0 &&
        sTiles[tI(tile_x + xdir, tile_y - ydir)]->isVisible())
    {
        Point corner;
        corner.x = xside;
        corner.y = yside - ydir * tileSize.height;
        
        //if it hits the corner
        const float t1 = RayCircleIntersect(pos, newPos, corner, radius);
        if (t1 != -1)
        {
            pos.x += x * t1;
            pos.y += y * t1;
            return;
        }
    }
    
    // if it gets here then it didnt hit anything and we can move it freely
    pos = newPos;
    return;
    
}

//This will initialize and add the layer to the scene
void TheBoxLayer::runThisGame(Object* pSender)
{
    //init
    MyScene* scene = new MyScene();
    TheBoxLayer* layer = new TheBoxLayer();
    layer->initWithColor(Color4B(255,255,255,255));

    scene->runLayer(layer);

}

