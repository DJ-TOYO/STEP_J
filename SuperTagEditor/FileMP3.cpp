// ======================================================
// �T  �v    �F �l�o�R�t�@�C���Ǘ��N���X
// �^�[�Q�b�g�F Windows95/98/NT
// �����n    �F Visual C++ Ver 6.0
// �쐬��    �F MERCURY
// �쐬��    �F 00/01/18(��)
// ���쌠�\�L�F Copyright(C) 2000 MERCURY.
// ======================================================



// ======================================
// =====   �����R���p�C���t���O     =====
// ======================================



// ======================================
// =====   �C���N���[�h�t�@�C��     =====
// ======================================
#include "stdafx.h"
#include "FileMP3.h"
#include "supertageditor.h"
#include "DlgFileOverWrite.h"
#include "DlgFileNameChange.h"


#define USE_SCMPX_GENRE_ANIMEJ g_genreListSCMPX[255].bAddList /* BeachMonster 099 */

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
int  g_nWriteTagProcFlag = 0;

// ======================================
// =====        static �ϐ�         =====
// ======================================
// �W���������̃e�[�u��
GENRE_LIST  g_genreListSCMPX[] = {
    {true,   0, _T("Blues")},                   // 0(ID3 Tag�݊�)
    {true,   1, _T("Classic Rock")},            // 1(ID3 Tag�݊�)
    {true,   2, _T("Country")},                 // 2(ID3 Tag�݊�)
    {true,   3, _T("Dance")},                   // 3(ID3 Tag�݊�)
    {true,   4, _T("Disco")},                   // 4(ID3 Tag�݊�)
    {true,   5, _T("Funk")},                    // 5(ID3 Tag�݊�)
    {true,   6, _T("Grunge")},                  // 6(ID3 Tag�݊�)
    {true,   7, _T("Hip-Hop")},                 // 7(ID3 Tag�݊�)
    {true,   8, _T("Jazz")},                    // 8(ID3 Tag�݊�)
    {true,   9, _T("Metal")},                   // 9(ID3 Tag�݊�)
    {true,  10, _T("New Age")},                 // 10(ID3 Tag�݊�)
    {true,  11, _T("Oldies")},                  // 11(ID3 Tag�݊�)
    {true,  12, _T("Other")},                   // 12(ID3 Tag�݊�)
    {true,  13, _T("Pop")},                     // 13(ID3 Tag�݊�)
    {true,  14, _T("R&B")},                     // 14(ID3 Tag�݊�)
    {true,  15, _T("Rap")},                     // 15(ID3 Tag�݊�)
    {true,  16, _T("Reggae")},                  // 16(ID3 Tag�݊�)
    {true,  17, _T("Rock")},                    // 17(ID3 Tag�݊�)
    {true,  18, _T("Techno")},                  // 18(ID3 Tag�݊�)
    {true,  19, _T("Industrial")},              // 19(ID3 Tag�݊�)
    {true,  20, _T("Alternative")},             // 20(ID3 Tag�݊�)
    {true,  21, _T("Ska")},                     // 21(ID3 Tag�݊�)
    {true,  22, _T("Death Metal")},             // 22(ID3 Tag�݊�)
    {true,  23, _T("Pranks")},                  // 23(ID3 Tag�݊�)
    {true,  24, _T("Soundtrack")},              // 24(ID3 Tag�݊�)
    {true,  25, _T("Euro-Techno")},             // 25(ID3 Tag�݊�)
    {true,  26, _T("Ambient")},                 // 26(ID3 Tag�݊�)
    {true,  27, _T("Trip-Hop")},                // 27(ID3 Tag�݊�)
    {true,  28, _T("Vocal")},                   // 28(ID3 Tag�݊�)
    {true,  29, _T("Jazz+Funk")},               // 29(ID3 Tag�݊�)
    {true,  30, _T("Fusion")},                  // 30(ID3 Tag�݊�)
    {true,  31, _T("Trance")},                  // 31(ID3 Tag�݊�)
    {true,  32, _T("Classical")},               // 32(ID3 Tag�݊�)
    {true,  33, _T("Instrumental")},            // 33(ID3 Tag�݊�)
    {true,  34, _T("Acid")},                    // 34(ID3 Tag�݊�)
    {true,  35, _T("House")},                   // 35(ID3 Tag�݊�)
    {true,  36, _T("Game")},                    // 36(ID3 Tag�݊�)
    {true,  37, _T("Sound Clip")},              // 37(ID3 Tag�݊�)
    {true,  38, _T("Gospel")},                  // 38(ID3 Tag�݊�)
    {true,  39, _T("Noise")},                   // 39(ID3 Tag�݊�)
    {true,  40, _T("AlternRock")},              // 40(ID3 Tag�݊�)
    {true,  41, _T("Bass")},                    // 41(ID3 Tag�݊�)
    {true,  42, _T("Soul")},                    // 42(ID3 Tag�݊�)
    {true,  43, _T("Punk")},                    // 43(ID3 Tag�݊�)
    {true,  44, _T("Space")},                   // 44(ID3 Tag�݊�)
    {true,  45, _T("Meditative")},              // 45(ID3 Tag�݊�)
    {true,  46, _T("Instrumental Pop")},        // 46(ID3 Tag�݊�)
    {true,  47, _T("Instrumental Rock")},       // 47(ID3 Tag�݊�)
    {true,  48, _T("Ethnic")},                  // 48(ID3 Tag�݊�)
    {true,  49, _T("Gothic")},                  // 49(ID3 Tag�݊�)
    {true,  50, _T("Darkwave")},                // 50(ID3 Tag�݊�)
    {true,  51, _T("Techno-Industrial")},       // 51(ID3 Tag�݊�)
    {true,  52, _T("Electronic")},              // 52(ID3 Tag�݊�)
    {true,  53, _T("Pop-Folk")},                // 53(ID3 Tag�݊�)
    {true,  54, _T("Eurodance")},               // 54(ID3 Tag�݊�)
    {true,  55, _T("Dream")},                   // 55(ID3 Tag�݊�)
    {true,  56, _T("Southern Rock")},           // 56(ID3 Tag�݊�)
    {true,  57, _T("Comedy")},                  // 57(ID3 Tag�݊�)
    {true,  58, _T("Cult")},                    // 58(ID3 Tag�݊�)
    {true,  59, _T("Gangsta")},                 // 59(ID3 Tag�݊�)
    {true,  60, _T("Top 40")},                  // 60(ID3 Tag�݊�)
    {true,  61, _T("Christian Rap")},           // 61(ID3 Tag�݊�)
    {true,  62, _T("Pop/Funk")},                // 62(ID3 Tag�݊�)
    {true,  63, _T("Jungle")},                  // 63(ID3 Tag�݊�)
    {true,  64, _T("Native American")},         // 64(ID3 Tag�݊�)
    {true,  65, _T("Cabaret")},                 // 65(ID3 Tag�݊�)
    {true,  66, _T("New Wave")},                // 66(ID3 Tag�݊�)
    {true,  67, _T("Psychedelic")},             // 67(ID3 Tag�݊�)
    {true,  68, _T("Rave")},                    // 68(ID3 Tag�݊�)
    {true,  69, _T("Showtunes")},               // 69(ID3 Tag�݊�)
    {true,  70, _T("Trailer")},                 // 70(ID3 Tag�݊�)
    {true,  71, _T("Lo-Fi")},                   // 71(ID3 Tag�݊�)
    {true,  72, _T("Tribal")},                  // 72(ID3 Tag�݊�)
    {true,  73, _T("Acid Punk")},               // 73(ID3 Tag�݊�)
    {true,  74, _T("Acid Jazz")},               // 74(ID3 Tag�݊�)
    {true,  75, _T("Polka")},                   // 75(ID3 Tag�݊�)
    {true,  76, _T("Retro")},                   // 76(ID3 Tag�݊�)
    {true,  77, _T("Musical")},                 // 77(ID3 Tag�݊�)
    {true,  78, _T("Rock & Roll")},             // 78(ID3 Tag�݊�)
    {true,  79, _T("Hard Rock")},               // 79(ID3 Tag�݊�)
    {true,  80, _T("Folk")},                    // 80(ID3 Tag�݊�)
    {true,  81, _T("Folk/Rock")},               // 81(ID3 Tag�݊�)
    {true,  82, _T("National Folk")},           // 82(ID3 Tag�݊�)
    {true,  83, _T("Swing")},                   // 83(ID3 Tag�݊�)
    {true,  84, _T("Fast-Fusion")},             // 84(Winamp�݊�)
    {true,  85, _T("Bebob")},                   // 85(Winamp�݊�)
    {true,  86, _T("Latin")},                   // 86(Winamp�݊�)
    {true,  87, _T("Revival")},                 // 87(Winamp�݊�)
    {true,  88, _T("Celtic")},                  // 88(Winamp�݊�)
    {true,  89, _T("Bluegrass")},               // 89(Winamp�݊�)
    {true,  90, _T("Avantgarde")},              // 90(Winamp�݊�)
    {true,  91, _T("Gothic Rock")},             // 91(Winamp�݊�)
    {true,  92, _T("Progressive Rock")},        // 92(Winamp�݊�)
    {true,  93, _T("Psychedelic Rock")},        // 93(Winamp�݊�)
    {true,  94, _T("Symphonic Rock")},          // 94(Winamp�݊�)
    {true,  95, _T("Slow Rock")},               // 95(Winamp�݊�)
    {true,  96, _T("Big Band")},                // 96(Winamp�݊�)
    {true,  97, _T("Chorus")},                  // 97(Winamp�݊�)
    {true,  98, _T("Easy Listening")},          // 98(Winamp�݊�)
    {true,  99, _T("Acoustic")},                // 99(Winamp�݊�)
    {true, 100, _T("Humour")},                  // 100(Winamp�݊�)
    {true, 101, _T("Speech")},                  // 101(Winamp�݊�)
    {true, 102, _T("Chanson")},                 // 102(Winamp�݊�)
    {true, 103, _T("Opera")},                   // 103(Winamp�݊�)
    {true, 104, _T("Chamber Music")},           // 104(Winamp�݊�)
    {true, 105, _T("Sonata")},                  // 105(Winamp�݊�)
    {true, 106, _T("Symphony")},                // 106(Winamp�݊�)
    {true, 107, _T("Booty Bass")},              // 107(Winamp�݊�)
    {true, 108, _T("Primus")},                  // 108(Winamp�݊�)
    {true, 109, _T("Porn Groove")},             // 109(Winamp�݊�)
    {true, 110, _T("Satire")},                  // 110(Winamp�݊�)
    {true, 111, _T("Slow Jam")},                // 111(Winamp�݊�)
    {true, 112, _T("Club")},                    // 112(Winamp�݊�)
    {true, 113, _T("Tango")},                   // 113(Winamp�݊�)
    {true, 114, _T("Samba")},                   // 114(Winamp�݊�)
    {true, 115, _T("Folklore")},                // 115(Winamp�݊�)
    {true, 116, _T("Ballad")},                  // 116(Winamp�݊�)
    {true, 117, _T("Power Ballad")},            // 117(Winamp�݊�)
    {true, 118, _T("Phythmic Soul")},           // 118(Winamp�݊�)
    {true, 119, _T("Freestyle")},               // 119(Winamp�݊�)
    {true, 120, _T("Duet")},                    // 120(Winamp�݊�)
    {true, 121, _T("Punk Rock")},               // 121(Winamp�݊�)
    {true, 122, _T("Drum Solo")},               // 122(Winamp�݊�)
    {true, 123, _T("A Cappella")},              // 123(Winamp�݊�)
    {true, 124, _T("Euro-House")},              // 124(Winamp�݊�)
    {true, 125, _T("Dance Hall")},              // 125(Winamp�݊�)
    {true, 126, _T("Goa")},                     // 126(Winamp�݊�)
    {true, 127, _T("Drum & Bass")},             // 127(Winamp�݊�)
    {true, 128, _T("Club-House")},              // 128(Winamp�݊�)
    {true, 129, _T("Hardcore")},                // 129(Winamp�݊�)
    {true, 130, _T("Terror")},                  // 130(Winamp�݊�)
    {true, 131, _T("Indie")},                   // 131(Winamp�݊�)
    {true, 132, _T("BritPop")},                 // 132(Winamp�݊�)
    {true, 133, _T("Negerpunk")},               // 133(Winamp�݊�)
    {true, 134, _T("Polsk Punk")},              // 134(Winamp�݊�)
    {true, 135, _T("Beat")},                    // 135(Winamp�݊�)
    {true, 136, _T("Christian Gangsta Rap")},   // 136(Winamp�݊�)
    {true, 137, _T("Heavy Metal")},             // 137(Winamp�݊�)
    {true, 138, _T("Black Metal")},             // 138(Winamp�݊�)
    {true, 139, _T("Crossover")},               // 139(Winamp�݊�)
    {true, 140, _T("Contemporary Christian")},  // 140(Winamp�݊�)
    {true, 141, _T("Christian Rock")},          // 141(Winamp�݊�)
    {true, 142, _T("Merengue")},                // 142(Winamp�݊�)
    {true, 143, _T("Salsa")},                   // 143(Winamp�݊�)
    {true, 144, _T("Thrash Metal")},            // 144(Winamp�݊�)
    {true, 145, _T("Anime")},                   // 145(Winamp�݊�)
    {true, 146, _T("JPop")},                    // 146(Winamp�݊�)
    {true, 147, _T("Synthpop")},                // 147(Winamp�݊�)
    {false, 148, _T("����`(148)")},            // 148
    {false, 148, _T("����`(148)")},            // 148
    {false, 149, _T("����`(149)")},            // 149
    {false, 150, _T("����`(150)")},            // 150
    {false, 151, _T("����`(151)")},            // 151
    {false, 152, _T("����`(152)")},            // 152
    {false, 153, _T("����`(153)")},            // 153
    {false, 154, _T("����`(154)")},            // 154
    {false, 155, _T("����`(155)")},            // 155
    {false, 156, _T("����`(156)")},            // 156
    {false, 157, _T("����`(157)")},            // 157
    {false, 158, _T("����`(158)")},            // 158
    {false, 159, _T("����`(159)")},            // 159
    {false, 160, _T("����`(160)")},            // 160
    {false, 161, _T("����`(161)")},            // 161
    {false, 162, _T("����`(162)")},            // 162
    {false, 163, _T("����`(163)")},            // 163
    {false, 164, _T("����`(164)")},            // 164
    {false, 165, _T("����`(165)")},            // 165
    {false, 166, _T("����`(166)")},            // 166
    {false, 167, _T("����`(167)")},            // 167
    {false, 168, _T("����`(168)")},            // 168
    {false, 169, _T("����`(169)")},            // 169
    {false, 170, _T("����`(170)")},            // 170
    {false, 171, _T("����`(171)")},            // 171
    {false, 172, _T("����`(172)")},            // 172
    {false, 173, _T("����`(173)")},            // 173
    {false, 174, _T("����`(174)")},            // 174
    {false, 175, _T("����`(175)")},            // 175
    {false, 176, _T("����`(176)")},            // 176
    {false, 177, _T("����`(177)")},            // 177
    {false, 178, _T("����`(178)")},            // 178
    {false, 179, _T("����`(179)")},            // 179
    {false, 180, _T("����`(180)")},            // 180
    {false, 181, _T("����`(181)")},            // 181
    {false, 182, _T("����`(182)")},            // 182
    {false, 183, _T("����`(183)")},            // 183
    {false, 184, _T("����`(184)")},            // 184
    {false, 185, _T("����`(185)")},            // 185
    {false, 186, _T("����`(186)")},            // 186
    {false, 187, _T("����`(187)")},            // 187
    {false, 188, _T("����`(188)")},            // 188
    {false, 189, _T("����`(189)")},            // 189
    {false, 190, _T("����`(190)")},            // 190
    {false, 191, _T("����`(191)")},            // 191
    {false, 192, _T("����`(192)")},            // 192
    {false, 193, _T("����`(193)")},            // 193
    {false, 194, _T("����`(194)")},            // 194
    {false, 195, _T("����`(195)")},            // 195
    {false, 196, _T("����`(196)")},            // 196
    {false, 197, _T("����`(197)")},            // 197
    {false, 198, _T("����`(198)")},            // 198
    {false, 199, _T("����`(199)")},            // 199
    {false, 200, _T("����`(200)")},            // 200
    {false, 201, _T("����`(201)")},            // 201
    {false, 202, _T("����`(202)")},            // 202
    {false, 203, _T("����`(203)")},            // 203
    {false, 204, _T("����`(204)")},            // 204
    {false, 205, _T("����`(205)")},            // 205
    {false, 206, _T("����`(206)")},            // 206
    {false, 207, _T("����`(207)")},            // 207
    {false, 208, _T("����`(208)")},            // 208
    {false, 209, _T("����`(209)")},            // 209
    {false, 210, _T("����`(210)")},            // 210
    {false, 211, _T("����`(211)")},            // 211
    {false, 212, _T("����`(212)")},            // 212
    {false, 213, _T("����`(213)")},            // 213
    {false, 214, _T("����`(214)")},            // 214
    {false, 215, _T("����`(215)")},            // 215
    {false, 216, _T("����`(216)")},            // 216
    {false, 217, _T("����`(217)")},            // 217
    {false, 218, _T("����`(218)")},            // 218
    {false, 219, _T("����`(219)")},            // 219
    {false, 220, _T("����`(220)")},            // 220
    {false, 221, _T("����`(221)")},            // 221
    {false, 222, _T("����`(222)")},            // 222
    {false, 223, _T("����`(223)")},            // 223
    {false, 224, _T("����`(224)")},            // 224
    {false, 225, _T("����`(225)")},            // 225
    {false, 226, _T("����`(226)")},            // 226
    {false, 227, _T("����`(227)")},            // 227
    {false, 228, _T("����`(228)")},            // 228
    {false, 229, _T("����`(229)")},            // 229
    {false, 230, _T("����`(230)")},            // 230
    {false, 231, _T("����`(231)")},            // 231
    {false, 232, _T("����`(232)")},            // 232
    {false, 233, _T("����`(233)")},            // 233
    {false, 234, _T("����`(234)")},            // 234
    {false, 235, _T("����`(235)")},            // 235
    {false, 236, _T("����`(236)")},            // 236
    {false, 237, _T("����`(237)")},            // 237
    {false, 238, _T("����`(238)")},            // 238
    {false, 239, _T("����`(239)")},            // 239
    {false, 240, _T("����`(240)")},            // 240
    {false, 241, _T("����`(241)")},            // 241
    {false, 242, _T("����`(242)")},            // 242
    {false, 243, _T("����`(243)")},            // 243
    {false, 244, _T("����`(244)")},            // 244
    {false, 245, _T("����`(245)")},            // 245
    {false, 246, _T("����`(246)")},            // 246
    {false, 247, _T("����`(247)")},            // 247
    {false, 248, _T("Heavy Rock(J)")},          // 248(SCMPX�݊�)
    {false, 249, _T("Doom Rock(J)")},           // 249(SCMPX�݊�)
    {false, 250, _T("J-POP(J)")},               // 250(SCMPX�݊�)
    {false, 251, _T("Seiyu(J)")},               // 251(SCMPX�݊�)
    {false, 252, _T("Tecno Ambient(J)")},       // 252(SCMPX�݊�)
    {false, 253, _T("Moemoe(J)")},              // 253(SCMPX�݊�)
    {false, 254, _T("Tokusatsu(J)")},           // 254(SCMPX�݊�)
    {false, 255, _T("Anime(J)")},               // 255(SCMPX�݊�)
    {false, 255, _T("���w��")},                 // 255
    {false, 255, NULL},                     // �I�[�R�[�h
};
USER_GENRE_LIST*    g_genreListUSER = NULL;



