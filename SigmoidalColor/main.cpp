#include <Windows.h>
#include <mathimf.h>

#if _DEBUG
#include <cilk\cilk_stub.h>
#else
#include <cilk\cilk.h>
#endif

#include <cilk\reducer_vector.h>
#include <ipp.h>
#include <vector>
#include "filter.h"

//---------------------------------------------------------------------
//		�t�B���^�\���̒�`
//---------------------------------------------------------------------
/*#define	TRACK_N	3														//	�g���b�N�o�[�̐�
TCHAR	*track_name[] = { "track0", "track1", "track2" };	//	�g���b�N�o�[�̖��O
int		track_default[] = { 0, 0, 0 };	//	�g���b�N�o�[�̏����l
int		track_s[] = { -999, -999, -999 };	//	�g���b�N�o�[�̉����l
int		track_e[] = { +999, +999, +999 };	//	�g���b�N�o�[�̏���l

#define	CHECK_N	2														//	�`�F�b�N�{�b�N�X�̐�
TCHAR	*check_name[] = { "check0", "check1" };				//	�`�F�b�N�{�b�N�X�̖��O
int		check_default[] = { 0, 0 };				//	�`�F�b�N�{�b�N�X�̏����l (�l��0��1)

FILTER_DLL filter = {
	FILTER_FLAG_EX_INFORMATION,	//	�t�B���^�̃t���O
	//	FILTER_FLAG_ALWAYS_ACTIVE		: �t�B���^����ɃA�N�e�B�u�ɂ��܂�
	//	FILTER_FLAG_CONFIG_POPUP		: �ݒ���|�b�v�A�b�v���j���[�ɂ��܂�
	//	FILTER_FLAG_CONFIG_CHECK		: �ݒ���`�F�b�N�{�b�N�X���j���[�ɂ��܂�
	//	FILTER_FLAG_CONFIG_RADIO		: �ݒ�����W�I�{�^�����j���[�ɂ��܂�
	//	FILTER_FLAG_EX_DATA				: �g���f�[�^��ۑ��o����悤�ɂ��܂��B
	//	FILTER_FLAG_PRIORITY_HIGHEST	: �t�B���^�̃v���C�I���e�B����ɍŏ�ʂɂ��܂�
	//	FILTER_FLAG_PRIORITY_LOWEST		: �t�B���^�̃v���C�I���e�B����ɍŉ��ʂɂ��܂�
	//	FILTER_FLAG_WINDOW_THICKFRAME	: �T�C�Y�ύX�\�ȃE�B���h�E�����܂�
	//	FILTER_FLAG_WINDOW_SIZE			: �ݒ�E�B���h�E�̃T�C�Y���w��o����悤�ɂ��܂�
	//	FILTER_FLAG_DISP_FILTER			: �\���t�B���^�ɂ��܂�
	//	FILTER_FLAG_EX_INFORMATION		: �t�B���^�̊g������ݒ�ł���悤�ɂ��܂�
	//	FILTER_FLAG_NO_CONFIG			: �ݒ�E�B���h�E��\�����Ȃ��悤�ɂ��܂�
	//	FILTER_FLAG_AUDIO_FILTER		: �I�[�f�B�I�t�B���^�ɂ��܂�
	//	FILTER_FLAG_RADIO_BUTTON		: �`�F�b�N�{�b�N�X�����W�I�{�^���ɂ��܂�
	//	FILTER_FLAG_WINDOW_HSCROLL		: �����X�N���[���o�[�����E�B���h�E�����܂�
	//	FILTER_FLAG_WINDOW_VSCROLL		: �����X�N���[���o�[�����E�B���h�E�����܂�
	//	FILTER_FLAG_IMPORT				: �C���|�[�g���j���[�����܂�
	//	FILTER_FLAG_EXPORT				: �G�N�X�|�[�g���j���[�����܂�
	0, 0,						//	�ݒ�E�C���h�E�̃T�C�Y (FILTER_FLAG_WINDOW_SIZE�������Ă��鎞�ɗL��)
	"�T���v���t�B���^",			//	�t�B���^�̖��O
	TRACK_N,					//	�g���b�N�o�[�̐� (0�Ȃ疼�O�����l����NULL�ł悢)
	track_name,					//	�g���b�N�o�[�̖��O�S�ւ̃|�C���^
	track_default,				//	�g���b�N�o�[�̏����l�S�ւ̃|�C���^
	track_s, track_e,			//	�g���b�N�o�[�̐��l�̉������ (NULL�Ȃ�S��0�`256)
	CHECK_N,					//	�`�F�b�N�{�b�N�X�̐� (0�Ȃ疼�O�����l����NULL�ł悢)
	check_name,					//	�`�F�b�N�{�b�N�X�̖��O�S�ւ̃|�C���^
	check_default,				//	�`�F�b�N�{�b�N�X�̏����l�S�ւ̃|�C���^
	func_proc,					//	�t�B���^�����֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	NULL,						//	�J�n���ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	NULL,						//	�I�����ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	NULL,						//	�ݒ肪�ύX���ꂽ�Ƃ��ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	NULL,						//	�ݒ�E�B���h�E�ɃE�B���h�E���b�Z�[�W���������ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	NULL, NULL,					//	�V�X�e���Ŏg���܂��̂Ŏg�p���Ȃ��ł�������
	NULL,						//  �g���f�[�^�̈�ւ̃|�C���^ (FILTER_FLAG_EX_DATA�������Ă��鎞�ɗL��)
	NULL,						//  �g���f�[�^�T�C�Y (FILTER_FLAG_EX_DATA�������Ă��鎞�ɗL��)
	"�T���v���t�B���^ version 0.06 by �j�d�m����",
	//  �t�B���^���ւ̃|�C���^ (FILTER_FLAG_EX_INFORMATION�������Ă��鎞�ɗL��)
	NULL,						//	�Z�[�u���J�n����钼�O�ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	NULL,						//	�Z�[�u���I���������O�ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
};*/

//---------------------------------------------------------------------
//		Cache(32f planar) and cache size
//---------------------------------------------------------------------
Ipp32f* cache_sc[3] = { 0 };
Ipp32f* cache_sdc[3] = { 0 };
Ipp32f* cache_sb[3] = { 0 };
Ipp32f* cache_sbm[3] = { 0 };

IppiSize cache_sc_size = { 0, 0 };
IppiSize cache_sdc_size = { 0, 0 };
IppiSize cache_sb_size = { 0, 0 };
IppiSize cache_sbm_size = { 0, 0 };

int cache_sc_step = 0;
int cache_sdc_step = 0;
int cache_sb_step = 0;
int cache_sbm_step = 0;

//---------------------------------------------------------------------
//		Sigmoidal Contrast
//---------------------------------------------------------------------
#define	TRACK_N_SC	2														//	�g���b�N�o�[�̐�
TCHAR	*track_name_sc[] = { "Strength", "Midtone"};	//	�g���b�N�o�[�̖��O
int		track_default_sc[] = { 300, 500 };	//	Str /100; Midtone/1000;
int		track_s_sc[] = { 1, 1 };	//	�g���b�N�o�[�̉����l
int		track_e_sc[] = { 2000, 1000 };	//	�g���b�N�o�[�̏���l

#define	CHECK_N_SC	7														//	�`�F�b�N�{�b�N�X�̐�
TCHAR	*check_name_sc[] = { "Y", "Cb", "Cr", "R", "G", "B", "HELP" };				//	�`�F�b�N�{�b�N�X�̖��O
int		check_default_sc[] = { 1, 1, 1, 0, 0, 0, -1 };				//	�`�F�b�N�{�b�N�X�̏����l (�l��0��1)

//---------------------------------------------------------------------
//		Sigmoidal DeContrast
//---------------------------------------------------------------------
#define	TRACK_N_SDC	2														//	�g���b�N�o�[�̐�
TCHAR	*track_name_sdc[] = { "Strength", "Midtone" };	//	�g���b�N�o�[�̖��O
int		track_default_sdc[] = { 300, 500 };	//	Str /100; Midtone/1000;
int		track_s_sdc[] = { 1, 1 };	//	�g���b�N�o�[�̉����l
int		track_e_sdc[] = { 2000, 1000 };	//	�g���b�N�o�[�̏���l

#define	CHECK_N_SDC	7														//	�`�F�b�N�{�b�N�X�̐�
TCHAR	*check_name_sdc[] = { "Y", "Cb", "Cr", "R", "G", "B", "HELP" };				//	�`�F�b�N�{�b�N�X�̖��O
int		check_default_sdc[] = { 1, 1, 1, 0, 0, 0, -1 };				//	�`�F�b�N�{�b�N�X�̏����l (�l��0��1)

//---------------------------------------------------------------------
//		Sigmoidal Brightness+
//---------------------------------------------------------------------
#define	TRACK_N_SB	1														//	�g���b�N�o�[�̐�
TCHAR	*track_name_sb[] = { "Strength" };	//	�g���b�N�o�[�̖��O
int		track_default_sb[] = { 300 };	//	Str/100;
int		track_s_sb[] = { 1 };	//	�g���b�N�o�[�̉����l
int		track_e_sb[] = { 2000 };	//	�g���b�N�o�[�̏���l

