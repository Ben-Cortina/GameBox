/**
 *  @file   Bound.cpp
 *  @author Ben Cortina
 *  @date   8/29/13.
 */


#include "Bound.h"
#include "HomeScene.h"
#include <fstream>

BScene::BScene()
{
    init();
    layerFocus = 0;
    curLevel = 0;
    
    //add the keyboard handler
    keyHandler = new KeyboardHandler();
    keyHandler->addKeyPress(53, BScene::handleEsc, this, true);
    addChild(keyHandler);
    //keyHandler->release();
    
    //create background Layer
    backgroundLayer = new LayerColor();
    backgroundLayer->initWithColor(Color4B(150, 150, 150, 255));
    addChild(backgroundLayer,1);
    backgroundLayer->release();
    
    //create player Layer
    playerLayer = new BPlayer("Pixel.png", BScene::winCB);
    
    //load level Data
    int ldCount;
    LD * levellist = loadLevelDict(ldCount);
    
    //load menu
    menuLayer = new BLevelMenu(BScene::runLoadLevel, levellist, ldCount);
    
    //load the levelmenu
    showLevelMenu();
    
    keyHandler->setEnabled(true);
}

LD* BScene::loadLevelDict(int & cnt)
{
    std::string name;
    std::string path;
    cnt = 0;
    std::ifstream levelFile;
    
    std::string pathKey = FileUtils::getInstance()->fullPathForFilename("BoundLevels.txt");
    levelFile.open(pathKey.c_str());
    
    if (!levelFile)
        return 0;
    
    std::streampos start = levelFile.tellg();
    
    //count lines and check errors
    while (levelFile >> name && levelFile >> path)
    {
        if ( !( path.substr(path.length() - 4) == ".bdl") )
        {
            std::cout << "Level \"" << name << "\" does not have a valid filepath" << std::endl;
        } else
            cnt++;
    }
    //reset file
    levelFile.clear();
    levelFile.seekg(start);
    
    //init levels
    LD * levels = new LD[cnt];
    int idx = 0;
    
    //populate levels
    while (levelFile >> name && levelFile >> path && idx < cnt)
    {
        if ( path.substr(path.length() - 4) == ".bdl")
        {
            levels[idx].level_name = name;
            levels[idx].filepath = path;
            idx++;
        }
    }
    
    levelFile.close();
    
    std::cout << idx << " levels loaded "<<std::endl;
    
    return levels;
}

void BScene::newLevel(const char* filepath)
{
    Director::getInstance()->pause();
    
    //load the new one
    std::cout << "curLevel " <<curLevel<< std::endl;
    
    levelLayer = new BLevel(filepath);
    
    // if it loaded
    if (levelLayer->isValid())
    {
        //replace the level
        playerLayer->setLevel(levelLayer);

        //kill the menu and show the level
        addChild(levelLayer,2);
        addChild(playerLayer,3);
        
        levelLayer->release();
        layerFocus = 2;
    }
    
    Director::getInstance()->resume();
}

void BScene::loadLevel(const int idx)
{
    removeChild(menuLayer);
    curLevel = idx;
    newLevel(menuLayer->getLevelPath(idx));
}

void BScene::nextLevel()
{
    removeChild(levelLayer);
    removeChild(playerLayer);
    curLevel++;
    
    if (curLevel < menuLayer->getLevelCount())  //if there are no more levels
        newLevel(menuLayer->getLevelPath(curLevel));
    else
        addChild(menuLayer, 2);
}


void BScene::handleEsc(Object* scene)
{
    BScene* thisScene = (BScene*)scene;
    
    switch (thisScene->getFocus())
    {
        case 0:
            thisScene->exitGame();
            break;
        case 1:
            thisScene->showLevelMenu();
            break;
        case 2:
            thisScene->showPauseMenu();
            break;
            
        default:
            break;
    }
}

void BScene::showLevelMenu()
{
    addChild(menuLayer,1);
    if(layerFocus!=0)
    {
        levelLayer->cleanup();
        playerLayer->cleanup();
        removeChild(levelLayer);
        removeChild(playerLayer);
    }
    playerLayer->setKeyboardEnabled(false);
    
    layerFocus = 0;
}

