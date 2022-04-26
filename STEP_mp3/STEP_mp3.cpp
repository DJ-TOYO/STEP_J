// STEP_mp3.cpp : DLL �p�̏����������̒�`���s���܂��B
//

#include "stdafx.h"
#include "STEP_mp3.h"
#include "Id3tagv1.h"
#include "Id3tagv2.h"
#include "RMP.h"
#include "Mp3Info.h"
#include "STEPlugin.h"

#include "DlgFileRmpID3v2.h"
#include "DlgDefaultValue.h"
#include "DlgConvID3v2Version.h"

#include "STEP_mp3_ConvFormat.h"
#include "..\SuperTagEditor\INI\ini.h"
//�ݒ�̓ǂݏ���
//WritePrivateProfileString �̓t�@�C�������݂��Ȃ��ꍇ��A
//���t�@�C���� ANSI ���� ANSI �ŕ��������������
//�g���h���̂� STEP �{�̂� INI �ǂݏ����N���X���g����
//UTF8/UTF16/ANSI �Ή�

#define STEP_API
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma pack(push, 1)
struct  ID3TAG  {                       // <<< ID3 Tag �̃f�[�^�`�� >>>
    BYTE    byData[128];                // 128�o�C�g�f�[�^
};

#define ID3_LEN_TRACK_NAME      30      // �g���b�N��    (������c30BYTE)
#define ID3_LEN_ARTIST_NAME     30      // �A�[�e�B�X�g��(������c30BYTE)
#define ID3_LEN_ALBUM_NAME      30      // �A���o����    (������c30BYTE)
#define ID3_LEN_COMMENT         30      // �R�����g      (������c30BYTE)
#define ID3_LEN_YEAR            4       // �����[�X�N��  (������c 4BYTE)

struct  ID3TAG_V10  {                           // <<< ID3 Tag v1.0 �̃f�[�^�`�� >>>
    char    sTAG[3];                            // "TAG"         (������c 3BYTE)
    char    sTrackName[ID3_LEN_TRACK_NAME];     // �g���b�N��    (������c30BYTE)
    char    sArtistName[ID3_LEN_ARTIST_NAME];   // �A�[�e�B�X�g��(������c30BYTE)
    char    sAlbumName[ID3_LEN_ALBUM_NAME];     // �A���o����    (������c30BYTE)
    char    sYear[4];                           // �����[�X�N��  (������c 4BYTE)
    char    sComment[ID3_LEN_COMMENT];          // �R�����g      (������c30BYTE)
    BYTE    byGenre;                            // �W�������ԍ�  (�����c�c 1BYTE)
};

struct  ID3TAG_V11  {                           // <<< ID3 Tag v1.1 �̃f�[�^�`�� >>>
    char    sTAG[3];                            // "TAG"         (������c 3BYTE)
    char    sTrackName[ID3_LEN_TRACK_NAME];     // �g���b�N��    (������c30BYTE)
    char    sArtistName[ID3_LEN_ARTIST_NAME];   // �A�[�e�B�X�g��(������c30BYTE)
    char    sAlbumName[ID3_LEN_ALBUM_NAME];     // �A���o����    (������c30BYTE)
    char    sYear[4];                           // �����[�X�N��  (������c 4BYTE)
    char    sComment[ID3_LEN_COMMENT-2];        // �R�����g      (������c30BYTE)
    char    cZero;                              // '\0'          (������c 1BYTE)
    BYTE    byTrackNumber;                      // �g���b�N�ԍ�  (�����c�c 1BYTE)
    BYTE    byGenre;                            // �W�������ԍ�  (�����c�c 1BYTE)
};
#pragma pack(pop)

//
//  ����!
//
//      ���� DLL �� MFC DLL �ɑ΂��ē��I�Ƀ����N�����ꍇ�A
//      MFC ���ŌĂяo����邱�� DLL ����G�N�X�|�[�g���ꂽ
//      �ǂ̊֐����֐��̍ŏ��ɒǉ������ AFX_MANAGE_STATE
//      �}�N�����܂�ł��Ȃ���΂Ȃ�܂���B
//
//      ��:
//
//      extern "C" BOOL PASCAL EXPORT ExportedFunction()
//      {
//          AFX_MANAGE_STATE(AfxGetStaticModuleState());
//          // �ʏ�֐��̖{�̂͂��̈ʒu�ɂ���܂�
//      }
//
//      ���̃}�N�����e�֐��Ɋ܂܂�Ă��邱�ƁAMFC ����
//      �ǂ̌Ăяo�����D�悷�邱�Ƃ͔��ɏd�v�ł��B
//      ����͊֐����̍ŏ��̃X�e�[�g�����g�łȂ���΂�
//      ��Ȃ����Ƃ��Ӗ����܂��A�R���X�g���N�^�� MFC
//      DLL ���ւ̌Ăяo�����s���\��������̂ŁA�I�u
//      �W�F�N�g�ϐ��̐錾�����O�łȂ���΂Ȃ�܂���B
//
//      �ڍׂɂ��Ă� MFC �e�N�j�J�� �m�[�g 33 �����
//      58 ���Q�Ƃ��Ă��������B
//

/////////////////////////////////////////////////////////////////////////////
// CSTEP_mp3App

BEGIN_MESSAGE_MAP(CSTEP_mp3App, CWinApp)
    //{{AFX_MSG_MAP(CSTEP_mp3App)
        // ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
        //        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSTEP_mp3App �̍\�z