// ======================================
// =====     �֐��v���g�^�C�v       =====
// ======================================


// =============================================
// GetGenreNameSIF
// �T�v  : �W�������ԍ�����W�����������擾(SI�t�B�[���h�p)
// ����  : byGenre          = �W�������ԍ�
// �߂�l: const char *     = �W��������
// =============================================
const TCHAR *GetGenreNameSIF(BYTE byGenre)
{
    int     i;
    for (i = 0; g_genreListSCMPX[i].sName != NULL; i++) {
        if (g_genreListSCMPX[i].byGenre == byGenre) {
            if (byGenre == 0xff) {
                // SCMPX �݊����ǂ����ŕ������ς���
                return(g_genreListSCMPX[i].bAddList ? _T("Anime(J)") : _T(""));
            }
            return(g_genreListSCMPX[i].sName);
        }
    }
    return(_T(""));
}

// =============================================
// GetGenreCode
// �T�v  : �W������������W�������ԍ����擾
// ����  : sGenre       = �W��������
// �߂�l: int          = �W�������ԍ�(-1:���Ή�)
// =============================================
int GetGenreCode(const TCHAR *sGenre)
{
    int     i;
    for (i = 0; g_genreListSCMPX[i].sName != NULL; i++) {
        if (_tcsicmp(g_genreListSCMPX[i].sName, sGenre) == 0) {
            return(g_genreListSCMPX[i].byGenre);
        }
    }
    for (i = 0; i < USER_GENRE_LIST_MAX; i++) {
        if (_tcsicmp(g_genreListUSER[i].sName, sGenre) == 0 && g_genreListUSER[i].bUse) {
            return(g_genreListUSER[i].byGenre);
        }
    }
    return(-1);
}

