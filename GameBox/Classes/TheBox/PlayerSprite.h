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
struct Velocity
{
    float x;
    float y;
};

class PlayerSprite : public Sprite
{
protected:
    float _weight; //unused
    Velocity _vel; //unused
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
    static PlayerSprite* create(const char *pszFileName);
    
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
     * Get/Set methods for weight
     * Not used.
     */
    float getWeight() {return _weight;};
    void setWeight(float weight){_weight=weight;};
    
    /**
     * Get/Set methods for velocity.
     * Velocity is currently unused
     */
    Velocity getVelocity() {return _vel;};
    void setVelocity(Velocity vel){_vel.x=vel.x; _vel.y=vel.y;};
    void setVelocityX(float x) {_vel.x=x;};
    void setVelocityY(float y) {_vel.y=y;};
    
    /**
     @brief         Updates player velocity based on the applied force and duration
     *              This would be what I would used if I were to incorporate friction
     *              and momentum. I will not use it here, in its place I will just setVelocity
     @param x       The X value of the force
     @param y       The Y value of the force
     @param dt      The duration the force was applied for
     */
    void applyForce(float x, float y, float dt);
    
    /**
     @brief     Updates the X position of the Sprite based on the time it has held that velocity
     @param vel_x   The velocity in the X direction
     @param dt      The time the velocity has been applied
     */
    void applyVelocityX(float vel_x, float dt);

    /**
     @brief     Updates the Y position of the Sprite based on the time it has held that velocity
     @param vel_y   The velocity in the Y direction
     @param dt      The time the velocity has been applied
     */
    void applyVelocityY(float vel_y, float dt);
    
    /**
     @brief     Updates the position of the Sprite based on dt and its current velocity
     * Not Used.
     @param dt       The time the velocity has been applied
     */
    void updatePos(float dt);
};

#endif /* defined(__GameBox__PlayerSprite__) */
