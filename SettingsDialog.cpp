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
#include "WhatPulseLogitech.h"
#include "SettingsDialog.h"
#include "afxdialogex.h"
#include "IniHandler.h"

extern WhatPulseLogitech dlg;
extern LPWSTR iniDir;
// SettingsDialog dialog

IMPLEMENT_DYNAMIC(SettingsDialog, CDialog)

SettingsDialog::SettingsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(SettingsDialog::IDD, pParent)
{
}

SettingsDialog::~SettingsDialog()
{
}

afx_msg void SettingsDialog::ShowWindow(int nCmdShow)
{
	// Load Client API URL from the settings ini file
	CSimpleIniA ini;
	ini.SetUnicode();
	ini.LoadFile(iniDir);
	const char *pVal = ini.GetValue("Settings", "ClientAPIURL", "http://localhost:3490/v1/unpulsed");
	m_clientAPIURL.SetWindowTextW(CA2W(pVal));
	pVal = ini.GetValue("Settings", "RefreshSeconds", "5");
	m_refreshRate.SetWindowTextW(CA2W(pVal));

	// Set window ontop and in center of screen
	SetWindowPos(&this->wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	CenterWindow();

	CDialog::ShowWindow(nCmdShow);
}

void 
SettingsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CLIENTAPIURL, m_clientAPIURL); 
	DDX_Control(pDX, IDC_REFRESHRATE, m_refreshRate);
}

void 
SettingsDialog::OnOK()
{
	wchar_t text[MAX_PATH];

	// Save settings to ini configuration file
	CSimpleIniA ini;
	ini.SetUnicode();
	ini.LoadFile(iniDir);

	// Retrieve and save the URL
	m_clientAPIURL.GetWindowText(text, MAX_PATH);
	ini.SetValue("Settings", "ClientAPIURL", convertWStringToString(text).c_str());
	// Retrieve and save the refresh rate
	m_refreshRate.GetWindowText(text, MAX_PATH);
	ini.SetValue("Settings", "RefreshSeconds", convertWStringToString(text).c_str());
	// Save to ini file
	ini.SaveFile(iniDir);
	// Restart timer in the main window to reset the refresh rate
	dlg.restartStatsTimer();

	CDialog::OnOK();
}




BEGIN_MESSAGE_MAP(SettingsDialog, CDialog)
END_MESSAGE_MAP()


// SettingsDialog message handlers