CSTEP_mp3App::CSTEP_mp3App()
{
    // TODO: ���̈ʒu�ɍ\�z�p�̃R�[�h��ǉ����Ă��������B
    // ������ InitInstance �̒��̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

/////////////////////////////////////////////////////////////////////////////
// �B��� CSTEP_mp3App �I�u�W�F�N�g

CSTEP_mp3App theApp;

UINT nPluginID;
UINT nFileTypeMP3;
UINT nFileTypeMP3V1;
UINT nFileTypeMP3V11;
UINT nFileTypeID3V2;
UINT nFileTypeRMP;

enum    {SIF_CONV_ALL_FIELD, SIF_CONV_LENGTH_OK};

CString strINI;
// �I�v�V�����ݒ�

const bool bOptAutoConvID3v2 = true;extern const bool bOptAutoConvID3v2_default = true;
//bool    bOptAutoConvRMP;            extern const bool bOptAutoConvRMP_default = true;
int     nOptSIFieldConvType;        extern const int  nOptSIFieldConvType_default = SIF_CONV_ALL_FIELD;
bool    bOptID3v2ID3tagAutoWrite;   extern const bool bOptID3v2ID3tagAutoWrite_default = true;
bool    bOptRmpID3tagAutoWrite;     extern const bool bOptRmpID3tagAutoWrite_default = true;
bool    bOptID3v2GenreAddNumber;    extern const bool bOptID3v2GenreAddNumber_default = false;
bool    bOptChangeFileExt;          extern const bool bOptChangeFileExt_default = false;
bool    bOptID3v2GenreListSelect;   extern const bool bOptID3v2GenreListSelect_default = false; //���̃v���O�C���ɍ��킹�����肾�������s��񍐁E�v�]����������̂ŏ����l�� STEP_M �̍��ɖ߂�(�̂�����ς��߂�)(true ���� STEP_M �ȑO�Ɠ���)
bool    bOptRmpGenreListSelect;     extern const bool bOptRmpGenreListSelect_default = false;   //����(������ RMP ���g���l�͂��Ȃ��Ǝv����)
bool    bOptID3v2ID3tagAutoDelete;  extern const bool bOptID3v2ID3tagAutoDelete_default = true;
CString strOptSoftwareTag;          extern const TCHAR strOptSoftwareTag_default[] = _T("");

bool    bOptUnSync;                 extern const bool bOptUnSync_default = false;

int     nId3v2Encode;               extern const int  nId3v2Encode_default = 0; //ID3v2 �����G���R�[�h(�ύX���Ȃ�)
int     nId3v2Version;              extern const int  nId3v2Version_default = 0;//ID3v2 �o�[�W����(�ύX���Ȃ�)
int     nId3v2EncodeNew;            extern const int  nId3v2EncodeNew_default = 1; //�V�K�쐬�� ID3v2 �����G���R�[�h(UTF16)
int     nId3v2VersionNew;           extern const int  nId3v2VersionNew_default = 1;//�V�K�쐬�� ID3v2 �o�[�W����(v2.3)
int     nId3v2EncodeConv = nId3v2Encode_default;
int     nId3v2VersionConv = nId3v2Version_default;
bool    bOptUnSyncConv = bOptUnSync_default;

const bool    bAutoISO8859_1toUtf16 = true;
//��added by Kobarin
//�G���R�[�h�̎w�肪�u�ύX�Ȃ��v�ōX�V�O���uISO_8859_1�v�̂Ƃ��AUNICODE �ŗL����
//�g�p���� UTF16 �ɕϊ�����ꍇ�� true
//�ݒ�o���������ǂ���������Ȃ����A�ʓ|�Ȃ̂ŏ�ɗL��
//�G���R�[�h�̎w�肪�����I�ɁuISO_8859_1�v�ƂȂ��Ă���ꍇ�͂������D��

// �R�}���hID
//UINT nIDFileConvAutoID3;//�W��MP3/ID3v2 �`���Ɏ����ϊ�(�p�~)
UINT nIDFileConvMP3;     //ID3v1 �ɕϊ�
UINT nIDFileConvID3v2;   //ID3v2 �ɕϊ�&ID3v2�o�[�W����/�����G���R�[�h�̕ϊ�
UINT nIDFileConvRMP;     //RIFF MP3 �`���ɕϊ�
UINT nIDDeleteID3;       //ID3v1/v2 ���폜
//UINT nIDConvID3v2Version;//ID3v2�o�[�W����/�����G���R�[�h�̕ϊ�(�p�~)(ID3v2 �ɕϊ��Ɠ���)

bool ID3v1IsEmpty(FILE_INFO *pFileMP3)
{//ID3v1 ����̏ꍇ�� true ��Ԃ�
    if(GetTrackName(pFileMP3)[0]){
        return false;
    }
    if(GetArtistName(pFileMP3)[0]){
        return false;
    }
    if(GetAlbumName(pFileMP3)[0]){
        return false;
    }
    if(GetYear(pFileMP3)[0]){
        return false;
    }
    if(GetComment(pFileMP3)[0]){
        return false;
    }
    if(GetBGenre(pFileMP3) != 0xFF){
        return false;
    }
    if(GetBTrackNumber(pFileMP3) != 0xFF){
        return false;
    }
    return true;
}
#ifdef _UNICODE
static bool IsUnicodeStr(const WCHAR *str)
{//UNICODE �ŗL��(ANSI �ŕ\���o���Ȃ�)�������g�p���Ă���ꍇ�� true ��Ԃ�
    int len_ansi = WideCharToMultiByte(CP_ACP, 0, str, -1, 0, 0, NULL, NULL);
    char *str_ansi = (char*)malloc(len_ansi);
    WideCharToMultiByte(CP_ACP, 0, str, -1, str_ansi, len_ansi, NULL, NULL);
    //UTF16 �ɖ߂��Č��̕�����ƈ�v���邩�m�F
    int len_utf16 = MultiByteToWideChar(CP_ACP, 0, str_ansi, -1, 0, 0);
    WCHAR *str_utf16 = (WCHAR*)malloc(len_utf16*sizeof(WCHAR));
    MultiByteToWideChar(CP_ACP, 0, str_ansi, -1, str_utf16, len_utf16);
    bool bRet = (wcscmp(str, str_utf16) != 0);
    free(str_utf16);
    free(str_ansi);
    return bRet;
}
#endif
STEP_API LPCTSTR WINAPI STEPGetPluginInfo(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return _T("Version 1.02 Copyright (C) 2003-2006 haseta\r\n")
           _T("Version 1.04M Copyright (C) 2008-2010 Mimura\r\n")
           _T("Version 1.07 Copyright (C) 2016-2019 Kobarin\r\n")
           _T("MP3(ID3v1/ID3v2)/RIFF�`�����T�|�[�g���Ă��܂�");
}

void AddConvMenu(HMENU hMenu) {
    InsertMenu(hMenu, MF_BYPOSITION, MF_BYPOSITION | MFT_SEPARATOR, 0, NULL);
    InsertMenu(hMenu, MF_BYPOSITION, MF_BYPOSITION | MFT_STRING, nIDFileConvMP3, _T("ID3v1 �ɕϊ�(MP3)"));
    InsertMenu(hMenu, MF_BYPOSITION, MF_BYPOSITION | MFT_STRING, nIDFileConvRMP, _T("RIFF MP3 �ɕϊ�(MP3)"));
    InsertMenu(hMenu, MF_BYPOSITION, MF_BYPOSITION | MFT_STRING, nIDFileConvID3v2, _T("ID3v2 �ɕϊ�/�o�[�W�����E�����G���R�[�h�ϊ�(MP3)"));
//    InsertMenu(hMenu, MF_BYPOSITION, MF_BYPOSITION | MFT_STRING, nIDConvID3v2Version, _T("ID3v2�o�[�W����/�����G���R�[�h�̕ϊ�"));
}

void AddEditMenu(HMENU hMenu) {
    TCHAR szMenu[100];
    MENUITEMINFO mii = {0};
    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask = MIIM_TYPE;
    bool bAddDeleteID3 = false;
    for (int iPos=0;iPos<GetMenuItemCount(hMenu);iPos++) {
        mii.dwTypeData = (LPTSTR)szMenu;
        mii.cch = sizeof(szMenu)/sizeof(TCHAR)-1;
        if (GetMenuItemInfo(hMenu, iPos, true, &mii)) {
            if (mii.fType == MFT_STRING) {
                if (_tcscmp(szMenu, _T("�ҏW�O�̏�Ԃɖ߂�")) == 0) {
                    InsertMenu(hMenu, iPos, MF_BYPOSITION | MFT_STRING, nIDDeleteID3, _T("ID3v1/v2 ���폜(MP3)"));
                    bAddDeleteID3 = true;
                    iPos++;
                }
            }
        }
    }

    if (!bAddDeleteID3) {
        InsertMenu(hMenu, MF_BYPOSITION, MF_BYPOSITION | MFT_STRING, nIDDeleteID3, _T("ID3v1/v2 ���폜(MP3)"));
    }
}

extern "C" STEP_API bool WINAPI STEPInit(UINT pID, LPCTSTR szPluginFolder)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (Initialize() == false)  return false;
    nPluginID = pID;

    // INI�t�@�C���̓ǂݍ���
    strINI = szPluginFolder;
    strINI +=_T( "STEP_mp3.ini");
    CIniFile iniFile(strINI);
    //bOptAutoConvID3v2 = true;//iniFile.ReadInt(_T("RMP_ID3V2"), _T("AutoConvID3v2"), bOptAutoConvID3v2_default)!=0;
    //bOptAutoConvRMP = true;//iniFile.ReadInt(_T("RMP_ID3V2"), _T("AutoConvRMP"), bOptAutoConvRMP_default)!=0;
    nOptSIFieldConvType = iniFile.ReadInt(_T("RMP_ID3V2"), _T("SIFieldConvType"), nOptSIFieldConvType_default);
    bOptID3v2ID3tagAutoWrite = iniFile.ReadInt(_T("RMP_ID3V2"), _T("ID3v2ID3tagAutoWrite"), bOptID3v2ID3tagAutoWrite_default) != 0;
    bOptRmpID3tagAutoWrite = iniFile.ReadInt(_T("RMP_ID3V2"), _T("RmpID3tagAutoWrite"), bOptRmpID3tagAutoWrite_default) != 0;
    bOptID3v2GenreListSelect = iniFile.ReadInt(_T("RMP_ID3V2"), _T("ID3v2GenreListSelect"), bOptID3v2GenreListSelect_default) != 0;
    bOptRmpGenreListSelect = iniFile.ReadInt(_T("RMP_ID3V2"), _T("RmpGenreListSelect"), bOptRmpGenreListSelect_default) != 0;
    bOptID3v2ID3tagAutoDelete = iniFile.ReadInt(_T("RMP_ID3V2"), _T("ID3v2ID3tagAutoDelete"), bOptID3v2ID3tagAutoDelete_default) != 0;
    bOptID3v2GenreAddNumber = false;// 2005.08.23 iniFile.ReadInt("RMP_ID3V2", "ID3v2GenreAddNumber", bOptID3v2GenreAddNumber_default) != 0;
    bOptChangeFileExt = iniFile.ReadInt(_T("RMP_ID3V2"), _T("ChangeFileExt"), bOptChangeFileExt_default) != 0;
    iniFile.ReadStr(_T("OTHER"), _T("SoftwareTag"), __T(""), strOptSoftwareTag.GetBufferSetLength(255+1), 255);
    strOptSoftwareTag.ReleaseBuffer();

    bOptUnSync = iniFile.ReadInt(_T("OTHER"), _T("ID3v2UnSync"), bOptUnSync_default) != 0;

    nId3v2Encode = iniFile.ReadInt(_T("OTHER"), _T("ID3v2CharEncode"), nId3v2Encode_default);
    nId3v2Version = iniFile.ReadInt(_T("OTHER"), _T("ID3v2Version"), nId3v2Version_default);
    nId3v2EncodeNew = iniFile.ReadInt(_T("OTHER"), _T("ID3v2CharEncodeNew"), nId3v2EncodeNew_default);
    nId3v2VersionNew = iniFile.ReadInt(_T("OTHER"), _T("ID3v2VersionNew"), nId3v2VersionNew_default);

    //�l�͈̔̓`�F�b�N(���[�U�[���蓮��ini�������������ꍇ�ɔO�̈�)
    switch(nId3v2Encode){
    //�ύX�Ȃ�: ISO_8859_1: UTF-16:  UTF-8:
    case 0:         case 1: case 2: case 3: break;
    default: nId3v2Encode = nId3v2Encode_default; break;
    }
    switch (nId3v2Version) {
    //�ύX�Ȃ��F  v2.2:   v2.3:   v2.4:
    case 0:     case 1: case 2: case 3: break;
    default: nId3v2Version = nId3v2Version_default; break;
    }
    switch (nId3v2EncodeNew) {
    //ISO_8859_1: UTF-16:  UTF-8:
    case 0:       case 1: case 2:break;
    default: nId3v2EncodeNew = nId3v2EncodeNew_default; break;
    }
    switch (nId3v2VersionNew) {
    //v2.2:   v2.3:   v2.4:
    case 0: case 1: case 2: break;
    default: nId3v2VersionNew = nId3v2VersionNew_default; break;
    }
    //ID3v2.2/2.3 �� UTF8 ���g��Ȃ��悤��
    if (nId3v2Encode == 2 && nId3v2Version != 2) {
        nId3v2Encode = 1;//UTF16 �ɂ���
    }
    if (nId3v2EncodeNew == 2 && nId3v2VersionNew != 2) {
        nId3v2EncodeNew = 1;//UTF16 �ɂ���
    }
    //
    // �T�|�[�g���Ă���g���q��o�^
    HBITMAP hMP3Bitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_MP3));
    HBITMAP hMP3V1Bitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_MP3));
    HBITMAP hMP3V11Bitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_MP3));
    HBITMAP hID3v2Bitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_ID3V2));
    HBITMAP hRMPBitmap = LoadBitmap(theApp.m_hInstance, MAKEINTRESOURCE(IDB_BITMAP_RMP));
    nFileTypeMP3 = STEPRegisterExt(nPluginID, _T("mp3"), hMP3Bitmap);
    nFileTypeMP3V1 = STEPRegisterExt(nPluginID, _T("mp3"), hMP3V1Bitmap);
    nFileTypeMP3V11 = STEPRegisterExt(nPluginID, _T("mp3"), hMP3V11Bitmap);
    nFileTypeID3V2 = STEPRegisterExt(nPluginID, _T("mp3"), hID3v2Bitmap);
    nFileTypeRMP = STEPRegisterExt(nPluginID, _T("rmp"), hRMPBitmap);
    DeleteObject(hMP3Bitmap);
    DeleteObject(hMP3V1Bitmap);
    DeleteObject(hMP3V11Bitmap);
    DeleteObject(hID3v2Bitmap);
    DeleteObject(hRMPBitmap);

    // �c�[���o�[�ւ̓o�^
    COLORMAP map; // �w�i�F�̕ϊ�
    map.from = RGB(192,192,192);
    map.to = GetSysColor(COLOR_3DFACE);

    nIDFileConvMP3 = STEPGetCommandID();
    STEPAddToolBarButton(CreateMappedBitmap(theApp.m_hInstance, IDB_FILE_CONV_MP3, 0, &map, 1), nIDFileConvMP3, _T("STEP_mp3_FILE_CONV_MP3"));
    STEPKeyAssign(nIDFileConvMP3, _T("ID3v1 �ɕϊ�"), _T("STEP_mp3_KEY_FILE_CONV_MP3"));

    nIDFileConvRMP = STEPGetCommandID();
    STEPAddToolBarButton(CreateMappedBitmap(theApp.m_hInstance, IDB_FILE_CONV_RMP, 0, &map, 1), nIDFileConvRMP, _T("STEP_mp3_FILE_CONV_RMP"));
    STEPKeyAssign(nIDFileConvRMP, _T("RIFF MP3 �ɕϊ�"), _T("STEP_mp3_KEY_FILE_CONV_RMP"));

    nIDFileConvID3v2 = STEPGetCommandID();
    STEPAddToolBarButton(CreateMappedBitmap(theApp.m_hInstance, IDB_FILE_CONV_ID3V2, 0, &map, 1), nIDFileConvID3v2, _T("STEP_mp3_FILE_CONV_ID3V2"));
    STEPKeyAssign(nIDFileConvID3v2, _T("ID3v2 �ɕϊ�"), _T("STEP_mp3_KEY_FILE_CONV_ID3V2"));

    nIDDeleteID3 = STEPGetCommandID();
    STEPKeyAssign(nIDDeleteID3, _T("ID3v1/v2 ���폜"), _T("STEP_mp3_KEY_DELETE_ID3"));

    //nIDConvID3v2Version = STEPGetCommandID();
    //STEPKeyAssign(nIDConvID3v2Version, _T("ID3v2�o�[�W����/�����G���R�[�h�̕ϊ�"), _T("STEP_mp3_KEY_CONV_ID3V2_VERSION"));

    return true;
}

extern "C" STEP_API void WINAPI STEPFinalize() {
    Finalize();
}

extern "C" STEP_API UINT WINAPI STEPGetAPIVersion(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return STEP_API_VERSION;
}

extern "C" STEP_API LPCTSTR WINAPI STEPGetPluginName(void)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return _T("STEP_mp3");
}

extern "C" STEP_API bool WINAPI STEPSupportSIF(UINT nFormat) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (nFormat == nFileTypeMP3 || nFormat == nFileTypeMP3V1 || nFormat == nFileTypeMP3V11) {
        if(bOptAutoConvID3v2){
            return true;
        }
        return false;
    }
    if (nFormat == nFileTypeID3V2) {
        return true;
    }
    if (nFormat == nFileTypeRMP) {
        return true;
    }
    return true;
}

extern "C" STEP_API bool WINAPI STEPSupportTrackNumberSIF(UINT nFormat) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (nFormat == nFileTypeID3V2) {
        return true;
    }
    if ((nFormat == nFileTypeMP3  || nFormat == nFileTypeMP3V1 || nFormat == nFileTypeMP3V11) && bOptAutoConvID3v2) {
        return true;
    }
    return false;
}

extern "C" STEP_API bool WINAPI STEPSupportGenreSIF(UINT nFormat) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (nFormat == nFileTypeID3V2) {
        return true;
    }
    if (nFormat == nFileTypeRMP) {
        return true;
    }
    if ((nFormat == nFileTypeMP3  || nFormat == nFileTypeMP3V1 || nFormat == nFileTypeMP3V11) && (bOptAutoConvID3v2 /*|| bOptAutoConvRMP*/)) {
        return true;
    }
    return false;
}

