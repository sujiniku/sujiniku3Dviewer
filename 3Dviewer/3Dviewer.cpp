﻿// 3Dviewer.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "3Dviewer.h"

#include <math.h>

#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名


int camX = 300;
int camZ = 150;


int arrow_Head_X = camX; // 矢印の先端のX座標
int arrow_Head_Z = camZ; //


int arrow_Head_X0rot = camX; // 回転に入る前の位置保存。ケタ落ちの誤差対策のため。矢印の先端のX座標
int arrow_Head_Z0rot = camZ;


double arrow_Head_Xdelta = (double)arrow_Head_X;
double arrow_Head_Zdelta = (double)arrow_Head_Z;


int arrow_Bottom_X = camX ;
int arrow_Bottom_Z = camZ + 100;

double arrow_Bottom_Xdelta = (double) arrow_Bottom_X;
double arrow_Bottom_Zdelta = (double) arrow_Bottom_Z;


int arrow_rot_centerX = arrow_Bottom_X;
int arrow_rot_centerZ = arrow_Bottom_Z;


int sLX = 280; int sLZ=80;
int eLX = 320; int eLZ=80;

int wallWidth = eLX- sLX; // 40
int wallHeight = 100;



int rotXcenter = camX + 10;
int rotZcenter = camZ + 10;

int camY = 70; // 視線の高さ


double camXdelta =(double)camX;
double camYtemp =  (double)camY;
double camZdelta = (double)camY;

int zure_X = 0; // 初期位置からの、矢印の先端のX座標の差分
int zure_Y = 0; //

float magnification0 = 1.0;


double angleAccumulation = 0.05;
int angleCoefficient = 1;


TCHAR WordBuffer[100] = TEXT("テスト");



enum move_what
{
	moveCamera,
	moveModel,
};

enum move_what now_movewhat = moveCamera ;


enum move_type
{
	moveParallel,
	moveRotate,
};

