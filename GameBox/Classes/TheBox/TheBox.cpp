//
//  TheBox.cpp
//  GameBox
//
//  Created by Ben Cortina on 7/25/13.
//
//

#include "TheBox.h"
//debugging
#include <iostream>
//I need these for the timer
#include <string>
#include <sstream>
#include <iomanip>

// returns the index position for the tile at x,y
int tI(int x, int y) {return y * THEBOX_MAX_WIDTH + x;}

TheBoxLayer::TheBoxLayer()
{
    player = Coords(0,0);
    playerColor = Color3B(100, 200, 125);
    
    exit = Coords(0,0);
    
    timePerLevel = 5;
    
    timeLabel = LabelTTF::create("00.00", "Arial", 24);
    windowSize = Director::getInstance()->getVisibleSize();
    
    tileColor = Color3B(0,0,0);
    initTiles();
    setLayerSize(9, 9);
}

void TheBoxLayer::setPlayer(Sprite* tile, bool set)
{
    tile->setVisible( set );
    tile->setColor( (set)? playerColor : tileColor );
    
}

void TheBoxLayer::initTiles()
{
    //load the tile texture
    Texture2D* pixelTex = TextureCache::getInstance()->addImage("Pixel.png");
    
    for (int i = 0; i < THEBOX_MAX_HEIGHT * THEBOX_MAX_WIDTH; i++)
    {
        //create the sprite and add it
        sTiles[i] = Sprite::createWithTexture(pixelTex, Rect(0,0,1,1));
        sTiles[i]->setColor(tileColor);
        addChild(sTiles[i], 0);
    }
}

void TheBoxLayer::initPlayer()
{
    player.x = (layerSize.x + 0.5) / 2;
    player.y = (layerSize.y + 0.5) / 2;
    setPlayer(sTiles[tI(player.x, player.y)], true);
    
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

    //turn off touch
    setTouchEnabled(true);
    setKeyboardEnabled(true);
    
    return true;
}

void TheBoxLayer::nextLevel()
{
    //this will reset my timers
    unscheduleAllSelectors();
    
    //this will start the constant calls to update
    scheduleUpdate();
    
    // using schedule allows me to have cocos2d call a function exery X seconds
    this->schedule( schedule_selector(TheBoxLayer::spawnTile), TIME_PER_SPAWN );
    this->schedule( schedule_selector(TheBoxLayer::changeExit), 1.5+((float)layerSize.y+2.0)/20.0 );
    
    std::cout << 2+((float)layerSize.y+2.0)/20.0 << std::endl;
    
    //turn off player
    setPlayer(sTiles[tI(player.x, player.y)], false);
    
    //increase time allowed per level
    timePerLevel += TIME_INC_PER_LEVEL;
    
    //reset the timer
    timeRemaining = timePerLevel;
    
    //increase board size
    setLayerSize(layerSize.x+2, layerSize.y+2);
    
    //get a new exit
    changeExit(0);
    
    //turn on player
    player.x = (layerSize.x + 0.5) / 2;
    player.y = (layerSize.y + 0.5) / 2;
    setPlayer(sTiles[tI(player.x, player.y)], true);
}

void TheBoxLayer::update(float dt)
{
    //Update timer
    timeRemaining = timeRemaining - dt;
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << timeRemaining;
    timeLabel->setString((ss.str()).c_str());
}


void TheBoxLayer::spawnTile(float dt)
{
    //choose a random tile that is not on
    Coords tile;
    do {
        tile.x = (rand() % (int)(layerSize.x - 2)) + 1;
        tile.y = (rand() % (int)(layerSize.y - 2)) + 1;
        
        //while its not on
    } while (sTiles[tI(tile.x, tile.y)]->isVisible() );
    
    sTiles[tI(tile.x, tile.y)]->setVisible(true);
}

void TheBoxLayer::changeExit(float dt)
{
    
    //choose the next exit
    const int tile = (rand() % (int)(layerSize.x - 2)) + 1;
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
            exit.y = layerSize.y - 1;
            break;
            
        case 3: // right
            exit.x = layerSize.x - 1;
            exit.y = tile;
            break;
            
        default:
            break;
    }
    
    //open the next exit
    sTiles[tI(exit.x, exit.y)]->setVisible(false);
}

void TheBoxLayer::checkConditions()
{
    //check to see if the player has made it into the exit
    if (player == exit)
        nextLevel();
    
}

void TheBoxLayer::updateTiles()
{
    const Size margins = Size((windowSize.width - tileSize.width * layerSize.x)/2,
                        (windowSize.height - tileSize.height * layerSize.y)/2
                        );
    
    for (int y = 0; y < THEBOX_MAX_HEIGHT; y++)
        for (int x = 0; x < THEBOX_MAX_WIDTH; x++)
        {
            if (y < layerSize.y && x < layerSize.x)
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

void TheBoxLayer::setLayerSize(int x, int y)
{
    //set layerSize
    layerSize = Coords(x, y);

    //update TileSize
    float minSize = MIN(windowSize.width, windowSize.height);
    tileSize = Size(minSize / layerSize.x, minSize / layerSize.y);
    
    //Update Tiles and borders
    updateTiles();
    updateBorder();
}

void TheBoxLayer::updateBorder()
{
    //turn on the left and right edges
    for (int x = 0; x < layerSize.x; x++)
    {
        sTiles[tI(x,0)]->setVisible(true);
        sTiles[tI(x,(int)(layerSize.y-1))]->setVisible(true);;
    }
    
    //turn on the top and bottom edges
    for (int y = 0; y < layerSize.y; y++)
    {
        sTiles[tI(0, y)]->setVisible(true);;
        sTiles[tI((int)(layerSize.x-1), y)]->setVisible(true);;
    }
}

bool TheBoxLayer::checkPlayerCollision(int dx, int dy)
{
    return (sTiles[tI(player.x + dx, player.y + dy)]->isVisible());
}

void TheBoxLayer::keyPressed(int keyCode)
{
    
    //the change as a result of the keypress
    int x = 0;
    int y = 0;
    
    switch(keyCode)
    {
      // Exit 
      case 53: //Esc
        //returnHome();
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
    
    //run the resulting press
    if(!checkPlayerCollision(x, y))
    {
        //turn off old player tile
        setPlayer(sTiles[tI(player.x, player.y)], false);
        
        //update player position
        player += Coords(x,y);
        
        //turn on new player tile
        setPlayer(sTiles[tI(player.x, player.y)], true);
        
        //Check for Victory or defeat
        checkConditions();
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

}

