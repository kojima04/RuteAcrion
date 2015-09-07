#include <time.h>
#include <iostream>
#include <crtdbg.h>

#include "WindowClass.h"

MSTATE MState;  // �}�E�X���
HWND combo[COMBONUM];
HWND button[BUTTONNUM][2];
bool Clickflg[BUTTONNUM][2];
HWND combo_Block[COMBO_BLOCKNUM];
HWND edit_Block[EDIT_BLOCKNUM];
int G_SelectBlockRot;


LRESULT CALLBACK WndProc_Box(HWND hwnd , UINT msg , WPARAM wp , LPARAM lp);

/*************************************************************************************************
		name	:	Window::Window
		brief	:	�f�t�H���g�R���X�g���N�^
*************************************************************************************************/
Window::Window()
{
	// �t���X�N���[���ɂ��邩�ǂ����̔���
	isFullScreen = false;
}

Window::~Window()
{
	delete Sys;
}

/*************************************************************************************************
		name	:	WinMain
		brief	:	�A�v���P�[�V�����̃G���g���[�|�C���g
		return	:
**************************************************************************************************/
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{
	static Window wnd_main;

	   // COM �̏�����
    if ( FAILED(CoInitialize(NULL)) ) return (FALSE);
	
    // DirectX Audio�̏�����
//	if ( FAILED(InitDXAudio(wnd_main.GetWindowHandle())) ) return (FALSE);


	// DirectInput�I�u�W�F�N�g�̏�����
	if ( FAILED(InitDInput(hInstance)) ) return (FALSE);


	// Keyboard Object �̏�����
//	if ( FAILED(InitDXAudio(wnd_main.GetWindowHandle())) ) return (FALSE);
    if ( FAILED(InitKeyboard(wnd_main.GetWindowHandle())) ) return (FALSE);
	 // Joypad Object �̏�����
    if ( FAILED(InitJoypad2()) ) OutputDebugString("Joypad Not Found\n");

	// Mouse Object �̏�����
    if ( FAILED(InitMouse(wnd_main.GetWindowHandle())) ) return (FALSE);

	SetRect(&MState.moveRect, 10, 10, 630, 470);	// �}�E�X�J�[�\���̓����͈�
	MState.x = MState.moveRect.left;	// �}�E�X�J�[�\���̂w���W��������
	MState.y = MState.moveRect.top;	// �}�E�X�J�[�\���̂x���W��������
	MState.lButton = false;	// ���{�^���̏���������
	MState.rButton = false;	// �E�{�^���̏���������
	MState.cButton = false;	// �����{�^���̏���������
	MState.moveAdd = 2;	// �}�E�X�J�[�\���̈ړ��ʂ�ݒ�
	SetRect(&MState.imgRect, 400, 0, 420, 20);	// �}�E�X�J�[�\���摜�̋�`��ݒ�
	MState.imgWidth = MState.imgRect.right - MState.imgRect.left;	// �摜�̕����v�Z
	MState.imgHeight = MState.imgRect.bottom - MState.imgRect.top;	// �摜�̍������v�Z

	//���������[�N�𔭌�����
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	if(FAILED(wnd_main.Init(hInstance, nCmdShow)))
	{
		return 0;
	}
	
	wnd_main.Loop();
	
	return static_cast<int>(wnd_main.GetMessage().wParam);
}

/***********************************************************************************************
		name	:	WindowProc
		brief	:	�v���V�[�W��
		return	:
***********************************************************************************************/
HRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
		srand((unsigned)timeGetTime());
		break;
	
	case WM_DESTROY:
		ReleaseDInput();
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		// �ǂ̃L�[�������ꂽ������
		switch(wParam)
		{
		case VK_ESCAPE:
			// WM_DESTROY���b�Z�[�W�𑗐M
			SendMessage(hWnd, WM_DESTROY, wParam, lParam);
			break;

		default:
			break;
		}
		break;


	case WM_SETCURSOR:
		while(1)
		{
			if(ShowCursor(false) <= 0)
				break;
		}
		return 0;
		
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

