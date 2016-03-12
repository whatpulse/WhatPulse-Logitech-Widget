//************************************************************************
//
// LCDIcon.cpp
//
// The CLCDIcon class draws icons onto the lcd.
// 
// Logitech LCD SDK
//
// Copyright 2008 Logitech Inc.
//************************************************************************

#include "LCDUI.h"


//************************************************************************
//
// CLCDIcon::CLCDIcon
//
//************************************************************************

CLCDIcon::CLCDIcon(void)
:   m_hIcon(NULL),
    m_nIconWidth(16),
    m_nIconHeight(16)
{
}


//************************************************************************
//
// CLCDIcon::CLCDIcon
//
//************************************************************************

CLCDIcon::~CLCDIcon(void)
{
}


//************************************************************************
//
// CLCDIcon::SetIcon
//
//************************************************************************

void CLCDIcon::SetIcon(HICON hIcon, int nWidth /* = 16 */, int nHeight /* = 16 */)
{
    m_hIcon = hIcon;
    m_nIconWidth = nWidth;
    m_nIconHeight = nHeight;
}


//************************************************************************
//
// CLCDIcon::OnDraw
//
//************************************************************************

void CLCDIcon::OnDraw(CLCDGfxBase &rGfx)
{
    if (m_hIcon)
    {
        int nOldBkMode = SetBkMode(rGfx.GetHDC(), TRANSPARENT);

        DrawIconEx(rGfx.GetHDC(), 0, 0, m_hIcon,
                   m_nIconWidth, m_nIconHeight, 0, NULL, DI_NORMAL);

        SetBkMode(rGfx.GetHDC(), nOldBkMode);
    }
}


//** end of LCDIcon.cpp **************************************************
