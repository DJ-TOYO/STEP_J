#include "stdafx.h"
#include "RiffSif.h"
#include "STEPlugin.h"
#include "MMCommand.h"

//WAV �� AVI �̃^�O����ǂݏ�������
//��{�I�� WAV �� AVI ����������
bool LoadAttributeFileWAV(FILE_INFO *pFileMP3);
bool WriteAttributeFileWAV(FILE_INFO *pFileMP3);

bool LoadAttributeFileWAV(FILE_INFO *pFileMP3)
{
	CRiffSIF riff;
	TCHAR ext[_MAX_EXT];
	_tsplitpath(GetFullPath(pFileMP3), NULL, NULL, NULL, ext);
    if(_tcsicmp(ext, _T(".wav")) == 0){
        if(riff.Load(GetFullPath(pFileMP3),'W','A','V','E') != ERROR_SUCCESS){
            return false;
        }
    }
    else if(_tcsicmp(ext, _T(".avi")) == 0){
        if(riff.Load(GetFullPath(pFileMP3),'A','V','I',' ') != ERROR_SUCCESS){
            return false;
        }
	    //ISBJ songname
	    SetTrackNameSI(pFileMP3, riff.GetField('I','S','B','J'));
    }
    else{
        return false;
    }
    //INAM/ISBJ �^�C�g��
    //ISBJ ���� INAM ��D��
    SetTrackNameSI(pFileMP3, riff.GetField('I','N','A','M'));
    if(_tcslen(GetTrackNameSI(pFileMP3)) == 0){
        SetTrackNameSI(pFileMP3, riff.GetField('I','S','B','J'));
    }
	//IART �A�[�e�B�X�g��
	SetArtistNameSI(pFileMP3, riff.GetField('I','A','R','T'));
	//IPRD �A���o����
	SetAlbumNameSI(pFileMP3, riff.GetField('I','P','R','D'));
	//ICMT �R�����g
	SetCommentSI(pFileMP3, riff.GetField('I','C','M','T'));
	//ICRD ���t
	SetYearSI(pFileMP3, riff.GetField('I','C','R','D'));
	//IGNR �W������
	SetGenreSI(pFileMP3, riff.GetField('I','G','N','R'));
	//ICOP ���쌠
	SetCopyrightSI(pFileMP3, riff.GetField('I','C','O','P'));
	//IENG �G���W�j�A	
    SetEngineerSI(pFileMP3, riff.GetField('I','E','N','G'));
	//ISRC �\�[�X	
	SetSourceSI(pFileMP3, riff.GetField('I','S','R','C'));
	//ISFT �\�t�g�E�F�A
	SetSoftwareSI(pFileMP3, riff.GetField('I','S','F','T'));
	//ITRK �g���b�N�ԍ�
	SetTrackNumberSI(pFileMP3, riff.GetField('I','T','R','K'));
    //�t�H�[�}�b�g
    CString strFormat;
    CString strTime;
    if(_tcsicmp(ext, _T(".wav")) == 0){
        GetWaveAudioFormat(GetFullPath(pFileMP3), riff.GetStreamSize(),
                            strFormat, strTime, 0);
    }
    else{
        CString strAudioFormat;
        CString strVideoFormat;
        CString strStreamFormat;
        BOOL bAvi = false;
        GetAviFormat(GetFullPath(pFileMP3), 
                     strAudioFormat, strVideoFormat, strStreamFormat,
                     strTime,  bAvi, 0);
        strFormat = strVideoFormat;
        if(!strFormat.IsEmpty() && !strStreamFormat.IsEmpty()){
            strFormat += _T(", ");
        }
        strFormat += strStreamFormat;
        if(!strFormat.IsEmpty() && !strAudioFormat.IsEmpty()){
            strFormat += _T(", ");
        }
        strFormat += strAudioFormat;
    }
    SetAudioFormat(pFileMP3, strFormat);
    TCHAR *time = strTime.GetBuffer();
    //szTime = "xx:xx (xxxsec) �̂悤�ɂȂ��Ă���
    TCHAR *pszSec = _tcschr(time, _T('('));
    if(pszSec){
        pszSec++;
        TCHAR *end;
        int sec = _tcstol(pszSec, &end, 10);
        SetPlayTime(pFileMP3, sec);
    }
    strTime.ReleaseBuffer();
    return true;
}

bool WriteAttributeFileWAV(FILE_INFO *pFileMP3)
{
	CRiffSIF riff;
	TCHAR ext[_MAX_EXT];
	_tsplitpath(GetFullPath(pFileMP3), NULL, NULL, NULL, ext);
    if(_tcsicmp(ext, _T(".wav")) == 0){
        if(riff.Load(GetFullPath(pFileMP3),'W','A','V','E') != ERROR_SUCCESS){
            return false;
        }
    }
    else if(_tcsicmp(ext, _T(".avi")) == 0){
        if(riff.Load(GetFullPath(pFileMP3),'A','V','I',' ') != ERROR_SUCCESS){
            return false;
        }
    }
    else{
        return false;
    }
    //INAM �^�C�g��
	riff.SetField('I','N','A','M', GetTrackNameSI(pFileMP3));
	riff.SetField('I','S','B','J', _T("")); /* 2005.12.26 mp3infp�Ɠ��l�� ISBJ���폜 */
	//IART �A�[�e�B�X�g��
	riff.SetField('I','A','R','T', GetArtistNameSI(pFileMP3));
	//IPRD �A���o����
	riff.SetField('I','P','R','D', GetAlbumNameSI(pFileMP3));
	//ICMT �R�����g
	riff.SetField('I','C','M','T', GetCommentSI(pFileMP3));
	//ICRD ���t
	riff.SetField('I','C','R','D', GetYearSI(pFileMP3));
	//IGNR �W������
	riff.SetField('I','G','N','R', GetGenreSI(pFileMP3));
	//ICOP ���쌠
	riff.SetField('I','C','O','P', GetCopyrightSI(pFileMP3));
	//IENG �G���W�j�A	
	riff.SetField('I','E','N','G', GetEngineerSI(pFileMP3));
	//ISRC �\�[�X	
	riff.SetField('I','S','R','C', GetSourceSI(pFileMP3));
	//ISFT �\�t�g�E�F�A
	riff.SetField('I','S','F','T', GetSoftwareSI(pFileMP3));
	//ITRK �g���b�N�ԍ�
	riff.SetField('I','T','R','K', GetTrackNumberSI(pFileMP3));
    return riff.Save(GetFullPath(pFileMP3)) == ERROR_SUCCESS;
}
