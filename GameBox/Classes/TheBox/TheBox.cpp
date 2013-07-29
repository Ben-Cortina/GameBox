//
//  TheBox.cpp
//  GameBox
//
//  Created by Ben Cortina on 7/25/13.
//
//

#include "TheBox.h"
#include <iostream>


// on "init" you need to initialize your instance
bool TheBoxLayer::init()
{
    visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    Texture2D* pixelTex = TextureCache::getInstance()->addImage("Pixel.png");
    texelRect.setRect(0, 0, visibleSize.height/20, visibleSize.height/20);
    pSprite = Sprite::createWithTexture(pixelTex, texelRect);
    pSprite->setColor(Color3B(100,100,100));
    // position the sprite on the center of the screen
    pSprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    
    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);
    scheduleUpdate();
    
    return true;
}

void TheBoxLayer::update(float dt)
{
    Color3B color = pSprite->getColor();
    color.b++;
    color.g--;
    color.r++;
    color.r++;
    if (color.r > 255) color.r = 0;
    if (color.b > 255) color.b = 0;
    if (color.g < 0) color.g = 255;
    pSprite->setColor(color);
}

void TheBoxLayer::runThisGame(Object* pSender)
{
    MyScene* scene = new MyScene();
    TheBoxLayer* layer = new TheBoxLayer();
    layer->initWithColor(Color4B(255, 255, 255, 255));
    scene->runLayer(layer);

}
