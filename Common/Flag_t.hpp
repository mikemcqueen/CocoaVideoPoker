////////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2008 Mike McQueen.  All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

template<class TFlag>
class FlagBase_t
{
private:

    TFlag m_Flags;

public:

    typedef TFlag value_type;

//    static const FlagBase_t<TFlag>      NoFlags;

//    FlagBase_t()                                  { Clear(); }
    FlagBase_t(value_type flags = 0) : m_Flags(flags) {}

    FlagBase_t<TFlag>& operator=(const value_type flags) { m_Flags = flags; }

    void Reset()                        { m_Flags = 0;                       }
    void Clear(TFlag Flags)             { m_Flags ^= (m_Flags & Flags);       }
    void Set(value_type Flags)               { *(value_type*)&m_Flags |= Flags;   }
    bool Test(TFlag Flags) const        { return Flags == (Flags & m_Flags); }
    bool TestAny(TFlag Flags) const     { return 0 != (Flags & m_Flags);     }

//    value_type value() const { return m_Flags; }

};

typedef FlagBase_t<unsigned> Flag_t;

////////////////////////////////////////////////////////////////////////////////
