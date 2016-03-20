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

// SettingsDialog dialog

class SettingsDialog : public CDialog
{
	DECLARE_DYNAMIC(SettingsDialog)

public:
	SettingsDialog(CWnd *pParent = NULL);   // standard constructor
	virtual ~SettingsDialog();
	afx_msg void ShowWindow(int nCmdShow);

// Dialog Data
	enum { IDD = IDD_SETTINGSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);      // DDX/DDV support
	void OnExitClientClicked();

	CEdit m_clientAPIURL;
	CEdit m_refreshRate;

	afx_msg void OnOK();

	DECLARE_MESSAGE_MAP()
};