/**************************************************************************************************
		name	:	MessageLoop
		brief	:	���b�Z�[�W���[�v
		return	:	HRESULT
***************************************************************************************************/
void Window::Loop()
{	
	// ���b�Z�[�W��������ѕ`�惋�[�v
	while(true)
	{
		// �������ׂ����b�Z�[�W������
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// PostQuitMessage���Ă΂ꂽ
			if(msg.message == WM_QUIT)
			{
				// ���[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		// �������ׂ����b�Z�[�W���Ȃ��ꍇ
		else
		{
				//���݂̎��Ԃ��擾
			Sys->nowTime = timeGetTime();

			//�o�ߎ��Ԃ�16ms���傫���Ȃ�
			if(Sys->nowTime - Sys->beforeTime > 8){
				//�ߋ��̎����̍X�V
				Sys->beforeTime = Sys->nowTime;
				if(FAILED(g_diJoyState2[0]))
					g_diJoyCount = 1;
				if(FAILED(g_diJoyState2[1]))
					g_diJoyCount = 2;
				
				GetCursorPos(&m_MouseLocalPos);
				ScreenToClient(hWnd,&m_MouseLocalPos);
				Sys->SetInputState();
				Sys->Input();
				Sys->Main();

			}
			// �E�B���h�E�������Ă��鎞�����`�悷�邽�߂̏���
			WINDOWPLACEMENT wndpl;
			GetWindowPlacement(hWnd, &wndpl);	// �E�B���h�E�̏�Ԃ��擾
			if((wndpl.showCmd != SW_HIDE) &&
				(wndpl.showCmd != SW_MINIMIZE) &&
				(wndpl.showCmd != SW_SHOWMINIMIZED) &&
				(wndpl.showCmd != SW_SHOWMINNOACTIVE))
			{//--- �����ŕ`�� ---//	
				Sys->renderer.RenderBGN();
				Sys->Paint();
				Sys->renderer.RenderEND();
			}
		}
		Sleep(1);
	}
}

/*************************************************************************************************
		name	:	InitApp
		brief	:	�E�B���h�E�̐���
		return	:	E_FAIL	���s
************************************************************************************************/	
HRESULT Window::Init(HINSTANCE hInstance, INT nCmdShow)
{	
	#ifdef STAGEEDIT

	WNDCLASS wc_box;

	wc_box.style		= CS_HREDRAW | CS_VREDRAW;
	wc_box.lpfnWndProc	= WindowProc_Box;
	wc_box.cbClsExtra	= wc_box.cbWndExtra	= 0;
	wc_box.hInstance		= hInstance;
	wc_box.hIcon		= LoadIcon(NULL , IDI_APPLICATION);
	wc_box.hCursor		= LoadCursor(NULL , IDC_ARROW);
	wc_box.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc_box.lpszMenuName	= NULL;
	wc_box.lpszClassName	= TEXT("STAGE Window");

	if (!RegisterClass(&wc_box)) 
		return -1;

	hWnd_Box = CreateWindow(
			TEXT("STAGE Window") , TEXT("STAGE Window") ,
			WS_OVERLAPPEDWINDOW | WS_VISIBLE ,
			1000, 						// �E�C���h�E�̉������̈ʒu
			500, 						// �E�C���h�E�̏c�����̈ʒu
			BOX_WIDTH, 			// �E�C���h�E�̕�
			BOX_HEIGHT,			// �E�C���h�E�̍���
			NULL , NULL , hInstance , NULL
	);

	WNDCLASS wc_blkinf;

	wc_blkinf.style		= CS_HREDRAW | CS_VREDRAW;
	wc_blkinf.lpfnWndProc	= WindowProc_BlkInfo;
	wc_blkinf.cbClsExtra	= wc_blkinf.cbWndExtra	= 0;
	wc_blkinf.hInstance		= hInstance;
	wc_blkinf.hIcon		= LoadIcon(NULL , IDI_APPLICATION);
	wc_blkinf.hCursor		= LoadCursor(NULL , IDC_ARROW);
	wc_blkinf.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc_blkinf.lpszMenuName	= NULL;
	wc_blkinf.lpszClassName	= TEXT("Box Window");

	if (!RegisterClass(&wc_blkinf)) 
		return -1;

	hWnd_BlkInfo = CreateWindow(
			TEXT("Box Window") , TEXT("Box Window") ,
			WS_OVERLAPPEDWINDOW | WS_VISIBLE ,
			1000, 						// �E�C���h�E�̉������̈ʒu
			200, 						// �E�C���h�E�̏c�����̈ʒu
			BLKINF_WIDTH, 			// �E�C���h�E�̕�
			BLKINF_HEIGHT,			// �E�C���h�E�̍���
			NULL , NULL , hInstance , NULL
	);

	#endif
	
	char clsName[] = "RuteAction";	// �E�B���h�E�N���X��

	// �E�B���h�E�N���X�̏�����
	WNDCLASSEX wcex = {
		sizeof(WNDCLASSEX),					// ���̍\���̂̃T�C�Y
		NULL,								// �E�B���h�E�̃X�^�C��
		WindowProc, 						// ���b�Z�[�W�����֐��̓o�^
		0,									// �ʏ�͎g��Ȃ��̂ŏ��0
		0,									// �ʏ�͎g��Ȃ��̂ŏ��0
		hInstance,							// �C���X�^���X�ւ̃n���h��
		LoadIcon(NULL, IDI_APPLICATION),	// �A�C�R��
		LoadCursor(NULL, IDC_ARROW),		// �J�[�\���̌`
		(HBRUSH)COLOR_BACKGROUND+1,			// �w�i
		NULL,								// ���j���[�Ȃ�
		clsName,							// �N���X���̎w��
		NULL								// ���A�C�R���i�Ȃ��j
	};

	// �E�B���h�E�N���X�̓o�^
	if(RegisterClassEx(&wcex) == 0)
	{
		MessageBox(NULL, "�E�B���h�E�N���X�̍쐬�Ɏ��s���܂���.\n�v���O�������I�����܂�.", NULL, MB_OK | MB_ICONERROR);
		return 0;	// �o�^���s
	}

	// �E�B���h�E�̍쐬
	if(isFullScreen)	// �t���X�N���[��
	{
		hWnd = CreateWindow(
					clsName, 				// �o�^����Ă���N���X��
					WINDOW_NAME, 			// �E�C���h�E��
					WS_POPUP,				// �E�C���h�E�X�^�C���i�|�b�v�A�b�v�E�C���h�E���쐬�j
					0, 						// �E�C���h�E�̉������̈ʒu
					0, 						// �E�C���h�E�̏c�����̈ʒu
					CLIENT_WIDTH, 			// �E�C���h�E�̕�
					CLIENT_HEIGHT,			// �E�C���h�E�̍���
					NULL,					// �e�E�C���h�E�̃n���h���i�ȗ��j
					NULL,					// ���j���[��q�E�C���h�E�̃n���h��
					hInstance, 				// �A�v���P�[�V�����C���X�^���X�̃n���h��
					NULL					// �E�C���h�E�̍쐬�f�[�^
				);

		if(hWnd == NULL)
		{
			MessageBox(NULL, "�E�B���h�E�̐����Ɏ��s���܂���.\n�v���O�������I�����܂�.", NULL, MB_OK | MB_ICONERROR);
			return 0;	// �E�B���h�E�̐����Ɏ��s
		}
	}
	else				// �E�B���h�E
	{
		hWnd = CreateWindow(
					clsName,					// �o�^����Ă���N���X��
					WINDOW_NAME,				// �E�C���h�E��
					WS_OVERLAPPED | WS_SYSMENU,	// �E�C���h�E�X�^�C���i�I�[�o�[���b�v�E�B���h�E���쐬�j
					CW_USEDEFAULT,				// �E�C���h�E�̉������̈ʒu�i�f�t�H���g�̎w����g���j
					CW_USEDEFAULT,				// �E�C���h�E�̏c�����̈ʒu�i�f�t�H���g�̎w����g���j
					CW_USEDEFAULT,				// �E�C���h�E�̕��i�f�t�H���g�̎w����g���j
					CW_USEDEFAULT,				// �E�C���h�E�̍����i�f�t�H���g�̎w����g���j
					NULL,						// �e�E�C���h�E�̃n���h���i�ȗ��j
					NULL,						// ���j���[��q�E�C���h�E�̃n���h��
					hInstance,					// �A�v���P�[�V�����C���X�^���X�̃n���h��
					NULL						// �E�C���h�E�̍쐬�f�[�^
					);

		if(hWnd == NULL)
		{
			MessageBox(NULL, "�E�B���h�E�̐����Ɏ��s���܂���.\n�v���O�������I�����܂�.", NULL, MB_OK | MB_ICONERROR);
			return 0;	// �E�B���h�E�̐����Ɏ��s
		}
	// �E�B���h�E�T�C�Y���Đݒ肷��
		RECT rect;
		int ww, wh;
		int cw, ch;
		// �N���C�A���g�̈�̊O�̕����v�Z
		GetClientRect(hWnd, &rect);		// �N���C�A���g�����̃T�C�Y�̎擾
		cw = rect.right - rect.left;	// �N���C�A���g�̈�O�̉������v�Z
		ch = rect.bottom - rect.top;	// �N���C�A���g�̈�O�̏c�����v�Z

		// �E�C���h�E�S�̂̉����̕����v�Z
		GetWindowRect(hWnd, &rect);		// �E�C���h�E�S�̂̃T�C�Y�擾
		ww = rect.right - rect.left;	// �E�C���h�E�S�̂̕��̉������v�Z
		wh = rect.bottom - rect.top;	// �E�C���h�E�S�̂̕��̏c�����v�Z
		ww = ww - cw;					// �N���C�A���g�̈�ȊO�ɕK�v�ȕ�
		wh = wh - ch;					// �N���C�A���g�̈�ȊO�ɕK�v�ȍ���

		// �E�B���h�E�T�C�Y�̍Čv�Z
		ww = CLIENT_WIDTH + ww;			// �K�v�ȃE�C���h�E�̕�
		wh = CLIENT_HEIGHT + wh;		// �K�v�ȃE�C���h�E�̍���

		// �E�C���h�E�T�C�Y�̍Đݒ�
		SetWindowPos(hWnd, HWND_TOP, 0, 0, ww, wh, SWP_NOMOVE);

	}

	Sys = new System(isFullScreen,hWnd);


	// �E�B���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	ValidateRect(hWnd, 0);				//WM_PAINT�𖳌���

	HWND hWnd2 = NULL;

	return S_OK;
}


/************************************************************************************************
		name	:	InitMutex
		brief	:	�~���[�e�b�N�X�J�n�����B���d�N���̖h�~
		return	:	HRESULT
************************************************************************************************/
HRESULT Window::MutexCreate()
{
	hMutex = CreateMutex(NULL, TRUE, TEXT("Mutex"));
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, TEXT("���d�N�����m�F���܂���"), 0, MB_OK | MB_ICONERROR);
		return E_FAIL;
	}
	
	return S_OK;
}

