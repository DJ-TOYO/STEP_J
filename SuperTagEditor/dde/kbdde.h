//---------------------------------------------------------------------------
#ifndef kbddeH
#define kbddeH
//---------------------------------------------------------------------------
#include <ddeml.h>

class KbDDE
{
protected:
    DWORD m_ddeInst;
    HSZ   m_hszService;
    HSZ   m_hszTopic;
public:
    KbDDE(PFNCALLBACK pfnCallBack, LPCTSTR cszService, LPCTSTR cszTopic, DWORD afCmd);
    ~KbDDE(void);
};
///////////////////////////////////////////////////////////////////////////////
class KbDDEServer : public KbDDE
{
public:
    DWORD __fastcall QueryString(HSZ hsz, TCHAR *szBuffer, int Size);
    HDDEDATA __fastcall CreateDataHandle(LPBYTE pSrc, DWORD cb, HSZ hsz, UINT wFmt);
    KbDDEServer(PFNCALLBACK pfnCallBack, LPCTSTR cszService, LPCTSTR cszTopic);
    ~KbDDEServer(void);
};
///////////////////////////////////////////////////////////////////////////////
class KbDDEClient : public KbDDE
{
private:
    HCONV m_hConv;
    HDDEDATA ClientTransaction(
        LPBYTE pData,       // �T�[�o�[�ɓn���f�[�^�̐擪�o�C�g�̃|�C���^
        DWORD cbData,       // �f�[�^�̒���
    //    HCONV hConv,        // �ʐM�n���h��
        HSZ hszItem,        // �f�[�^���ڂ̃n���h��
        UINT wFmt,          // �N���b�v�{�[�h�t�H�[�}�b�g
        UINT wType,         // �g�����U�N�V�����^�C�v
        DWORD dwTimeout,    // �҂�����
        LPDWORD pdwResult   // �g�����U�N�V�����̌��ʂւ̃|�C���^
    );
public:
    bool __fastcall Execute(LPCTSTR cszCommand, DWORD dwTimeOut);
    bool __fastcall Request(TCHAR** ppszReturn, const TCHAR *cszCommand, DWORD dwWait, BOOL bCfTextAsUnicode);
    KbDDEClient(PFNCALLBACK pfnCallBack, LPCTSTR cszService, LPCTSTR cszTopic);
    ~KbDDEClient(void);
};
///////////////////////////////////////////////////////////////////////////////
#endif
