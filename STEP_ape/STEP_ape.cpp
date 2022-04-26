// STEP_ape.cpp : DLL �p�̏����������̒�`���s���܂��B
//

#include "stdafx.h"
#include "STEP_ape.h"
#include "STEPlugin.h"

#include "Tag_Ape.h"
#include "Id3tagv1.h"
#include "DlgSetup.h"
#include "FileAPE.h"
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

#define ID3_LEN_TRACK_NAME		30		// �g���b�N��    (������c30BYTE)
#define ID3_LEN_ARTIST_NAME		30		// �A�[�e�B�X�g��(������c30BYTE)
#define ID3_LEN_ALBUM_NAME		30		// �A���o����    (������c30BYTE)
#define ID3_LEN_COMMENT			30		// �R�����g      (������c30BYTE)
#define ID3_LEN_YEAR			4		// �����[�X�N��  (������c 4BYTE)

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
// CSTEP_apeApp

BEGIN_MESSAGE_MAP(CSTEP_apeApp, CWinApp)
	//{{AFX_MSG_MAP(CSTEP_apeApp)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTEP_apeApp �̍\�z

CSTEP_apeApp::CSTEP_apeApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
	// ������ InitInstance �̒��̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CSTEP_apeApp �I�u�W�F�N�g

CSTEP_apeApp theApp;

UINT nPluginID;
//UINT nFileTypeAPE;
//UINT nFileTypeAPEID3; /* �^�O��ID3�`�� */

TCHAR g_szPluginFolder[MAX_PATH];
static struct FILE_TYPES
{
    UINT nFileTypeSIF;
    UINT nFileTypeID3;
    LPCTSTR cszBMP;
    LPCTSTR cszExt;
    LPCTSTR cszTypeName;
}g_FileTypes[] = {
    {0, 0, MAKEINTRESOURCE(IDB_BITMAP_APE), _T("ape"), _T("Monkey's Audio")},
    {0, 0, MAKEINTRESOURCE(IDB_BITMAP_MPC), _T("mpc"), _T("Musepack")},
    {0, 0, MAKEINTRESOURCE(IDB_BITMAP_MPC), _T("mp+"), _T("Musepack")},
    {0, 0, MAKEINTRESOURCE(IDB_BITMAP_APE), _T("tak"), _T("TAK")},
    {0, 0, MAKEINTRESOURCE(IDB_BITMAP_APE), _T("ofr"), _T("OptimFROG")},
    {0, 0, MAKEINTRESOURCE(IDB_BITMAP_APE), _T("ofs"), _T("OptimFROG")},
    {0, 0, MAKEINTRESOURCE(IDB_BITMAP_APE), _T("wv"), _T("WavPack")},
    {0, 0, NULL, NULL, NULL}
};

CString strINI;
bool bOptGenreListSelect;

UINT GetFileTypeFromExt(const TCHAR *cszExt)
{
    int i = 0;
    while(g_FileTypes[i].cszExt){
        if(_tcsicmp(cszExt, g_FileTypes[i].cszExt) == 0){
            return g_FileTypes[i].nFileTypeSIF;
        }
        i++;
    }
    return 0;
}
UINT GetFileTypeID3FromExt(const TCHAR *cszExt)
{
    int i = 0;
    while(g_FileTypes[i].cszExt){
        if(_tcsicmp(cszExt, g_FileTypes[i].cszExt) == 0){
            return g_FileTypes[i].nFileTypeID3;
        }
        i++;
    }
    return 0;
}
bool IsSupportTypeSIF(int nFormat)
{
    int i = 0;
    while(g_FileTypes[i].cszExt){
        if(g_FileTypes[i].nFileTypeSIF == nFormat){
            return true;
        }
        i++;
    }
    return false;
}
bool IsSupportTypeID3(int nFormat)
{
    int i = 0;
    while(g_FileTypes[i].cszExt){
        if(g_FileTypes[i].nFileTypeID3 == nFormat){
            return true;
        }
        i++;
    }
    return false;
}
bool IsSupportExt(const TCHAR *cszExt)
{
    int i = 0;
    while(g_FileTypes[i].cszExt){
        if(_tcsicmp(cszExt, g_FileTypes[i].cszExt) == 0){
            return true;
        }
        i++;
    }
    return false;
}
LPCTSTR GetFileTypeName(const TCHAR *cszExt)
{
    int i = 0;
    while(g_FileTypes[i].cszExt){
        if(_tcsicmp(cszExt, g_FileTypes[i].cszExt) == 0){
            return g_FileTypes[i].cszTypeName;
        }
        i++;
    }
    return _T("");
}
LPCTSTR GetFileTypeName(int nFormat)
{
    int i = 0;
    while(g_FileTypes[i].cszExt){
        if(nFormat == g_FileTypes[i].nFileTypeSIF){
            return g_FileTypes[i].cszTypeName;
        }
        if(nFormat == g_FileTypes[i].nFileTypeID3){
            return g_FileTypes[i].cszTypeName;
        }
        i++;
    }
    return _T("");
}

