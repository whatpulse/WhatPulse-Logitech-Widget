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
#include "stdafx.h"
#include "WhatPulseLogitechApp.h"
#include "WhatPulseLogitech.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define LCDSAMPLE_MUTEXNAME _T("WhatPulse_Logitech_Widget_0_1")

BEGIN_MESSAGE_MAP(WhatPulseLogitechApp, CWinApp)
    ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


WhatPulseLogitechApp::WhatPulseLogitechApp()
{
    m_hMutex = NULL;
    m_gdiplusToken = NULL;
}

WhatPulseLogitechApp theApp;
WhatPulseLogitech dlg;

BOOL WhatPulseLogitechApp::InitInstance()
{
	// If there's a "-delayedstart" in the parameters (on system boot), go to sleep for 30 seconds
	// This is primarily to make sure we give the WhatPulse client some time to start
	if (StrStrI(AfxGetApp()->m_lpCmdLine, _T("-delayedstart")))
	{
		::Sleep(30000);
	}

    // InitCommonControlsEx() is required on Windows XP if an application
    // manifest specifies use of ComCtl32.dll version 6 or later to enable
    // visual styles.  Otherwise, any window creation will fail.
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    // Set this to include all the common control classes you want to use
    // in your application.
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    InitCommonControlsEx(&InitCtrls);

    // Initialize GDI+
    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

    CWinApp::InitInstance();


    m_hMutex = ClaimMutex(LCDSAMPLE_MUTEXNAME, NULL);
    if(NULL == m_hMutex)
    {
        return FALSE;
    }

    m_pMainWnd = &dlg;
    INT_PTR nResponse = dlg.DoModal();
    if (nResponse == IDOK)
    {
        //  dismissed with OK
    }
    else if (nResponse == IDCANCEL)
    {
        //  dismissed with Cancel
    }

    // Since the dialog has been closed, return FALSE so that we exit the
    //  application, rather than start the application's message pump.
    return FALSE;
}

INT WhatPulseLogitechApp::ExitInstance()
{
    if(NULL != m_hMutex)
    {
        ReleaseMutex(m_hMutex);
        CloseHandle(m_hMutex);
        m_hMutex = NULL;
    }
    
    Gdiplus::GdiplusShutdown(m_gdiplusToken);

    return CWinApp::ExitInstance();
}

HANDLE WhatPulseLogitechApp::ClaimMutex(LPCTSTR szMutex, LPCTSTR szWndClassName)
{
    // attempt to claim the mutex
    HANDLE hMutex = CreateMutex(NULL, TRUE, szMutex);
    if (ERROR_ALREADY_EXISTS == GetLastError())
    {
        if (hMutex)
        {
            CloseHandle(hMutex);
            hMutex = NULL;
        }

        // bring the existing wnd to the foreground
        if (szWndClassName)
        {
            HWND hExistingWnd = FindWindow(szWndClassName, NULL);
            if (hExistingWnd)
            {
                ShowWindow(hExistingWnd, SW_RESTORE);
                SetForegroundWindow(hExistingWnd);
            }
        }
        return NULL;
    }

    return hMutex;
}