extern "C" STEP_API CONTROLTYPE WINAPI STEPGetControlType(UINT nFormat, COLUMNTYPE nColumn, bool isEditSIF)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    switch (nColumn) {
    case COLUMN_TRACK_NAME:
    case COLUMN_ARTIST_NAME:
    case COLUMN_ALBUM_NAME:
    case COLUMN_TRACK_NUMBER:
    case COLUMN_YEAR:
        if (!isEditSIF && TYPE_IS_MP3(nFormat) && bOptID3v2ID3tagAutoWrite) {
            return _NULL;
        }
        if (!isEditSIF && TYPE_IS_RMP(nFormat) && bOptRmpID3tagAutoWrite) {
            return _NULL;
        }
        if (!isEditSIF && TYPE_IS_MP3(nFormat) && bOptID3v2ID3tagAutoDelete) {
            return _NULL;
        }
        return _EDIT;
    case COLUMN_GENRE:
        if (!isEditSIF && TYPE_IS_MP3(nFormat) && bOptID3v2ID3tagAutoWrite) {
            return _NULL;
        }
        if (!isEditSIF && TYPE_IS_RMP(nFormat) && bOptRmpID3tagAutoWrite) {
            return _NULL;
        }
        if (!isEditSIF && TYPE_IS_MP3(nFormat) && bOptID3v2ID3tagAutoDelete) {
            return _NULL;
        }
        if (TYPE_IS_MP3V2(nFormat) && isEditSIF) {
            if (bOptID3v2GenreListSelect) {
                return _CBOX;
            } else {
                return _EDIT;
            }
        }
        if (TYPE_IS_RMP(nFormat) && isEditSIF) {
            if (bOptRmpGenreListSelect) {
                return _CBOX;
            } else {
                return _EDIT;
            }
        }
        return _CBOX;
    case COLUMN_SOFTWARE:
    case COLUMN_COPYRIGHT:
        if (isEditSIF) {
            return _EDIT;
        } else {
            return _NULL;
        }
    case COLUMN_COMMENT:
        if (!isEditSIF && TYPE_IS_MP3(nFormat) && bOptID3v2ID3tagAutoWrite) {
            return _NULL;
        }
        if (!isEditSIF && TYPE_IS_MP3(nFormat) && bOptID3v2ID3tagAutoDelete) {
            return _NULL;
        }
        if (!isEditSIF && TYPE_IS_RMP(nFormat) && bOptRmpID3tagAutoWrite) {
            return _NULL;
        }
        if ((isEditSIF && TYPE_IS_MP3V1(nFormat) && bOptAutoConvID3v2) || (isEditSIF && TYPE_IS_MP3V2(nFormat))) {
            return _MEDIT;
        } else {
            return _EDIT;
        }
    case COLUMN_TRACK_TOTAL: //ID3v2 ��p
    case COLUMN_DISC_NUMBER:
    case COLUMN_DISC_TOTAL:
    case COLUMN_WRITER:
    case COLUMN_COMPOSER:
    case COLUMN_ALBM_ARTIST:
    case COLUMN_ORIG_ARTIST:
    case COLUMN_URL:
    case COLUMN_ENCODEST:
        if (!isEditSIF) {
            return _NULL;
        }
        if(TYPE_IS_RMP(nFormat)){
            return _NULL;
        }
        return _EDIT;
    case COLUMN_SOURCE:     //RIFF MP3 ��p
    case COLUMN_KEYWORD:
    case COLUMN_TECHNICIAN:
    case COLUMN_LYRIC:
    case COLUMN_COMMISSION:
        if (!isEditSIF) {
            return _NULL;
        }
        if (!TYPE_IS_RMP(nFormat)){
            return _EDIT;
        }
        return _NULL;
    case COLUMN_ENGINEER:
        if (!isEditSIF) {
            return _NULL;
        }
        return _EDIT;
    }
    return _NULL;
}

extern "C" STEP_API UINT WINAPI STEPGetColumnMax(UINT nFormat, COLUMNTYPE nColumn, bool isEditSIF) {
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (!isEditSIF) {
        switch (nColumn) {
        case COLUMN_TRACK_NAME:     return ID3_LEN_TRACK_NAME;
        case COLUMN_ARTIST_NAME:    return ID3_LEN_ARTIST_NAME;
        case COLUMN_ALBUM_NAME:     return ID3_LEN_ALBUM_NAME;
        case COLUMN_TRACK_NUMBER:   return 3;
        case COLUMN_YEAR:           return ID3_LEN_YEAR;
        case COLUMN_GENRE:          return 128; // ID3v1�̏ꍇ�A�W���������̂Ɠ��͂ł���ő咷�͈Ⴄ�̂�
        case COLUMN_COMMENT:        return ID3_LEN_COMMENT;
        default:                    return 0;
        }
    }
    if (((nFormat == nFileTypeMP3 || nFormat == nFileTypeMP3V1 || nFormat == nFileTypeMP3V11) && bOptAutoConvID3v2) || nFormat == nFileTypeID3V2) {
        switch (nColumn) {
        case COLUMN_TRACK_NAME:     // �g���b�N��
        case COLUMN_ARTIST_NAME:    // �A�[�e�B�X�g��
        case COLUMN_ALBUM_NAME:     // �A���o����
        case COLUMN_YEAR:           // �����[�X�N��
        case COLUMN_GENRE:          // �W�������ԍ�
        case COLUMN_COPYRIGHT:      // ���쌠
        case COLUMN_SOFTWARE:       // �\�t�g�E�F�A
        case COLUMN_WRITER:         // �쎌
        case COLUMN_COMPOSER:       // ���
        case COLUMN_ALBM_ARTIST:    // Albm.�A�[�e�B�X�g
        case COLUMN_ORIG_ARTIST:    // Orig.�A�[�e�B�X�g
        case COLUMN_URL:            // URL
        case COLUMN_ENCODEST:       // �G���R�[�h�����l
        case COLUMN_ENGINEER:       // �G���W�j�A�i�o�Łj
        case COLUMN_COMMENT:        // �R�����g
            return 2048; /* 2003.06.20 ���₵�� */
        case COLUMN_TRACK_NUMBER:   // �g���b�N�ԍ�
        case COLUMN_TRACK_TOTAL:    // �g���b�N��
        case COLUMN_DISC_NUMBER:    // �f�B�X�N�ԍ�
        case COLUMN_DISC_TOTAL:     // �f�B�X�N��
            return 32;
        }
    } else {
        switch (nColumn) {
        case COLUMN_TRACK_NUMBER:   // �g���b�N�ԍ�
        case COLUMN_TRACK_TOTAL:    // �g���b�N��
        case COLUMN_DISC_NUMBER:    // �f�B�X�N�ԍ�
        case COLUMN_DISC_TOTAL:     // �f�B�X�N��
            return 32;
        case COLUMN_TRACK_NAME:     // �g���b�N��
        case COLUMN_ARTIST_NAME:    // �A�[�e�B�X�g��
        case COLUMN_ALBUM_NAME:     // �A���o����
        case COLUMN_YEAR:           // �����[�X�N��
        case COLUMN_GENRE:          // �W�������ԍ�
        case COLUMN_COMMENT:        // �R�����g
        case COLUMN_COPYRIGHT:      // ���쌠
        case COLUMN_ENGINEER:       // �G���W�j�A(�����)
        case COLUMN_SOURCE:         // �\�[�X
        case COLUMN_SOFTWARE:       // �\�t�g�E�F�A
        case COLUMN_KEYWORD:        // �L�[���[�h
        case COLUMN_TECHNICIAN:     // �Z�p��
        case COLUMN_LYRIC:          // �̎�
            return 2048;
        }
    }
    return 0;
}

// ID3 Tag v1.0/v1.1 ���ǂ����𒲂ׂ�
static  bool    IsID3Tag(const ID3TAG *data) {
    return((((const ID3TAG_V10 *)(data))->sTAG[0] == 'T' &&
            ((const ID3TAG_V10 *)(data))->sTAG[1] == 'A' &&
            ((const ID3TAG_V10 *)(data))->sTAG[2] == 'G') ? true : false);
}
// ID3 Tag v1.0 ���ǂ����𒲂ׂ�
static  bool    IsID3Tag10(const ID3TAG *data) {
    return(((IsID3Tag(data) && (((const ID3TAG_V11 *)data)->cZero != '\0') || ((const ID3TAG_V11 *)data)->byTrackNumber == '\0')) ? true : false);
}
// ID3 Tag v1.1 ���ǂ����𒲂ׂ�
static  bool    IsID3Tag11(const ID3TAG *data) {
    return(((IsID3Tag(data) && (((const ID3TAG_V11 *)data)->cZero == '\0') && ((const ID3TAG_V11 *)data)->byTrackNumber != '\0'))? true : false);
}

void StringCopyN(TCHAR *sDest, const TCHAR *sSrc, int nLen);//sDest �̃T�C�Y�� (nLen+1) �ȏ゠�邱�Ƃ��O��

void StringCopyN(TCHAR *sDest, const TCHAR *sSrc, int nLen)
{//���� nLen ���� sDest �ɃR�s�[(ID3v2=>ID3v1, RIFF=>ID3v1�p)
 //sDest �̃T�C�Y�� nLen+1 �ȏ゠�邱�Ƃ��O��
 //nLen �� ANSI ���Z�̒����ł��邽�߁AUNICODE �łł� nLen �����R�s�[�ł͂Ȃ����Ƃɒ���
 //�Ⴆ�� sSrc �����p���������ō\������Ă���ꍇ�� nLen �����̃R�s�[�ƂȂ邪�A
 //sSrc ���S�p���������ō\������Ă���ꍇ�� nLen/2 �����̃R�s�[�ƂȂ�
 //UNICODE �łł� sSrc �� ANSI �ŕ\���o���Ȃ��������܂ނ� ? ���֕����ɒu������邱�Ƃɒ���
#ifndef _UNICODE
    _mbsncpy_s((unsigned char*)sDest, (nLen+1), (unsigned char*)sSrc, _TRUNCATE);
    //��_mbsncpy_s �Ń}���`�o�C�g�̂Q�o�C�g�ڂ��؂�邱�Ƃ͂Ȃ��������O�̂��߃`�F�b�N
    int i = 0;
    while(sDest[i]){
        if(IsDBCSLeadByte((BYTE)sDest[i])){
            if(!sDest[i+1]){//�����񂪐؂�Ă���
                sDest[i] = 0;
                break;
            }
            i += 2;
        }
        else{
            i++;
        }   
    }
#else
    CHAR *str_ansi = (CHAR*)malloc(nLen+1);
    //nLen �ɂ���ă}���`�o�C�g�̂Q�o�C�g�ڂŐ؂��ꍇ�A
    //�؂�镶��(�}���`�o�C�g1������)�� 0 ������Ȃ�
    ZeroMemory(str_ansi, nLen+1);//���Ȃ̂Ń��������[���N���A���Ă���(�ǂ��Ő؂�邩������Ȃ��̂�)
    int len_ansi = WideCharToMultiByte(CP_ACP, 0, sSrc, -1, 0, 0, NULL, NULL);
    WideCharToMultiByte(CP_ACP, 0, sSrc, -1, str_ansi, nLen, NULL, NULL);
    str_ansi[nLen] = 0;
    //�}���`�o�C�g�̂Q�o�C�g�ڂŐ؂�Ă��邩�ǂ����m�F
    //(OS �ɂ���ċ������قȂ邩������Ȃ��̂�)
    int i = 0;
    while(str_ansi[i]){
        if(IsDBCSLeadByte((BYTE)str_ansi[i])){
            if(!str_ansi[i+1]){//�����񂪐؂�Ă���
                str_ansi[i] = 0;
                break;
            }
            i += 2;
        }
        else{
            i++;
        }   
    }
    MultiByteToWideChar(CP_ACP, 0, str_ansi, -1, sDest, nLen);
    free(str_ansi);
    sDest[nLen] = 0;
#endif
}

void setFileType(CId3tagv2& id3v2, FILE_INFO *pFileMP3)
{
    CString strFileTypeName;
    switch(id3v2.GetVer()){
    case 0x0200:
        strFileTypeName = _T("MP3(ID3v2.2");
        break;
    case 0x0300:
        strFileTypeName = _T("MP3(ID3v2.3");
        break;
    case 0x0400:
        strFileTypeName = _T("MP3(ID3v2.4");
        break;
    default:
        strFileTypeName = _T("MP3(ID3v2.?");
        break;
    }
    if (GetFormat(pFileMP3) == nFileTypeMP3V1) {
        strFileTypeName = strFileTypeName + _T("+v1.0");
    }
    if (GetFormat(pFileMP3) == nFileTypeMP3V11) {
        strFileTypeName = strFileTypeName + _T("+v1.1");
    }
    strFileTypeName = strFileTypeName + _T(")");
    if (id3v2.GetCharEncoding() == CId3tagv2::ID3V2CHARENCODING_ISO_8859_1) {
    } else if (id3v2.GetCharEncoding() == CId3tagv2::ID3V2CHARENCODING_UTF_16) {
        strFileTypeName = strFileTypeName + _T(",UTF16");
    } else if (id3v2.GetCharEncoding() == CId3tagv2::ID3V2CHARENCODING_UTF_16BE) {
        strFileTypeName = strFileTypeName + _T(",UTF16BE");
    } else if (id3v2.GetCharEncoding() == CId3tagv2::ID3V2CHARENCODING_UTF_8) {
        strFileTypeName = strFileTypeName + _T(",UTF8");
    }
    if (id3v2.GetUnSynchronization()) {
        strFileTypeName = strFileTypeName + _T(",US");
    }
    SetFileTypeName(pFileMP3, strFileTypeName);
    SetFormat(pFileMP3, nFileTypeID3V2);        // �t�@�C���`���FMP3(ID3v2)
}

