/**-------------------------------------------------------------*
 *  @file   Bound.h                                             *
 *  @author Ben Cortina                                         *
 *  @date   8/29/13.                                            *
 *                                                              *
 * This is a cocos2d-x rendition of a popular Starcraft: Brood  *
 * War game mode. The objective is to get to the end of the     *
 * level without being caught in an "explosion." For accuracy   *
 * and simplicity, these explosions with be represented by a    *
 * flash in the tile.                                           *
 *                                                              *
 * Eventually I hope to add procedural generation and/or a      *
 * level creator to this game                                   *
 * In the meantime, if you want to make levels refer to         *
 * LevelMaking.bdl on how to create a level then add it to the  *
 * list in BoundLevels.txt                                      *
 *                                                              *
 * Features: Keyboard movement, BB collision detection, Sound,  *
 *           Scheduling and callbacks                           *
 *--------------------------------------------------------------*/

#ifndef __GameBox__Bound__
#define __GameBox__Bound__

//#include "BoundLevel.h" BoundPlayer.h includes BoundLevel
#include "BoundPlayer.h"
#include "BoundLevelMenu.h"
#include "KeyboardHandler.h"
#include "OverLayer.h"

/** @brief  This is the Scene for Bound. It wll hold the layers that make up the Bound game. */
class BScene : public Scene
{
private:
    KeyboardHandler* keyHandler;
    BLevel* levelLayer;
    BPlayer* playerLayer;
    LayerColor* backgroundLayer;
    BLevelMenu* menuLayer;
    OverLayer* overlay;
    
    int layerFocus; //0 = level, 1 = pause, 2 = game
    int curLevel;
    
public:

    /** @brief  This will create and init a BScene */
    BScene();
    
    /** @brief  This will read all the levels from the level dictionary and populate 'levels' */
    LD* loadLevelDict(int & cnt);
    
    /** @brief  Returns the current focus of the game */
    int getFocus(){return layerFocus;};
    
    /** @brief  Sets the value for layerFocus   */
    void setFocus(const int f){layerFocus = f;};
    
    /** @brief  This will load a new level */
    void newLevel(const char* filepath);
    
    /** @brief  This will load a level from the level menu */
    void loadLevel(const int idx);
    
    /** @brief  This will load the next level from a current level */
    void nextLevel();
    
    /** @brief  This will handle EscPresses */
    static void handleEsc(Object * scene);
    
    /** @brief  Show the level Menu */
    void showLevelMenu();
    
    /** @brief  Show the Pause Menu */
    void showPauseMenu();
    
    /** @brief  Resume the game */
    void resumeGame();
    
    //win callback
    static void winCB(Object* pSender);
    
    //menu callbacks
    static void resumeCB(Object* pSender);
    static void levelSelectCB(Object* pSender);
    static void exitGameCB(Object* pSender);
    
    /**
     *  @brief  Loads the level stored in sender->levelname
     *          This is called from BLevelMenu
     */
    static void runLoadLevel(Object* pSender);
    
    /** @brief  Exits this game and returns to the Home Screen */
    void exitGame();
    
    /** @brief  This method creates a new BScene and runs it */
    static void runThisGame(Object* pSender);
};

#endif /* defined(__GameBox__Bound__) */
