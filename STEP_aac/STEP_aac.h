// STEP_aac.h : STEP_AAC アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_STEP_AAC_H__F323C16B_BA05_4A27_9EA6_970617BD5AFA__INCLUDED_)
#define AFX_STEP_AAC_H__F323C16B_BA05_4A27_9EA6_970617BD5AFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"        // メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CSTEP_aacApp
// このクラスの動作の定義に関しては STEP_aac.cpp ファイルを参照してください。
//

class CSTEP_aacApp : public CWinApp
{
public:
    CSTEP_aacApp();

// オーバーライド
    // ClassWizard は仮想関数のオーバーライドを生成します。
    //{{AFX_VIRTUAL(CSTEP_aacApp)
    //}}AFX_VIRTUAL

    //{{AFX_MSG(CSTEP_aacApp)
        // メモ -  ClassWizard はこの位置にメンバ関数を追加または削除します。
        //         この位置に生成されるコードを編集しないでください。
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STEP_AAC_H__F323C16B_BA05_4A27_9EA6_970617BD5AFA__INCLUDED_)