bool IsUserGenre(const TCHAR *sGenre)
{
    if (_tcslen(sGenre) == 0) return false; /* WildCherry2 075 */
    int     i;
    for (i = 0; g_genreListSCMPX[i].sName != NULL; i++) {
        if (_tcsicmp(g_genreListSCMPX[i].sName, sGenre) == 0) {
            return false;
        }
    }
    for (i = 0; i < USER_GENRE_LIST_MAX; i++) {

        if (_tcsicmp(g_genreListUSER[i].sName, sGenre) == 0 && g_genreListUSER[i].bUse) {
            return true;
        }
    }
    return true;
}

static  bool MyChangeFileName(const TCHAR *sFileName, const TCHAR *sNewName)
{
    if (MoveFile(sFileName, sNewName) == FALSE) {
        CString strBuffer;
        LPVOID lpMsgBuf;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                      FORMAT_MESSAGE_FROM_SYSTEM |
                      FORMAT_MESSAGE_IGNORE_INSERTS,
                      NULL, GetLastError(),
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // �f�t�H���g����
                      (LPTSTR)&lpMsgBuf, 0, NULL);
        strBuffer.Format(_T("�ύX�O�F%s\n�ύX��F%s\n\n%s"), sFileName, sNewName, (const TCHAR *)lpMsgBuf);
        LocalFree(lpMsgBuf);
        MessageBox(NULL, strBuffer, _T("�t�@�C������ύX�ł��܂���ł���"), MB_ICONSTOP|MB_OK|MB_TOPMOST);
        return(false);
    }
    return(true);
}

