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
 * Features: Keyboard movement                                  *
 *--------------------------------------------------------------*/

// Welp, this game used to have a circular player with pixel perfect collision and momentum
// but I did not find it nearly as fun so I made it a LOT simpler yet more enjoyable.
// Helpers/CustomCC has some of the removed code.

// Ill come back to this and add a snake like version where you leave a trail

#ifndef __GameBox___TheBoxScene__
#define __GameBox___TheBoxScene__

#define THEBOX_MAX_WIDTH 100
#define THEBOX_MAX_HEIGHT 100 
#define TIME_PER_SPAWN 1.0f
#define TIME_INC_PER_LEVEL 0.5f


#include "../Helpers/CustomCC.h"
#include "PlayerSprite.h"

USING_NS_CC;



struct Coords {
    int x;
    int y;
    Coords(){};
    Coords(int px, int py):x(px),y(py){};
    
    inline bool operator== (const Coords r) {return (x == r.x && y == r.y);};
    inline Coords& operator= (const Coords r) {x = r.x; y = r.y; return *this;};
    inline Coords& operator+= (const Coords r) {(*this) = (*this) + r; return *this;};
    Coords operator+ (Coords r) {r.x = x + r.x; r.y = y + r.y; return r;};
    inline Coords& operator-= (const Coords r) {(*this) = (*this) - r; return *this;};
    Coords operator- (Coords r) {r.x = x - r.x; r.y = y - r.y; return r;};
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
    Coords layerSize;   // The size of the screen in terms of tiles
    
    Sprite* sTiles[THEBOX_MAX_HEIGHT * THEBOX_MAX_WIDTH]; //array holding all the sprites
    Color3B tileColor; // The defualt color of a tile;
    
    Coords player;   // The location of the player
    Color3B playerColor; // the color of the player
    
    Coords exit; // where the exit is
    
    float timeRemaining;
    float timePerLevel;
    LabelTTF * timeLabel;
    
    /**
     @brief     Creates, sizes, and adds all the sprites
     */
    void initTiles();
    
    /**
     @brief     initializes the player
     */
    void initPlayer();
    
    /**
     @brief     This method sets up the next level of the game.
     */
    void nextLevel();
    
    /**
     @brief Updates the size and position of each Sprite
     */
    void updateTiles();
    
    /**
     @brief     Check the victory and defeat condition
     */
    void checkConditions();
    
    /**
     @brief     Changes the coordinates of the exit
     */
    void changeExit(float dt);
    
    /**
     @brief     Turns on a tile in a random location
     */
    void spawnTile(float dt);
    
    /**
     @brief Updates the screenSize and tileSize then calls updateTiles
     */
    void setLayerSize(int x, int y);
    
    /**
     @brief     Changes the appearence of the passed sprite to represent the players
     *          presence or lack there of.
     @param tile    the Sprite in question
     @param set     if its the player or not
     */
    void setPlayer(Sprite * tile, bool set);
    
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
     @brief     This is a function to catch KeyPress events sent from KeyboardDispatcher
     @param  KeyCode    The keycode of the pressed key
     */
    void keyPressed(int keyCode);
    
    /**
     @brief     Checks if dx, dy from the player is on. 
     @param dx  x change from the player
     @param dy  y change from player
     */
    bool checkPlayerCollision(int dx, int dy);
    
    
    /**
     @brief     This method creates a new MyScene and adds a TheBoxLayer to it.
     */
    static void runThisGame(Object* pSender);
    
    
    // implement the "static create()" method
    CREATE_FUNC(TheBoxLayer);
};

#endif /* defined(__GameBox___TheBoxScene__) */