//---------------------------------------------------------------------
//		Sigmoidal Brightness-
//---------------------------------------------------------------------
#define	TRACK_N_SBM	1														//	�g���b�N�o�[�̐�
TCHAR	*track_name_sbm[] = { "Strength" };	//	�g���b�N�o�[�̖��O
int		track_default_sbm[] = { 300 };	//	Str/100;
int		track_s_sbm[] = { 1 };	//	�g���b�N�o�[�̉����l
int		track_e_sbm[] = { 2000 };	//	�g���b�N�o�[�̏���l

//---------------------------------------------------------------------
//		Forward declaration
//---------------------------------------------------------------------
BOOL func_init_sc(FILTER *fp);
BOOL func_init_sdc(FILTER *fp);
BOOL func_init_sb(FILTER *fp);
BOOL func_init_sbm(FILTER *fp);

BOOL func_proc_sc(FILTER *fp, FILTER_PROC_INFO *fpip);
BOOL func_proc_sdc(FILTER *fp, FILTER_PROC_INFO *fpip);
BOOL func_proc_sb(FILTER *fp, FILTER_PROC_INFO *fpip);
BOOL func_proc_sbm(FILTER *fp, FILTER_PROC_INFO *fpip);

BOOL func_WndProc_sc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam, void *editp, FILTER *fp);
BOOL func_WndProc_sdc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam, void *editp, FILTER *fp);
BOOL func_WndProc_sb(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam, void *editp, FILTER *fp);
BOOL func_WndProc_sbm(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam, void *editp, FILTER *fp);

BOOL func_update_sc(FILTER *fp, int status);
BOOL func_update_sdc(FILTER *fp, int status);

BOOL func_exit_sc(FILTER *fp);
BOOL func_exit_sdc(FILTER *fp);
BOOL func_exit_sb(FILTER *fp);
BOOL func_exit_sbm(FILTER *fp);


//---------------------------------------------------------------------
//		Filter Structs
//---------------------------------------------------------------------
FILTER_DLL fSigCon = {
	FILTER_FLAG_EX_INFORMATION,	//	�t�B���^�̃t���O
	0, 0,						//	�ݒ�E�C���h�E�̃T�C�Y (FILTER_FLAG_WINDOW_SIZE�������Ă��鎞�ɗL��)
	"SigContrast+",			//	�t�B���^�̖��O
	TRACK_N_SC,					//	�g���b�N�o�[�̐� (0�Ȃ疼�O�����l����NULL�ł悢)
	track_name_sc,					//	�g���b�N�o�[�̖��O�S�ւ̃|�C���^
	track_default_sc,				//	�g���b�N�o�[�̏����l�S�ւ̃|�C���^
	track_s_sc, track_e_sc,			//	�g���b�N�o�[�̐��l�̉������ (NULL�Ȃ�S��0�`256)
	CHECK_N_SC,					//	�`�F�b�N�{�b�N�X�̐� (0�Ȃ疼�O�����l����NULL�ł悢)
	check_name_sc,					//	�`�F�b�N�{�b�N�X�̖��O�S�ւ̃|�C���^
	check_default_sc,				//	�`�F�b�N�{�b�N�X�̏����l�S�ւ̃|�C���^
	func_proc_sc,					//	�t�B���^�����֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	func_init_sc,						//	�J�n���ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	func_exit_sc,						//	�I�����ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	func_update_sc,						//	�ݒ肪�ύX���ꂽ�Ƃ��ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	func_WndProc_sc,						//	�ݒ�E�B���h�E�ɃE�B���h�E���b�Z�[�W���������ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	NULL, NULL,					//	�V�X�e���Ŏg���܂��̂Ŏg�p���Ȃ��ł�������
	NULL,						//  �g���f�[�^�̈�ւ̃|�C���^ (FILTER_FLAG_EX_DATA�������Ă��鎞�ɗL��)
	NULL,						//  �g���f�[�^�T�C�Y (FILTER_FLAG_EX_DATA�������Ă��鎞�ɗL��)
	"SigContrast+ v1.0 by MT",
	//  �t�B���^���ւ̃|�C���^ (FILTER_FLAG_EX_INFORMATION�������Ă��鎞�ɗL��)
	NULL,						//	�Z�[�u���J�n����钼�O�ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	NULL,						//	�Z�[�u���I���������O�ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
};
FILTER_DLL fSigDeCon = {
	FILTER_FLAG_EX_INFORMATION,	//	�t�B���^�̃t���O
	0, 0,						//	�ݒ�E�C���h�E�̃T�C�Y (FILTER_FLAG_WINDOW_SIZE�������Ă��鎞�ɗL��)
	"SigContrast-",			//	�t�B���^�̖��O
	TRACK_N_SDC,					//	�g���b�N�o�[�̐� (0�Ȃ疼�O�����l����NULL�ł悢)
	track_name_sdc,					//	�g���b�N�o�[�̖��O�S�ւ̃|�C���^
	track_default_sdc,				//	�g���b�N�o�[�̏����l�S�ւ̃|�C���^
	track_s_sdc, track_e_sdc,			//	�g���b�N�o�[�̐��l�̉������ (NULL�Ȃ�S��0�`256)
	CHECK_N_SDC,					//	�`�F�b�N�{�b�N�X�̐� (0�Ȃ疼�O�����l����NULL�ł悢)
	check_name_sdc,					//	�`�F�b�N�{�b�N�X�̖��O�S�ւ̃|�C���^
	check_default_sdc,				//	�`�F�b�N�{�b�N�X�̏����l�S�ւ̃|�C���^
	func_proc_sdc,					//	�t�B���^�����֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	func_init_sdc,						//	�J�n���ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	func_exit_sdc,						//	�I�����ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	func_update_sdc,						//	�ݒ肪�ύX���ꂽ�Ƃ��ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	func_WndProc_sdc,						//	�ݒ�E�B���h�E�ɃE�B���h�E���b�Z�[�W���������ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	NULL, NULL,					//	�V�X�e���Ŏg���܂��̂Ŏg�p���Ȃ��ł�������
	NULL,						//  �g���f�[�^�̈�ւ̃|�C���^ (FILTER_FLAG_EX_DATA�������Ă��鎞�ɗL��)
	NULL,						//  �g���f�[�^�T�C�Y (FILTER_FLAG_EX_DATA�������Ă��鎞�ɗL��)
	"SigContrast- v1.0 by MT",
	//  �t�B���^���ւ̃|�C���^ (FILTER_FLAG_EX_INFORMATION�������Ă��鎞�ɗL��)
	NULL,						//	�Z�[�u���J�n����钼�O�ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	NULL,						//	�Z�[�u���I���������O�ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
};

FILTER_DLL fSigB = {
	FILTER_FLAG_EX_INFORMATION,	//	�t�B���^�̃t���O
	0, 0,						//	�ݒ�E�C���h�E�̃T�C�Y (FILTER_FLAG_WINDOW_SIZE�������Ă��鎞�ɗL��)
	"SigBrightness+",			//	�t�B���^�̖��O
	TRACK_N_SB,					//	�g���b�N�o�[�̐� (0�Ȃ疼�O�����l����NULL�ł悢)
	track_name_sb,					//	�g���b�N�o�[�̖��O�S�ւ̃|�C���^
	track_default_sb,				//	�g���b�N�o�[�̏����l�S�ւ̃|�C���^
	track_s_sb, track_e_sb,			//	�g���b�N�o�[�̐��l�̉������ (NULL�Ȃ�S��0�`256)
	NULL,					//	�`�F�b�N�{�b�N�X�̐� (0�Ȃ疼�O�����l����NULL�ł悢)
	NULL,					//	�`�F�b�N�{�b�N�X�̖��O�S�ւ̃|�C���^
	NULL,				//	�`�F�b�N�{�b�N�X�̏����l�S�ւ̃|�C���^
	func_proc_sb,					//	�t�B���^�����֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	func_init_sb,						//	�J�n���ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	func_exit_sb,						//	�I�����ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	NULL,						//	�ݒ肪�ύX���ꂽ�Ƃ��ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	func_WndProc_sb,						//	�ݒ�E�B���h�E�ɃE�B���h�E���b�Z�[�W���������ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	NULL, NULL,					//	�V�X�e���Ŏg���܂��̂Ŏg�p���Ȃ��ł�������
	NULL,						//  �g���f�[�^�̈�ւ̃|�C���^ (FILTER_FLAG_EX_DATA�������Ă��鎞�ɗL��)
	NULL,						//  �g���f�[�^�T�C�Y (FILTER_FLAG_EX_DATA�������Ă��鎞�ɗL��)
	"SigBrightness+ v1.0 by MT",
	//  �t�B���^���ւ̃|�C���^ (FILTER_FLAG_EX_INFORMATION�������Ă��鎞�ɗL��)
	NULL,						//	�Z�[�u���J�n����钼�O�ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	NULL,						//	�Z�[�u���I���������O�ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
};

