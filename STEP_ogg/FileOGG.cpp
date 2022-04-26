#include "stdafx.h"
#include "FileOGG.h"

#include "Tag_Ogg.h"
#include "opus.h"
#include "opusfile.h"
#include "../SuperTagEditor/strcnv.h"

bool LoadFileOGG(FILE_INFO *pFileMP3)
{
    SetFileSpecificInfo(pFileMP3, new CTag_Ogg());
    CTag_Ogg* fileOGG = (CTag_Ogg*)GetFileSpecificInfo(pFileMP3);
    if (fileOGG->Load(GetFullPath(pFileMP3)) != ERROR_SUCCESS) {
        return false;
    }

    CString strTrackName;
    CString strArtistName;
    CString strAlbumName;
    CString strAlbumArtist;
    CString strGenre;
    CString strYear;
    CString strComment;
    CString strTrackNumber;
    CString strTrackTotal;
    CString strDiscNumber;
    CString strDiscTotal;
    CString strCopyright;
    CString strLyricist;
    CString strComposer;
    CString strSoftware;
    struct _VORBISCOMMENT_{
        TCHAR *szField;
        CString *pstrContents;
    }VC[] = {
        {_T("TITLE"),       &strTrackName}, //�^�C�g��
        {_T("ARTIST"),      &strArtistName},//�A�[�e�B�X�g
        {_T("ALBUM"),       &strAlbumName}, //�A���o��
        {_T("ALBUMARTIST"), &strAlbumArtist}, //�A���o���A�[�e�B�X�g
        {_T("GENRE"),       &strGenre},     //�W������
        {_T("DATE"),        &strYear},      //�N
        {_T("COMMENT"),     &strComment},   //�R�����g
        {_T("TRACKNUMBER"), &strTrackNumber}, //�g���b�N�ԍ�
        {_T("TRACKTOTAL"),  &strTrackTotal},  //�g���b�N��
        {_T("DISCNUMBER"),  &strDiscNumber},  //�f�B�X�N�ԍ�
        {_T("DISCTOTAL"),   &strDiscTotal},   //�f�B�X�N��
        {_T("COPYRIGHT"),   &strCopyright},   //���쌠
        {_T("LYRICIST"),    &strLyricist},    //�쎌��
        {_T("COMPOSER"),    &strComposer},    //��Ȏ�
        {_T("ENCODED BY"),  &strSoftware},    //�\�t�g�E�F�A
        {NULL,          NULL}
    };
    int i = 0;
    while(VC[i].szField){
        fileOGG->GetComment(VC[i].szField, 0, *VC[i].pstrContents);
        i++;
    }
    SetTrackNameSI(pFileMP3, strTrackName);
    SetArtistNameSI(pFileMP3, strArtistName);
    SetAlbumNameSI(pFileMP3, strAlbumName);
    SetAlbumArtistSI(pFileMP3, strAlbumArtist);
    SetGenreSI(pFileMP3, strGenre);
    SetYearSI(pFileMP3, strYear);
    SetCommentSI(pFileMP3, strComment);
    SetTrackNumberSI(pFileMP3, strTrackNumber);
    SetTrackTotalSI(pFileMP3, strTrackTotal);
    SetDiscNumberSI(pFileMP3, strDiscNumber);
    SetDiscTotalSI(pFileMP3, strDiscTotal);
    SetCopyrightSI(pFileMP3, strCopyright);
    SetWriterSI(pFileMP3, strLyricist);
    SetComposerSI(pFileMP3, strComposer);
    SetSoftwareSI(pFileMP3, strSoftware);

#if 0
    { // ��W���^�O
        CString strDisp;
        CStringArray strArray;
        fileOGG->GetCommentNames(strArray);
        long items = 0;
        int i; for(i=0; i<strArray.GetSize(); i++)
        {
            CString strName = strArray.GetAt(i);
            CString strValue;
            CString _strValue;
            int j = 0;
            while(1)
            {
                if(!fileOGG->GetComment(strName,j++,strValue))
                {
                    break;
                }

                //�W�����͏��O
                if((j == 1) &&
                    (
                    !strName.Compare(_T("TITLE")) ||
                    !strName.Compare(_T("TRACKNUMBER")) ||
                    !strName.Compare(_T("ARTIST")) ||
                    !strName.Compare(_T("ALBUM")) ||
                    !strName.Compare(_T("DATE")) ||
                    !strName.Compare(_T("GENRE")) ||
                    !strName.Compare(_T("COMMENT")))
                    )
                {
                    continue;
                }
                if (strDisp.IsEmpty()) {
                } else {
                    strDisp += _T("\r\n");
                }
                strDisp += _T("[") + strName + _T("] ") + strValue;
                items++;
            }
        }
        SetOther(pFileMP3, strDisp);
    }
#endif
	SetAudioFormat(pFileMP3, fileOGG->GetAudioFormatString());
    CString strTimeString = fileOGG->GetTimeString();
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
    return true;
}

