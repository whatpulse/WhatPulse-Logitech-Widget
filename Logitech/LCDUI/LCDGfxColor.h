//************************************************************************
//
// NOTE: This version of ColorLCDUI is pre-release and is subject to 
// change.
//
// LCDGfxColor.h
//
// This Gfx object handles drawing to a 320x240 color display.
//
// Logitech LCD SDK
//
// Copyright 2008 Logitech Inc.
//************************************************************************

#ifndef __LCDGFXCOLOR_H__
#define __LCDGFXCOLOR_H__

#include "LCDGfxBase.h"

class CLCDGfxColor : public CLCDGfxBase
{
public:
    CLCDGfxColor(void);
    virtual ~CLCDGfxColor(void);

    virtual HRESULT Initialize(void);
    virtual lgLcdBitmap *GetLCDScreen(void);

    virtual DWORD GetFamily(void)
    {
        return LGLCD_DEVICE_FAMILY_QVGA_BASIC;
    }

private:
    lgLcdBitmapQVGAx32 m_LCDScreen;
};

#endif

//** end of LCDGfxColor.h*************************************************