bool ReadTagID3(LPCTSTR sFileName, FILE_INFO *pFileMP3)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CId3tagv1 id3v1;
    id3v1.Load(sFileName);
    if(id3v1.IsEnable()){
        SetTrackName(pFileMP3, id3v1.GetTitle());  //�g���b�N��
        SetArtistName(pFileMP3, id3v1.GetArtist());//�A�[�e�B�X�g��
        SetAlbumName(pFileMP3, id3v1.GetAlbum());  //�A���o����
        SetYear(pFileMP3, id3v1.GetYear());        //�����[�X�N��
        SetComment(pFileMP3, id3v1.GetComment());  //�R�����g
        CString strTrackNo = id3v1.GetTrackNo();
        SetTrackNumber(pFileMP3, strTrackNo);
        if(!strTrackNo.IsEmpty()){
            SetFormat(pFileMP3, nFileTypeMP3V11);   // �t�@�C���`���FMP3V1.1
            SetFileTypeName(pFileMP3, _T("MP3(ID3v1.1)"));
        } else {
            SetFormat(pFileMP3, nFileTypeMP3V1);    // �t�@�C���`���FMP3V1.0
            SetFileTypeName(pFileMP3, _T("MP3(ID3v1.0)"));
        }
        SetGenre(pFileMP3, id3v1.GetGenre());
        // �W�������ԍ�
        //SetBGenre(pFileMP3, pTag->byGenre);
        //SetGenre(pFileMP3, STEPGetGenreNameSIF(pTag->byGenre));
    }
    else {
    //���̊֐����Ă΂ꂽ���_�Ńt�@�C���I�[�v���͐������Ă��锤�Ȃ̂�
    //id3tag �̗L���Ɋւ�炸�u�t�@�C���`���FMP3�v�Ƃ���
        SetFileTypeName(pFileMP3, _T("MP3"));
        SetFormat(pFileMP3, nFileTypeMP3);  // �t�@�C���`���FMP3
    }
    return true;
}

bool ReadTagID3v2(LPCTSTR sFileName, FILE_INFO *pFileMP3)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    // ID3v2 �^�O�̎擾
    CId3tagv2   id3v2/*(USE_SCMPX_GENRE_ANIMEJ)*/;
    if (id3v2.Load(sFileName) != ERROR_SUCCESS// �ǂݍ��ݎ��s
    ||  id3v2.IsEnable() == FALSE           // ID3v2 �ł͂Ȃ�
    ||  !TRUE/*id3v2.IsSafeVer()*/) {               // ���Ή��̃o�[�W����
        // �ǂݍ��ݎ��s
        if (id3v2.GetVer() > 0x0000 && !TRUE/*id3v2.IsSafeVer()*/) {
            return(true);
        }
        return(false);
    }

    // *** �^�O���̐ݒ� ***
    SetTrackNameSI(pFileMP3, id3v2.GetTitle());     // �^�C�g��
    SetArtistNameSI(pFileMP3, id3v2.GetArtist());   // �A�[�e�B�X�g��
    SetAlbumNameSI(pFileMP3, id3v2.GetAlbum());     // �A���o����
    SetYearSI(pFileMP3, id3v2.GetYear());           // �����[�X
    SetCommentSI(pFileMP3, id3v2.GetComment());     // �R�����g
    SetGenreSI(pFileMP3, id3v2.GetGenre());         // �W��������
    {//�g���b�N�ԍ�
     // x/y �`���ɂȂ��Ă���� y �̕����g���b�N���Ƃ��ď���
        CString strTrackNo = id3v2.GetTrackNo();
        TCHAR *trk_number = strTrackNo.GetBuffer();
        TCHAR *trk_total = _tcschr(trk_number, _T('/'));
        if(trk_total){
            *trk_total++ = 0;
        }
        SetTrackNumberSI(pFileMP3, trk_number);
        if(trk_total){
            SetTrackTotalSI(pFileMP3, trk_total);
        }
        strTrackNo.ReleaseBuffer();
    }
    {//�f�B�X�N�ԍ�
     //x/y �`���ɂȂ��Ă���� y �̕����f�B�X�N���Ƃ��ď���
        CString strDiscNo = id3v2.GetDiscNo();
        TCHAR *disc_number = strDiscNo.GetBuffer();
        TCHAR *disc_total = _tcschr(disc_number, _T('/'));
        if(disc_total){
            *disc_total++ = 0;
        }
        SetDiscNumberSI(pFileMP3, disc_number);
        if(disc_total){
            SetDiscTotalSI(pFileMP3, disc_total);
        }
        strDiscNo.ReleaseBuffer();
    }
    SetSoftwareSI(pFileMP3, id3v2.GetEncoder());        // �G���R�[�_
    SetCopyrightSI(pFileMP3, id3v2.GetCopyright());     // ���쌠
    SetWriterSI(pFileMP3,id3v2.GetWriter());            // �쎌
    SetComposerSI(pFileMP3, id3v2.GetComposer());       // ���
    SetAlbumArtistSI(pFileMP3, id3v2.GetAlbumArtist()); // Albm. �A�[�e�B�X�g
    SetOrigArtistSI(pFileMP3, id3v2.GetOrigArtist());   // Orig.�A�[�e�B�X�g
    SetURLSI(pFileMP3, id3v2.GetUrl());                 // URL
    SetEncodest(pFileMP3, id3v2.GetEncodedBy());        // �G���R�[�h�����l
    SetEngineerSI(pFileMP3,id3v2.GetEngineer());        // �G���W�j�A�i�o�Łj

//  SetFileTypeName(pFileMP3, "MP3(ID3v2)");
    setFileType(id3v2, pFileMP3);
    return(true);
}

bool ReadTagSIF(LPCTSTR sFileName, FILE_INFO *pFileMP3)
{
    CRMP rmp/*(USE_SCMPX_GENRE_ANIMEJ)*/;
    if (rmp.Load(GetFullPath(pFileMP3))  != ERROR_SUCCESS   // �ǂݍ��ݎ��s
        ||  rmp.IsEnable() == FALSE) {
        // �ǂݍ��ݎ��s
        return(false);
    }

    // �ǂݍ��ݐ���
    // SI �t�B�[���h
    SetTrackNameSI(pFileMP3,    rmp.GetNAM());  // �Ȗ�
    SetArtistNameSI(pFileMP3,   rmp.GetART());  // �A�[�e�B�X�g��
    SetAlbumNameSI(pFileMP3,    rmp.GetPRD());  // ���i��
    SetCommentSI(pFileMP3,      rmp.GetCMT());  // �R�����g������
    SetYearSI(pFileMP3,         rmp.GetCRD());  // �����[�X�N��
    SetGenreSI(pFileMP3,        rmp.GetGNR());  // �W��������
    SetCopyrightSI(pFileMP3,    rmp.GetCOP());  // ���쌠
    SetEngineerSI(pFileMP3,     rmp.GetENG());  // �G���W�j�A
    SetSourceSI(pFileMP3,       rmp.GetSRC());  // �\�[�X
    SetSoftwareSI(pFileMP3,     rmp.GetSFT());  // �\�t�g�E�F�A
    SetKeywordSI(pFileMP3,      rmp.GetKEY());  // �L�[���[�h
    SetTechnicianSI(pFileMP3,   rmp.GetTCH());  // �Z�p��
    SetLyricSI(pFileMP3,        rmp.GetLYC());  // �̎�
    SetCommissionSI(pFileMP3,   rmp.GetCMS());  // �R�~�b�V����
    CString strFileTypeName = _T("RIFF MP3");
    if (GetFormat(pFileMP3) == nFileTypeMP3V1) {
        strFileTypeName = strFileTypeName + _T("+ID3v1.0");
    }
    if (GetFormat(pFileMP3) == nFileTypeMP3V11) {
        strFileTypeName = strFileTypeName + _T("+ID3v1.1");
    }
    SetFormat(pFileMP3, nFileTypeRMP);      // �t�@�C���`���FRIFF MP3
    SetFileTypeName(pFileMP3, strFileTypeName);

    //if (bOptRmpID3tagAutoWrite)
    //    SetGenreSI(pFileMP3, GetGenre(pFileMP3));

    return(true);
}

bool LoadFileMP3(FILE_INFO *pFileMP3)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    // === ID3v1.0/1.1 �`���t�@�C�� ===
    BYTE Head[4] = {0};
    CString strFileName = GetFullPath(pFileMP3);
    FILE *fp;
    _tfopen_s(&fp, strFileName, _T("rb"));
    if(!fp){
        CString strMsg;
        strMsg.Format(_T("%s ���I�[�v���ł��܂���ł���"), strFileName);
        MessageBox(NULL, strMsg, _T("�t�@�C���̃I�[�v�����s"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
        return false;
    }
    fread(Head, 1, 4, fp);
    fclose(fp);
    ReadTagID3(strFileName, pFileMP3);
    
    if (memcmp(Head, "ID3", 3) == 0) {
        // === ID3v2 �`���t�@�C�� ===
        if (bOptAutoConvID3v2/*bOptID3v2ID3tagAutoWrite*/) {
            SetGenreSI(pFileMP3, GetGenre(pFileMP3));
            SetTrackNumberSI(pFileMP3, GetTrackNumber(pFileMP3));
        }
        if (ReadTagID3v2(strFileName, pFileMP3) == false) {
            MessageBox(NULL, _T("ID3v2 �^�O�̓ǂݍ��݂Ɏ��s���܂���"), GetFullPath(pFileMP3), MB_ICONSTOP|MB_OK|MB_TOPMOST);
            return false;
        }
    }
    else if (memcmp(Head, "RIFF", 4) == 0) {
        // === RIFF MP3 �`���t�@�C��(SI�t�B�[���h�̓ǂݍ���) ===
        if (ReadTagSIF(strFileName, pFileMP3) == false) {
            MessageBox(NULL, _T("RIFF MP3 �^�O�̓ǂݍ��݂Ɏ��s���܂���"), GetFullPath(pFileMP3), MB_ICONSTOP|MB_OK|MB_TOPMOST);
            return(false);
        }
    }
    else{// === ID3v1.0/1.1 �̂� ===
        if (bOptAutoConvID3v2 /*|| bOptAutoConvRMP*/) {
            if(GetBGenre(pFileMP3) != 0xFF){
                SetGenreSI(pFileMP3, GetGenre(pFileMP3));
            }
            else{
                SetGenreSI(pFileMP3, _T(""));
            }
            SetTrackNumberSI(pFileMP3, GetTrackNumber(pFileMP3));
        }
        SetTrackNameSI(pFileMP3, GetTrackName(pFileMP3));
        SetArtistNameSI(pFileMP3, GetArtistName(pFileMP3));
        SetAlbumNameSI(pFileMP3, GetAlbumName(pFileMP3));
        SetCommentSI(pFileMP3, GetComment(pFileMP3));
        SetYearSI(pFileMP3, GetYear(pFileMP3));
    }

    // mp3infp�ɂ��擾
    //GetValues_mp3infp(pFileMP3);
    CMp3Info mp3info;
    if(mp3info.Load(GetFullPath(pFileMP3))){
        SetAudioFormat(pFileMP3, mp3info.GetFormatString());
        CString strTimeString = mp3info.GetTimeString();
        TCHAR *time = strTimeString.GetBuffer();
        //szTime = "xx:xx (xxxsec) �̂悤�ɂȂ��Ă���
        TCHAR *pszSec = _tcschr(time, _T('('));
        if(pszSec){
            pszSec++;
            TCHAR *end;
            int sec = _tcstol(pszSec, &end, 10);
            SetPlayTime(pFileMP3, sec);
        }
        strTimeString.ReleaseBuffer();
    }

    return true;
}

STEP_API UINT WINAPI STEPLoad(FILE_INFO *pFileMP3, LPCTSTR szExt)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (_tcsicmp(szExt, _T("mp3")) == 0 || _tcsicmp(szExt, _T("rmp")) == 0) {
        if (LoadFileMP3(pFileMP3) == false) {
            return STEP_ERROR;
        }
        if (GetFormat(pFileMP3) != FILE_FORMAT_UNKNOWN) {
            return STEP_SUCCESS;
        }
    }
    return STEP_UNKNOWN_FORMAT;
}

