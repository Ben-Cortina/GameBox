/**
 *  @file   BoundLevelMenu.h
 *  @author Ben Cortina
 *  @date   9/14/13.
 */

#ifndef __GameBox__BoundLevelMenu__
#define __GameBox__BoundLevelMenu__

#include "cocos2d.h"


USING_NS_CC;

struct LD{
    std::string level_name;
    std::string filepath;
};

class BLevelMenu : public Layer
{
public:
    BLevelMenu(std::function<void(Object*)> cb, LD levels[], int ldCount);
    ~BLevelMenu();
    
    /** @brief  Gets the path of the file at the given index */
    const char* getLevelPath(const int idx);
    
    /** @brief  Gets the number of levels avaliable */
    int getLevelCount() { return levelCount; };
    
    /** @brief  Handles clicks/touches */
    virtual void ccTouchesBegan(Set *pTouches, Event *pEvent);
    
    /** @brief  Handles click/touch drags */
    virtual void ccTouchesMoved(Set *pTouches, Event *pEvent);
    
private:
    Rect visRect;
    Point beginPos;
    Menu* itemMenu;
    std::function<void(Object*)> callback;
    LD *levelData;
    int levelCount;
};
#endif /* defined(__GameBox__BoundLevelMenu__) */