enum move_type now_movetype = moveParallel ;


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

			static TCHAR convertStringBuffer[50]; // 文字列を格納するための変数 convertStringBuffer を準備


			int vecCSX = sLX - camX ; int vecCSZ = sLZ - camZ ; // カメラから起点に向かうベクトルa
			int vecCEX = eLX - camX ; int vecCEZ = eLZ - camZ; // カメラから終点に向かうベクトルb



			int wallYunder = 0; int wallYtop = wallYunder + wallHeight ; // 高さはY軸にしている。

			int wallZ = sLZ; // 壁のZ位置は、sLZで代用した。



			int vecCksZ = wallZ - camZ; int vecCksY = wallYunder - camY; // カメラから壁下に向かうベクトル
			int vecCkuZ = wallZ - camZ; int vecCkuY = wallYtop - camY; // カメラから壁下に向かうベクトル


			// Y
			_stprintf_s(convertStringBuffer, 200, TEXT("カメラY: %d"), camY); // デバッグ用メッセージ 
			TextOut(hdc, 450, 160, convertStringBuffer, lstrlen(convertStringBuffer));
			

			_stprintf_s(convertStringBuffer, 200, TEXT("壁下Y: %d"), wallYunder); // デバッグ用メッセージ 
			TextOut(hdc, 450, 160+20, convertStringBuffer, lstrlen(convertStringBuffer));

			_stprintf_s(convertStringBuffer, 200, TEXT("カメラ → 壁下Y: %d"), vecCksY); // デバッグ用メッセージ 
			TextOut(hdc, 450, 160+20+20, convertStringBuffer, lstrlen(convertStringBuffer));



			// Z
			_stprintf_s(convertStringBuffer, 200, TEXT("カメラZ: %d"), camZ); // デバッグ用メッセージ 
			TextOut(hdc, 650, 160, convertStringBuffer, lstrlen(convertStringBuffer));

			_stprintf_s(convertStringBuffer, 200, TEXT("壁Z: %d"), wallZ); // デバッグ用メッセージ 
			TextOut(hdc, 650, 160 + 20, convertStringBuffer, lstrlen(convertStringBuffer));

			_stprintf_s(convertStringBuffer, 200, TEXT("カメラ → 壁Z: %d"), vecCksZ); // デバッグ用メッセージ 
			TextOut(hdc, 650, 160 + 20 + 20, convertStringBuffer, lstrlen(convertStringBuffer));








			// int vec_unitX_x = 1; int vec_unitX_y = 0; // x方向（画面で右側）を向いている単位ベクトルの成分


			double naiseki1 = vecCSX * vecCEX + vecCSZ * vecCEZ ;  // 内積a・b
			double naiseki2 = vecCSX * 1 ;  // 内積a・unitX
			double naiseki3 = vecCEX * 1 ;  // 内積 unitX・b


		
			// int vec_unitY_y = 1; int vec_unitY_z = 0; // y方向（画面からユーザーの向き）を向いている単位ベクトルの成分

			double naisekiZY1 = vecCksZ * vecCkuZ + vecCksY * vecCkuY;  // ZY側面の内積a・b
			double naisekiZY2 = vecCksY * 1 ;  // 内積a・unitY
			double naisekiZY3 = vecCkuY * 1 ;  // 内積 unitY・b



			double absoluteZY1 = sqrt(
				(vecCksZ * vecCksZ + vecCksY * vecCksY) * (vecCkuZ * vecCkuZ + vecCkuY * vecCkuY)
			); // 絶対値|a| |b|


	//		_stprintf_s(convertStringBuffer, 200, TEXT("%d"), (int)absoluteZY1); // デバッグ用メッセージ absoluteZY1 のつもり
	//		TextOut(hdc, 650, 415, convertStringBuffer, lstrlen(convertStringBuffer));



			double absoluteZY2 = sqrt(
				(vecCksZ * vecCksZ + vecCksY * vecCksY) * 1
			); // 絶対値|a| |1|


			double absoluteZY3 = sqrt(
				(vecCkuZ * vecCkuZ + vecCkuY * vecCkuY) * 1
			); // 絶対値|b| |1|





			double absolute1 = sqrt (
							(vecCSX * vecCSX + vecCSZ * vecCSZ ) * (vecCEX * vecCEX + vecCEZ * vecCEZ )   
							)  ; // 絶対値|a| |b|




			double absolute2 = sqrt(
				(vecCSX * vecCSX + vecCSZ * vecCSZ) * 1
			); // 絶対値|a| |1|


			double absolute3 = sqrt(
				(vecCEX * vecCEX + vecCEZ * vecCEZ) * 1
			); // 絶対値|b| |1|


			double costhetaZY1 = naisekiZY1 / absoluteZY1;
			double costhetaZY2 = naisekiZY2 / absoluteZY2;
			double costhetaZY3 = naisekiZY3 / absoluteZY3;

			double costheta1 = naiseki1 / absolute1   ;
			double costheta2 = naiseki2 / absolute2;
			double costheta3 = naiseki3 / absolute3;


			float thetaZY1 = (float) acos( costhetaZY1 );
			float thetaZY2 = (float) acos( costhetaZY2 );
			float thetaZY3 = (float) acos( costhetaZY3 );


			float theta1 = (float) acos( costheta1 ) ;
			float theta2 = (float) acos( costheta2 ) ;
			float theta3 = (float) acos( costheta3 ) ;

			float magnification1 = theta1 / 0.3 ;
			float magnificationZY1 = thetaZY1 / 0.3;


			double Pi = 3.141 ;

			double magnification2 = (theta2 - Pi / 2) / 0.1;
			double magnification3 = (theta3 - Pi / 2) / 0.1 ;

			double magnificationZY2 = (thetaZY2 - Pi / 2) / 0.1;
			double magnificationZY3 = (thetaZY3 - Pi / 2) / 0.1;






