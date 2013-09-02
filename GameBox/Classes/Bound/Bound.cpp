/**
 *  @file   Bound.cpp
 *  @author Ben Cortina
 *  @date   8/29/13.
 */


#include "Bound.h"

BScene::BScene()
{
    //create background Layer
    
    
    //create player Layer
}

void BScene::runThisGame(Object* pSender)
{
    //init
    BScene* scene = new BScene();
    
    //create background Layer
    
    
    //create player Layer

    
    //replace current scene
    CCDirector::getInstance()->replaceScene(scene);
    
    //release
    scene->release();
}