static  bool MyCopyFile(const TCHAR *sFileName, const TCHAR *sNewName, bool bMoveFlag)
{
    BOOL    bResult;

    // �R�s�[�^�ړ������s
    if (bMoveFlag) {
        // �ړ���ɓ����̃t�@�C��������ꍇ�́A�\�ߍ폜����
        if (SetFileAttributes(sNewName, CFile::normal) == TRUE) {
            if (DeleteFile(sNewName) == FALSE) {
//              CString strBuffer;
//              LPVOID lpMsgBuf;
//              FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
//                            FORMAT_MESSAGE_FROM_SYSTEM |
//                            FORMAT_MESSAGE_IGNORE_INSERTS,
//                            NULL, GetLastError(),
//                            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // �f�t�H���g����
//                            (LPTSTR)&lpMsgBuf, 0, NULL);
//              strBuffer.Format("�t�@�C�����F%s\n\n%s", sNewName, (const char *)lpMsgBuf);
//              LocalFree(lpMsgBuf);
//              MessageBox(NULL, strBuffer, "�t�@�C���̍폜�Ɏ��s���܂���", MB_OK/*MB_ABORTRETRYIGNORE*/|MB_TOPMOST);
//              return(false);
            }
        }
        bResult = MoveFile(sFileName, sNewName);
    } else {
        bResult = CopyFile(sFileName, sNewName, FALSE);
    }

    // �G���[����
    if (bResult == FALSE) {
        static  const TCHAR *sMessage[][2] = {
            {_T("�R�s�[���F%s\n�R�s�[��F%s\n\n%s"), _T("�t�@�C���̃R�s�[�Ɏ��s���܂���")},
            {_T("�ړ����F%s\n�ړ���F%s\n\n%s"), _T("�t�@�C���̈ړ��Ɏ��s���܂���")},
        };
        CString strBuffer;
        LPVOID lpMsgBuf;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
                      FORMAT_MESSAGE_FROM_SYSTEM |
                      FORMAT_MESSAGE_IGNORE_INSERTS,
                      NULL, GetLastError(),
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // �f�t�H���g����
                      (LPTSTR)&lpMsgBuf, 0, NULL);
        strBuffer.Format(sMessage[bMoveFlag?1:0][0], sFileName, sNewName, (const TCHAR *)lpMsgBuf);
        LocalFree(lpMsgBuf);
        MessageBox(NULL, strBuffer, sMessage[bMoveFlag?1:0][1], MB_ICONSTOP|MB_OK|MB_TOPMOST);
        return(false);
    }
    return(true);
}

static  void ChangeFileAttr(const TCHAR *sFileName, DWORD dwAttr)
{
    DWORD   dwAttrNow;
    if ((dwAttrNow = GetFileAttributes(sFileName)) != (DWORD)-1L) {
        if (dwAttrNow != dwAttr) {
            SetFileAttributes(sFileName, dwAttr);
        }
    }
}

// �Ōオ '\\' �ɂȂ��Ă��邩�ǂ����̃`�F�b�N�֐�
bool IsFolderName(const TCHAR *sName)
{
    const TCHAR *path_delimiter = _tcsrchr(sName, _T('\\'));
    return path_delimiter && !path_delimiter[1];
}


// =============================================
// CFileMP3::CFileMP3
// Func  : �R���X�g���N�^
// Input : none
// Output: none
// =============================================
CFileMP3::CFileMP3()
{
    m_fileMP3.fileSpecificInfo = NULL;
    Initialize();
}

// =============================================
// CFileMP3::~CFileMP3
// Func  : �f�X�g���N�^
// Input : none
// Output: none
// =============================================
CFileMP3::~CFileMP3()
{
    //if (m_fileMP3.fileOGG != NULL)    delete m_fileMP3.fileOGG; /* Conspiracy 196 */
}

