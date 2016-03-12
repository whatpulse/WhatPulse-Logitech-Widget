
#ifndef EZLCD_PAGE_H_INCLUDED_
#define EZLCD_PAGE_H_INCLUDED_




#include "EZ_LCD_Defines.h"
#include "LCDPage.h"

class CEzLcd;


class CEzLcdPage : public CLCDPage
{
public:
    CEzLcdPage();
    CEzLcdPage(CEzLcd * container);
    ~CEzLcdPage();

    HANDLE AddText(LGObjectType type, LGTextSize size, INT alignment, INT maxLengthPixels, INT numberOfLines = 1);
    HRESULT SetText(HANDLE handle, LPCTSTR text, BOOL resetScrollingTextPosition = FALSE);

    HANDLE AddColorText(LGObjectType type, LGTextSize size, INT alignment, INT maxLengthPixels, INT numberOfLines = 1, LONG fontWeight = FW_DONTCARE);
    HRESULT SetTextBackground(HANDLE handle, INT backMode, COLORREF color = RGB(0,0,0));
    HRESULT SetTextFontColor(HANDLE handle, COLORREF color);

    HANDLE AddIcon(HICON hIcon, INT sizeX, INT sizeY);

    HANDLE AddProgressBar(LGProgressBarType type);
    HRESULT SetProgressBarPosition(HANDLE handle, FLOAT percentage);
    HRESULT SetProgressBarSize(HANDLE handle, INT width, INT height);

    //Need a separate function for a color progress bar
    //this is invisible on the EZLCD level
    HANDLE AddColorProgressBar(LGProgressBarType type);
    HRESULT SetProgressBarColors(HANDLE handle, COLORREF cursorcolor, COLORREF bordercolor); 
    HRESULT SetProgressBarBackgroundColor(HANDLE handle, COLORREF color);

    HANDLE AddSkinnedProgressBar(LGProgressBarType type);

    HANDLE AddBitmap(INT width, INT height);
    HRESULT SetBitmap(HANDLE handle, HBITMAP bitmap);

    HRESULT SetOrigin(HANDLE handle, INT originX, INT originY);
    HRESULT SetVisible(HANDLE handle, BOOL visible);

    VOID Update();

protected:
    CLCDBase* GetObject(HANDLE handle);
    VOID Init();

protected:
    CEzLcd *    m_container;
    BOOL        m_buttonIsPressed[NUMBER_SOFT_BUTTONS];
    BOOL        m_buttonWasPressed[NUMBER_SOFT_BUTTONS];

};

typedef std::vector <CEzLcdPage*> LCD_PAGE_LIST;
typedef LCD_PAGE_LIST::iterator LCD_PAGE_LIST_ITER;


#endif		// EZLCD_PAGE_H_INCLUDED_
