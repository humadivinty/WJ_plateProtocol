#include <stdio.h>

const char* Tool_GetCurrentPath();

void Tool_ReadIntValueFromConfigFile(const char* IniFileName, const char* nodeName, const char* keyName, int&keyValue);