FILTER_DLL fSigBM = {
	FILTER_FLAG_EX_INFORMATION,	//	�t�B���^�̃t���O
	0, 0,						//	�ݒ�E�C���h�E�̃T�C�Y (FILTER_FLAG_WINDOW_SIZE�������Ă��鎞�ɗL��)
	"SigBrightness-",			//	�t�B���^�̖��O
	TRACK_N_SBM,					//	�g���b�N�o�[�̐� (0�Ȃ疼�O�����l����NULL�ł悢)
	track_name_sbm,					//	�g���b�N�o�[�̖��O�S�ւ̃|�C���^
	track_default_sbm,				//	�g���b�N�o�[�̏����l�S�ւ̃|�C���^
	track_s_sbm, track_e_sbm,			//	�g���b�N�o�[�̐��l�̉������ (NULL�Ȃ�S��0�`256)
	NULL,					//	�`�F�b�N�{�b�N�X�̐� (0�Ȃ疼�O�����l����NULL�ł悢)
	NULL,					//	�`�F�b�N�{�b�N�X�̖��O�S�ւ̃|�C���^
	NULL,				//	�`�F�b�N�{�b�N�X�̏����l�S�ւ̃|�C���^
	func_proc_sbm,					//	�t�B���^�����֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	func_init_sbm,						//	�J�n���ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	func_exit_sbm,						//	�I�����ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	NULL,						//	�ݒ肪�ύX���ꂽ�Ƃ��ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	func_WndProc_sbm,						//	�ݒ�E�B���h�E�ɃE�B���h�E���b�Z�[�W���������ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	NULL, NULL,					//	�V�X�e���Ŏg���܂��̂Ŏg�p���Ȃ��ł�������
	NULL,						//  �g���f�[�^�̈�ւ̃|�C���^ (FILTER_FLAG_EX_DATA�������Ă��鎞�ɗL��)
	NULL,						//  �g���f�[�^�T�C�Y (FILTER_FLAG_EX_DATA�������Ă��鎞�ɗL��)
	"SigBrightness- v1.0 by MT",
	//  �t�B���^���ւ̃|�C���^ (FILTER_FLAG_EX_INFORMATION�������Ă��鎞�ɗL��)
	NULL,						//	�Z�[�u���J�n����钼�O�ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
	NULL,						//	�Z�[�u���I���������O�ɌĂ΂��֐��ւ̃|�C���^ (NULL�Ȃ�Ă΂�܂���)
};

//---------------------------------------------------------------------
//		Filter List
//---------------------------------------------------------------------
FILTER_DLL *filter_list[] = { &fSigCon, &fSigDeCon, &fSigB, &fSigBM, NULL };
EXTERN_C FILTER_DLL __declspec(dllexport) ** __stdcall GetFilterTableList(void)
{
	return (FILTER_DLL **)&filter_list;
}

//---------------------------------------------------------------------
//		�t�B���^�\���̂̃|�C���^��n���֐�
//---------------------------------------------------------------------
/*EXTERN_C FILTER_DLL __declspec(dllexport) * __stdcall GetFilterTable(void)
{
	return &filter;
}*/
//	���L�̂悤�ɂ����1��auf�t�@�C���ŕ����̃t�B���^�\���̂�n�����Ƃ��o���܂�
/*
FILTER_DLL *filter_list[] = {&filter,&filter2,NULL};
EXTERN_C FILTER_DLL __declspec(dllexport) ** __stdcall GetFilterTableList( void )
{
return (FILTER_DLL **)&filter_list;
}
*/

//---------------------------------------------------------------------
//		Helper Functions
//---------------------------------------------------------------------


__forceinline double sig(double strength, double midtone, double u)
{
	double halft = 0.5*strength*(u- midtone);
	double result = 0.5*(1.0 + tanh(halft));
	return result;
}

IppStatus sigContrast_32f_C1IR(Ipp32f* src, int src_step, IppiSize size, double strength, double midtone)
{
	double numB = sig(strength, midtone, 0.0);
	double denomA = sig(strength, midtone, 1.0);
	double denomB = numB;
	double denom = denomA - denomB;
	cilk::reducer<cilk::op_vector<IppStatus>> vSTS;
	_Cilk_for (int line = 0; line < size.height; ++line)
	{
		IppStatus STS= ippStsNoErr;
		IppiSize aLine;
		aLine.height = 1;
		aLine.width = size.width;
		Ipp32f* pStart = (Ipp32f*)(src + src_step* line/sizeof(Ipp32f));
		//
		STS = ippiMulC_32f_C1IR(-1.0, pStart, src_step, aLine);
		if (STS != ippStsNoErr)	vSTS->push_back(STS);
		STS = ippiAddC_32f_C1IR((Ipp32f)midtone, pStart, src_step, aLine);
		if (STS != ippStsNoErr)	vSTS->push_back(STS);
		STS = ippiMulC_32f_C1IR((Ipp32f)strength, pStart, src_step, aLine);
		if (STS != ippStsNoErr)	vSTS->push_back(STS);
		STS = ippiExp_32f_C1IR(pStart, src_step, aLine);
		if (STS != ippStsNoErr)	vSTS->push_back(STS);
		STS = ippiAddC_32f_C1IR(1.0, pStart, src_step, aLine);
		if (STS != ippStsNoErr)	vSTS->push_back(STS);
		//Reciporcal: Exp(-1*Ln(x))
		//Need to avoid x=0 before using Ln
		STS = ippiThreshold_32f_C1IR(pStart, src_step, aLine, 1.0E-5, IppCmpOp::ippCmpLess);
		if (STS != ippStsNoErr)	vSTS->push_back(STS);
		STS = ippiLn_32f_C1IR(pStart, src_step, aLine);
		if (STS != ippStsNoErr)	vSTS->push_back(STS);
		STS = ippiMulC_32f_C1IR(-1.0, pStart, src_step, aLine);
		if (STS != ippStsNoErr)	vSTS->push_back(STS);
		STS = ippiExp_32f_C1IR(pStart, src_step, aLine);
		if (STS != ippStsNoErr)	vSTS->push_back(STS);
		//
		STS = ippiSubC_32f_C1IR(numB, pStart, src_step, aLine);
		if (STS != ippStsNoErr)	vSTS->push_back(STS);
		STS = ippiDivC_32f_C1IR(denom, pStart, src_step, aLine);
		if (STS != ippStsNoErr)	vSTS->push_back(STS);
	}
	std::vector<IppStatus> oSTS = vSTS.get_value();
	IppStatus rStatus = ippStsNoErr;
	if (oSTS.size() > 0)
	{
		return oSTS[0];
	}
	return rStatus;
}

IppStatus sigDeContrast_32f_C1IR(Ipp32f* src, int src_step, IppiSize size, double strength, double midtone)
{
	double numB = sig(strength, midtone, 0.0);
	double denomA = sig(strength, midtone, 1.0);
	double denomB = numB;
	double denom = denomA - denomB;
	cilk::reducer<cilk::op_vector<IppStatus>> vSTS;
	_Cilk_for(int line = 0; line < size.height; ++line)
	{
		IppStatus STS = ippStsNoErr;
		IppiSize aLine;
		aLine.height = 1;
		aLine.width = size.width;
		Ipp32f* pStart = (Ipp32f*)((byte*)src + src_step* line);
		//
		STS = ippiMulC_32f_C1IR(denom, pStart, src_step, aLine);
		if (STS != ippStsNoErr)	vSTS->push_back(STS);
		STS = ippiAddC_32f_C1IR(numB, pStart, src_step, aLine);
		if (STS != ippStsNoErr)	vSTS->push_back(STS);
		STS = ippiThreshold_32f_C1IR(pStart, src_step, aLine, 1.0E-12, IppCmpOp::ippCmpLess);
		if (STS != ippStsNoErr)	vSTS->push_back(STS);
		STS = ippiLn_32f_C1IR(pStart, src_step, aLine);
		if (STS != ippStsNoErr)	vSTS->push_back(STS);
		STS = ippiMulC_32f_C1IR(-1.0, pStart, src_step, aLine);
		if (STS != ippStsNoErr)	vSTS->push_back(STS);
		STS = ippiExp_32f_C1IR(pStart, src_step, aLine);
		if (STS != ippStsNoErr)	vSTS->push_back(STS);
		STS = ippiSubC_32f_C1IR(1.0, pStart, src_step, aLine);
		if (STS != ippStsNoErr)	vSTS->push_back(STS);
		STS = ippiThreshold_32f_C1IR(pStart, src_step, aLine, 1.0E-12, IppCmpOp::ippCmpLess);
		if (STS != ippStsNoErr)	vSTS->push_back(STS);
		STS = ippiLn_32f_C1IR(pStart, src_step, aLine);
		if (STS != ippStsNoErr)	vSTS->push_back(STS);
		STS = ippiDivC_32f_C1IR(-1.0*strength, pStart, src_step, aLine);
		if (STS != ippStsNoErr)	vSTS->push_back(STS);
		STS = ippiAddC_32f_C1IR(midtone, pStart, src_step, aLine);
		if (STS != ippStsNoErr)	vSTS->push_back(STS);
	}
	std::vector<IppStatus> oSTS = vSTS.get_value();
	IppStatus rStatus = ippStsNoErr;
	if (oSTS.size() > 0)
	{
		return oSTS[0];
	}
	return rStatus;
}

