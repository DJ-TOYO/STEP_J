// MainFrm.cpp : CMainFrame クラスの動作の定義を行います。
//

#include "stdafx.h"
#include "AutoBuildCount.h"
#include "SuperTagEditor.h"
#include "SuperTagEditorView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    //{{AFX_MSG_MAP(CMainFrame)
    ON_WM_CREATE()
    ON_WM_SIZE()
    ON_WM_MOVE()
    ON_WM_CLOSE()
    ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_TOOLBAR, OnToolbarDropDown)
    ON_UPDATE_COMMAND_UI(ID_INDICATOR_LOCATION, OnUpdateIndicatorEnable)
    ON_MESSAGE(WM_USER_SET_STATUS_POS, OnSetStatusPos)
    ON_MESSAGE(WM_USER_SET_STATUS_SIZE, OnSetStatusSize)
    ON_MESSAGE(WM_USER_SET_STATUS_INPUT_MODE, OnSetStatusInputMode)
    ON_COMMAND(ID_POPUP_CUSTOMIZE, OnPopupCustomize)
    ON_UPDATE_COMMAND_UI(ID_POPUP_CUSTOMIZE, OnUpdatePopupCustomize)
    ON_NOTIFY_EX( TTN_NEEDTEXT, 0, OnToolTipNotify )
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

