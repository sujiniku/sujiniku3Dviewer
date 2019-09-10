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
int ya_sentan_Z = 150; //


int sLX = 280; int sLZ=80;
int eLX = 320; int eLZ=80;

int labehaba = eLX- sLX; // 40
int kabeTakasa = 100;

int camX = ya_sentan_X; int camZ = ya_sentan_Z;

int camY = 70; // 視線の高さ


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


			int vecCSX = sLX - camX ; int vecCSZ = sLZ - camZ ; // カメラから起点に向かうベクトルa
			int vecCEX = eLX - camX ; int vecCEZ = eLZ - camZ; // カメラから終点に向かうベクトルb



			int kabeYsita = 0; int kabeYue = kabeYsita + kabeTakasa ; // 高さはY軸にしている。

			int kabeZ = sLZ; // 壁のZ位置は、sLZで代用した。



			int vecCksZ = kabeZ - camZ; int vecCksY = kabeYsita - camY; // カメラから壁下に向かうベクトル
			int vecCkuZ = kabeZ - camZ; int vecCkuY = kabeYue - camY; // カメラから壁下に向かうベクトル


			// Y
			_stprintf_s(henkan, 200, TEXT("カメラY: %d"), camY); // デバッグ用メッセージ 
			TextOut(hdc, 450, 160, henkan, lstrlen(henkan));
			

			_stprintf_s(henkan, 200, TEXT("壁下Y: %d"), kabeYsita); // デバッグ用メッセージ 
			TextOut(hdc, 450, 160+20, henkan, lstrlen(henkan));

			_stprintf_s(henkan, 200, TEXT("カメラ → 壁下Y: %d"), vecCksY); // デバッグ用メッセージ 
			TextOut(hdc, 450, 160+20+20, henkan, lstrlen(henkan));



			// Z
			_stprintf_s(henkan, 200, TEXT("カメラZ: %d"), camZ); // デバッグ用メッセージ 
			TextOut(hdc, 650, 160, henkan, lstrlen(henkan));

			_stprintf_s(henkan, 200, TEXT("壁Z: %d"), kabeZ); // デバッグ用メッセージ 
			TextOut(hdc, 650, 160 + 20, henkan, lstrlen(henkan));

			_stprintf_s(henkan, 200, TEXT("カメラ → 壁Z: %d"), vecCksZ); // デバッグ用メッセージ 
			TextOut(hdc, 650, 160 + 20 + 20, henkan, lstrlen(henkan));








			// int vec_unitX_x = 1; int vec_unitX_y = 0; // x方向（画面で右側）を向いている単位ベクトルの成分


			double naiseki1 = vecCSX * vecCEX + vecCSZ * vecCEZ ;  // 内積a・b
			double naiseki2 = vecCSX * 1 ;  // 内積a・unitX
			double naiseki3 = vecCEX * 1 ;  // 内積 unitX・b


		
			// int vec_unitY_y = 1; int vec_unitY_z = 0; // y方向（画面からユーザーの向き）を向いている単位ベクトルの成分

			double naisekiZY1 = vecCksZ * vecCkuZ + vecCksY * vecCkuY;  // ZY側面の内積a・b
			double naisekiZY2 = vecCksY * 1 ;  // 内積a・unitY
			double naisekiZY3 = vecCkuY * 1 ;  // 内積 unitY・b



			double zettaiZY1 = sqrt(
				(vecCksZ * vecCksZ + vecCksY * vecCksY) * (vecCkuZ * vecCkuZ + vecCkuY * vecCkuY)
			); // 絶対値|a| |b|


	//		_stprintf_s(henkan, 200, TEXT("%d"), (int)zettaiZY1); // デバッグ用メッセージ zettaiZY1 のつもり
	//		TextOut(hdc, 650, 415, henkan, lstrlen(henkan));



			double zettaiZY2 = sqrt(
				(vecCksZ * vecCksZ + vecCksY * vecCksY) * 1
			); // 絶対値|a| |1|


			double zettaiZY3 = sqrt(
				(vecCkuZ * vecCkuZ + vecCkuY * vecCkuY) * 1
			); // 絶対値|b| |1|





			double zettai1 = sqrt (
							(vecCSX * vecCSX + vecCSZ * vecCSZ ) * (vecCEX * vecCEX + vecCEZ * vecCEZ )   
							)  ; // 絶対値|a| |b|




			double zettai2 = sqrt(
				(vecCSX * vecCSX + vecCSZ * vecCSZ) * 1
			); // 絶対値|a| |1|


			double zettai3 = sqrt(
				(vecCEX * vecCEX + vecCEZ * vecCEZ) * 1
			); // 絶対値|b| |1|


			double costhetaZY1 = naisekiZY1 / zettaiZY1;
			double costhetaZY2 = naisekiZY2 / zettaiZY2;
			double costhetaZY3 = naisekiZY3 / zettaiZY3;

			double costheta1 = naiseki1 / zettai1   ;
			double costheta2 = naiseki2 / zettai2;
			double costheta3 = naiseki3 / zettai3;


			float thetaZY1 = (float) acos( costhetaZY1 );
			float thetaZY2 = (float) acos( costhetaZY2 );
			float thetaZY3 = (float) acos( costhetaZY3 );


			float theta1 = (float) acos( costheta1 ) ;
			float theta2 = (float) acos( costheta2 ) ;
			float theta3 = (float) acos( costheta3 ) ;

			float bairitu1 = theta1 / 0.3 ;
			float bairituZY1 = thetaZY1 / 0.3;


			double Pi = 3.141 ;

			double bairitu2 = (theta2 - Pi / 2) / 0.1;
			double bairitu3 = (theta3 - Pi / 2) / 0.1 ;

			double bairituZY2 = (thetaZY2 - Pi / 2) / 0.1;
			double bairituZY3 = (thetaZY3 - Pi / 2) / 0.1;



			
		//	_stprintf_s(henkan, 200, TEXT("%d"), 100*theta); // デバッグ用メッセージ
		//	TextOut(hdc, 400, 400, henkan, lstrlen(henkan));


				// X軸の拡大率の計算デバッグ用

			int debugMx1 = 300 ; int debugMy = 300;

			_stprintf_s(henkan, 200, TEXT("内積: %d"), (int)naiseki1); // デバッグ用メッセージ 内積のつもり
			TextOut(hdc, debugMx1, debugMy, henkan, lstrlen(henkan));

			_stprintf_s(henkan, 200, TEXT("絶対値: %d"), (int)zettai1); // デバッグ用メッセージ 絶対値のつもり
			TextOut(hdc, debugMx1, debugMy + 30, henkan, lstrlen(henkan));

			int bufseisuu = (int)100 * costheta2;

			_stprintf_s(henkan, 200, TEXT("100 cosθ2: %d"), bufseisuu); // デバッグ用メッセージ cosθのつもり
			TextOut(hdc, debugMx1,  debugMy + 60, henkan, lstrlen(henkan));


			bufseisuu = (int)100 * theta2;
			_stprintf_s(henkan, 200, TEXT("100 θ2: %d"), (int)bufseisuu); // デバッグ用メッセージ 角度θのつもり
			TextOut(hdc, debugMx1, debugMy + 90, henkan, lstrlen(henkan));

			bufseisuu = (int)100 * bairitu2;
			_stprintf_s(henkan, 200, TEXT("倍率2: %d"), (int)bufseisuu); // デバッグ用メッセージ 角度θのつもり
			TextOut(hdc, debugMx1, debugMy + 120, henkan, lstrlen(henkan));




			// Y軸の拡大率の計算デバッグ用
			int debugMx2 = 600;

			_stprintf_s(henkan, 200, TEXT("内積: %d"), (int)naisekiZY1); // デバッグ用メッセージ 内積のつもり
			TextOut(hdc, debugMx2, debugMy, henkan, lstrlen(henkan));

			_stprintf_s(henkan, 200, TEXT("絶対値: %d"), (int)zettaiZY1); // デバッグ用メッセージ 絶対値のつもり
			TextOut(hdc, debugMx2, debugMy +30 , henkan, lstrlen(henkan));

			bufseisuu = (int)100 * costhetaZY2;

			_stprintf_s(henkan, 200, TEXT("100 cosθ2: %d"), bufseisuu); // デバッグ用メッセージ cosθのつもり
			TextOut(hdc, debugMx2, debugMy +60, henkan, lstrlen(henkan));


			bufseisuu = (int)100 * thetaZY2;
			_stprintf_s(henkan, 200, TEXT("100 θ2: %d"), (int)bufseisuu); // デバッグ用メッセージ 角度θのつもり
			TextOut(hdc, debugMx2, debugMy + 90, henkan, lstrlen(henkan));

			bufseisuu = (int)100 * bairituZY2;
			_stprintf_s(henkan, 200, TEXT("倍率2: %d"), (int)bufseisuu); // デバッグ用メッセージ 角度θのつもり
			TextOut(hdc, debugMx2, debugMy + 120, henkan, lstrlen(henkan));



			// 視界

			int kuroXkiten = 20 ;
			int kutoYkiten = 70 ;

			int kuroXhaba = 150 ;
			int kutoYhaba = 100 ;


			HBRUSH brasi_buhin_1;
			brasi_buhin_1 = CreateSolidBrush(RGB(0, 0, 0)); // 黒色のブラシを作成。背景用。
			SelectObject(hdc, brasi_buhin_1); // ウィンドウhdcと、さきほど作成したブラシを関連づけ
			Rectangle(hdc, kuroXkiten, kutoYkiten, kuroXkiten + kuroXhaba, kutoYkiten + kutoYhaba); // 図形の描画


			int kuroXtyuo = kuroXkiten + ( kuroXhaba / 2) ;
			int kuroYtyuo = kutoYkiten + ( kutoYhaba / 2) ;


			int tyousei = 3; // 単に、ピンク壁の初期位置での、視界での大きさを調整するための係数。

			HBRUSH brasi_buhin_2;
			brasi_buhin_2 = CreateSolidBrush(RGB(255, 100, 100)); // 壁の表示用のピンク色のブラシを作成
			SelectObject(hdc, brasi_buhin_2); // ウィンドウhdcと、さきほど作成したブラシを関連づけ
			// Rectangle(hdc, ((20+170)/2) - 20 * bairitu1 -zure_X, 50 +40, ((20 + 170) / 2) + 20 * bairitu1 - zure_X, 100 +40 ); // 図形の描画
			Rectangle(hdc,
				kuroXtyuo - tyousei * bairitu2,
				kuroYtyuo - tyousei * bairituZY2,
				kuroXtyuo - tyousei * bairitu3 ,
				kuroYtyuo - tyousei * bairituZY3 ); // 基準の状態


			lstrcpy(mojibuf, TEXT("視界"));
			TextOut(hdc, 80, 30, mojibuf, lstrlen(mojibuf));




			//上面図

			//被写体の上面図
			

		//	MoveToEx(hdc, 300 + 20, 60, NULL);
	//	LineTo(hdc, 300 + 20, 80);
			
			
			MoveToEx(hdc, sLX, sLZ, NULL);
			LineTo(hdc, eLX, eLZ);



			//カメラの位置と向き。矢印の先端を位置の基準とする。

			MoveToEx(hdc, ya_sentan_X , ya_sentan_Z, NULL);
			LineTo(hdc, ya_sentan_X , ya_sentan_Z +100);

			MoveToEx(hdc, ya_sentan_X , ya_sentan_Z, NULL);
			LineTo(hdc, ya_sentan_X  +20, ya_sentan_Z + 20);



			MoveToEx(hdc, ya_sentan_X , ya_sentan_Z, NULL);
			LineTo(hdc, ya_sentan_X - 20, ya_sentan_Z + 20);


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
			//bairitu0 = bairitu0 + 0.1;
			ya_sentan_Z = ya_sentan_Z -5;
			camZ = ya_sentan_Z;

			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);

			break;
		}


		case VK_DOWN:
		{
			//bairitu0 = bairitu0 - 0.1;
			ya_sentan_Z = ya_sentan_Z + 5;
			camZ = ya_sentan_Z;

			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);

			break;
		}

		case VK_RIGHT:
		{
			//zure_X = zure_X + 5;

			ya_sentan_X = ya_sentan_X + 5;

			camX = ya_sentan_X;



			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);

			break;
		}

		case VK_LEFT:
		{
			//zure_X = zure_X - 5;

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
