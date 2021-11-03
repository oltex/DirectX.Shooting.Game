#define WIN32_LEAN_AND_MEAN

#include "define.h"

LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
bool InitializeDirectX(void);
extern BOOL _InitDirectSound();
bool LoadBMPandInitSurface();

void GameMain(void);
void InitGame();

HWND g_hwnd;
HINSTANCE g_hInstance;

LPDIRECTDRAW7 g_lpDirectDrawObject = NULL;
LPDIRECTDRAWSURFACE7 g_lpPrimarySurface = NULL;
LPDIRECTDRAWSURFACE7 g_lpSecondarySurface = NULL;

LPDIRECTINPUT8A g_lpDirectInputObject = NULL;
LPDIRECTINPUTDEVICE8A g_lpDirectInputKeyboard = NULL;

CSprite g_TitleSprite;
CSprite g_BackgroundSprite;
CSprite g_Background2Sprite;
CSprite g_Background3Sprite;
CSprite g_Background4Sprite;

CSprite g_InterfaceSprite;
CSprite g_Interface2Sprite;
CSprite g_Interface3Sprite;
CSprite g_Interface4Sprite;

CSprite g_MyShipSprite;
CSprite g_MyShip2Sprite;
CSprite g_MyMissileSprite;
CSprite g_EnemySprite;
CSprite g_EnemyMissileSprite;
CSprite g_BossSprite;
CSprite g_BossMissileSprite;
CSprite g_ExplodingSprite;
CSprite g_ItemSprite;

CSprite g_ClearSprite;
CSprite g_DeadSprite;

