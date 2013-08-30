/**
 *  @file   TheBox.cpp
 *  @author Ben Cortina
 *  @date   7/25/13.
 */

#include "TheBox.h"
#include "HomeScene.h"
//debugging
#include <iostream>
//I need these for the timer
#include <string>
#include <sstream>
#include <iomanip>

// returns the index position for the tile at x,y
inline int tI(int x, int y) {return y * THEBOX_MAX_WIDTH + x;}

TheBoxLayer::TheBoxLayer()
{
    player.loc = Coords(0,0);
    player.color = Color3B(100, 200, 125);
    
    exitLoc = Coords(0,0);
    
    timePerLevel = 8;
    
    timeLabel = LabelTTF::create("00.00", "Arial", 24);
    windowSize = Director::getInstance()->getVisibleSize();
    
    tileColor = Color3B(0,0,0);
    initTiles();
    setLayerSize(11, 11);
    isSnake = 3;
}

void TheBoxLayer::setPlayer(Sprite* tile, bool set)
{
    if(isSnake == 0)
        tile->setVisible( set );
    else
        tile->setVisible(true);
    
    tile->setColor( (set)? player.color : tileColor );
    
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
    player.loc.x = (layerSize.x + 0.5) / 2;
    player.loc.y = (layerSize.y + 0.5) / 2;
    setPlayer(sTiles[tI(player.loc.x, player.loc.y)], true);
    
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

    startMenu();
    
    return true;
}

void TheBoxLayer::restartGame()
{
    //this will reset my timers
    unscheduleAllSelectors();
    
    //reset size
    setLayerSize(9, 9);
    
    setTouchEnabled(true);
    setKeyboardEnabled(true);
    
    //reset game
    nextLevel();
    
}

void TheBoxLayer::nextLevel()
{
    std::cout << 2+((float)layerSize.y+2.0)/20.0 << std::endl;
    //this will reset my timers
    unscheduleAllSelectors();
    
    //turn off player
    setPlayer(sTiles[tI(player.loc.x, player.loc.y)], false);
    if (isSnake == 1) //we'll need to manually turn off the old player if its snakemode
        sTiles[tI(player.loc.x, player.loc.y)]->setVisible(false);
    
    //increase time allowed per level
    timePerLevel += TIME_INC_PER_LEVEL;
    
    //reset the timer
    timeRemaining = timePerLevel;
    
    //increase board size and clear tiles
    setLayerSize(layerSize.x+2, layerSize.y+2);
    
    //get a new exitLoc
    changeExit(-1);
    
    //turn on player
    player.loc.x = (layerSize.x + 0.5) / 2;
    player.loc.y = (layerSize.y + 0.5) / 2;
    setPlayer(sTiles[tI(player.loc.x, player.loc.y)], true);
    
    //this will start the constant calls to update
    scheduleUpdate();
    
    // using schedule allows me to have cocos2d call a function exery X seconds
    if (isSnake == 0)
        this->schedule( schedule_selector(TheBoxLayer::spawnTile), TIME_PER_SPAWN );
    this->schedule( schedule_selector(TheBoxLayer::changeExit), 2.0+((float)layerSize.y+2.0)/15.0 );
}

void TheBoxLayer::update(float dt)
{
    //Update time remaining
    timeRemaining = timeRemaining - dt;
    
    //out of time
    if (timeRemaining <= 0)
    {
        timeRemaining = 0;
        startMenu();
        isSnake = 3;
    }
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << timeRemaining;
    timeLabel->setString((ss.str()).c_str());
}


void TheBoxLayer::spawnTile(float dt)
{
    std::cout << "tile " << dt << std::endl;
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
    
    //choose the next exitLoc
    const int tile = (rand() % (int)(layerSize.x - 2)) + 1;
    const int side = rand() % 4;
    
    //close the old exitLoc if its not a new level
    if (dt !=-1)
        sTiles[tI(exitLoc.x, exitLoc.y)]->setVisible(true);
    
    //choose the new exitLoc
    switch (side) {
        case 0: // top
            exitLoc.x = tile;
            exitLoc.y = 0;
            break;
            
        case 1: // left
            exitLoc.x = 0;
            exitLoc.y = tile;
            break;
            
        case 2: // bot
            exitLoc.x = tile;
            exitLoc.y = layerSize.y - 1;
            break;
            
        case 3: // right
            exitLoc.x = layerSize.x - 1;
            exitLoc.y = tile;
            break;
            
        default:
            break;
    }
    
    //open the next exitLoc
    sTiles[tI(exitLoc.x, exitLoc.y)]->setVisible(false);
}

void TheBoxLayer::checkConditions()
{
    //check to see if the player has made it into the exitLoc
    if (player.loc== exitLoc)
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
    return (sTiles[tI(player.loc.x + dx, player.loc.y + dy)]->isVisible());
}

void TheBoxLayer::keyPressed(int keyCode)
{
    //the change as a result of the keypress
    int x = 0;
    int y = 0;
    
    switch(keyCode)
    {
      // exitLoc 
      case 53: //Esc
            openMenu();
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
        setPlayer(sTiles[tI(player.loc.x, player.loc.y)], false);
        
        //update player position
        player.loc+= Coords(x,y);
        
        //turn on new player tile
        setPlayer(sTiles[tI(player.loc.x, player.loc.y)], true);
        
        //Check for Victory or defeat
        checkConditions();
    }
}

void TheBoxLayer::setSnake(int b)
{
    //if there was a change we will restart with snake change
    if(isSnake != b)
    {
        isSnake = b;
        restartGame();
    }
    
    std::cout << "snakey" << std::endl;
    //resume game
    Director::getInstance()->resume();
    setTouchEnabled(true);
    setKeyboardEnabled(true);
}

