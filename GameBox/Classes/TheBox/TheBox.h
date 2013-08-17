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

#define THEBOX_MAX_WIDTH 100
#define THEBOX_MAX_HEIGHT 100 


#include "../Helpers/CustomCC.h"
#include "PlayerSprite.h"

USING_NS_CC;



struct BoxCommand
{
    float pressed,
          released;
    BoxCommand();
};

/**
 @brief This will store the time each command was issued
 *      Each time will be set on keypresses and unset on key releases
 */
struct CommandTimes
{
    BoxCommand Up,
          Down,
          Left,
          Right;
};

/**
 @class TheBoxLayer
 @brief This layer holds everythign needed for TheBox game
        Im not using any TMX feature because they dont allow me
        its features and my needs dont line up as well as I'd like
 */
class TheBoxLayer : public Layer
{
private:
    Size tileSize;     // This is the size of each game pixel in terms of window pixels
    Size windowSize;   // The size of the window in terms of pixels
    Size layerSize;   // The size of the screen in terms of tiles
    Sprite* sTiles[THEBOX_MAX_HEIGHT * THEBOX_MAX_WIDTH]; //array holding all the sprites
    PlayerSprite* player;   // A sprite class to handle the players sprite
    CommandTimes commandTime; // A struct containing all the dt's of important key events
    bool keyChange; // A simple bool that will be set to true every time a key event is triggered
    
    /**
     @brief     Creates, sizes, and adds all the sprites
     */
    void initTiles();
    
    /**
     @brief     initializes the player
     */
    void initPlayer();
    
    /**
     @brief Updates the size and position of each Sprite
     */
    void updateTiles();
    
    /**
     @brief Updates the screenSize and tileSize then calls updateTiles
     */
    void setLayerSize(int width, int height);
    
    /**
     @brief Sets the outer tiles of the layerSize.X by layerSize.y box to black
     */
    void updateBorder();

public:

    TheBoxLayer();
    //~TheBoxLayer();

    /**
     @brief     Initialize everything needed for this Layer
     *          Once scheduleUpdate() has been called once, update will repeatedly.
     */
    virtual bool init();
    
    
    /**
     @brief     the update method, initially defined in Node, this is where the games time based actions are calculated.
     *          Once scheduleUpdate() has been called once, update will repeatedly.
     */
    void update(float dt);

    /**
     @brief      This is a functions to catch KeyPress events sent from KeyboardDispatcher
     @param  KeyCode    The keycode of the pressed key
     */
    void keyPressed(int keyCode);

    /**
     @brief      This is a functions to catch KeyRelease events sent from KeyboardDispatcher
     @param KeyCode     The keycode of the released key
     */
    void keyReleased(int keyCode);
    
    /**
     @brief      This handles the results of key changes that occured since the last update
     @param dt   The time since last update
     */
    void keyHandling(float dt);

    /**
     @brief     This method creates a new MyScene and adds a TheBoxLayer to it.
     */
    static void runThisGame(Object* pSender);
    
    // implement the "static create()" method
    CREATE_FUNC(TheBoxLayer);
};

#endif /* defined(__GameBox___TheBoxScene__) */
