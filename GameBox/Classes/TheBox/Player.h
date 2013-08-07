/*--------------------------------------------------------------*
 *  Player.h                                                    *
 *  GameBox                                                     *
 *                                                              *
 *  Created by Ben Cortina on 8/6/13.                           *
 *                                                              *
 * This is the class that handle the Sprite the player controls *
 *                                                              *
 *--------------------------------------------------------------*/

#ifndef __GameBox__Player__
#define __GameBox__Player__

#include "cocos2d.h"

USING_NS_CC;

// I could use CCSize for this but a simpler stucture will meet my needs
struct Velocity
{
    float x;
    float y;
};

class Player : public Sprite
{
protected:
    float _weight;
    Velocity _vel;
public:
    
    /**
     * Creates a sprite with an image filename.
     *
     * After creation, the rect of sprite will be the size of the image,
     * and the offset will be (0,0).
     *
     * @param   pszFileName The string which indicates a path to image file, e.g., "scene1/monster.png".
     * @return  A valid sprite object that is marked as autoreleased.
     */
    static Player* create(const char *pszFileName);
    
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
    virtual bool initWithFile(const char *pszFilename);
    
    /**
     @brief     Get/Set methods for weight
     */
    float getWeight() {return _weight;};
    void setWeight(float weight){_weight=weight;};
    
    /**
     @brief     Get/Set methods for velocity
     */
    Velocity getVelocity() {return _vel;};
    void setVelocity(Velocity vel){_vel.x=vel.x; _vel.y=vel.y;};
    void setVelocity(float x, float y){_vel.x=x; _vel.y=y;};
    
    /**
     @brief     Calculates the change of the Player based on the applied force
     @param x       The X value of the force
     @param y       The Y value of the force
     @param dt      The duration the force was applied for
     */
    void applyForce(float x, float y, float dt);
    
    /**
     @brief     Updates the position of the Sprite based on the
     @param dt       The time since last update
     */
    //void update(float dt);
    
    
};

#endif /* defined(__GameBox__Player__) */