void BScene::showPauseMenu()
{
    //pause game and input
    Director::getInstance()->pause();
    playerLayer->setKeyboardEnabled(false);
    
    Size windowSize = Director::getInstance()->getWinSize();
    
    OverLayer* overlay = new OverLayer();
    
    //create menu
    Menu* pMenu = Menu::create();
    
    //create menu items
    const Point origin = Director::getInstance()->getVisibleOrigin();
    
    MenuItemLabel* title;
    MenuItemLabel* exitGame;
    MenuItemLabel* levelSelect;
    MenuItemLabel* resume;
    
    //set up Main menu item
    resume = MenuItemLabel::create(LabelTTF::create("Resume", "Arial", 20),
                                   BScene::resumeCB);
    resume->setPosition(0,0);
    pMenu->addChild(resume);
    
    //set up Snake Mode item
    levelSelect = MenuItemLabel::create(LabelTTF::create("Level Select", "Arial", 20),
                                        BScene::levelSelectCB);
    levelSelect->setPosition(0, resume->getPosition().y + resume->getContentSize().height+10);
    pMenu->addChild(levelSelect);
    
    //set up Standard Mode item
    exitGame = MenuItemLabel::create(LabelTTF::create("Exit Game", "Arial", 20),
                                     BScene::exitGameCB);
    exitGame->setPosition(0, levelSelect->getPosition().y + levelSelect->getContentSize().height+10);
    pMenu->addChild(exitGame);
    
    //set up Title item
    title = MenuItemLabel::create(LabelTTF::create("Pause Menu", "Arial", 30));
    title->setPosition(0, exitGame->getPosition().y + exitGame->getContentSize().height+20);
    pMenu->addChild(title);
    
    //center the menu
    float heightofitems = exitGame->getBoundingBox().getMaxY() - resume->getBoundingBox().getMinY();
    pMenu->setPosition(origin.x + windowSize.width/2,
                       origin.y + windowSize.height/2 - heightofitems/2);
    
    overlay->initWithColorMenu( Color4B(0,0,0,150), pMenu);
    addChild(overlay,4);
    layerFocus = 1;
    
    overlay->release();
}

void BScene::resumeGame()
{
    layerFocus=2;
    playerLayer->setKeyboardEnabled(true);
}

void BScene::winCB(Object* pSender)
{
    BScene* thisScene = (BScene*)( ((BPlayer*)pSender)->getParent());
    
    thisScene->nextLevel();
}

void BScene::resumeCB(Object* pSender)
{
    //release overlay and resume
    //release pauseLayer and resume
    OverLayer* overlay = (OverLayer*)((Node*)((Node*)pSender )->getParent() //MenuItemLabel->getParent()
                                      )->getParent(); //Menu->getParent()
    BScene * thisScene = (BScene*)(overlay->getParent());
    
    thisScene->removeChild(overlay);
    thisScene->resumeGame();
    
    Director::getInstance()->resume();
}

void BScene::levelSelectCB(Object* pSender)
{
    //Return to level select
    OverLayer* overlay = (OverLayer*)((Node*)((Node*)pSender )->getParent() //MenuItemLabel->getParent()
                                      )->getParent(); //Menu->getParent()
    BScene * thisScene = (BScene*)(overlay->getParent());
    
    thisScene->removeChild(overlay);
    thisScene->showLevelMenu();
    
    Director::getInstance()->resume();
    
}

void BScene::exitGameCB(Object* pSender)
{
    //remove all children and replace with homelayer
    OverLayer* overlay = (OverLayer*)((Node*)((Node*)pSender )->getParent() //MenuItemLabel->getParent()
                                      )->getParent(); //Menu->getParent()
    BScene * thisScene = (BScene*)(overlay->getParent());
    
    thisScene->exitGame();
}

void BScene::runLoadLevel(Object * pSender)
{
	Director::getInstance()->purgeCachedData();
    
    // get the userdata, it's the index of the menu item clicked
    MenuItem* menuItem = (MenuItem *)(pSender);
    int idx = menuItem->getZOrder() - 10000;
    Menu* pmenu = (((Menu*)(menuItem->getParent())));
    BLevelMenu* menuL = (BLevelMenu*)(pmenu->getParent());
    BScene* thisScene = (BScene*)(menuL->getParent());
    
    // replace the old level
    thisScene->loadLevel(idx);
}

void BScene::exitGame()
{
    removeAllChildrenWithCleanup(true);
    
    HomeLayer* layer = HomeLayer::create();
    
    Director::getInstance()->replaceScene(layer->scene());
   
    layer->release();
}

void BScene::runThisGame(Object* pSender)
{
    //pause while loading
    Director::getInstance()->pause();
    
    //init
    BScene* scene = new BScene();
    
    //replace current scene
    CCDirector::getInstance()->replaceScene(scene);
    
    //resume when loaded
    Director::getInstance()->resume();
    
    //release
    scene->release();
}