void TheBoxLayer::startMenu()
{
    //pause game and input
    Director::getInstance()->pause();
    setTouchEnabled(false);
    setKeyboardEnabled(false);
    
    OverLayer* overlay = new OverLayer();
    
    //create menu
    Menu* pMenu = Menu::create();
    
    //create menu items
    const Point origin = Director::getInstance()->getVisibleOrigin();

    MenuItemLabel* title;
    MenuItemLabel* snake;
    MenuItemLabel* standard;
    MenuItemLabel* mainMenu;
    
    //set up Main menu item
    mainMenu = MenuItemLabel::create(LabelTTF::create("Return to Main Menu", "Arial", 20),
                                  TheBoxLayer::restoreHomeLayer);
    mainMenu->setPosition(0,0);
    pMenu->addChild(mainMenu);
    
    //set up Snake Mode item
    snake = MenuItemLabel::create(LabelTTF::create("Snake Mode", "Arial", 20),
                                  TheBoxLayer::onSnake);
    snake->setPosition(0,mainMenu->getPosition().y + mainMenu->getContentSize().height+10);
    pMenu->addChild(snake);
    
    //set up Standard Mode item
    standard = MenuItemLabel::create(LabelTTF::create("Standard Mode", "Arial", 20),
                                     TheBoxLayer::offSnake);
    standard->setPosition(0,snake->getPosition().y + snake->getContentSize().height+10);
    pMenu->addChild(standard);
    
    //set up Title item
    title = MenuItemLabel::create(LabelTTF::create("Start Menu", "Arial", 30));
    title->setPosition(0,standard->getPosition().y + standard->getContentSize().height+20);
    pMenu->addChild(title);
    
    //center the menu
    float heightofitems = standard->getBoundingBox().getMaxY() - mainMenu->getBoundingBox().getMinY();
    pMenu->setPosition(origin.x + windowSize.width/2,
                       origin.y + windowSize.height/2 - heightofitems/2);
    
    overlay->initWithColorMenu( Color4B(0,0,0,150), pMenu);
    addChild(overlay);
    
    
}

void TheBoxLayer::openMenu()
{
    //pause game and input
    Director::getInstance()->pause();
    setTouchEnabled(false);
    setKeyboardEnabled(false);
    
    OverLayer* overlay = new OverLayer();
    
    //create menu
    Menu* pMenu = Menu::create();
    
    //create menu items
    const Point origin = Director::getInstance()->getVisibleOrigin();
    std::string toggleSnake;
    MenuItemLabel* title;
    MenuItemLabel* snake;
    MenuItemLabel* mainMenu;
    
    //set up Main menu item
    mainMenu = MenuItemLabel::create(LabelTTF::create("Return to Main Menu", "Arial", 20),
                                     TheBoxLayer::restoreHomeLayer);
    mainMenu->setPosition(0,0);
    pMenu->addChild(mainMenu);
    
    if (isSnake == 1)
        snake = MenuItemLabel::create(LabelTTF::create("Turn Snake Mode OFF", "Arial", 20),
                                                         TheBoxLayer::offSnake);
    else
        snake = MenuItemLabel::create(LabelTTF::create("Turn Snake Mode ON", "Arial", 20),
                                         TheBoxLayer::onSnake);
        
    snake->setPosition(0, mainMenu->getPosition().y + mainMenu->getContentSize().height+10);
    pMenu->addChild(snake);
    
    //set up Title item
    title = MenuItemLabel::create(LabelTTF::create("Pause Menu", "Arial", 30));
    title->setPosition(0,snake->getPosition().y + snake->getContentSize().height+20);
    pMenu->addChild(title);
    
    //center the menu
    float heightofitems = snake->getBoundingBox().getMaxY() - mainMenu->getBoundingBox().getMinY();
    pMenu->setPosition(origin.x + windowSize.width/2,
                       origin.y + windowSize.height/2 - heightofitems/2);
    
    overlay->initWithColorMenu( Color4B(0,0,0,150), pMenu);
    addChild(overlay);
    
    
}

void TheBoxLayer::offSnake(Object* pSender)
{
    
    //trace back to get this
    OverLayer* overlay = (OverLayer*)((Node*)((Node*)pSender )->getParent() //MenuItemLabel->getParent()
                                      )->getParent(); //Menu->getParent()
    
    TheBoxLayer* thisLayer =(TheBoxLayer*)(overlay)->getParent();
    
    //kill overlay
    thisLayer->removeChild(overlay);
    
    thisLayer->setSnake(false);
    
    
}

void TheBoxLayer::onSnake(Object* pSender)
{

    //trace back to get this
    OverLayer* overlay = (OverLayer*)((Node*)((Node*)pSender )->getParent() //MenuItemLabel->getParent()
                                      )->getParent(); //Menu->getParent()

    TheBoxLayer* thisLayer =(TheBoxLayer*)(overlay)->getParent();
    
    //kill overlay
    thisLayer->removeChild(overlay);
    
    thisLayer->setSnake(true);
    
    
}

void TheBoxLayer::restoreHomeLayer(Object* pSender)
{
    HomeLayer* layer = HomeLayer::create();
    
    Director::getInstance()->replaceScene(layer->scene());
    layer->release();
    
}


//This will initialize and add the layer to the scene
void TheBoxLayer::runThisGame(Object* pSender)
{
    
    //init
    MyScene* scene = new MyScene();
    TheBoxLayer* layer = new TheBoxLayer();
    layer->initWithColor(Color4B(255,255,255,255));

    scene->runLayer(layer);
    
    //release
    scene->release();


}

