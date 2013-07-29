//
//  GameList.h
//  GameBox
//
//  Created by Ben Cortina on 7/26/13.
//
//  This file simply holds all the includes, names
//  and icons for each of the games. This allows me
//  to just include this as a header in the HomeScrene.h
//  rather than have a massive pile of junk before the class is declared.

#ifndef GameBox_GameList_h
#define GameBox_GameList_h

#include "cocos2d.h"

//  Number of Games in the GameBox
#define GAME_COUNT 1

// The main headers for each game
// This header should include the game's layer
#include "TheBox/TheBox.h"

// So this is for the menu on the HomeScene
/**
 @brief             This will be used to provide a elements for each game's MyMenuItemImage
 @param name:       name that appears when the image is touched/clicked/selected
 @attr  image:      the image that is used in the menu
 @attr  callback:   This is the function that will be called on click
 */
struct {
	const char* name;
    const char* image;
	std::function<void(Object*)> callback;
} g_GameList[] = {
    {"TheBox", "",TheBoxLayer::runThisGame}
};





#endif
