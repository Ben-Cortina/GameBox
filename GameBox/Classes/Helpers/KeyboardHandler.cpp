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

void KeyboardHandler::enableByReceiver(Object* r)
{
    int size = keyPresses.size();
    for (int i = 0; i < size; i++)
        if ( keyPresses[i]->receiver == r)
            keyPresses[i]->enabled = true;
    
    size = keyReleases.size();
    for (int i = 0; i < size; i++)
        if ( keyReleases[i]->receiver == r)
            keyReleases[i]->enabled = true;
    
}

void KeyboardHandler::disableByReceiver(Object* r)
{
    int size = keyPresses.size();
    for (int i = 0; i < size; i++)
        if ( keyPresses[i]->receiver == r)
            keyPresses[i]->enabled = false;
    
    size = keyReleases.size();
    for (int i = 0; i < size; i++)
        if ( keyReleases[i]->receiver == r)
            keyReleases[i]->enabled = false;
    
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
            keyStates[i]->isDown = true;
            break;
        }
    
    if (!found) //if this is a newly pressed key, add it to the keyStates
        keyStates.push_back(new KeyState(keyCode, true));
        
    size = keyPresses.size();
    
    //call the expected funcs
    for (int i = 0; i < size; i++)
        if ( !keyPresses.empty() &&
             keyPresses[i]->keyCode == keyCode          &&
             (!keyPresses[i]->callOnce || !alreadyDown) &&
             keyPresses[i]->enabled
            )
            keyPresses[i]->func(keyPresses[i]->receiver);
}

/** @brief  Handles keyReleases */
void KeyboardHandler::keyReleased(int keyCode)
{
    int size = keyStates.size();
    
    //unset downstate
    for (int i = 0; i < size; i++)
        if( keyStates[i]->keyCode == keyCode )
        {
            keyStates[i]->isDown = false;
            break;
        }
    size = keyReleases.size();
    
    //call the expected funcs
    for (int i = 0; i < size; i++)
        if ( keyReleases[i]->keyCode == keyCode && keyReleases[i]->enabled)
            keyReleases[i]->func(keyReleases[i]->receiver);
}