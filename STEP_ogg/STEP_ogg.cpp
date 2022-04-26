// STEP_ogg.cpp : DLL �p�̏����������̒�`���s���܂��B
//

#include "stdafx.h"
#include "STEP_ogg.h"
#include "STEPlugin.h"
#include "FileOGG.h"
#include "Tag_Ogg.h"

#include "DlgSetup.h"
#include "..\SuperTagEditor\INI\ini.h"
//�ݒ�̓ǂݏ���
//WritePrivateProfileString �̓t�@�C�������݂��Ȃ��ꍇ��A
//���t�@�C���� ANSI ���� ANSI �ŕ��������������
//�g���h���̂� STEP �{�̂� INI �ǂݏ����N���X���g����
//UTF8/UTF16/ANSI �Ή�

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	����!
//
//		���� DLL �� MFC DLL �ɑ΂��ē��I�Ƀ����N�����ꍇ�A
//		MFC ���ŌĂяo����邱�� DLL ����G�N�X�|�[�g���ꂽ
//		�ǂ̊֐����֐��̍ŏ��ɒǉ������ AFX_MANAGE_STATE 
//		�}�N�����܂�ł��Ȃ���΂Ȃ�܂���B
//
//		��:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �ʏ�֐��̖{�̂͂��̈ʒu�ɂ���܂�
//		}
//
//		���̃}�N�����e�֐��Ɋ܂܂�Ă��邱�ƁAMFC ����
//		�ǂ̌Ăяo�����D�悷�邱�Ƃ͔��ɏd�v�ł��B
//		����͊֐����̍ŏ��̃X�e�[�g�����g�łȂ���΂�
//		��Ȃ����Ƃ��Ӗ����܂��A�R���X�g���N�^�� MFC 
//		DLL ���ւ̌Ăяo�����s���\��������̂ŁA�I�u
//		�W�F�N�g�ϐ��̐錾�����O�łȂ���΂Ȃ�܂���B
//
//		�ڍׂɂ��Ă� MFC �e�N�j�J�� �m�[�g 33 �����
//		58 ���Q�Ƃ��Ă��������B
//

/////////////////////////////////////////////////////////////////////////////
// CSTEP_oggApp

BEGIN_MESSAGE_MAP(CSTEP_oggApp, CWinApp)
	//{{AFX_MSG_MAP(CSTEP_oggApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTEP_oggApp �̍\�z

CSTEP_oggApp::CSTEP_oggApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	// ������ InitInstance �̒��̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CSTEP_oggApp �I�u�W�F�N�g

CSTEP_oggApp theApp;

UINT nPluginID;
UINT nFileTypeOGG;
UINT nFileTypeOGA;
UINT nFileTypeOPUS;

CString strINI;
bool bOptGenreListSelect;

STEP_API LPCTSTR WINAPI STEPGetPluginInfo(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return _T("Version 1.00 Copyright (C) 2003-2005 haseta\r\n")
           _T("Version 1.02 Copyright (C) 2016 Kobarin\r\n")
           _T("OggVorbis/OggOpus�`�����T�|�[�g���Ă��܂�\r\n")
           _T("OggOpus �͏��擾�̂ݑΉ�(���������͖��Ή�)�ƂȂ�܂�");
}

STEP_API bool WINAPI STEPInit(UINT pID, LPCTSTR szPluginFolder)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (Initialize() == false)	return false;
	nPluginID = pID;

	// INI�t�@�C���̓ǂݍ���
	strINI = szPluginFolder;
	strINI += _T("STEP_ogg.ini");
	CIniFile iniFile(strINI);
    bOptGenreListSelect = iniFile.ReadInt(_T("OGGVorbis"), _T("GenreListSelect"), 0) != 0;

	HBITMAP hOGGBitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_OGG));
	HBITMAP hOGABitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_OGG));
	nFileTypeOGG = STEPRegisterExt(nPluginID, _T("ogg"), hOGGBitmap);
	nFileTypeOGA = STEPRegisterExt(nPluginID, _T("oga"), hOGABitmap);
	nFileTypeOPUS = STEPRegisterExt(nPluginID, _T("opus"), hOGABitmap);
	DeleteObject(hOGGBitmap);
	DeleteObject(hOGABitmap);

	return true;
}

STEP_API void WINAPI STEPFinalize() {
	Finalize();
}

STEP_API UINT WINAPI STEPGetAPIVersion(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return STEP_API_VERSION;
}

STEP_API LPCTSTR WINAPI STEPGetPluginName(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return _T("STEP_ogg");
}

STEP_API bool WINAPI STEPSupportSIF(UINT nFormat) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return true;
}

STEP_API bool WINAPI STEPSupportTrackNumberSIF(UINT nFormat) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return true;
}

STEP_API CONTROLTYPE WINAPI STEPGetControlType(UINT nFormat, COLUMNTYPE nColumn, bool isEditSIF)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (!isEditSIF)	return _NULL;
    if(nFormat == nFileTypeOPUS){//���擾�݂̂̑Ή�
        return _NULL;
    }
	switch (nColumn) {
	case COLUMN_TRACK_NAME:
	case COLUMN_ARTIST_NAME:
	case COLUMN_ALBUM_NAME:
    case COLUMN_ALBM_ARTIST:
	case COLUMN_TRACK_NUMBER:
    case COLUMN_TRACK_TOTAL:
    case COLUMN_DISC_NUMBER:
    case COLUMN_DISC_TOTAL:
	case COLUMN_YEAR:
    case COLUMN_COPYRIGHT:
    case COLUMN_WRITER:
	case COLUMN_COMPOSER:
    case COLUMN_SOFTWARE:

		return _EDIT;
	case COLUMN_COMMENT:
		return _MEDIT;
	case COLUMN_GENRE:
		if (bOptGenreListSelect) {
			return _CBOX;
		} else {
			return _EDIT;
		}
	}
	return _NULL;
}

