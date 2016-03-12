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
#include "json\json.h"
#include "WhatPulseLogitechApp.h"
#include "WhatPulseLogitech.h"
#include "WinHTTPClient\WinHttpClient.h"
#include "IniHandler.h"

#include <codecvt>
#include <sstream>
#include <locale>

#ifdef UNICODE
typedef std::wostringstream tstringstream;
#else
typedef std::ostringstream tstringstream;
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


WhatPulseLogitech::WhatPulseLogitech(CWnd* pParent /*=NULL*/) : CDialog(WhatPulseLogitech::IDD, pParent)
{
    
}

void WhatPulseLogitech::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(WhatPulseLogitech, CDialog)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_WM_DESTROY()
    ON_WM_TIMER()
    ON_WM_WINDOWPOSCHANGING()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL WhatPulseLogitech::OnInitDialog()
{
    CDialog::OnInitDialog();
	
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_alertClientAPINotWorking = false;
	settingsDlg = new SettingsDialog(this);
	settingsDlg->Create(SettingsDialog::IDD);

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

    ShowWindow(SW_HIDE);
    SetWindowPos(NULL, 0, 0, 0, 0, NULL);

    HRESULT hRes = m_lcd.Initialize(_T("WhatPulse Statistics"), LG_DUAL_MODE, FALSE, TRUE);

    if (hRes != S_OK)
    {
		MessageBox(_T("Unable to connect to Logitech Display. Is the Logitech Gaming Software running? Exiting Widget."));
        PostQuitMessage(0);
        return FALSE;
    }

	// Initialise default settings if they don't exist yet
	CSimpleIniA ini;
	ini.SetUnicode();
	ini.LoadFile(INI_FILE_NAME);

	const char *pVal = ini.GetValue("Settings", "ClientAPIURL", "");
	if (strcmp(pVal, "") == 0) {
		ini.SetValue("Settings", "ClientAPIURL", "http://localhost:3490/v1/unpulsed");
	}

	pVal = ini.GetValue("Settings", "RefreshSeconds", "");
	if (strcmp(pVal, "") == 0) {
		ini.SetValue("Settings", "RefreshSeconds", "5");
	}
	// Save ini file
	ini.SaveFile(INI_FILE_NAME);


	// Setup both the monochrome and color display layouts
    InitLCDObjectsMonochrome();
    InitLCDObjectsColor();
	// Initial refresh of WhatPulse stats
	refeshWhatPulseStats();

	// Check for button usage every 30ms
    SetTimer(1, 30, NULL); 

	// Start statistics timer 
	restartStatsTimer();

    return TRUE;
}

void 
WhatPulseLogitech::restartStatsTimer()
{
	// Stop timer first
	KillTimer(2);

	// Get the refresh rate from the ini configuration file
	CSimpleIniA ini;
	ini.SetUnicode();
	ini.LoadFile(INI_FILE_NAME);

	// Get the value for the stats refresh timer
	const char *pVal = ini.GetValue("Settings", "RefreshSeconds", "5");
	int refreshRate = atoi(pVal);
	// Sanity check
	if (refreshRate > 300 || refreshRate < 1)
		refreshRate = 5;

	// Update the statistics every 5 seconds (default)
	SetTimer(2, refreshRate * 1000, NULL);
}

void WhatPulseLogitech::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

HCURSOR WhatPulseLogitech::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}

void WhatPulseLogitech::OnDestroy()
{
	KillTimer(1);
	KillTimer(2);
}

void WhatPulseLogitech::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1)
	{
		// Check whether the menu button has been pushed
		if (m_lcd.ButtonTriggered(LG_BUTTON_MENU))
		{
			// Show settings dialog in Windows
			if (!settingsDlg->IsWindowVisible())
				settingsDlg->ShowWindow(SW_SHOW);
		}
		// Update LCD screen
		m_lcd.Update();
	}
	// Timer 2 goes every 5 seconds, update the WhatPulse stats
	else if (nIDEvent == 2)
	{
		refeshWhatPulseStats();
	}
}

void WhatPulseLogitech::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
    ASSERT(NULL != lpwndpos);

    if(NULL != lpwndpos)
    {
        lpwndpos->flags &= ~SWP_SHOWWINDOW;
    }

    CDialog::OnWindowPosChanging(lpwndpos);
}

