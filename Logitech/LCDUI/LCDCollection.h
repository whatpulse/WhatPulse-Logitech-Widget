//************************************************************************
//
// LCDCollection.h
//
// Holds a collection of base items.  Its draw will draw everything
// in its list.
// 
// Logitech LCD SDK
//
// Copyright 2008 Logitech Inc.
//************************************************************************

#ifndef _LCDCOLLECTION_H_INCLUDED_ 
#define _LCDCOLLECTION_H_INCLUDED_ 

#include "LCDBase.h"
#include <vector>

class CLCDCollection : public CLCDBase
{
public:
    CLCDCollection();
    virtual ~CLCDCollection();

    bool AddObject(CLCDBase *pObject);
    bool RemoveObject(CLCDBase *pObject);

    // CLCDBase
    virtual void OnDraw(CLCDGfxBase &rGfx);
    virtual void OnUpdate(DWORD dwTimestamp);

protected:
    CLCDBase* RetrieveObject(int objpos);
    int GetObjectCount(void);
    bool RemoveObject(int objnum);
    void RemoveAll(void);

protected:
    typedef std::vector <CLCDBase*> LCD_OBJECT_LIST;

    LCD_OBJECT_LIST m_Objects;
};

#endif