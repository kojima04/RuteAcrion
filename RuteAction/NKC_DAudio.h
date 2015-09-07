#pragma once

#include <dmusici.h>
#define RELEASE(x) if(x!=NULL){x->Release();x=NULL;}
//-----------------------------------------------------------------------------
// File: NKC_DAudio.h
// Desc: NKC_DAudio.cpp�Ŏg�p������̂̂����A���̃\�[�X�ł��g�����̂�錾
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// �}�N��
//-----------------------------------------------------------------------------
#define VOLUME_MAX 1000
#define VOLUME_MIN -10000

//-----------------------------------------------------------------------------
// �\����
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------------------------------------------
extern IDirectMusicPerformance8* g_pPerformance;// �p�t�H�[�}���X
extern IDirectMusicLoader8* g_pLoader;			// ���[�_�[
extern IDirectMusicAudioPath8* g_pDefAudioPath;	// �f�t�H���g�E�I�[�f�B�I�p�X
extern IDirectMusicAudioPath8* g_pAudioPath1;	// �I�[�f�B�I�E�p�X�P�i�K�v�ɉ����č쐬�j
extern IDirectMusicAudioPath8* g_pAudioPath2;	// �I�[�f�B�I�E�p�X�Q�i�K�v�ɉ����č쐬�j
extern IDirectMusicSegment8* g_pSeg[32];		// �Z�O�����g
extern IDirectMusicSegmentState8* g_pSegState[32]; // �Z�O�����g�̏��
extern MUSIC_TIME g_pSegLength[32];				// �Z�O�����g�̒���
extern long g_nMasterVolume;					// �}�X�^�[�E�{�����[��
extern long g_nVolume1;							// �I�[�f�B�I�E�p�X�P�p�̃{�����[���i�K�v�ɉ����ė��p�j
extern long g_nVolume2;							// �I�[�f�B�I�E�p�X�Q�p�̃{�����[���i�K�v�ɉ����ė��p�j

//-----------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------------------------------------------
bool InitDXAudio(HWND hWnd);				// DirectAudio�̏�����
bool ReleaseDMSegment(void);		// �Z�O�����g�̍폜
bool ReleaseDXAudio(void);			// DirectAudio�I�u�W�F�N�g�̍폜
bool LoadSeg(int, char*, bool,bool);		// ���t�@�C���̓ǂݍ���