// Initialise the monochrome screen layout
// this function paints text on a coordinate scheme
VOID WhatPulseLogitech::InitLCDObjectsMonochrome()
{
    m_lcd.ModifyDisplay(LG_MONOCHROME);

    /***************/
    /* FIRST PAGE */
    /***************/

	int y = 3;
	int x_text = 5;
	int x_value = 55;

	// Keys
	m_keysTextMono = m_lcd.AddText(LG_STATIC_TEXT, LG_SMALL, DT_LEFT, LGLCD_BW_BMP_WIDTH, 3);
	m_lcd.SetOrigin(m_keysTextMono, x_text, y);
	m_lcd.SetText(m_keysTextMono, _T("Keys:"));

	m_keysValueMono = m_lcd.AddText(LG_STATIC_TEXT, LG_SMALL, DT_LEFT, LGLCD_BW_BMP_WIDTH, 3);
	m_lcd.SetOrigin(m_keysValueMono, x_value, y);
	m_lcd.SetText(m_keysValueMono, _T("n/a"));

	// Move down
	y += 10;

	// Clicks
	m_clicksTextMono = m_lcd.AddText(LG_STATIC_TEXT, LG_SMALL, DT_LEFT, LGLCD_BW_BMP_WIDTH, 3);
	m_lcd.SetOrigin(m_clicksTextMono, x_text, y);
	m_lcd.SetText(m_clicksTextMono, _T("Clicks:"));

	m_clicksValueMono = m_lcd.AddText(LG_STATIC_TEXT, LG_SMALL, DT_LEFT, LGLCD_BW_BMP_WIDTH, 3);
	m_lcd.SetOrigin(m_clicksValueMono, x_value, y);
	m_lcd.SetText(m_clicksValueMono, _T("n/a"));

	// Move down
	y += 10;

	// Download & Upload
	m_downAndUploadTextMono = m_lcd.AddText(LG_STATIC_TEXT, LG_SMALL, DT_LEFT, LGLCD_BW_BMP_WIDTH, 3);
	m_lcd.SetOrigin(m_downAndUploadTextMono, x_text, y);
	m_lcd.SetText(m_downAndUploadTextMono, _T("Down / Up:"));

	m_downAndUploadValueMono = m_lcd.AddText(LG_STATIC_TEXT, LG_SMALL, DT_LEFT, LGLCD_BW_BMP_WIDTH, 3);
	m_lcd.SetOrigin(m_downAndUploadValueMono, x_value, y);
	m_lcd.SetText(m_downAndUploadValueMono, _T("n/a"));

	// Move down
	y += 10;

	// Uptime
	m_uptimeTextMono = m_lcd.AddText(LG_STATIC_TEXT, LG_SMALL, DT_LEFT, LGLCD_BW_BMP_WIDTH, 3);
	m_lcd.SetOrigin(m_uptimeTextMono, x_text, y);
	m_lcd.SetText(m_uptimeTextMono, _T("Uptime:"));

	m_uptimeValueMono = m_lcd.AddText(LG_STATIC_TEXT, LG_SMALL, DT_LEFT, LGLCD_BW_BMP_WIDTH, 3);
	m_lcd.SetOrigin(m_uptimeValueMono, x_value, y);
	m_lcd.SetText(m_uptimeValueMono, _T("n/a"));
}


