/**
 *  @file   BoundLevelMenu.cpp
 *  @author Ben Cortina
 *  @date   9/14/13.
 */

#include "BoundLevelMenu.h"


struct {
	const char *level_name;
	const char *filepath;
} levelData[] = {
	{ "Level Making", "LevelMaking.bdl" }
};

static int levelCount = sizeof(levelData) / sizeof(levelData[0]);

#define LINE_SPACE          40

static Point s_tCurPos = Point::ZERO;

BLevelMenu::BLevelMenu(std::function<void(Object*)> cb)
: beginPos(Point::ZERO), callback(cb)
{
    Size visSize = Director::getInstance()->getVisibleSize();
    Point visOrigin = Director::getInstance()->getVisibleOrigin();
    visRect = Rect(visOrigin.x, visOrigin.y, visSize.width, visSize.height);
    
    // add menu items for tests
    itemMenu = Menu::create();
    for (int i = 0; i < levelCount; ++i)
    {
        // #if (CC_TARGET_PLATFORM == CC_PLATFORM_MARMALADE)
        //         LabelBMFont* label = LabelBMFont::create(g_aTestNames[i].c_str(),  "fonts/arial16.fnt");
        // #else
        LabelTTF* label = LabelTTF::create( levelData[i].level_name, "Arial", 24);
        // #endif
        MenuItemLabel* menuItem = MenuItemLabel::create(label, cb);
        
        itemMenu->addChild(menuItem, i + 10000);
        menuItem->setPosition( Point( visRect.getMidX(), (visRect.getMaxY() - (i + 1) * LINE_SPACE) ));
    }
    
    itemMenu->setContentSize(Size(visRect.size.width, (levelCount + 1) * (LINE_SPACE)));
    itemMenu->setPosition(s_tCurPos);
    addChild(itemMenu);
    
    setTouchEnabled(true);
    
}

const char* BLevelMenu::getLevelPath(const int idx)
{
    return levelData[idx].filepath;
}

void BLevelMenu::closeCallback(Object * pSender)
{
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void BLevelMenu::ccTouchesBegan(Set *pTouches, Event *pEvent)
{
    Touch* touch = (Touch*)pTouches->anyObject();
    
    beginPos = touch->getLocation();
}

void BLevelMenu::ccTouchesMoved(Set *pTouches, Event *pEvent)
{
    Touch* touch = (Touch*)pTouches->anyObject();
    
    Point touchLocation = touch->getLocation();
    float nMoveY = touchLocation.y - beginPos.y;
    
    Point curPos  = itemMenu->getPosition();
    Point nextPos = Point(curPos.x, curPos.y + nMoveY);
    
    if (nextPos.y < 0.0f)
    {
        itemMenu->setPosition(Point::ZERO);
        return;
    }
    
    if (nextPos.y > ((levelCount + 1)* LINE_SPACE - visRect.size.height))
    {
        itemMenu->setPosition(Point(0, ((levelCount + 1)* LINE_SPACE - visRect.size.height)));
        return;
    }
    
    itemMenu->setPosition(nextPos);
    beginPos = touchLocation;
    s_tCurPos   = nextPos;
}
