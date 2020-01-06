#include "stdafx.h"
#include "ToolFunction_simple.h"
#include <string.h>
#include <direct.h>
#include<Dbghelp.h>

#include <Dbghelp.h>
#pragma comment(lib, "Dbghelp.lib")


#ifndef MAX_PATH
#define  MAX_PATH 260
#endif


const char* Tool_GetCurrentPath()
{
    static char szPath[256] = { 0 };
    memset(szPath, '\0', sizeof(szPath));
    _getcwd(szPath, sizeof(szPath));
    return szPath;
}


void Tool_ReadIntValueFromConfigFile(const char* IniFileName, const char* nodeName, const char* keyName, int&keyValue)
{
    char iniFileName[MAX_PATH] = { 0 };
    sprintf_s(iniFileName, sizeof(iniFileName), "%s\\%s", Tool_GetCurrentPath(), IniFileName);

    int iValue = GetPrivateProfileIntA(nodeName, keyName, keyValue, iniFileName);
    keyValue = iValue;

    char chTemp[128] = { 0 };
    sprintf_s(chTemp, sizeof(chTemp), "%d", iValue);
    WritePrivateProfileStringA(nodeName, keyName, chTemp, iniFileName);
}