/**
 *  @file   Bound.cpp
 *  @author Ben Cortina
 *  @date   8/29/13.
 */


#include "Bound.h"

BScene::BScene()
{
    init();
    layerFocus = 1;
    
    //create background Layer
    backgroundLayer = LayerColor::create(Color4B(255, 255, 255, 255));
    addChild(backgroundLayer);
    
    //create player Layer
    playerLayer = new BPlayer("Pixel.png");
    
    //load menu
    menuLayer = new BMenu(BScene::loadLevel);
    
    menuLayer->setVisible(true);
    playerLayer->setVisible(false);
    playerLayer->setActive(false);
    
    //for now well load a level
    newLevel("LevelMaking.bdl");
    addChild(levelLayer,1);
    addChild(playerLayer,1);
}

void BScene::newLevel(const char* filepath)
{
    Director::getInstance()->pause();
    
    
    //release the old level
    if (levelLayer)
        levelLayer->release();
    
    //load the new one
    levelLayer = new BLevel(filepath);
    
    //replace the level
    playerLayer->setLevel(levelLayer);
    
    //hide the menu and show the level
    menuLayer->setVisible(false);
    playerLayer->setVisible(true);
    levelLayer->setVisible(true);
    playerLayer->setActive(true);
    
    
    Director::getInstance()->resume();
}

void BScene::loadLevel(Object * pSender)
{
	Director::getInstance()->purgeCachedData();
    
    // get the userdata, it's the index of the menu item clicked
    MenuItem* menuItem = (MenuItem *)(pSender);
    int idx = menuItem->getZOrder() - 10000;
    Menu* pmenu = (((Menu*)(menuItem->getParent())));
    BMenu* menuL = (BMenu*)(pmenu->getParent());
    BScene* thisScene = (BScene*)(menuL->getParent());
    
    // replace the old level
    thisScene->newLevel(menuL->getLevelPath(idx));
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