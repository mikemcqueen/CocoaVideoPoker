
//
// Copyright (C) 2008 Mike McQueen.  All rights reserved.
//

#pragma once
#ifndef Include_TIMER_T_H
#define Include_TIMER_T_H

#define LogAlways wprintf
#define LogInfo wprintf

class Timer_t
{

private:

    std::wstring strText;
    

    FILETIME  FileTime;
    bool      _seconds;
    bool      _log;

public:

    explicit
    Timer_t(
        const wchar_t* pszText = L"Timer_t",
              bool     log     = true,
              bool     seconds = false)
    :
        strText(pszText),
        _seconds(seconds),
        _log(log)
    {
        Now(FileTime);
        if (log)
        {
            printf("++%ls\n", strText.c_str());
        }
    }

    ~Timer_t()
    {
        Show();
    }

    static
    const FILETIME&
    Now(
        FILETIME& ft)
    {
#ifdef WIN32
        SYSTEMTIME st;
        GetSystemTime(&st);
        SystemTimeToFileTime(&st, &ft);
#endif
        return ft;
    }

 
   size_t
    Diff()
    {
#ifdef WIN32
        FILETIME ft;
        Now(ft);
        LARGE_INTEGER lNow;
        lNow.LowPart =  ft.dwLowDateTime;
        lNow.HighPart = ft.dwHighDateTime;
        LARGE_INTEGER lThen;
        lThen.LowPart =  FileTime.dwLowDateTime;
        lThen.HighPart = FileTime.dwHighDateTime;
        FileTime = ft;
        return (size_t)((lNow.QuadPart - lThen.QuadPart) / 10000);
#else
        return 0;
#endif
    }

    void
    Show(
        const wchar_t* pszText = NULL,
              bool     bAlways = false)
    {
        const wchar_t* suffix = L"ms";
        size_t diff = Diff();
        if (_seconds)
        {
            suffix = L"seconds";
            diff /= 1000;
        }
        if (_log)
        {
            if (NULL == pszText)
            {
                if (bAlways)
                    LogAlways(L"--%ls: %d %ls\n", strText.c_str(), diff, suffix);
                else
                    LogInfo(L"--%ls: %d %ls\n", strText.c_str(), diff, suffix);
            }
            else
            {
                if (bAlways)
                    LogAlways(L"%ls: %d %ls\n", pszText, diff, suffix);
                else
                    LogInfo(L"%ls: %d %ls\n", pszText, diff, suffix);
            }
        }
    }

    void
    Set(const FILETIME& Time)
    {
        FileTime = Time;
    }

    void
    Set()
    {
        Now(FileTime);
    }

};

#endif // Include_TIMER_T_H