// Initialise the color screen layout
// this function paints text on a coordinate scheme
VOID WhatPulseLogitech::InitLCDObjectsColor()
{
    m_lcd.ModifyDisplay(LG_COLOR);

	// Title
	m_titleText = m_lcd.AddText(LG_STATIC_TEXT, LG_BIG, DT_LEFT, LGLCD_QVGA_BMP_WIDTH, 3);
	m_lcd.SetOrigin(m_titleText, 35, 25);
	m_lcd.SetText(m_titleText, _T("WhatPulse Stats"));
	m_lcd.SetTextFontColor(m_titleText, RGB(255, 255, 255));

	int y = 80;
	int x_text = 35;
	int x_value = 145;

	// Keys
    m_keysText = m_lcd.AddText(LG_STATIC_TEXT, LG_MEDIUM, DT_LEFT, LGLCD_QVGA_BMP_WIDTH, 3);
	m_lcd.SetOrigin(m_keysText, x_text, y);
	m_lcd.SetText(m_keysText, _T("Keys:"));
	m_lcd.SetTextFontColor(m_keysText, RGB(255, 255, 255));

	m_keysValue = m_lcd.AddText(LG_STATIC_TEXT, LG_MEDIUM, DT_LEFT, LGLCD_QVGA_BMP_WIDTH, 3);
	m_lcd.SetOrigin(m_keysValue, x_value, y);
	m_lcd.SetText(m_keysValue, _T("n/a"));
	m_lcd.SetTextFontColor(m_keysValue, RGB(255, 255, 255));

	// Move down
	y += 25;

	// Clicks
	m_clicksText = m_lcd.AddText(LG_STATIC_TEXT, LG_MEDIUM, DT_LEFT, LGLCD_QVGA_BMP_WIDTH, 3);
	m_lcd.SetOrigin(m_clicksText, x_text, y);
	m_lcd.SetText(m_clicksText, _T("Clicks:"));
	m_lcd.SetTextFontColor(m_clicksText, RGB(255, 255, 255));

	m_clicksValue = m_lcd.AddText(LG_STATIC_TEXT, LG_MEDIUM, DT_LEFT, LGLCD_QVGA_BMP_WIDTH, 3);
	m_lcd.SetOrigin(m_clicksValue, x_value, y);
	m_lcd.SetText(m_clicksValue, _T("n/a"));
	m_lcd.SetTextFontColor(m_clicksValue, RGB(255, 255, 255));

	// Move down
	y += 25;

	// Download
	m_downloadText = m_lcd.AddText(LG_STATIC_TEXT, LG_MEDIUM, DT_LEFT, LGLCD_QVGA_BMP_WIDTH, 3);
	m_lcd.SetOrigin(m_downloadText, x_text, y);
	m_lcd.SetText(m_downloadText, _T("Download:"));
	m_lcd.SetTextFontColor(m_downloadText, RGB(255, 255, 255));

	m_downloadValue = m_lcd.AddText(LG_STATIC_TEXT, LG_MEDIUM, DT_LEFT, LGLCD_QVGA_BMP_WIDTH, 3);
	m_lcd.SetOrigin(m_downloadValue, x_value, y);
	m_lcd.SetText(m_downloadValue, _T("n/a"));
	m_lcd.SetTextFontColor(m_downloadValue, RGB(255, 255, 255));

	// Move down
	y += 25;

	// Upload
	m_uploadText = m_lcd.AddText(LG_STATIC_TEXT, LG_MEDIUM, DT_LEFT, LGLCD_QVGA_BMP_WIDTH, 3);
	m_lcd.SetOrigin(m_uploadText, x_text, y);
	m_lcd.SetText(m_uploadText, _T("Upload:"));
	m_lcd.SetTextFontColor(m_uploadText, RGB(255, 255, 255));

	m_uploadValue = m_lcd.AddText(LG_STATIC_TEXT, LG_MEDIUM, DT_LEFT, LGLCD_QVGA_BMP_WIDTH, 3);
	m_lcd.SetOrigin(m_uploadValue, x_value, y);
	m_lcd.SetText(m_uploadValue, _T("n/a"));
	m_lcd.SetTextFontColor(m_uploadValue, RGB(255, 255, 255));

	// Move down
	y += 25;

	// Uptime
	m_uptimeText = m_lcd.AddText(LG_STATIC_TEXT, LG_MEDIUM, DT_LEFT, LGLCD_QVGA_BMP_WIDTH, 3);
	m_lcd.SetOrigin(m_uptimeText, x_text, y);
	m_lcd.SetText(m_uptimeText, _T("Uptime:"));
	m_lcd.SetTextFontColor(m_uptimeText, RGB(255, 255, 255));

	m_uptimeValue = m_lcd.AddText(LG_STATIC_TEXT, LG_MEDIUM, DT_LEFT, LGLCD_QVGA_BMP_WIDTH, 3);
	m_lcd.SetOrigin(m_uptimeValue, x_value, y);
	m_lcd.SetText(m_uptimeValue, _T("n/a"));
	m_lcd.SetTextFontColor(m_uptimeValue, RGB(255, 255, 255));
}

