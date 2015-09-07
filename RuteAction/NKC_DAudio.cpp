//=============================================================================
//  DirectX Audio�֌W�̎���֐��Q
//  Copyright NKC Game Staff(�������̖��O)�@
//-----------------------------------------------------------------------------
/*---------- ���ʃw�b�_�t�@�C��common.h�ɁA�ȉ���錾����
#include <dmusici.h>    // DirectAudio���g�p�\�ɂ���
#include "NKC_DAudio.h"   // NKC_DAudio.cpp�𗘗p���邽�߂̃w�b�_�E�t�@�C��
----------*/
#include "NKC_DAudio.h"


// �O���[�o���ϐ�
/* ���̃\�[�X��������p�������� */
IDirectMusicPerformance8* g_pPerformance = NULL;// �p�t�H�[�}���X
IDirectMusicLoader8* g_pLoader = NULL;			// ���[�_�[
IDirectMusicAudioPath8* g_pDefAudioPath = NULL;	// �f�t�H���g�E�I�[�f�B�I�p�X
IDirectMusicAudioPath8* g_pAudioPath1;			// �I�[�f�B�I�E�p�X�P�i�K�v�ɉ����ė��p�j
IDirectMusicAudioPath8* g_pAudioPath2;			// �I�[�f�B�I�E�p�X�Q�i�K�v�ɉ����ė��p�j
IDirectMusicSegment8* g_pSeg[32];				// �Z�O�����g
IDirectMusicSegmentState8* g_pSegState[32];		// �Z�O�����g�̏��
MUSIC_TIME g_pSegLength[32];					// �Z�O�����g�̒���
long g_nMasterVolume;							// �}�X�^�[�E�{�����[��
long g_nVolume1;								// �I�[�f�B�I�E�p�X�P�p�̃{�����[���i�K�v�ɉ����ė��p�j
long g_nVolume2;								// �I�[�f�B�I�E�p�X�Q�p�̃{�����[���i�K�v�ɉ����ė��p�j