// =============================================
// CFileMP3::InitData
// �T�v  : MP3 �t�@�C�����̍\���̂����������܂�
// ����  : fileMP3      = ����������\����
// �߂�l: none
// =============================================
void CFileMP3::InitData(FILE_MP3 *fileMP3)
{
    extern void InitFileSpecificInfo(FILE_MP3* pFileMP3);
    if (fileMP3->fileSpecificInfo != NULL) {
        InitFileSpecificInfo(fileMP3);
        fileMP3->fileSpecificInfo = NULL;
    }

    fileMP3->bModifyFlag    = false;    // �O������ҏW���ꂽ���ǂ����̃t���O
    fileMP3->bCheckFlag     = false;    // �`�F�b�N���
    fileMP3->lFileSize      = 0L;       // �t�@�C���T�C�Y
    fileMP3->time           = CTime(0); // �t�@�C���X�V��
    fileMP3->timeCreate     = CTime(0); // �t�@�C���쐬��
#ifdef USE_VBMP3_DLL
    fileMP3->nPlayTime      = 0;        // ���t����(�P�ʁF�b)
#endif
    fileMP3->nPlayTime      = -1;       // ���t����(�P�ʁF�b) /* Conspiracy 195 */
    fileMP3->bExistLyricFile = false;   // �̎��t�@�C���̑���
    fileMP3->nFormat        = FILE_FORMAT_UNKNOWN;  // �t�@�C���`���F���m�̌`��
    fileMP3->strFullPathName.Empty();   // �t�@�C����(�t���p�X)
    fileMP3->strFileName.Empty();       // �t�@�C����
    fileMP3->strFilePath.Empty();       // �p�X��

    // ID3 �̃N���A
    InitDataID3(fileMP3); /* STEP 029 */

    // SIF �̃N���A
    InitDataSIF(fileMP3);
}

void CFileMP3::InitDataID3(FILE_MP3 *fileMP3) /* STEP 029 */
{
    fileMP3->strTrackName.Empty();      // �g���b�N��
    fileMP3->strArtistName.Empty();     // �A�[�e�B�X�g��
    fileMP3->strAlbumName.Empty();      // �A���o����
    fileMP3->strComment.Empty();        // �R�����g
    fileMP3->strYear.Empty();           // �����[�X�N��
    fileMP3->byTrackNumber  = 0xff;     // �g���b�N�ԍ�(���l�ҏW�p)
    fileMP3->strTrackNumber.Empty();    // �g���b�N�ԍ�(������ҏW�p)
    fileMP3->byGenre        = 0xff;     // �W�������ԍ�
    fileMP3->strGenre.Empty();          // �W��������

}

// =============================================
//  CFileMP3::InitDataSIF
//  �T�v  : MP3 �t�@�C�����̍\���̂����������܂�(SIF�֘A�̂�)
//  ����  : fileMP3     = ����������\����
//  �߂�l: �Ȃ�
// =============================================
void CFileMP3::InitDataSIF(FILE_MP3 *fileMP3)
{
    // RIFF MP3 �� SI �t�B�[���h
    fileMP3->strTrackNameSI.Empty();    // �Ȗ�
    fileMP3->strArtistNameSI.Empty();   // �A�[�e�B�X�g��
    fileMP3->strAlbumNameSI.Empty();    // ���i��
    fileMP3->strCommentSI.Empty();      // �R�����g������
    fileMP3->strYearSI.Empty();         // �����[�X�N��
    fileMP3->strGenreSI.Empty();        // �W��������
    fileMP3->strCopyrightSI.Empty();    // ���쌠
    fileMP3->strEngineerSI.Empty();     // �G���W�j�A
    fileMP3->strSourceSI.Empty();       // �\�[�X
    fileMP3->strSoftwareSI.Empty();     // �\�t�g�E�F�A
    fileMP3->strKeywordSI.Empty();      // �L�[���[�h
    fileMP3->strTechnicianSI.Empty();   // �Z�p��
    fileMP3->strLyricSI.Empty();        // �̎�
    fileMP3->strCommissionSI.Empty();   // �R�~�b�V����

    if (IsUserGenre(fileMP3->strGenre)) {
        fileMP3->strGenre = GetGenreNameSIF(fileMP3->byGenre);
    }
    fileMP3->strTrackNumber.Empty();
    if (fileMP3->byTrackNumber != 0xFF) {
        fileMP3->strTrackNumber.Format(_T("%d"), fileMP3->byTrackNumber);
    }
    fileMP3->strTrackNumberSI.Empty();
    fileMP3->strTrackTotalSI.Empty();   // �g���b�N��(Kobarin)
    fileMP3->strDiscNumberSI.Empty();
    fileMP3->strDiscTotalSI.Empty();    // �f�B�X�N��(Kobarin)
    fileMP3->strWriterSI.Empty();       // �쎌
    fileMP3->strComposerSI.Empty();     // ���
    fileMP3->strAlbmArtistSI.Empty();   // Albm.�A�[�e�B�X�g
    fileMP3->strOrigArtistSI.Empty();   // Orig.�A�[�e�B�X�g
    fileMP3->strURLSI.Empty();          // URL
    fileMP3->strEncodest.Empty();       // �G���R�[�h�����l
    fileMP3->strOther.Empty();          // ���̑�

}

// =============================================
// CFileMP3::Initialize
// �T�v  : ����������
// ����  : none
// �߂�l: none
// =============================================
void CFileMP3::Initialize(void)
{
    //m_fileMP3.fileSpecificInfo = NULL;
    InitData(&m_fileMP3);
}

// =============================================
// CFileMP3::DeleteLineEndSpace
// �T�v  : �s���̃X�y�[�X����菜��
// ����  : sBuffer      = ������
// �߂�l: none
// =============================================
void CFileMP3::DeleteLineEndSpace(TCHAR *sBuffer)
{
#ifndef _UNICODE
    TCHAR *spc = NULL;
    while(*sBuffer){
        if(_istlead(*sBuffer)){
            if(!sBuffer[1]){
                *sBuffer = 0;
                break;
            }
            sBuffer+=2;
            spc = NULL;
            continue;
        }
        if(*sBuffer == _T(' ')){
            if(!spc){
                spc = sBuffer;
            }
        }
        else{
            spc = NULL;
        }
    }
    if(spc){
        *spc = 0;
    }
#else
    size_t len = _tcslen(sBuffer);
    while(len >= 1 && sBuffer[len-1] == _T(' ')) {
        sBuffer[len-1] = 0;
        len--;
    }
#endif
}

