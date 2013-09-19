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
    layerFocus = 1;
    curLevel = 0;
    
    //create background Layer
    backgroundLayer = LayerColor::create(Color4B(255, 255, 255, 255));
    addChild(backgroundLayer);
    
    //create player Layer
    playerLayer = new BPlayer("Pixel.png", BScene::handleEsc);
    
    //load level Data
    LD * levellist;
    int ldCount = loadLevelDict(levellist);
    
    //load menu
    menuLayer = new BLevelMenu(BScene::loadLevel, levellist, ldCount);
    
    menuLayer->setVisible(true);
    playerLayer->setVisible(false);
    playerLayer->setActive(false);
    
    //for now well load a level
    showLevelMenu();
    addChild(menuLayer, 1);
}

int BScene::loadLevelDict(LD * levels)
{
    std::string name;
    std::string path;
    int cnt = 0;
    std::ifstream levelFile;
    
    std::string pathKey = FileUtils::getInstance()->fullPathForFilename("BoundLevels.txt");
    levelFile.open(pathKey.c_str());
    
    if (!levelFile.is_open())
        return 0;
    
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
    
    //init levels
    levels = new LD[cnt];
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
    
    return cnt;
}

void BScene::newLevel(const char* filepath)
{
    Director::getInstance()->pause();
    
    //release the old level
    if (levelLayer)
        levelLayer->release();
    
    //load the new one
    levelLayer = new BLevel(filepath);
    
    // if it loaded
    if (levelLayer->isValid())
    {
        removeAllChildren();
        //replace the level
        playerLayer->setLevel(levelLayer);

        //kill the menu and show the level
        addChild(levelLayer,1);
        addChild(playerLayer,1);
    }
    
    Director::getInstance()->resume();
}

void BScene::handleEsc(Object* scene)
{
    BScene* thisScene = (BScene*)scene;
    
    switch (thisScene->getFocus())
    {
        case 0:
            thisScene->exitGame(thisScene);
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
    removeAllChildren();
    
    addChild(menuLayer);
}

void BScene::showPauseMenu()
{
    //pause game and input
    Director::getInstance()->pause();
    playerLayer->setTouchEnabled(false);
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
    addChild(overlay);
}

void BScene::loadLevel(Object * pSender)
{
	Director::getInstance()->purgeCachedData();
    
    // get the userdata, it's the index of the menu item clicked
    MenuItem* menuItem = (MenuItem *)(pSender);
    int idx = menuItem->getZOrder() - 10000;
    Menu* pmenu = (((Menu*)(menuItem->getParent())));
    BLevelMenu* menuL = (BLevelMenu*)(pmenu->getParent());
    BScene* thisScene = (BScene*)(menuL->getParent());
    
    // replace the old level
    thisScene->newLevel(menuL->getLevelPath(idx));
}

void BScene::exitGame(Object* scene)
{
    HomeLayer* layer = HomeLayer::create();
    
    Director::getInstance()->replaceScene(layer->scene());
   
    scene->release();
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