#ifndef BT_FULL_LIB_PATH_H
#define BT_FULL_LIB_PATH_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tchar.h>

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

class CBTFullLibPath
{
public:
                            CBTFullLibPath();
                            ~CBTFullLibPath();

public:
        bool                        IsFullPath();
        bool                        GetFullInstallPathOf(TCHAR* szFileNameOnly, TCHAR* pOutBuf, size_t OutBufCapacity);
        bool                        GetConditionalDllPathOf(TCHAR* szFileNameOnly, TCHAR* pOutBuf, size_t OutBufCapacity);
        //                          Note: OutBufCapacity size is in TCHARs

public:
        static TCHAR                m_szInstallDir[MAX_PATH+1];
        static TCHAR                m_szInstDirSysWow64[MAX_PATH+1];
        static TCHAR                m_szBtRezPath[MAX_PATH+1];

private:
    
		static long                 m_nRefCount;
        static bool                 m_fLoadedFullPath;
        static bool                 m_bIsWow64;

};


#endif