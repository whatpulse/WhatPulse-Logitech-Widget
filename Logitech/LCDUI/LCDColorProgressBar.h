//************************************************************************
//
// LCDColorProgressBar.h
//
// The CLCDColorProgressBar class draws a progress bar onto the color LCD.
// 
// Logitech LCD SDK
//
// Copyright 2008 Logitech Inc.
//************************************************************************

#ifndef __LCDCOLORPROGRESSBAR_H__
#define __LCDCOLORPROGRESSBAR_H__

#include <math.h>
#include "LCDProgressBar.h"


class CLCDColorProgressBar : public CLCDProgressBar
{
public:
    CLCDColorProgressBar();
    virtual ~CLCDColorProgressBar();

    //CLCDBase
    virtual void OnDraw(CLCDGfxBase &rGfx);
    
    //Style settings
    virtual void SetCursorColor(COLORREF color);
    virtual void EnableBorder(BOOL bEnable);
    virtual void SetBorderColor(COLORREF color);
    virtual void SetBorderThickness(int thickness);
    virtual void SetCursorWidth(int width);

protected:
    //Does the actual draw work...
    void DoDrawWork();
    
    BOOL m_bBorderOn;
    int m_nBorderThickness;

    COLORREF m_crBorderColor;
};

#endif