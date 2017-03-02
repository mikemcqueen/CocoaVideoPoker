//
// Copyright (C) 2008 Mike McQueen.  All rights reserved.
//

#pragma once

#ifndef _AUTOHAND_H_
#define _AUTOHAND_H_

#include "Log.h"

class CAutoHandle
{
private:
    HANDLE m_h;

public:
    CAutoHandle() :
        m_h(INVALID_HANDLE_VALUE)
        { }

    ~CAutoHandle()
    {
        Close();
    }

    CAutoHandle& operator=( HANDLE h )
    {
        ASSERT(NULL != h);
        ASSERT(h != m_h);
        Close();
        m_h = h;
        return *this;
    }

    void Close( void )
    { 
        if (valid())
        {
            CloseHandle( m_h );
            m_h = INVALID_HANDLE_VALUE;
        }
    }

/*    operator HANDLE() const
    {
        return m_h;
    }*/

    HANDLE get() const
    {
        return m_h;
    }

    bool valid() const { return INVALID_HANDLE_VALUE != m_h; }

private:
    CAutoHandle(const CAutoHandle&);
};

class CAutoGdiHandle
{
    HANDLE m_h;

public:
    CAutoGdiHandle()
    {
        m_h = 0;
    }
    ~CAutoGdiHandle()
    {
        Close();
    }
    CAutoGdiHandle operator=( HANDLE h )
    {
        Close();
        m_h = h;
        return *this;
    }
    void Close( void )
    { 
        if( m_h )
        {
            DeleteObject( m_h );
            m_h = 0;
        }
    }
/*    operator HANDLE()
    {
        return m_h;
    }*/
    HANDLE get()
    {
        return m_h;
    }
private:
    CAutoGdiHandle(const CAutoGdiHandle&);
};

#endif // _AUTOHAND_H_
