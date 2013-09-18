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

//#include "BoundLevel.h" BoundPlayer.h includes BoundLevel
#include "BoundPlayer.h"
#include "BoundMenu.h"


/**
 *  @brief  This is the Scene for Bound. It wll hold the layers that make up the Bound game.
 */
class BScene : public Scene
{
private:
    BLevel* levelLayer;
    BPlayer* playerLayer;
    LayerColor* backgroundLayer;
    BMenu* menuLayer;
    
    int layerFocus; //0 = main, 1 = level, 2 = game, 3 = pause
    
public:

    /**
     *  @brief  This will create and init a BScene
     */
    BScene();
    
    /**
     *  @brief  This will load a new level
     */
    void newLevel(const char* filepath);
    
    /**
     *  @brief  Show main Menu UNIMPLEMENTED
     */
    void showMainMenu();
    
    /**
     *  @brief  Show the level Menu
     */
    void showLevelMenu();
    
    /**
     *  @brief  Show the Pause Menu
     */
    void showPauseMenu();
    
    /**
     *  @brief  Loads the level stored in sender->levelname
     *          This is called from BMenu
     */
    static void loadLevel(Object* pSender);
    
    /**
     *  @brief  Exits this game and returns to the Home Screen
     */
    void exitGame();
    
    /**
     *  @brief  Handles keyPresses. Will handle Esc presses
     */
    void keyPressed(int keyCode);
    
    /**
     *  @brief  This method creates a new BScene and runs it
     */
    static void runThisGame(Object* pSender);
};

#endif /* defined(__GameBox__Bound__) */
