/*
*	WhatPulse Logitech Gaming Keyboard Widget
*
*	This widget uses the WhatPulse Client API to retrieve statistics from the WhatPulse Client
*	and display them on a Logitech Gaming Keyboard LCD screen. It also serves as an open example
*	of how the Client API can be used.
*
*	This has been tested on a G19 and G510. It requires the WhatPulse Client and that the
*	client API has been enabled in the Settings tab.
*
*	WhatPulse (c) 2016 - http://whatpulse.org
*/
#pragma once

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


class WhatPulseLogitechApp : public CWinApp
{
public:
	WhatPulseLogitechApp();

    // Overrides
public:
    virtual BOOL InitInstance();
    virtual INT ExitInstance();

    // Implementation

    DECLARE_MESSAGE_MAP()

protected:
    HANDLE m_hMutex;
    ULONG_PTR m_gdiplusToken;
    HANDLE ClaimMutex(LPCTSTR szMutex, LPCTSTR szWndClassName);
};

extern WhatPulseLogitechApp theApp;