// =============================================
//  CFileMP3::Attach
//  �T�v  : �N���X�Ƀt�@�C���Ɋ֘A�t������
//  ����  : sFileName   = �t�@�C����
//  �߂�l: bool        = true:����I�� / false:�G���[
// =============================================
bool CFileMP3::Attach(const TCHAR *sFileName)
{
    Initialize();

    // �^�C���X�^���v���擾
    CFileStatus fileStatus = {0};
    try{//�^�C���X�^���v���ُ킾�Ɨ�O����������炵���̂ł��܂���

    if (CFile::GetStatus(sFileName, fileStatus) == FALSE || fileStatus.m_mtime == -1) {
        fileStatus.m_mtime = CTime(0);
        fileStatus.m_ctime = CTime(0);
    }

    }catch(...){
        fileStatus.m_mtime = CTime(0);
        fileStatus.m_ctime = CTime(0);
    }
    m_fileMP3.time          = fileStatus.m_mtime;   // �t�@�C���X�V��
    m_fileMP3.timeCreate    = fileStatus.m_ctime;   // �t�@�C���쐬��
    m_fileMP3.lFileSize     = fileStatus.m_size;    // �t�@�C���T�C�Y

    // �t���p�X���p�X���ƃt�@�C�����ɕ���
    TCHAR   drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
    _tsplitpath_s(sFileName, drive, dir, fname, ext);
    m_fileMP3.strFilePath.Format(_T("%s%s"), drive, dir);
    m_fileMP3.strFileName.Format(_T("%s%s"), fname, ext);
    m_fileMP3.strFullPathName = sFileName;

    // �t�@�C�����̓ǂݍ���
    extern bool LoadFile(const TCHAR *sFileName, const TCHAR *sExt, FILE_MP3* pFileMP3);
    if (LoadFile(sFileName, &ext[1], &m_fileMP3) == false) {
        Initialize();
        return(false);
    }

    // �̎��t�@�C���̑��݂��`�F�b�N
    if (g_bOptSearchLyricFile) {
        // �̎��t�@�C��������
        m_fileMP3.bExistLyricFile = SearchLyricsFile(&m_fileMP3).IsEmpty() ? false : true;
    }

    return(true);
}

// =============================================
//  CFileMP3::Detach
//  �T�v  : �N���X�ƃt�@�C���̊֘A�t�����폜����
//  ����  : �Ȃ�
//  �߂�l: �Ȃ�
// =============================================
void CFileMP3::Detach(void)
{
    Initialize();
}

// =============================================
//  CFileMP3::WriteTag
//  �T�v  : �^�O���̍X�V
//  ����  : fileMP3     = �t�@�C�����
//        : bKeepTimeStamp= �^�C���X�^���v��ێ�����t���O
//  �߂�l: bool        = true:����I�� / false:�G���[
// =============================================
bool CFileMP3::WriteTag(FILE_MP3 *fileMP3, bool bKeepTimeStamp)
{
    CFileStatus fileStatus;
    bool    bResult = false;

    if (g_nWriteTagProcFlag == 3) {
        return(false);                      // �ȍ~���~
    }

    try{
        // �^�C���X�^���v���擾
        if (CFile::GetStatus(fileMP3->strFullPathName, fileStatus) == FALSE) {
            return(false);
        }
        if (fileStatus.m_mtime == -1)   fileStatus.m_mtime = fileStatus.m_ctime; /* 2005.10.11 ���������������ꍇ�ɗ����Ȃ��悤�� */
        if (fileStatus.m_atime == -1)   fileStatus.m_atime = fileStatus.m_mtime; /* 2005.10.11 ���������������ꍇ�ɗ����Ȃ��悤�� */
    }
    catch (...) {
        if (fileStatus.m_mtime == -1)   fileStatus.m_mtime = fileStatus.m_ctime;
        if (fileStatus.m_atime == -1)   fileStatus.m_atime = fileStatus.m_mtime;
    }

    // �ǂݏo����p�t�@�C���́A�㏑���m�F���s��
    if (fileStatus.m_attribute & CFile::readOnly) {
        if (g_nWriteTagProcFlag != 1) {
            // �㏑���m�F�_�C�A���O���J��
            CDlgFileOverWrite   dialog;
            dialog.m_strFileName = fileMP3->strFullPathName;
            dialog.m_strSize.Format(_T("%ld byte"), fileStatus.m_size);
            if (fileStatus.m_mtime.GetTime() < 0) {
                dialog.m_strTimeStamp.Format(_T("----/--/-- --:--:--"));
            } else {
                dialog.m_strTimeStamp.Format(_T("%04d/%02d/%02d %02d:%02d:%02d"),
                                             fileStatus.m_mtime.GetYear(),
                                             fileStatus.m_mtime.GetMonth(),
                                             fileStatus.m_mtime.GetDay(),
                                             fileStatus.m_mtime.GetHour(),
                                             fileStatus.m_mtime.GetMinute(),
                                             fileStatus.m_mtime.GetSecond());
            }
            dialog.DoModal();
            g_nWriteTagProcFlag = dialog.m_nResult;
            switch(dialog.m_nResult) {
            case 0:             // ���̃t�@�C�����㏑��
            case 1:             // �ȍ~�S�ď㏑��
                break;
            case 2:             // �X�L�b�v
            case 3:             // ���~
            default:
                return(false);
            }
        }

        // �ǂݍ��ݐ�p����������
        SetFileAttributes(fileMP3->strFullPathName, CFile::normal);
        // �ύX��ɑ�����߂����邽�߂� bKeepTimeStamp �� true �ɂ���
        bKeepTimeStamp = true;
    }

    extern bool SaveFile(FILE_MP3* pFileMP3);
    if ((bResult = SaveFile(fileMP3)) == true) {
        // �t�@�C�����ϊ�
        //bResult = ConvFileName(fileMP3);
    }

    // �^�C���X�^���v���t�@�C���X�V�O�ɖ߂�
    if (bResult) {
        if (bKeepTimeStamp) {
            if (fileMP3->time.GetTime() != -1) {
                // �t�@�C���X�V���Ԃ�ݒ�
                fileStatus.m_mtime = fileMP3->time;
                if (g_bOptSyncCreateTime) fileStatus.m_ctime = fileMP3->time;
            }
            TRY {
                CFile::SetStatus(fileMP3->strFullPathName, fileStatus);
            }
            CATCH(CFileException, e) {
                CString strMsg;
                strMsg.Format(_T("%s ���I�[�v���ł��܂���ł���"), fileMP3->strFullPathName);
                MessageBox(NULL, strMsg, _T("�^�C���X�^���v�̍X�V�G���["), MB_ICONSTOP|MB_OK|MB_TOPMOST);
                bResult = false;
            }
            END_CATCH
        } else {
            // �X�V��̃^�C���X�^���v���擾
            try{
            if (CFile::GetStatus(fileMP3->strFullPathName, fileStatus) != FALSE) {
                fileMP3->time = fileStatus.m_mtime;
            }
            }
            catch(...){
                fileMP3->time = fileStatus.m_mtime;
            }
        }
    }

    return(bResult);
}