IppStatus yc2rgb48(Ipp16s* src, int src_step, Ipp16s* dst, int dst_step, IppiSize size)
{
	cilk::reducer<cilk::op_vector<IppStatus>> rSTS;
	
	_Cilk_for(int line = 0; line < size.height; ++line)
	{
		Ipp16s* src_start = src + line*src_step / sizeof(Ipp16s);
		Ipp16s* dst_start = dst + line*dst_step / sizeof(Ipp16s);
		IppiSize aLine = { size.width, 1 };
		Ipp32f matrix[3][4] = {
			0.9998526134457961, 0.000977435614231581, 1.3973906492455004, 0,
			0.9992527432131624, -0.3404853357654929, -0.7098168683235085, 0,
			0.9999433501193996, 1.766912980864788, 0.0018422065248463288, 0
		};
		IppStatus STS= ippiColorTwist32f_16s_C3R(src_start, src_step, dst_start, dst_step, aLine, matrix);
		if (STS != ippStsNoErr)
		{
			rSTS->push_back(STS);
		}
	}
	std::vector<IppStatus> lSTS = rSTS.get_value();
	if (lSTS.size() > 0)
	{
		return lSTS[0];
	}
	else
	{
		return ippStsNoErr;
	}
	return ippStsNoErr;
}

IppStatus rgb2yc48(Ipp16s* src, int src_step, Ipp16s* dst, int dst_step, IppiSize size)
{
	cilk::reducer<cilk::op_vector<IppStatus>> rSTS;

	_Cilk_for(int line = 0; line < size.height; ++line)
	{
		Ipp16s* src_start = src + line*src_step / sizeof(Ipp16s);
		Ipp16s* dst_start = dst + line*dst_step / sizeof(Ipp16s);
		IppiSize aLine = { size.width, 1 };
		Ipp32f matrix[3][4] = {
			0.29877450980392156, 0.5884803921568628, 0.11323529411764706, 0,
			-0.1696078431372549, -0.33259803921568626, +0.5019607843137255, 0,
			0.5019607843137255, -0.42083333333333334, -0.08137254901960785, 0
		};
		IppStatus STS = ippiColorTwist32f_16s_C3R(src_start, src_step, dst_start, dst_step, aLine, matrix);
		if (STS != ippStsNoErr)
		{
			rSTS->push_back(STS);
		}
	}
	std::vector<IppStatus> lSTS = rSTS.get_value();
	if (lSTS.size() > 0)
	{
		return lSTS[0];
	}
	else
	{
		return ippStsNoErr;
	}
	return ippStsNoErr;
}

//---------------------------------------------------------------------
//		Filter init functions
//---------------------------------------------------------------------
BOOL func_init_sc(FILTER *fp)
{
	IppStatus STS = ippInit();
	if (STS == ippStsNoErr)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	return FALSE;
}

BOOL func_init_sdc(FILTER *fp)
{
	IppStatus STS = ippInit();
	if (STS == ippStsNoErr)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	return FALSE;
}

BOOL func_init_sb(FILTER *fp)
{
	IppStatus STS = ippInit();
	if (STS == ippStsNoErr)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	return FALSE;
}

