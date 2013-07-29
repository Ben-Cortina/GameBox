/*--------------------------------------------------------------*
 *  HomeScene.h                                                 *
 *  GameBox                                                     *
 *                                                              *
 *  Created by Ben Cortina on 7/25/13.                          *
 *                                                              *
 * Welcome to GameBox, this will be an ongoing project of mine  *
 * These will be a collection of various small games that I     *
 * create while learning cocos2dx.                              *
 *                                                              *
 *--------------------------------------------------------------*/

#ifndef __HOME_SCENE_H__
#define __HOME_SCENE_H__

#include "Helpers/CustomCC.h"
#include "GameList.h"

class HomeLayer : public Layer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static Scene* scene();
    
    // a selector callback
    static void menuCloseCallback(Object* pSender);
    
    // implement the "static create()" method
    CREATE_FUNC(HomeLayer);

};

#endif // __HOME_SCENE_H__