// Convert a wstring/wchar to a string
std::string 
convertWStringToString(std::wstring sourceString)
{
	// Setup the codectv converter
	typedef std::codecvt_utf8<wchar_t> convert_type;
	std::wstring_convert<convert_type, wchar_t> converter;

	// convert the wstring httpResponseContent to a string
	std::string converted_str = converter.to_bytes(sourceString);

	return converted_str;
}

std::wstring 
convertCharToWString(const char* sourceString)
{
	// Take the easy way out
	tstringstream stros;
	stros << sourceString;
	return stros.str();
}


// Refresh the WhatPulse statistics from the Client API to the LCD screen
VOID WhatPulseLogitech::refeshWhatPulseStats()
{
	// Load Client API URL from the settings ini file
	CSimpleIniA ini;
	ini.SetUnicode();
	ini.LoadFile(INI_FILE_NAME);
	const char *pVal = ini.GetValue("Settings", "ClientAPIURL", "http://localhost:3490/v1/unpulsed");

	long bandwidthMultiplier = 1;
	if (std::strstr(pVal, "account-totals") != NULL) {
		bandwidthMultiplier = 1024 * 1024; // the bandwidth in account-totals in returned in MB
	}

	// Send a GET HTTP Request to the WhatPulse Client API
	WinHttpClient client(convertCharToWString(pVal));
	client.SendHttpRequest();

	// Check the header, we need it to be '200 OK'
	wstring httpResponseHeader = client.GetResponseHeader();

	if (httpResponseHeader.find(L"200 OK") != 0)
	{
		// Get the response body, which contains the JSON the Client API generates
		wstring httpResponseContent = client.GetResponseContent();

		// We need to convert the wstring from WinHttpClient to a string so we can give that to the JSON parser
		std::string json_string = convertWStringToString(httpResponseContent);
	
		// Convert the string to a JSON array so that we can extract the statistics
		Json::Value root;
		Json::Reader reader;
		bool parsingSuccessful = reader.parse(json_string, root);
		if (parsingSuccessful)
		{
			// The JSON array has now been converted...We can update the text values!
			tstringstream stros;

			// Check if key is in the JSON output
			if (!root["keys"].empty())
			{
				stros << FormatWithCommas((long)root["keys"].asInt64()).c_str();
				m_lcd.SetText(m_keysValue, stros.str().c_str());
				m_lcd.SetText(m_keysValueMono, stros.str().c_str());
				stros.str(L"");
			}
			else {
				// If the key is not in the JSON output, put "n/a" as the value
				m_lcd.SetText(m_keysValue, _T("n/a"));
				m_lcd.SetText(m_keysValueMono, _T("n/a"));
			}

			// Check if key is in the JSON output
			if (!root["clicks"].empty())
			{
				stros << FormatWithCommas((long long)root["clicks"].asInt64()).c_str();
				m_lcd.SetText(m_clicksValue, stros.str().c_str());
				m_lcd.SetText(m_clicksValueMono, stros.str().c_str());
				stros.str(L"");
			}
			else {
				// If the key is not in the JSON output, put "n/a" as the value
				m_lcd.SetText(m_clicksValue, _T("n/a"));
				m_lcd.SetText(m_clicksValueMono, _T("n/a"));
			}

			// Check if key is in the JSON output
			if (!root["download"].empty())
			{
				stros << formatBandwidth((long long)(root["download"].asInt64() * bandwidthMultiplier)).c_str();
				m_lcd.SetText(m_downloadValue, stros.str().c_str());
				stros.str(L"");
			}
			else {
				// If the key is not in the JSON output, put "n/a" as the value
				m_lcd.SetText(m_downloadValue, _T("n/a"));
			}

			// Check if key is in the JSON output
			if (!root["upload"].empty())
			{
				stros << formatBandwidth((long long)root["upload"].asInt64() * bandwidthMultiplier).c_str();
				m_lcd.SetText(m_uploadValue, stros.str().c_str());
				stros.str(L"");
			}
			else {
				// If the key is not in the JSON output, put "n/a" as the value
				m_lcd.SetText(m_uploadValue, _T("n/a"));
			}

			// Check if key is in the JSON output
			if (!root["upload"].empty() && !root["download"].empty())
			{
				stros << formatBandwidth((long long)root["download"].asInt64() * bandwidthMultiplier).c_str();
				stros << " / ";
				stros << formatBandwidth((long long)root["upload"].asInt64() * bandwidthMultiplier).c_str();
				m_lcd.SetText(m_downAndUploadValueMono, stros.str().c_str());
				stros.str(L"");
			}
			else {
				// If the key is not in the JSON output, put "n/a" as the value
				m_lcd.SetText(m_downAndUploadValueMono, _T("n/a"));
			}

			// Check if key is in the JSON output
			if (!root["uptime"].empty())
			{
				stros << formatDateTimeShort((long long)root["uptime"].asInt64(), false).c_str();
				m_lcd.SetText(m_uptimeValue, stros.str().c_str());
				m_lcd.SetText(m_uptimeValueMono, stros.str().c_str());
				stros.str(L"");
			}
			else {
				// If the key is not in the JSON output, put "n/a" as the value
				m_lcd.SetText(m_uptimeValue, _T("n/a"));
				m_lcd.SetText(m_uptimeValueMono, _T("n/a"));
			}

			m_alertClientAPINotWorking = false;
		}
		else
		{
			// Alert the user if the Client API is not working as expected, just once
			if (!m_alertClientAPINotWorking)
			{
				MessageBox(_T("The WhatPulse Client API is not responding. Do you have it enabled in the WhatPulse Settings?"));
				m_alertClientAPINotWorking = true;
			}
		}
	}
	else 
	{
		// Alert the user if the Client API is not working as expected, just once
		if (!m_alertClientAPINotWorking)
		{
			MessageBox(_T("The WhatPulse Client API is not responding. Do you have it enabled in the WhatPulse Settings?"));
			m_alertClientAPINotWorking = true;
		}
	}
}

