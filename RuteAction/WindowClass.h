#pragma once

#include "Common.h"
#include <string>

/**************************************************************************************************
		name	:	Window
		brief	:	�E�B���h�E���Ǘ�����
***************************************************************************************************/
class Window
{
public:

	//---------------------------------------------------------
	// �R���X�g���N�^
	//---------------------------------------------------------
	Window();
	//---------------------------------------------------------
	// �f�X�g���N�^
	//---------------------------------------------------------
	~Window();

	HRESULT	Init(HINSTANCE, int);					
	static LRESULT CALLBACK	WindowProc(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK	WindowProc_Box(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK	WindowProc_BlkInfo(HWND, UINT, WPARAM, LPARAM);


	HRESULT	MutexCreate();							//�~���[�e�b�N�X�J�n
	void	MutexDestroy();							//�~���[�e�b�N�X�I��

	void	Loop();									//���b�Z�[�W���[�v
	int		ReturnWinMain(){return msg.wParam;}		//WinMain�֐��ւ̕Ԃ�l
	
protected:
	HWND	hWnd;				//�E�B���h�E�n���h��
	MSG		msg;					//���b�Z�[�W
	HWND	hWnd_Box;				//�E�B���h�E�n���h��
	MSG		msg_Box;					//���b�Z�[�W
	HWND	hWnd_BlkInfo;				//�E�B���h�E�n���h��
	MSG		msg_BlkInfo;					//���b�Z�[�W
	HANDLE	hMutex;				//�~���[�e�b�N�X
	System	*Sys;
	bool 	isFullScreen;
public:
	HWND	GetWindowHandle(){return hWnd;}
	MSG		GetMessage(){return msg;}
};



