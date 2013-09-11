/**
 *  @file   Bound.cpp
 *  @author Ben Cortina
 *  @date   8/29/13.
 */


#include "Bound.h"

BScene::BScene()
{
    init();
    //create background Layer
    backgroundLayer = LayerColor::create(Color4B(255, 255, 255, 255));
    addChild(backgroundLayer);
    
    //create player Layer
    playerLayer = new BPlayer("Pixel.png");
    
    //TODO:load menu
    //for now well load a level
    newLevel("LevelMaking.bdl");
    addChild(levelLayer,1);
    addChild(playerLayer,1);
}

void BScene::newLevel(const char* filepath)
{
    levelLayer = new BLevel(filepath);
    playerLayer->setLevel(levelLayer);
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