// Convert an integer to a localized number
std::string WhatPulseLogitech::FormatWithCommas(long long value)
{
	std::stringstream ss;
	ss.imbue(std::locale(""));
	ss << std::fixed << value;
	return ss.str();
}

// Convert seconds into a readable format
std::string
WhatPulseLogitech::formatDateTimeShort(long long diff, bool includeSeconds)
{
	if (diff < 60) {
		return "<1 min";
	}
	else
	{
		std::stringstream msg;

		bool append_comma = false;
		long long years = diff / (3600 * 24 * 356);
		diff -= years * (3600 * 24 * 356);
		if (years >= 1) {
			msg << years << "y";
			append_comma = true;
		}

		long long days = diff / (3600 * 24);
		diff -= days * (3600 * 24);
		if (days >= 1)
		{
			if (append_comma) {
				msg << ", ";
			}
			msg << days << "d";
			append_comma = true;
		}

		long long hours = diff / 3600;
		diff -= hours * 3600;
		if (hours >= 1)
		{
			if (append_comma) {
				msg << ", ";
			}
			msg << hours << "h";
			append_comma = true;
		}

		long long minutes = diff / 60;
		diff -= minutes * 60;
		if (minutes >= 1) 
		{
			if (append_comma) {
				msg << ", ";
			}
			msg << minutes << "m";
			append_comma = true;
		}

		if (diff > 0 && includeSeconds)
			msg << diff << "s";

		return msg.str();
	}
}

// Round a double to the requested precision
double GetFloatPrecision(double value, double precision)
{
	return (floor((value * pow(10, precision) + 0.5)) / pow(10, precision));
}

// Convert bytes to human readable filesize
std::string
WhatPulseLogitech::formatBandwidth(long long bytes)
{
	std::stringstream msg;

	if (bytes >= 1099511627776.0)
		msg << GetFloatPrecision(bytes / 1099511627776.0, 2) << "TB";
	else if (bytes >= 1073741824)
		msg << GetFloatPrecision(bytes / 1073741824.0, 2) << "GB";
	else if (bytes >= 1048576.0)
		msg << GetFloatPrecision(bytes / 1048576.0, 2) << "MB";
	else if (bytes >= 1024)
		msg << GetFloatPrecision(bytes / 1024.0, 2) << "KB";
	else
		msg << GetFloatPrecision(bytes / 1.0, 2) << "B";

	return msg.str();
}