#ifdef _WIN64
#define _RESERVED_PARAM 0,0,0,0,0,0
#else
#define _RESERVED_PARAM 0,0
#endif
CToolBarInfo CMainFrame::mainToolBar[] =
{
    {{0, ID_FILE_NEW  , TBSTATE_ENABLED, TBSTYLE_BUTTON,    _RESERVED_PARAM, 0, 0}, _T("リストをクリア")     },
    {{1, ID_OPEN_FOLDER , TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN /* StartInaction 053 */,_RESERVED_PARAM, 0, 1}, _T("フォルダを開く")   },
    {{2, ID_SAVE_ALL_TAG , TBSTATE_ENABLED, TBSTYLE_BUTTON, _RESERVED_PARAM, 0, 2}, _T("タグ情報を更新")    },
    {{3, ID_SELECT_EDIT_DESTORY  , TBSTATE_ENABLED, TBSTYLE_BUTTON,_RESERVED_PARAM, 0, 3}, _T("編集前の状態に戻す") },
    {{0, ID_SEPARATOR , TBSTATE_ENABLED, TBSTYLE_SEP, _RESERVED_PARAM,0, 4}, _T("")        },
    {{4, ID_EDIT_CUT , TBSTATE_ENABLED, TBSTYLE_BUTTON, _RESERVED_PARAM,0, 5}, _T("切り取り")    },
    {{5, ID_EDIT_COPY, TBSTATE_ENABLED, TBSTYLE_BUTTON, _RESERVED_PARAM,0, 6}, _T("コピー")   },
    {{6, ID_EDIT_PASTE, TBSTATE_ENABLED, TBSTYLE_BUTTON, _RESERVED_PARAM,0, 7}, _T("貼り付け")   },
    {{0, ID_SEPARATOR , TBSTATE_ENABLED, TBSTYLE_SEP, _RESERVED_PARAM,0, 8}, _T("")        },
    {{7, ID_CELL_COPY_DOWN , TBSTATE_ENABLED, TBSTYLE_BUTTON, _RESERVED_PARAM,0, 9}, _T("下方向へコピー")},
    {{0, ID_SEPARATOR , TBSTATE_ENABLED, TBSTYLE_SEP, _RESERVED_PARAM,0, 10}, _T("")        },
    {{8, ID_EDIT_TD3_TAG , TBSTATE_ENABLED, TBSTYLE_CHECK, _RESERVED_PARAM,0, 11}, _T("ID3 tagを表示／編集")},
    {{9, ID_EDIT_FIELD_SI , TBSTATE_ENABLED, TBSTYLE_CHECK, _RESERVED_PARAM,0, 12}, _T("SIフィールドを表示／編集")},
    {{0, ID_SEPARATOR , TBSTATE_ENABLED, TBSTYLE_SEP, _RESERVED_PARAM,0, 13}, _T("")        },
    {{10, ID_WINAMP_PLAY_PREV , TBSTATE_ENABLED, TBSTYLE_BUTTON, _RESERVED_PARAM, 0, 14}, _T("前の曲へ")},
    {{11, ID_WINAMP_PLAY , TBSTATE_ENABLED, TBSTYLE_BUTTON,_RESERVED_PARAM, 0, 15}, _T("再生")},
    {{12, ID_WINAMP_STOP , TBSTATE_ENABLED, TBSTYLE_BUTTON, _RESERVED_PARAM,0, 16}, _T("停止")},
    {{13, ID_WINAMP_PLAY_NEXT , TBSTATE_ENABLED, TBSTYLE_BUTTON, _RESERVED_PARAM,0, 17}, _T("次の曲へ")},
    {{0, ID_SEPARATOR , TBSTATE_ENABLED, TBSTYLE_SEP, _RESERVED_PARAM,0, 18}, _T("")        },
    {{14, ID_DLG_ENVIRONMENT , TBSTATE_ENABLED, TBSTYLE_BUTTON, _RESERVED_PARAM,0, 19}, _T("オプション")},
    {{15, ID_APP_ABOUT , TBSTATE_ENABLED, TBSTYLE_BUTTON, _RESERVED_PARAM,0, 20}, _T("バージョン情報")},
    {{0, ID_SEPARATOR , TBSTATE_ENABLED, TBSTYLE_SEP, _RESERVED_PARAM,0, 21}, _T("")        },
    {{16, ID_CONV_FORMAT_USER , TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN, _RESERVED_PARAM,0, 22}, _T("ユーザ書式切替")},
    {{17, ID_SET_NUMBER , TBSTATE_ENABLED, TBSTYLE_BUTTON,_RESERVED_PARAM, 0, 23}, _T("下方向へ連番")},
    {{18, ID_SET_NUMBER_ADD , TBSTATE_ENABLED, TBSTYLE_BUTTON,_RESERVED_PARAM, 0, 24}, _T("下方向へ連番を追加")},
    {{19, ID_SELECT_TREE_COLUM , TBSTATE_ENABLED, TBSTYLE_BUTTON,_RESERVED_PARAM, 0, 25}, _T("ツリー配下のセル一括選択")},
    {{20, ID_TEIKEI_TOOL , TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,_RESERVED_PARAM, 0, 26}, _T("定型文貼り付け")},
    {{21, ID_CHECK_FILENAME_MAX , TBSTATE_ENABLED, TBSTYLE_BUTTON,_RESERVED_PARAM, 0, 27}, _T("ファイル名の長さをチェック")},
    {{22, ID_SELECT_DELETE_LIST , TBSTATE_ENABLED, TBSTYLE_BUTTON,_RESERVED_PARAM, 0, 28}, _T("リストから削除")},
    {{23, ID_DELETE_CHAR , TBSTATE_ENABLED, TBSTYLE_BUTTON,_RESERVED_PARAM, 0, 29}, _T("先頭/末尾のｎ文字を削除")},
    {{24, ID_UNIFY_CHAR , TBSTATE_ENABLED, TBSTYLE_BUTTON,_RESERVED_PARAM, 0, 30}, _T("文字種の統一")},
    {{25, ID_END_EDIT_RIGHT , TBSTATE_ENABLED, TBSTYLE_CHECK,_RESERVED_PARAM, 0, 31}, _T("項目変更後にカーソルを右に移動")}, /* BeachMonster 091 */
    {{26, ID_FAVORITES_FOLDER , TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_DROPDOWN,_RESERVED_PARAM, 0, 32}, _T("お気に入りのフォルダ")}, /* RockDance 129 */
    {{27, ID_CALC_FOLDER_TOTAL , TBSTATE_ENABLED, TBSTYLE_BUTTON,_RESERVED_PARAM, 0, 33}, _T("合計の算出")}, /* RockDance 128 */
    {{28, ID_EDIT_PASTE_ADD , TBSTATE_ENABLED, TBSTYLE_BUTTON,_RESERVED_PARAM, 0, 34}, _T("追加で貼り付け")}, /* Baja 171 */
    {{29, ID_DELETE_CHAR_SPACE , TBSTATE_ENABLED, TBSTYLE_BUTTON,_RESERVED_PARAM, 0, 35}, _T("先頭/末尾の空白文字を削除")}, /* Rumble 192 */
    {{30, ID_SELECT_TREE_FILE , TBSTATE_ENABLED, TBSTYLE_BUTTON,_RESERVED_PARAM, 0, 36}, _T("ツリー配下のファイル一括選択")}, /* STEP 013 */
    {{31, ID_MOVE_TO_PARENT , TBSTATE_ENABLED, TBSTYLE_BUTTON,_RESERVED_PARAM, 0, 37}, _T("カーソルを親ツリーへ移動")}, /* STEP 014 */
//    {{32, ID_MOVE_TO_PREVIOUS , TBSTATE_ENABLED, TBSTYLE_BUTTON,_RESERVED_PARAM, 0, 38}, _T("カーソル位置を一つ上のツリーへ移動")}, /* STEP 014 */
//    {{33, ID_MOVE_TO_NEXT , TBSTATE_ENABLED, TBSTYLE_BUTTON,_RESERVED_PARAM, 0, 39}, _T("カーソル位置を一つ下のツリーへ移動")}, /* STEP 014 */
    {{32, ID_DLG_SETUP_PLUGIN , TBSTATE_ENABLED, TBSTYLE_BUTTON,_RESERVED_PARAM, 0, 38}, _T("プラグインオプション")}, /* STEP 039*/
    /**
     【注意】コマンドを追加した場合は、_APS_NEXT_COMMAND_VALUEが更新されるのでSTEP_api.cppをリコンパイルすること
     **/
};

