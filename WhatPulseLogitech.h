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

#include <GdiPlus.h>
#include "stdafx.h"
#include "EZ_LCD.h"
#include "resource.h"
#include "SettingsDialog.h"

#define INI_FILE_NAME ".\\WhatPulseLogitechWidget.ini"

std::string convertWStringToString(std::wstring sourceString);
std::wstring convertCharToWString(const char* sourceString);

class WhatPulseLogitech : public CDialog
{
    // Construction
public:
	WhatPulseLogitech(CWnd* pParent = NULL);	// standard constructor
	void restartStatsTimer();

    // Dialog Data
	enum { IDD = IDD_WHATPULSELOGITECH_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Our settings dialog
	SettingsDialog *settingsDlg;

	BOOL m_alertClientAPINotWorking;
    HICON m_hIcon;

    CEzLcd m_lcd;

    // Monochrome
	HANDLE m_keysTextMono;
	HANDLE m_keysValueMono;

	HANDLE m_clicksTextMono;
	HANDLE m_clicksValueMono;

	HANDLE m_downAndUploadTextMono;
	HANDLE m_downAndUploadValueMono;

	HANDLE m_uptimeTextMono;
	HANDLE m_uptimeValueMono;

	// Color
	HANDLE m_titleText;

	HANDLE m_keysText;
	HANDLE m_keysValue;

	HANDLE m_clicksText;
	HANDLE m_clicksValue;

	HANDLE m_downloadText;
	HANDLE m_downloadValue;

	HANDLE m_uploadText;
	HANDLE m_uploadValue;

	HANDLE m_uptimeText;
	HANDLE m_uptimeValue;


    VOID InitLCDObjectsMonochrome();
    VOID InitLCDObjectsColor();

	VOID refeshWhatPulseStats();

	std::string formatDateTimeShort(long long diff, bool includeSeconds);
	std::string FormatWithCommas(long long value);
	std::string formatBandwidth(long long bytes);

    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnDestroy( );
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
    DECLARE_MESSAGE_MAP()
};