bool WriteTagID3(FILE_INFO *pFileMP3)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CId3tagv1 id3v1;

    id3v1.SetTitle(GetTrackName(pFileMP3));  //�g���b�N��
    id3v1.SetArtist(GetArtistName(pFileMP3));//�A�[�e�B�X�g��
    id3v1.SetAlbum(GetAlbumName(pFileMP3));  //�A���o����
    id3v1.SetYear(GetYear(pFileMP3));        //�����[�X�N��
    id3v1.SetComment(GetComment(pFileMP3));  //�R�����g
    BYTE byTrackNumber = GetBTrackNumber(pFileMP3);
    if(byTrackNumber != 0xFF){
        id3v1.SetTrackNo(byTrackNumber);
        // �Đݒ�
        CString strTrackNumber;
        strTrackNumber.Format(_T("%d"), byTrackNumber);
        SetTrackNumber(pFileMP3, strTrackNumber);
        SetFormat(pFileMP3, nFileTypeMP3V11);   // �t�@�C���`���FMP3V1.1
        SetFileTypeName(pFileMP3, _T("MP3(ID3v1.1)"));
    }
    else{
        // �Đݒ�
        SetFormat(pFileMP3, nFileTypeMP3V1);    // �t�@�C���`���FMP3V1.0
        SetFileTypeName(pFileMP3, _T("MP3(ID3v1.0)"));
    }
    // �W������
    BYTE byGenre = GetBGenre(pFileMP3);
    id3v1.SetGenre(byGenre);
    SetGenre(pFileMP3, STEPGetGenreNameSIF(byGenre));
    if(id3v1.Save(GetFullPath(pFileMP3)) != 0){
        CString strMsg;
        strMsg.Format(_T("%s ���I�[�v���ł��܂���ł���"), GetFullPath(pFileMP3));
        MessageBox(NULL, strMsg, _T("�t�@�C���̃I�[�v�����s"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
        return false;
    }
    return true;
}

bool ConvID3tagToSIField(FILE_INFO *pFileMP3)
{
    SetModifyFlag(pFileMP3, true);                      // �ύX���ꂽ�t���O���Z�b�g
    SetTrackNameSI(pFileMP3, GetTrackName(pFileMP3));   // �Ȗ�
    SetArtistNameSI(pFileMP3, GetArtistName(pFileMP3)); // �A�[�e�B�X�g��
    SetAlbumNameSI(pFileMP3, GetAlbumName(pFileMP3));   // ���i��
    SetCommentSI(pFileMP3, GetComment(pFileMP3));       // �R�����g������
    SetYearSI(pFileMP3, GetYear(pFileMP3));             // �����[�X�N��

#define LIMIT_TEXT_LENGTH(strID3, nLen) {           \
    TCHAR   sWorkBuffer[nLen+1];                    \
    StringCopyN(sWorkBuffer, GetValue(pFileMP3, strID3), nLen);         \
    sWorkBuffer[nLen] = '\0';                       \
    SetValue(pFileMP3, strID3, sWorkBuffer);            \
}
    // ID3 tag �̕������𒲐�(�����ϊ��ׂ̈̑Ώ�)
    LIMIT_TEXT_LENGTH(FIELD_TRACK_NAME      , ID3_LEN_TRACK_NAME);
    LIMIT_TEXT_LENGTH(FIELD_ARTIST_NAME     , ID3_LEN_ARTIST_NAME);
    LIMIT_TEXT_LENGTH(FIELD_ALBUM_NAME      , ID3_LEN_ALBUM_NAME);
    LIMIT_TEXT_LENGTH(FIELD_YEAR            , ID3_LEN_YEAR);
    if (GetBTrackNumber(pFileMP3) != (BYTE)0xff) {
        LIMIT_TEXT_LENGTH(FIELD_COMMENT, ID3_LEN_COMMENT-2);
    } else {
        LIMIT_TEXT_LENGTH(FIELD_COMMENT, ID3_LEN_COMMENT);
    }
#undef LIMIT_TEXT_LENGTH
    // �\�t�g�E�F�A�̐ݒ�
    UINT nFormat = GetFormat(pFileMP3);
    if (!(nFormat == nFileTypeMP3 || nFormat == nFileTypeMP3V1 || nFormat == nFileTypeMP3V11)) {
        if (_tcslen(GetSoftwareSI(pFileMP3)) == 0) {
            SetSoftwareSI(pFileMP3, strOptSoftwareTag);
        }
    }

    if (_tcslen(GetGenreSI(pFileMP3)) == 0 || false /* ��ɍĐݒ聨���ݒ莞�̂� */) {
        //SetGenreSI(pFileMP3, STEPGetGenreNameSIF(GetBGenre(pFileMP3)));
        SetGenreSI(pFileMP3, GetGenre(pFileMP3));
    }
    if (_tcslen(GetTrackNumberSI(pFileMP3)) == 0) { /* ���ݒ莞�̂� */
        SetTrackNumberSI(pFileMP3, GetTrackNumber(pFileMP3));
    }
    return true;
}

static void COPY_FIELD(FILE_INFO *pFileMP3, FIELDTYPE fieldType, const CString& strSIF, int len) 
{// �R�s�[�Ώۂ̃t�B�[���h���ǂ������`�F�b�N���āA�K�v�ȏꍇ�����R�s�[���܂�
 //UNIOCDE �łł� len ���������ŒP���Ȓ�����r�ł͂��܂������Ȃ��̂ŁA
 //StringCopyN �� ANSI ���Z �ł� len �����R�s�[�����ꍇ�̕�����ŕK�v���ۂ��𔻒f����
    TCHAR *sBuffer = (TCHAR*)malloc((len+1)*sizeof(TCHAR));
    StringCopyN(sBuffer, strSIF, len);//���� len �܂ŃR�s�[
    if (nOptSIFieldConvType == SIF_CONV_ALL_FIELD || //��ɃR�s�[
        _tcscmp(sBuffer, strSIF) == 0) {             //���� len �܂ŃR�s�[�������̂Ɠ����Ȃ�R�s�[
        SetValue(pFileMP3, fieldType, sBuffer);        
    }          
    free(sBuffer);
}

void ConvSIFieldToID3tag(FILE_INFO *pFileMP3)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    //TCHAR   sBuffer[256+1];

    //if (GetFormat(pFileMP3) != nFileTypeID3V2 && GetFormat(pFileMP3) != nFileTypeRMP) {
    //    return;
    //}

    SetModifyFlag(pFileMP3, TRUE);              // �ύX���ꂽ�t���O���Z�b�g

/*
// �R�s�[�Ώۂ̃t�B�[���h���ǂ������`�F�b�N���āA�K�v�ȏꍇ�����R�s�[���܂�
#define COPY_FIELD(strID3, strSIF, len) {           \
    if (nOptSIFieldConvType == SIF_CONV_ALL_FIELD   \
    || _tcslen(strSIF) <= (len)) {                  \
        StringCopyN(sBuffer, strSIF, len);          \
        sBuffer[len] = '\0';                        \
        SetValue(pFileMP3, strID3, sBuffer);        \
    }                                               \
}
*/
    //���֐���
    // �Ȗ�
    COPY_FIELD(pFileMP3, FIELD_TRACK_NAME, GetTrackNameSI(pFileMP3), ID3_LEN_TRACK_NAME);
    // �A�[�e�B�X�g��
    COPY_FIELD(pFileMP3, FIELD_ARTIST_NAME, GetArtistNameSI(pFileMP3), ID3_LEN_ARTIST_NAME);
    // �A���o����
    COPY_FIELD(pFileMP3, FIELD_ALBUM_NAME, GetAlbumNameSI(pFileMP3), ID3_LEN_ALBUM_NAME);
    // �g���b�N�ԍ� //�R�����g�ݒ����ɏ�������
    if (_tcslen(GetTrackNumberSI(pFileMP3)) == 0) {
        if (GetFormat(pFileMP3) == nFileTypeID3V2) {
            SetBTrackNumber(pFileMP3, (BYTE)0xff);
        }
    } else {
        SetBTrackNumber(pFileMP3, (BYTE)STEPGetIntegerTrackNumber(GetTrackNumberSI(pFileMP3)));
    }
    // �R�����g������
    if (GetBTrackNumber(pFileMP3) == (BYTE)0xff) {
        // ID3 v1.0
        COPY_FIELD(pFileMP3, FIELD_COMMENT, GetCommentSI(pFileMP3), ID3_LEN_COMMENT);
    } else {
        // ID3 v1.1
        COPY_FIELD(pFileMP3, FIELD_COMMENT, GetCommentSI(pFileMP3), ID3_LEN_COMMENT-2);
    }
    // �����[�X�N��
    COPY_FIELD(pFileMP3, FIELD_YEAR, GetYearSI(pFileMP3), ID3_LEN_YEAR);
    // �W������
    SetBGenre(pFileMP3, STEPGetGenreCode(GetGenreSI(pFileMP3)));
    SetGenre(pFileMP3, STEPGetGenreNameSIF(GetBGenre(pFileMP3)));
//#undef COPY_FIELD
//#undef COPY_FIELD2

    return;
}


bool WriteTagID3v2(FILE_INFO *pFileMP3)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    // ID3v2 �^�O�̎擾
    CId3tagv2   id3v2/*(USE_SCMPX_GENRE_ANIMEJ)*/;
    if (id3v2.Load(GetFullPath(pFileMP3))  != ERROR_SUCCESS // �ǂݍ��ݎ��s
    ||  id3v2.IsEnable() == FALSE           // ID3v2 �ł͂Ȃ�
    ||  !TRUE/*id3v2.IsSafeVer()*/) {               // ���Ή��̃o�[�W����
        // �ǂݍ��ݎ��s
        return(false);
    }

    switch (nId3v2Version) {
    case 1: // v2.2
        id3v2.SetVer(0x0200);
        break;
    case 2: // v2.3
        id3v2.SetVer(0x0300);
        break;
    case 3: // v2.4
        id3v2.SetVer(0x0400);
        break;
    default:
        break;
    }
    switch (nId3v2Encode) {
    case 1:
        id3v2.SetCharEncoding(CId3tagv2::ID3V2CHARENCODING_ISO_8859_1);
        break;
    case 2:
        id3v2.SetCharEncoding(CId3tagv2::ID3V2CHARENCODING_UTF_16);
        break;
    case 3:
        id3v2.SetCharEncoding(CId3tagv2::ID3V2CHARENCODING_UTF_8);
        break;
    //case 4:
    //  id3v2.SetCharEncoding(CId3tagv2::ID3V2CHARENCODING_UTF_16BE);
    //  break;
    default:
        break;
    }
    // ID3tag �̎����X�V
//  if (bOptID3v2ID3tagAutoWrite) {
//      ConvSIFieldToID3tag(pFileMP3);
//  }
    // �\�t�g�E�F�A�̐ݒ�
    if (_tcslen(GetSoftwareSI(pFileMP3)) == 0) {
        SetSoftwareSI(pFileMP3, strOptSoftwareTag);
    }

