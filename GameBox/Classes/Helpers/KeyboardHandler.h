/**
 *  @file   KeyboardHandler.h
 *  @author Ben Cortina
 *  @date   9/21/13.
 */

#ifndef __GameBox__KeyHandler__
#define __GameBox__KeyHandler__

#include "cocos2d.h"
#include <vector>

USING_NS_CC;

/** @brief  Will hold the state of a key */
struct KeyState
{
    int keyCode;
    bool isDown;
    
    KeyState(int kC, bool s): keyCode(kC), isDown(s){};
};

/** @struct KeyFunc
 *  @brief  This holds the information needed to handle a key press
 */
struct KeyFunc
{
    /** @var    int keyCode
     *  @brief  The keyCode that will call the func
     */
    int keyCode;
    
    /** @var    std::function<void(Object*)> func
     *  @brief  The function that will be called on keyCode event
     */
    std::function<void(Object*)> func;
    
    /** @var    Object* receiver
     *  @brief  The Object pointer that will be passed to the func
     */
    Object* receiver;
    
    /** @var    bool callOnce
     *  @brief  Whether or not the function should be called multiple times on one press
     */
    bool callOnce;
    
    KeyFunc(int kC, std::function<void(Object*)> f, Object* r):
        keyCode(kC), func(f), receiver(r), callOnce(false){};
    
    KeyFunc(int kC, std::function<void(Object*)> f, Object* r, bool cO):
    keyCode(kC), func(f), receiver(r), callOnce(cO){};
};

/**
 *  @class  KeyboardHandler
 *  @brief  This is a class I can add to any game which will watch key events and call
 *          functions in response
 */
class KeyboardHandler : public Layer
{
private:
    std::vector<KeyFunc*> keyPresses;
    std::vector<KeyFunc*> keyReleases;
    std::vector<KeyState*> keyStates;
    
public:
    KeyboardHandler() { setVisible(false); }; //we dont want any draw calls for this 
    
    /** @brief  Turn on and off the keyboard listening */
    void setEnabled(bool on) { setKeyboardEnabled(on); };
    
    /** @brief  Adds a function to call on keyPress */
    void addKeyPress(const int keyCode, std::function<void(Object*)> func, Object* r, bool callOnce);
    /** @brief  Adds a function to call on keyPress */
    void addKeyPress(const int keyCode, std::function<void(Object*)> func, Object* r)
                        { addKeyPress(keyCode, func, r, false); };
    
    /** @brief  Adds a function to call on keyRelease */
    void addKeyRelease(const int keyCode, std::function<void(Object*)> func, Object* r);
    
    /**
     *  @brief  Handles keyPresses
     */
    void keyPressed(int keyCode);
    
    /** @brief  Handles keyReleases */
    void keyReleased(int keyCode);
    
};

#endif /* defined(__GameBox__KeyHandler__) */