/***********************************************************************************************
		name	:	FinaMutex
		brief	:	�~���[�e�b�N�X�I������
**************************************************************************************************/
VOID Window::MutexDestroy()
{
	CloseHandle(hMutex);
}


HRESULT CALLBACK Window::WindowProc_Box(HWND hwnd , UINT msg , WPARAM wp , LPARAM lp) {
	
	switch (msg) {
	case WM_DESTROY:
		// �A�v���P�[�V�����̏I����ʒm
		// DirectInput�I�u�W�F�N�g�̊J��
	    ReleaseDInput();
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		for(int i = 0;i < COMBONUM;i++)
		{
		combo[i] = CreateWindow(
			TEXT("COMBOBOX") , NULL , 
			WS_CHILD | WS_VISIBLE | CBS_SORT | CBS_DROPDOWN  , 
	//		100 + 140*(x > 2), 66*x - 198 * (x>2), 50 , 300 , hwnd , (HMENU)1 ,
			BOX_WIDTH/4*(1 + 2*(i > 2)),
			BOX_HEIGHT/6*(2*(i % 3)), BOX_WIDTH/4 , 300 , hwnd , (HMENU)i ,
			((LPCREATESTRUCT)(lp))->hInstance , NULL
		);
		for (int x = 0 ; x < 1 ; x++)
			SendMessage(combo[i] , CB_ADDSTRING , 0 , (LPARAM)"NOTHING");
			SendMessage(combo[i] , CB_SETCURSEL  , 0 , 0);
		}

		for(int i = 0;i < BUTTONNUM;i++)
		{
			button[i][0] = CreateWindow( 
			"button",                           // �X�^�e�B�b�N�R���g���[���̃N���X��
			"SAVE",                               // �\�����镶����
			BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
												// �E�C���h�E�X�^�C��
			BOX_WIDTH/4*(2),
			BOX_HEIGHT/6*(2*(i % 3)),                               // ����̍��W
			BOX_WIDTH/4 , 25 ,
			hwnd,                            // �e�E�C���h�E�̃E�C���h�E�n���h��
			(HMENU)i,                               // ���j���[�n���h���BNULL�ł悢�B
			((LPCREATESTRUCT)(lp))->hInstance,                          // �A�v���P�[�V�����̃C���X�^���X�n���h���B
			NULL                                // �E�C���h�E�쐬�f�[�^�BNULL�ł悢
			) ;

			button[i][1] = CreateWindow( 
			"button",                           // �X�^�e�B�b�N�R���g���[���̃N���X��
			"LOAD",                               // �\�����镶����
			BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE,
												// �E�C���h�E�X�^�C��
			BOX_WIDTH/4*(3),
			BOX_HEIGHT/6*(2*(i % 3)),                               // ����̍��W
			BOX_WIDTH/4 , 25 ,
			hwnd,                            // �e�E�C���h�E�̃E�C���h�E�n���h��
			(HMENU)(i + BUTTONNUM),                               // ���j���[�n���h���BNULL�ł悢�B
			((LPCREATESTRUCT)(lp))->hInstance,                          // �A�v���P�[�V�����̃C���X�^���X�n���h���B
			NULL                                // �E�C���h�E�쐬�f�[�^�BNULL�ł悢
			) ;
		}


		return 0;

	case WM_COMMAND:
		for(int i = 0;i < BUTTONNUM;i++)
		{
			if( LOWORD(wp) == i)
			{
				if(HIWORD( wp) == BN_CLICKED)
				{
					// �N���b�N���ꂽ�Ƃ��̏���
					Clickflg[i][0] = true;
				}
			}
			if( LOWORD(wp) == i + BUTTONNUM)
			{
				if(HIWORD( wp) == BN_CLICKED)
				{
					// �N���b�N���ꂽ�Ƃ��̏���
					Clickflg[i][1] = true;
				}
			}
		}


	case WM_SETCURSOR:
		while(1)
		{
			if(ShowCursor(true) > 0)
				break;
		}
		return 0;
	
	}
	return DefWindowProc(hwnd , msg , wp , lp);

}