STEP_API UINT WINAPI STEPGetColumnMax(UINT nFormat, COLUMNTYPE nColumn, bool isEditSIF) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	switch (nColumn) {
    case COLUMN_TRACK_NAME:
	case COLUMN_ARTIST_NAME:
    case COLUMN_ALBUM_NAME:
    case COLUMN_ALBM_ARTIST:
	case COLUMN_YEAR:
	case COLUMN_GENRE:
	case COLUMN_COMMENT:    
    case COLUMN_COPYRIGHT:
    case COLUMN_WRITER:
	case COLUMN_COMPOSER:
    case COLUMN_SOFTWARE:
        return 1024;
    case COLUMN_TRACK_NUMBER:
    case COLUMN_TRACK_TOTAL:
    case COLUMN_DISC_NUMBER:
    case COLUMN_DISC_TOTAL:     
        return 32;
	}
	return 0;
}


STEP_API UINT WINAPI STEPLoad(FILE_INFO *pFileMP3, LPCTSTR szExt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if ((_tcsicmp(szExt, _T("ogg")) == 0) || 
        (_tcsicmp(szExt, _T("oga")) == 0)) {
		if (LoadFileOGG(pFileMP3) == false) {
            CString	strMsg;
			strMsg.Format(_T("%s �̓ǂݍ��݂Ɏ��s���܂���"), GetFullPath(pFileMP3));
			MessageBox(NULL, strMsg, _T("OggVorbis�t�@�C���̓ǂݍ��ݎ��s"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return STEP_ERROR;
		} else {
			if(_tcsicmp(szExt, _T("ogg")) == 0){
				SetFormat(pFileMP3, nFileTypeOGG);
				SetFileTypeName(pFileMP3, _T("OggVorbis"));
				return STEP_SUCCESS;
			}
			if(_tcsicmp(szExt, _T("oga")) == 0){
				SetFormat(pFileMP3, nFileTypeOGA);
				SetFileTypeName(pFileMP3, _T("OggVorbis"));
				return STEP_SUCCESS;
			}
		}
	}
    if(_tcsicmp(szExt, _T("opus")) == 0){
		if (LoadFileOPUS(pFileMP3) == false) {
            CString	strMsg;
			strMsg.Format(_T("%s �̓ǂݍ��݂Ɏ��s���܂���"), GetFullPath(pFileMP3));
			MessageBox(NULL, strMsg, _T("Ogg Opus�t�@�C���̓ǂݍ��ݎ��s"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return STEP_ERROR;
		} 
        else {
            SetFormat(pFileMP3, nFileTypeOPUS);
    		SetFileTypeName(pFileMP3, _T("OggOpus"));
			return STEP_SUCCESS;
		}
    }
	return STEP_UNKNOWN_FORMAT;
}

STEP_API UINT WINAPI STEPSave(FILE_INFO *pFileMP3)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UINT nFormat = GetFormat(pFileMP3);

	if ((nFormat == nFileTypeOGG)|(nFormat == nFileTypeOGA)) {
		if (WriteFileOGG(pFileMP3) == false) {
			CString	strMsg;
			strMsg.Format(_T("%s �̏������݂Ɏ��s���܂���"), GetFullPath(pFileMP3));
			MessageBox(NULL, strMsg, _T("OggVorbis�t�@�C���̏������ݎ��s"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return STEP_ERROR;
		}
		return STEP_SUCCESS;
	}
	else if (nFormat == nFileTypeOPUS) {
		if (WriteFileOPUS(pFileMP3) == false) {
			CString	strMsg;
			strMsg.Format(_T("%s �̏������݂Ɏ��s���܂���"), GetFullPath(pFileMP3));
			MessageBox(NULL, strMsg, _T("OggOpus�t�@�C���̏������ݎ��s"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return STEP_ERROR;
		}
		return STEP_SUCCESS;
	}
	return STEP_UNKNOWN_FORMAT;
}

STEP_API void WINAPI STEPShowOptionDialog(HWND hWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CDlgSetup dlg1;
	CPropertySheet page;
	dlg1.m_bGenreListSelect = bOptGenreListSelect;
	page.AddPage(&dlg1);
	page.SetTitle(CString(STEPGetPluginName()) + _T(" �I�v�V�����ݒ�"));
	if (page.DoModal() == IDOK) {
		bOptGenreListSelect = dlg1.m_bGenreListSelect ? true : false;
        CIniFile iniFile(strINI);
        iniFile.WriteInt(_T("OGGVorbis"), _T("GenreListSelect"), bOptGenreListSelect);
        iniFile.Flush();//�ۑ����s
	}
}

STEP_API void WINAPI STEPInitFileSpecificInfo(FILE_INFO* pFileMP3) {
	CTag_Ogg* fileOGG = (CTag_Ogg*)GetFileSpecificInfo(pFileMP3);
	if (fileOGG != NULL)	delete fileOGG;
}