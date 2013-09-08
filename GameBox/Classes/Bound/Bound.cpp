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
    backgroundLayer = LayerColor::create(Color4B(20, 20, 20, 255));
    
    //create player Layer
    playerLayer = new BPlayer("Pixel.png");
    
    //TODO:load menu
    //for now well load a level
    newLevel("Level01.bdl");
}

void BScene::newLevel(const char* file)
{
    levelLayer = new BLevel(file);
    playerLayer->setLevel(levelLayer);
}

void BScene::runThisGame(Object* pSender)
{
    //init
    BScene* scene = new BScene();
    
    //replace current scene
    CCDirector::getInstance()->replaceScene(scene);
    
    //release
    scene->release();
}