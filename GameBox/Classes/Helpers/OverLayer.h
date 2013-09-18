/**
 *  @file   OverLayer.h
 *  @author Ben Cortina
 *  @date   8/28/13.
 */


#ifndef __GameBox__OverLayer__
#define __GameBox__OverLayer__

#include "cocos2d.h"

USING_NS_CC;

/**
 * @class   OverLayer
 * @brief   Provides a transparent overlay with a menu on it. Releases on menu selection
 */
class OverLayer : public LayerColor
{
    
public:
    OverLayer(){};
    
    /**
     *  @brief  Initializes the layer with a menu and background color.
     *  @param  color   The color of the background
     *  @param  pMenu   The menu to display on this layer
     */
    void initWithColorMenu(const Color4B color, Menu* pMenu);
};

#endif /* defined(__GameBox__OverLayer__) */
