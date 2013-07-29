/*--------------------------------------------------------------*
 *  TheBox.h                                                    *
 *  GameBox                                                     *
 *                                                              *
 *  Created by Ben Cortina on 7/25/13.                          *
 *                                                              *
 * This is a game that tests a players speed and reactions.     *
 * There is no win condition, the goal is to see how many boxs  *
 * the player can get out of.                                   *
 *                                                              *
 * The player controls a dot stuck in a box with one exit. As   *
 * time progresses, squares will randomly appear. These blocks  *
 * may block the players path. Additionally, if a player takes  *
 * too long to get to an exit, the location of that exit may    *
 * change.                                                      *
 *                                                              *
 * Features: Keyboard movement, BB collision detection.         *
 *--------------------------------------------------------------*/

#ifndef __GameBox___TheBoxScene__
#define __GameBox___TheBoxScene__


#include "../Helpers/CustomCC.h"

USING_NS_CC;

/**
 @class TheBoxLayer
 @brief This layer holds everythign needed for TheBox game
 */
class TheBoxLayer : public LayerColor
{
private:
    Rect texelRect;     // This is the size of each game pixel in terms of window pixels
    Size visibleSize;   // The size of the window
    Sprite* pSprite;
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    /**
     @brief     the update method, initially defined in Node, this is where the games time based actions are calculated.
                Once scheduleUpdate() has been called once, update will repeatedly.
     */
    void update(float dt);
    
    /**
     @brief     This method creates a new MyScene and adds a TheBoxLayer to it.
     */
    static void runThisGame(Object* pSender);
    
    // implement the "static create()" method
    CREATE_FUNC(TheBoxLayer);
};


#endif /* defined(__GameBox___TheBoxScene__) */