static UINT indicators[] =
{
    ID_SEPARATOR,           // ステータス ライン インジケータ
    ID_INDICATOR_SIZE, /* WildCherry 072 */
    ID_INDICATOR_LOCATION,
    ID_INDICATOR_INPUT_MODE, /* Misirlou 150 */
    ID_INDICATOR_KANA,
    ID_INDICATOR_CAPS,
    ID_INDICATOR_NUM,
    ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame クラスの構築/消滅

CMainFrame::CMainFrame()
{
    // TODO: この位置にメンバの初期化処理コードを追加してください。

}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;

    if (/*!m_wndToolBar.CreateEx(this) || AstralCircle 041 */
        !m_wndToolBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP,
        AFX_IDW_TOOLBAR, mainToolBar,
        _T("Software\\MERCURY\\SuperTagEditor\\haseta\\Toolbar"),
        _T("ToolBar Settings")) ||
        !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
    {
        TRACE0("Failed to create toolbar\n");
        return -1;      // 作成に失敗
    }

    extern CDkToolBar* STEP_wndToolBar;
    STEP_wndToolBar = &m_wndToolBar;

    // ダイアログバー作成
//    if (!m_wndDlgBar.Create(this, IDR_MAINFRAME, CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR)) {
//        TRACE0("Failed to create dialogbar\n");
//        return -1;        // 作成に失敗
//    }

    if (!m_wndReBar.Create(this) ||
        !m_wndReBar.AddBar(&m_wndToolBar)/* ||
        !m_wndReBar.AddBar(&m_wndDlgBar)*/)
    {
        TRACE0("Failed to create rebar\n");
        return -1;      // 作成に失敗
    }

    if (!m_wndStatusBar.Create(this) ||
        !m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT)))
    {
        TRACE0("Failed to create status bar\n");
        return -1;      // 作成に失敗
    }

    // TODO: もしツール チップスが必要ない場合、ここを削除してください。
    m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
        CBRS_TOOLTIPS | CBRS_FLYBY);


    // バージョンアップ後の初回起動時処理
//    if (strcmp(PROG_VERSION, "0.32") == 0) {
//        if (g_bIsVersionUp == false) {
//            LoadBarState(_T("ToolBar"));
//        }
//    } else {
        LoadBarState(_T("ToolBar"));
//    }

    { /* AstralCircle 041 */
        LONG lStyle = GetWindowLong(m_wndToolBar, GWL_STYLE);
        //  TBSTYLE_FLATスタイルを指定
        lStyle |= TBSTYLE_FLAT;
        //  新しいウィンドウスタイルを設定
        SetWindowLong(m_wndToolBar, GWL_STYLE, lStyle);
        m_wndToolBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);
        DWORD dwStyle = m_wndToolBar.GetButtonStyle(m_wndToolBar.CommandToIndex(ID_CONV_FORMAT_USER));
        dwStyle |= TBSTYLE_DROPDOWN;
        m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_CONV_FORMAT_USER), dwStyle);
        dwStyle = m_wndToolBar.GetButtonStyle(m_wndToolBar.CommandToIndex(ID_TEIKEI_TOOL));
        dwStyle |= TBSTYLE_DROPDOWN;
        m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_TEIKEI_TOOL), dwStyle);
        // StartInaction 053 */
        dwStyle = m_wndToolBar.GetButtonStyle(m_wndToolBar.CommandToIndex(ID_OPEN_FOLDER));
        dwStyle |= TBSTYLE_DROPDOWN;
        m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_OPEN_FOLDER), dwStyle);
        // RockDance 129 */
        dwStyle = m_wndToolBar.GetButtonStyle(m_wndToolBar.CommandToIndex(ID_FAVORITES_FOLDER));
        dwStyle |= TBSTYLE_DROPDOWN;
        m_wndToolBar.SetButtonStyle(m_wndToolBar.CommandToIndex(ID_FAVORITES_FOLDER), dwStyle);

        int nIndex = m_wndToolBar.GetToolBarCtrl().AddBitmap(USER_CONV_FORMAT_MAX, IDR_TOOLBAR_DYNAMIC); /* 3 LastTrain 057 */
        m_wndToolBar.nDyImageIndex = nIndex;

        m_wndToolBar.RestoreState();
    }
    UpdateToolBar();

    return 0;
}

