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

int ParallelDefferX = 0;
int ParallelDefferZ = 0;

double  angleStep = +0.05;

int skipRotFlag = 0;

double rotArrowSideRightDefferX = 0;
double rotArrowSideRightDefferZ = 0;

double rotArrowSideLeftDefferX = 0;
double rotArrowSideLeftDefferZ = 0;

double rotBufferXinput1=0;
double rotBufferXoutput1 = 0;
double rotBufferX3 = 0;


double rotBufferZinput1 = 0;
double rotBufferZoutput1 = 0;
double rotBufferZ3 = 0;



int camX = 300;
int camZ = 150;

int arrow_center_X = camX; // 矢印の中心
int arrow_center_Z = camZ; //

int arrow_length = 100;

int arrow_Head_X = arrow_center_X; // 矢印の先端のX座標
int arrow_Head_Z = arrow_center_Z - (arrow_length /2) ; //

int arrow_Head_LeftX = arrow_Head_X - 20;
int arrow_Head_LeftZ = arrow_Head_Z + 20;

int arrow_Head_RightX = arrow_Head_X + 20;
int arrow_Head_RightZ = arrow_Head_Z + 20;

int arrow_Head_RightXrot0 = 0;
int arrow_Head_RightZrot0 = 0;

int arrow_Head_LeftXrot0 = 0;
int arrow_Head_LeftZrot0 = 0;


int arrow_Head_Xrot0 = arrow_Head_X; // 回転に入る前の位置保存。ケタ落ちの誤差対策のため。矢印の先端のX座標
int arrow_Head_Zrot0 = arrow_Head_Z;


double arrow_Head_Xdelta = 0;
double arrow_Head_Zdelta = 0;


int arrow_Bottom_X = arrow_center_X;
int arrow_Bottom_Z = arrow_center_Z + (arrow_length / 2);

double arrow_Bottom_Xdelta = 0;
double arrow_Bottom_Zdelta = 0;


int arrow_rot_centerX = arrow_center_X;
int arrow_rot_centerZ = arrow_center_Z;




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




struct WallHaveParamDef {
	int vecCamWall_StartX;
	int vecCamWall_StartZ; // カメラから起点に向かうベクトルa
	int vecCamWall_EndX;
	int vecCamWall_EndZ; // カメラから終点に向かうベクトルb


	//int WallObje[iCount].vecCamWall_EndX = WallObje[iCount].endLineX - camX; int WallObje[iCount].vecCamWall_EndZ = WallObje[iCount].endLineZ - camZ; // カメラから終点に向かうベクトルb

	int Yunder;
	int Ytop;

	int daihyouZ;


	int vecCamUnderZ;
	int vecCamUnderY;

	int vecCamTopZ;
	int vecCamTopY;

	int	startLineX;
	int	startLineZ;


	int	endLineX;
	int endLineZ;

	int Width;
	int Height;



	double InnerAB_XZ ; // 内積a・b
	double InnerAE_XZ ; // 内積a・unitX
	double InnerEB_XZ ; // 内積 unitX・b


	double InnerAB_ZY; // ZY側面の内積a・b
	double InnerAE_ZY; // 内積a・unitY
	double InnerEB_ZY; // 内積 unitY・b

	double absoluteAB_ZY;



	double absoluteAE_ZY ; // 絶対値|a| |1|


	double absoluteEB_ZY ; // 絶対値|b| |1|



	double absoluteAB_XZ ; // 絶対値|a| |b|


	double absoluteAE_XZ ; // 絶対値|a| |1|


	double absoluteEB_XZ ; // 絶対値|b| |1|


	double cosThetaAB_ZY ;
	double cosThetaAE_ZY ;
	double cosThetaEB_ZY ;

	double cosThetaAB_XZ ;
	double cosThetaAE_XZ ;
	double cosThetaEB_XZ ;


	float ThetaAB_ZY ;
	float ThetaAE_ZY ;
	float ThetaEB_ZY ;


	float ThetaAB_XZ ;
	float ThetaAE_XZ ;
	float ThetaEB_XZ ;


	float ThetaAE_XZwithCamera ;

	float ThetaEB_XZwithCamera ;



	float magnificationAB_XZ ;
	float magnificationAB_ZY ;




	double magnificationAE_XZ ;
	double magnificationEB_XZ ;