#ifdef _UNICODE
    if(nId3v2Encode == 0 && //�uID3v2 �̕ۑ��`���v-�u�����G���R�[�h�v-�u�ύX���Ȃ��v
       id3v2.GetCharEncoding() == CId3tagv2::ID3V2CHARENCODING_ISO_8859_1 && //�X�V�O�� ISO_8859_1
       bAutoISO8859_1toUtf16){
    //by Kobarin
    //�ύX���Ȃ��ݒ�ł��A�X�V�O�̕����G���R�[�h�� ISO_8859_1 �̏ꍇ�͍X�V��̕��� UNICODE �ŗL
    //�̕������g�p���Ă��邩�ǂ������`�F�b�N���A�g�p���Ă���ꍇ�� UTF16 �ŏ�������
        if(IsUnicodeStr(GetTrackNameSI(pFileMP3)) ||
           IsUnicodeStr(GetArtistNameSI(pFileMP3)) ||
           IsUnicodeStr(GetAlbumNameSI(pFileMP3)) ||
           IsUnicodeStr(GetSoftwareSI(pFileMP3)) ||
           IsUnicodeStr(GetTrackNumberSI(pFileMP3)) ||
           IsUnicodeStr(GetTrackTotalSI(pFileMP3)) ||
           IsUnicodeStr(GetDiscNumberSI(pFileMP3)) ||
           IsUnicodeStr(GetDiscTotalSI(pFileMP3)) ||
           IsUnicodeStr(GetCommentSI(pFileMP3)) ||
           IsUnicodeStr(GetCopyrightSI(pFileMP3)) ||
           IsUnicodeStr(GetWriterSI(pFileMP3)) ||
           IsUnicodeStr(GetComposerSI(pFileMP3)) ||
           IsUnicodeStr(GetAlbumArtistSI(pFileMP3)) ||
           IsUnicodeStr(GetOrigArtistSI(pFileMP3)) ||
           IsUnicodeStr(GetURLSI(pFileMP3)) ||
           IsUnicodeStr(GetEncodest(pFileMP3)) ||
           IsUnicodeStr(GetEngineerSI(pFileMP3)) ||
           IsUnicodeStr(GetGenreSI(pFileMP3)) ||
           0){
            id3v2.SetCharEncoding(CId3tagv2::ID3V2CHARENCODING_UTF_16);
        }
    }
#endif

    // *** �^�O���̐ݒ� ***
    id3v2.SetTitle(GetTrackNameSI(pFileMP3));           // �^�C�g��
    id3v2.SetArtist(GetArtistNameSI(pFileMP3));         // �A�[�e�B�X�g��
    id3v2.SetAlbum(GetAlbumNameSI(pFileMP3));           // �A���o����
    id3v2.SetYear(GetYearSI(pFileMP3));                 // �����[�X
    id3v2.SetEncoder(GetSoftwareSI(pFileMP3));          // �G���R�[�_
    {//�g���b�N�ԍ�/�g���b�N��
     //�g���b�N���p�� ID �� ID3v2 �ɂ͂Ȃ��̂� "�g���b�N�ԍ�/�g���b�N��" ��
     //�悤�ȕ�����ɕϊ����ăg���b�N���ɏ�������
        CString strTrackNumber = GetTrackNumberSI(pFileMP3);
        CString strTrackTotal = GetTrackTotalSI(pFileMP3);
        if(!strTrackNumber.IsEmpty() && !strTrackTotal.IsEmpty()){
            strTrackNumber = strTrackNumber + _T("/") + strTrackTotal;
        }
        id3v2.SetTrackNo(strTrackNumber);
    }
    {//�f�B�X�N�ԍ�/�f�B�X�N��
     //�f�B�X�N���p�� ID �� ID3v2 �ɂ͂Ȃ��̂� "�f�B�X�N�ԍ�/�f�B�X�N��" ��
     //�悤�ȕ�����ɕϊ����ăf�B�X�N���ɏ�������
        CString strDiscNumber = GetDiscNumberSI(pFileMP3);
        CString strDiscTotal = GetDiscTotalSI(pFileMP3);
        if(!strDiscNumber.IsEmpty() && !strDiscTotal.IsEmpty()){
            strDiscNumber = strDiscNumber + _T("/") + strDiscTotal;
        }
        id3v2.SetDiscNo(strDiscNumber);
    }
    id3v2.SetComment(GetCommentSI(pFileMP3));           // �R�����g
    id3v2.SetCopyright(GetCopyrightSI(pFileMP3));       // ���쌠
    id3v2.SetWriter(GetWriterSI(pFileMP3));             // �쎌
    id3v2.SetComposer(GetComposerSI(pFileMP3));         // ���
    id3v2.SetAlbumArtist(GetAlbumArtistSI(pFileMP3));   // Albm. �A�[�e�B�X�g
    id3v2.SetOrigArtist(GetOrigArtistSI(pFileMP3));     // Orig.�A�[�e�B�X�g
    id3v2.SetUrl(GetURLSI(pFileMP3));                   // URL
    id3v2.SetEncodedBy(GetEncodest(pFileMP3));          // �G���R�[�h�����l
    id3v2.SetEngineer(GetEngineerSI(pFileMP3));         // �G���W�j�A�i�o�Łj
    // �W��������
    CString strGenre;
    BYTE    byGenre;
    byGenre = STEPGetGenreCode(GetGenreSI(pFileMP3));
    if (!_tcslen(GetGenreSI(pFileMP3)) == 0) {
        if (byGenre == (BYTE)0xff || STEPIsUserGenre(GetGenreSI(pFileMP3)) || bOptID3v2GenreAddNumber == false) strGenre.Format(_T("%s"), GetGenreSI(pFileMP3));
        else                       strGenre.Format(_T("(%d)%s"), STEPGetGenreCode(GetGenreSI(pFileMP3)), GetGenreSI(pFileMP3));
    }
    id3v2.SetGenre(strGenre/* 2005.08.23 del , bOptID3v2GenreAddNumber*/);

    id3v2.SetUnSynchronization(bOptUnSync);//�t���[���񓯊���
    // *** �^�O�����X�V���� ***
    bool result = id3v2.Save(GetFullPath(pFileMP3)) == ERROR_SUCCESS;
    if (result) {
        setFileType(id3v2, pFileMP3);
    }
    return result;
}

bool WriteTagSIF(FILE_INFO *pFileMP3)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (_tcslen(GetSoftwareSI(pFileMP3)) == 0) {
        SetSoftwareSI(pFileMP3, strOptSoftwareTag);
    }

    CRMP rmp/*(USE_SCMPX_GENRE_ANIMEJ)*/;
    if (rmp.Load(GetFullPath(pFileMP3))  != ERROR_SUCCESS   // �ǂݍ��ݎ��s
        ||  rmp.IsEnable() == FALSE) {
        // �ǂݍ��ݎ��s
        return(false);
    }

    bool isNeedID3 = false;
    if (_tcslen(GetTrackName(pFileMP3)) > 0)    isNeedID3 = true;
    if (_tcslen(GetArtistName(pFileMP3))> 0)    isNeedID3 = true;
    if (_tcslen(GetAlbumName(pFileMP3)) > 0)    isNeedID3 = true;
    if (_tcslen(GetComment(pFileMP3))   > 0)    isNeedID3 = true;
    if (_tcslen(GetYear(pFileMP3))      > 0)    isNeedID3 = true;
    if (_tcslen(GetGenre(pFileMP3))     > 0)    isNeedID3 = true;
    if (_tcslen(GetTrackNumber(pFileMP3))> 0)   isNeedID3 = true;
    if (isNeedID3 && !rmp.HasId3tag()) {
        rmp.SetHasId3tag(TRUE);
    }

    // SI �t�B�[���h���X�V
    rmp.SetNAM(GetTrackNameSI(pFileMP3));   // �Ȗ�
    rmp.SetART(GetArtistNameSI(pFileMP3));  // �A�[�e�B�X�g��
    rmp.SetPRD(GetAlbumNameSI(pFileMP3));   // ���i��
    rmp.SetCMT(GetCommentSI(pFileMP3));     // �R�����g������
    rmp.SetCRD(GetYearSI(pFileMP3));        // �����[�X�N��
    rmp.SetGNR(GetGenreSI(pFileMP3));       // �W��������
    rmp.SetCOP(GetCopyrightSI(pFileMP3));   // ���쌠
    rmp.SetENG(GetEngineerSI(pFileMP3));    // �G���W�j�A
    rmp.SetSRC(GetSourceSI(pFileMP3));      // �\�[�X
    rmp.SetSFT(GetSoftwareSI(pFileMP3));    // �\�t�g�E�F�A
    rmp.SetKEY(GetKeywordSI(pFileMP3));     // �L�[���[�h
    rmp.SetTCH(GetTechnicianSI(pFileMP3));  // �Z�p��
    rmp.SetLYC(GetLyricSI(pFileMP3));       // �̎�
    rmp.SetCMS(GetCommissionSI(pFileMP3));  // �R�~�b�V����
    // SI �t�B�[���h���X�V
    bool result = rmp.Save(GetFullPath(pFileMP3)) == ERROR_SUCCESS ? true : false;
    if (result) {
        rmp.Release();
        if (isNeedID3) {
            result = WriteTagID3(pFileMP3);
        }
        SetFormat(pFileMP3, nFileTypeRMP);      // �t�@�C���`���FRIFF MP3
        SetFileTypeName(pFileMP3, _T("RIFF MP3"));
    }
    return result;
}

static bool CheckLimitId3v1(const TCHAR *str, int nLimit, bool bCheckUnicode)
{//by Kobarin
 //id3v1 �̐������Ȃ� true ���A�����𒴂��Ă����� false ��Ԃ�
 //������ str �� ANSI �ɕϊ����Ē����� nLimit �𒴂����� true ��Ԃ�
 //bCheckUnicode == true �̏ꍇ�� UNICODE �ŗL�������g���Ă��邩�ǂ��������ׂ�
 //UNICODE �łł� str �� UNICODE �ŗL�������g�p���Ă���ꍇ�� false ��Ԃ�
#ifndef _UNICODE
    return (strlen(str) <= nLimit);//�����̊m�F������ OK(str �͌��X ANSI �Ȃ̂�)
#else
    int len_ansi = WideCharToMultiByte(CP_ACP, 0, str, -1, 0, 0, NULL, NULL);
    char *str_ansi = (char*)malloc(len_ansi);
    WideCharToMultiByte(CP_ACP, 0, str, -1, str_ansi, len_ansi, NULL, NULL);
    bool bRet = (strlen(str_ansi) <= nLimit);
    if(bRet && bCheckUnicode){//������ OK
    //UTF16 �ɖ߂��Č��̕�����ƈ�v���邩�m�F
        int len_utf16 = MultiByteToWideChar(CP_ACP, 0, str_ansi, -1, 0, 0);
        WCHAR *str_utf16 = (WCHAR*)malloc(len_utf16*sizeof(WCHAR));
        MultiByteToWideChar(CP_ACP, 0, str_ansi, -1, str_utf16, len_utf16);
        if(wcscmp(str, str_utf16) != 0){//���̕�����ƕs��v
            bRet = false;//UNICODE �ŗL�������g�p���Ă���
        }
        free(str_utf16);
    }
    free(str_ansi);
    return bRet;
#endif
}

bool IsCreateID3v2(FILE_INFO *pFileMP3)
{//ID3v2 �Ɏ����ϊ����ׂ����ǂ����̔���
 //�����ɉ����� UNICODE �ŗL�������g���Ă��邩�̔��ʂ��s���悤�ɂ���
    if (!CheckLimitId3v1(GetTrackName(pFileMP3), ID3_LEN_TRACK_NAME, true)) return true;
    if (!CheckLimitId3v1(GetArtistName(pFileMP3), ID3_LEN_ARTIST_NAME, true))   return true;
    if (!CheckLimitId3v1(GetAlbumName(pFileMP3), ID3_LEN_ALBUM_NAME, true)) return true;
    if (!CheckLimitId3v1(GetComment(pFileMP3), ID3_LEN_COMMENT-2, true))        return true;
    if (!CheckLimitId3v1(GetYear(pFileMP3), ID3_LEN_YEAR, true))        return true;

    if (STEPIsUserGenre(GetGenreSI(pFileMP3)))                  return true;
    if (CString(GetComment(pFileMP3)).Find('\n') > -1)          return true;
    if (!STEPIsNumeric(GetTrackNumberSI(pFileMP3)))             return true;
    /*
    if (_tcslen(GetCopyrightSI(pFileMP3)) > 0)                  return true;    // ���쌠
    if (_tcslen(GetComposerSI(pFileMP3)) > 0)                   return true;    // ���
    if (_tcslen(GetOrigArtistSI(pFileMP3)) > 0)                 return true;    // Orig.�A�[�e�B�X�g
    if (_tcslen(GetURLSI(pFileMP3)) > 0)                            return true;    // URL
    if (_tcslen(GetEncodest(pFileMP3)) > 0)                     return true;    // �G���R�[�h�����l
    if (_tcslen(GetSoftwareSI(pFileMP3)) > 0
        && _tcscmp(GetSoftwareSI(pFileMP3), strOptSoftwareTag) != 0)        return true;    // �\�t�g�E�F�A
    */
    return false;
}

