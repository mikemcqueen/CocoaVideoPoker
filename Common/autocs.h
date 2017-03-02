//
// Copyright (C) 2008 Mike McQueen.  All rights reserved.
//

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef Include_AUTOCS_H
#define Include_AUTOCS_H

class CAutoCritSec
{
private:
    CRITICAL_SECTION m_cs;

public:
    CAutoCritSec()      { InitializeCriticalSection(&m_cs ); }
    ~CAutoCritSec()     { DeleteCriticalSection(&m_cs); }

    bool Try()          { return TRUE==TryEnterCriticalSection( &m_cs ); }
    void Enter()        { EnterCriticalSection( &m_cs ); }
    void Leave()        { LeaveCriticalSection( &m_cs ); }
};

class CLock
{
private:
    CLock();
    CLock(const CLock&);
    CLock& operator=(const CLock&);

private:
    CAutoCritSec& m_cs;

public:
    explicit CLock( CAutoCritSec& cs ) : m_cs(cs)   { m_cs.Enter(); }
    ~CLock()                                        { m_cs.Leave(); }

};

#endif // Include_AUTOCS_H_