BOOL func_init_sbm(FILTER *fp)
{
	IppStatus STS = ippInit();
	if (STS == ippStsNoErr)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	return FALSE;
}
//---------------------------------------------------------------------
//		Filter main functions
//---------------------------------------------------------------------
BOOL func_proc_sc(FILTER *fp, FILTER_PROC_INFO *fpip)
{
	//common var
	double strength = (double)fp->track[0] / 100.0;
	double midtone = (double)fp->track[1] / 1000.0;
	IppiSize frmSize = { fpip->w, fpip->h };
	int rawycp_step = fpip->max_w * sizeof(PIXEL_YC);
	IppStatus STS = ippStsNoErr;
	// buffer
	if ((fpip->w != cache_sc_size.width) || (fpip->h != cache_sc_size.height))//free old buffer if size changed
	{
		for (int p = 0; p < 3; ++p)
		{
			if (cache_sc[p])
			{
				ippiFree(cache_sc[p]);
				cache_sc[p] = NULL;
			}
		}
		cache_sc_size.height = 0;
		cache_sc_size.width = 0;
	}
	for (int p = 0; p < 3; ++p) //allocate buffer if not done so
	{
		if (cache_sc[p]==NULL)
		{
			cache_sc[p] = ippiMalloc_32f_C1(frmSize.width, frmSize.height, &cache_sc_step);
		}
	}
	cache_sc_size.height = frmSize.height;
	cache_sc_size.width = frmSize.width;
	int buffer_step = cache_sc_step;
	Ipp32f* buffer[3] = { 0 };
	buffer[0] = cache_sc[0];
	buffer[1] = cache_sc[1];
	buffer[2] = cache_sc[2];
	if (!(buffer[0] && buffer[1] && buffer[2]))
	{
		for (int p = 0; p < 3; ++p)
		{
			if (buffer[p])
			{
				ippiFree(buffer[p]);
				buffer[p] = NULL;
				cache_sc[p] = NULL;
			}
		}
		cache_sc_size.height = 0;
		cache_sc_size.width = 0;
		return FALSE;
	}
	// [YC48>RGB] > planar buffer
	if (fp->check[0] || fp->check[1] || fp->check[2])
	{
		//Normal YC48 routine
		// AOD -> SOA
		Ipp16s* planar16s[3] = { 0 };
		planar16s[0] = (Ipp16s*)fpip->ycp_temp;
		planar16s[1] = planar16s[0] + frmSize.height * frmSize.width;
		planar16s[2] = planar16s[1] + frmSize.height * frmSize.width;
		STS = ippiCopy_16s_C3P3R((Ipp16s*)fpip->ycp_edit, rawycp_step, planar16s, frmSize.width*sizeof(Ipp16s), frmSize);
		// 16s -> 32f buffer
		_Cilk_for(int p = 0; p < 3; ++p)
		{
			_Cilk_for(int line = 0; line < frmSize.height; ++line)
			{
				IppiSize aLine = { frmSize.width, 1 };
				Ipp16s* pSrc = planar16s[p] + frmSize.width * line;
				Ipp32f* pDst = buffer[p] + buffer_step*line/sizeof(Ipp32f);
				ippiConvert_16s32f_C1R(pSrc, frmSize.width*sizeof(Ipp16s), pDst, buffer_step, aLine);
			}
		}
		// Normalize to 0~1
		_Cilk_for(int line = 0; line < frmSize.height; ++line) //Y-plane
		{
			IppiSize aLine = { frmSize.width, 1 };
			Ipp32f* pStart = buffer[0] + buffer_step*line / sizeof(Ipp32f);
			ippiDivC_32f_C1IR(4096, pStart, buffer_step, aLine);
			ippiThreshold_LTValGTVal_32f_C1IR(pStart, buffer_step, aLine, 0.0, 0.0, 1.0, 1.0);
		}
		_Cilk_for(int p = 1; p < 3; ++p)
		{
			_Cilk_for(int line = 0; line < frmSize.height; ++line) // Cb and Cr
			{
				IppiSize aLine = { frmSize.width, 1 };
				Ipp32f* pStart = buffer[p] + buffer_step * line/ sizeof(Ipp32f);
				ippiAddC_32f_C1IR(2048, pStart, buffer_step, aLine);
				ippiDivC_32f_C1IR(4096, pStart, buffer_step, aLine);
				ippiThreshold_LTValGTVal_32f_C1IR(pStart, buffer_step, aLine, 0.0, 0.0, 1.0, 1.0);
			}
		}
		// Apply sigmoidal transform
		_Cilk_for(int p = 0; p < 3; ++p)
		{
			if (fp->check[p])
			{
				sigContrast_32f_C1IR(buffer[p], buffer_step, frmSize, strength, midtone);
			}
		}
		// Re-Scale *4096 on all planes
		_Cilk_for(int p = 0; p < 3; ++p)
		{
			_Cilk_for(int line = 0; line < frmSize.height; ++line)
			{
				IppiSize aLine = { frmSize.width, 1 };
				Ipp32f* pStart = buffer[p] + buffer_step*line / sizeof(Ipp32f);
				ippiMulC_32f_C1IR(4096, pStart, buffer_step, aLine);
			}
		}
		// Minus 2048 for Cb and Cr planes
		_Cilk_for(int p = 1; p < 3; ++p)
		{
			_Cilk_for(int line = 0; line < frmSize.height; ++line)
			{
				IppiSize aLine = { frmSize.width, 1 };
				Ipp32f* pStart = buffer[p] + buffer_step*line / sizeof(Ipp32f);
				ippiSubC_32f_C1IR(2048, pStart, buffer_step, aLine);
			}
		}
		// 32f -> 16s
		_Cilk_for(int p = 0; p < 3; ++p)
		{
			_Cilk_for(int line = 0; line < frmSize.height; ++line)
			{
				IppiSize aLine = { frmSize.width, 1 };
				Ipp16s* pDst = planar16s[p] + frmSize.width * line;
				Ipp32f* pSrc = buffer[p] + buffer_step*line / sizeof(Ipp32f);
				ippiConvert_32f16s_C1R(pSrc, buffer_step, pDst, frmSize.width*sizeof(Ipp16s), aLine, IppRoundMode::ippRndNear);
			}
		}
		// SOA -> AOS
		STS = ippiCopy_16s_P3C3R(planar16s, frmSize.width*sizeof(Ipp16s), (Ipp16s*)fpip->ycp_edit, rawycp_step, frmSize);
	}
	else if (fp->check[3] || fp->check[4] || fp->check[5])
	{
		//RGB routine
		//Convert to RGB, need more buffer
		int rgbdata_step = 0;
		Ipp16s* rgbdata = ippiMalloc_16s_C3(frmSize.width, frmSize.height, &rgbdata_step);
		STS = yc2rgb48((Ipp16s*)fpip->ycp_edit, rawycp_step, rgbdata, rgbdata_step, frmSize);
		if (STS != ippStsNoErr)
		{
			ippiFree(rgbdata);
			for (int p = 0; p < 3; ++p)
			{
				if (buffer[p])
				{
					ippiFree(buffer[p]);
					buffer[p] = NULL;
					cache_sc[p] = NULL;
				}
			}
			cache_sc_size.height = 0;
			cache_sc_size.width = 0;
			return FALSE;
		}
		// AOD -> SOA
		Ipp16s* planar16s[3] = { 0 };
		planar16s[0] = (Ipp16s*)fpip->ycp_temp;
		planar16s[1] = planar16s[0] + frmSize.height * frmSize.width;
		planar16s[2] = planar16s[1] + frmSize.height * frmSize.width;
		STS = ippiCopy_16s_C3P3R(rgbdata, rgbdata_step, planar16s, frmSize.width*sizeof(Ipp16s), frmSize);
		if (STS != ippStsNoErr)
		{
			ippiFree(rgbdata);
			for (int p = 0; p < 3; ++p)
			{
				if (buffer[p])
				{
					ippiFree(buffer[p]);
					buffer[p] = NULL;
					cache_sc[p] = NULL;
				}
			}
			cache_sc_size.height = 0;
			cache_sc_size.width = 0;
			return FALSE;
		}
		// 16s -> 32f buffer
		_Cilk_for(int p = 0; p < 3; ++p)
		{
			_Cilk_for(int line = 0; line < frmSize.height; ++line)
			{
				IppiSize aLine = { frmSize.width, 1 };
				Ipp16s* pSrc = planar16s[p] + frmSize.width * line;
				Ipp32f* pDst = buffer[p] + buffer_step*line / sizeof(Ipp32f);
				ippiConvert_16s32f_C1R(pSrc, frmSize.width*sizeof(Ipp16s), pDst, buffer_step, aLine);
			}
		}
		// Normalize to 0~1
		
		_Cilk_for(int p = 0; p < 3; ++p)
		{
			_Cilk_for(int line = 0; line < frmSize.height; ++line) // Simpler for RGB
			{
				IppiSize aLine = { frmSize.width, 1 };
				Ipp32f* pStart = buffer[p] + buffer_step * line / sizeof(Ipp32f);
				ippiDivC_32f_C1IR(4080, pStart, buffer_step, aLine);
				ippiThreshold_LTValGTVal_32f_C1IR(pStart, buffer_step, aLine, 0.0, 0.0, 1.0, 1.0);
			}
		}
		// Apply sigmoidal transform
		_Cilk_for(int p = 3; p < 6; ++p)
		{
			if (fp->check[p])
			{
				sigContrast_32f_C1IR(buffer[p-3], buffer_step, frmSize, strength, midtone);
			}
		}
		// Re-Scale *4080 on all planes
		_Cilk_for(int p = 0; p < 3; ++p)
		{
			_Cilk_for(int line = 0; line < frmSize.height; ++line)
			{
				IppiSize aLine = { frmSize.width, 1 };
				Ipp32f* pStart = buffer[p] + buffer_step*line / sizeof(Ipp32f);
				ippiMulC_32f_C1IR(4080, pStart, buffer_step, aLine);
			}
		}
		
		// 32f -> 16s
		_Cilk_for(int p = 0; p < 3; ++p)
		{
			_Cilk_for(int line = 0; line < frmSize.height; ++line)
			{
				IppiSize aLine = { frmSize.width, 1 };
				Ipp16s* pDst = planar16s[p] + frmSize.width * line;
				Ipp32f* pSrc = buffer[p] + buffer_step*line / sizeof(Ipp32f);
				ippiConvert_32f16s_C1R(pSrc, buffer_step, pDst, frmSize.width*sizeof(Ipp16s), aLine, IppRoundMode::ippRndNear);
			}
		}
		// SOA -> AOS
		STS = ippiCopy_16s_P3C3R(planar16s, frmSize.width*sizeof(Ipp16s), rgbdata, rgbdata_step, frmSize);
		if (STS != ippStsNoErr)
		{
			ippiFree(rgbdata);
			for (int p = 0; p < 3; ++p)
			{
				if (buffer[p])
				{
					ippiFree(buffer[p]);
					buffer[p] = NULL;
					cache_sc[p] = NULL;
				}
			}
			cache_sc_size.height = 0;
			cache_sc_size.width = 0;
			return FALSE;
		}
		// RGB -> YC
		STS = rgb2yc48(rgbdata, rgbdata_step, (Ipp16s*)fpip->ycp_edit, rawycp_step, frmSize);
		if (STS != ippStsNoErr)
		{
			ippiFree(rgbdata);
			for (int p = 0; p < 3; ++p)
			{
				if (buffer[p])
				{
					ippiFree(buffer[p]);
					buffer[p] = NULL;
					cache_sc[p] = NULL;
				}
			}
			cache_sc_size.height = 0;
			cache_sc_size.width = 0;
			return FALSE;
		}
		ippiFree(rgbdata);
	}
	else
	{
		//None selected
		for (int p = 0; p < 3; ++p)
		{
			if (buffer[p])
			{
				ippiFree(buffer[p]);
				buffer[p] = NULL;
				cache_sc[p] = NULL;
			}
			cache_sc_size.height = 0;
			cache_sc_size.width = 0;
		}
		return FALSE;
	}
	//Free buffer
	/*for (int p = 0; p < 3; ++p)
	{
		if (buffer[p])
		{
			ippiFree(buffer[p]);
		}
	}*/
	return TRUE;
}