bool IsCreateRMP(FILE_INFO *pFileMP3)
{//RMP(RIFF MP3)�ɕϊ����ׂ����ǂ����̔���
 //������ ID3v1 �̐����𒴂��Ă邩�ǂ��������Ŕ���
    //UNICODE �̏ꍇ�A�����̃`�F�b�N�� _tcslen �ł͕s�\��(�}���`�o�C�g������1�����Ƃ��Đ�����̂�)
    //UNICODE �ŗL�̕������g�p���Ă��邩�ǂ����͒��ׂȂ�(�ǂ��� UNICODE �ŏ������߂Ȃ��̂�)
    if (!CheckLimitId3v1(GetTrackName(pFileMP3), ID3_LEN_TRACK_NAME, false))    return true;
    if (!CheckLimitId3v1(GetArtistName(pFileMP3), ID3_LEN_ARTIST_NAME, false))  return true;
    if (!CheckLimitId3v1(GetAlbumName(pFileMP3), ID3_LEN_ALBUM_NAME, false))    return true;
    if (!CheckLimitId3v1(GetComment(pFileMP3), ID3_LEN_COMMENT-2, false))       return true;
    if (!CheckLimitId3v1(GetYear(pFileMP3), ID3_LEN_YEAR, false))       return true;

    if (STEPIsUserGenre(GetGenreSI(pFileMP3)))                  return true;
    return false;
}

bool IsCreateID3v2SI(FILE_INFO *pFileMP3, bool bID3v1Only = FALSE)
{//SIF �̕��ɓ��͂��ꂽ�^�O��񂩂� ID3v2 �Ɏ����ϊ����ׂ����ǂ����̔��ʁH
    if (!CheckLimitId3v1(GetTrackNameSI(pFileMP3), ID3_LEN_TRACK_NAME, true))   return true;
    if (!CheckLimitId3v1(GetArtistNameSI(pFileMP3), ID3_LEN_ARTIST_NAME, true)) return true;
    if (!CheckLimitId3v1(GetAlbumNameSI(pFileMP3), ID3_LEN_ALBUM_NAME, true))   return true;
    if (!CheckLimitId3v1(GetCommentSI(pFileMP3), ID3_LEN_COMMENT-2, true))      return true;
    if (!CheckLimitId3v1(GetYearSI(pFileMP3), ID3_LEN_YEAR, true))      return true;

    if (STEPIsUserGenre(GetGenreSI(pFileMP3)))                      return true;
    if (CString(GetCommentSI(pFileMP3)).Find(_T('\n')) > -1)            return true;
    if (!STEPIsNumeric(GetTrackNumberSI(pFileMP3)))                 return true;
    //if (!STEPIsNumeric(GetDiscNumberSI(pFileMP3)))                  return true;//���l���ǂ����Ɋւ�炸 Disc�ԍ��� ID3v1 �ł͏������߂Ȃ��悤�ȁc�E�B
    if (bID3v1Only == TRUE) return false;
    if (_tcslen(GetTrackTotalSI(pFileMP3)) > 0)                     return true;    // �g���b�N��
    if (_tcslen(GetDiscNumberSI(pFileMP3)) > 0)                     return true;    // �f�B�X�N�ԍ�
    if (_tcslen(GetDiscTotalSI(pFileMP3)) > 0)                      return true;    // �f�B�X�N��
    if (_tcslen(GetCopyrightSI(pFileMP3)) > 0)                      return true;    // ���쌠
    if (_tcslen(GetComposerSI(pFileMP3)) > 0)                       return true;    // ���
    if (_tcslen(GetOrigArtistSI(pFileMP3)) > 0)                     return true;    // Orig.�A�[�e�B�X�g
    if (_tcslen(GetAlbumArtistSI(pFileMP3)) > 0)                        return true;    // Albm.�A�[�e�B�X�g
    if (_tcslen(GetWriterSI(pFileMP3)) > 0)                         return true;    // �쎌��
    if (_tcslen(GetURLSI(pFileMP3)) > 0)                                return true;    // URL
    if (_tcslen(GetEncodest(pFileMP3)) > 0)                         return true;    // �G���R�[�h�����l
    if (_tcslen(GetEngineerSI(pFileMP3)) > 0)                       return true;    // �G���W�j�A
    if (_tcslen(GetSoftwareSI(pFileMP3)) > 0
        && _tcscmp(GetSoftwareSI(pFileMP3), strOptSoftwareTag) != 0)        return true;    // �\�t�g�E�F�A
    return false;
}

/*
bool IsMatchID3SI(FILE_INFO* pFileMP3)
{
    if (CString(GetTrackName(pFileMP3)) != GetTrackNameSI(pFileMP3))        return false;
    if (CString(GetArtistName(pFileMP3)) != GetArtistNameSI(pFileMP3))      return false;
    if (CString(GetAlbumName(pFileMP3)) != GetAlbumNameSI(pFileMP3))        return false;
    if (CString(GetComment(pFileMP3)) != GetCommentSI(pFileMP3))            return false;
    if (CString(GetYear(pFileMP3)) != GetYearSI(pFileMP3))                  return false;
    if (CString(GetGenre(pFileMP3)) != GetGenreSI(pFileMP3))                return false;
    if (CString(GetTrackNumber(pFileMP3)) != GetTrackNumberSI(pFileMP3))    return false;
    return true;
}*/

void copySIField(FILE_INFO *pFileMP3)
{
    if (GetFormat(pFileMP3) != nFileTypeRMP) {
        CString strTrackNumber = GetTrackNumberSI(pFileMP3);
        if (strTrackNumber.IsEmpty()) {
            SetBTrackNumber(pFileMP3, 0xff);
        } else {
            SetBTrackNumber(pFileMP3, (BYTE)STEPGetIntegerTrackNumber(strTrackNumber));
        }
    }

    CString strGenre = STEPGetGenreNameSIF(STEPGetGenreCode(GetGenreSI(pFileMP3)));
    SetGenre(pFileMP3, strGenre);
    SetBGenre(pFileMP3, STEPGetGenreCode(GetGenreSI(pFileMP3)));
}

STEP_API UINT WINAPI STEPSave(FILE_INFO *pFileMP3)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    UINT nFormat = GetFormat(pFileMP3);

    CString strTrack = GetTrackNameSI(pFileMP3);
    
    if (nFormat == nFileTypeMP3 || nFormat == nFileTypeMP3V1 || nFormat == nFileTypeMP3V11) {
        // MP3 => ID3v2 �����t�H�[�}�b�g�ϊ�����
        copySIField(pFileMP3);
        if (MakeTagID3v2(GetFullPath(pFileMP3), NULL) == false){
            return STEP_ERROR;
        } 
        else {
            nFormat = nFileTypeID3V2;
            SetFormat(pFileMP3, nFileTypeID3V2);
        }
    }
    if (nFormat == nFileTypeID3V2) {
        // ID3tag �̎����X�V
        if (bOptID3v2ID3tagAutoWrite) {
            ConvSIFieldToID3tag(pFileMP3);
        }
        if (bOptID3v2ID3tagAutoDelete) {
            SetFormat(pFileMP3, nFileTypeID3V2);
        }
        else if (WriteTagID3(pFileMP3) == false) { // FileType�ݒ�̂���ID3v2�̑O�ɏ�������
            return STEP_ERROR;
        }
        if (WriteTagID3v2(pFileMP3) == false) {
            return STEP_ERROR;
        }
        if (bOptID3v2ID3tagAutoDelete) {
            // ID3 tag���폜
            DeleteTagID3v1(GetFullPath(pFileMP3), NULL);
            STEPInitDataID3(pFileMP3);
        }
        else if(ID3v1IsEmpty(pFileMP3)){//ID3v1 ����̏ꍇ�͍폜
            DeleteTagID3v1(GetFullPath(pFileMP3), NULL);
        }
        //LoadFileMP3(pFileMP3);//�ǉ�(�X�V��̎��ۂ̕�����f�[�^���\���ɔ��f�����悤�ɂ���)
        return STEP_SUCCESS;
    }
    if (nFormat == nFileTypeRMP) {
        // ID3tag �̎����X�V
        if (bOptRmpID3tagAutoWrite) {
            ConvSIFieldToID3tag(pFileMP3);
        }
        if (WriteTagSIF(pFileMP3) == false) {
            return STEP_ERROR;
        }
        if(ID3v1IsEmpty(pFileMP3)){//ID3v1 ����̏ꍇ�͍폜
            DeleteTagID3v1(GetFullPath(pFileMP3), NULL);
        }
        //LoadFileMP3(pFileMP3);//�ǉ�(�X�V��̎��ۂ̕�����f�[�^���\���ɔ��f�����悤�ɂ���)
        return STEP_SUCCESS;
    }
    return STEP_UNKNOWN_FORMAT;
}

STEP_API void WINAPI STEPShowOptionDialog(HWND hWnd)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CPropertySheet page;
    CDlgFileRmpID3v2 dlg1;
    CDlgDefaultValue dlg2;
    page.SetTitle(CString(STEPGetPluginName()) + " �I�v�V�����ݒ�");
    dlg1.m_bRmpID3tagAutoWrite = bOptRmpID3tagAutoWrite;
    dlg1.m_bAutoConvRMP = true;//bOptAutoConvRMP;
    dlg1.m_nSIFieldConvertType = nOptSIFieldConvType;
    dlg1.m_bRmpGenreListSelect = bOptRmpGenreListSelect;
    dlg1.m_bChangeFileExt = bOptChangeFileExt;
    dlg1.m_bAutoConvID3v2 = bOptAutoConvID3v2;
    dlg1.m_bID3v2ID3tagAutoWrite = bOptID3v2ID3tagAutoWrite;
    dlg1.m_bID3v2GenreListSelect = bOptID3v2GenreListSelect;
    dlg1.m_bID3v2GenreAddNumber = bOptID3v2GenreAddNumber;
    dlg1.m_bID3v2Id3tagAutoDelete = bOptID3v2ID3tagAutoDelete;
    page.AddPage(&dlg1);
    dlg2.m_strSoftwareTag = strOptSoftwareTag;
    dlg2.m_bID3v2UnSync = bOptUnSync;
    dlg2.m_nId3v2Encode = nId3v2Encode;
    dlg2.m_nId3v2Version = nId3v2Version;
    dlg2.m_nId3v2EncodeNew = nId3v2EncodeNew;
    dlg2.m_nId3v2VersionNew = nId3v2VersionNew;

    page.AddPage(&dlg2);
    if (page.DoModal() == IDOK) {
        bOptRmpID3tagAutoWrite = dlg1.m_bRmpID3tagAutoWrite ? true : false;
        //bOptAutoConvRMP = dlg1.m_bAutoConvRMP ? true : false;
        nOptSIFieldConvType = dlg1.m_nSIFieldConvertType;
        bOptRmpGenreListSelect = dlg1.m_bRmpGenreListSelect ? true : false;
        bOptChangeFileExt = dlg1.m_bChangeFileExt ? true : false;
        //bOptAutoConvID3v2 = dlg1.m_bAutoConvID3v2 ? true : false;
        bOptID3v2ID3tagAutoWrite = dlg1.m_bID3v2ID3tagAutoWrite ? true : false;
        bOptID3v2GenreListSelect = dlg1.m_bID3v2GenreListSelect ? true : false;
        bOptID3v2GenreAddNumber = dlg1.m_bID3v2GenreAddNumber ? true : false;
        bOptID3v2ID3tagAutoDelete = dlg1.m_bID3v2Id3tagAutoDelete ? true : false;
        strOptSoftwareTag = dlg2.m_strSoftwareTag;
        bOptUnSync = dlg2.m_bID3v2UnSync ? true : false;
        nId3v2Encode = dlg2.m_nId3v2Encode;
        nId3v2Version = dlg2.m_nId3v2Version;
        nId3v2EncodeNew = dlg2.m_nId3v2EncodeNew;
        nId3v2VersionNew = dlg2.m_nId3v2VersionNew;
        //INI�ɕۑ�
        //WritePrivateProfileString �̓t�@�C�������݂��Ȃ��ꍇ��A
        //���t�@�C���� ANSI ���� ANSI �ŕ��������������
        CIniFile iniFile(strINI);
        //iniFile.WriteInt(_T("RMP_ID3V2"), _T("AutoConvID3v2"), bOptAutoConvID3v2);//�p�~
        //iniFile.WriteInt(_T("RMP_ID3V2"), _T("AutoConvRMP"), bOptAutoConvRMP);   //�p�~
        iniFile.WriteInt(_T("RMP_ID3V2"), _T("SIFieldConvType"), nOptSIFieldConvType == SIF_CONV_ALL_FIELD);
        iniFile.WriteInt(_T("RMP_ID3V2"), _T("ID3v2ID3tagAutoWrite"), bOptID3v2ID3tagAutoWrite);
        iniFile.WriteInt(_T("RMP_ID3V2"), _T("RmpID3tagAutoWrite"), bOptRmpID3tagAutoWrite);
        iniFile.WriteInt(_T("RMP_ID3V2"), _T("ID3v2GenreListSelect"), bOptID3v2GenreListSelect);
        iniFile.WriteInt(_T("RMP_ID3V2"), _T("RmpGenreListSelect"), bOptRmpGenreListSelect);
        iniFile.WriteInt(_T("RMP_ID3V2"), _T("ID3v2ID3tagAutoDelete"), bOptID3v2ID3tagAutoDelete);
        iniFile.WriteInt(_T("RMP_ID3V2"), _T("ID3v2GenreAddNumber"), bOptID3v2GenreAddNumber);
        iniFile.WriteInt(_T("RMP_ID3V2"), _T("ChangeFileExt"), bOptChangeFileExt);
        //OTHER
        iniFile.WriteStr(_T("OTHER"), _T("SoftwareTag"), strOptSoftwareTag);
        iniFile.WriteInt(_T("OTHER"), _T("ID3v2UnSync"), bOptUnSync);
        iniFile.WriteInt(_T("OTHER"), _T("ID3v2CharEncode"), nId3v2Encode);
        iniFile.WriteInt(_T("OTHER"), _T("ID3v2Version"), nId3v2Version);
        iniFile.WriteInt(_T("OTHER"), _T("ID3v2CharEncodeNew"), nId3v2EncodeNew);
        iniFile.WriteInt(_T("OTHER"), _T("ID3v2VersionNew"), nId3v2VersionNew);
        
        iniFile.Flush();//�ۑ����s(�f�X�g���N�^�� Flush �͌Ă΂�Ȃ�)

        STEPUpdateCellInfo();
    }
}

