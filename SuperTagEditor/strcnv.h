#pragma once

enum {
    CONV_SUJI = 0x01, 
    CONV_ALPHA = 0x01 << 1, 
    CONV_KATA = 0x01 << 2, 
    CONV_KIGOU = 0x01 << 3, 
    //CONV_KIGOU_KATA = 0x01 << 4, //�A�B�u�v�E���J�^�J�i�Ƃ��Ĉ���
    CONV_ALL = 0xFFFFFFFF};

int conv_han2zens(TCHAR *zen, int zen_size, const TCHAR *han, DWORD flag, bool bZenHanKigouKana);//zen_size �͕������P��
int conv_zen2hans(TCHAR *han, int han_size, const TCHAR *zen, DWORD flag, bool bZenHanKigouKana);//han_size �͕������P��
void conv_kata2hira(TCHAR *str);
void conv_kata_erase_dakuon(TCHAR *str);
void conv_hira2kata(TCHAR *str);
void conv_upper(TCHAR *str);
void conv_lower(TCHAR *str);
void conv_first_upper(TCHAR *str, const TCHAR *suffixs, const TCHAR* separators , bool bUseSuffix);
void conv_fixed_upper_lower(TCHAR *_str, CStringArray& fixedWords); /* STEP 040 */

//�ȉ��̊֐��͖߂�l���Ăяo������ free ����
//�ϊ����� 0 �ŏI���Ȃ��\��������ꍇ�A�������P�ʂ̃T�C�Y�� size �ɓn��(0 �ŏI���ꍇ��-1)
WCHAR* conv_ansi_to_utf16(const char *str_ansi, int size = -1);  //ansi => utf16(le)
char*  conv_utf16_to_ansi(const WCHAR *str_utf16, int size = -1);//utf16(le) => ansi
WCHAR* conv_utf8_to_utf16(const char *str_utf8, int size = -1);  //utf8 => utf16
char*  conv_utf16_to_utf8(const WCHAR *str_utf16, int size = -1);//utf16 => utf8
char*  conv_ansi_to_utf8(const char *str_ansi, int size = -1);   //ansi => utf8
char*  conv_utf8_to_ansi(const char *str_utf8, int size = -1);   //utf8 => ansi

#ifdef _UNICODE
#define conv_tstr_to_utf8 conv_utf16_to_utf8
#define conv_utf8_to_tstr conv_utf8_to_utf16
#else
#define conv_tstr_to_utf8 conv_ansi_to_utf8
#define conv_utf8_to_tstr conv_utf8_to_ansi
#endif

//BOM ���������ʂ��� TCHAR* �ϊ�
//BOM �Ȃ� UTF-8 �̎������ʂɂ͖��Ή�(BOM �Ȃ��� ANSI �ƌ��Ȃ�)
//���� conv_xxx_to_xxx �Ƃ͈قȂ�A�߂�l�� free ���Ă͂����Ȃ�
//*pFree != NULL �̂Ƃ��� *pFree ���Ăяo������ free ����
//*pFree == NULL �̂Ƃ��� �߂�l�� data �̈ꕔ�ł��邽�߁Afree ���Ȃ�
//dwBufferSize �̓o�b�t�@�̃T�C�Y(�������ł͂Ȃ�)
//�I�[�� 0 �ŏI��邱�Ƃ��������Ă��Ă� dwBufferSize �� -1 ��n���Ă͂����Ȃ�
TCHAR* conv_data_to_tstr(const BYTE *data, DWORD dwBufferSize, void **ppFree);