// =============================================
// CFileMP3::SearchFileReent
// �T�v  : �t�@�C�������F��������
// ����  : sDir         = ��������f�B���N�g��
//       : sTargetName  = ��������t�@�C����
// �߂�l: CString      = ���������t�@�C���̃t���p�X(IsEmpty():������)
// =============================================
CString CFileMP3::SearchFileReent(const TCHAR *sDir, const TCHAR *sTargetName)
{
    HANDLE  hFindFile;
    WIN32_FIND_DATA data;
    CString strTargetFile;

    // ����f�B���N�g���̑S�Ẵt�@�C��������
    // �����t�@�C���̃p�^�[����ݒ�
    CString strCurFile;
    strCurFile.Format(_T("%s%s"), sDir, sTargetName);
    if ((hFindFile = FindFirstFile(strCurFile, &data)) != INVALID_HANDLE_VALUE) {
        do {
            if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
                // �t�@�C����������
                strCurFile.Format(_T("%s%s"), sDir, data.cFileName);
                return(strCurFile);
            }
        } while(FindNextFile(hFindFile, &data));
    }
    if (hFindFile != INVALID_HANDLE_VALUE) FindClose(hFindFile);

    // �T�u�f�B���N�g���̌���
    if (g_bOptSearchLyricsSubDir == true) {
        strCurFile.Format(_T("%s*.*"), sDir);
        if ((hFindFile = FindFirstFile(strCurFile, &data)) != INVALID_HANDLE_VALUE) {
            do {
                if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
                    if (_tcscmp(data.cFileName, _T(".") ) != 0      // �J�����g�f�B���N�g���ȊO
                    &&  _tcscmp(data.cFileName, _T("..")) != 0      // �e�f�B���N�g���ȊO
                    && (data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == 0 /* STEP 032 */) { // �B���t�H���_�ȊO
                        // �T�u�f�B���N�g����������
                        strCurFile.Format(_T("%s%s\\"), sDir, data.cFileName);
                        strTargetFile = SearchFileReent(strCurFile, sTargetName);
                        if (strTargetFile.IsEmpty() == false) {
                            break;              // �t�@�C������
                        }
                    }
                }
            } while(FindNextFile(hFindFile, &data));
        }
        if (hFindFile != INVALID_HANDLE_VALUE) FindClose(hFindFile);
    }

    return(strTargetFile);
}

// =============================================
//  CFileMP3::SearchLyricsFile
//  �T�v  : �̎��t�@�C���̌���
//        : �D�揇�ʁFMP3�Ɠ����t�H���_���̎��t�H���_���̎��t�H���_�̃T�u�f�B���N�g��
//  ����  : fileMP3     = �t�@�C�����
//  �߂�l: CString     = �̎��t�@�C����(������Ȃ������ꍇ�͋�)
// =============================================
CString CFileMP3::SearchLyricsFile(FILE_MP3 *fileMP3)
{
    TCHAR   sTextName[FILENAME_MAX+1];

    // �܂��� MP3 �t�@�C���Ɠ����t�H���_���`�F�b�N
    TCHAR   drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME];
    _tsplitpath_s(fileMP3->strFullPathName, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, NULL, 0);
    // .lrc ������
    _tmakepath_s(sTextName, drive, dir, fname, _T(".lrc"));
    if (GetFileAttributes(sTextName) != 0xFFFFFFFF) {
        // �t�@�C���𔭌�
        return(CString(sTextName));
    }
    // .txt ������
    _tmakepath_s(sTextName, drive, dir, fname, _T(".txt"));
    if (GetFileAttributes(sTextName) != 0xFFFFFFFF) {
        // �t�@�C���𔭌�
        return(CString(sTextName));
    }

    // �̎��t�@�C�����쐬
    if (g_bOptSetLyricsDir == true && g_strOptLyricsPath.IsEmpty() == false) {
        // �w�肳�ꂽ�t�H���_
        TCHAR   sLyricsPath[FILENAME_MAX+1];
        CString strTarget;
        _tsplitpath_s(fileMP3->strFullPathName, NULL, 0, NULL, 0, fname, _MAX_FNAME, NULL, 0);
        _tcsncpy_s(sLyricsPath, FILENAME_MAX, g_strOptLyricsPath, _TRUNCATE);
        if (IsFolderName(sLyricsPath) == false) {
            _tcsncat_s(sLyricsPath, FILENAME_MAX+1, _T("\\"), _TRUNCATE);
        }
        // .lrc ������
        strTarget.Format(_T("%s.lrc"), fname);
        strTarget = SearchFileReent(sLyricsPath, strTarget);
        if (strTarget.IsEmpty() == false
        &&  GetFileAttributes(strTarget) != 0xFFFFFFFF) {
            // �t�@�C���𔭌�
            return(strTarget);
        }
        // .txt ������
        strTarget.Format(_T("%s.txt"), fname);
        strTarget = SearchFileReent(sLyricsPath, strTarget);
        if (strTarget.IsEmpty() == false
        &&  GetFileAttributes(strTarget) != 0xFFFFFFFF) {
            // �t�@�C���𔭌�
            return(strTarget);
        }
    }

    // ������Ȃ�����
    return(CString(""));
}