BOOL func_proc_sdc(FILTER *fp, FILTER_PROC_INFO *fpip)
{
	//common var
	double strength = (double)fp->track[0] / 100.0;
	double midtone = (double)fp->track[1] / 1000.0;
	IppiSize frmSize = { fpip->w, fpip->h };
	int rawycp_step = fpip->max_w * sizeof(PIXEL_YC);
	IppStatus STS = ippStsNoErr;
	// buffer
	if ((fpip->w != cache_sdc_size.width) || (fpip->h != cache_sdc_size.height))//free old buffer if size changed
	{
		for (int p = 0; p < 3; ++p)
		{
			if (cache_sdc[p])
			{
				ippiFree(cache_sdc[p]);
				cache_sdc[p] = NULL;
			}
		}
		cache_sdc_size.height = 0;
		cache_sdc_size.width = 0;
		cache_sdc_step = 0;
	}
	for (int p = 0; p < 3; ++p) //allocate buffer if not done so
	{
		if (cache_sdc[p] == NULL)
		{
			cache_sdc[p] = ippiMalloc_32f_C1(frmSize.width, frmSize.height, &cache_sdc_step);
		}
	}
	cache_sdc_size.height = frmSize.height;
	cache_sdc_size.width = frmSize.width;
	int buffer_step = cache_sdc_step;
	Ipp32f* buffer[3] = { 0 };
	buffer[0] = cache_sdc[0];
	buffer[1] = cache_sdc[1];
	buffer[2] = cache_sdc[2];
	if (!(buffer[0] && buffer[1] && buffer[2]))
	{
		for (int p = 0; p < 3; ++p)
		{
			if (buffer[p])
			{
				ippiFree(buffer[p]);
				buffer[p] = NULL;
				cache_sdc[p] = NULL;
			}
		}
		cache_sdc_size.height = 0;
		cache_sdc_size.width = 0;
		return FALSE;
	}
	// [YC48>RGB] > planar buffer
	if (fp->check[0] || fp->check[1] || fp->check[2])
	{
		//Normal YC48 routine
		// AOD -> SOA
		Ipp16s* planar16s[3] = { 0 };
		planar16s[0] = (Ipp16s*)fpip->ycp_temp;
		planar16s[1] = planar16s[0] + frmSize.height * frmSize.width;
		planar16s[2] = planar16s[1] + frmSize.height * frmSize.width;
		STS = ippiCopy_16s_C3P3R((Ipp16s*)fpip->ycp_edit, rawycp_step, planar16s, frmSize.width*sizeof(Ipp16s), frmSize);
		// 16s -> 32f buffer
		_Cilk_for(int p = 0; p < 3; ++p)
		{
			_Cilk_for(int line = 0; line < frmSize.height; ++line)
			{
				IppiSize aLine = { frmSize.width, 1 };
				Ipp16s* pSrc = planar16s[p] + frmSize.width * line;
				Ipp32f* pDst = buffer[p] + buffer_step*line / sizeof(Ipp32f);
				ippiConvert_16s32f_C1R(pSrc, frmSize.width*sizeof(Ipp16s), pDst, buffer_step, aLine);
			}
		}
		// Normalize to 0~1
		_Cilk_for(int line = 0; line < frmSize.height; ++line) //Y-plane
		{
			IppiSize aLine = { frmSize.width, 1 };
			Ipp32f* pStart = buffer[0] + buffer_step*line / sizeof(Ipp32f);
			ippiDivC_32f_C1IR(4096, pStart, buffer_step, aLine);
			ippiThreshold_LTValGTVal_32f_C1IR(pStart, buffer_step, aLine, 0.0, 0.0, 1.0, 1.0);
		}
		_Cilk_for(int p = 1; p < 3; ++p)
		{
			_Cilk_for(int line = 0; line < frmSize.height; ++line) // Cb and Cr
			{
				IppiSize aLine = { frmSize.width, 1 };
				Ipp32f* pStart = buffer[p] + buffer_step * line / sizeof(Ipp32f);
				ippiAddC_32f_C1IR(2048, pStart, buffer_step, aLine);
				ippiDivC_32f_C1IR(4096, pStart, buffer_step, aLine);
				ippiThreshold_LTValGTVal_32f_C1IR(pStart, buffer_step, aLine, 0.0, 0.0, 1.0, 1.0);
			}
		}
		// Apply sigmoidal transform
		_Cilk_for(int p = 0; p < 3; ++p)
		{
			if (fp->check[p])
			{
				sigDeContrast_32f_C1IR(buffer[p], buffer_step, frmSize, strength, midtone);
			}
		}
		// Re-Scale *4096 on all planes
		_Cilk_for(int p = 0; p < 3; ++p)
		{
			_Cilk_for(int line = 0; line < frmSize.height; ++line)
			{
				IppiSize aLine = { frmSize.width, 1 };
				Ipp32f* pStart = buffer[p] + buffer_step*line / sizeof(Ipp32f);
				ippiMulC_32f_C1IR(4096, pStart, buffer_step, aLine);
			}
		}
		// Minus 2048 for Cb and Cr planes
		_Cilk_for(int p = 1; p < 3; ++p)
		{
			_Cilk_for(int line = 0; line < frmSize.height; ++line)
			{
				IppiSize aLine = { frmSize.width, 1 };
				Ipp32f* pStart = buffer[p] + buffer_step*line / sizeof(Ipp32f);
				ippiSubC_32f_C1IR(2048, pStart, buffer_step, aLine);
			}
		}
		// 32f -> 16s
		_Cilk_for(int p = 0; p < 3; ++p)
		{
			_Cilk_for(int line = 0; line < frmSize.height; ++line)
			{
				IppiSize aLine = { frmSize.width, 1 };
				Ipp16s* pDst = planar16s[p] + frmSize.width * line;
				Ipp32f* pSrc = buffer[p] + buffer_step*line / sizeof(Ipp32f);
				ippiConvert_32f16s_C1R(pSrc, buffer_step, pDst, frmSize.width*sizeof(Ipp16s), aLine, IppRoundMode::ippRndNear);
			}
		}
		// SOA -> AOS
		STS = ippiCopy_16s_P3C3R(planar16s, frmSize.width*sizeof(Ipp16s), (Ipp16s*)fpip->ycp_edit, rawycp_step, frmSize);
	}
	else if (fp->check[3] || fp->check[4] || fp->check[5])
	{
		//RGB routine
		//Convert to RGB, need more buffer
		int rgbdata_step = 0;
		Ipp16s* rgbdata = ippiMalloc_16s_C3(frmSize.width, frmSize.height, &rgbdata_step);
		STS = yc2rgb48((Ipp16s*)fpip->ycp_edit, rawycp_step, rgbdata, rgbdata_step, frmSize);
		if (STS != ippStsNoErr)
		{
			ippiFree(rgbdata);
			for (int p = 0; p < 3; ++p)
			{
				if (buffer[p])
				{
					ippiFree(buffer[p]);
					buffer[p] = NULL;
					cache_sdc[p] = NULL;
				}
			}
			cache_sdc_size.height = 0;
			cache_sdc_size.width = 0;
			cache_sdc_step = 0;
			return FALSE;
		}
		// AOD -> SOA
		Ipp16s* planar16s[3] = { 0 };
		planar16s[0] = (Ipp16s*)fpip->ycp_temp;
		planar16s[1] = planar16s[0] + frmSize.height * frmSize.width;
		planar16s[2] = planar16s[1] + frmSize.height * frmSize.width;
		STS = ippiCopy_16s_C3P3R(rgbdata, rgbdata_step, planar16s, frmSize.width*sizeof(Ipp16s), frmSize);
		if (STS != ippStsNoErr)
		{
			ippiFree(rgbdata);
			for (int p = 0; p < 3; ++p)
			{
				if (buffer[p])
				{
					ippiFree(buffer[p]);
					buffer[p] = NULL;
					cache_sdc[p] = NULL;
				}
			}
			cache_sdc_size.height = 0;
			cache_sdc_size.width = 0;
			cache_sdc_step = 0;
			return FALSE;
		}
		// 16s -> 32f buffer
		_Cilk_for(int p = 0; p < 3; ++p)
		{
			_Cilk_for(int line = 0; line < frmSize.height; ++line)
			{
				IppiSize aLine = { frmSize.width, 1 };
				Ipp16s* pSrc = planar16s[p] + frmSize.width * line;
				Ipp32f* pDst = buffer[p] + buffer_step*line / sizeof(Ipp32f);
				ippiConvert_16s32f_C1R(pSrc, frmSize.width*sizeof(Ipp16s), pDst, buffer_step, aLine);
			}
		}
		// Normalize to 0~1

		_Cilk_for(int p = 0; p < 3; ++p)
		{
			_Cilk_for(int line = 0; line < frmSize.height; ++line) // Simpler for RGB
			{
				IppiSize aLine = { frmSize.width, 1 };
				Ipp32f* pStart = buffer[p] + buffer_step * line / sizeof(Ipp32f);
				ippiDivC_32f_C1IR(4080, pStart, buffer_step, aLine);
				ippiThreshold_LTValGTVal_32f_C1IR(pStart, buffer_step, aLine, 0.0, 0.0, 1.0, 1.0);
			}
		}
		// Apply sigmoidal transform
		_Cilk_for(int p = 3; p < 6; ++p)
		{
			if (fp->check[p])
			{
				sigDeContrast_32f_C1IR(buffer[p - 3], buffer_step, frmSize, strength, midtone);
			}
		}
		// Re-Scale *4080 on all planes
		_Cilk_for(int p = 0; p < 3; ++p)
		{
			_Cilk_for(int line = 0; line < frmSize.height; ++line)
			{
				IppiSize aLine = { frmSize.width, 1 };
				Ipp32f* pStart = buffer[p] + buffer_step*line / sizeof(Ipp32f);
				ippiMulC_32f_C1IR(4080, pStart, buffer_step, aLine);
			}
		}

		// 32f -> 16s
		_Cilk_for(int p = 0; p < 3; ++p)
		{
			_Cilk_for(int line = 0; line < frmSize.height; ++line)
			{
				IppiSize aLine = { frmSize.width, 1 };
				Ipp16s* pDst = planar16s[p] + frmSize.width * line;
				Ipp32f* pSrc = buffer[p] + buffer_step*line / sizeof(Ipp32f);
				ippiConvert_32f16s_C1R(pSrc, buffer_step, pDst, frmSize.width*sizeof(Ipp16s), aLine, IppRoundMode::ippRndNear);
			}
		}
		// SOA -> AOS
		STS = ippiCopy_16s_P3C3R(planar16s, frmSize.width*sizeof(Ipp16s), rgbdata, rgbdata_step, frmSize);
		if (STS != ippStsNoErr)
		{
			ippiFree(rgbdata);
			for (int p = 0; p < 3; ++p)
			{
				if (buffer[p])
				{
					ippiFree(buffer[p]);
					buffer[p] = NULL;
					cache_sdc[p] = NULL;
				}
				
			}
			cache_sdc_size.height = 0;
			cache_sdc_size.width = 0;
			cache_sdc_step = 0;
			return FALSE;
		}
		// RGB -> YC
		STS = rgb2yc48(rgbdata, rgbdata_step, (Ipp16s*)fpip->ycp_edit, rawycp_step, frmSize);
		if (STS != ippStsNoErr)
		{
			ippiFree(rgbdata);
			for (int p = 0; p < 3; ++p)
			{
				if (buffer[p])
				{
					ippiFree(buffer[p]);
					buffer[p] = NULL;
					cache_sdc[p] = NULL;
				}
			}
			cache_sdc_size.height = 0;
			cache_sdc_size.width = 0;
			cache_sdc_step = 0;
			return FALSE;
		}
		ippiFree(rgbdata);
	}
	else
	{
		//None selected
		for (int p = 0; p < 3; ++p)
		{
			if (buffer[p])
			{
				ippiFree(buffer[p]);
				buffer[p] = NULL;
				cache_sdc[p] = NULL;
			}
		}
		cache_sdc_size.height = 0;
		cache_sdc_size.width = 0;
		cache_sdc_step = 0;
		return FALSE;
	}
	//Free buffer
	/*for (int p = 0; p < 3; ++p)
	{
		if (buffer[p])
		{
			ippiFree(buffer[p]);
		}
	}*/
	return TRUE;
}

