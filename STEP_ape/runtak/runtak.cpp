#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#include "tak.h"

//tak_deco_lib.dll ���g�p���ĉ��t���Ԃƃt�H�[�}�b�g�����擾
//64bit�� STEP_ape.ste ����Ăяo�����
//tak_deco_lib.dll �� 64bit �ł����J���ꂽ��s�v�ɂȂ�
//
//�\�����[�V�����v���b�g�t�H�[���� x86 �̏ꍇ�̓r���h����Ȃ�
//(STEP_ape.ste ���g�Ŏ擾����̂�)
//�\�����[�V�����v���b�g�t�H�[���� x64 �̏ꍇ��
//���̃v���O�������̂� 32bit �Ńr���h�����悤�ɐݒ肵�Ă���
//
//
int WINAPI _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{//�R�}���h���C���̑�1������TAK�t�@�C����
 //�R�}���h���C���̑�2�����͏����i�[����t�@�C���}�b�v�I�u�W�F�N�g��
    if(__argc < 3){
        return 0;
    }
    LPCTSTR cszFileName = __targv[1];
    LPCTSTR cszFileMapName = __targv[2];
    GetAudioFormatTakToFileMap(cszFileName, cszFileMapName);
    return 0;
}
