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

CommandState::CommandState()
{
    Up = Down = Right = Left = false;
}

TheBoxLayer::TheBoxLayer()
{
    player = PlayerSprite::create("Ball.png");
    windowSize = Director::getInstance()->getVisibleSize();
    initTiles();
    setLayerSize(1, 1);
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
    
    addChild(player, 1);
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

    //set player speed
    player->setMaxSpeed(2* tileSize.height);
    
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

