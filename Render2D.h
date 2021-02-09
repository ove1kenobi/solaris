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
THOUGHTS:
The 2D render work in such a way as
if the 2D render should work like the normal render

Direct2D is a set of 2D drawing APIs used to draw pixel-based primitives and effects. 
When starting out with Direct2D, it's best to keep things simple. 
Complex layouts and interface behaviors need time and planning.
If your game requires a complex user interface, like those found in simulation and strategy games, consider using XAML instead.

Direct2D isn't specifically designed for user interfaces or layouts like HTML and XAML. 
It doesn't provide user interface components like lists, boxes, or buttons. 
It also doesn't provide layout components like divs, tables, or grids.


STRUCTURE:
- something has to store all the shapes that need to rendered in each module
- something needs to render them (as you can't just slap in 2D images that easily)
*/