BOOL CMainFrame::OnCreateClient( LPCREATESTRUCT lpcs,
    CCreateContext* pContext)
{
    return CFrameWnd::OnCreateClient(lpcs, pContext);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
    if( !CFrameWnd::PreCreateWindow(cs) )
        return FALSE;

    if (g_rectMainWindow.top != 0
    &&  g_rectMainWindow.left != 0
    &&  g_rectMainWindow.bottom != 0
    &&  g_rectMainWindow.right != 0) {
        // 前回のウィンドウサイズで開く
        cs.x    = g_rectMainWindow.left;
        cs.y    = g_rectMainWindow.top;
        cs.cx    = g_rectMainWindow.right - g_rectMainWindow.left;
        cs.cy    = g_rectMainWindow.bottom - g_rectMainWindow.top;
    }

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame クラスの診断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
    CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
    CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame メッセージ ハンドラ
void CMainFrame::OnMove(int x, int y)
{
    CFrameWnd::OnMove(x, y);

    // ウィンドウの状態を保存
    WINDOWPLACEMENT wndpl;
    AfxGetMainWnd()->GetWindowPlacement(&wndpl);
    if (AfxGetMainWnd()->IsZoomed() == FALSE && AfxGetMainWnd()->IsIconic() == FALSE) {
        GetWindowRect(&g_rectMainWindow);
    }
}

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
    CFrameWnd::OnSize(nType, cx, cy);

    // ウィンドウの状態を保存
    WINDOWPLACEMENT wndpl;
    AfxGetMainWnd()->GetWindowPlacement(&wndpl);
    if (AfxGetMainWnd()->IsZoomed() == FALSE && AfxGetMainWnd()->IsIconic() == FALSE) {
        GetWindowRect(&g_rectMainWindow);
    }
}

void CMainFrame::OnClose()
{
    SaveBarState(_T("ToolBar"));

    CFrameWnd::OnClose();
}

void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
    CFrameWnd::OnUpdateFrameTitle(bAddToTitle);

    CString strTitle(PROG_NAME " Ver " PROG_VERSION "(" PROG_PLATFORM ")");
    SetWindowText(strTitle);
}

void CMainFrame::OnUpdateIndicatorEnable(CCmdUI* pCmdUI)
{
    pCmdUI->Enable(TRUE);
}

LRESULT CMainFrame::OnSetStatusPos(WPARAM wParam, LPARAM lParam)
{
    static    CString str;
    str.Format(_T("%d：%d"), (signed short)((lParam>>16)&0xffff), (signed short)(lParam&0xffff));
    m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_LOCATION), (LPCTSTR)str);
    m_wndStatusBar.UpdateWindow();
    return 0;
}

LRESULT CMainFrame::OnSetStatusSize(WPARAM wParam, LPARAM lParam) /* WildCherry 072 */
{
    CClientDC dc(&m_wndStatusBar);
    CFont *pOldFont = dc.SelectObject(m_wndStatusBar.GetFont());
    CSize size = dc.GetTextExtent((LPCTSTR)lParam);
    //int margin = dc.GetTextExtent(_T(" ")).cx * 2;
    dc.SelectObject(pOldFont);
    m_wndStatusBar.SetPaneInfo(m_wndStatusBar.CommandToIndex(ID_INDICATOR_SIZE), ID_INDICATOR_SIZE, 0, size.cx); /* + margin*/
    m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_SIZE), (LPCTSTR)lParam);
    m_wndStatusBar.UpdateWindow();
    return 0;
}

LRESULT CMainFrame::OnSetStatusInputMode(WPARAM wParam, LPARAM lParam) /* Misirlou 150 */
{
    CClientDC dc(&m_wndStatusBar);
    CFont *pOldFont = dc.SelectObject(m_wndStatusBar.GetFont());
    CSize size = dc.GetTextExtent((LPCTSTR)lParam);
    //int margin = dc.GetTextExtent(_T(" ")).cx * 2;
    dc.SelectObject(pOldFont);
    m_wndStatusBar.SetPaneInfo(m_wndStatusBar.CommandToIndex(ID_INDICATOR_INPUT_MODE), ID_INDICATOR_INPUT_MODE, 0, size.cx/* + margin*/);
    m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_INPUT_MODE), (LPCTSTR)lParam);
    m_wndStatusBar.UpdateWindow();
    return 0;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
    CSuperTagEditorApp    *pApp = (CSuperTagEditorApp *)AfxGetApp();

    if (pApp != NULL && pApp->GetAccelHandle() != 0) {
        // 独自のアクセラレータキーを処理する
        if (TranslateAccelerator(m_hWnd, pApp->GetAccelHandle(), pMsg)) {
            return TRUE;
        }
    }

    return CFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::ToolbarDropDown(int item){
    NMTOOLBAR nmhdr = {0};
    nmhdr.iItem = item;

    OnToolbarDropDown((NMHDR*)&nmhdr,NULL);
}

