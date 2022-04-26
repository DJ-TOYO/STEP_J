#pragma once

//�v���C���[������Ǘ�
//SuperTagEditorView.h/cpp ���番��

class CPlayerControl
{
private:
    enum{
        COMMAND_NOADDPLAY,//�ǉ����Ȃ��ōĐ�(���Ή��̏ꍇ�� COMMAND_PLAY ���Ă�)
        COMMAND_PLAY,   //�Đ�
        COMMAND_STOP,   //��~
        COMMAND_NEXT,   //���̋�
        COMMAND_PREV,   //�O�̋�
        COMMAND_CLOSE,  //��~&�t�@�C���N���[�Y(�Đ����̋Ȃ̃^�O�X�V���o����悤��)
        COMMAND_CLEAR,  //���X�g��j��
        COMMAND_ADD,    //���X�g�ɒǉ�
        COMMAND_QUIT    //�v���C���[�I��
    };
    TCHAR m_szPlayerPath[MAX_PATH];
    int   m_nPlayerType;
    //�Ή��v���C���[�𑝂₵���ꍇ�͈ȉ��̊֐���ύX����
    BOOL __fastcall PlayerIsActive(void);//�Ώۃv���C���[���N���ς݂Ȃ� TRUE ��Ԃ�
    BOOL __fastcall ExecPlayer(const TCHAR *cszFileName, BOOL bClear);//�v���C���[���N������(bClear=TRUE�̏ꍇ�͋N���ナ�X�g��j��)
    void __fastcall ExecCommand(int nCommand, const TCHAR *cszFileName);
    BOOL __fastcall SupportCommand(int nCommand);//COMMAND_XXXX �ɑΉ����Ă���� TRUE ��Ԃ�
    BOOL __fastcall RequestCommand(const TCHAR *cszCommand, TCHAR **ppszReturn);
    //�����܂�
    //�K�v�ɉ����� ExecCommandXXXX/RequestCommandXXXX ��ǉ�
    void __fastcall ExecCommandWinamp(int nCommand, const TCHAR *cszFileName);
    void __fastcall ExecCommandKbmplay(int nCommand, const TCHAR *cszFileName);
    void __fastcall ExecCommandLilith(int nCommand, const TCHAR *cszFileName);
    void __fastcall ExecCommandFoobar(int nCommand, const TCHAR *cszFileName);
    BOOL __fastcall RequestCommandKbmplay(const TCHAR *cszCommand, TCHAR **ppszReturn);
    BOOL __fastcall RequestCommandLilith(const TCHAR *cszCommand, TCHAR **ppszReturn);
public:
    CPlayerControl(void);
    CPlayerControl(const TCHAR *cszPlayerPath, int nPlayerType){
        SetPlayer(cszPlayerPath, nPlayerType);
    }
    ~CPlayerControl(void);
    void __fastcall SetPlayer(const TCHAR *cszPlayerPath, int nPlayerType);
    BOOL __fastcall SupportNoAddPlay(void){return SupportCommand(COMMAND_NOADDPLAY);}
    BOOL __fastcall SupportNext(void){return SupportCommand(COMMAND_NEXT);}
    BOOL __fastcall SupportPrev(void){return SupportCommand(COMMAND_PREV);}
    BOOL __fastcall SupportStop(void){return SupportCommand(COMMAND_STOP);}
    BOOL __fastcall SupportQuit(void){return SupportCommand(COMMAND_QUIT);}
    BOOL __fastcall SupportClose(void){return SupportCommand(COMMAND_CLOSE);}
    BOOL __fastcall SupportClear(void){return SupportCommand(COMMAND_CLEAR);}
    BOOL __fastcall SupportAdd(void){return SupportCommand(COMMAND_ADD);}
    void __fastcall Play(const TCHAR *cszFileName, BOOL bAdd);
    void __fastcall Stop(void){ExecCommand(COMMAND_STOP, NULL);}
    void __fastcall FileClose(void){ExecCommand(COMMAND_CLOSE, NULL);}
    void __fastcall Next(void){ExecCommand(COMMAND_NEXT, NULL);}
    void __fastcall Prev(void){ExecCommand(COMMAND_PREV, NULL);}
    void __fastcall Quit(void){ExecCommand(COMMAND_QUIT, NULL);}
    void __fastcall Clear(void){ExecCommand(COMMAND_CLEAR, NULL);}
    void __fastcall Add(const TCHAR *cszFileName){ExecCommand(COMMAND_ADD, cszFileName);}
    BOOL __fastcall GetPlayingFileName(TCHAR *pszFileName, int nSize);//nSize �͕�����
};