bool WriteFileOGG(FILE_INFO *pFileMP3)
{
    CTag_Ogg* fileOGG = (CTag_Ogg*)GetFileSpecificInfo(pFileMP3);

    CString strTrackName = GetTrackNameSI(pFileMP3);
    CString strArtistName = GetArtistNameSI(pFileMP3);
    CString strAlbumName = GetAlbumNameSI(pFileMP3);
    CString strAlbumArtist = GetAlbumArtistSI(pFileMP3);
    CString strGenre = GetGenreSI(pFileMP3);
    CString strYear = GetYearSI(pFileMP3);
    CString strComment = GetCommentSI(pFileMP3);
    CString strTrackNumber = GetTrackNumberSI(pFileMP3);
    CString strTrackTotal = GetTrackTotalSI(pFileMP3);
    CString strDiscNumber = GetDiscNumberSI(pFileMP3);
    CString strDiscTotal = GetDiscTotalSI(pFileMP3);
    CString strCopyright = GetCopyrightSI(pFileMP3);
    CString strLyricist = GetWriterSI(pFileMP3);
    CString strComposer = GetComposerSI(pFileMP3);
    CString strSoftware = GetSoftwareSI(pFileMP3);
    struct _VORBISCOMMENT_{
        TCHAR *szField;
        CString *pstrContents;
    }VC[] = {
        {_T("TITLE"),       &strTrackName}, //�^�C�g��
        {_T("ARTIST"),      &strArtistName},//�A�[�e�B�X�g
        {_T("ALBUM"),       &strAlbumName}, //�A���o��
        {_T("ALBUMARTIST"), &strAlbumArtist}, //�A���o���A�[�e�B�X�g
        {_T("GENRE"),       &strGenre},     //�W������
        {_T("DATE"),        &strYear},      //�N
        {_T("COMMENT"),     &strComment},   //�R�����g
        {_T("TRACKNUMBER"), &strTrackNumber}, //�g���b�N�ԍ�
        {_T("TRACKTOTAL"),  &strTrackTotal},  //�g���b�N��
        {_T("DISCNUMBER"),  &strDiscNumber},  //�f�B�X�N�ԍ�
        {_T("DISCTOTAL"),   &strDiscTotal},   //�f�B�X�N��
        {_T("COPYRIGHT"),   &strCopyright},   //���쌠
        {_T("LYRICIST"),    &strLyricist},    //�쎌��
        {_T("COMPOSER"),    &strComposer},    //��Ȏ�
        {_T("ENCODED BY"),  &strSoftware},    //�\�t�g�E�F�A
        {NULL,          NULL}
    };
    int i = 0;
    while(VC[i].szField){
        fileOGG->DelComment(VC[i].szField, 0);
        if (_tcslen(*VC[i].pstrContents) > 0) {
            fileOGG->AddComment(VC[i].szField, *VC[i].pstrContents);
        }
        //fileOGG->ReplaceComment(VC[i].szField, *VC[i].pstrContents, 0);
        i++;
    }
    if (fileOGG->Save(GetFullPath(pFileMP3)) != ERROR_SUCCESS) {
        return false;
    }
    return true;
}