STEP_API LPCTSTR WINAPI STEPGetPluginInfo(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return _T("Version 1.01 Copyright (C) 2003-2006 haseta\r\n")
           _T("Version 1.03 Copyright (C) 2016 Kobarin\r\n")
           _T("Monkey's Audio/Musepack/TAK/WavPack/OptimFROG �`�����T�|�[�g���Ă��܂�");
}


STEP_API bool WINAPI STEPInit(UINT pID, LPCTSTR szPluginFolder)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (Initialize() == false)	return false;
	nPluginID = pID;
    _tcsncpy_s(g_szPluginFolder, szPluginFolder, _TRUNCATE);
	// INI�t�@�C���̓ǂݍ���
	strINI = szPluginFolder;
	strINI += _T("STEP_ape.ini");
    CIniFile iniFile(strINI);
	bOptGenreListSelect = iniFile.ReadInt(_T("APE"), _T("GenreListSelect"), 0) != 0;

    int i = 0;
    while(g_FileTypes[i].cszExt){
    	HBITMAP hBitmap = LoadBitmap(theApp.m_hInstance, g_FileTypes[i].cszBMP);
        g_FileTypes[i].nFileTypeSIF = STEPRegisterExt(nPluginID, g_FileTypes[i].cszExt, hBitmap);
	    g_FileTypes[i].nFileTypeID3 = STEPRegisterExt(nPluginID, g_FileTypes[i].cszExt, hBitmap);
        DeleteObject(hBitmap);
        i++;
    }
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
	return _T("STEP_ape");
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
	case COLUMN_ORIG_ARTIST:
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
	if (/*IsSupportTypeSIF(nFormat) ||*/ true) {
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
		case COLUMN_ORIG_ARTIST:
            return 1024;
		case COLUMN_TRACK_NUMBER:
		case COLUMN_TRACK_TOTAL:
        case COLUMN_DISC_NUMBER:
        case COLUMN_DISC_TOTAL:
            return 32;
		}
	/*
	} else if (nFormat == nFileTypeAPEID3) {
		switch (nColumn) {
		case COLUMN_TRACK_NAME:		return ID3_LEN_TRACK_NAME;
		case COLUMN_ARTIST_NAME:	return ID3_LEN_ARTIST_NAME;
		case COLUMN_ALBUM_NAME:		return ID3_LEN_ALBUM_NAME;
		case COLUMN_TRACK_NUMBER:	return 3;
		case COLUMN_YEAR:			return ID3_LEN_YEAR;
		case COLUMN_GENRE:			return 3;
		case COLUMN_COMMENT:		return ID3_LEN_COMMENT;
		}
	*/
	}
	return 0;
}

