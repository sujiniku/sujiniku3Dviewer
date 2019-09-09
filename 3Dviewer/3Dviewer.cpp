// 3Dviewer.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "3Dviewer.h"

#include <math.h>

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名





int ya_sentan_X = 300; // 矢印の先端のX座標
int ya_sentan_Y = 150; //


int sLX = 280; int sLY=80;
int eLX = 320; int eLY=80;

int camX = ya_sentan_X; int camY = ya_sentan_Y;



int zure_X = 0; // 初期位置からの、矢印の先端のX座標の差分
int zure_Y = 0; //

float bairitu0 = 1.0;

TCHAR mojibuf[100] = TEXT("テスト");


// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY3DVIEWER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY3DVIEWER));

    MSG msg;

    // メイン メッセージ ループ:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY3DVIEWER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY3DVIEWER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 選択されたメニューの解析:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: HDC を使用する描画コードをここに追加してください...

			static TCHAR henkan[50]; // 文字列を格納するための変数 henkan を準備


			int vecCSX = sLX - camX ; int vecCSY = sLY - camY ; // カメラから起点に向かうベクトルa
			int vecCEX = eLX - camX ; int vecCEY = eLY - camY; // カメラから終点に向かうベクトルb

			
			int vec_unitX = 1; int vec_unitY = 0; // 単位ベクトル x方向


			double naiseki1 = vecCSX * vecCEX + vecCSY * vecCEY ;  // 内積a・b

			double naiseki2 = vecCSX * 1 + vecCSY * 0;  // 内積a・unitX

			double naiseki3 = vecCEX * 1 + vecCEY * 0;  // 内積 unitX・b






			double zettai1 = sqrt (
							(vecCSX * vecCSX + vecCSY * vecCSY ) * (vecCEX * vecCEX + vecCEY * vecCEY )   
							)  ; // 絶対値|a| |b|

			double zettai2 = sqrt(
				(vecCSX * vecCSX + vecCSY * vecCSY) * 1
			); // 絶対値|a| |1|


			double zettai3 = sqrt(
				(vecCEX * vecCEX + vecCEY * vecCEY) * 1
			); // 絶対値|a| |1|


			double costheta1 = naiseki1 / zettai1   ;


			double costheta2 = naiseki2 / zettai2;

			double costheta3 = naiseki3 / zettai3;



			_stprintf_s(henkan, 200, TEXT("%d"), (int)naiseki1 ) ; // デバッグ用メッセージ 内積のつもり
			TextOut(hdc, 400, 400, henkan, lstrlen(henkan)) ;

			_stprintf_s(henkan, 200, TEXT("%d"), (int)zettai1); // デバッグ用メッセージ 絶対値のつもり
			TextOut(hdc, 400, 430, henkan, lstrlen(henkan));

			int bufseisuu = (int) 100 * costheta1 ;

			_stprintf_s(henkan, 200, TEXT("%d"), bufseisuu); // デバッグ用メッセージ cosθのつもり
			TextOut(hdc, 450, 415, henkan, lstrlen(henkan));

			// aaaaaaa; // エラー文の表示用

			float theta1 = (float) acos( naiseki1 / zettai1 ) ;
			float theta2 = (float) acos( naiseki2 / zettai2 ) ;
			float theta3 = (float) acos( naiseki3 / zettai3 ) ;

			float bairitu1 = theta1 / 0.3 ;


			double Pi = 3.141 ;

			double bairitu2 = (theta2 - Pi / 2) / 0.1;
			double bairitu3 = (theta3 - Pi / 2) / 0.1 ;





			bufseisuu = (int)100 * theta1;
			_stprintf_s(henkan, 200, TEXT("%d"), (int)bufseisuu); // デバッグ用メッセージ 角度θのつもり
			TextOut(hdc, 550, 415, henkan, lstrlen(henkan));



			
		//	_stprintf_s(henkan, 200, TEXT("%d"), 100*theta); // デバッグ用メッセージ
		//	TextOut(hdc, 400, 400, henkan, lstrlen(henkan));



			// 視界

			HBRUSH brasi_buhin_1;
			brasi_buhin_1 = CreateSolidBrush(RGB(0, 0, 0)); // 黒色のブラシを作成。背景用。
			SelectObject(hdc, brasi_buhin_1); // ウィンドウhdcと、さきほど作成したブラシを関連づけ
			Rectangle(hdc, 20, 70, 20+150, 70 +100); // 図形の描画

			HBRUSH brasi_buhin_2;
			brasi_buhin_2 = CreateSolidBrush(RGB(255, 100, 100)); // ピンク色のブラシを作成
			SelectObject(hdc, brasi_buhin_2); // ウィンドウhdcと、さきほど作成したブラシを関連づけ
			// Rectangle(hdc, ((20+170)/2) - 20 * bairitu1 -zure_X, 50 +40, ((20 + 170) / 2) + 20 * bairitu1 - zure_X, 100 +40 ); // 図形の描画
			Rectangle(hdc, ((20 + 170) / 2) - 10 * bairitu2, 120 - 15 * sqrt(bairitu1), ((20 + 170) / 2) - 10 * bairitu3 , 120 + 15 * sqrt(bairitu1) ); // 基準の状態


			lstrcpy(mojibuf, TEXT("視界"));
			TextOut(hdc, 80, 30, mojibuf, lstrlen(mojibuf));




			//上面図

			//被写体の上面図
			

			MoveToEx(hdc, 300 + 20, 60, NULL);
			LineTo(hdc, 300 + 20, 80);
			
			
			MoveToEx(hdc, sLX, sLY, NULL);
			LineTo(hdc, eLX, eLY);



			//カメラの位置と向き。矢印の先端を位置の基準とする。

			MoveToEx(hdc, ya_sentan_X , ya_sentan_Y, NULL);
			LineTo(hdc, ya_sentan_X , ya_sentan_Y +100);

			MoveToEx(hdc, ya_sentan_X , ya_sentan_Y, NULL);
			LineTo(hdc, ya_sentan_X  +20, ya_sentan_Y + 20);



			MoveToEx(hdc, ya_sentan_X , ya_sentan_Y, NULL);
			LineTo(hdc, ya_sentan_X - 20, ya_sentan_Y + 20);


			lstrcpy(mojibuf, TEXT("上面図"));
			TextOut(hdc, 280, 30, mojibuf, lstrlen(mojibuf));



            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;


	case WM_KEYDOWN:

		switch (wParam)		{
		case VK_UP:
		{
			bairitu0 = bairitu0 + 0.1;
			ya_sentan_Y = ya_sentan_Y -5;
			camY = ya_sentan_Y;

			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);

			break;
		}


		case VK_DOWN:
		{
			bairitu0 = bairitu0 - 0.1;
			ya_sentan_Y = ya_sentan_Y + 5;
			camY = ya_sentan_Y;

			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);

			break;
		}

		case VK_RIGHT:
		{
			zure_X = zure_X + 5;

			ya_sentan_X = ya_sentan_X + 5;

			camX = ya_sentan_X;



			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);

			break;
		}

		case VK_LEFT:
		{
			zure_X = zure_X - 5;

			ya_sentan_X = ya_sentan_X - 5;

			camX = ya_sentan_X;


			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);

			break;
		}

		break;
		}

	break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
