//=============================================================
// Window.cpp
// WinMain�֐������,�E�C���h�E�v���V�[�W���̎���
//=============================================================

//=============================================================
// include
//=============================================================
#include <crtdbg.h>		// Memory���[�N�����֐��p
#include <windows.h>

#include "Common.h"


//=============================================================
// �}�N����`
//=============================================================

//=============================================================
// �񋓑̒�`
//=============================================================

// �V�[���؂�ւ��p
enum eSceneObject
{
	NONE,
	TRIANGLE,
	SQUARE,
	TEXTURE,
	ROBOT,
};

//=============================================================
// �֐��v���g�^�C�v�錾
//=============================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc_Box(HWND hwnd , UINT msg , WPARAM wp , LPARAM lp);

//=============================================================
// �O���[�o���ϐ���`
//=============================================================
eSceneObject g_key = NONE;
MSTATE MState;  // �}�E�X���
HWND combo[6];

LPCTSTR strBlock[] = {
	TEXT("NULL") ,
};




// ��O�N���X
class KeyNumberException
{
private:
	eSceneObject m_key;
public:
	KeyNumberException(eSceneObject key)
		: m_key(key) {}
	eSceneObject getErrorKey() const { return m_key; }
};

//-------------------------------------------------------------
// WinMain�֐�
// ����
//		hInstance     : ���݂̃C���X�^���X�̃n���h��
//		hPrevInstance : �ȑO�̃C���X�^���X�̃n���h��
//		lpCmdLine	  : �R�}���h���C���p�����[�^
//		nCmdShow	  : �E�B���h�E�̕\�����
// �߂�l
//		����������0�ȊO�̒l
//-------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

	


	// �E�B���h�E�쐬
	// �t���X�N���[���ɂ��邩�ǂ����̔���
	bool isFullScreen = false;
	HWND hWnd = NULL;
	char clsName[] = "RuteAction";	// �E�B���h�E�N���X��
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	// ���d�N���̖h�~
	HANDLE hMutex = NULL;
	const char mutexName[] = "RuteAction";
	hMutex = CreateMutex(NULL, TRUE, mutexName);
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL, "���d�N�����m�F���܂���.\n�v���O�������I�����܂�.", "�G���[�F���d�N��", MB_OK);
		return 0;
	}


    // COM �̏�����
    if ( FAILED(CoInitialize(NULL)) ) return (FALSE);
	
    // DirectX Audio�̏�����
    if ( FAILED(InitDXAudio(hWnd)) ) return (FALSE);


	// DirectInput�I�u�W�F�N�g�̏�����
	if ( FAILED(InitDInput(hInstance)) ) return (FALSE);
	// Keyboard Object �̏�����
    if ( FAILED(InitKeyboard(hWnd)) ) return (FALSE);
	 // Joypad Object �̏�����
    if ( FAILED(InitJoypad2()) ) OutputDebugString("Joypad Not Found\n");

	// Mouse Object �̏�����
    if ( FAILED(InitMouse(hWnd)) ) return (FALSE);

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

	HWND hwnd;
	
	WNDCLASS winc;

	winc.style		= CS_HREDRAW | CS_VREDRAW;
	winc.lpfnWndProc	= WndProc_Box;
	winc.cbClsExtra	= winc.cbWndExtra	= 0;
	winc.hInstance		= hInstance;
	winc.hIcon		= LoadIcon(NULL , IDI_APPLICATION);
	winc.hCursor		= LoadCursor(NULL , IDC_ARROW);
	winc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);
	winc.lpszMenuName	= NULL;
	winc.lpszClassName	= TEXT("KITTY");

	if (!RegisterClass(&winc)) return -1;

	hwnd = CreateWindow(
			TEXT("KITTY") , TEXT("Kitty on your lap") ,
			WS_OVERLAPPEDWINDOW | WS_VISIBLE ,
			1000, 						// �E�C���h�E�̉������̈ʒu
			300, 						// �E�C���h�E�̏c�����̈ʒu
			300, 			// �E�C���h�E�̕�
			200,			// �E�C���h�E�̍���
			NULL , NULL , hInstance , NULL
	);


	// Memory���[�N�����֐�
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

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

		// �}�E�X�J�[�\�����\���ɂ���
		//ShowCursor(FALSE);

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

	// �E�B���h�E�̕\��
    ShowWindow(hWnd, nShowCmd);
	UpdateWindow(hWnd);

	// WM_PAINT���Ă΂�Ȃ��悤�ɂ���
	ValidateRect(hWnd, 0);

	System *Sys = new System(isFullScreen,hWnd,hwnd);

	eSceneObject prev_key = NONE;


	



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
				
				
				Sys->Paint();
			}
		}

		// OS�ɐ���𖾂��n�����߂̃X���[�v
		Sleep(1);
	}

	


	//============//
	//  �������  //
	//============//
	
	delete Sys;

	 // �Z�O�����g�̊J������
    ReleaseDMSegment();

    // DirectX Audio�̊J������
    ReleaseDXAudio();


	// �~���[�e�b�N�X�I�u�W�F�N�g�̉��(���d�N���̖h�~�p)
	CloseHandle(hMutex);
	hMutex = NULL;

	return static_cast<int>(msg.wParam);
}

