/*--------------------------------------------------------------*
 *  Player.h                                                    *
 *  GameBox                                                     *
 *                                                              *
 *  Created by Ben Cortina on 8/6/13.                           *
 *                                                              *
 * This is the class that handle the Sprite the player controls *
 *                                                              *
 *--------------------------------------------------------------*/

#ifndef __GameBox__PlayerSprite__
#define __GameBox__PlayerSprite__

#include "cocos2d.h"

USING_NS_CC;

// I could use CCSize for this but a simpler stucture will meet my needs
struct Vector
{
    float x; // % max speed
    float y; // % max speed
};

class PlayerSprite : public Sprite
{
private:
    timeval lastUpdate;
    float dtCalculated;
    Layer* owner;
protected:
    float _weight; //unused
    Vector _vel;
    Vector _force; //yes, I know "force" isnt technically correct for this, but its close enough
    float _maxSpeed;
public:
    
    PlayerSprite();
    
    /**
     * Creates a sprite with an image filename.
     *
     * After creation, the rect of sprite will be the size of the image,
     * and the offset will be (0,0).
     *
     * @param   pszFileName The string which indicates a path to image file, e.g., "scene1/monster.png".
     * @return  A valid sprite object that is marked as autoreleased.
     */
    static PlayerSprite* create(const char *pszFileName, Layer* caller);
    
    /**
     * Initializes a sprite with an image filename.
     *
     * This method will find pszFilename from local file system, load its content to Texture2D,
     * then use Texture2D to create a sprite.
     * After initialization, the rect used will be the size of the image. The offset will be (0,0).
     *
     * @param   pszFilename The path to an image file in local file system
     * @return  true if the sprite is initialized properly, false otherwise.
     */
    virtual bool initWithFile(const char *pszFilename, Layer* caller);
    
    
    /**
     * Get/Set methods for weight
     * Not used.
     */
    float getWeight() {return _weight;};
    void setWeight(float weight){_weight=weight;};
    
    /**
     * Get/Set methods for maxspeed
     * Not used.
     */
    float getMaxSpeed() {return _maxSpeed;};
    void setMaxSpeed(float maxSpeed){_maxSpeed=maxSpeed;};
    
    /**
     * Get/Set methods for velocity.
     */
    Vector getVelocity() {return _vel;};
    void updateVelocity();
    
    /**
     * Get/Set methods for force.
     */
    Vector getForce() {return _force;};
    void setForce(Vector force){_force.x=force.x; _force.y=force.y; updateVelocity();};
    void setForceX(float x) {_force.x=x;updateVelocity();};
    void setForceY(float y) {_force.y=y;updateVelocity();};
    
    /**
     @brief         Updates player velocity based on the applied force and duration
     *              This would be what I would used if I were to incorporate friction
     *              and momentum. I will likely  not use it here, in its place I will just setVelocity
     @param x       The X value of the force
     @param y       The Y value of the force
     @param dt      The duration the force was applied for
     */
    void applyForce(float x, float y, float dt);
    
    /**
     @brief     Updates the position of the Sprite based on dt and its current velocity
     * Not Used.
     @param dt       The time over which the velocity has been applied
     */
    void update(float dt);
    
    
    /**
     @brief     Updates the position of the Sprite based on the currenttime - lastupdate
     * Ths allows me to call update whenever a keychanges so the movement is calculated based on
     * the time of the key event rather than the time of each update.
     * Not Used.
     */
    void update();
};

#endif /* defined(__GameBox__PlayerSprite__) */
