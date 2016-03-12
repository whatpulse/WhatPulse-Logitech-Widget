//************************************************************************
//
// LCDBitmap.h
//
// The CLCDBitmap class draws bitmaps onto the LCD.
// 
// Logitech LCD SDK
//
// Copyright 2008 Logitech Inc.
//************************************************************************

#ifndef _LCDBITMAP_H_INCLUDED_ 
#define _LCDBITMAP_H_INCLUDED_ 

#include "LCDBase.h"

class CLCDBitmap : public CLCDBase
{
public:
    CLCDBitmap();
    virtual ~CLCDBitmap(); 

    // CLCDBase
    virtual void OnDraw(CLCDGfxBase &rGfx);

    void SetBitmap(HBITMAP hBitmap);
    HBITMAP GetBitmap(void);
    void SetROP(DWORD dwROP);
    void SetZoomLevel(float fzoom);
    float GetZoomLevel(void);
    void SetAlpha(BOOL bAlpha);

protected:   
    HBITMAP m_hBitmap;
    DWORD   m_dwROP;
    float   m_fZoom;
    // this indicates the bitmap has an alpha channel
    BOOL    m_bAlpha;

private:
};


#endif // !_LCDBITMAP_H_INCLUDED_ 

//** end of LCDBitmap.h **************************************************
