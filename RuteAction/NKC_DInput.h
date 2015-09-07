#pragma once

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>     // DirectInput���g�p�\�ɂ���
#define RELEASE(x) if(x!=NULL){x->Release();x=NULL;}


//-----------------------------------------------------------------------------
// File: NKC_DInput.h
// Desc: NKC_DInput.cpp�Ŏg�p������̂̂����A���̃\�[�X�ł��g�����̂�錾
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// �}�N��
//-----------------------------------------------------------------------------
/* Joypad��� */

#define BUTTON_UP		0x00000001l	// �����L�[��(dijs.IY<0)
#define BUTTON_DOWN		0x00000002l	// �����L�[��(dijs.IY>0)
#define BUTTON_LEFT		0x00000004l	// �����L�[��(dijs.IX<0)
#define BUTTON_RIGHT	0x00000008l	// �����L�[�E(dijs.IX>0)
#define BUTTON_A		0x00000010l	// �`�{�^��(dijs.rgbButtons[0]&0x80)
#define BUTTON_B		0x00000020l	// �a�{�^��(dijs.rgbButtons[1]&0x80)
#define BUTTON_C		0x00000040l	// �b�{�^��(dijs.rgbButtons[2]&0x80)
#define BUTTON_X		0x00000080l	// �w�{�^��(dijs.rgbButtons[3]&0x80)
#define BUTTON_Y		0x00000100l	// �x�{�^��(dijs.rgbButtons[4]&0x80)
#define BUTTON_Z		0x00000200l	// �y�{�^��(dijs.rgbButtons[5]&0x80)
#define BUTTON_L		0x00000400l	// �k�{�^��(dijs.rgbButtons[6]&0x80)
#define BUTTON_R		0x00000800l	// �q�{�^��(dijs.rgbButtons[7]&0x80)
#define BUTTON_START	0x00001000l	// �r�s�`�q�s�{�^��(dijs.rgbButtons[8]&0x80)
#define BUTTON_M		0x00002000l	// �l�{�^��(dijs.rgbButtons[9]&0x80)




#define JOYPADMAX		4			// �����ɐڑ�����W���C�p�b�h�̍ő吔���Z�b�g
//-----------------------------------------------------------------------------
// �\����
//-----------------------------------------------------------------------------
/* �}�E�X�|�C���^�̏�� */
typedef struct _MSTATE {
    RECT    moveRect;     // ��ʏ�œ�����͈�
    int     x;            // X���W
    int     y;            // Y���W
    bool    lButton;      // ���{�^��
    bool    rButton;      // �E�{�^��
    bool    cButton;      // �^�񒆃{�^��
	bool    exlButton;    // ���{�^��
    bool    exrButton;    // �E�{�^��
    bool    excButton;    // �^�񒆃{�^��	
    int     moveAdd;      // �ړ���
    RECT    imgRect;      // �}�E�X�p�摜��`
    int     imgWidth;     // �}�E�X�摜��
    int     imgHeight;    // �}�E�X�摜����
} MSTATE, *LPMSTATE;

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
/* �L�[�{�[�h�p */
extern BYTE					g_diKeyState[256];
extern BYTE					g_ExdiKeyState[256];
/* �W���C�p�b�h�p */
extern DWORD				g_diJoyState;
extern DWORD				g_diJoyState2[JOYPADMAX];
extern DWORD				g_ExdiJoyState2[JOYPADMAX];
extern int					g_diJoyCount;
/* �}�E�X�p */
extern DIMOUSESTATE			g_diMouseState;

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾 function-prototypes
//-----------------------------------------------------------------------------
/* DirectInput���� */
bool InitDInput(HINSTANCE);
void ReleaseDInput(void);
/* �L�[�{�[�h�p */
bool InitKeyboard(HWND hwnd);
void GetKeyboardState(void);
/* �W���C�p�b�h�p */
bool InitJoypad(void);
void GetJoypadState(void);
bool InitJoypad2(void);
void GetJoypadState2(void);
/* �}�E�X�p */
bool InitMouse(HWND hwnd);
void GetMouseState(void);
POINT& GetMouseLocalPos();
