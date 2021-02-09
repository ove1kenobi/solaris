#pragma once
#include "pch.h"
#include "EventSystem/IEventListener.h"
#include "EventSystem/EventPublisher.h"
#include "EventSystem/RenderEvents.h"
#include "EventSystem\UtilityEvents.h"

class Render2D : public EventPublisher, IEventListener {
private:
public:
};

/*
API OPTIONS:
Direct2D: 
+ Can be used for drawing 2D shapes on the screen, shapes can also contain pictures.
+ Easy to incoperate as it can use the swapchain buffer to directly write over the render.
- Can NOT handle user interface components like lists, boxes, or buttons.
- Does also not provide layout components like divs, tables, or grids.

DirectWrite:

HTML (WebKit/Ultralight):

THOUGHTS:
The 2D render work in such a way as
if the 2D render should work like the normal render

Direct2D is a set of 2D drawing APIs used to draw pixel-based primitives and effects. 
When starting out with Direct2D, it's best to keep things simple. 
Complex layouts and interface behaviors need time and planning.
If your game requires a complex user interface, like those found in simulation and strategy games, consider using XAML instead.



STRUCTURE:
- we have a UIHandler which should use the event handler to know which UIModule it should render at the moment
- then we have UIModule
- something has to store all the shapes that need to rendered in each module
- something needs to render them (as you can't just slap in 2D images that easily)
Option 1: Create a single UI class and then create functions for each module
Option 2: Create a UI class for each module
*/

