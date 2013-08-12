//
//  TheBox.cpp
//  GameBox
//
//  Created by Ben Cortina on 7/25/13.
//
//

#include "TheBox.h"
#include <iostream>

#define TILE_ON Color3B(0,0,0)
#define TILE_OFF Color3B(255,255,255)

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

void TheBoxLayer::initPlayer()
{
    player = Player::create("Ball.png");
    
    // we want it to be about 1/20th of the grid
    float winSize = MIN(windowSize.width, windowSize.height);
    
    // winSize/20 will be 1/20th of the window in pixels. Divide this by the sprites size to
    // adjust the scale to match the image
    float scale = (winSize/40.0)/player->boundingBox().size.height;

    player->setScale(scale);
    player->setPosition(Point(windowSize.width/2,
                              windowSize.height/2));
    player->setColor(Color3B(100,200,0));
    
    this->addChild(player, 1);
}

// on "init" you need to initialize your instance
bool TheBoxLayer::init()
{    
    windowSize = Director::getInstance()->getVisibleSize();
    
    initTiles();
    
    setLayerSize(10, 10);
    
    initPlayer();
    
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
    int rand_r = random() % 255;
    int rand_g = random() % 255;
    int rand_b = random() % 255;
    //Color3B color = sTiles[rand_y * (int)layerSize.width + rand_x]->getColor();
    //sTiles[rand_y * (int)layerSize.width + rand_x]->setColor(Color3B(rand_r,rand_g,rand_b));
    /*if (color.r == 0)
        sTiles[rand_y * (int)layerSize.width + rand_x]->setColor(Color3B(255,255,255));
    else
        sTiles[rand_y * (int)layerSize.width + rand_x]->setColor(Color3B(0,0,0));*/
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
            sTiles[y * THEBOX_MAX_WIDTH + x]->setPosition(Point(x * tileSize.width + tileSize.width/2 + margins.width,
                                                                y * tileSize.height + tileSize.height/2 + margins.height
                                                                )
                                                          );
            // update the size of the sprite
            sTiles[y * THEBOX_MAX_WIDTH + x]->setScale(tileSize.width);
            // set each tiles color to white
            sTiles[y * THEBOX_MAX_WIDTH + x]->setColor(TILE_OFF);
        }
}

void TheBoxLayer::setLayerSize(int width, int height)
{
    layerSize = Size(width, height);
    float minSize = MIN(windowSize.width, windowSize.height);
    tileSize = Size(minSize / layerSize.width, minSize / layerSize.height);
    updateTiles();
    updateBorder();
}

void TheBoxLayer::updateBorder()
{
    for (int x = 0; x < layerSize.width; x++)
    {
        sTiles[x]->setColor(TILE_ON);
        sTiles[((int)layerSize.height-1) * THEBOX_MAX_WIDTH + x]->setColor(TILE_ON);
    }
    for (int y = 0; y < layerSize.height; y++)
    {
        sTiles[y * THEBOX_MAX_WIDTH]->setColor(TILE_ON);
        sTiles[y * THEBOX_MAX_WIDTH + ((int)layerSize.width-1)]->setColor(TILE_ON);
    }
}

void TheBoxLayer::runThisGame(Object* pSender)
{
    MyScene* scene = new MyScene();
    TheBoxLayer* layer = new TheBoxLayer();
    layer->initWithColor(Color4B(255, 255, 255, 255));
    scene->runLayer(layer);

}
