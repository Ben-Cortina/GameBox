//
//  CustomCC.cpp
//  GameBox
//
//  Created by Blaeeap on 7/25/13.
//
//

#include "CustomCC.h"


//////////////////////////////////////////////      --MyMenuItemImage--      //////////////////////////////////////////////
// myMenuItemImage Constructors
MyMenuItemImage* MyMenuItemImage::create()
{
    MyMenuItemImage *pRet = new MyMenuItemImage();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

MyMenuItemImage * MyMenuItemImage::create(const char *normalImage, const char *selectedImage)
{
    return MyMenuItemImage::create(normalImage, selectedImage, NULL, (const ccMenuCallback&)nullptr);
}

// XXX deprecated
MyMenuItemImage * MyMenuItemImage::create(const char *normalImage, const char *selectedImage, Object* target, SEL_MenuHandler selector)
{
    return MyMenuItemImage::create(normalImage, selectedImage, NULL, target, selector);
}

MyMenuItemImage * MyMenuItemImage::create(const char *normalImage, const char *selectedImage, const ccMenuCallback& callback)
{
    return MyMenuItemImage::create(normalImage, selectedImage, NULL, callback);
}

// XXX deprecated
MyMenuItemImage * MyMenuItemImage::create(const char *normalImage, const char *selectedImage, const char *disabledImage, Object* target, SEL_MenuHandler selector)
{
    MyMenuItemImage *pRet = new MyMenuItemImage();
    if (pRet && pRet->initWithNormalImage(normalImage, selectedImage, disabledImage, target, selector))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

MyMenuItemImage * MyMenuItemImage::create(const char *normalImage, const char *selectedImage, const char *disabledImage, const ccMenuCallback& callback)
{
    MyMenuItemImage *pRet = new MyMenuItemImage();
    if (pRet && pRet->initWithNormalImage(normalImage, selectedImage, disabledImage, callback))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

MyMenuItemImage * MyMenuItemImage::create(const char *normalImage, const char *selectedImage, const char *disabledImage)
{
    MyMenuItemImage *pRet = new MyMenuItemImage();
    if (pRet && pRet->initWithNormalImage(normalImage, selectedImage, disabledImage, (const ccMenuCallback&)nullptr))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

//selected with bounce
void MyMenuItemImage::selected()
{
    this->runAction(CCSequence::create(
                                       CCScaleBy::create(0.125f, 1.111f),
                                       CCScaleBy::create(0.125f, 0.9f), NULL));
    MenuItemImage::selected();
}

//////////////////////////////////////////////     --MyScene--      //////////////////////////////////////////////


void MyScene::runLayer(Layer* pLayer)
{
    //run the Layer's initialization
    pLayer->init();
    
    //add it to this scene
    addChild(pLayer);
    pLayer->release();
    
    //replace current scene
    CCDirector::getInstance()->replaceScene(this);
}