//-------------------------------------------------------------
// ���b�Z�[�W�����p�R�[���o�b�N�֐�
// ����
//		hWnd	: �E�B���h�E�n���h��
//		msg		: ���b�Z�[�W
//		wParam	: ���b�Z�[�W�̍ŏ��̃p�����[�^
//		lParam	: ���b�Z�[�W��2�Ԗڂ̃p�����[�^
// �߂�l
//		���b�Z�[�W��������
//-------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
	{
	case WM_DESTROY:
		// �A�v���P�[�V�����̏I����ʒm
		// DirectInput�I�u�W�F�N�g�̊J��
	    ReleaseDInput();
		PostQuitMessage(0);
		break;

	// �L�[�������ꂽ
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
		ShowCursor(false);
		return 0;

	//case WM_LBUTTONDOWN:

	
	//case WM_MOUSEMOVE:	// �}�E�X�J�[�\�����ړ������Ƃ��ɑ����Ă���
	//	// �ړ���̍��W���擾
	//	m_MouseLocalPos.x = LOWORD( lParam );
	//	m_MouseLocalPos.y = HIWORD( lParam );

	//	return 0;


	case WM_KILLFOCUS:
		
		m_MouseLocalPos.x = -10000;
		m_MouseLocalPos.y = -10000;
		MState.cButton = MState.rButton = MState.lButton = false;
		//SendMessage(hWnd, WM_DESTROY, wParam, lParam);
			
		return 0;

	// �f�t�H���g�̏���
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	}

	return 0;
}




LRESULT CALLBACK WndProc_Box(HWND hwnd , UINT msg , WPARAM wp , LPARAM lp) {
	
	int i;
	PSTR strText;

	switch (msg) {
	case WM_DESTROY:
		// �A�v���P�[�V�����̏I����ʒm
		// DirectInput�I�u�W�F�N�g�̊J��
	    ReleaseDInput();
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		for(int x = 0;x < 6;x++)
		{
		combo[x] = CreateWindow(
			TEXT("COMBOBOX") , NULL , 
			WS_CHILD | WS_VISIBLE | CBS_SORT | CBS_DROPDOWN  , 
			100 + 140*(x > 2), 66*x - 198 * (x>2), 50 , 300 , hwnd , (HMENU)1 ,
			((LPCREATESTRUCT)(lp))->hInstance , NULL
		);
		for (i = 0 ; i < 5 ; i++)
			SendMessage(combo[x] , CB_ADDSTRING , 0 , (LPARAM)strBlock[i]);
			SendMessage(combo[x] , CB_SETCURSEL  , 0 , 0);
		}
		strText = (PSTR)malloc(
				SendMessage(combo[1] , CB_GETLBTEXTLEN ,
				SendMessage(combo[1] , CB_GETCURSEL , 0 , 0) , 0
				) + 1
			);
				SendMessage(combo[1] , CB_GETLBTEXT , 
				SendMessage(combo[1] , CB_GETCURSEL , 0 , 0) ,
				(LPARAM)strText
			);
			SetWindowText(hwnd , (LPCTSTR)strText);
			free(strText);

		return 0;		
	case WM_COMMAND:

		if (HIWORD(wp) == CBN_SELCHANGE) {

			strText = (PSTR)malloc(
				SendMessage(combo[1] , CB_GETLBTEXTLEN ,
				SendMessage(combo[1] , CB_GETCURSEL , 0 , 0) , 0
				) + 1
			);
				SendMessage(combo[1] , CB_GETLBTEXT , 
				SendMessage(combo[1] , CB_GETCURSEL , 0 , 0) ,
				(LPARAM)strText
			);
			SetWindowText(hwnd , (LPCTSTR)strText);
			free(strText);
		}

		return 0;


	
	case WM_SETCURSOR:
		ShowCursor(true);
		return 0;
	
	}
	return DefWindowProc(hwnd , msg , wp , lp);

}
