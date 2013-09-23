/**
 * @file    BoundPlayer.h
 * @author  Ben Cortina
 * @date    9/2/13.
 */


#ifndef __GameBox__BoundPlayer__
#define __GameBox__BoundPlayer__

#include "cocos2d.h"
#include "BoundLevel.h" //I'm routing this trhough here so I can call collisio detection

USING_NS_CC;

struct Vector
{
    float x; 
    float y; 
};

struct KeyStateStruct
{
    bool up;
    bool down;
    bool left;
    bool right;
};

/**
 *  @class  BPlayer
 *  @brief  This will hold and draw the data for a Player. It will also handle player movement.
 */
class BPlayer : public Layer
{
private:
    //last update
    timeval lastUpdate;
    float dtCalculated;
    
    BLevel* level;
    Sprite* player;
    
    Color3B playerColor;
    
    float maxSpeed;
    Vector force;
    Vector velocity;
    
    KeyStateStruct keyState;
    bool isDying;

    std::function<void(Object*)> winFunc; //this is the function to call when the player wins
    
public:
    BPlayer(const char* image, std::function<void(Object*)> winF);
    
    /**
     *  @brief  Set the level to check collision on
     */
    void setLevel(BLevel* newLevel);
    
    /**
     *  @brief  Set the max speed of the player
     */
    void setMaxSpeed(const float speed){ maxSpeed = speed; };
    
    /**
     *  @brief  Get/Set methods for force.
     */
    Vector getForce() {return force;};
    void setForce(const Vector pforce){ force.x = pforce.x; force.y = pforce.y; updateVelocity(); };
    void setForceX(const float x) { force.x = x; updateVelocity(); };
    void setForceY(const float y) { force.y = y; updateVelocity(); };
    
    
    /** @brief  runs the actions needed to represent a falling player   */
    void fall();
    
    /** @brief  runs the actions needed to represent a dying player */
    void die();
    
    /** @brief  runs the actions needed to represent a win */
    void win();
    
    /** @brief  pauses all movement commands */
    void stopMovement();
    
    /** @brief  Spawns the player at the start tile */
    void spawn(float dt);
    
    
    /** @brief  Updates the current velocity based on the force being applied */
    void updateVelocity();
    
    /**
     @brief     Updates the position of the Sprite based on dt and its current velocity
     @param dt       The time over which the velocity has been applied
     */
    void update(float dt);
    
    
    /**
     @brief     Updates the position of the Sprite based on the currenttime - lastupdate
     * Ths allows me to call update whenever a keychanges so the movement is calculated based on
     * the time of the key event rather than the time of each update.
     */
    void update();
    
    /** @brief  Handles the collision and movement of the player */
    void handleCollisions(float x, float y);
    
    /** Press and release handling */
    static void upPCB(Object* pThis) { BPlayer* t = (BPlayer*)pThis; t->up(true);};
    static void downPCB(Object* pThis) { BPlayer* t = (BPlayer*)pThis; t->down(true);};
    static void leftPCB(Object* pThis) { BPlayer* t = (BPlayer*)pThis; t->left(true);};
    static void rightPCB(Object* pThis) { BPlayer* t = (BPlayer*)pThis; t->right(true);};
    static void upRCB(Object* pThis) { BPlayer* t = (BPlayer*)pThis; t->up(false);};
    static void downRCB(Object* pThis) { BPlayer* t = (BPlayer*)pThis; t->down(false);};
    static void leftRCB(Object* pThis) { BPlayer* t = (BPlayer*)pThis; t->left(false);};
    static void rightRCB(Object* pThis) { BPlayer* t = (BPlayer*)pThis; t->right(false);};
    
    /** @brief  For keyboards, this is digital movement, full speed of 0 */
    void up(bool down);
    void down(bool down);
    void left(bool down);
    void right(bool down);

};

#endif /* defined(__GameBox__BoundPlayer__) */
