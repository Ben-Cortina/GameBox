/*--------------------------------------------------------------*
 *  CustomCC.h                                                  *
 *  GameBox                                                     *
 *                                                              *
 *  Created by Ben Cortina on 7/25/13.                          *
 *                                                              *
 * This file just contains all the minor edits I do to CC       *
 * classes to add features and such that I desire for this      *
 * project.
 *--------------------------------------------------------------*/

#ifndef __GameBox__CustomCC__
#define __GameBox__CustomCC__

#include "cocos2d.h"

USING_NS_CC;


//////////////////////////////////////////////      --MyMenuItemImage--      //////////////////////////////////////////////

// I should eventually expand this to allow for effects to be added for each item

// Custom MenuItemImage that grows a bit when selected
class MyMenuItemImage : public MenuItemImage
{
public:
    /** Creates an MyMenuItemImage. */
    static MyMenuItemImage* create();
    /** creates a menu item with a normal and selected image*/
    static MyMenuItemImage* create(const char *normalImage, const char *selectedImage);
    /** creates a menu item with a normal,selected  and disabled image*/
    static MyMenuItemImage* create(const char *normalImage, const char *selectedImage, const char *disabledImage);
    /** creates a menu item with a normal and selected image with target/selector */
    CC_DEPRECATED_ATTRIBUTE static MyMenuItemImage* create(const char *normalImage, const char *selectedImage, Object* target, SEL_MenuHandler selector);
    /** creates a menu item with a normal and selected image with a callable object */
    static MyMenuItemImage* create(const char *normalImage, const char *selectedImage, const ccMenuCallback& callback);
    
    /** creates a menu item with a normal,selected  and disabled image with target/selector */
    CC_DEPRECATED_ATTRIBUTE static MyMenuItemImage* create(const char *normalImage, const char *selectedImage, const char *disabledImage, Object* target, SEL_MenuHandler selector);
    /** creates a menu item with a normal,selected  and disabled image with a callable object */
    static MyMenuItemImage* create(const char *normalImage, const char *selectedImage, const char *disabledImage, const ccMenuCallback& callback);
    
    MyMenuItemImage(){}
    virtual ~MyMenuItemImage(){}
    
    /**
     @brief  This triggers when the MenuItem(Bounding Box) is touched/clicked
                      This override just adds an enlarge effect
     @param pLayer    the layer that will be added to the scene
     */
    virtual void selected() override;
};

//////////////////////////////////////////////     --MyScene--      //////////////////////////////////////////////

// Custom blank Scene that adds the passed layer to itself
// Because currently everything on each scene is run on one layer
// This allows me to not have to create a custom scene class for each game (each layer)
class MyScene : public Scene
{
public:
    /**
     @brief adds a layer to this scene then replaces the current scene with this one
     @param pLayer    the layer that will be added to the scene
     */
    virtual void runLayer(Layer* pLayer);
    
    // implement the "static create()" method
    CREATE_FUNC(MyScene);
};

#endif /* defined(__GameBox__CustomCC__) */