// =============================================
//  CFileMP3::ConvFileName
//  �T�v  : �t�@�C�����̕ύX
//        : ���V�����t�@�C������ fileMP3->strFileName ���g�p
//  ����  : fileMP3     = �t�@�C�����
//  �߂�l: bool        = true:����I�� / false:�G���[
// =============================================
bool CFileMP3::ConvFileName(FILE_MP3 *fileMP3)
{
    // �t�@�C�����𕪉�
    CString strFileName;
    TCHAR   drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
    _tsplitpath_s(fileMP3->strFullPathName, drive, dir, fname, ext);
    strFileName.Format(_T("%s%s"), fname, ext);

    if (_tcscmp(strFileName, fileMP3->strFileName) != 0) {
        // �t�@�C�������ύX����Ă���
        if (g_bConfFileNameMaxCheck) {
            if (fileMP3->strFileName.GetLength() > g_nConfFileNameMaxChar) {
                // �t�@�C�����������I�[�o�[
                _tsplitpath_s(fileMP3->strFileName, NULL, 0, NULL, 0, fname, _MAX_FNAME, ext, _MAX_EXT);
                CDlgFileNameChange dialog;
                int ext_len = _tcslen(ext);
                if(ext_len > g_nConfFileNameMaxChar){
                    ext_len = g_nConfFileNameMaxChar;
                }
                dialog.m_nMaxChar = g_nConfFileNameMaxChar - ext_len;
                dialog.m_strFileName = fname;
                dialog.m_strOrgFileName = fname;
                //dialog.m_strMsg.Format("�t�@�C������ %d �����ȓ��ɕύX���Ă�������", dialog.m_nMaxChar);
                dialog.m_strMsgFormat = _T("�t�@�C������ %d(%d) �����ȓ��ɕύX���Ă�������");
                if (dialog.DoModal() == IDOK) {
                    fileMP3->strFileName = dialog.m_strFileName + ext;
                    _tsplitpath_s(fileMP3->strFileName, NULL, 0, NULL, 0, fname, _MAX_FNAME, ext, _MAX_EXT);
                }
            }
        }
        TCHAR   sNewName[FILENAME_MAX+1];
        _tmakepath_s(sNewName, drive, dir, NULL, NULL);
        _tcsncat_s(sNewName, fileMP3->strFileName, _TRUNCATE);
        if (MyChangeFileName(fileMP3->strFullPathName, sNewName) == false) {
            return(false);
        }

        // �����̃e�L�X�g�t�@�C���̃t�@�C�������ϊ�����
        if (g_bOptChangeTextFile) {
            CString strLyricsFile;
            // �̎��t�@�C��������
            strLyricsFile = SearchLyricsFile(fileMP3);
            if (strLyricsFile.IsEmpty() == false) {
                // ���������̎��t�@�C���̃p�X�𕪉�
                _tsplitpath_s(strLyricsFile, drive, dir, fname, ext);

                // �̎��t�@�C�������l�[������
                TCHAR   sNewText[FILENAME_MAX+1];
                _tsplitpath_s(fileMP3->strFileName, NULL, 0, NULL, 0, fname, FILENAME_MAX+1, NULL, 0);
                _tmakepath_s(sNewText, drive, dir, fname, ext);
                if (MyChangeFileName(strLyricsFile, sNewText) == false) {
                    // return(false);
                }
            }
        }

        // �t�@�C�������X�V����
        fileMP3->strFullPathName = sNewName;
    }

    return(true);
}

// =============================================
//  CFileMP3::CopyFile
//  �T�v  : �t�@�C���̃R�s�[�^�ړ�
//  ����  : fileMP3     = �t�@�C�����
//        : sNewDir     = �ړ���t�H���_��
//        : bMoveFlag   = �t���O(true:�ړ�/false:�R�s�[)
//  �߂�l: bool        = true:����I�� / false:�G���[
// =============================================
bool CFileMP3::CopyFile(FILE_MP3 *fileMP3, const TCHAR *sNewDir, bool bMoveFlag)
{
    // �t�@�C�����𕪉�
    CString strFileName;
    TCHAR   drive[_MAX_DRIVE], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
    _tsplitpath_s(fileMP3->strFullPathName, drive, dir, fname, ext);
    strFileName.Format(_T("%s%s"), fname, ext);

    if (_tcsicmp(fileMP3->strFilePath, sNewDir) != 0) {
        // �t�@�C�������ύX����Ă���
        CString strNewName;
        strNewName.Format(_T("%s%s"), sNewDir, strFileName);
        if (MyCopyFile(fileMP3->strFullPathName, strNewName, bMoveFlag) == false) {
            // �t�@�C���̃R�s�[�^�ړ��Ɏ��s!!
            return(false);
        }

        // �ړ��̏ꍇ�́A�̎��t�@�C�����ړ�����
        if (g_bSyncLyricsFileMove && bMoveFlag == true) {
            TCHAR   sLyricsFile[FILENAME_MAX+1];
            // MP3 �t�@�C���Ɠ����t�H���_�ɂ���̎��t�@�C��������
            _tsplitpath_s(fileMP3->strFullPathName, drive, dir, fname, ext);
            // .lrc => .txt �̏��Ō���
            int i; for (i = 0; i < 2; i++) {
                LPTSTR  sLyricsExt = (i == 0) ? _T(".lrc") : _T(".txt");
                _tmakepath_s(sLyricsFile, drive, dir, fname, sLyricsExt);
                if (GetFileAttributes(sLyricsFile) != 0xFFFFFFFF) {
                    // �t�@�C���𔭌��F�̎��t�@�C�����ړ�����
                    CString strNewName;
                    strNewName.Format(_T("%s%s%s"), sNewDir, fname, sLyricsExt);
                    if (MyCopyFile(sLyricsFile, strNewName, true) == false) {
                        // ���s!!
                        // return(false);
                    }
                    break;
                }
            }
        }

        // �Ǘ����̃t�@�C�������X�V����
        if (bMoveFlag) {
            fileMP3->strFilePath = sNewDir;
            fileMP3->strFullPathName = strNewName;
        }
    }

    return(true);
}

CString CFileMP3::GetIntTrackNo(const CString &no)
{
    //�g���b�N�ԍ�
    if (IsNumeric(no)) {
        return no;
    } else {
        /* BeachMonster 094 */
        {
            int index = 0;
            for (int i=1;i<no.GetLength();i++) {
                if (IsNumeric(no.Left(i))) {
                    index = i;
                    continue;
                } else {
                    break;
                }
            }
            if (index == 0) {
                //return "255";
            } else {
                CString track;
                track.Format(_T("%d"), _ttoi(no.Left(index)));
                return track;
            }
        }
        {
            int index = 0;
            for (int i=1;i<no.GetLength();i++) {
                if (IsNumeric(no.Right(i))) {
                    index = i;
                    continue;
                } else {
                    break;
                }
            }
            if (index == 0) {
                return "";
            } else {
                CString track;
                track.Format(_T("%d"), _ttoi(no.Right(index)));
                return track;
            }
        }
    }
}

CString CFileMP3::GetIntDiscNo(const CString &no)
{
    //�g���b�N�ԍ�
    if (IsNumeric(no)) {
        return no;
    } else {
        /* BeachMonster 094 */
        {
            int index = 0;
            for (int i=1;i<no.GetLength();i++) {
                if (IsNumeric(no.Left(i))) {
                    index = i;
                    continue;
                } else {
                    break;
                }
            }
            if (index == 0) {
                //return "255";
            } else {
                CString Disc;
                Disc.Format(_T("%d"), _ttoi(no.Left(index)));
                return Disc;
            }
        }
        {
            int index = 0;
            for (int i=1;i<no.GetLength();i++) {
                if (IsNumeric(no.Right(i))) {
                    index = i;
                    continue;
                } else {
                    break;
                }
            }
            if (index == 0) {
                return "";
            } else {
                CString Disc;
                Disc.Format(_T("%d"), _ttoi(no.Right(index)));
                return Disc;
            }
        }
    }
}


bool CFileMP3::IsNumeric(const CString &str)
{
    if (str.IsEmpty()) {
        return TRUE/*FALSE*/; /* WildCherry 063 */
    }
    for (int i=0;i<str.GetLength();i++) {
        if (_istlead(str[i])) {
            // �������[�f�B���O�o�C�g
            i++;
            return FALSE;
        } else {
            if (!_istdigit(str[i])) {
                return FALSE;
            }
        }
    }
    return TRUE;
}