bool LoadFileOPUS(FILE_INFO *pFileMP3)
{
//    return LoadFileOGG(pFileMP3);
    char *fname_utf8 = conv_tstr_to_utf8(GetFullPath(pFileMP3));
    int err = 0;
    OggOpusFile *pOpusFile = op_open_file(fname_utf8, &err);
    free(fname_utf8);
    if(!pOpusFile){
        return false;
    }
    const OpusTags *pTags = op_tags(pOpusFile, -1);
    if(!pTags){
        op_free(pOpusFile);
        return true;
    }
    CString strTrackName;
    CString strArtistName;
    CString strAlbumName;
    CString strAlbumArtist;
    CString strGenre;
    CString strYear;
    CString strComment;
    CString strTrackNumber;
    CString strTrackTotal;
    CString strDiscNumber;
    CString strDiscTotal;
    struct _OPUSCOMMENT_{
        const char *cszField;
        CString *pstrContents;
    }oc[] = {
        {"TITLE",       &strTrackName}, //�^�C�g��
        {"ARTIST",      &strArtistName},//�A�[�e�B�X�g
        {"ALBUM",       &strAlbumName}, //�A���o��
        {"ALBUMARTIST", &strAlbumArtist}, //�A���o���A�[�e�B�X�g
        {"GENRE",       &strGenre},     //�W������
        {"DATE",        &strYear},      //�N
        {"DESCRIPTION", &strComment},   //�R�����g(�{���������炪�������H)
        {"COMMENT",     &strComment},   //�R�����g(foobar2000 �ŏ������ނƂ�����ɂȂ�)
        {"TRACKNUMBER", &strTrackNumber}, //�g���b�N�ԍ�
        {"TRACKTOTAL",  &strTrackTotal},  //�g���b�N��
        {"DISCNUMBER",  &strDiscNumber},  //�f�B�X�N�ԍ�
        {"DISCTOTAL",   &strDiscTotal},   //�f�B�X�N��
        {NULL,          NULL}
    };
    int i = 0;
    while(oc[i].cszField){
        const char *value = opus_tags_query(pTags, oc[i].cszField, 0);
        if(value){
            TCHAR *tvalue = conv_utf8_to_tstr(value);
            *oc[i].pstrContents = tvalue;
            free(tvalue);
        }
        i++;
    }
    SetTrackNameSI(pFileMP3, strTrackName);
    SetArtistNameSI(pFileMP3, strArtistName);
    SetAlbumNameSI(pFileMP3, strAlbumName);
    SetAlbumArtistSI(pFileMP3, strAlbumArtist);
    SetGenreSI(pFileMP3, strGenre);
    SetYearSI(pFileMP3, strYear);
    SetCommentSI(pFileMP3, strComment);
    SetTrackNumberSI(pFileMP3, strTrackNumber);
    SetTrackTotalSI(pFileMP3, strTrackTotal);
    SetDiscNumberSI(pFileMP3, strDiscNumber);
    SetDiscTotalSI(pFileMP3, strDiscTotal);
    const OpusHead *opusHead = op_head(pOpusFile, -1);
    if(opusHead){
        const int samplerate = opusHead->input_sample_rate;
        const int bitrate = op_bitrate(pOpusFile, -1);
        const int ch = opusHead->channel_count;
        TCHAR format[256];
        _sntprintf_s(format, _TRUNCATE,
                     _T("OggOpus, %dkbps, %dHz(input), %dch"), 
                     bitrate/1000, samplerate, ch);
        SetAudioFormat(pFileMP3, format);
    }
    UINT64 qwTotalSample = op_pcm_total(pOpusFile, -1);
    int nTimeSec = qwTotalSample / 48000;//
    SetPlayTime(pFileMP3, nTimeSec);
    op_free(pOpusFile);
    
    return true;
}
bool WriteFileOPUS(FILE_INFO *pFileMP3)
{
    static DWORD s_dwTime = 0;
    if(GetTickCount() - s_dwTime > 30*1000){
        //�O��_�C�A���O�\������  30�b�ȏ�o��
        MessageBox(NULL, _T("OggOpus �̃^�O�X�V�͖��Ή��ł��B"), _T("STEP_ogg"), MB_OK);
        s_dwTime = GetTickCount();
    }
#if 0
    char *fname_utf8 = conv_tstr_to_utf8(GetFullPath(pFileMP3));
    TCHAR szFileName[MAX_PATH*2];
    int err = 0;
    OggOpusFile *pOpusFile = op_open_file(fname_utf8, &err);
    free(fname_utf8);
    if(!pOpusFile){
        return false;
    }
    const OpusTags *pTags = op_tags(pOpusFile, -1);
    OpusTags TagsWrite;
    opus_tags_init(&TagsWrite);
    if(!pTags){
        return true;
    }
    if(pTags){
        opus_tags_copy(&TagsWrite, pTags);
    }
    CString strTrackName = GetTrackNameSI(pFileMP3);
    CString strArtistName = GetArtistNameSI(pFileMP3);
    CString strAlbumName = GetAlbumNameSI(pFileMP3);
    CString strAlbumArtist = GetAlbumArtistSI(pFileMP3);
    CString strGenre = GetGenreSI(pFileMP3);
    CString strYear = GetYearSI(pFileMP3);
    CString strComment = GetCommentSI(pFileMP3);
    CString strTrackNumber = GetTrackNumberSI(pFileMP3);
    CString strTrackTotal = GetTrackTotalSI(pFileMP3);
    CString strDiscNumber = GetDiscNumberSI(pFileMP3);
    CString strDiscTotal = GetDiscTotalSI(pFileMP3);

    struct _OPUSCOMMENT_{
        const char *cszField;
        CString *pstrContents;
    }oc[] = {
        {"TITLE",       &strTrackName}, //�^�C�g��
        {"ARTIST",      &strArtistName},//�A�[�e�B�X�g
        {"ALBUM",       &strAlbumName}, //�A���o��
        {"ALBUMARTIST", &strAlbumArtist}, //�A���o���A�[�e�B�X�g
        {"GENRE",       &strGenre},     //�W������
        {"DATE",        &strYear},      //�N
        {"DESCRIPTION", &strComment},   //�R�����g
        {"TRACKNUMBER", &strTrackNumber}, //�g���b�N�ԍ�
        {"TRACKTOTAL",  &strTrackTotal},  //�g���b�N��
        {"DISCNUMBER",  &strDiscNumber},  //�f�B�X�N�ԍ�
        {"DISCTOTAL",   &strDiscTotal},   //�f�B�X�N��
        {NULL,          NULL}
    };
    int i = 0;
    while(oc[i].cszField){
        char *value_utf8 = conv_tstr_to_utf8(*oc[i].pstrContents);
        opus_tags_add(&TagsWrite, oc[i].cszField, value_utf8);
        i++;
    }
    opus_tags_clear(&TagsWrite);
    op_free(pOpusFile);
#endif
    return true;
}