	double magnificationAE_ZY ;
	double magnificationEB_ZY ;

};


struct WallHaveParamDef WallObje[2]; // 構造体配列の宣言




int camY = 70; // 視線の高さ


double camXdelta = (double)camX;
double camYtemp = (double)camY;
double camZdelta = (double)camY;


double deffer_angleAccumulation = 0.05;
int deffer_angleCount = 0;



double save_angleAccumulation = 0.05;
int save_angleCount = 0;



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





	int iCount = 0;

	WallObje[iCount].startLineX = 280; WallObje[iCount].startLineZ = 80;
	WallObje[iCount].endLineX = 320; WallObje[iCount].endLineZ = 80;

//	WallObje[iCount].Width = WallObje[iCount].endLineX - WallObje[iCount].startLineX; // 40 // 未使用?
	WallObje[iCount].Height = 100;


	iCount = 1;
	WallObje[iCount].startLineX = 320; WallObje[iCount].startLineZ = 80;
	WallObje[iCount].endLineX = 320;	WallObje[iCount].endLineZ = 60;

	WallObje[iCount].Height = 100;



	iCount = 0;





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
			
			int iCount = 0;
			for (iCount = 0; iCount < 21; ++iCount) {

				if (iCount == 1) { break; }
				WallObje[iCount].vecCamWall_StartX = WallObje[iCount].startLineX - camX;
				WallObje[iCount].vecCamWall_StartZ = WallObje[iCount].startLineZ - camZ; // カメラから起点に向かうベクトルa
				WallObje[iCount].vecCamWall_EndX = WallObje[iCount].endLineX - camX;
				WallObje[iCount].vecCamWall_EndZ = WallObje[iCount].endLineZ - camZ; // カメラから終点に向かうベクトルb




				WallObje[iCount].Yunder = 0;
				WallObje[iCount].Ytop = WallObje[iCount].Yunder + WallObje[iCount].Height; // 高さはY軸にしている。

				WallObje[iCount].daihyouZ = WallObje[iCount].startLineZ; // 壁のZ位置は、WallObje[iCount].startLineZで代用した。



				WallObje[iCount].vecCamUnderZ = WallObje[iCount].daihyouZ - camZ;
				WallObje[iCount].vecCamUnderY = WallObje[iCount].Yunder - camY; // カメラから壁下に向かうベクトル
				WallObje[iCount].vecCamTopZ = WallObje[iCount].daihyouZ - camZ;
				WallObje[iCount].vecCamTopY = WallObje[iCount].Ytop - camY; // カメラから壁上に向かうベクトル






				// Y
				_stprintf_s(convertStringBuffer, 200, TEXT("カメラY: %d"), camY); // デバッグ用メッセージ 
				TextOut(hdc, 450, 160, convertStringBuffer, lstrlen(convertStringBuffer));


				_stprintf_s(convertStringBuffer, 200, TEXT("壁下Y: %d"), WallObje[iCount].Yunder); // デバッグ用メッセージ 
				TextOut(hdc, 450, 160 + 20, convertStringBuffer, lstrlen(convertStringBuffer));

				_stprintf_s(convertStringBuffer, 200, TEXT("カメラ → 壁下Y: %d"), WallObje[iCount].vecCamUnderY); // デバッグ用メッセージ 
				TextOut(hdc, 450, 160 + 20 + 20, convertStringBuffer, lstrlen(convertStringBuffer));



				// Z
				_stprintf_s(convertStringBuffer, 200, TEXT("カメラZ: %d"), camZ); // デバッグ用メッセージ 
				TextOut(hdc, 650, 160, convertStringBuffer, lstrlen(convertStringBuffer));

				_stprintf_s(convertStringBuffer, 200, TEXT("壁Z: %d"), WallObje[iCount].daihyouZ); // デバッグ用メッセージ 
				TextOut(hdc, 650, 160 + 20, convertStringBuffer, lstrlen(convertStringBuffer));

				_stprintf_s(convertStringBuffer, 200, TEXT("カメラ → 壁Z: %d"), WallObje[iCount].vecCamUnderZ); // デバッグ用メッセージ 
				TextOut(hdc, 650, 160 + 20 + 20, convertStringBuffer, lstrlen(convertStringBuffer));



				// int vec_unitX_x = 1; int vec_unitX_y = 0; // x方向（画面で右側）を向いている単位ベクトルの成分

				//vecCamWall_EndX
				WallObje[iCount].InnerAB_XZ = WallObje[iCount].vecCamWall_StartX * WallObje[iCount].vecCamWall_EndX + WallObje[iCount].vecCamWall_StartZ * WallObje[iCount].vecCamWall_EndZ;  // 内積a・b
				WallObje[iCount].InnerAE_XZ = WallObje[iCount].vecCamWall_StartX * 1;  // 内積a・unitX
				WallObje[iCount].InnerEB_XZ = WallObje[iCount].vecCamWall_EndX * 1;  // 内積 unitX・b



				// int vec_unitY_y = 1; int vec_unitY_z = 0; // y方向（画面からユーザーの向き）を向いている単位ベクトルの成分

				WallObje[iCount].InnerAB_ZY = WallObje[iCount].vecCamUnderZ * WallObje[iCount].vecCamTopZ + WallObje[iCount].vecCamUnderY * WallObje[iCount].vecCamTopY;  // ZY側面の内積a・b
				WallObje[iCount].InnerAE_ZY = WallObje[iCount].vecCamUnderY * 1;  // 内積a・unitY
				WallObje[iCount].InnerEB_ZY = WallObje[iCount].vecCamTopY * 1;  // 内積 unitY・b



				WallObje[iCount].absoluteAB_ZY = sqrt(
					(WallObje[iCount].vecCamUnderZ * WallObje[iCount].vecCamUnderZ + WallObje[iCount].vecCamUnderY * WallObje[iCount].vecCamUnderY) * (WallObje[iCount].vecCamTopZ * WallObje[iCount].vecCamTopZ + WallObje[iCount].vecCamTopY * WallObje[iCount].vecCamTopY)
				); // 絶対値|a| |b|


		//		_stprintf_s(convertStringBuffer, 200, TEXT("%d"), (int)WallObje[iCount].absoluteAB_ZY); // デバッグ用メッセージ WallObje[iCount].absoluteAB_ZY のつもり
		//		TextOut(hdc, 650, 415, convertStringBuffer, lstrlen(convertStringBuffer));



				WallObje[iCount].absoluteAE_ZY = sqrt(
					(WallObje[iCount].vecCamUnderZ * WallObje[iCount].vecCamUnderZ + WallObje[iCount].vecCamUnderY * WallObje[iCount].vecCamUnderY) * 1
				); // 絶対値|a| |1|


				WallObje[iCount].absoluteEB_ZY = sqrt(
					(WallObje[iCount].vecCamTopZ * WallObje[iCount].vecCamTopZ + WallObje[iCount].vecCamTopY * WallObje[iCount].vecCamTopY) * 1
				); // 絶対値|b| |1|



				WallObje[iCount].absoluteAB_XZ = sqrt(
					(WallObje[iCount].vecCamWall_StartX * WallObje[iCount].vecCamWall_StartX + WallObje[iCount].vecCamWall_StartZ * WallObje[iCount].vecCamWall_StartZ) * (WallObje[iCount].vecCamWall_EndX * WallObje[iCount].vecCamWall_EndX + WallObje[iCount].vecCamWall_EndZ * WallObje[iCount].vecCamWall_EndZ)
				); // 絶対値|a| |b|


				WallObje[iCount].absoluteAE_XZ = sqrt(
					(WallObje[iCount].vecCamWall_StartX * WallObje[iCount].vecCamWall_StartX + WallObje[iCount].vecCamWall_StartZ * WallObje[iCount].vecCamWall_StartZ) * 1
				); // 絶対値|a| |1|


				WallObje[iCount].absoluteEB_XZ = sqrt(
					(WallObje[iCount].vecCamWall_EndX * WallObje[iCount].vecCamWall_EndX + WallObje[iCount].vecCamWall_EndZ * WallObje[iCount].vecCamWall_EndZ) * 1
				); // 絶対値|b| |1|


				WallObje[iCount].cosThetaAB_ZY = WallObje[iCount].InnerAB_ZY / WallObje[iCount].absoluteAB_ZY;
				WallObje[iCount].cosThetaAE_ZY = WallObje[iCount].InnerAE_ZY / WallObje[iCount].absoluteAE_ZY;
				WallObje[iCount].cosThetaEB_ZY = WallObje[iCount].InnerEB_ZY / WallObje[iCount].absoluteEB_ZY;

				WallObje[iCount].cosThetaAB_XZ = WallObje[iCount].InnerAB_XZ / WallObje[iCount].absoluteAB_XZ;
				WallObje[iCount].cosThetaAE_XZ = WallObje[iCount].InnerAE_XZ / WallObje[iCount].absoluteAE_XZ;
				WallObje[iCount].cosThetaEB_XZ = WallObje[iCount].InnerEB_XZ / WallObje[iCount].absoluteEB_XZ;


				WallObje[iCount].ThetaAB_ZY = (float)acos(WallObje[iCount].cosThetaAB_ZY);
				WallObje[iCount].ThetaAE_ZY = (float)acos(WallObje[iCount].cosThetaAE_ZY);
				WallObje[iCount].ThetaEB_ZY = (float)acos(WallObje[iCount].cosThetaEB_ZY);


				WallObje[iCount].ThetaAB_XZ = (float)acos(WallObje[iCount].cosThetaAB_XZ);
				WallObje[iCount].ThetaAE_XZ = (float)acos(WallObje[iCount].cosThetaAE_XZ);
				WallObje[iCount].ThetaEB_XZ = (float)acos(WallObje[iCount].cosThetaEB_XZ);




				WallObje[iCount].ThetaAE_XZwithCamera = WallObje[iCount].ThetaAE_XZ + save_angleAccumulation; // これが2周目以降にズレルので要 修正。

				WallObje[iCount].ThetaEB_XZwithCamera = WallObje[iCount].ThetaEB_XZ + save_angleAccumulation;


				float angleAE_Buf1 = cos(WallObje[iCount].ThetaAE_XZwithCamera);
				float angleAE_Buf2 = acos(angleAE_Buf1);


				float angleEB_Buf1 = cos(WallObje[iCount].ThetaEB_XZwithCamera);
				float angleEB_Buf2 = acos(angleEB_Buf1);


				WallObje[iCount].magnificationAB_XZ = WallObje[iCount].ThetaAB_XZ / 0.3;
				WallObje[iCount].magnificationAB_ZY = WallObje[iCount].ThetaAB_ZY / 0.3;


				double Pi = 3.141;

				WallObje[iCount].magnificationAE_XZ = (angleAE_Buf2 - (Pi / 2)) / 0.1;
				WallObje[iCount].magnificationEB_XZ = (angleEB_Buf2 - (Pi / 2)) / 0.1;

				WallObje[iCount].magnificationAE_ZY = (WallObje[iCount].ThetaAE_ZY - Pi / 2) / 0.1;
				WallObje[iCount].magnificationEB_ZY = (WallObje[iCount].ThetaEB_ZY - Pi / 2) / 0.1;

				if (iCount == 0) { break; }
			}

			// now_movewhat = moveCamera ;

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


			
		//	_stprintf_s(convertStringBuffer, 200, TEXT("%d"), 100*Theta); // デバッグ用メッセージ
		//	TextOut(hdc, 400, 400, convertStringBuffer, lstrlen(convertStringBuffer));


				// X軸の拡大率の計算デバッグ用

			int debugMx1 = 300 ; int debugMy = 300;

			_stprintf_s(convertStringBuffer, 200, TEXT("内積: %d"), (int)WallObje[iCount].InnerAB_XZ); // デバッグ用メッセージ 内積のつもり
			TextOut(hdc, debugMx1, debugMy, convertStringBuffer, lstrlen(convertStringBuffer));

			_stprintf_s(convertStringBuffer, 200, TEXT("絶対値: %d"), (int)WallObje[iCount].absoluteAB_XZ); // デバッグ用メッセージ 絶対値のつもり
			TextOut(hdc, debugMx1, debugMy + 30, convertStringBuffer, lstrlen(convertStringBuffer));

			int bufSeisuu = (int)100 * WallObje[iCount].cosThetaAE_XZ;

			_stprintf_s(convertStringBuffer, 200, TEXT("100 cosθ2: %d"), bufSeisuu); // デバッグ用メッセージ cosθのつもり
			TextOut(hdc, debugMx1,  debugMy + 60, convertStringBuffer, lstrlen(convertStringBuffer));


			bufSeisuu = (int)100 * WallObje[iCount].ThetaAE_XZ;
			_stprintf_s(convertStringBuffer, 200, TEXT("100 θ2: %d"), (int)bufSeisuu); // デバッグ用メッセージ 角度θのつもり
			TextOut(hdc, debugMx1, debugMy + 90, convertStringBuffer, lstrlen(convertStringBuffer));

			bufSeisuu = (int)100 * WallObje[iCount].magnificationAE_XZ;
			_stprintf_s(convertStringBuffer, 200, TEXT("倍率2: %d"), (int)bufSeisuu); // デバッグ用メッセージ 角度θのつもり
			TextOut(hdc, debugMx1, debugMy + 120, convertStringBuffer, lstrlen(convertStringBuffer));




			// Y軸の拡大率の計算デバッグ用
			int debugMx2 = 600;

			_stprintf_s(convertStringBuffer, 200, TEXT("内積: %d"), (int)WallObje[iCount].InnerAB_ZY); // デバッグ用メッセージ 内積のつもり
			TextOut(hdc, debugMx2, debugMy, convertStringBuffer, lstrlen(convertStringBuffer));

			_stprintf_s(convertStringBuffer, 200, TEXT("絶対値: %d"), (int)WallObje[iCount].absoluteAB_ZY); // デバッグ用メッセージ 絶対値のつもり
			TextOut(hdc, debugMx2, debugMy +30 , convertStringBuffer, lstrlen(convertStringBuffer));

			bufSeisuu = (int)100 * WallObje[iCount].cosThetaAE_ZY;

			_stprintf_s(convertStringBuffer, 200, TEXT("100 cosθ2: %d"), bufSeisuu); // デバッグ用メッセージ cosθのつもり
			TextOut(hdc, debugMx2, debugMy +60, convertStringBuffer, lstrlen(convertStringBuffer));


			bufSeisuu = (int)100 * WallObje[iCount].ThetaAE_ZY;
			_stprintf_s(convertStringBuffer, 200, TEXT("100 θ2: %d"), (int)bufSeisuu); // デバッグ用メッセージ 角度θのつもり
			TextOut(hdc, debugMx2, debugMy + 90, convertStringBuffer, lstrlen(convertStringBuffer));

			bufSeisuu = (int)100 * WallObje[iCount].magnificationAE_ZY;
			_stprintf_s(convertStringBuffer, 200, TEXT("倍率2: %d"), (int)bufSeisuu); // デバッグ用メッセージ 角度θのつもり
			TextOut(hdc, debugMx2, debugMy + 120, convertStringBuffer, lstrlen(convertStringBuffer));



			// 視界

			int blackXstartPoint = 20 ;
			int blackYstartPoint = 70 ;

			int blackXWidth = 150 ;
			int blackYWidth = 100 ;


			HBRUSH brasi_parts_1;
			brasi_parts_1 = CreateSolidBrush(RGB(0, 0, 0)); // 黒色のブラシを作成。背景用。
			SelectObject(hdc, brasi_parts_1); // ウィンドウhdcと、さきほど作成したブラシを関連づけ
			Rectangle(hdc, blackXstartPoint, blackYstartPoint, blackXstartPoint + blackXWidth, blackYstartPoint + blackYWidth); // 図形の描画


			int blackXcentral = blackXstartPoint + ( blackXWidth / 2) ;
			int blackYcentral = blackYstartPoint + ( blackYWidth / 2) ;




			int adjustParam = 3; // 単に、ピンク壁の初期位置での、視界での大きさを調整するための係数。

			int pinkWallXcentral = blackXcentral - adjustParam *10 * deffer_angleCount;
			int pinkWallYcentral = blackYcentral;



			HBRUSH brasi_parts_2;
			brasi_parts_2 = CreateSolidBrush(RGB(255, 100, 100)); // 壁の表示用のピンク色のブラシを作成
			SelectObject(hdc, brasi_parts_2); // ウィンドウhdcと、さきほど作成したブラシを関連づけ
			
			for (iCount = 0; iCount < 21; ++iCount) {
				if (iCount == 1) { break; }
				Rectangle(hdc,
					blackXcentral - adjustParam * WallObje[iCount].magnificationAE_XZ,
					blackYcentral - adjustParam * WallObje[iCount].magnificationAE_ZY,
					blackXcentral - adjustParam * WallObje[iCount].magnificationEB_XZ,
					blackYcentral - adjustParam * WallObje[iCount].magnificationEB_ZY); // 基準の状態
			}

			lstrcpy(WordBuffer, TEXT("視界"));
			TextOut(hdc, 80, 30, WordBuffer, lstrlen(WordBuffer));




			//上面図
			//被写体の上面図
						
			for (iCount = 0; iCount < 21; ++iCount) {
				if (iCount == 1) { break; }
				MoveToEx(hdc, WallObje[iCount].startLineX, WallObje[iCount].startLineZ, NULL);
				LineTo(hdc, WallObje[iCount].endLineX, WallObje[iCount].endLineZ);
			}


			//カメラの位置と向き。矢印の先端を位置の基準とする。

			MoveToEx(hdc, arrow_Head_X , arrow_Head_Z, NULL); // 矢軸の矢先側
			LineTo(hdc, arrow_center_X, arrow_center_Z ); // 回転によりcenterを中心に移動するので、centerまで線をひく


			MoveToEx(hdc, arrow_center_X, arrow_center_Z, NULL); // 矢軸のうしろ側
			LineTo(hdc, arrow_Bottom_X, arrow_Bottom_Z);



			MoveToEx(hdc, arrow_Head_X , arrow_Head_Z, NULL); // 矢先の斜め線 右側
			LineTo(hdc, arrow_Head_RightX, arrow_Head_RightZ);

			MoveToEx(hdc, arrow_Head_X , arrow_Head_Z, NULL); // 左側
			LineTo(hdc, arrow_Head_LeftX, arrow_Head_LeftZ);


			MoveToEx(hdc, arrow_center_X -30 , arrow_center_Z, NULL); // 矢の中心の横線
			LineTo(hdc, arrow_center_X +30 , arrow_center_Z);

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
					ParallelDefferX = 0;
					ParallelDefferZ = -5;
					
					break;
				}


				case VK_DOWN:
				{
					ParallelDefferX = 0;
					ParallelDefferZ = +5;

					break;
				}

				case VK_RIGHT:
				{
					ParallelDefferX = +5;
					ParallelDefferZ = 0;

					break;
				}

				case VK_LEFT:
				{
					ParallelDefferX = -5;
					ParallelDefferZ = 0;

					break;
				}

				case 'R':
				{
					skipRotFlag = 0;

					ParallelDefferX = 0 ; // 念のため移動量をゼロに設定。
					ParallelDefferZ = 0 ;

					now_movetype = moveRotate;


					// arrow_Head_RightXrot0


					arrow_Head_Xrot0 = arrow_Head_X;
					arrow_Head_Zrot0 = arrow_Head_Z;

					arrow_Head_RightXrot0 = arrow_Head_RightX;
					arrow_Head_RightZrot0 = arrow_Head_RightZ;

					arrow_Head_LeftXrot0 = arrow_Head_LeftX;
					arrow_Head_LeftZrot0 = arrow_Head_LeftZ;


					arrow_rot_centerX = arrow_center_X;
					arrow_rot_centerZ = arrow_center_Z;


					save_angleAccumulation = save_angleAccumulation + deffer_angleAccumulation;
					save_angleCount = save_angleCount + deffer_angleCount;

					deffer_angleCount = 0;
					deffer_angleAccumulation = 0;

					break;
				}
			//	break;

				} // これはswitch文を終わり

				arrow_center_X = arrow_center_X + ParallelDefferX;
				arrow_Head_X = arrow_Head_X + ParallelDefferX;
				arrow_Head_RightX = arrow_Head_RightX + ParallelDefferX;
				arrow_Head_LeftX = arrow_Head_LeftX + ParallelDefferX;
				arrow_Bottom_X = arrow_Bottom_X + ParallelDefferX;

				camX = arrow_center_X;


				arrow_center_Z = arrow_center_Z + ParallelDefferZ;
				arrow_Head_Z = arrow_Head_Z + ParallelDefferZ;
				arrow_Head_RightZ = arrow_Head_RightZ + ParallelDefferZ;
				arrow_Head_LeftZ = arrow_Head_LeftZ + ParallelDefferZ;
				arrow_Bottom_Z = arrow_Bottom_Z + ParallelDefferZ;

				camZ = arrow_center_Z;


				InvalidateRect(hWnd, NULL, TRUE);
				UpdateWindow(hWnd);

			}

			if (now_movetype == moveRotate) {

				switch (wParam) {
				case VK_RIGHT:
					angleStep = +0.05;

					deffer_angleCount = deffer_angleCount + 1;
					deffer_angleAccumulation = deffer_angleCount * angleStep;



					save_angleAccumulation = save_angleAccumulation + angleStep;
					save_angleCount = save_angleCount + 1;


					break;

				case VK_LEFT:
					angleStep = 0.05;

					deffer_angleCount = deffer_angleCount - 1;
					deffer_angleAccumulation = deffer_angleCount * angleStep;



					save_angleAccumulation = save_angleAccumulation - angleStep;
					save_angleCount = save_angleCount - 1;



					break;

				case 'P':
					now_movetype = moveParallel;




					deffer_angleCount = 0;
					deffer_angleAccumulation = 0;


					skipRotFlag = 1;

					InvalidateRect(hWnd, NULL, TRUE);
					UpdateWindow(hWnd);

					break;
				}

				if (skipRotFlag == 1)  break; 
				else 
				for (int j = 1; j <= 20; ++j) {

					// ここに共通する前段階の作業を記述;


					if (j == 1) {
						rotBufferXinput1 = arrow_Head_Xrot0; // 回転行列のインプット側に、矢先端の中央の位置を入力
						rotBufferZinput1 = arrow_Head_Zrot0;
					}
					if (j == 2) {
						rotBufferXinput1 = arrow_Head_RightXrot0; // 矢先端の右側の位置入力
						rotBufferZinput1 = arrow_Head_RightZrot0;
					}
					if (j == 3) {
						rotBufferXinput1 = arrow_Head_LeftXrot0;
						rotBufferZinput1 = arrow_Head_LeftZrot0;
					}
					if (j == 4) { ; }
					if (j == 5) {
						;
						break;

					}

					// 共通処理
					rotBufferXoutput1 = cos(deffer_angleAccumulation) * (rotBufferXinput1 - arrow_rot_centerX) + (-1) * sin(deffer_angleAccumulation) * (rotBufferZinput1 - arrow_rot_centerZ);
					rotBufferZoutput1 = sin(deffer_angleAccumulation) * (rotBufferXinput1 - arrow_rot_centerX) + cos(deffer_angleAccumulation) * (rotBufferZinput1 - arrow_rot_centerZ);




					// 後工程の分岐
					if (j == 1) {
						arrow_Head_Xdelta = rotBufferXoutput1; // 回転行列の答えをキャッチ
						arrow_Head_Zdelta = rotBufferZoutput1;

						arrow_Head_X = (int)(arrow_rot_centerX + arrow_Head_Xdelta);
						arrow_Head_Z = (int)(arrow_rot_centerZ + arrow_Head_Zdelta);

						arrow_Bottom_X = (int)(arrow_rot_centerX - arrow_Head_Xdelta);
						arrow_Bottom_Z = (int)(arrow_rot_centerZ - arrow_Head_Zdelta);
					}
					if (j == 2) {
						rotArrowSideRightDefferX = rotBufferXoutput1; // 回転行列の答えをキャッチ
						rotArrowSideRightDefferZ = rotBufferZoutput1;

						arrow_Head_RightX = (int)(arrow_rot_centerX + rotArrowSideRightDefferX);
						arrow_Head_RightZ = (int)(arrow_rot_centerZ + rotArrowSideRightDefferZ);
					}
					if (j == 3) {
						rotArrowSideLeftDefferX = rotBufferXoutput1; // 回転行列の答えをキャッチ
						rotArrowSideLeftDefferZ = rotBufferZoutput1;

						arrow_Head_LeftX = (int)(arrow_rot_centerX + rotArrowSideLeftDefferX);
						arrow_Head_LeftZ = (int)(arrow_rot_centerZ + rotArrowSideLeftDefferZ);

					}
					if (j == 4) { ; }
					if (j == 5) {
						;
						// break;

					}
				}// for文の終わり



				InvalidateRect(hWnd, NULL, TRUE);
				UpdateWindow(hWnd);


			} // moveRotate の if の終わり
		} // moveCamera の if の終わり

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
