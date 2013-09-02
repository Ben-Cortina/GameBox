/**-------------------------------------------------------------*
 * @file    Bound.h                                             *
 * @author  Ben Cortina                                         *
 * @date    8/29/13.                                            *
 *                                                              *
 * This is a cocos2d-x rendition of a popular Starcraft: Brood  *
 * War game mode. The objective is to get to the end of the     *
 * level without being caught in an "explosion." For accuracy   *
 * and simplicity, these explosions with be represented by a    *
 * flash in the tile.                                           *
 *                                                              *
 * Eventually I hope to add procedural generation and/or a      *
 * level creator to this game                                   *
 *                                                              *
 * Features: Keyboard movement, BB collision detection          *
 *--------------------------------------------------------------*/

#ifndef __GameBox__Bound__
#define __GameBox__Bound__

#include "BoundLevel.h"
#include "BoundPlayer.h"


/**
 *  @brief  This is the Scene for Bound. It wll hold the layers that make up the Bound game.
 */
class BScene : public Scene
{
private:
    BLevel* levelLayer;
    BPlayer* playerLayer;
    LayerColor* backgroundLayer;
public:

    /**
     *  @brief  This will create and init a BScene
     */
    BScene();
    
    /**
     *  @brief  This will load a new level
     */
    void newLevel();
    
    /**
     * @brief   This method creates a new BScene and runs it
     */
    static void runThisGame(Object* pSender);
};

#endif /* defined(__GameBox__Bound__) */
