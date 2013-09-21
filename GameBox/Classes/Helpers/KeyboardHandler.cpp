//
/**
 *  @file   KeyvoardHandler.cpp
 *  @author Ben Cortina
 *  @date   9/21/13.
 */

#include "KeyboardHandler.h"

void KeyboardHandler::addKeyPress(const int keyCode, std::function<void(Object*)> func, Object* r, bool callOnce)
{
    keyPresses.push_back(new KeyFunc(keyCode, func, r, callOnce));
}

void KeyboardHandler::addKeyRelease(const int keyCode, std::function<void(Object*)> func, Object* r)
{
    keyReleases.push_back(new KeyFunc(keyCode, func, r));
}

void KeyboardHandler::keyPressed(int keyCode)
{
    int size = keyStates.size();
    bool alreadyDown = false;
    bool found = false;
    
    //check keystates
    for (int i = 0; i < size; i++)
        if( keyStates[i]->keyCode == keyCode )
        {
            found = true;
            alreadyDown = keyStates[i]->isDown;
            break;
        }
    
    if (!found) //if this is a newly pressed key, add it to the keyStates
        keyStates.push_back(new KeyState(keyCode, true));
        
        
    size = keyPresses.size();
    
    //call the expected funcs
    for (int i = 0; i < size; i++)
        if ( keyPresses[i]->keyCode == keyCode && keyPresses[i]->callOnce != alreadyDown)
            keyPresses[i]->func(keyPresses[i]->receiver);
}

/** @brief  Handles keyReleases */
void KeyboardHandler::keyReleased(int keyCode)
{
    int size = keyReleases.size();
    
    //call the expected funcs
    for (int i = 0; i < size; i++)
        if ( keyReleases[i]->keyCode == keyCode)
            keyReleases[i]->func(keyPresses[i]->receiver);
}