//-----------------------------------------------------------------------------------------
//DirectX Audio�̏�����
//-----------------------------------------------------------------------------------------
bool InitDXAudio(HWND hWnd)
{
	HRESULT hr;

	// �p�t�H�[�}���X�̍쐬
	hr = CoCreateInstance(
		CLSID_DirectMusicPerformance,
		NULL,
		CLSCTX_INPROC,
		IID_IDirectMusicPerformance8,
		(void**)&g_pPerformance );
	if (FAILED(hr)) {
		MessageBox(NULL,NULL,"DirectMusicPerformance8�I�u�W�F�N�g�̍쐬�Ɏ��s", MB_ICONERROR | MB_OK);
		return false;
	}

	// �p�t�H�[�}���X�̏�����
	hr = g_pPerformance->InitAudio( 
		NULL,								// IDirectMusic�C���^�[�t�F�C�X�͕s�v
		NULL,								// IDirectSound�C���^�[�t�F�C�X�͕s�v
		hWnd,								// �E�B���h�E�̃n���h��
		DMUS_APATH_SHARED_STEREOPLUSREVERB,	// �f�t�H���g�̃I�[�f�B�I�p�X�E�^�C�v
		64,									// �p�t�H�[�}���X�E�`�����l���̐�
		DMUS_AUDIOF_ALL,					// �V���Z�T�C�U�̋@�\
		NULL);								// �I�[�f�B�I�E�p�����[�^�ɂ̓f�t�H���g���g�p
	if (FAILED(hr)) {
		MessageBox(NULL,NULL,"DirectMusicPerformance8�̏������Ɏ��s", MB_ICONERROR | MB_OK);
		return false;
	}

	// �f�t�H���g�E�I�[�f�B�I�p�X�̎擾
	hr = g_pPerformance->GetDefaultAudioPath(&g_pDefAudioPath);
	if (FAILED(hr)) {
		MessageBox(NULL,NULL,"�f�t�H���g�E�I�[�f�B�I�p�X�̎擾�Ɏ��s", MB_ICONERROR | MB_OK);
		return false;
	}

	// �I�[�f�B�I�E�p�X���쐬�i�K�v�ɉ����ė��p����j
	/* �P�� */
	hr = g_pPerformance->CreateStandardAudioPath( 
		DMUS_APATH_DYNAMIC_STEREO,	// �p�X�̎�ށB
		64,							// �p�t�H�[�}���X �`�����l���̐��B
		TRUE,						// �����ŃA�N�e�B�u�ɂȂ�B
		&g_pAudioPath1 );			// �I�[�f�B�I�p�X���󂯎��|�C���^�B
	if (FAILED(hr))	MessageBox(NULL,NULL,"�I�[�f�B�I�E�p�X�ug_pAudioPath1�v�̍쐬�Ɏ��s", MB_ICONERROR | MB_OK);
	/* �Q�� */
	hr = g_pPerformance->CreateStandardAudioPath( 
		DMUS_APATH_DYNAMIC_STEREO,	// �p�X�̎�ށB
		64,							// �p�t�H�[�}���X �`�����l���̐��B
		TRUE,						// �����ŃA�N�e�B�u�ɂȂ�B
		&g_pAudioPath2 );			// �I�[�f�B�I�p�X���󂯎��|�C���^�B
	if (FAILED(hr))	MessageBox(NULL,NULL,"�I�[�f�B�I�E�p�X�ug_pAudioPath2�v�̍쐬�Ɏ��s", MB_ICONERROR | MB_OK);

	// �}�X�^�[�E�{�����[�����擾���A�����l�Ƃ��ăZ�b�g
	hr = g_pPerformance->SetGlobalParam(GUID_PerfMasterVolume, (void*)&g_nMasterVolume, sizeof(long));
	if (FAILED(hr)) {
		MessageBox(NULL,NULL,"�}�X�^�[�E�{�����[���̎擾�Ɏ��s\n", MB_ICONERROR | MB_OK);
		return false;
	}

	long nVolume1 = -200;	// �{�����[����-5.00db�ɂ���

	hr = g_pAudioPath1->SetVolume(nVolume1, 0);  // �{�����[���𑦎��ύX
	if (FAILED(hr)) {
		// �{�����[���̕ύX�Ɏ��s
	}

	g_nMasterVolume = 0;
	g_nVolume1 = g_nVolume2 = g_nMasterVolume;

	// ���[�_�[�̍쐬
	hr = CoCreateInstance(
		CLSID_DirectMusicLoader,
		NULL, 
		CLSCTX_INPROC,
		IID_IDirectMusicLoader8,
		(void**)&g_pLoader );
	if (FAILED(hr)) {
		MessageBox(NULL,NULL,"DirectMusicLoader8�I�u�W�F�N�g�̍쐬�Ɏ��s", MB_ICONERROR | MB_OK);
		return false;
	}

	// ���[�_�[�̏������i�����p�X���J�����g�E�f�B���N�g���ɐݒ�j
	CHAR strPath[MAX_PATH];
	DWORD cntPath = GetCurrentDirectory(MAX_PATH, strPath);
	if (cntPath==0 || MAX_PATH<cntPath) {
		MessageBox(NULL,NULL,"�J�����g�E�f�B���N�g���̎擾�Ɏ��s", MB_ICONERROR | MB_OK);
		return false;
	}

	// �}���`�E�o�C�g������UNICODE�ɕϊ�
	WCHAR wstrSearchPath[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, strPath, -1, wstrSearchPath, MAX_PATH);

	// ���[�_�[�Ɍ����p�X��ݒ�
	hr = g_pLoader->SetSearchDirectory(GUID_DirectMusicAllTypes, wstrSearchPath, FALSE);
	if (FAILED(hr)) {
		MessageBox(NULL,NULL,"�����p�X�̐ݒ�Ɏ��s", MB_ICONERROR | MB_OK);
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------------------
// �֐����FLoadSeg(int seg, char* FileName, bool flgMIDI)
// ���@���Fint seg �ǂݍ��܂������Z�O�����g�ԍ�
//         char FileName �ǂݍ��܂������t�@�C���̖��O
// �@�@�\�F�t�@�C�����ƃZ�O�����g�ԍ����w�肵�ăt�@�C������ǂݍ��܂���B
//-----------------------------------------------------------------------------------------
bool LoadSeg(int seg, char* FileName, bool flgMIDI,bool flgLOOP) 
{
	HRESULT	hr;

	// �Z�O�����g�̏�����
	if(g_pSeg[seg] != NULL) g_pSeg[seg] = NULL;

	// �Z�O�����g�Ƀt�@�C�������w�肵�ă��[�h����
	WCHAR wstrFileName[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, FileName, -1, wstrFileName, MAX_PATH);
	hr = g_pLoader->LoadObjectFromFile(
		CLSID_DirectMusicSegment,
		IID_IDirectMusicSegment8,
		wstrFileName,
		(LPVOID*)&g_pSeg[seg]);
	if (FAILED(hr)) {
		MessageBox(NULL,NULL,"MIDI�t�@�C���̃��[�h�Ɏ��s", MB_ICONERROR | MB_OK);
		return false;
	}

	// MIDI�t�@�C���̏ꍇ�A�p�����[�^��ݒ肷��
	// �i��x�X�^�[�g����ƁA��~�����s�����܂Ń��[�v������j
	if (flgMIDI) {
		hr = g_pSeg[seg]->SetParam(GUID_StandardMIDIFile, 0xFFFFFFFF, 0, 0, NULL);
		if (FAILED(hr)) {
			MessageBox(NULL,NULL,"�Z�O�����g�̃p�����[�^�ݒ�Ɏ��s", MB_ICONERROR | MB_OK);
			return false;
		}
	}
	if(flgLOOP){
		// �Z�O�����g�̒������擾
		hr = g_pSeg[seg]->GetLength(&g_pSegLength[seg]);
		if (FAILED(hr))	MessageBox(NULL,NULL,"�Z�O�����g�̒����̎擾�Ɏ��s", MB_ICONERROR | MB_OK);

		// �Z�O�����g�̃��[�v�񐔐ݒ�
		//�i�O�F���[�v�Ȃ��@DMUS_SEG_REPEAT_INFINITE�F�����I�Ɏ~�߂���܂Ń��[�v�j
		hr = g_pSeg[seg]->SetRepeats(DMUS_SEG_REPEAT_INFINITE);
		if (FAILED(hr))	MessageBox(NULL,NULL,"�Z�O�����g�̃��[�v�񐔂̐ݒ�Ɏ��s", MB_ICONERROR | MB_OK);
	}

	// �o���h�̃_�E�����[�h
	hr = g_pSeg[seg]->Download(g_pPerformance);
	if (FAILED(hr)) {
		MessageBox(NULL,NULL,"�Z�O�����g�̃o���h�_�E�����[�h�Ɏ��s", MB_ICONERROR | MB_OK);
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------------------
//	�Z�O�����g�̏I������
//-----------------------------------------------------------------------------------------
bool ReleaseDMSegment(void)
{
	HRESULT hr;

	// ���t��~
	if (g_pPerformance) {
		hr = g_pPerformance->Stop(NULL, NULL, 0, 0);
		if (FAILED(hr))	MessageBox(NULL,NULL,"�S�Z�O�����g�̉��t��~�Ɏ��s", MB_ICONERROR | MB_OK);
	}

	// ���F�f�[�^�̃A�����[�h�E�Z�O�����g�̊J��
	for (int i=0 ; i<32 ; i++) {
		if (g_pSeg[i]) {
			hr = g_pSeg[i]->Unload(g_pPerformance);
			if (FAILED(hr))	MessageBox(NULL,NULL,"�Z�O�����g�̉��F�f�[�^�̃A�����[�h�Ɏ��s", MB_ICONERROR | MB_OK);
		}
		RELEASE(g_pSegState[i]);
		RELEASE(g_pSeg[i]);
	}

	return true;
}

//-----------------------------------------------------------------------------------------
//	DirectX Audio�̏I������
//-----------------------------------------------------------------------------------------
bool ReleaseDXAudio(void)
{
	HRESULT hr;

	// ���[�_���J��
	RELEASE(g_pLoader);

	// �I�[�f�B�I�p�X���J��
	RELEASE(g_pDefAudioPath);
	RELEASE(g_pAudioPath1);
	RELEASE(g_pAudioPath2);

	// �p�t�H�[�}���X���J��
	if (g_pPerformance) {
		hr = g_pPerformance->CloseDown();
		if (FAILED(hr))	MessageBox(NULL,NULL,"IDirectMusicPerformance8::CloseDown�Ɏ��s", MB_ICONERROR | MB_OK);
	}
	RELEASE(g_pPerformance);

	return true;
}