BOOL func_proc_sb(FILTER *fp, FILTER_PROC_INFO *fpip)
{
	//common vars
	IppiSize frmSize = { fpip->w, fpip->h };
	IppStatus STS = ippStsNoErr;
	int buffer_step = 0; //as byte
	int ycp_pStep = 0;
	int ycp_step = fpip->max_w* sizeof(PIXEL_YC); //as byte
	double alpha = 0, beta = 0;
	beta = 0;
	alpha = (double)fp->track[0] / 100.0;
	if (alpha == 0) return FALSE;
	//Create buffer
	if ((fpip->w != cache_sb_size.width) || (fpip->h != cache_sb_size.height))
	{
		if (cache_sb[0] != NULL)
		{
			ippiFree(cache_sb[0]);
			cache_sb[0] = NULL;
			cache_sb_size.height = 0;
			cache_sb_size.width = 0;
			cache_sb_step = 0;
		}
	}
	if (cache_sb[0] == NULL)
	{
		cache_sb[0] = ippiMalloc_32f_C1(frmSize.width, frmSize.height, &cache_sb_step);
		cache_sb_size.height = frmSize.height;
		cache_sb_size.width = frmSize.width;
	}
	Ipp32f* pbuffer = { 0 };
	
	pbuffer = cache_sb[0];
	buffer_step = cache_sb_step;
	if (!pbuffer) return FALSE;

	Ipp16s* ycpY = (Ipp16s*)fpip->ycp_temp;
	ycp_pStep = frmSize.width*sizeof(Ipp16s);
	/*Ipp16s* ycpY = ippiMalloc_16s_C1(frmSize.width, frmSize.height, &ycp_pStep);
	if (!ycpY)
	{
		ippiFree(pbuffer);
		return FALSE;
	}*/
	
	//Extract Y-plane
	STS = ippiCopy_16s_C3C1R((Ipp16s*)fpip->ycp_edit, ycp_step, ycpY, ycp_pStep, frmSize);
	// 16s->32f
	STS= ippiConvert_16s32f_C1R(ycpY, ycp_pStep, pbuffer, buffer_step, frmSize);
	// Scale to 0~1 for Y-plane
	_Cilk_for(int line = 0; line < frmSize.height; ++line)
	{
		Ipp32f* pStart = (Ipp32f*)(pbuffer + line*buffer_step/sizeof(Ipp32f));
		IppiSize aLine = { frmSize.width, 1 };
		ippiDivC_32f_C1IR(4096.0, pStart, buffer_step, aLine); //max is 4096
		ippiThreshold_LTValGTVal_32f_C1IR(pStart, buffer_step, aLine, 0.0, 0.0, 1.0, 1.0);
	}
	// Run sigB
	STS = sigContrast_32f_C1IR(pbuffer, buffer_step, frmSize, alpha, beta);
	if (STS != ippStsNoErr)
	{
		//ippiFree(ycpY);
		ippiFree(pbuffer);
		pbuffer = NULL;
		cache_sb[0] = NULL;
		cache_sb_size.height = 0;
		cache_sb_size.width = 0;
		cache_sb_step = 0;
		return FALSE;
	}
	_Cilk_for(int line = 0; line < frmSize.height; ++line)
	{
		Ipp32f* pStart = pbuffer + line*buffer_step/sizeof(Ipp32f);
		IppiSize aLine = { frmSize.width, 1 };
		ippiMulC_32f_C1IR(4096.0, pStart, buffer_step, aLine); //max is 4096
		ippiThreshold_LTValGTVal_32f_C1IR(pStart, buffer_step, aLine, 0.0, 0.0, 4096.0, 4096.0);
	}
	//32f->16s
	STS = ippiConvert_32f16s_C1R(pbuffer, buffer_step, ycpY, ycp_pStep, frmSize, IppRoundMode::ippRndNear);
	if (STS != ippStsNoErr)
	{
		//ippiFree(ycpY);
		ippiFree(pbuffer);
		pbuffer = NULL;
		cache_sb[0] = NULL;
		cache_sb_size.height = 0;
		cache_sb_size.width = 0;
		cache_sb_step = 0;
		return FALSE;
	}
	//SOA->AOS
	
	STS = ippiCopy_16s_C1C3R(ycpY, ycp_pStep, (Ipp16s*)fpip->ycp_edit, ycp_step, frmSize);
	if (STS != ippStsNoErr)
	{
		//ippiFree(ycpY);
		ippiFree(pbuffer);
		pbuffer = NULL;
		cache_sb[0] = NULL;
		cache_sb_size.height = 0;
		cache_sb_size.width = 0;
		cache_sb_step = 0;
		return FALSE;
	}
	//ippiFree(ycpY);
	//ippiFree(pbuffer);
	return TRUE;
}

BOOL func_proc_sbm(FILTER *fp, FILTER_PROC_INFO *fpip)
{
	//common vars
	IppiSize frmSize = { fpip->w, fpip->h };
	IppStatus STS = ippStsNoErr;
	int buffer_step = 0; //as byte
	int ycp_pStep = 0;
	int ycp_step = fpip->max_w* sizeof(PIXEL_YC); //as byte
	double alpha = 0, beta = 0;
	beta = 0;
	alpha = (double)fp->track[0] / 100.0;
	if (alpha == 0) return FALSE;
	//Create buffer
	if ((fpip->w != cache_sbm_size.width) || (fpip->h != cache_sbm_size.height))
	{
		if (cache_sbm[0] != NULL)
		{
			ippiFree(cache_sbm[0]);
			cache_sbm[0] = NULL;
			cache_sbm_size.height = 0;
			cache_sbm_size.width = 0;
			cache_sbm_step = 0;
		}
	}
	if (cache_sbm[0] == NULL)
	{
		cache_sbm[0] = ippiMalloc_32f_C1(frmSize.width, frmSize.height, &cache_sbm_step);
		cache_sbm_size.height = frmSize.height;
		cache_sbm_size.width = frmSize.width;
	}
	Ipp32f* pbuffer = { 0 };

	pbuffer = cache_sbm[0];
	buffer_step = cache_sbm_step;
	if (!pbuffer) return FALSE;

	Ipp16s* ycpY = (Ipp16s*)fpip->ycp_temp;
	ycp_pStep = frmSize.width*sizeof(Ipp16s);

	//Extract Y-plane
	STS = ippiCopy_16s_C3C1R((Ipp16s*)fpip->ycp_edit, ycp_step, ycpY, ycp_pStep, frmSize);
	// 16s->32f
	STS = ippiConvert_16s32f_C1R(ycpY, ycp_pStep, pbuffer, buffer_step, frmSize);
	// Scale to 0~1 for Y-plane
	_Cilk_for(int line = 0; line < frmSize.height; ++line)
	{
		Ipp32f* pStart = (Ipp32f*)(pbuffer + line*buffer_step / sizeof(Ipp32f));
		IppiSize aLine = { frmSize.width, 1 };
		ippiDivC_32f_C1IR(4096.0, pStart, buffer_step, aLine); //max is 4096
		ippiThreshold_LTValGTVal_32f_C1IR(pStart, buffer_step, aLine, 0.0, 0.0, 1.0, 1.0);
	}
	// Run sigB
	STS = sigDeContrast_32f_C1IR(pbuffer, buffer_step, frmSize, alpha, beta);
	if (STS != ippStsNoErr)
	{
		//ippiFree(ycpY);
		ippiFree(pbuffer);
		pbuffer = NULL;
		cache_sbm[0] = NULL;
		cache_sbm_size.height = 0;
		cache_sbm_size.width = 0;
		cache_sbm_step = 0;
		return FALSE;
	}
	_Cilk_for(int line = 0; line < frmSize.height; ++line)
	{
		Ipp32f* pStart = pbuffer + line*buffer_step / sizeof(Ipp32f);
		IppiSize aLine = { frmSize.width, 1 };
		ippiMulC_32f_C1IR(4096.0, pStart, buffer_step, aLine); //max is 4096
		ippiThreshold_LTValGTVal_32f_C1IR(pStart, buffer_step, aLine, 0.0, 0.0, 4096.0, 4096.0);
	}
	//32f->16s
	STS = ippiConvert_32f16s_C1R(pbuffer, buffer_step, ycpY, ycp_pStep, frmSize, IppRoundMode::ippRndNear);
	if (STS != ippStsNoErr)
	{
		//ippiFree(ycpY);
		ippiFree(pbuffer);
		pbuffer = NULL;
		cache_sbm[0] = NULL;
		cache_sbm_size.height = 0;
		cache_sbm_size.width = 0;
		cache_sbm_step = 0;
		return FALSE;
	}
	//SOA->AOS

	STS = ippiCopy_16s_C1C3R(ycpY, ycp_pStep, (Ipp16s*)fpip->ycp_edit, ycp_step, frmSize);
	if (STS != ippStsNoErr)
	{
		//ippiFree(ycpY);
		ippiFree(pbuffer);
		pbuffer = NULL;
		cache_sbm[0] = NULL;
		cache_sbm_size.height = 0;
		cache_sbm_size.width = 0;
		cache_sbm_step = 0;
		return FALSE;
	}
	//ippiFree(ycpY);
	//ippiFree(pbuffer);
	return TRUE;
}