void CMainFrame::OnToolbarDropDown(NMHDR* pnmtb_, LRESULT *plr) /* AstralCircle 041 */
{
    NMTOOLBAR *pnmtb;
    pnmtb = (NMTOOLBAR*)pnmtb_;
    CWnd *pWnd = &m_wndToolBar;
    //UINT nID;

    // Switch on button command id's.
    CMenu menu;
    CMenu* pPopup = NULL;
    switch (pnmtb->iItem) {
    case ID_CONV_FORMAT_USER:
        {
            menu.LoadMenu(IDR_MENU_CONV_FORMAT_USER);
            pPopup = menu.GetSubMenu(0);
            ASSERT(pPopup);
            CString strName;
            strName.Format(_T("01：%s"), g_userConvFormat[0].strName);
            pPopup->ModifyMenu((UINT)ID_CONV_FORMAT_USER_01, MF_BYCOMMAND, ID_CONV_FORMAT_USER_01, (LPCTSTR)strName);
            strName.Format(_T("02：%s"), g_userConvFormat[1].strName);
            pPopup->ModifyMenu((UINT)ID_CONV_FORMAT_USER_02, MF_BYCOMMAND, ID_CONV_FORMAT_USER_02, (LPCTSTR)strName);
            strName.Format(_T("03：%s"), g_userConvFormat[2].strName);
            pPopup->ModifyMenu((UINT)ID_CONV_FORMAT_USER_03, MF_BYCOMMAND, ID_CONV_FORMAT_USER_03, (LPCTSTR)strName);
            strName.Format(_T("04：%s"), g_userConvFormat[3].strName); /* LastTrain 057 */
            pPopup->ModifyMenu((UINT)ID_CONV_FORMAT_USER_04, MF_BYCOMMAND, ID_CONV_FORMAT_USER_04, (LPCTSTR)strName);
            strName.Format(_T("05：%s"), g_userConvFormat[4].strName); /* LastTrain 057 */
            pPopup->ModifyMenu((UINT)ID_CONV_FORMAT_USER_05, MF_BYCOMMAND, ID_CONV_FORMAT_USER_05, (LPCTSTR)strName);
        }
        break;
    case ID_TEIKEI_TOOL:
        {
            menu.LoadMenu(IDR_MENU_TEIKEI);
            pPopup = menu.GetSubMenu(0);
            ASSERT(pPopup);
            CString strName;
            // グループ１
            strName.Format(_T("01: %s"), shortString(g_teikeiInfo[0].strTeikei, TEIKEI_MENU_STRING_MAX)); /* STEP 035, FreeFall 051 */
            pPopup->ModifyMenu((UINT)ID_TEIKEI_01, MF_BYCOMMAND, ID_TEIKEI_01, (LPCTSTR)strName);
            strName.Format(_T("02: %s"), shortString(g_teikeiInfo[1].strTeikei, TEIKEI_MENU_STRING_MAX)); /* STEP 035, FreeFall 051 */
            pPopup->ModifyMenu((UINT)ID_TEIKEI_02, MF_BYCOMMAND, ID_TEIKEI_02, (LPCTSTR)strName);
            strName.Format(_T("03: %s"), shortString(g_teikeiInfo[2].strTeikei, TEIKEI_MENU_STRING_MAX)); /* STEP 035, FreeFall 051 */
            pPopup->ModifyMenu((UINT)ID_TEIKEI_03, MF_BYCOMMAND, ID_TEIKEI_03, (LPCTSTR)strName);
            strName.Format(_T("04: %s"), shortString(g_teikeiInfo[3].strTeikei, TEIKEI_MENU_STRING_MAX)); /* STEP 035, FreeFall 051 */
            pPopup->ModifyMenu((UINT)ID_TEIKEI_04, MF_BYCOMMAND, ID_TEIKEI_04, (LPCTSTR)strName);
            strName.Format(_T("05: %s"), shortString(g_teikeiInfo[4].strTeikei, TEIKEI_MENU_STRING_MAX)); /* STEP 035, FreeFall 051 */
            pPopup->ModifyMenu((UINT)ID_TEIKEI_05, MF_BYCOMMAND, ID_TEIKEI_05, (LPCTSTR)strName);
            strName.Format(_T("06: %s"), shortString(g_teikeiInfo[5].strTeikei, TEIKEI_MENU_STRING_MAX)); /* STEP 035, FreeFall 051 */
            pPopup->ModifyMenu((UINT)ID_TEIKEI_06, MF_BYCOMMAND, ID_TEIKEI_06, (LPCTSTR)strName);
            strName.Format(_T("07: %s"), shortString(g_teikeiInfo[6].strTeikei, TEIKEI_MENU_STRING_MAX)); /* STEP 035, FreeFall 051 */
            pPopup->ModifyMenu((UINT)ID_TEIKEI_07, MF_BYCOMMAND, ID_TEIKEI_07, (LPCTSTR)strName);
            strName.Format(_T("08: %s"), shortString(g_teikeiInfo[7].strTeikei, TEIKEI_MENU_STRING_MAX)); /* STEP 035, FreeFall 051 */
            pPopup->ModifyMenu((UINT)ID_TEIKEI_08, MF_BYCOMMAND, ID_TEIKEI_08, (LPCTSTR)strName);
            strName.Format(_T("09: %s"), shortString(g_teikeiInfo[8].strTeikei, TEIKEI_MENU_STRING_MAX)); /* STEP 035, FreeFall 051 */
            pPopup->ModifyMenu((UINT)ID_TEIKEI_09, MF_BYCOMMAND, ID_TEIKEI_09, (LPCTSTR)strName);
            strName.Format(_T("10: %s"), shortString(g_teikeiInfo[9].strTeikei, TEIKEI_MENU_STRING_MAX)); /* STEP 035, FreeFall 051 */
            pPopup->ModifyMenu((UINT)ID_TEIKEI_10, MF_BYCOMMAND, ID_TEIKEI_10, (LPCTSTR)strName);
            /* FreeFall 046 */
            // グループ２
            strName.Format(_T("01: %s"), shortString(g_teikeiInfo[10].strTeikei, TEIKEI_MENU_STRING_MAX)); /* STEP 035, FreeFall 051 */
            pPopup->ModifyMenu((UINT)ID_TEIKEI_2_01, MF_BYCOMMAND, ID_TEIKEI_2_01, (LPCTSTR)strName);
            strName.Format(_T("02: %s"), shortString(g_teikeiInfo[11].strTeikei, TEIKEI_MENU_STRING_MAX)); /* STEP 035, FreeFall 051 */
            pPopup->ModifyMenu((UINT)ID_TEIKEI_2_02, MF_BYCOMMAND, ID_TEIKEI_2_02, (LPCTSTR)strName);
            strName.Format(_T("03: %s"), shortString(g_teikeiInfo[12].strTeikei, TEIKEI_MENU_STRING_MAX)); /* STEP 035, FreeFall 051 */
            pPopup->ModifyMenu((UINT)ID_TEIKEI_2_03, MF_BYCOMMAND, ID_TEIKEI_2_03, (LPCTSTR)strName);
            strName.Format(_T("04: %s"), shortString(g_teikeiInfo[13].strTeikei, TEIKEI_MENU_STRING_MAX)); /* STEP 035, FreeFall 051 */
            pPopup->ModifyMenu((UINT)ID_TEIKEI_2_04, MF_BYCOMMAND, ID_TEIKEI_2_04, (LPCTSTR)strName);
            strName.Format(_T("05: %s"), shortString(g_teikeiInfo[14].strTeikei, TEIKEI_MENU_STRING_MAX)); /* STEP 035, FreeFall 051 */
            pPopup->ModifyMenu((UINT)ID_TEIKEI_2_05, MF_BYCOMMAND, ID_TEIKEI_2_05, (LPCTSTR)strName);
            strName.Format(_T("06: %s"), shortString(g_teikeiInfo[15].strTeikei, TEIKEI_MENU_STRING_MAX)); /* STEP 035, FreeFall 051 */
            pPopup->ModifyMenu((UINT)ID_TEIKEI_2_06, MF_BYCOMMAND, ID_TEIKEI_2_06, (LPCTSTR)strName);
            strName.Format(_T("07: %s"), shortString(g_teikeiInfo[16].strTeikei, TEIKEI_MENU_STRING_MAX)); /* STEP 035, FreeFall 051 */
            pPopup->ModifyMenu((UINT)ID_TEIKEI_2_07, MF_BYCOMMAND, ID_TEIKEI_2_07, (LPCTSTR)strName);
            strName.Format(_T("08: %s"), shortString(g_teikeiInfo[17].strTeikei, TEIKEI_MENU_STRING_MAX)); /* STEP 035, FreeFall 051 */
            pPopup->ModifyMenu((UINT)ID_TEIKEI_2_08, MF_BYCOMMAND, ID_TEIKEI_2_08, (LPCTSTR)strName);
            strName.Format(_T("09: %s"), shortString(g_teikeiInfo[18].strTeikei, TEIKEI_MENU_STRING_MAX)); /* STEP 035, FreeFall 051 */
            pPopup->ModifyMenu((UINT)ID_TEIKEI_2_09, MF_BYCOMMAND, ID_TEIKEI_2_09, (LPCTSTR)strName);
            strName.Format(_T("10: %s"), shortString(g_teikeiInfo[19].strTeikei, TEIKEI_MENU_STRING_MAX)); /* STEP 035, FreeFall 051 */
            pPopup->ModifyMenu((UINT)ID_TEIKEI_2_10, MF_BYCOMMAND, ID_TEIKEI_2_10, (LPCTSTR)strName);
            // グループ３
            strName.Format(_T("01: %s"), shortString(g_teikeiInfo[20].strTeikei, TEIKEI_MENU_STRING_MAX)); /* STEP 035, FreeFall 051 */
            pPopup->ModifyMenu((UINT)ID_TEIKEI_3_01, MF_BYCOMMAND, ID_TEIKEI_3_01, (LPCTSTR)strName);
            strName.Format(_T("02: %s"), shortString(g_teikeiInfo[21].strTeikei, TEIKEI_MENU_STRING_MAX)); /* STEP 035, FreeFall 051 */
            pPopup->ModifyMenu((UINT)ID_TEIKEI_3_02, MF_BYCOMMAND, ID_TEIKEI_3_02, (LPCTSTR)strName);
            strName.Format(_T("03: %s"), shortString(g_teikeiInfo[22].strTeikei, TEIKEI_MENU_STRING_MAX)); /* STEP 035, FreeFall 051 */
            pPopup->ModifyMenu((UINT)ID_TEIKEI_3_03, MF_BYCOMMAND, ID_TEIKEI_3_03, (LPCTSTR)strName);
            strName.Format(_T("04: %s"), shortString(g_teikeiInfo[23].strTeikei, TEIKEI_MENU_STRING_MAX)); /* STEP 035, FreeFall 051 */
            pPopup->ModifyMenu((UINT)ID_TEIKEI_3_04, MF_BYCOMMAND, ID_TEIKEI_3_04, (LPCTSTR)strName);
            strName.Format(_T("05: %s"), shortString(g_teikeiInfo[24].strTeikei, TEIKEI_MENU_STRING_MAX)); /* STEP 035, FreeFall 051 */
            pPopup->ModifyMenu((UINT)ID_TEIKEI_3_05, MF_BYCOMMAND, ID_TEIKEI_3_05, (LPCTSTR)strName);
            strName.Format(_T("06: %s"), shortString(g_teikeiInfo[25].strTeikei, TEIKEI_MENU_STRING_MAX)); /* STEP 035, FreeFall 051 */
            pPopup->ModifyMenu((UINT)ID_TEIKEI_3_06, MF_BYCOMMAND, ID_TEIKEI_3_06, (LPCTSTR)strName);
            strName.Format(_T("07: %s"), shortString(g_teikeiInfo[26].strTeikei, TEIKEI_MENU_STRING_MAX)); /* STEP 035, FreeFall 051 */
            pPopup->ModifyMenu((UINT)ID_TEIKEI_3_07, MF_BYCOMMAND, ID_TEIKEI_3_07, (LPCTSTR)strName);
            strName.Format(_T("08: %s"), shortString(g_teikeiInfo[27].strTeikei, TEIKEI_MENU_STRING_MAX)); /* STEP 035, FreeFall 051 */
            pPopup->ModifyMenu((UINT)ID_TEIKEI_3_08, MF_BYCOMMAND, ID_TEIKEI_3_08, (LPCTSTR)strName);
            strName.Format(_T("09: %s"), shortString(g_teikeiInfo[28].strTeikei, TEIKEI_MENU_STRING_MAX)); /* STEP 035, FreeFall 051 */
            pPopup->ModifyMenu((UINT)ID_TEIKEI_3_09, MF_BYCOMMAND, ID_TEIKEI_3_09, (LPCTSTR)strName);
            strName.Format(_T("10: %s"), shortString(g_teikeiInfo[29].strTeikei, TEIKEI_MENU_STRING_MAX)); /* STEP 035, FreeFall 051 */
            pPopup->ModifyMenu((UINT)ID_TEIKEI_3_10, MF_BYCOMMAND, ID_TEIKEI_3_10, (LPCTSTR)strName);
            // グループ名称を反映
            for (int k = 0; k < 3; k++) {
                CMenu* pSubMenu = pPopup->GetSubMenu(0);
                pPopup->RemoveMenu(0, MF_BYPOSITION);
                pPopup->AppendMenu(MF_POPUP , (UINT_PTR)pSubMenu->GetSafeHmenu(), (LPCTSTR)g_strTeikeiGroupName[k]);
            }
        }
        break;
    case ID_OPEN_FOLDER: /* StartInaction 053 */
        {
            menu.LoadMenu(IDR_MAINFRAME);
            pPopup = menu.GetSubMenu(0);
            ASSERT(pPopup);
            int nCount = pPopup->GetMenuItemCount();
            for (int j=0;j<nCount;j++) {
                CString strTitle;
                pPopup->GetMenuString(j, strTitle, MF_BYPOSITION);
                if (strTitle == _T("最近使ったフォルダ(&R)")) {
                    pPopup = pPopup->GetSubMenu(j);
                    break;
                }
            }
        }
        break;
    case ID_FAVORITES_FOLDER: /* RockDance 129 */
        {
            menu.LoadMenu(IDR_MENU_FAVORITE_FOLDER);
            pPopup = menu.GetSubMenu(0);
            ASSERT(pPopup);
            CString strName;
            strName.Format(_T("01：%s"), g_strFavorite[0]);
            pPopup->ModifyMenu((UINT)ID_FAVORITE_FOLDER_01, MF_BYCOMMAND, ID_FAVORITE_FOLDER_01, (LPCTSTR)strName);
            strName.Format(_T("02：%s"), g_strFavorite[1]);
            pPopup->ModifyMenu((UINT)ID_FAVORITE_FOLDER_02, MF_BYCOMMAND, ID_FAVORITE_FOLDER_02, (LPCTSTR)strName);
            strName.Format(_T("03：%s"), g_strFavorite[2]);
            pPopup->ModifyMenu((UINT)ID_FAVORITE_FOLDER_03, MF_BYCOMMAND, ID_FAVORITE_FOLDER_03, (LPCTSTR)strName);
            strName.Format(_T("04：%s"), g_strFavorite[3]);
            pPopup->ModifyMenu((UINT)ID_FAVORITE_FOLDER_04, MF_BYCOMMAND, ID_FAVORITE_FOLDER_04, (LPCTSTR)strName);
            strName.Format(_T("05：%s"), g_strFavorite[4]);
            pPopup->ModifyMenu((UINT)ID_FAVORITE_FOLDER_05, MF_BYCOMMAND, ID_FAVORITE_FOLDER_05, (LPCTSTR)strName);
            strName.Format(_T("06：%s"), g_strFavorite[5]);
            pPopup->ModifyMenu((UINT)ID_FAVORITE_FOLDER_06, MF_BYCOMMAND, ID_FAVORITE_FOLDER_06, (LPCTSTR)strName);
            strName.Format(_T("07：%s"), g_strFavorite[6]);
            pPopup->ModifyMenu((UINT)ID_FAVORITE_FOLDER_07, MF_BYCOMMAND, ID_FAVORITE_FOLDER_07, (LPCTSTR)strName);
            strName.Format(_T("08：%s"), g_strFavorite[7]);
            pPopup->ModifyMenu((UINT)ID_FAVORITE_FOLDER_08, MF_BYCOMMAND, ID_FAVORITE_FOLDER_08, (LPCTSTR)strName);
            strName.Format(_T("09：%s"), g_strFavorite[8]);
            pPopup->ModifyMenu((UINT)ID_FAVORITE_FOLDER_09, MF_BYCOMMAND, ID_FAVORITE_FOLDER_09, (LPCTSTR)strName);
            strName.Format(_T("10：%s"), g_strFavorite[9]);
            pPopup->ModifyMenu((UINT)ID_FAVORITE_FOLDER_10, MF_BYCOMMAND, ID_FAVORITE_FOLDER_10, (LPCTSTR)strName);
        }
        break;
    default:
        return;
    }

    CRect rc;
    pWnd->SendMessage(TB_GETRECT, pnmtb->iItem, (LPARAM)&rc);
    pWnd->ClientToScreen(&rc);

    pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL, rc.left, rc.bottom, this, &rc);
}

