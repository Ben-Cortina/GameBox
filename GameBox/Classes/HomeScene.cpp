#include "HomeScene.h"
#include <iostream>

USING_NS_CC;

HomeLayer::HomeLayer()
{
    // This will initialize all the menu sprites when we get around to creating them
}

HomeLayer::~HomeLayer()
{
	// unset this so we unregister with the touch dispatcher
	//this->setTouchEnabled(false);
}

Scene* HomeLayer::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    HomeLayer *layer = HomeLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void HomeLayer::addMenuItems()
{
    
    const Size visibleSize = Director::getInstance()->getVisibleSize();
    const Point origin = Director::getInstance()->getVisibleOrigin();
    
    Point nextLoc;
    nextLoc.x = visibleSize.width * 0.05;
    nextLoc.y = visibleSize.height * 0.95;
    const Point spacing = visibleSize * 0.05;
    
    //create the menu
    Menu* pMenu = Menu::create();
    MyMenuItemImage *pMenuItem;
    
    for (int i = 0; i < GAME_COUNT; i++) {
        //  Add a menu item with an image, which can be clicked to open a game
        pMenuItem = MyMenuItemImage::create(g_GameList[i].image,
                                            g_GameList[i].callback
                                            );
        
        //if the window is too small well need to scale the item down
        if (pMenuItem->getContentSize().height > (visibleSize.height * 0.9)) 
            pMenuItem->setScale( (visibleSize.height * 0.9) / pMenuItem->getContentSize().height );
        
        // if we have passed the bottom move over to the right
        if (nextLoc.y - pMenuItem->getBoundingBox().size.height < visibleSize.height * 0.05)
        {
            nextLoc.x += spacing.x + pMenuItem->getBoundingBox().size.width;
            nextLoc.y = visibleSize.height * 0.95;
        }
        
        //set the position
        pMenuItem->setPosition(Point(origin.x + nextLoc.x + pMenuItem->getBoundingBox().size.width / 2 ,
                                     origin.y + nextLoc.y - pMenuItem->getBoundingBox().size.height / 2
                                     )
                                );
        
        // set the next location
        nextLoc.y -= spacing.y + pMenuItem->getBoundingBox().size.height;
        
        //add the item
        pMenu->addChild(pMenuItem);
    }
    
    //add the menu to the layer
    pMenu->setPosition(Point::ZERO);
    addChild(pMenu);
    
}

// on "init" you need to initialize your instance
bool HomeLayer::init()
{
    //clear the cache
    Director::getInstance()->purgeCachedData();
    
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    //add the buttons
    addMenuItems();
    
    /////////////////////////////
    // 3. add your codes below...
    // add a label shows "Hello World"
    // create and initialize a label
    
    LabelTTF* pLabel = LabelTTF::create("Game Box", "Arial", 24);
    
    // position the label on the center of the screen
    pLabel->setPosition(Point(origin.x + visibleSize.width/2,
                              origin.y + visibleSize.height - pLabel->getContentSize().height/2));
    
    // add the label as a child to this layer
    addChild(pLabel, 1);
    
    return true;
}

void HomeLayer::menuCloseCallback(Object* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