BOOL func_update_sc(FILTER *fp, int status)
{
	if ((status == FILTER_UPDATE_STATUS_CHECK) ||
		(status == FILTER_UPDATE_STATUS_CHECK + 1) ||
		(status == FILTER_UPDATE_STATUS_CHECK + 2))
	{
		if (fp->check[0] || fp->check[1] || fp->check[2])
		{
			fp->check[3] = FALSE;
			fp->check[4] = FALSE;
			fp->check[5] = FALSE;
		}
	}

	if ((status == FILTER_UPDATE_STATUS_CHECK + 3) ||
		(status == FILTER_UPDATE_STATUS_CHECK + 4) ||
		(status == FILTER_UPDATE_STATUS_CHECK + 5))
	{
		if (fp->check[3] || fp->check[4] || fp->check[5])
		{
			fp->check[0] = FALSE;
			fp->check[1] = FALSE;
			fp->check[2] = FALSE;
		}
	}
	fp->exfunc->filter_window_update(fp);
	return TRUE;
}
BOOL func_update_sdc(FILTER *fp, int status)
{
	if ((status == FILTER_UPDATE_STATUS_CHECK) ||
		(status == FILTER_UPDATE_STATUS_CHECK + 1) ||
		(status == FILTER_UPDATE_STATUS_CHECK + 2))
	{
		if (fp->check[0] || fp->check[1] || fp->check[2])
		{
			fp->check[3] = FALSE;
			fp->check[4] = FALSE;
			fp->check[5] = FALSE;
		}
	}

	if ((status == FILTER_UPDATE_STATUS_CHECK + 3) ||
		(status == FILTER_UPDATE_STATUS_CHECK + 4) ||
		(status == FILTER_UPDATE_STATUS_CHECK + 5))
	{
		if (fp->check[3] || fp->check[4] || fp->check[5])
		{
			fp->check[0] = FALSE;
			fp->check[1] = FALSE;
			fp->check[2] = FALSE;
		}
	}
	fp->exfunc->filter_window_update(fp);
	return TRUE;
}

BOOL func_WndProc_sc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam, void *editp, FILTER *fp)
{
	if (message == WM_COMMAND)
	{
		if (wparam == MID_FILTER_BUTTON + 6)
		{
			MessageBox(NULL,
				"SigContrast+ by MT(2015)\n"
				"---------------------------\n"
				"This plugin INCREASE contrast with pixels near to midtone\n"
				"changing more than those near to the black and white ends.\n"
				"This is similar to ImageMagick6's Sigmoidal contrast but\n"
				"this do not use lookup-table.\n\n"
				"Usage\n"
				"---------------------------\n"
				"1. Select the channel(s) you want to adjust\n"
				"(Note: YCbCr mode is mutually exclusive with RGB mode)\n"
				"2. Activate plugin\n"
				"3. Adjust [Strength] and [Midtone]\n"
				"(Note: internally, Strength is divided by 100 while\n"
				"Midtone is divided by 1000 prior feeding into\n"
				"sigmoidal transform function)\n", "INFO", MB_OK | MB_ICONINFORMATION);
			return FALSE;
		}
	}
	if (message == WM_FILTER_CHANGE_ACTIVE)
	{
		if (!fp->exfunc->is_filter_active(fp)) //if deactivated
		{
			//clear cache
			for (int p = 0; p < 3; ++p)
			{
				if (cache_sc[p] != NULL)
				{
					ippiFree(cache_sc[p]);
					cache_sc[p] = NULL;
				}
			}
			cache_sc_size.height = 0;
			cache_sc_size.width = 0;
			cache_sc_step = 0;
		}
		return FALSE;
	}
	return FALSE;
}
BOOL func_WndProc_sdc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam, void *editp, FILTER *fp)
{
	if (message == WM_COMMAND)
	{
		if (wparam == MID_FILTER_BUTTON + 6)
		{
			MessageBox(NULL,
				"SigContrast- by MT(2015)\n"
				"---------------------------\n"
				"This plugin DECREASE contrast with pixels near to midtone\n"
				"changing more than those near to the black and white ends.\n"
				"This is similar to ImageMagick6's Sigmoidal contrast but\n"
				"this do not use lookup-table.\n\n"
				"Usage\n"
				"---------------------------\n"
				"1. Select the channel(s) you want to adjust\n"
				"(Note: YCbCr mode is mutually exclusive with RGB mode)\n"
				"2. Activate plugin\n"
				"3. Adjust [Strength] and [Midtone]\n"
				"(Note: internally, Strength is divided by 100 while\n"
				"Midtone is divided by 1000 prior feeding into\n"
				"sigmoidal transform function)\n", "INFO", MB_OK | MB_ICONINFORMATION);
			return FALSE;
		}
	}
	if (message == WM_FILTER_CHANGE_ACTIVE)
	{
		if (!fp->exfunc->is_filter_active(fp)) //if deactivated
		{
			//clear cache
			for (int p = 0; p < 3; ++p)
			{
				if (cache_sdc[p] != NULL)
				{
					ippiFree(cache_sdc[p]);
					cache_sdc[p] = NULL;
				}
			}
			cache_sdc_size.height = 0;
			cache_sdc_size.width = 0;
			cache_sdc_step = 0;
		}
		return FALSE;
	}
	return FALSE;
}

BOOL func_exit_sc(FILTER *fp)
{
	for (int p = 0; p < 3; ++p)
	{
		if (cache_sc[p] != NULL)
		{
			ippiFree(cache_sc[p]);
			cache_sc[p] = NULL;
		}
	}
	cache_sc_size.height = 0;
	cache_sc_size.width = 0;
	cache_sc_step = 0;
	return TRUE;
}

BOOL func_exit_sdc(FILTER *fp)
{
	for (int p = 0; p < 3; ++p)
	{
		if (cache_sdc[p] != NULL)
		{
			ippiFree(cache_sdc[p]);
			cache_sdc[p] = NULL;
		}
	}
	cache_sdc_size.height = 0;
	cache_sdc_size.width = 0;
	cache_sdc_step = 0;
	return TRUE;
}

BOOL func_WndProc_sb(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam, void *editp, FILTER *fp)
{
	
	if (message == WM_FILTER_CHANGE_ACTIVE)
	{
		if (!fp->exfunc->is_filter_active(fp)) //if deactivated
		{
			//clear cache
			if (cache_sb[0] != NULL)
			{
				ippiFree(cache_sb[0]);
				cache_sb[0] = NULL;
			}
			
			cache_sb_size.height = 0;
			cache_sb_size.width = 0;
			cache_sb_step = 0;
		}
		return FALSE;
	}
	return FALSE;
}

BOOL func_exit_sb(FILTER *fp)
{
	if (cache_sb[0] != NULL)
	{
		ippiFree(cache_sb[0]);
		cache_sb[0] = NULL;
	}
	
	cache_sb_size.height = 0;
	cache_sb_size.width = 0;
	cache_sb_step = 0;
	return TRUE;
}

BOOL func_WndProc_sbm(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam, void *editp, FILTER *fp)
{

	if (message == WM_FILTER_CHANGE_ACTIVE)
	{
		if (!fp->exfunc->is_filter_active(fp)) //if deactivated
		{
			//clear cache
			if (cache_sbm[0] != NULL)
			{
				ippiFree(cache_sbm[0]);
				cache_sbm[0] = NULL;
			}

			cache_sbm_size.height = 0;
			cache_sbm_size.width = 0;
			cache_sbm_step = 0;
		}
		return FALSE;
	}
	return FALSE;
}

BOOL func_exit_sbm(FILTER *fp)
{
	if (cache_sbm[0] != NULL)
	{
		ippiFree(cache_sbm[0]);
		cache_sbm[0] = NULL;
	}

	cache_sbm_size.height = 0;
	cache_sbm_size.width = 0;
	cache_sbm_step = 0;
	return TRUE;
}