bool ReadTagAPE(FILE_INFO *pFileMP3, const TCHAR *cszExt)
{
	CTag_Ape ape;
	if (ape.Load(GetFullPath(pFileMP3)) != ERROR_SUCCESS) {
		return false;
	}
	
	CString buff; 
//    if(ape.IsEnable()){
	    // �g���b�N��
	    ape.GetComment(CTag_Ape::APE_TAG_FIELD_TITLE, buff);
	    SetTrackNameSI(pFileMP3, buff);
	    // �A�[�e�B�X�g��
	    ape.GetComment(CTag_Ape::APE_TAG_FIELD_ARTIST, buff);
	    SetArtistNameSI(pFileMP3, buff);
	    // �A���o����
	    ape.GetComment(CTag_Ape::APE_TAG_FIELD_ALBUM, buff);
	    SetAlbumNameSI(pFileMP3, buff);
        // �A���o���A�[�e�B�X�g
        ape.GetComment(_T("Album Artist"), buff);//Album �� Artist �̊Ԃɋ󔒂��܂�
        SetAlbumArtistSI(pFileMP3, buff);
	    // �����[�X�N��
	    ape.GetComment(CTag_Ape::APE_TAG_FIELD_YEAR, buff);
	    SetYearSI(pFileMP3, buff);
	    // �R�����g
	    ape.GetComment(CTag_Ape::APE_TAG_FIELD_COMMENT, buff);
	    SetCommentSI(pFileMP3, buff);
	    // �g���b�N�ԍ�
	    ape.GetComment(CTag_Ape::APE_TAG_FIELD_TRACK, buff);
	    // x/y �`���ɂȂ��Ă���� y �̕����g���b�N���Ƃ��ď���
        {
            TCHAR *trk_number = buff.GetBuffer();
            TCHAR *trk_total = _tcschr(trk_number, _T('/'));
            if(trk_total){
                *trk_total++ = 0;
            }
            SetTrackNumberSI(pFileMP3, trk_number);
            if(trk_total){
                SetTrackTotalSI(pFileMP3, trk_total);
            }
            buff.ReleaseBuffer();
        }
	    // �f�B�X�N�ԍ�
	    ape.GetComment(_T("Disc"), buff);
	    // x/y �`���ɂȂ��Ă���� y �̕����f�B�X�N���Ƃ��ď���
        {
            TCHAR *disc_number = buff.GetBuffer();
            TCHAR *disc_total = _tcschr(disc_number, _T('/'));
            if(disc_total){
                *disc_total++ = 0;
            }
            SetDiscNumberSI(pFileMP3, disc_number);
            if(disc_total){
                SetDiscTotalSI(pFileMP3, disc_total);
            }
            buff.ReleaseBuffer();
        }
	    // �W������
	    ape.GetComment(CTag_Ape::APE_TAG_FIELD_GENRE, buff);
	    SetGenreSI(pFileMP3, buff);
	    //SetBGenre(STEPGetGenreCode(buff));
	    // ���쌠
        ape.GetComment(_T("Copyright"), buff);
        SetCopyrightSI(pFileMP3, buff);
        // �쎌��
	    ape.GetComment(_T("Lyricist"), buff);
	    SetWriterSI(pFileMP3, buff);
	    // ��Ȏ�
	    ape.GetComment(_T("Composer"), buff);
	    SetComposerSI(pFileMP3, buff);
	    // ���t��
	    ape.GetComment(_T("Performer"), buff);
	    SetOrigArtistSI(pFileMP3, buff);
        // �\�t�g�E�F�A
        ape.GetComment(_T("Encoded By"), buff);
        SetSoftwareSI(pFileMP3, buff);
//    }
/*    else if(ape.HasId3tag()){
        CId3tagv1 id3v1;
        id3v1.Load(GetFullPath(pFileMP3));
	    SetTrackNameSI(pFileMP3, id3v1.GetTitle());
	    SetArtistNameSI(pFileMP3, id3v1.GetArtist());
	    SetAlbumNameSI(pFileMP3, id3v1.GetAlbum());
	    SetYearSI(pFileMP3, id3v1.GetYear());
	    SetCommentSI(pFileMP3, id3v1.GetComment());
        SetTrackNumberSI(pFileMP3, id3v1.GetTrackNo());
	    SetGenreSI(pFileMP3, id3v1.GetGenre());
    }
*/
	// �t�@�C���`��
    TCHAR szFileTypeName[1024];
    _tcsncpy_s(szFileTypeName, GetFileTypeName(cszExt), _TRUNCATE);
	if (!ape.IsEnable() && ape.HasId3tag()) {
		SetFormat(pFileMP3, GetFileTypeID3FromExt(cszExt));
        _tcsncat_s(szFileTypeName, _T("(ID3)"), _TRUNCATE);
	} else {
		SetFormat(pFileMP3, GetFileTypeFromExt(cszExt));
		if (ape.IsEnable()) {
			if (ape.GetApeVersion() == 1000) {
               _tcsncat_s(szFileTypeName, _T("(APE)"), _TRUNCATE);
			} else {
               _tcsncat_s(szFileTypeName, _T("(APEv2)"), _TRUNCATE);
			}
		}
	}
    SetFileTypeName(pFileMP3, szFileTypeName);
    // 
    SetAudioFormatFromExt(pFileMP3, cszExt);
	return true;
}

STEP_API UINT WINAPI STEPLoad(FILE_INFO *pFileMP3, LPCTSTR szExt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(IsSupportExt(szExt)){
        if (ReadTagAPE(pFileMP3, szExt) == false) {
			CString	strMsg;
			strMsg.Format(_T("%s �̓ǂݍ��݂Ɏ��s���܂���"), GetFullPath(pFileMP3));
            CString strTitle;
            strTitle.Format(_T("%s�t�@�C���̓ǂݍ��ݎ��s"), GetFileTypeName(szExt));
			MessageBox(NULL, strMsg, strTitle, MB_ICONSTOP|MB_OK|MB_TOPMOST);
			return STEP_ERROR;
		} else {
			return STEP_SUCCESS;
		}
	}
	return STEP_UNKNOWN_FORMAT;
}

