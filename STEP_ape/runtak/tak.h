#pragma once

#ifndef _WIN64
//32bit�� STEP_ape.ste or 32bit�� runtak.exe �Ŏg�p
void GetAudioFormatTak(const TCHAR *cszFileName, 
                       TCHAR *pszFormat, int nSize,
                       int *pTimeSec);
//32bit�� runtak.exe �ł̂ݎg�p
void GetAudioFormatTakToFileMap(const TCHAR *cszFileName, 
                                const TCHAR *cszFileMapName);
#else
//64bit�� STEP_ape.ste �ł̂ݎg�p
void GetAudioFormatTakFromFileMap(void *pFileMap, 
                                  TCHAR *pszFormat, int nSize,
                                  int *pTimeSec);
void* CreateTakFileMap(const TCHAR *cszFileMapName);
void  CloseTakFileMap(void *pFileMap);
#endif
