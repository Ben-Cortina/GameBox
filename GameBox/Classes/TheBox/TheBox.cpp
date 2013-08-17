//
//  TheBox.cpp
//  GameBox
//
//  Created by Ben Cortina on 7/25/13.
//
//

#include "TheBox.h"
#include <iostream>

#define TILE_ON Color3B(0,0,0)
#define TILE_OFF Color3B(255,255,255)

BoxCommand::BoxCommand()
{
    // -1 will be the "not set" time
    pressed = released = -1;
}

TheBoxLayer::TheBoxLayer()
{
    player = PlayerSprite::create("Ball.png");
    windowSize = Director::getInstance()->getVisibleSize();
    initTiles();
    setLayerSize(1, 1);

    keyChange = false;
}

void TheBoxLayer::initTiles()
{
    //load the tile texture
    Texture2D* pixelTex = TextureCache::getInstance()->addImage("Pixel.png");
    
    for (int i = 0; i < THEBOX_MAX_HEIGHT * THEBOX_MAX_WIDTH; i++)
    {
        //create the sprite and add it
        sTiles[i] = Sprite::createWithTexture(pixelTex, Rect(0,0,1,1));
        this->addChild(sTiles[i], 0);
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
    
    this->addChild(player, 1);
}

// on "init" you need to initialize your instance
bool TheBoxLayer::init()
{    

    setLayerSize(10, 10);
    
    initPlayer();
    
    // add the sprite as a child to this layer
    scheduleUpdate();

    //turn off touch
    setTouchEnabled(true);
    setKeyboardEnabled(true);
    
    return true;
}

void TheBoxLayer::update(float dt)
{

    //Update Player Position
    keyHandling(dt);

    //Check for collision

    //Check for victory

}

void TheBoxLayer::updateTiles()
{
    Size margins = Size((windowSize.width - tileSize.width * layerSize.width)/2,
                        (windowSize.height - tileSize.height * layerSize.height)/2
                        );
    
    for (int y = 0; y < layerSize.height; y++)
        for (int x = 0; x < layerSize.width; x++)
        {
            // update the center of the Sprite based on
            sTiles[y * THEBOX_MAX_WIDTH + x]->setPosition(Point(x * tileSize.width + tileSize.width/2 + margins.width,
                                                                y * tileSize.height + tileSize.height/2 + margins.height
                                                                )
                                                          );
            // update the size of the sprite
            sTiles[y * THEBOX_MAX_WIDTH + x]->setScale(tileSize.width);
            // set each tiles color to white
            sTiles[y * THEBOX_MAX_WIDTH + x]->setColor(TILE_OFF);
        }
}

void TheBoxLayer::setLayerSize(int width, int height)
{
    //set layerSize
    layerSize = Size(width, height);

    //update TileSize
    float minSize = MIN(windowSize.width, windowSize.height);
    tileSize = Size(minSize / layerSize.width, minSize / layerSize.height);

    //Update Tiles and borders
    updateTiles();
    updateBorder();
}

void TheBoxLayer::updateBorder()
{
    for (int x = 0; x < layerSize.width; x++)
    {
        sTiles[x]->setColor(TILE_ON);
        sTiles[((int)layerSize.height-1) * THEBOX_MAX_WIDTH + x]->setColor(TILE_ON);
    }
    for (int y = 0; y < layerSize.height; y++)
    {
        sTiles[y * THEBOX_MAX_WIDTH]->setColor(TILE_ON);
        sTiles[y * THEBOX_MAX_WIDTH + ((int)layerSize.width-1)]->setColor(TILE_ON);
    }
}

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

void TheBoxLayer::keyPressed(int keyCode)
{
    
    //get the time of press
    float time = getTime();


    switch(keyCode)
    {
      // Exit 
      case 53: //Esc
        //returnHome();
        break;

      // Player Up
      case 126: // Up Arrow
      case 13:  // W
          if (commandTime.Up.pressed !=-1)
          {
              commandTime.Up.pressed = time;
              keyChange = true;
          }
          break;

      // Player Left
      case 123: // Left Arrow
      case 0:  // A
          if (commandTime.Left.pressed !=-1)
          {
              commandTime.Left.pressed = time;
              keyChange = true;
          }
          break;

      // Player Down
      case 125: // Down Arrow
      case 1: // S
          if (commandTime.Down.pressed !=-1)
          {
              commandTime.Down.pressed = time;
              keyChange = true;
          }
          break;

      // Player Right
      case 124: // Right Arrow
      case 2: // D
          if (commandTime.Right.pressed !=-1)
          {
              commandTime.Right.pressed = time;
              keyChange = true;
          }
          break;

      default:
          break;
    }
}

void TheBoxLayer::keyReleased(int keyCode)
{
    //get the time of release
    float time = getTime();

    /*
     * This sets the times.
     * to ensure I dont zero a velocity when one opposing key is released
     * while another is still being pressed, I check the opposing key
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
          commandTime.Up.released = time;
          keyChange = true;
          break;

      // Player Left
      case 123: // Left Arrow
      case 0:  // A
          commandTime.Left.released = time;
          keyChange = true;
          break;

      // Player Down
      case 125: // Down Arrow
      case 1: // S
          commandTime.Down.released = time;
          keyChange = true;
          break;

      // Player Right
      case 124: // Right Arrow
      case 2: // D
          commandTime.Right.released = time;
          keyChange = true;
          break;

      default:
          break;
    }

}

void TheBoxLayer::keyHandling(float dt)
{
    
    // if there was a key change since the last call we will need to calculate how long the key
    // was pressed so we'll get the time and subtract it from the keyevent time
    float time = dt;
    float temp;
    if (keyChange)
        time = getTime();
    
    /*
     * now one thing we have to be aware of here is the interaction between opposing keys.
     * The two most common interactions are:
     * A: Opposing keys cancel out 
     * B: The most recently pressed opposing key is used
     * I'm using B because if someone is quickly switching to reverse direction is feasibly
     * they may press the new key before they release the previous one and that slight pause
     * that would occur from A is likely unwanted
     */ 

    /*
     * The way this works is:
     * if (press time == set)
     *   if (release time == set)
     *     time of press = released time - pressed time //note these times are relative to the last update
     *   else
     *     time of press = dt - pressed time
     *   applyposition change using time of press
     *   zero press timers
     */
    
    // Player Up
    // set +y vel
    if (commandTime.Up.pressed !=-1)
    {
       if (commandTime.Up.pressed !=0)
           temp = time - commandTime.Up.pressed;
       if (commandTime.Up.released !=-1)
       {
           temp = commandTime.Up.released - commandTime.Up.pressed;
           commandTime.Up.released = -1;
       }
       else
           temp = dt - commandTime.Up.pressed;

       player->applyVelocityY(windowSize.height/50, temp);
       commandTime.Up.pressed = 0;
    }

    // Player Left
    // set -x vel
    if (commandTime.Left.pressed !=-1)
    {
       if (commandTime.Left.released !=-1)
       {
           time = commandTime.Left.released - commandTime.Left.pressed;
           commandTime.Left.released = -1;
       }
       else
           time = dt - commandTime.Left.pressed;

       player->applyVelocityX(-windowSize.height/50, time);
       commandTime.Left.pressed = 0;
    }

    // Player Down
    // set -y vel
    if (commandTime.Down.pressed !=-1)
    {
       if (commandTime.Down.released !=-1)
       {
           time = commandTime.Down.released - commandTime.Down.pressed;
           commandTime.Down.released = -1;
       }
       else
           time = dt - commandTime.Down.pressed;

       player->applyVelocityY(-windowSize.height/50, time);
       commandTime.Down.pressed = 0;
    }

    // Player Right
    // set +x vel
    if (commandTime.Right.pressed !=-1)
    {
       if (commandTime.Right.released !=-1)
       {
           time = commandTime.Right.released - commandTime.Right.pressed;
           commandTime.Right.released = -1;
       }
       else
           time = dt - commandTime.Right.pressed;

       player->applyVelocityY(windowSize.height/50, time);
       commandTime.Right.pressed = 0;
    }
    
    keyChange = false;
}


//This will initialize and add the layer to the scene
void TheBoxLayer::runThisGame(Object* pSender)
{
    //init
    MyScene* scene = new MyScene();
    TheBoxLayer* layer = new TheBoxLayer();
    layer->initWithColor(Color4B(255,255,255,255));

    scene->runLayer(layer);

    //Director::getInstance()->replaceScene(scene);
    
    //release the sender
    //pSender->release();

}