bool WriteTagAPE(FILE_INFO *pFileMP3)
{
	CTag_Ape ape;
	if (ape.Load(GetFullPath(pFileMP3)) != ERROR_SUCCESS) {
		return false;
	}
	// �g���b�N��
	ape.SetComment(CTag_Ape::APE_TAG_FIELD_TITLE, GetTrackNameSI(pFileMP3));
	// �A�[�e�B�X�g��
	ape.SetComment(CTag_Ape::APE_TAG_FIELD_ARTIST, GetArtistNameSI(pFileMP3));
	// �A���o����
	ape.SetComment(CTag_Ape::APE_TAG_FIELD_ALBUM, GetAlbumNameSI(pFileMP3));
	// �A���o���A�[�e�B�X�g
	ape.SetComment(_T("Album Artist"), GetAlbumArtistSI(pFileMP3));
	// �����[�X�N��
	ape.SetComment(CTag_Ape::APE_TAG_FIELD_YEAR, GetYearSI(pFileMP3));
	// �R�����g
	ape.SetComment(CTag_Ape::APE_TAG_FIELD_COMMENT, GetCommentSI(pFileMP3));
	// �g���b�N�ԍ�/�g���b�N��
    {//TrackTotal �Ƃ����t�B�[���h���� Ape �ɂ͂Ȃ��̂� "�g���b�N�ԍ�/�g���b�N��" ��
     //�悤�ȕ�����ɕϊ����ď�������
        CString strTrackNumber = GetTrackNumberSI(pFileMP3);
        CString strTrackTotal = GetTrackTotalSI(pFileMP3);
        if(!strTrackNumber.IsEmpty() && !strTrackTotal.IsEmpty()){
            strTrackNumber = strTrackNumber + _T("/") + strTrackTotal;
        }
        ape.SetComment(CTag_Ape::APE_TAG_FIELD_TRACK, strTrackNumber);
    }
    // �f�B�X�N�ԍ�/�f�B�X�N��
    {//DiscTotal �Ƃ����t�B�[���h���� Ape �ɂ͂Ȃ��̂� "�f�B�X�N�ԍ�/�f�B�X�N��" ��
     //�悤�ȕ�����ɕϊ����ď�������
        CString strDiscNumber = GetDiscNumberSI(pFileMP3);
        CString strDiscTotal = GetDiscTotalSI(pFileMP3);
        if(!strDiscNumber.IsEmpty() && !strDiscTotal.IsEmpty()){
            strDiscNumber = strDiscNumber + _T("/") + strDiscTotal;
        }
        ape.SetComment(_T("Disc"), strDiscNumber);
    }
    // �W�������ԍ�
	ape.SetComment(CTag_Ape::APE_TAG_FIELD_GENRE, GetGenreSI(pFileMP3));
	// ���쌠
    ape.SetComment(_T("Copyright"), GetCopyrightSI(pFileMP3));
	// �쎌��
	ape.SetComment(_T("Lyricist"), GetWriterSI(pFileMP3));
	// ��Ȏ�
	ape.SetComment(_T("Composer"), GetComposerSI(pFileMP3));
	// ���t��
	ape.SetComment(_T("Performer"), GetOrigArtistSI(pFileMP3));
    // �\�t�g�E�F�A
	ape.SetComment(_T("Encoded By"), GetSoftwareSI(pFileMP3));
    //  ID3v1 ��ۑ����Ȃ�
    ape.SetDonotsaveId3v1(TRUE);
	if (ape.Save(GetFullPath(pFileMP3)) != ERROR_SUCCESS) {
		return false;
	}
	return true;
}

STEP_API UINT WINAPI STEPSave(FILE_INFO *pFileMP3)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	UINT nFormat = GetFormat(pFileMP3);

	if(IsSupportTypeSIF(nFormat) || IsSupportTypeID3(nFormat)){
        if (WriteTagAPE(pFileMP3) == false) {
			CString	strMsg;
            CString strTitle;
			strMsg.Format(_T("%s �̏������݂Ɏ��s���܂���"), GetFullPath(pFileMP3));
            strTitle.Format(_T("%s�t�@�C���̏������ݎ��s"), GetFileTypeName(nFormat));
			MessageBox(NULL, strMsg, strTitle, MB_ICONSTOP|MB_OK|MB_TOPMOST);
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
        iniFile.WriteInt(_T("APE"), _T("GenreListSelect"), bOptGenreListSelect);
        iniFile.Flush();//�ۑ����s
	}
}

STEP_API LPCTSTR WINAPI STEPGetColumnName(UINT nFormatType, COLUMNTYPE nColumn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	switch (nColumn) {
	case COLUMN_ORIG_ARTIST:
		return _T("���t��");
	}
	return NULL;
}

STEP_API bool WINAPI STEPHasSpecificColumnName(UINT)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return true;
}