now_movewhat = moveCamera ;

			// 移動対象の明記のためのメッセージ表示

			int NowModePx = 500; int NowModePy = 20 ;


			if (now_movewhat == moveCamera) 			{
				lstrcpy(WordBuffer, TEXT("カメラ移動モード"));
			}

			if (now_movewhat == moveModel) {
				lstrcpy(WordBuffer, TEXT("被写体 移動モード"));
			}

			_stprintf_s(convertStringBuffer, 200, TEXT("現在は %s です。"), WordBuffer); // 現在のモード表示
			TextOut(hdc, NowModePx, NowModePy, convertStringBuffer, lstrlen(convertStringBuffer));


			if (now_movetype == moveParallel) {
				_stprintf_s(convertStringBuffer, 200, TEXT("平行移動の指定中です。"), WordBuffer); // 現在の移動タイプ表示
				TextOut(hdc, NowModePx, NowModePy + 25, convertStringBuffer, lstrlen(convertStringBuffer));	

				_stprintf_s(convertStringBuffer, 200, TEXT("Rボタンで回転移動に切りかえます。※ 未実装 "), WordBuffer);
				TextOut(hdc, NowModePx, NowModePy + 50, convertStringBuffer, lstrlen(convertStringBuffer));

			}

			if (now_movetype == moveRotate) {
				_stprintf_s(convertStringBuffer, 200, TEXT("回転移動の指定中です。"), WordBuffer); // 現在の移動タイプ表示
				TextOut(hdc, NowModePx, NowModePy + 25, convertStringBuffer, lstrlen(convertStringBuffer));

				_stprintf_s(convertStringBuffer, 200, TEXT("Pボタンで平行移動に切りかえます。"), WordBuffer); 
				TextOut(hdc, NowModePx, NowModePy + 50, convertStringBuffer, lstrlen(convertStringBuffer));

			}


			
		//	_stprintf_s(convertStringBuffer, 200, TEXT("%d"), 100*theta); // デバッグ用メッセージ
		//	TextOut(hdc, 400, 400, convertStringBuffer, lstrlen(convertStringBuffer));


				// X軸の拡大率の計算デバッグ用

			int debugMx1 = 300 ; int debugMy = 300;

			_stprintf_s(convertStringBuffer, 200, TEXT("内積: %d"), (int)naiseki1); // デバッグ用メッセージ 内積のつもり
			TextOut(hdc, debugMx1, debugMy, convertStringBuffer, lstrlen(convertStringBuffer));

			_stprintf_s(convertStringBuffer, 200, TEXT("絶対値: %d"), (int)absolute1); // デバッグ用メッセージ 絶対値のつもり
			TextOut(hdc, debugMx1, debugMy + 30, convertStringBuffer, lstrlen(convertStringBuffer));

			int bufseisuu = (int)100 * costheta2;

			_stprintf_s(convertStringBuffer, 200, TEXT("100 cosθ2: %d"), bufseisuu); // デバッグ用メッセージ cosθのつもり
			TextOut(hdc, debugMx1,  debugMy + 60, convertStringBuffer, lstrlen(convertStringBuffer));


			bufseisuu = (int)100 * theta2;
			_stprintf_s(convertStringBuffer, 200, TEXT("100 θ2: %d"), (int)bufseisuu); // デバッグ用メッセージ 角度θのつもり
			TextOut(hdc, debugMx1, debugMy + 90, convertStringBuffer, lstrlen(convertStringBuffer));

			bufseisuu = (int)100 * magnification2;
			_stprintf_s(convertStringBuffer, 200, TEXT("倍率2: %d"), (int)bufseisuu); // デバッグ用メッセージ 角度θのつもり
			TextOut(hdc, debugMx1, debugMy + 120, convertStringBuffer, lstrlen(convertStringBuffer));




			// Y軸の拡大率の計算デバッグ用
			int debugMx2 = 600;

			_stprintf_s(convertStringBuffer, 200, TEXT("内積: %d"), (int)naisekiZY1); // デバッグ用メッセージ 内積のつもり
			TextOut(hdc, debugMx2, debugMy, convertStringBuffer, lstrlen(convertStringBuffer));

			_stprintf_s(convertStringBuffer, 200, TEXT("絶対値: %d"), (int)absoluteZY1); // デバッグ用メッセージ 絶対値のつもり
			TextOut(hdc, debugMx2, debugMy +30 , convertStringBuffer, lstrlen(convertStringBuffer));

			bufseisuu = (int)100 * costhetaZY2;

			_stprintf_s(convertStringBuffer, 200, TEXT("100 cosθ2: %d"), bufseisuu); // デバッグ用メッセージ cosθのつもり
			TextOut(hdc, debugMx2, debugMy +60, convertStringBuffer, lstrlen(convertStringBuffer));


			bufseisuu = (int)100 * thetaZY2;
			_stprintf_s(convertStringBuffer, 200, TEXT("100 θ2: %d"), (int)bufseisuu); // デバッグ用メッセージ 角度θのつもり
			TextOut(hdc, debugMx2, debugMy + 90, convertStringBuffer, lstrlen(convertStringBuffer));

			bufseisuu = (int)100 * magnificationZY2;
			_stprintf_s(convertStringBuffer, 200, TEXT("倍率2: %d"), (int)bufseisuu); // デバッグ用メッセージ 角度θのつもり
			TextOut(hdc, debugMx2, debugMy + 120, convertStringBuffer, lstrlen(convertStringBuffer));



			// 視界

			int blackXstartPoint = 20 ;
			int blackYstartPoint = 70 ;

			int blackXWidth = 150 ;
			int blackYWidth = 100 ;


			HBRUSH brasi_buhin_1;
			brasi_buhin_1 = CreateSolidBrush(RGB(0, 0, 0)); // 黒色のブラシを作成。背景用。
			SelectObject(hdc, brasi_buhin_1); // ウィンドウhdcと、さきほど作成したブラシを関連づけ
			Rectangle(hdc, blackXstartPoint, blackYstartPoint, blackXstartPoint + blackXWidth, blackYstartPoint + blackYWidth); // 図形の描画


			int blackXcentral = blackXstartPoint + ( blackXWidth / 2) ;
			int blackYcentral = blackYstartPoint + ( blackYWidth / 2) ;


			int tyousei = 3; // 単に、ピンク壁の初期位置での、視界での大きさを調整するための係数。

			HBRUSH brasi_buhin_2;
			brasi_buhin_2 = CreateSolidBrush(RGB(255, 100, 100)); // 壁の表示用のピンク色のブラシを作成
			SelectObject(hdc, brasi_buhin_2); // ウィンドウhdcと、さきほど作成したブラシを関連づけ
			// Rectangle(hdc, ((20+170)/2) - 20 * magnification1 -zure_X, 50 +40, ((20 + 170) / 2) + 20 * magnification1 - zure_X, 100 +40 ); // 図形の描画
			Rectangle(hdc,
				blackXcentral - tyousei * magnification2,
				blackYcentral - tyousei * magnificationZY2,
				blackXcentral - tyousei * magnification3 ,
				blackYcentral - tyousei * magnificationZY3 ); // 基準の状態


			lstrcpy(WordBuffer, TEXT("視界"));
			TextOut(hdc, 80, 30, WordBuffer, lstrlen(WordBuffer));




			//上面図

			//被写体の上面図
			

		//	MoveToEx(hdc, 300 + 20, 60, NULL);
	//	LineTo(hdc, 300 + 20, 80);
			
			
			MoveToEx(hdc, sLX, sLZ, NULL);
			LineTo(hdc, eLX, eLZ);



			//カメラの位置と向き。矢印の先端を位置の基準とする。

			MoveToEx(hdc, arrow_Head_X , arrow_Head_Z, NULL);
			LineTo(hdc, arrow_Bottom_X , arrow_Bottom_Z );

			MoveToEx(hdc, arrow_Head_X , arrow_Head_Z, NULL);
			LineTo(hdc, arrow_Head_X  +20, arrow_Head_Z + 20);



			MoveToEx(hdc, arrow_Head_X , arrow_Head_Z, NULL);
			LineTo(hdc, arrow_Head_X - 20, arrow_Head_Z + 20);


			lstrcpy(WordBuffer, TEXT("上面図"));
			TextOut(hdc, 280, 30, WordBuffer, lstrlen(WordBuffer));



            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;


	case WM_KEYDOWN:
		
		if (now_movewhat == moveCamera ) {
			
			if (now_movetype == moveParallel) {

				switch (wParam) {
				case VK_UP:
				{
					arrow_Head_Z = arrow_Head_Z - 5;
					arrow_Bottom_Z = arrow_Bottom_Z - 5;

					camZ = arrow_Head_Z;

					InvalidateRect(hWnd, NULL, TRUE);
					UpdateWindow(hWnd);

					break;
				}


				case VK_DOWN:
				{
					arrow_Head_Z = arrow_Head_Z + 5;
					arrow_Bottom_Z = arrow_Bottom_Z + 5;

					camZ = arrow_Head_Z;

					InvalidateRect(hWnd, NULL, TRUE);
					UpdateWindow(hWnd);

					break;
				}

				case VK_RIGHT:
				{
					arrow_Head_X = arrow_Head_X + 5;
					arrow_Bottom_X = arrow_Bottom_X + 5;

					camX = arrow_Head_X;

					InvalidateRect(hWnd, NULL, TRUE);
					UpdateWindow(hWnd);

					break;
				}

				case VK_LEFT:
				{
					arrow_Head_X = arrow_Head_X - 5;
					arrow_Bottom_X = arrow_Bottom_X - 5;

					camX = arrow_Head_X;

					InvalidateRect(hWnd, NULL, TRUE);
					UpdateWindow(hWnd);

					break;
				}

				case 'R':
				{
					now_movetype = moveRotate;

					InvalidateRect(hWnd, NULL, TRUE);
					UpdateWindow(hWnd);


					arrow_Head_X0rot = arrow_Head_X;
					arrow_Head_Z0rot = arrow_Head_Z;



					break;
				}
				break;

				}
			}

			if (now_movetype == moveRotate) {

				arrow_rot_centerX = arrow_Bottom_X ;
				arrow_rot_centerZ = arrow_Bottom_Z ;
				
				switch (wParam)
				{


				case VK_RIGHT:
				{
					// arrow_Head_X = arrow_Head_X + 5;


					double angleStep = +0.05;

					angleCoefficient = angleCoefficient + 1;			
					angleAccumulation = angleCoefficient * angleStep;


					arrow_Head_Xdelta = cos(angleAccumulation) * (arrow_Head_X0rot - arrow_rot_centerX) + (-1) * sin(angleAccumulation) * (arrow_Head_Z0rot - arrow_rot_centerZ);
					arrow_Head_Zdelta = sin(angleAccumulation) * (arrow_Head_X0rot - arrow_rot_centerX) + cos(angleAccumulation) * (arrow_Head_Z0rot - arrow_rot_centerZ);

					arrow_Head_X = (int) (arrow_rot_centerX + arrow_Head_Xdelta) ;
					arrow_Head_Z = (int) (arrow_rot_centerZ + arrow_Head_Zdelta) ;

					camX = arrow_Head_X;
					camZ = arrow_Head_Z;

					InvalidateRect(hWnd, NULL, TRUE);
					UpdateWindow(hWnd);

					break;
				}

				case VK_LEFT:
				{

					double angleStep = 0.05;

					angleCoefficient = angleCoefficient - 1;
					angleAccumulation = angleCoefficient * angleStep;


					arrow_Head_Xdelta = cos(angleAccumulation) * (arrow_Head_X0rot - arrow_rot_centerX) + (-1) * sin(angleAccumulation) * (arrow_Head_Z0rot - arrow_rot_centerZ);
					arrow_Head_Zdelta = sin(angleAccumulation) * (arrow_Head_X0rot - arrow_rot_centerX) + cos(angleAccumulation) * (arrow_Head_Z0rot - arrow_rot_centerZ);

					arrow_Head_X = (int)(arrow_rot_centerX + arrow_Head_Xdelta);
					arrow_Head_Z = (int)(arrow_rot_centerZ + arrow_Head_Zdelta);

					camX = arrow_Head_X;
					camZ = arrow_Head_Z;

					InvalidateRect(hWnd, NULL, TRUE);
					UpdateWindow(hWnd);

					break;
				}





				case 'P':
				{
					now_movetype = moveParallel;

					InvalidateRect(hWnd, NULL, TRUE);
					UpdateWindow(hWnd);

					break;
				}
				break;
				}

			}
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
