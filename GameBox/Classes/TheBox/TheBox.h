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

#define THEBOX_MAX_WIDTH 20
#define THEBOX_MAX_HEIGHT 20 


#include "../Helpers/CustomCC.h"
#include "PlayerSprite.h"

USING_NS_CC;


/**
 @brief This will store the state of each command
 *      Each state will be set on keypresses and unset on key releases
 */
struct CommandState
{
    bool Up,
         Down,
         Left,
         Right;
    CommandState();
};

/**
 @class TheBoxLayer
 @brief This layer holds everythign needed for TheBox game
        Im not using any TMX feature because they dont allow me
        its features and my needs dont line up as well as I'd like
 */
class TheBoxLayer : public LayerColor
{
private:
    Size tileSize;     // This is the size of each game pixel in terms of window pixels
    Size windowSize;   // The size of the window in terms of pixels
    Size layerSize;   // The size of the screen in terms of tiles
    Sprite* sTiles[THEBOX_MAX_HEIGHT * THEBOX_MAX_WIDTH]; //array holding all the sprites
    PlayerSprite* player;   // A sprite class to handle the players sprite
    CommandState commandStates; // A struct containing all the states of important commands
    
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
     @param dt  The time since update was last called
     */
    void update(float dt);
    
    /**
     @brief     Calls the player update method and handles any collisions.
     */
    void updatePlayer(float dt);

    /**
     @brief     This is a function to catch KeyPress events sent from KeyboardDispatcher
     @param  KeyCode    The keycode of the pressed key
     */
    void keyPressed(int keyCode);

    /**
     @brief     This is a function to catch KeyRelease events sent from KeyboardDispatcher
     @param KeyCode     The keycode of the released key
     */
    void keyReleased(int keyCode);
    
    /**
     @brief     This checks for and handles collisions and movement for the player. 
     *          There are an incredible number of ways to handle collision detection and using the most 
     *          efficient yet accurate method for you game is important for performance. See the TheBox.cpp
     *          handleCollisions() for more details.
     *          This collision detection method is specific to checking collisions of a moving circle and 
     *          stationary rectangles. I went for excessive accuracy in this to give me a good challenge.
     *          Really all this game needs is a simple bounding box collision detection.
     @param pos The position of the object
     @param x   The x change of the object
     @param y   The y change of the object
     @return    Whether or not there was a collision
     */
    void handlePlayerCollisions(Point &pos, float x, float y, float radius);
    
    /**
     @brief     This method creates a new MyScene and adds a TheBoxLayer to it.
     */
    static void runThisGame(Object* pSender);
    
    // implement the "static create()" method
    CREATE_FUNC(TheBoxLayer);
};

#endif /* defined(__GameBox___TheBoxScene__) */