STEP_API LPCTSTR WINAPI STEPGetToolTipText(UINT nID)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    if (nID == nIDFileConvMP3) {
        return _T("ID3v1 �ɕϊ�(�񐄏�)");
    }
    if (nID == nIDFileConvID3v2) {
        return _T("ID3v2 �ɕϊ�");
    }
    if (nID == nIDFileConvRMP) {
        return _T("RIFF MP3 �ɕϊ�(�񐄏�)");
    }
    if (nID == nIDDeleteID3) {
        return _T("ID3v1/v2 ���폜");
    }
    //if (nID == nIDConvID3v2Version) {
    //    return _T("ID3v2�o�[�W�����̕ϊ�");
    //}
    return NULL;
}

STEP_API LPCTSTR WINAPI STEPGetStatusMessage(UINT nID)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    //if (nID == nIDFileConvAutoID3) {
    //    return _T("�^�O�̓��͌���/���e�ɂ���ĕW��MP3�`���܂���ID3v2�`���̂����ꂩ�ɕϊ����܂�");
    //}
    if (nID == nIDFileConvMP3) {
        return _T("�I�𒆂� MP3 �t�@�C���� ID3v1 �ɕϊ����܂��BID3v1 �ւ̕ϊ��͂����߂��܂���B�ʏ�� ID3v2 �̎g�p�������߂��܂��B");
    }
    if (nID == nIDFileConvID3v2) {
        return _T("�I�𒆂� MP3 �t�@�C���� ID3v2 �ɕϊ����܂��BID3 �̃o�[�W�����ƕ����G���R�[�h�A�t���[���񓯊����̗L�����w��o���܂��B");
    }
    if (nID == nIDFileConvRMP) {
        return _T("�I�𒆂� MP3 �t�@�C���� RIFF MP3 �ɕϊ����܂��BRIFF MP3 �ւ̕ϊ��͂����߂��܂���B�ʏ�� ID3v2 �̎g�p�������߂��܂��B");
    }
    if (nID == nIDDeleteID3) {
        return _T("�I�𒆂� MP3 �t�@�C������ ID3v1/v2 ���폜���܂�");
    }
//    if (nID == nIDConvID3v2Version) {
//        return _T("ID3v2�o�[�W����/�����G���R�[�h�̕ϊ�");
//    }
    return NULL;
}

STEP_API bool WINAPI STEPOnUpdateCommand(UINT nID)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    //if (nID == nIDFileConvAutoID3) {
    //    if (STEPGetSelectedItem() < 0) return false;
    //    else return true;
    //}
    if (nID == nIDFileConvMP3) {
        if (STEPGetSelectedItem() < 0) return false;
        else return true;
    }
    if (nID == nIDFileConvID3v2) {
        if (STEPGetSelectedItem() < 0) return false;
        else return true;
    }
    if (nID == nIDFileConvRMP) {
        if (STEPGetSelectedItem() < 0) return false;
        else return true;
    }
    if (nID == nIDDeleteID3) {
        if (STEPGetSelectedItem() < 0) return false;
        else return true;
    }
//    if (nID == nIDConvID3v2Version) {
//        if (STEPGetSelectedItem() < 0) return false;
//        else return true;
//    }
    return false;
}

STEP_API bool WINAPI STEPOnCommand(UINT nID, HWND hWnd)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());//STEP �{�̂̊֐����Ăяo���O�ɉ������Ȃ���΂Ȃ�Ȃ�
    /*if (nID == nIDFileConvAutoID3) {
        static const TCHAR sMessage[] = _T("�I������Ă���t�@�C���� �W��MP3 �`�� �܂��� ID3v2 �`���ɕϊ����܂�\n\n")
                                        _T("�ϊ������s���Ă���낵���ł����H");
        if (MessageBox(hWnd, sMessage, _T("�W��MP3 �`��/ID3v2 �`���ɕϊ�"), MB_YESNO|MB_TOPMOST) == IDYES) {
            STEPProcessSelectedFilesForUpdate(_T("�W��MP3�`��/ID3v2 �`���ɕϊ���....."), ConvFileFormatAuto);
        }
        return true;
    }*/
    if (nID == nIDFileConvMP3) {
        static const TCHAR sMessage[] = _T("�I�𒆂� MP3 �t�@�C���� ID3v1 �ɕϊ����܂��B\n")
                                        _T("ID3v1 �̎g�p�͐������܂���B\n")
                                        _T("�ʏ�� ID3v2 �̎g�p�������߂��܂��B\n\n")
                                        _T("ID3v1 �ɂ͕����񒷂ƈ����镶���ɐ��������邽�߁A�ϊ����\r\n")
                                        _T("���̈ꕔ���������Ƃ�����܂��B\n\n")
                                        _T("�ϊ������s���Ă���낵���ł����H");
        if (MessageBox(hWnd, sMessage, _T("ID3v1 �ɕϊ�"), MB_YESNO|MB_TOPMOST) == IDYES) {
            STEPProcessSelectedFilesForUpdate(_T("ID3v1 �ɕϊ���....."), ConvFileFormatID3v1);
        }
        return true;
    }
    if (nID == nIDFileConvRMP) {
        static  const TCHAR sMessage[] = _T("�I�𒆂� MP3 �t�@�C���� RIFF MP3 �ɕϊ����܂��B\n")
                                         _T("RIFF MP3 �̎g�p�͐������܂���B\n")
                                         _T("�ʏ�� ID3v2 �̎g�p�������߂��܂��B\n\n")
                                         _T("RIFF MP3 �ɂ͈����镶���ɐ��������邽�߁A�ϊ����\r\n")
                                         _T("���̈ꕔ���������Ƃ�����܂��B\r\n")
                                         _T("�܂��Ή��\�t�g�����Ȃ��ׁA�\�t�g�ɂ���Ă̓^�O����\r\n")
                                         _T("�擾�o���Ȃ����Ƃ�����܂��B\n\n")
                                         _T("�ϊ������s���Ă���낵���ł����H");
        if (MessageBox(hWnd, sMessage, _T("RIFF MP3 �ɕϊ�"), MB_YESNO|MB_TOPMOST) == IDYES) {
            STEPProcessSelectedFilesForUpdate(_T("RIFF MP3 �ɕϊ���....."), ConvFileFormatRMP);
        }
        return true;
    }
    if (nID == nIDDeleteID3) {
        {
        static const TCHAR sMessage[] = _T("�I�𒆂� MP3 �t�@�C������ ID3v1 ���폜���܂�\n\n")
                                        _T("���s���Ă���낵���ł����H\r\n")
                                        _T("ID3v2 �̍폜�͂��̃_�C�A���O�������Ɋm�F���܂�");
        int ret = MessageBox(hWnd, sMessage, _T("ID3v1 �폜"), MB_YESNOCANCEL|MB_TOPMOST);
        if(ret == IDCANCEL){
            return true;
        }
        if(ret == IDYES) {
            STEPProcessSelectedFilesForUpdate(_T("ID3v1 ���폜��....."), DeleteId3tagv1);
        }
        }
        {
        static const TCHAR sMessage[] = _T("�I�𒆂� MP3 �t�@�C������ ID3v2 ���폜���܂�\n\n")
                                        _T("���s���Ă���낵���ł����H\r\n")
                                        _T("RIFF MP3 �̃^�O���폜����ɂ́A��x ID3v1 �� ID3v2 �ɕϊ����ĉ�����\r\n");
        if (MessageBox(hWnd, sMessage, _T("ID3v2 �폜"), MB_YESNO | MB_TOPMOST) == IDYES) {
            STEPProcessSelectedFilesForUpdate(_T("ID3v2 ���폜��....."), DeleteId3tagv2);
        }
        }
        return true;
    }
    if (nID == nIDFileConvID3v2) {
        int ret;
        int nOldId3v2Encode = nId3v2Encode;
        int nOldId3v2Version = nId3v2Version;
        bool bOldOptUnSync = bOptUnSync;
        {
            AFX_MANAGE_STATE(AfxGetStaticModuleState());//�_�C�A���O��\������Ԃ����L��
            CDlgConvID3v2Version dlg;
            dlg.m_nId3v2Encode = nId3v2Encode;
            dlg.m_nId3v2Version = nId3v2Version;
            dlg.m_bID3v2UnSync = bOptUnSync;
            ret = dlg.DoModal();
            if (ret == IDOK){
                nId3v2EncodeConv = dlg.m_nId3v2Encode;
                nId3v2VersionConv = dlg.m_nId3v2Version;
                bOptUnSyncConv = dlg.m_bID3v2UnSync;
            }
        }
        if (ret == IDOK){
            STEPProcessSelectedFilesForUpdate(_T("ID3v2 �ɕϊ� & ID3v2 �o�[�W����/�����G���R�[�h��ϊ���....."), ConvID3v2Version);
            nId3v2Encode = nOldId3v2Encode;
            nId3v2Version = nOldId3v2Version;
            bOptUnSync = bOldOptUnSync;
        }
        return true;
    }
    return false;
}

STEP_API void WINAPI STEPOnLoadMenu(HMENU hMenu, UINT nType)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    switch (nType) {
    case MENU_SELECT_FOLDER:
    case MENU_FILE_RANGE:
    case MENU_FILE_EDIT_OK:
        AddConvMenu(hMenu);
        AddEditMenu(hMenu);
        break;
    }
}

STEP_API void WINAPI STEPOnLoadMainMenu()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    // ���j���[�ւ̒ǉ�
    HMENU hMenu = STEPGetMenu(MENU_CONV);
    AddConvMenu(hMenu);
    hMenu = STEPGetMenu(MENU_EDIT);
    AddEditMenu(hMenu);
}

STEP_API bool WINAPI STEPOnConvSiFieldToId3tag(FILE_INFO* pFileInfo)
{
    ConvSIFieldToID3tag(pFileInfo);
    return false;
}