HRESULT CALLBACK Window::WindowProc_BlkInfo(HWND hwnd , UINT msg , WPARAM wp , LPARAM lp) {
	char strText[64]={0};

	switch (msg) {
	case WM_DESTROY:
		// �A�v���P�[�V�����̏I����ʒm
		// DirectInput�I�u�W�F�N�g�̊J��
	    ReleaseDInput();
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		combo_Block[0] = CreateWindow(
			TEXT("COMBOBOX") , NULL , 
			WS_CHILD | WS_VISIBLE | CBS_SORT | CBS_DROPDOWN  , 
			0,
			0, BLKINF_WIDTH/4 , 300 , hwnd , (HMENU)1 ,
			((LPCREATESTRUCT)(lp))->hInstance , NULL
		);
		combo_Block[1] = CreateWindow(
			TEXT("COMBOBOX") , NULL , 
			WS_CHILD | WS_VISIBLE | CBS_SORT | CBS_DROPDOWN  , 
			BLKINF_WIDTH/4,
			0, BLKINF_WIDTH/4 , 300 , hwnd , (HMENU)1 ,
			((LPCREATESTRUCT)(lp))->hInstance , NULL
		);

			for (int x = 0 ; x < 1 ; x++)
			{
				SendMessage(combo_Block[x] , CB_ADDSTRING , 0 , (LPARAM)"NULL");
				SendMessage(combo_Block[x] , CB_SETCURSEL  , 0 , 0);
			}
		
		edit_Block[0] = CreateWindow(
			TEXT("EDIT") ,
			NULL,
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT ,
			0,
			40, BLKINF_WIDTH/4 , 25 , hwnd , (HMENU)1 ,
			((LPCREATESTRUCT)(lp))->hInstance , NULL
		);
		SetWindowText(edit_Block[0],(LPCTSTR)"0") ;

		return 0;		

	case WM_SETCURSOR:
		while(1)
		{
			if(ShowCursor(true) > 0)
				break;
		}
		return 0;
	case WM_COMMAND:
		if (LOWORD(wp) == 1 && HIWORD(wp) == EN_UPDATE) {
			//strText = new char;
			GetWindowText(edit_Block[0] , strText,64);
			G_SelectBlockRot = atoi(strText); 
			//delete strText;
		}
		return 0;

		
	}
	return DefWindowProc(hwnd , msg , wp , lp);

}
