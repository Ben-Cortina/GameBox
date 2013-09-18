/**
 *  @file   BoundLevelMenu.h
 *  @author Ben Cortina
 *  @date   9/14/13.
 */

#ifndef __GameBox__BoundLevelMenu__
#define __GameBox__BoundLevelMenu__

#include "cocos2d.h"


USING_NS_CC;

class BLevelMenu : public Layer
{
public:
    BLevelMenu(std::function<void(Object*)> cb);
    
    const char* getLevelPath(const int idx);
    void closeCallback(Object * pSender);
    
    virtual void ccTouchesBegan(Set *pTouches, Event *pEvent);
    virtual void ccTouchesMoved(Set *pTouches, Event *pEvent);
    
private:
    Rect visRect;
    Point beginPos;
    Menu* itemMenu;
    std::function<void(Object*)> callback;
};
#endif /* defined(__GameBox__BoundLevelMenu__) */
