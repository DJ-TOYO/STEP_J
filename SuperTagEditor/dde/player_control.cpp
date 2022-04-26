#include "stdafx.h"
#include "player_control.h"
#include "winamp.h"
#include "wa_ipc.h"
#include "scmpx.h"
#include "kbdde.h"
#include "SuperTagEditor.h"
#include "strcnv.h"
//Winamp
static const TCHAR g_cszClassNameWinamp[] = _T("Winamp v1.x");
//SCMPX
static const TCHAR g_cszClassNameSCMPX[] = _T("SCMPX");
//foobar2000
static const TCHAR g_cszClassNameFoobar[] = _T("{97E27FAA-C0B3-4b8e-A693-ED7881E99FC1}");//�o�[�W�����ɂ���ĕς��\��������H
//Lilith/uLilith
static const TCHAR g_cszClassNameOldLilith[] = _T("Lilith");
static const TCHAR g_cszClassNameNewLilith[] = _T("uLilith");
static const TCHAR g_cszServiceNameOldLilith[] = _T("LILITH_EXT_CONTROL");//��SoundPlayer Lilith(UNICODE ��Ή���)
static const TCHAR g_cszTopicNameOldLilith[] =   _T("LILITH_EXT_CONTROL");//��SoundPlayer Lilith(UNICODE ��Ή���)
static const TCHAR g_cszServiceNameNewLilith[] = _T("uLilithDdeControl"); //uLilith(UNICODE �Ή���)
static const TCHAR g_cszTopicNameNewLilith[] = _T("uLilithDdeControl"); //uLilith(UNICODE �Ή���)
//static const TCHAR g_cszTopicNameNewLilith[] = _T("uLilithDdeControlFixed"); //uLilith(UNICODE �Ή���)
//KbMedia Player
static const TCHAR g_cszMutexKbmplay[] = _T("KbMedia Player");    //�N���� Mutex ��(2.80beta9�ȍ~��64bit�łł��L��)
static const TCHAR g_cszMutexKbmplay64[] = _T("KbMedia Player64");//64bit�ŋN���� Mutex ��
static const TCHAR g_cszServiceNameKbmplay[] = _T("KbMedia Player");//32bit/64bit����
static const TCHAR g_cszTopicNameKbmplay[] = _T("KbMedia Player");  //32bit/64bit����

static const DWORD DDE_TIMEOUT = 3000;
///////////////////////////////////////////////////////////////////////////////
CPlayerControl::CPlayerControl(void)
{
    m_szPlayerPath[0] = 0;
    m_nPlayerType = PLAYER_EXTEND;
};
///////////////////////////////////////////////////////////////////////////////
CPlayerControl::~CPlayerControl(void)
{
}
///////////////////////////////////////////////////////////////////////////////
BOOL __fastcall CPlayerControl::ExecPlayer(const TCHAR *cszFileName, BOOL bClear)
{//cszFileName ���w�肵�ĊJ��
 //bClear == TRUE �̏ꍇ�́i�Ή����Ă���΁j�v���C���X�g��j������
    if(!m_szPlayerPath[0]){
        return FALSE;
    }
    TCHAR szCommand[2048];
    BOOL bExecCommand = FALSE;//��� ExecCommand �����s����ꍇ�� TRUE;
    switch(m_nPlayerType){
        case PLAYER_KBMPLAY:
            if(bClear){//���X�g��j��=>�ǉ�=>�Đ�
                _sntprintf_s(szCommand, _TRUNCATE, 
                             _T("\"%s\" \"%s\" /clear /a /p"), m_szPlayerPath, cszFileName);
            }
            else{
                //���X�g�ɒǉ����Ȃ��ōĐ�
                _sntprintf_s(szCommand, _TRUNCATE, 
                             _T("\"%s\" \"%s\" /na /play"), m_szPlayerPath, cszFileName);
            }
            break;
        case PLAYER_LILITH:
        case PLAYER_ULILITH:
        	// �N�����邾��(��� DDE �R�}���h���s)
            _sntprintf_s(szCommand, _TRUNCATE, 
                         _T("\"%s\""), m_szPlayerPath);//"" �Ŋ���
            bExecCommand = TRUE;
            break;
        case PLAYER_WINAMP:
        case PLAYER_SCMPX:
            // �N�����邾��(��ōĐ��J�n�R�}���h���s)
            _sntprintf_s(szCommand, _TRUNCATE, 
                         _T("\"%s\""), m_szPlayerPath);//"" �Ŋ���
            bExecCommand = TRUE;
            break;
        case PLAYER_FOOBAR:
        default:
        	// �����Ƀt�@�C������n���ċN��
            _sntprintf_s(szCommand, _TRUNCATE, 
                         _T("\"%s\" \"%s\""), m_szPlayerPath, cszFileName);//"" �Ŋ���
            break;
    }
    PROCESS_INFORMATION pi = {0};
    STARTUPINFO si = {0};
    si.cb = sizeof(si);
    if(CreateProcess(m_szPlayerPath,
                     szCommand,
                     NULL,
                     NULL,
                     FALSE, //bInheritHandles
                     0,     //dwCreationFlags
                     0,
                     NULL,  //lpCurrentDirectory,
                     &si,
                     &pi)){
        DWORD dwWaitRet = WaitForInputIdle(pi.hProcess, 3000);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        if(dwWaitRet != WAIT_TIMEOUT && bExecCommand){
            if(m_nPlayerType == PLAYER_LILITH ||
               m_nPlayerType == PLAYER_ULILITH){
            //DDE ���䂪�\�ɂȂ�܂ő҂�
                DWORD dwStartTime = GetTickCount();
                do{
                    TCHAR *pszReturn;
                    RequestCommand(_T("/query_dde_status"), &pszReturn);
                    if(pszReturn){
                        if(_tcsicmp(pszReturn, _T("dde_acceptable")) == 0){
                            free(pszReturn);
                            break;
                        }
                        free(pszReturn);
                    }
                    Sleep(500);
                }while(GetTickCount() - dwStartTime < 10000);//�ő��10�b�ԑ҂�
            }
            else{
                Sleep(2000);//�����҂��Ă���ɂ���
            }
            if(bClear){//���X�g��j��+�ǉ�+���t
                ExecCommand(COMMAND_PLAY, cszFileName);
            }
            else{//�ǉ����Ȃ��ōĐ�
                ExecCommand(COMMAND_NOADDPLAY, cszFileName);
            }
        }
        return TRUE;
    }
    else{
        return FALSE;
    }
}
///////////////////////////////////////////////////////////////////////////////
void __fastcall CPlayerControl::ExecCommand(int nCommand, const TCHAR *cszFileName)
{
    if(!SupportCommand(nCommand)){
        if(nCommand == COMMAND_NOADDPLAY){
            nCommand = COMMAND_PLAY;
        }
        else{
            return;
        }
    }
    if(!PlayerIsActive()){
        return;
    }
    switch(m_nPlayerType){
        case PLAYER_WINAMP:
        case PLAYER_SCMPX:
            ExecCommandWinamp(nCommand, cszFileName);
            break;
        case PLAYER_KBMPLAY:
            ExecCommandKbmplay(nCommand, cszFileName);
            break;
        case PLAYER_LILITH:
        case PLAYER_ULILITH:
            ExecCommandLilith(nCommand, cszFileName);
            break;
        case PLAYER_FOOBAR:
            ExecCommandFoobar(nCommand, cszFileName);
        default:
            break;
    }
}
///////////////////////////////////////////////////////////////////////////////
BOOL __fastcall CPlayerControl::SupportCommand(int nCommand)
{//����Ώۂ̃v���C���[�� nCommand �ɑΉ����Ă���� TRUE ��Ԃ�
/*
    enum{
        COMMAND_NOADDPLAY,//�ǉ����Ȃ��ōĐ�(���Ή��̏ꍇ�� COMMAND_PLAY ���Ă�)
        COMMAND_PLAY,  //�Đ�
        COMMAND_STOP,  //��~
        COMMAND_NEXT,  //���̋�
        COMMAND_PREV,  //�O�̋�
        COMMAND_CLOSE, //��~&�t�@�C���N���[�Y(�Đ����̋Ȃ̃^�O�X�V���o����悤��)
        COMMAND_CLEAR, //���X�g��j��
        COMMAND_ADD,   //���X�g�ɒǉ�
        COMMAND_QUIT   //�v���C���[�I��
    };
*/
    if(!m_szPlayerPath[0]){
        return FALSE;
    }
    if(nCommand == COMMAND_PLAY){//�S�Ẵv���C���[���K���Ή�
        return TRUE;
    }
    BOOL bRet = FALSE;
    if(m_nPlayerType == PLAYER_WINAMP){
        switch(nCommand){
            //case COMMAND_NOADDPLAY://�ǉ����Ȃ��ōĐ�(���Ή��̏ꍇ�� COMMAND_PLAY ���Ă�)
            case COMMAND_STOP:  //��~
            case COMMAND_NEXT:  //���̋�
            case COMMAND_PREV:  //�O�̋�
            //case COMMAND_CLOSE: //��~&�t�@�C���N���[�Y(�Đ����̋Ȃ̃^�O�X�V���o����悤��)
            case COMMAND_CLEAR: //���X�g��j��
            case COMMAND_ADD:   //���X�g�ɒǉ�
            case COMMAND_QUIT:  //�v���C���[�I��
                bRet = TRUE;
                break;
        }
    }
    else if(m_nPlayerType == PLAYER_SCMPX){
        switch(nCommand){
            //case COMMAND_NOADDPLAY://�ǉ����Ȃ��ōĐ�(���Ή��̏ꍇ�� COMMAND_PLAY ���Ă�)
            //case COMMAND_STOP:  //��~
            //case COMMAND_NEXT:  //���̋�
            //case COMMAND_PREV:  //�O�̋�
            //case COMMAND_CLOSE: //��~&�t�@�C���N���[�Y(�Đ����̋Ȃ̃^�O�X�V���o����悤��)
            case COMMAND_CLEAR: //���X�g��j��
            case COMMAND_ADD:   //���X�g�ɒǉ�
            case COMMAND_QUIT:  //�v���C���[�I��
                bRet = TRUE;
                break;
        }
    }
    else if(m_nPlayerType == PLAYER_KBMPLAY){
        switch(nCommand){
            case COMMAND_NOADDPLAY://�ǉ����Ȃ��ōĐ�(���Ή��̏ꍇ�� COMMAND_PLAY ���Ă�)
            case COMMAND_STOP:  //��~
            case COMMAND_NEXT:  //���̋�
            case COMMAND_PREV:  //�O�̋�
            case COMMAND_CLOSE: //��~&�t�@�C���N���[�Y(�Đ����̋Ȃ̃^�O�X�V���o����悤��)
            case COMMAND_CLEAR: //���X�g��j��
            case COMMAND_ADD:   //���X�g�ɒǉ�
            case COMMAND_QUIT:  //�v���C���[�I��
                bRet = TRUE;//�S���Ή�
                break;
        }
    }
    else if(m_nPlayerType == PLAYER_LILITH ||
            m_nPlayerType == PLAYER_ULILITH){
        switch(nCommand){
            case COMMAND_NOADDPLAY://�ǉ����Ȃ��ōĐ�(���Ή��̏ꍇ�� COMMAND_PLAY ���Ă�)
            case COMMAND_STOP:  //��~
            case COMMAND_NEXT:  //���̋�
            case COMMAND_PREV:  //�O�̋�
            case COMMAND_CLOSE: //��~&�t�@�C���N���[�Y(�Đ����̋Ȃ̃^�O�X�V���o����悤��)
            case COMMAND_CLEAR: //���X�g��j��
            case COMMAND_ADD:   //���X�g�ɒǉ�
            case COMMAND_QUIT:  //�v���C���[�I��
                bRet = TRUE;//�S���Ή�
                break;
        }
    }
    else if(m_nPlayerType == PLAYER_FOOBAR){
        switch(nCommand){
            //case COMMAND_NOADDPLAY://�ǉ����Ȃ��ōĐ�(���Ή��̏ꍇ�� COMMAND_PLAY ���Ă�)
            case COMMAND_STOP:  //��~
            case COMMAND_NEXT:  //���̋�
            case COMMAND_PREV:  //�O�̋�
            //case COMMAND_CLOSE: //��~&�t�@�C���N���[�Y(�Đ����̋Ȃ̃^�O�X�V���o����悤��)
            //case COMMAND_CLEAR: //���X�g��j��
            case COMMAND_ADD:   //���X�g�ɒǉ�
            case COMMAND_QUIT:  //�v���C���[�I��
                bRet = TRUE;
                break;
        }
    }
    return bRet;
}
///////////////////////////////////////////////////////////////////////////////
BOOL __fastcall CPlayerControl::RequestCommand(const TCHAR *cszCommand, 
                                               TCHAR **ppszReturn)
{//*ppszReturn �͌Ăяo������ free() ���邱��
    *ppszReturn = NULL;
    if(!PlayerIsActive()){//����Ώۃv���C���[�����N��
        return FALSE;
    }
    if(m_nPlayerType == PLAYER_KBMPLAY){
        return RequestCommandKbmplay(cszCommand, ppszReturn);
    }
    else if(m_nPlayerType == PLAYER_LILITH ||
            m_nPlayerType == PLAYER_ULILITH){
        return RequestCommandLilith(cszCommand, ppszReturn);
    }
    return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
BOOL __fastcall CPlayerControl::RequestCommandKbmplay(const TCHAR *cszCommand, 
                                                      TCHAR **ppszReturn)
{//*ppszReturn �͌Ăяo������ free() ���邱��
    KbDDEClient client(NULL, g_cszServiceNameKbmplay, g_cszTopicNameKbmplay);
    return client.Request(ppszReturn, cszCommand, DDE_TIMEOUT, FALSE); 
}
///////////////////////////////////////////////////////////////////////////////
BOOL __fastcall CPlayerControl::RequestCommandLilith(const TCHAR *cszCommand, 
                                               TCHAR **ppszReturn)
{//*ppszReturn �͌Ăяo������ free() ���邱��
    BOOL bIsNewLilith = (m_nPlayerType == PLAYER_ULILITH);
    const TCHAR *cszServiceName;
    const TCHAR *cszTopicName;
    if(bIsNewLilith){
        cszServiceName = g_cszServiceNameNewLilith;
        cszTopicName = g_cszTopicNameNewLilith;
    }
    else{
        cszServiceName = g_cszServiceNameOldLilith;
        cszTopicName = g_cszTopicNameOldLilith;
    }
    KbDDEClient client(NULL, cszServiceName, cszTopicName);
    return client.Request(ppszReturn, cszCommand, DDE_TIMEOUT, bIsNewLilith); 
}
///////////////////////////////////////////////////////////////////////////////
void __fastcall CPlayerControl::ExecCommandWinamp(int nCommand, const TCHAR *cszFileName)
{
	HWND hWndPlayer = NULL;
    if(m_nPlayerType == PLAYER_WINAMP){
        hWndPlayer = FindWindow(g_cszClassNameWinamp, NULL);
    }
    else if(m_nPlayerType == PLAYER_SCMPX){
        hWndPlayer = FindWindow(g_cszClassNameSCMPX, NULL);
    }
    if(!hWndPlayer){
        return;
    }
    switch(nCommand){
    case COMMAND_NOADDPLAY://���Ή��Ȃ̂Œǉ����čĐ��Ɠ����Ɉ���
    case COMMAND_PLAY:{
        //���X�g��j�� => �ǉ� => �Đ�
        SendMessage(hWndPlayer, WM_WA_IPC, 0, IPC_DELETE);//���X�g��j��
        ExecCommandWinamp(COMMAND_ADD, cszFileName);      //�ǉ�
        SendMessage(hWndPlayer, WM_WA_IPC, 0, IPC_STARTPLAY);//�Đ�
		break;
    }
    case COMMAND_STOP://��~
		SendMessage(hWndPlayer, WM_COMMAND, WINAMP_BUTTON4, 0);
		break;
	case COMMAND_PREV://�O�̋�
		SendMessage(hWndPlayer, WM_COMMAND, WINAMP_BUTTON1, 0);
		break;
	case COMMAND_NEXT://���̋�
		SendMessage(hWndPlayer, WM_COMMAND, WINAMP_BUTTON5, 0);
		break;
    case COMMAND_CLEAR://���X�g��j��
        ::SendMessage(hWndPlayer, WM_WA_IPC, 0, IPC_DELETE);
        break;
    case COMMAND_ADD://���X�g�ɒǉ�
		if (m_nPlayerType == PLAYER_WINAMP) {
			// Winamp
            COPYDATASTRUCT cds = {0};
#ifdef _UNICODE
            cds.dwData = IPC_PLAYFILEW;
#else
            cds.dwData = IPC_PLAYFILE;
#endif
            cds.lpData = _tcsdup(cszFileName);
            cds.cbData = (_tcslen(cszFileName) + 1) * sizeof(TCHAR);
			::SendMessage(hWndPlayer, WM_COPYDATA, NULL, (LPARAM)&cds);
            free(cds.lpData);
		} else {
			// SCMPX
#ifdef _UNICODE
            char *str_ansi = conv_utf16_to_ansi(cszFileName);
#else
            const char *str_ansi = cszFileName;
#endif
            int len_ansi = strlen(str_ansi) + 1;
			int i; 
            for (i = 0; i < len_ansi; i++) {
				SendMessage(hWndPlayer, WM_WA_IPC, str_ansi[i], IPC_PLAYFILE);
			}
#ifdef _UNICODE
            free(str_ansi);
#endif
		} 
        break;
    case COMMAND_QUIT://�I��
        SendMessage(hWndPlayer, WM_CLOSE, NULL, NULL);
        break;
    }
}
///////////////////////////////////////////////////////////////////////////////
void __fastcall CPlayerControl::ExecCommandKbmplay(int nCommand, const TCHAR *cszFileName)
{//KbMedia Player �͋N���ς݂ł��邱�Ƃ��O��
    TCHAR szCommand[2048];
    szCommand[0] = 0;
    switch (nCommand){
    case COMMAND_NOADDPLAY://�ǉ����Ȃ��ōĐ�
        _sntprintf_s(szCommand, _TRUNCATE, _T("\"%s\" /na /play"), cszFileName);
        break;       
    case COMMAND_PLAY://���X�g��j�� => �ǉ� => �Đ�
        _sntprintf_s(szCommand, _TRUNCATE, _T("\"%s\" /clear /a /play"), cszFileName);//���X�g��j�����Ă���ǉ�
        break;
    case COMMAND_STOP:// ��~ 
		_tcsncpy_s(szCommand, _T("/stop"), _TRUNCATE);
		break;
	case COMMAND_PREV:// �O�̋�
		_tcsncpy_s(szCommand, _T("/prev"), _TRUNCATE);
		break;
	case COMMAND_NEXT:// ���̋�
		_tcsncpy_s(szCommand, _T("/next"), _TRUNCATE);
		break;
    case COMMAND_CLOSE://���t��~���t�@�C���N���[�Y
		_tcsncpy_s(szCommand, _T("/fileclose"), _TRUNCATE);
        break;
    case COMMAND_CLEAR://���X�g��j��
		_tcsncpy_s(szCommand, _T("/clear"), _TRUNCATE);
        break;
    case COMMAND_ADD://�ǉ�(�Đ��͂��Ȃ�)
        _sntprintf_s(szCommand, _TRUNCATE, _T("\"%s\" /a /np"), cszFileName);
        break;
    case COMMAND_QUIT://�I��
		_tcsncpy_s(szCommand, _T("/quit"), _TRUNCATE);
		break;
    }//switch(nCommand)
    if(szCommand[0]){
        KbDDEClient client(NULL, g_cszServiceNameKbmplay, g_cszTopicNameKbmplay);
        client.Execute(szCommand, DDE_TIMEOUT);
    }
}
///////////////////////////////////////////////////////////////////////////////
void __fastcall CPlayerControl::ExecCommandLilith(int nCommand, const TCHAR *cszFileName)
{//Lilith �͋N���ς݂ł��邱�Ƃ��O��
    const TCHAR *cszTopicName;
    const TCHAR *cszServiceName;
    if(m_nPlayerType == PLAYER_ULILITH){
        cszServiceName = g_cszServiceNameNewLilith;
        cszTopicName = g_cszTopicNameNewLilith;
    }
    else{
        cszServiceName = g_cszServiceNameOldLilith;
        cszTopicName = g_cszTopicNameOldLilith;
    }

    TCHAR szCommand[2048];
    szCommand[0] = 0;
    switch (nCommand){
    case COMMAND_NOADDPLAY://�ǉ����Ȃ��ōĐ�
        _sntprintf_s(szCommand, _TRUNCATE, _T("/dopen \"%s\" /play"), cszFileName);
        break;
    case COMMAND_PLAY://���X�g��j�� => �ǉ� => �Đ�
        _sntprintf_s(szCommand, _TRUNCATE, _T("/clear /open \"%s\" /play"), cszFileName);
        break;
    case COMMAND_STOP:// ��~ 
        _tcsncpy_s(szCommand, _T("/stop"), _TRUNCATE);
		break;
	case COMMAND_PREV:// �O�̋�
        _tcsncpy_s(szCommand, _T("/back"), _TRUNCATE);
		break;
	case COMMAND_NEXT:// ���̋�
        _tcsncpy_s(szCommand, _T("/next"), _TRUNCATE);
		break;
    case COMMAND_CLOSE://���t��~���t�@�C���N���[�Y
        _tcsncpy_s(szCommand, _T("/stop"), _TRUNCATE);
        break;
    case COMMAND_CLEAR://���X�g��j��
        _tcsncpy_s(szCommand, _T("/clear"), _TRUNCATE);
        break;
    case COMMAND_ADD:{//�ǉ�
        _sntprintf_s(szCommand, _TRUNCATE, _T("/add \"%s\""), cszFileName);
        break;
    }
    case COMMAND_QUIT://�I��
        _tcsncpy_s(szCommand, _T("/exit"), _TRUNCATE);
		break;
    }//switch(nCommand)
    if(szCommand[0]){
        KbDDEClient client(NULL, cszServiceName, cszTopicName);
        client.Execute(szCommand, DDE_TIMEOUT);
    }
}
///////////////////////////////////////////////////////////////////////////////
void __fastcall CPlayerControl::ExecCommandFoobar(int nCommand, const TCHAR *cszFileName)
{//Foobar �͋N���ς݂ł��邱�Ƃ��O��
    TCHAR szCommandLine[2048];
    TCHAR szCommand[2048];
    szCommand[0] = 0;
    switch (nCommand){
    case COMMAND_NOADDPLAY://�ǉ����Ȃ��ōĐ�(���Ή��Ȃ̂� COMMAND_PLAY �Ɠ����ɏ���)
    case COMMAND_PLAY://���X�g��j�� =>�ǉ�=>�Đ�
        //�t�@�C������n���ċN�����邾���Ł��̂悤�ɂȂ�
        _sntprintf_s(szCommand, _TRUNCATE, _T("\"%s\""), cszFileName);
        break;
    case COMMAND_ADD://�ǉ�
        _sntprintf_s(szCommand, _TRUNCATE, _T("\"%s\" /add"), cszFileName);
        break;
    case COMMAND_STOP://��~ 
        _tcsncpy_s(szCommand, _T("/stop"), _TRUNCATE);
		break;
	case COMMAND_PREV://�O�̋�
        _tcsncpy_s(szCommand, _T("/prev"), _TRUNCATE);
		break;
	case COMMAND_NEXT://���̋�
        _tcsncpy_s(szCommand, _T("/next"), _TRUNCATE);
		break;
    case COMMAND_CLOSE://���t��~���t�@�C���N���[�Y
        _tcsncpy_s(szCommand, _T("/stop"), _TRUNCATE);
        break;
    case COMMAND_QUIT://�I��
        _tcsncpy_s(szCommand, _T("/exit"), _TRUNCATE);
		break;
    //case COMMAND_CLEAR://���X�g��j��
    //    break;
    }//switch(nCommand)
    _sntprintf_s(szCommandLine, _TRUNCATE, 
                 _T("\"%s\" %s"), 
                 m_szPlayerPath, szCommand);

    PROCESS_INFORMATION pi = {0};
    STARTUPINFO si = {0};
    si.cb = sizeof(si);
    if(CreateProcess(m_szPlayerPath,
                     szCommandLine,
                     NULL,
                     NULL,
                     FALSE, //bInheritHandles
                     0,     //dwCreationFlags
                     0,
                     NULL,  //lpCurrentDirectory,
                     &si,
                     &pi)){
        WaitForSingleObject(pi.hProcess, 2000);//�v���Z�X���I������܂ōő�2�b�҂�
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        if(nCommand == COMMAND_PLAY){
        //���d�N�����̓t�@�C������n���Ă����莞�Ԃ��o�߂��Ȃ��ƃ��X�g��
        //�j������Ȃ��悤�Ȃ̂ŏ����҂i�ǂ̒��x�҂ĂΗǂ����͕s��)
            Sleep(2000);
        }
    }
}
///////////////////////////////////////////////////////////////////////////////
void __fastcall CPlayerControl::SetPlayer(const TCHAR *cszPlayerPath, int nPlayerType)
{//����Ώۂ̃v���C���[��ݒ�
    _tcsncpy_s(m_szPlayerPath, cszPlayerPath, _TRUNCATE);
    m_nPlayerType = nPlayerType;
    switch(m_nPlayerType){
        case PLAYER_WINAMP:
        case PLAYER_SCMPX:
        case PLAYER_KBMPLAY:
        case PLAYER_LILITH:
        case PLAYER_ULILITH:
        case PLAYER_FOOBAR:
            break;
        default:
            m_nPlayerType =PLAYER_EXTEND;
            break;
    }
}
///////////////////////////////////////////////////////////////////////////////
BOOL __fastcall CPlayerControl::PlayerIsActive(void)
{//����Ώۂ̃v���C���[���N���ς݂Ȃ� TRUE ��Ԃ�
    BOOL bRet = FALSE;
    switch(m_nPlayerType){
        case PLAYER_WINAMP:{
            bRet = FindWindow(g_cszClassNameWinamp, NULL) != NULL;
            break;
        }
        case PLAYER_SCMPX:{
            bRet = FindWindow(g_cszClassNameSCMPX, NULL) != NULL;
            break;
        }
        case PLAYER_LILITH:{
            bRet = FindWindow(g_cszClassNameOldLilith, NULL) != NULL;
            break;
        }
        case PLAYER_ULILITH:{
            bRet = FindWindow(g_cszClassNameNewLilith, NULL) != NULL;
            break;
        }
        case PLAYER_KBMPLAY:{
            HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, g_cszMutexKbmplay);
            if(!hMutex){
                hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, g_cszMutexKbmplay64);
            }
            if(hMutex){
                CloseHandle(hMutex);
                bRet = TRUE;
            }
            break;
        }
        case PLAYER_FOOBAR:{//�o�[�W�����ɂ���ăN���X�����قȂ邩���H
            bRet = FindWindow(g_cszClassNameFoobar, NULL) != NULL;
            break;
        }
    }
    return bRet;
}
///////////////////////////////////////////////////////////////////////////////
void __fastcall CPlayerControl::Play(const TCHAR *cszFileName, BOOL bAdd)
{//bAdd == TRUE �̏ꍇ�͒ǉ����čĐ�
    if(!PlayerIsActive()){//�v���C���[�����N���Ȃ�N��
        BOOL bClear = bAdd;//�ǉ����čĐ�����ꍇ�̓��X�g��j��
        ExecPlayer(cszFileName, bClear);
        return;
    }
    if(bAdd){
        ExecCommand(COMMAND_PLAY, cszFileName);
    }
    else{
        ExecCommand(COMMAND_NOADDPLAY, cszFileName);
    }
}
///////////////////////////////////////////////////////////////////////////////
BOOL __fastcall CPlayerControl::GetPlayingFileName(TCHAR *pszFileName, int nSize)//nSize �͕�����
{//�v���C���[���Đ����̃t�@�C������ pszFileName �Ɋi�[
    pszFileName[0] = 0;
    TCHAR *pszReturn = NULL;
    //RequestCommandXXXX �𒼐ڌĂ΂Ȃ��悤�ɂ���
    //m_szPlayerPath �̗L�����`�F�b�N�A�v���C���[�̋N���ς݃`�F�b�N���� RequestCommand ��
    //�ōs����
    if(m_nPlayerType == PLAYER_KBMPLAY){
        RequestCommand(_T("filename"), &pszReturn);
        if(pszReturn){
            _tcsncpy_s(pszFileName, nSize, pszReturn, _TRUNCATE);
            free(pszReturn);
        }
    }
    else if(m_nPlayerType == PLAYER_LILITH ||
            m_nPlayerType == PLAYER_ULILITH){
        //filepath => filename �̏��Ɏ擾(�ꔭ�Ŏ擾������@���Ȃ�)
        RequestCommand(_T("/fileinfo filepath"), &pszReturn);
        if(!pszReturn){
            return FALSE;
        }
        if(!pszReturn[0]){
            free(pszReturn);
            return FALSE;
        }
        TCHAR *pszReturn2;
        RequestCommand(_T("/fileinfo filename"), &pszReturn2);
        if(!pszReturn2){
            free(pszReturn);
            return FALSE;
        }
        if(pszReturn2[0]){
            _sntprintf_s(pszFileName, nSize, _TRUNCATE, _T("%s\\%s"), pszReturn, pszReturn2);
        }
        free(pszReturn2);
        free(pszReturn);
    }
    return pszFileName[0] != 0;
}
///////////////////////////////////////////////////////////////////////////////
