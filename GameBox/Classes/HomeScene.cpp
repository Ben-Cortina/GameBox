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

// on "init" you need to initialize your instance
bool HomeLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.
    // add a "close" icon to exit the progress. it's an autorelease object
    MyMenuItemImage *pCloseItem = MyMenuItemImage::create("CloseNormal.png",
                                                          "CloseSelected.png",
                                                          g_GameList[0].callback
                                                          );
    
	pCloseItem->setPosition(Point(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                  origin.y + pCloseItem->getContentSize().height/2
                                  )
                            );
    
    // create menu, it's an autorelease object
    Menu* pMenu = Menu::create(pCloseItem, NULL);
    pMenu->setPosition(Point::ZERO);
    this->addChild(pMenu, 1);
    
    /////////////////////////////
    // 3. add your codes below...
    // add a label shows "Hello World"
    // create and initialize a label
    
    LabelTTF* pLabel = LabelTTF::create("Hello World", "Arial", 24);
    
    // position the label on the center of the screen
    pLabel->setPosition(Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));

    // add the label as a child to this layer
    addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
    Sprite* pSprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    pSprite->setPosition(Point(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    addChild(pSprite, 0);

    setKeyboardEnabled(true);
    
    return true;
}

void HomeLayer::keyPressed(int keyCode)
{
    std::cout << "this happend";   
}

void HomeLayer::menuCloseCallback(Object* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
