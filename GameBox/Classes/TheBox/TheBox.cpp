//
//  TheBox.cpp
//  GameBox
//
//  Created by Ben Cortina on 7/25/13.
//
//

#include "TheBox.h"
#include <iostream>

void TheBoxLayer::initTiles()
{
    //load the tile texture
    Texture2D* pixelTex = TextureCache::getInstance()->addImage("Pixel.png");
    
    for (int i = 0; i < THEBOX_MAX_HEIGHT * THEBOX_MAX_WIDTH; i++)
    {
        //create the sprite and add it
        sTiles[i] = Sprite::createWithTexture(pixelTex, Rect(0,0,1,1));
        this->addChild(sTiles[i], 0);
    }
}

// on "init" you need to initialize your instance
bool TheBoxLayer::init()
{    
    initTiles();
    setLayerSize(10, 10);
    // add the sprite as a child to this layer
    scheduleUpdate();
    
    return true;
}

void TheBoxLayer::update(float dt)
{
    
    /*color.b +=100*dt;
    color.g -=100*dt;
    color.r +=200*dt;
    if (color.r > 255) color.r = 0;
    if (color.b > 255) color.b = 0;
    if (color.g < 0) color.g = 255;*/
    int rand_x = random() % (int)layerSize.width;
    int rand_y = random() % (int)layerSize.height;
    Color3B color = sTiles[rand_y * (int)layerSize.width + rand_x]->getColor();
    if (color.r == 0)
        sTiles[rand_y * (int)layerSize.width + rand_x]->setColor(Color3B(255,255,255));
    else
        sTiles[rand_y * (int)layerSize.width + rand_x]->setColor(Color3B(0,0,0));
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
            sTiles[y * (int)layerSize.width + x]->setPosition(Point(x * tileSize.width + tileSize.width/2 + margins.width,
                                                                y * tileSize.height + tileSize.height/2 + margins.height
                                                                )
                                                          );
            // update the size of the sprite
            sTiles[y * (int)layerSize.width + x]->setScale(tileSize.width);
        }
}

void TheBoxLayer::setLayerSize(int width, int height)
{
    windowSize = Director::getInstance()->getVisibleSize();
    layerSize = Size(width, height);
    float minSize = MIN(windowSize.width, windowSize.height);
    tileSize = Size(minSize / layerSize.width, minSize / layerSize.height);
    updateTiles();
}

void TheBoxLayer::runThisGame(Object* pSender)
{
    MyScene* scene = new MyScene();
    TheBoxLayer* layer = new TheBoxLayer();
    layer->initWithColor(Color4B(255, 255, 255, 255));
    scene->runLayer(layer);

}
