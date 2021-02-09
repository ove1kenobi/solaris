#include "pch.h"
#include "Render2D.h"

    //The ID2D1Factory interface is the starting point for using Direct2D,
    //use an ID2D1Factory to create Direct2D resources.
    //your application should create the factory once and retain it for the life of the application.
/*
	ID2D1Factory* pD2DFactory = NULL;
    HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);*/

    // Obtain the size of the drawing area.
   // RECT rc;
    //function we want somehow from the RenderWindow class
    //GetHandle();
    //GetClientRect(hwnd, &rc);

    //Create a Direct2D render target     
    //Your application should create render targets once and hold on to them for the life of the application
    /*
    ID2D1HwndRenderTarget* pRT = NULL;          
    HRESULT hr = pD2DFactory->CreateHwndRenderTarget(
    D2D1::RenderTargetProperties(),
    D2D1::HwndRenderTargetProperties(
        hwnd,
        D2D1::SizeU(
            rc.right - rc.left,
            rc.bottom - rc.top)
    ),
    &pRT
    );*/

    //Like a factory, a render target can create drawing resources. In this example, the render target creates a brush.
    /*
    ID2D1SolidColorBrush* pBlackBrush = NULL;
    if (SUCCEEDED(hr))
    {      
    pRT->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::Black),
        &pBlackBrush
        ); 
    }*/

    /*A brush is an object that paints an area, such as the stroke of a shape or the fill of a geometry. 
    The brush in this example paints an area with a predefined solid color, black.*/

    /*A brush can only be used with the render target that created it and with other render targets in the same resource domain. 
    In general, you should create brushes once and retain them for the life of the render target that created them.*/

    //Next, use the render target to draw the rectangle.
    /*
    pRT->BeginDraw();

    pRT->DrawRectangle(
    D2D1::RectF(
        rc.left + 100.0f,
        rc.top + 100.0f,
        rc.right - 100.0f,
        rc.bottom - 100.0f),
        pBlackBrush);

    HRESULT hr = pRT->EndDraw(); */
    
    //The DrawRectangle method takes two parameters: 
    //the rectangle to be drawn, and the brush to be used to paint the rectangle's outline.

    //When there are no more frames to draw, or when you receive the D2DERR_RECREATE_TARGET error, 
    //release the render target and any devices it created.
    /*
    pRT->Release();
    pBlackBrush->Release();*/

    //When your application has finished using Direct2D resources (such as when it is about to exit), release the Direct2D factory.
    //pD2DFactory->Release();