CTimer g_Timer;
bool g_bActiveApp = false;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	WNDCLASSEX wndclass;
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WinProc;
	wndclass.cbClsExtra = wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = CLASS_NAME;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (RegisterClassEx(&wndclass) == 0)
		return 0;
	g_hwnd = CreateWindowEx(WS_EX_TOPMOST, CLASS_NAME, CLASS_NAME, WS_POPUP | WS_VISIBLE, NULL, NULL,GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), NULL, NULL, hInstance, NULL);

	if (g_hwnd == NULL) return 0;
	g_hInstance = hInstance;

	SetFocus(g_hwnd);
	ShowCursor(FALSE);

	if (!InitializeDirectX())
		return 0;
	if (!_InitDirectSound())
		return 0;
	g_Timer.start();
	if (!LoadBMPandInitSurface())
		return 0;

	MSG msg;
	while (true){
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)){
			if (!GetMessage(&msg, NULL, 0, 0))
				return (int)msg.wParam;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else if (g_bActiveApp)
			GameMain();
		else
			WaitMessage();
	}
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam){
	switch (message){
	case WM_ACTIVATEAPP:
		if (wParam)
			g_bActiveApp = true;
		else
			g_bActiveApp = false;
		break;
	case WM_KEYDOWN:
		if (wParam == 'Y') InitGame();
		if (wParam == 'N') DestroyWindow(hwnd);
		if (wParam == VK_ESCAPE) DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		if (g_lpDirectDrawObject){
			if (g_lpSecondarySurface)
				g_lpSecondarySurface->Release();
			if (g_lpPrimarySurface)
				g_lpPrimarySurface->Release();
			g_DeadSprite.ReleaseAll();
			g_ClearSprite.ReleaseAll();
			g_BossMissileSprite.ReleaseAll();
			g_BossSprite.ReleaseAll();
			g_EnemyMissileSprite.ReleaseAll();
			g_EnemySprite.ReleaseAll();
			g_BackgroundSprite.ReleaseAll();
			g_TitleSprite.ReleaseAll();
			g_lpDirectDrawObject->Release();
		}
		if (g_lpDirectInputObject){
			if (g_lpDirectInputKeyboard){
				g_lpDirectInputKeyboard->Unacquire();
				g_lpDirectInputKeyboard->Release();
			}
			g_lpDirectInputObject->Release();
		}
		ShowCursor(TRUE);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}

bool LoadBMPandInitSurface(){
	int fileNameIndex;
	if (!g_TitleSprite.InitSprite(2, 640, 480, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!g_TitleSprite.LoadFrame(0, "data\\title.bmp"))
		return false;
	g_TitleSprite.Drawing(false, 0,  320, 240, g_lpSecondarySurface, false);
	HRESULT hResult;
	if (FAILED(hResult = g_lpPrimarySurface->Flip(NULL, DDFLIP_WAIT))){
		if (hResult == DDERR_SURFACELOST)
			g_lpPrimarySurface->Restore();
	}
	if (!g_BackgroundSprite.InitSprite(2, 4480, 409, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!g_BackgroundSprite.LoadFrame(0, "data\\background.bmp"))
		return false;
	if (!g_Background2Sprite.InitSprite(2, 4352, 410, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!g_Background2Sprite.LoadFrame(0, "data\\background2.bmp"))
		return false;
	if (!g_Background3Sprite.InitSprite(2, 4480, 409, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!g_Background3Sprite.LoadFrame(0, "data\\background3.bmp"))
		return false;
	if (!g_Background4Sprite.InitSprite(2, 4480, 409, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!g_Background4Sprite.LoadFrame(0, "data\\background4.bmp"))
		return false;



	if (!g_InterfaceSprite.InitSprite(20, 19, 19, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!g_InterfaceSprite.LoadFrame(0, "data\\interface_number0.bmp"))
		return false;
	if (!g_InterfaceSprite.LoadFrame(1, "data\\interface_number1.bmp"))
		return false;
	if (!g_InterfaceSprite.LoadFrame(2, "data\\interface_number2.bmp"))
		return false;
	if (!g_InterfaceSprite.LoadFrame(3, "data\\interface_number3.bmp"))
		return false;
	if (!g_InterfaceSprite.LoadFrame(4, "data\\interface_number4.bmp"))
		return false;
	if (!g_InterfaceSprite.LoadFrame(5, "data\\interface_number5.bmp"))
		return false;
	if (!g_InterfaceSprite.LoadFrame(6, "data\\interface_number6.bmp"))
		return false;
	if (!g_InterfaceSprite.LoadFrame(7, "data\\interface_number7.bmp"))
		return false;
	if (!g_InterfaceSprite.LoadFrame(8, "data\\interface_number8.bmp"))
		return false;
	if (!g_InterfaceSprite.LoadFrame(9, "data\\interface_number9.bmp"))
		return false;
	if (!g_Interface2Sprite.InitSprite(4, 40, 36, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!g_Interface2Sprite.LoadFrame(0, "data\\interface_weapon00.bmp"))
		return false;
	if (!g_Interface2Sprite.LoadFrame(1, "data\\interface_weapon01.bmp"))
		return false;
	if (!g_Interface3Sprite.InitSprite(4, 13, 13, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!g_Interface3Sprite.LoadFrame(0, "data\\interface_hpbar.bmp"))
		return false;
	if (!g_Interface4Sprite.InitSprite(4, 640, 480, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!g_Interface4Sprite.LoadFrame(0, "data\\interface.bmp"))
		return false;




	if (!g_MyShipSprite.InitSprite(530, 130, 130, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!g_MyShipSprite.LoadFrame(0, "data\\upper_body_stand00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(1, "data\\upper_body_stand00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(2, "data\\upper_body_stand00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(3, "data\\upper_body_stand01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(4, "data\\upper_body_stand01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(5, "data\\upper_body_stand01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(6, "data\\upper_body_stand02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(7, "data\\upper_body_stand02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(8, "data\\upper_body_stand02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(9, "data\\upper_body_stand03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(10, "data\\upper_body_stand03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(11, "data\\upper_body_stand03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(12, "data\\upper_body_move00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(13, "data\\upper_body_move01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(14, "data\\upper_body_move02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(15, "data\\upper_body_move03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(16, "data\\upper_body_move04.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(17, "data\\upper_body_move05.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(18, "data\\upper_body_move06.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(19, "data\\upper_body_move07.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(20, "data\\upper_body_move08.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(21, "data\\upper_body_move09.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(22, "data\\upper_body_move10.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(23, "data\\upper_body_move11.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(24, "data\\upper_body_up00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(25, "data\\upper_body_up00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(26, "data\\upper_body_up00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(27, "data\\upper_body_up01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(28, "data\\upper_body_up01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(29, "data\\upper_body_up01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(30, "data\\upper_body_up02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(31, "data\\upper_body_up02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(32, "data\\upper_body_up02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(33, "data\\upper_body_up03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(34, "data\\upper_body_up03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(35, "data\\upper_body_up03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(36, "data\\upper_body_shot_stand00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(37, "data\\upper_body_shot_stand01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(38, "data\\upper_body_shot_stand02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(39, "data\\upper_body_shot_stand03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(40, "data\\upper_body_shot_stand04.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(41, "data\\upper_body_shot_stand05.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(42, "data\\upper_body_shot_stand06.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(43, "data\\upper_body_shot_stand07.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(44, "data\\upper_body_shot_stand08.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(45, "data\\upper_body_shot_stand09.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(46, "data\\upper_body_shot_up00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(47, "data\\upper_body_shot_up01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(48, "data\\upper_body_shot_up02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(49, "data\\upper_body_shot_up03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(50, "data\\upper_body_shot_up04.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(51, "data\\upper_body_shot_up05.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(52, "data\\upper_body_shot_up06.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(53, "data\\upper_body_shot_up07.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(54, "data\\upper_body_shot_up08.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(55, "data\\upper_body_shot_up09.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(56, "data\\upper_body_down00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(57, "data\\upper_body_down00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(58, "data\\upper_body_down00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(59, "data\\upper_body_down01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(60, "data\\upper_body_down01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(61, "data\\upper_body_down01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(62, "data\\upper_body_down02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(63, "data\\upper_body_down02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(64, "data\\upper_body_down02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(65, "data\\upper_body_down03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(66, "data\\upper_body_down03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(67, "data\\upper_body_down03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(68, "data\\upper_body_jump_stand00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(69, "data\\upper_body_jump_stand00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(70, "data\\upper_body_jump_stand01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(71, "data\\upper_body_jump_stand01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(72, "data\\upper_body_jump_stand02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(73, "data\\upper_body_jump_stand02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(74, "data\\upper_body_jump_stand03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(75, "data\\upper_body_jump_stand03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(76, "data\\upper_body_jump_stand04.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(77, "data\\upper_body_jump_stand04.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(78, "data\\upper_body_jump_stand05.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(79, "data\\upper_body_jump_stand05.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(80, "data\\upper_body_jump_move00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(81, "data\\upper_body_jump_move00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(82, "data\\upper_body_jump_move01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(83, "data\\upper_body_jump_move01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(84, "data\\upper_body_jump_move02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(85, "data\\upper_body_jump_move02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(86, "data\\upper_body_jump_move03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(87, "data\\upper_body_jump_move03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(88, "data\\upper_body_jump_move04.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(89, "data\\upper_body_jump_move04.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(90, "data\\upper_body_jump_move05.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(91, "data\\upper_body_jump_move05.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(92, "data\\upper_body_shot_jump_down00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(93, "data\\upper_body_shot_jump_down01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(94, "data\\upper_body_shot_jump_down02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(95, "data\\upper_body_shot_jump_down03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(96, "data\\upper_body_shot_jump_down04.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(97, "data\\upper_body_shot_jump_down05.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(98, "data\\upper_body_shot_jump_down06.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(99, "data\\upper_body_shot_down00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(100, "data\\upper_body_shot_down01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(101, "data\\upper_body_shot_down02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(102, "data\\upper_body_shot_down03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(103, "data\\upper_body_shot_down04.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(104, "data\\upper_body_shot_down05.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(105, "data\\upper_body_shot_down06.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(106, "data\\upper_body_shot_down07.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(107, "data\\upper_body_shot_down08.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(108, "data\\upper_body_shot_down09.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(109, "data\\upper_body_move_down00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(110, "data\\upper_body_move_down01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(111, "data\\upper_body_move_down02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(112, "data\\upper_body_move_down03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(113, "data\\upper_body_move_down04.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(114, "data\\upper_body_move_down05.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(115, "data\\upper_body_move_down06.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(116, "data\\upper_body_move_down07.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(117, "data\\upper_body_move_down08.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(118, "data\\upper_body_move_down09.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(119, "data\\upper_body_move_down10.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(120, "data\\upper_body_bomb_stand00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(121, "data\\upper_body_bomb_stand01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(122, "data\\upper_body_bomb_stand02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(123, "data\\upper_body_bomb_stand03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(124, "data\\upper_body_bomb_stand04.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(125, "data\\upper_body_bomb_down00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(126, "data\\upper_body_bomb_down01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(127, "data\\upper_body_bomb_down02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(128, "data\\upper_body_bomb_down03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(129, "data\\upper_body_bomb_down04.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(130, "data\\upper_body_cut_stand00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(131, "data\\upper_body_cut_stand01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(132, "data\\upper_body_cut_stand02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(133, "data\\upper_body_cut_stand03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(134, "data\\upper_body_cut_stand04.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(135, "data\\upper_body_cut_down00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(136, "data\\upper_body_cut_down01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(137, "data\\upper_body_cut_down02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(138, "data\\upper_body_cut_down03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(139, "data\\upper_body_cut_down04.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(140, "data\\upper_body_weapon_stand00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(141, "data\\upper_body_weapon_stand00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(142, "data\\upper_body_weapon_stand00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(143, "data\\upper_body_weapon_stand01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(144, "data\\upper_body_weapon_stand01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(145, "data\\upper_body_weapon_stand01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(146, "data\\upper_body_weapon_stand02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(147, "data\\upper_body_weapon_stand02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(148, "data\\upper_body_weapon_stand02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(149, "data\\upper_body_weapon_stand03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(150, "data\\upper_body_weapon_stand03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(151, "data\\upper_body_weapon_stand03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(152, "data\\upper_body_weapon_move00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(153, "data\\upper_body_weapon_move01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(154, "data\\upper_body_weapon_move02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(155, "data\\upper_body_weapon_move03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(156, "data\\upper_body_weapon_move04.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(157, "data\\upper_body_weapon_move05.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(158, "data\\upper_body_weapon_move06.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(159, "data\\upper_body_weapon_move07.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(160, "data\\upper_body_weapon_move08.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(161, "data\\upper_body_weapon_move09.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(162, "data\\upper_body_weapon_move10.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(163, "data\\upper_body_weapon_move11.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(164, "data\\upper_body_weapon_up00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(165, "data\\upper_body_weapon_up00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(166, "data\\upper_body_weapon_up00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(167, "data\\upper_body_weapon_up01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(168, "data\\upper_body_weapon_up01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(169, "data\\upper_body_weapon_up01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(170, "data\\upper_body_weapon_up02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(171, "data\\upper_body_weapon_up02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(172, "data\\upper_body_weapon_up02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(173, "data\\upper_body_weapon_up03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(174, "data\\upper_body_weapon_up03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(175, "data\\upper_body_weapon_up03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(176, "data\\upper_body_weapon_down00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(177, "data\\upper_body_weapon_down00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(178, "data\\upper_body_weapon_down00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(179, "data\\upper_body_weapon_down01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(180, "data\\upper_body_weapon_down01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(181, "data\\upper_body_weapon_down01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(182, "data\\upper_body_weapon_down02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(183, "data\\upper_body_weapon_down02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(184, "data\\upper_body_weapon_down02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(185, "data\\upper_body_weapon_down03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(186, "data\\upper_body_weapon_down03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(187, "data\\upper_body_weapon_down03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(188, "data\\upper_body_weapon_down_move00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(189, "data\\upper_body_weapon_down_move01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(190, "data\\upper_body_weapon_down_move02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(191, "data\\upper_body_weapon_down_move03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(192, "data\\upper_body_weapon_down_move04.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(193, "data\\upper_body_weapon_down_move05.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(194, "data\\upper_body_weapon_down_move06.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(195, "data\\upper_body_weapon_down_move07.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(196, "data\\upper_body_weapon_down_move08.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(197, "data\\upper_body_weapon_down_move09.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(198, "data\\upper_body_weapon_down_move10.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(199, "data\\upper_body_weapon_shot00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(200, "data\\upper_body_weapon_shot01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(201, "data\\upper_body_weapon_shot02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(202, "data\\upper_body_weapon_shot03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(203, "data\\upper_body_weapon_shot_down00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(204, "data\\upper_body_weapon_shot_down01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(205, "data\\upper_body_weapon_shot_down02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(206, "data\\upper_body_weapon_shot_down03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(207, "data\\upper_body_weapon_shot_up00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(208, "data\\upper_body_weapon_shot_up01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(209, "data\\upper_body_weapon_shot_up02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(210, "data\\upper_body_weapon_shot_up03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(211, "data\\upper_body_weapon_jump_stand00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(212, "data\\upper_body_weapon_jump_stand01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(213, "data\\upper_body_weapon_jump_stand02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(214, "data\\upper_body_weapon_jump_stand03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(215, "data\\upper_body_weapon_jump_stand04.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(216, "data\\upper_body_weapon_jump_stand05.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(217, "data\\upper_body_weapon_jump_move00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(218, "data\\upper_body_weapon_jump_move01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(219, "data\\upper_body_weapon_jump_move02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(220, "data\\upper_body_weapon_jump_move03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(221, "data\\upper_body_weapon_jump_move04.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(222, "data\\upper_body_weapon_jump_move05.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(223, "data\\upper_body_weapon_up_diagonal00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(224, "data\\upper_body_weapon_up_diagonal01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(225, "data\\upper_body_weapon_shot_up_diagonal_up00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(226, "data\\upper_body_weapon_shot_up_diagonal_up01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(227, "data\\upper_body_weapon_shot_up_diagonal_down00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(228, "data\\upper_body_weapon_shot_up_diagonal_down01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(229, "data\\upper_body_weapon_shot_jump_down00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(230, "data\\upper_body_weapon_shot_jump_down01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(231, "data\\upper_body_weapon_shot_jump_down02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(232, "data\\upper_body_weapon_shot_jump_down03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(233, "data\\upper_body_weapon_down_jump00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(234, "data\\upper_body_weapon_down_jump01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(235, "data\\upper_body_weapon_down_jump02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(236, "data\\upper_body_weapon_down_jump03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(237, "data\\upper_body_weapon_down_jump04.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(238, "data\\upper_body_weapon_down_jump05.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(239, "data\\upper_body_weapon_down_jump_diagonal00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(240, "data\\upper_body_weapon_down_jump_diagonal01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(241, "data\\upper_body_weapon_shot_down_jump_diagonal_down00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(242, "data\\upper_body_weapon_shot_down_jump_diagonal_down01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(243, "data\\upper_body_weapon_shot_down_jump_diagonal_up00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(244, "data\\upper_body_weapon_shot_down_jump_diagonal_up01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(245, "data\\upper_body_weapon_bomb_stand00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(246, "data\\upper_body_weapon_bomb_stand01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(247, "data\\upper_body_weapon_bomb_stand02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(248, "data\\upper_body_weapon_bomb_stand03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(249, "data\\upper_body_weapon_bomb_stand04.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(250, "data\\upper_body_weapon_bomb_down00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(251, "data\\upper_body_weapon_bomb_down01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(252, "data\\upper_body_weapon_bomb_down02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(253, "data\\upper_body_weapon_bomb_down03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(254, "data\\upper_body_weapon_bomb_down04.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(255, "data\\upper_body_weapon_cut00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(256, "data\\upper_body_weapon_cut01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(257, "data\\upper_body_weapon_cut02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(258, "data\\upper_body_weapon_cut03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(259, "data\\upper_body_weapon_cut04.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(260, "data\\upper_body_weapon_cut_down00.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(261, "data\\upper_body_weapon_cut_down01.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(262, "data\\upper_body_weapon_cut_down02.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(263, "data\\upper_body_weapon_cut_down03.bmp"))
		return false;
	if (!g_MyShipSprite.LoadFrame(264, "data\\upper_body_weapon_cut_down04.bmp"))
		return false;


	if (!g_MyShip2Sprite.InitSprite(114, 130, 130, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(0, "data\\lower_body_stand00.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(1, "data\\lower_body_stand00.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(2, "data\\lower_body_stand00.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(3, "data\\lower_body_stand01.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(4, "data\\lower_body_stand01.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(5, "data\\lower_body_stand01.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(6, "data\\lower_body_stand02.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(7, "data\\lower_body_stand02.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(8, "data\\lower_body_stand02.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(9, "data\\lower_body_stand03.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(10, "data\\lower_body_stand03.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(11, "data\\lower_body_stand03.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(12, "data\\lower_body_stand04.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(13, "data\\lower_body_stand04.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(14, "data\\lower_body_stand04.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(15, "data\\lower_body_move00.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(16, "data\\lower_body_move01.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(17, "data\\lower_body_move02.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(18, "data\\lower_body_move03.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(19, "data\\lower_body_move04.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(20, "data\\lower_body_move05.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(21, "data\\lower_body_move06.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(22, "data\\lower_body_move07.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(23, "data\\lower_body_move08.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(24, "data\\lower_body_move09.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(25, "data\\lower_body_move10.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(26, "data\\lower_body_move11.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(27, "data\\lower_body_move12.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(28, "data\\lower_body_move13.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(29, "data\\lower_body_move14.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(30, "data\\lower_body_move15.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(31, "data\\lower_body_move16.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(32, "data\\lower_body_move17.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(33, "data\\lower_body_jump_stand00.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(34, "data\\lower_body_jump_stand01.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(35, "data\\lower_body_jump_stand02.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(36, "data\\lower_body_jump_stand03.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(37, "data\\lower_body_jump_stand04.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(38, "data\\lower_body_jump_stand05.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(39, "data\\lower_body_jump_stand06.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(40, "data\\lower_body_jump_stand07.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(41, "data\\lower_body_jump_stand08.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(42, "data\\lower_body_jump_stand09.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(43, "data\\lower_body_jump_stand10.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(44, "data\\lower_body_jump_stand11.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(45, "data\\lower_body_jump_move00.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(46, "data\\lower_body_jump_move00.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(47, "data\\lower_body_jump_move01.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(48, "data\\lower_body_jump_move01.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(49, "data\\lower_body_jump_move02.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(50, "data\\lower_body_jump_move02.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(51, "data\\lower_body_jump_move03.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(52, "data\\lower_body_jump_move03.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(53, "data\\lower_body_jump_move04.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(54, "data\\lower_body_jump_move04.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(55, "data\\lower_body_jump_move05.bmp"))
		return false;
	if (!g_MyShip2Sprite.LoadFrame(56, "data\\lower_body_jump_move05.bmp"))
		return false;





	if (!g_EnemySprite.InitSprite(224, 130, 130, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!g_EnemySprite.LoadFrame(0, "data\\enemy_stand00.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(1, "data\\enemy_stand00.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(2, "data\\enemy_stand01.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(3, "data\\enemy_stand01.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(4, "data\\enemy_stand02.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(5, "data\\enemy_stand02.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(6, "data\\enemy_stand03.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(7, "data\\enemy_stand03.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(8, "data\\enemy_stand02.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(9, "data\\enemy_stand02.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(10, "data\\enemy_stand01.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(11, "data\\enemy_stand01.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(12, "data\\enemy_move00.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(13, "data\\enemy_move01.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(14, "data\\enemy_move02.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(15, "data\\enemy_move03.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(16, "data\\enemy_move04.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(17, "data\\enemy_move05.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(18, "data\\enemy_move06.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(19, "data\\enemy_move07.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(20, "data\\enemy_move08.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(21, "data\\enemy_move09.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(22, "data\\enemy_move10.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(23, "data\\enemy_move11.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(24, "data\\enemy_throw00.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(25, "data\\enemy_throw01.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(26, "data\\enemy_throw02.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(27, "data\\enemy_throw03.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(28, "data\\enemy_throw04.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(29, "data\\enemy_throw05.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(30, "data\\enemy_throw06.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(31, "data\\enemy_throw07.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(32, "data\\enemy_throw08.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(33, "data\\enemy_throw09.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(34, "data\\enemy_throw10.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(35, "data\\enemy_throw11.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(36, "data\\enemy_throw12.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(37, "data\\enemy_throw13.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(38, "data\\enemy_roll00.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(39, "data\\enemy_roll01.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(40, "data\\enemy_roll02.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(41, "data\\enemy_roll03.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(42, "data\\enemy_roll04.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(43, "data\\enemy_roll05.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(44, "data\\enemy_roll06.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(45, "data\\enemy_roll07.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(46, "data\\enemy_roll08.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(47, "data\\enemy_roll09.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(48, "data\\enemy_cut00.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(49, "data\\enemy_cut01.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(50, "data\\enemy_cut02.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(51, "data\\enemy_cut03.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(52, "data\\enemy_cut04.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(53, "data\\enemy_cut05.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(54, "data\\enemy_cut06.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(55, "data\\enemy_cut07.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(56, "data\\enemy_cut08.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(57, "data\\enemy_cut09.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(58, "data\\enemy_cut10.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(59, "data\\enemy_cut11.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(60, "data\\enemy_cut_jump00.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(61, "data\\enemy_cut_jump01.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(62, "data\\enemy_cut_jump02.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(63, "data\\enemy_cut_jump03.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(64, "data\\enemy_cut_jump04.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(65, "data\\enemy_cut_jump05.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(66, "data\\enemy_cut_jump06.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(67, "data\\enemy_cut_jump07.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(68, "data\\enemy_cut_jump08.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(69, "data\\enemy_cut_jump09.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(70, "data\\enemy_cut_jump10.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(71, "data\\enemy_cut_jump11.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(72, "data\\enemy_cut_jump12.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(73, "data\\enemy2_stand00.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(74, "data\\enemy2_stand00.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(75, "data\\enemy2_stand01.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(76, "data\\enemy2_stand01.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(77, "data\\enemy2_move00.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(78, "data\\enemy2_move01.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(79, "data\\enemy2_move02.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(80, "data\\enemy2_move03.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(81, "data\\enemy2_move04.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(82, "data\\enemy2_move05.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(83, "data\\enemy2_shot00.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(84, "data\\enemy2_shot01.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(85, "data\\enemy2_shot02.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(86, "data\\enemy2_shot03.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(87, "data\\enemy2_shot04.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(88, "data\\enemy2_shot05.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(89, "data\\enemy2_shot06.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(90, "data\\enemy2_shot07.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(91, "data\\enemy2_shot08.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(92, "data\\enemy2_shot09.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(93, "data\\enemy2_shot10.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(94, "data\\enemy3_stand00.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(95, "data\\enemy3_stand01.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(96, "data\\enemy3_stand02.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(97, "data\\enemy3_stand03.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(98, "data\\enemy3_stand04.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(99, "data\\enemy3_stand05.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(100, "data\\enemy3_move0_00.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(101, "data\\enemy3_move0_01.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(102, "data\\enemy3_move0_02.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(103, "data\\enemy3_move0_03.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(104, "data\\enemy3_move0_04.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(105, "data\\enemy3_move0_05.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(106, "data\\enemy3_move1_00.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(107, "data\\enemy3_move1_01.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(108, "data\\enemy3_move1_02.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(109, "data\\enemy3_move1_03.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(110, "data\\enemy3_move1_04.bmp"))
		return false;
	if (!g_EnemySprite.LoadFrame(111, "data\\enemy3_move1_05.bmp"))
		return false;


	if (!g_MyMissileSprite.InitSprite(32, 130, 130, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!g_MyMissileSprite.LoadFrame(0, "data\\mymissile00.bmp"))
		return false;
	if (!g_MyMissileSprite.LoadFrame(1, "data\\mymissile01.bmp"))
		return false;
	if (!g_MyMissileSprite.LoadFrame(2, "data\\mymissile02.bmp"))
		return false;
	if (!g_MyMissileSprite.LoadFrame(3, "data\\mymissile03.bmp"))
		return false;
	if (!g_MyMissileSprite.LoadFrame(4, "data\\mymissile04.bmp"))
		return false;
	if (!g_MyMissileSprite.LoadFrame(5, "data\\mymissile05.bmp"))
		return false;
	if (!g_MyMissileSprite.LoadFrame(6, "data\\mymissile06.bmp"))
		return false;
	if (!g_MyMissileSprite.LoadFrame(7, "data\\mymissile07.bmp"))
		return false;
	if (!g_MyMissileSprite.LoadFrame(8, "data\\mymissile08.bmp"))
		return false;
	if (!g_MyMissileSprite.LoadFrame(9, "data\\mymissile09.bmp"))
		return false;
	if (!g_MyMissileSprite.LoadFrame(10, "data\\mymissile10.bmp"))
		return false;
	if (!g_MyMissileSprite.LoadFrame(11, "data\\mymissile11.bmp"))
		return false;
	if (!g_MyMissileSprite.LoadFrame(12, "data\\mymissile12.bmp"))
		return false;
	if (!g_MyMissileSprite.LoadFrame(13, "data\\mymissile13.bmp"))
		return false;
	if (!g_MyMissileSprite.LoadFrame(14, "data\\mymissile14.bmp"))
		return false;
	if (!g_MyMissileSprite.LoadFrame(15, "data\\mymissile15.bmp"))
		return false;
	if (!g_ExplodingSprite.InitSprite(170, 190, 190, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!g_ExplodingSprite.LoadFrame(0, "data\\exploding00.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(1, "data\\exploding01.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(2, "data\\exploding02.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(3, "data\\exploding03.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(4, "data\\exploding04.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(5, "data\\exploding05.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(6, "data\\exploding06.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(7, "data\\exploding07.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(8, "data\\exploding08.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(9, "data\\exploding09.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(10, "data\\exploding10.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(11, "data\\exploding11.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(12, "data\\exploding12.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(13, "data\\exploding13.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(14, "data\\exploding14.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(15, "data\\exploding15.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(16, "data\\exploding16.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(17, "data\\exploding17.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(18, "data\\exploding18.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(19, "data\\exploding19.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(20, "data\\exploding20.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(21, "data\\exploding21.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(22, "data\\exploding22.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(23, "data\\exploding23.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(24, "data\\exploding24.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(25, "data\\exploding25.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(26, "data\\exploding26.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(27, "data\\exploding27.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(28, "data\\exploding28.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(29, "data\\exploding29.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(30, "data\\exploding30.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(31, "data\\exploding31.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(32, "data\\exploding32.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(33, "data\\exploding33.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(34, "data\\exploding34.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(35, "data\\exploding35.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(36, "data\\exploding36.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(37, "data\\exploding37.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(38, "data\\exploding38.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(39, "data\\exploding39.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(40, "data\\exploding40.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(41, "data\\exploding41.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(42, "data\\exploding42.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(43, "data\\exploding43.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(44, "data\\exploding44.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(45, "data\\exploding45.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(46, "data\\exploding46.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(47, "data\\exploding47.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(48, "data\\exploding48.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(49, "data\\exploding49.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(50, "data\\exploding50.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(51, "data\\exploding51.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(52, "data\\exploding52.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(53, "data\\exploding53.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(54, "data\\exploding54.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(55, "data\\exploding55.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(56, "data\\exploding56.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(57, "data\\exploding57.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(58, "data\\exploding58.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(59, "data\\exploding59.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(60, "data\\exploding60.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(61, "data\\exploding61.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(62, "data\\exploding62.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(63, "data\\exploding63.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(64, "data\\exploding64.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(65, "data\\exploding65.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(66, "data\\exploding66.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(67, "data\\exploding67.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(68, "data\\exploding68.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(69, "data\\exploding69.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(70, "data\\exploding70.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(71, "data\\exploding71.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(72, "data\\exploding72.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(73, "data\\exploding73.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(74, "data\\exploding74.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(75, "data\\exploding75.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(76, "data\\exploding76.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(77, "data\\exploding77.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(78, "data\\exploding78.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(79, "data\\exploding79.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(80, "data\\exploding80.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(81, "data\\exploding81.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(82, "data\\exploding82.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(83, "data\\exploding83.bmp"))
		return false;
	if (!g_ExplodingSprite.LoadFrame(84, "data\\exploding84.bmp"))
		return false;
	if (!g_EnemyMissileSprite.InitSprite(60, 130, 130, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(0, "data\\enemymissile00.bmp"))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(1, "data\\enemymissile01.bmp"))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(2, "data\\enemymissile02.bmp"))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(3, "data\\enemymissile03.bmp"))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(4, "data\\enemymissile04.bmp"))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(5, "data\\enemymissile05.bmp"))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(6, "data\\enemymissile06.bmp"))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(7, "data\\enemymissile07.bmp"))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(8, "data\\enemymissile08.bmp"))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(9, "data\\enemymissile09.bmp"))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(10, "data\\enemymissile10.bmp"))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(11, "data\\enemymissile11.bmp"))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(12, "data\\enemymissile12.bmp"))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(13, "data\\enemymissile13.bmp"))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(14, "data\\enemymissile14.bmp"))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(15, "data\\enemymissile15.bmp"))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(16, "data\\enemymissile16.bmp"))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(17, "data\\enemymissile17.bmp"))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(18, "data\\enemymissile18.bmp"))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(19, "data\\enemymissile19.bmp"))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(20, "data\\enemymissile20.bmp"))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(21, "data\\enemymissile21.bmp"))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(22, "data\\enemymissile22.bmp"))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(23, "data\\enemymissile23.bmp"))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(24, "data\\enemymissile24.bmp"))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(25, "data\\enemymissile25.bmp"))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(26, "data\\enemymissile26.bmp"))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(27, "data\\enemymissile27.bmp"))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(28, "data\\enemymissile28.bmp"))
		return false;
	if (!g_EnemyMissileSprite.LoadFrame(29, "data\\enemymissile29.bmp"))
		return false;


	if (!g_ItemSprite.InitSprite(20, 130, 130, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!g_ItemSprite.LoadFrame(0, "data\\item00.bmp"))
		return false;
	if (!g_ItemSprite.LoadFrame(1, "data\\item01.bmp"))
		return false;
	if (!g_ItemSprite.LoadFrame(2, "data\\item02.bmp"))
		return false;
	if (!g_ItemSprite.LoadFrame(3, "data\\item03.bmp"))
		return false;
	if (!g_ItemSprite.LoadFrame(4, "data\\item04.bmp"))
		return false;
	if (!g_ItemSprite.LoadFrame(5, "data\\item05.bmp"))
		return false;
	if (!g_ItemSprite.LoadFrame(6, "data\\item06.bmp"))
		return false;
	if (!g_ItemSprite.LoadFrame(7, "data\\item07.bmp"))
		return false;
	if (!g_ItemSprite.LoadFrame(8, "data\\item08.bmp"))
		return false;
	if (!g_ItemSprite.LoadFrame(9, "data\\item09.bmp"))
		return false;

	if (!g_BossSprite.InitSprite(216, 250, 250, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!g_BossSprite.LoadFrame(0, "data\\boss_stand00.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(1, "data\\boss_stand01.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(2, "data\\boss_stand02.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(3, "data\\boss_stand03.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(4, "data\\boss_stand04.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(5, "data\\boss_stand05.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(6, "data\\boss_stand06.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(7, "data\\boss_stand07.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(8, "data\\boss_stand08.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(9, "data\\boss_shot00.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(10, "data\\boss_shot01.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(11, "data\\boss_shot02.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(12, "data\\boss_shot03.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(13, "data\\boss_shot04.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(14, "data\\boss_shot05.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(15, "data\\boss_shot06.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(16, "data\\boss_shot07.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(17, "data\\boss_shot08.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(18, "data\\boss_shot09.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(19, "data\\boss_shot10.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(20, "data\\boss_shot11.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(21, "data\\boss_shot12.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(22, "data\\boss_shot13.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(23, "data\\boss_shot14.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(24, "data\\boss_shot15.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(25, "data\\boss_shot16.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(26, "data\\boss_move00.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(27, "data\\boss_move01.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(28, "data\\boss_move02.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(29, "data\\boss_move03.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(30, "data\\boss_move04.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(31, "data\\boss_move05.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(32, "data\\boss_move06.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(33, "data\\boss_move07.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(34, "data\\boss_move08.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(35, "data\\boss_move09.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(36, "data\\boss_move10.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(37, "data\\boss_move11.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(38, "data\\boss_move12.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(39, "data\\boss_move13.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(40, "data\\boss_move14.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(41, "data\\boss_move15.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(42, "data\\boss_move16.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(43, "data\\boss_standing_up00.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(44, "data\\boss_standing_up01.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(45, "data\\boss_standing_up02.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(46, "data\\boss_standing_up03.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(47, "data\\boss_standing_up04.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(48, "data\\boss_standing_up05.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(49, "data\\boss_standing_up06.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(50, "data\\boss_standing_up07.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(51, "data\\boss_standing_up08.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(52, "data\\boss_standing_up09.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(53, "data\\boss_standing_up10.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(54, "data\\boss_standing_up11.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(55, "data\\boss_standing_up12.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(56, "data\\boss_standing_up13.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(57, "data\\boss_standing_up14.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(58, "data\\boss_standing_up15.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(59, "data\\boss_standing_up16.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(60, "data\\boss_standing_up17.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(61, "data\\boss_standing_up18.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(62, "data\\boss_standing_up19.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(63, "data\\boss_standing_up20.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(64, "data\\boss_standing_up21.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(65, "data\\boss_standing_up22.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(66, "data\\boss_standing_up23.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(67, "data\\boss_standing_move00.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(68, "data\\boss_standing_move01.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(69, "data\\boss_standing_move02.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(70, "data\\boss_standing_move03.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(71, "data\\boss_standing_move04.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(72, "data\\boss_standing_move05.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(73, "data\\boss_standing_move06.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(74, "data\\boss_standing_move07.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(75, "data\\boss_standing_smashy00.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(76, "data\\boss_standing_smashy01.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(77, "data\\boss_standing_smashy02.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(78, "data\\boss_standing_smashy03.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(79, "data\\boss_standing_smashy04.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(80, "data\\boss_standing_smashy05.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(81, "data\\boss_standing_smashy06.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(82, "data\\boss_standing_smashy07.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(83, "data\\boss_standing_smashy08.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(84, "data\\boss_standing_smashy09.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(85, "data\\boss_standing_smashy10.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(86, "data\\boss_standing_smashy11.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(87, "data\\boss_standing_smashy12.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(88, "data\\boss_standing_smashy13.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(89, "data\\boss_standing_smashy14.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(90, "data\\boss_standing_smashy15.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(91, "data\\boss_move16.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(92, "data\\boss_move15.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(93, "data\\boss_move14.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(94, "data\\boss_move13.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(95, "data\\boss_move12.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(96, "data\\boss_move11.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(97, "data\\boss_move10.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(98, "data\\boss_move09.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(99, "data\\boss_move08.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(100, "data\\boss_move07.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(101, "data\\boss_move06.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(102, "data\\boss_move05.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(103, "data\\boss_move04.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(104, "data\\boss_move03.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(105, "data\\boss_move02.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(106, "data\\boss_move01.bmp"))
		return false;
	if (!g_BossSprite.LoadFrame(107, "data\\boss_move00.bmp"))
		return false;


	if (!g_BossMissileSprite.InitSprite(32, 130, 130, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!g_BossMissileSprite.LoadFrame(0, "data\\bossmissile00.bmp"))
		return false;
	if (!g_BossMissileSprite.LoadFrame(1, "data\\bossmissile01.bmp"))
		return false;
	if (!g_BossMissileSprite.LoadFrame(2, "data\\bossmissile02.bmp"))
		return false;
	if (!g_BossMissileSprite.LoadFrame(3, "data\\bossmissile03.bmp"))
		return false;
	if (!g_BossMissileSprite.LoadFrame(4, "data\\bossmissile04.bmp"))
		return false;
	if (!g_BossMissileSprite.LoadFrame(5, "data\\bossmissile05.bmp"))
		return false;
	if (!g_BossMissileSprite.LoadFrame(6, "data\\bossmissile06.bmp"))
		return false;
	if (!g_BossMissileSprite.LoadFrame(7, "data\\bossmissile07.bmp"))
		return false;
	if (!g_BossMissileSprite.LoadFrame(8, "data\\bossmissile08.bmp"))
		return false;
	if (!g_BossMissileSprite.LoadFrame(9, "data\\bossmissile09.bmp"))
		return false;
	if (!g_BossMissileSprite.LoadFrame(10, "data\\bossmissile10.bmp"))
		return false;
	if (!g_BossMissileSprite.LoadFrame(11, "data\\bossmissile11.bmp"))
		return false;
	if (!g_BossMissileSprite.LoadFrame(12, "data\\bossmissile12.bmp"))
		return false;
	if (!g_BossMissileSprite.LoadFrame(13, "data\\bossmissile13.bmp"))
		return false;
	if (!g_BossMissileSprite.LoadFrame(14, "data\\bossmissile14.bmp"))
		return false;
	if (!g_BossMissileSprite.LoadFrame(15, "data\\bossmissile15.bmp"))
		return false;

	if (!g_ClearSprite.InitSprite(2, 640, 480, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!g_ClearSprite.LoadFrame(0, "data\\clear.bmp"))
		return false;
	if (!g_DeadSprite.InitSprite(2, 640, 480, COLOR_KEY, g_lpDirectDrawObject))
		return false;
	if (!g_DeadSprite.LoadFrame(0, "data\\dead.bmp"))
		return false;
}