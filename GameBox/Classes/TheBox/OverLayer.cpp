/**
 *  @file   OverLayer.cpp
 *  @author Ben Cortina
 *  @date   8/28/13.
 */

#include "OverLayer.h"


void OverLayer::initWithColorMenu(const Color4B color, Menu* menu)
{
    //init
    initWithColor(color);
    //add the menu
    addChild(menu);
}