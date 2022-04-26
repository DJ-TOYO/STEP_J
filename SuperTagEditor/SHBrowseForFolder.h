// ======================================================
// �T  �v    �F �t�H���_�I���_�C�A���O�̃��b�p�N���X
// �^�[�Q�b�g�F Windows95/98/NT
// �����n    �F Visual C++ Ver 6.0
// �쐬��    �F MERCURY
// �쐬��    �F 00/06/17(�y)
// ���쌠�\�L�F Copyright(C) 2000 MERCURY.
// ======================================================

#ifndef __SH_BROWSE_FOR_FOLDER_H__
#define __SH_BROWSE_FOR_FOLDER_H__


// ======================================
// =====   �����R���p�C���t���O     =====
// ======================================



// ======================================
// =====   �C���N���[�h�t�@�C��     =====
// ======================================



// ======================================
// =====           ��  ��           =====
// ======================================



// ======================================
// =====           �}�N��           =====
// ======================================



// ======================================
// =====       typedef�^enum        =====
// ======================================



// ======================================
// =====       �\���́^���p��       =====
// ======================================



// ======================================
// =====         const �ϐ�         =====
// ======================================



// ======================================
// =====        extern �ϐ�         =====
// ======================================



// ======================================
// =====        static �ϐ�         =====
// ======================================



// ======================================
// =====     �֐��v���g�^�C�v       =====
// ======================================



// ======================================
// =====      �v���O�����̈�        =====
// ======================================


#ifdef __cplusplus

class   CSHBrowseForFolder  {
public:
    CSHBrowseForFolder(bool = false, bool = true);
    virtual ~CSHBrowseForFolder();

public:
    void  SetEnableSubDirButton(bool bFlag) {m_bEnableSubDirButton = bFlag;}
    bool  GetEnableSubDirButton(void) {return(m_bEnableSubDirButton);}
    void  SetSearchSubDirState(bool bFlag) {m_bSearchSubDirState = bFlag;}
    bool  GetSearchSubDirState(void) {return(GetEnableSubDirButton() ? m_bSearchSubDirState : false);}
    bool  Exec(TCHAR *sLocal, int size);
    void  SetCheckBoxTitle(const TCHAR* title) {_tcsncpy_s(m_szCheckBoxTitle, title, _TRUNCATE);}
    const TCHAR* GetCheckBoxTitle(void) {return m_szCheckBoxTitle;}

private:
    TCHAR    m_szInitialFolder[MAX_PATH];
    TCHAR    m_szCheckBoxTitle[256];
    CButton *m_pSubDir;
    bool     m_bEnableSubDirButton;
    bool     m_bSearchSubDirState;
    WNDPROC  m_VSSelectOrgProc;

    static  LRESULT CALLBACK VSSelectFolderSubProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT VSSelectFolderSubProcInternal(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static  int CALLBACK CallbackSelectDir(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
    int CallbackSelectDirInternal(HWND hWnd, UINT uMsg, LPARAM lParam);

protected:  // ======================================
            // =====    protected �����o�֐�    =====
            // ======================================
};

#endif

#endif
