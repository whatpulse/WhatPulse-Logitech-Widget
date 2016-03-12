//************************************************************************
//
// LCDOutput.h
//
// The CLCDOutput manages the actual device and the various pages that
// are sent to that device
//
// This class is now managed by CLCDConnection. You no longer need to 
// derive or instantiate this class yourself
// 
// Logitech LCD SDK
//
// Copyright 2008 Logitech Inc.
//************************************************************************

#ifndef _CLCDOUTPUT_H_INCLUDED_ 
#define _CLCDOUTPUT_H_INCLUDED_ 

#include "LCDCollection.h"
#include "LCDGfxBase.h"
#include "LCDPage.h"


class CLCDOutput : public CLCDCollection
{
public:
    CLCDOutput(void);
    virtual ~CLCDOutput(void);

    // Assign the type of graphics this output supports.
    // This is set by CLCDConnection.
    void SetGfx(CLCDGfxBase *gfx);

    void AddPage(CLCDPage *pPage);
    void RemovePage(CLCDPage *pPage);
    void ShowPage(CLCDPage *pPage, BOOL bShow = TRUE);
    CLCDPage* GetShowingPage(void);

    BOOL Open(lgLcdOpenContext &OpenContext);
    BOOL OpenByType(lgLcdOpenByTypeContext &OpenContext);
    void Close(void);
    void Shutdown(void);

    void SetScreenPriority(DWORD priority);
    DWORD GetScreenPriority(void);

    BOOL IsOpened(void);
    HRESULT SetAsForeground(BOOL bSetAsForeground);

    virtual BOOL OnDraw(void);
    virtual void OnUpdate(DWORD dwTimestamp );
    virtual void OnLCDButtonDown(int nButton);
    virtual void OnLCDButtonUp(int nButton);

    virtual void OnSoftButtonEvent(DWORD dwButtonState);
    DWORD GetSoftButtonState(void);

    // This returns true, if the device got opened through
    // OpenByType(), instead of regular Open().
    BOOL HasBeenOpenedByDeviceType(void);

    // This is being called when we receive a device removal
    // notification. After that, we can't reopen devices anymore
    // (until the next device arrival comes in)
    void StopOpeningByDeviceType(void);

    // This is called, when a device function fails. This can
    // sometimes happen during plug/unplug and two same devices
    // are present.
    BOOL ReOpenDeviceType(void);

    int GetDeviceId(void);

protected:
    virtual BOOL DoesBitmapNeedUpdate(lgLcdBitmap* pBitmap);
    virtual void OnPageShown(CLCDCollection* pScreen);
    virtual void OnPageExpired(CLCDCollection* pScreen);
    virtual void OnEnteringIdle(void);
    virtual void OnClosingDevice(int hDevice);
    virtual void OnOpenedDevice(int hDevice);

private:
    HRESULT HandleErrorFromAPI(DWORD dwRes);
    void HandleButtonState(DWORD dwButtonState, DWORD dwButton);

    CLCDPage* m_pActivePage;

    int m_hDevice;
    BOOL m_bSetAsForeground;
    DWORD m_dwButtonState;

    DWORD m_nPriority;

    CLCDGfxBase* m_pGfx;

    lgLcdOpenByTypeContext m_OpenByTypeContext;
};

#endif

//** end of LCDOutput.h **************************************************
