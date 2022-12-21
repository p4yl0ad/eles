#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#include <locale.h>
#pragma comment(lib, "User32.lib")

int _tmain(int argc, TCHAR* argv[])
{
    WIN32_FIND_DATA ffd;
    LARGE_INTEGER filesize;
    TCHAR szDir[MAX_PATH];
    TCHAR GCDb[MAX_PATH];
    size_t length_of_arg;
    HRESULT rSCLW, rSCcpy, rSCCat = STRSAFE_E_INVALID_PARAMETER;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD dwError = 0;
    DWORD rGCD;
    TCHAR* buffer;
    SYSTEMTIME lpSystemTime;
    BOOL FTTSTr;

    rGCD = GetCurrentDirectory(MAX_PATH, GCDb);
    wprintf(TEXT("\nDirectory of %s\n\n"), GCDb);
    
    rSCLW = StringCchLength((TCHAR *) GCDb, MAX_PATH, &length_of_arg);
    if (rSCLW != S_OK)
    {
        _tprintf(TEXT("\nStringCchLengthW failed with STRSAFE_E_INVALID_PARAMETER\n"));
        return (-1);
    }

    if (length_of_arg > (MAX_PATH - 3))
    {
        _tprintf(TEXT("\nDirectory path is too long.\n"));
        return (-1);
    }

    rSCcpy = StringCchCopy(szDir, MAX_PATH, GCDb);
    if (rSCcpy != S_OK)
    {
        _tprintf(TEXT("\nStringCchCopy failed with STRSAFE_E_INVALID_PARAMETER\n"));
        wprintf(L"fuck %i\n", rSCLW);
        return (-1);
    }

    rSCCat = StringCchCat(szDir, MAX_PATH, TEXT("\\*"));
    if (rSCCat != S_OK)
    {
        _tprintf(TEXT("\nStringCchCat failed with STRSAFE_E_INVALID_PARAMETER\n"));
        wprintf(L"fuck %i\n", rSCCat);
        return (-1);
    }

    hFind = FindFirstFile(szDir, &ffd);
    if (INVALID_HANDLE_VALUE == hFind)
    {
        wprintf(TEXT("\nFindFirstFile %ld\n"), dwError);
        return dwError;
    }

    do
    {
        filesize.LowPart = ffd.nFileSizeLow;
        filesize.HighPart = ffd.nFileSizeHigh;
        
        FTTSTr = FileTimeToSystemTime(
            (FILETIME*)&ffd.ftLastWriteTime,
            (LPSYSTEMTIME)&lpSystemTime
        );

        long     length = 0;
        buffer = NULL;
        length = GetShortPathName(ffd.cFileName, NULL, 0);
        if (length == 0)
        {
            wprintf(TEXT("\nGetShortPathName 1 \n"));
            return -1;
        }
        buffer = new TCHAR[length];

        length = GetShortPathName(ffd.cFileName, buffer, length);
        if (length == 0)
        {
            wprintf(TEXT("\nGetShortPathName 2\n"));
            return -1;
        }

        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            wprintf(TEXT("%.2d/%.2d/%.2d  %.2d:%.2d    <DIR>        %s\n"),
                lpSystemTime.wDay, lpSystemTime.wMonth, lpSystemTime.wYear,
                lpSystemTime.wHour, lpSystemTime.wMinute,
                //buffer, // SHORTNAME
                ffd.cFileName);
        }
        else
        {
            wprintf(TEXT("%.2d/%.2d/%.2d  %.2d:%.2d\t%9d %s\n"), 
                lpSystemTime.wDay, lpSystemTime.wMonth, lpSystemTime.wYear,
                lpSystemTime.wHour, lpSystemTime.wMinute,
                (int)filesize.QuadPart,
                //buffer,// SHORTNAME
                ffd.cFileName
            );
        }

        delete[] buffer;

    } while (FindNextFile(hFind, &ffd) != 0);

    dwError = GetLastError();
    if (dwError != ERROR_NO_MORE_FILES)
    {
        wprintf(TEXT("\nFindFirstFile %ld\n"), dwError);
    }

    dwError = FindClose(hFind);
    return dwError;
}

/*
C:\Windows\System32>C:\Users\p4\source\repos\eles\x64\Debug\eles.exe | findstr /i .exe | head -n 20
06/10/2021  13:37           13312 agentactivationruntimestarter.exe                                                                          
09/11/2022  18:35         1220096 AgentService.exe                                                                                           
06/10/2021  13:38         3232056 aitstatic.exe                                                                                              
06/10/2021  13:38           95744 alg.exe                                                                                                    
22/06/2022  04:25          111616 ApiSetHost.AppExecutionAlias.dll                                                                           
06/10/2021  13:37          120320 AppHostRegistrationVerifier.exe                                                                            
14/09/2022  16:49           49152 appidcertstorecheck.exe                                                                                    
14/09/2022  16:49          160768 appidpolicyconverter.exe                                                                                   
14/09/2022  16:49           25600 appidtel.exe                                                                                               
06/10/2021  13:38           78456 ApplicationFrameHost.exe                                                                                   
22/06/2022  04:26         1157120 ApplySettingsTemplateCatalog.exe                                                                           
09/11/2022  18:34         1214280 ApplyTrustOffline.exe                                                                                      
06/10/2021  13:37          235008 ApproveChildRequest.exe                                                                                    
12/10/2022  03:27          782712 AppVClient.exe                                                                                             
22/06/2022  04:26          177000 AppVDllSurrogate.exe                                                                                       
06/05/2022  14:32          154448 appverif.exe                                                                                               
22/06/2022  04:26          168272 AppVNice.exe                                                                                               
22/06/2022  04:26          221032 AppVShNotify.exe                                                                                           
07/12/2019  09:09           26624 ARP.EXE                                                                                                    
22/06/2022  04:26          103936 AssignedAccessGuard.exe
*/