void CMainFrame::UpdateToolBar()
{
    m_wndToolBar.UpdateButtonImage();
}

void CMainFrame::OnPopupCustomize() /* WildCherry4 084 */
{
    // TODO: この位置にコマンド ハンドラ用のコードを追加してください
    m_wndToolBar.Customize();
}

void CMainFrame::OnUpdatePopupCustomize(CCmdUI* pCmdUI) /* WildCherry4 084 */
{
    // TODO: この位置に command update UI ハンドラ用のコードを追加してください
    pCmdUI->Enable(TRUE);
}

BOOL CMainFrame::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
 //なぜか ANSI 版だとデフォルトのツールボタンではここが呼ばれないが
 //UNICODE 版だと呼ばれ、これを処理しないとツールチップが表示されない
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
    UINT nID =pNMHDR->idFrom;
    TCHAR szText[512];
    int ret = LoadString(pTTT->hinst, nID, szText, _countof(szText));
    if(ret){//表示するべき文字列は改行以降
        TCHAR *pszText = _tcschr(szText, _T('\n'));
        if(pszText){
            _tcsncpy_s(pTTT->szText, pszText+1, _TRUNCATE);
            pTTT->lpszText = pTTT->szText;
        }
        return true;
    }
    extern bool OnToolTipNotify(UINT nID, LPTSTR& szText);
    if(OnToolTipNotify(nID, pTTT->lpszText)) {
        return(TRUE);
    }
    return(FALSE);
}

void CMainFrame::GetMessageString( UINT nID, CString& rMessage ) const
{
    extern bool GetMessageString(UINT nID, CString& strMessage);
    if(GetMessageString(nID, rMessage)) {
        ;
    } else {
        CFrameWnd::GetMessageString(nID, rMessage);
    }
}
