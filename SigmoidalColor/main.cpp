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
//		フィルタ構造体定義
//---------------------------------------------------------------------
/*#define	TRACK_N	3														//	トラックバーの数
TCHAR	*track_name[] = { "track0", "track1", "track2" };	//	トラックバーの名前
int		track_default[] = { 0, 0, 0 };	//	トラックバーの初期値
int		track_s[] = { -999, -999, -999 };	//	トラックバーの下限値
int		track_e[] = { +999, +999, +999 };	//	トラックバーの上限値

#define	CHECK_N	2														//	チェックボックスの数
TCHAR	*check_name[] = { "check0", "check1" };				//	チェックボックスの名前
int		check_default[] = { 0, 0 };				//	チェックボックスの初期値 (値は0か1)

FILTER_DLL filter = {
	FILTER_FLAG_EX_INFORMATION,	//	フィルタのフラグ
	//	FILTER_FLAG_ALWAYS_ACTIVE		: フィルタを常にアクティブにします
	//	FILTER_FLAG_CONFIG_POPUP		: 設定をポップアップメニューにします
	//	FILTER_FLAG_CONFIG_CHECK		: 設定をチェックボックスメニューにします
	//	FILTER_FLAG_CONFIG_RADIO		: 設定をラジオボタンメニューにします
	//	FILTER_FLAG_EX_DATA				: 拡張データを保存出来るようにします。
	//	FILTER_FLAG_PRIORITY_HIGHEST	: フィルタのプライオリティを常に最上位にします
	//	FILTER_FLAG_PRIORITY_LOWEST		: フィルタのプライオリティを常に最下位にします
	//	FILTER_FLAG_WINDOW_THICKFRAME	: サイズ変更可能なウィンドウを作ります
	//	FILTER_FLAG_WINDOW_SIZE			: 設定ウィンドウのサイズを指定出来るようにします
	//	FILTER_FLAG_DISP_FILTER			: 表示フィルタにします
	//	FILTER_FLAG_EX_INFORMATION		: フィルタの拡張情報を設定できるようにします
	//	FILTER_FLAG_NO_CONFIG			: 設定ウィンドウを表示しないようにします
	//	FILTER_FLAG_AUDIO_FILTER		: オーディオフィルタにします
	//	FILTER_FLAG_RADIO_BUTTON		: チェックボックスをラジオボタンにします
	//	FILTER_FLAG_WINDOW_HSCROLL		: 水平スクロールバーを持つウィンドウを作ります
	//	FILTER_FLAG_WINDOW_VSCROLL		: 垂直スクロールバーを持つウィンドウを作ります
	//	FILTER_FLAG_IMPORT				: インポートメニューを作ります
	//	FILTER_FLAG_EXPORT				: エクスポートメニューを作ります
	0, 0,						//	設定ウインドウのサイズ (FILTER_FLAG_WINDOW_SIZEが立っている時に有効)
	"サンプルフィルタ",			//	フィルタの名前
	TRACK_N,					//	トラックバーの数 (0なら名前初期値等もNULLでよい)
	track_name,					//	トラックバーの名前郡へのポインタ
	track_default,				//	トラックバーの初期値郡へのポインタ
	track_s, track_e,			//	トラックバーの数値の下限上限 (NULLなら全て0～256)
	CHECK_N,					//	チェックボックスの数 (0なら名前初期値等もNULLでよい)
	check_name,					//	チェックボックスの名前郡へのポインタ
	check_default,				//	チェックボックスの初期値郡へのポインタ
	func_proc,					//	フィルタ処理関数へのポインタ (NULLなら呼ばれません)
	NULL,						//	開始時に呼ばれる関数へのポインタ (NULLなら呼ばれません)
	NULL,						//	終了時に呼ばれる関数へのポインタ (NULLなら呼ばれません)
	NULL,						//	設定が変更されたときに呼ばれる関数へのポインタ (NULLなら呼ばれません)
	NULL,						//	設定ウィンドウにウィンドウメッセージが来た時に呼ばれる関数へのポインタ (NULLなら呼ばれません)
	NULL, NULL,					//	システムで使いますので使用しないでください
	NULL,						//  拡張データ領域へのポインタ (FILTER_FLAG_EX_DATAが立っている時に有効)
	NULL,						//  拡張データサイズ (FILTER_FLAG_EX_DATAが立っている時に有効)
	"サンプルフィルタ version 0.06 by ＫＥＮくん",
	//  フィルタ情報へのポインタ (FILTER_FLAG_EX_INFORMATIONが立っている時に有効)
	NULL,						//	セーブが開始される直前に呼ばれる関数へのポインタ (NULLなら呼ばれません)
	NULL,						//	セーブが終了した直前に呼ばれる関数へのポインタ (NULLなら呼ばれません)
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
#define	TRACK_N_SC	2														//	トラックバーの数
TCHAR	*track_name_sc[] = { "Strength", "Midtone"};	//	トラックバーの名前
int		track_default_sc[] = { 300, 500 };	//	Str /100; Midtone/1000;
int		track_s_sc[] = { 1, 1 };	//	トラックバーの下限値
int		track_e_sc[] = { 2000, 1000 };	//	トラックバーの上限値

#define	CHECK_N_SC	7														//	チェックボックスの数
TCHAR	*check_name_sc[] = { "Y", "Cb", "Cr", "R", "G", "B", "HELP" };				//	チェックボックスの名前
int		check_default_sc[] = { 1, 1, 1, 0, 0, 0, -1 };				//	チェックボックスの初期値 (値は0か1)

//---------------------------------------------------------------------
//		Sigmoidal DeContrast
//---------------------------------------------------------------------
#define	TRACK_N_SDC	2														//	トラックバーの数
TCHAR	*track_name_sdc[] = { "Strength", "Midtone" };	//	トラックバーの名前
int		track_default_sdc[] = { 300, 500 };	//	Str /100; Midtone/1000;
int		track_s_sdc[] = { 1, 1 };	//	トラックバーの下限値
int		track_e_sdc[] = { 2000, 1000 };	//	トラックバーの上限値

#define	CHECK_N_SDC	7														//	チェックボックスの数
TCHAR	*check_name_sdc[] = { "Y", "Cb", "Cr", "R", "G", "B", "HELP" };				//	チェックボックスの名前
int		check_default_sdc[] = { 1, 1, 1, 0, 0, 0, -1 };				//	チェックボックスの初期値 (値は0か1)

//---------------------------------------------------------------------
//		Sigmoidal Brightness+
//---------------------------------------------------------------------
#define	TRACK_N_SB	1														//	トラックバーの数
TCHAR	*track_name_sb[] = { "Strength" };	//	トラックバーの名前
int		track_default_sb[] = { 300 };	//	Str/100;
int		track_s_sb[] = { 1 };	//	トラックバーの下限値
int		track_e_sb[] = { 2000 };	//	トラックバーの上限値

//---------------------------------------------------------------------
//		Sigmoidal Brightness-
//---------------------------------------------------------------------
#define	TRACK_N_SBM	1														//	トラックバーの数
TCHAR	*track_name_sbm[] = { "Strength" };	//	トラックバーの名前
int		track_default_sbm[] = { 300 };	//	Str/100;
int		track_s_sbm[] = { 1 };	//	トラックバーの下限値
int		track_e_sbm[] = { 2000 };	//	トラックバーの上限値

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
	FILTER_FLAG_EX_INFORMATION,	//	フィルタのフラグ
	0, 0,						//	設定ウインドウのサイズ (FILTER_FLAG_WINDOW_SIZEが立っている時に有効)
	"SigContrast+",			//	フィルタの名前
	TRACK_N_SC,					//	トラックバーの数 (0なら名前初期値等もNULLでよい)
	track_name_sc,					//	トラックバーの名前郡へのポインタ
	track_default_sc,				//	トラックバーの初期値郡へのポインタ
	track_s_sc, track_e_sc,			//	トラックバーの数値の下限上限 (NULLなら全て0～256)
	CHECK_N_SC,					//	チェックボックスの数 (0なら名前初期値等もNULLでよい)
	check_name_sc,					//	チェックボックスの名前郡へのポインタ
	check_default_sc,				//	チェックボックスの初期値郡へのポインタ
	func_proc_sc,					//	フィルタ処理関数へのポインタ (NULLなら呼ばれません)
	func_init_sc,						//	開始時に呼ばれる関数へのポインタ (NULLなら呼ばれません)
	func_exit_sc,						//	終了時に呼ばれる関数へのポインタ (NULLなら呼ばれません)
	func_update_sc,						//	設定が変更されたときに呼ばれる関数へのポインタ (NULLなら呼ばれません)
	func_WndProc_sc,						//	設定ウィンドウにウィンドウメッセージが来た時に呼ばれる関数へのポインタ (NULLなら呼ばれません)
	NULL, NULL,					//	システムで使いますので使用しないでください
	NULL,						//  拡張データ領域へのポインタ (FILTER_FLAG_EX_DATAが立っている時に有効)
	NULL,						//  拡張データサイズ (FILTER_FLAG_EX_DATAが立っている時に有効)
	"SigContrast+ v1.0 by MT",
	//  フィルタ情報へのポインタ (FILTER_FLAG_EX_INFORMATIONが立っている時に有効)
	NULL,						//	セーブが開始される直前に呼ばれる関数へのポインタ (NULLなら呼ばれません)
	NULL,						//	セーブが終了した直前に呼ばれる関数へのポインタ (NULLなら呼ばれません)
};
FILTER_DLL fSigDeCon = {
	FILTER_FLAG_EX_INFORMATION,	//	フィルタのフラグ
	0, 0,						//	設定ウインドウのサイズ (FILTER_FLAG_WINDOW_SIZEが立っている時に有効)
	"SigContrast-",			//	フィルタの名前
	TRACK_N_SDC,					//	トラックバーの数 (0なら名前初期値等もNULLでよい)
	track_name_sdc,					//	トラックバーの名前郡へのポインタ
	track_default_sdc,				//	トラックバーの初期値郡へのポインタ
	track_s_sdc, track_e_sdc,			//	トラックバーの数値の下限上限 (NULLなら全て0～256)
	CHECK_N_SDC,					//	チェックボックスの数 (0なら名前初期値等もNULLでよい)
	check_name_sdc,					//	チェックボックスの名前郡へのポインタ
	check_default_sdc,				//	チェックボックスの初期値郡へのポインタ
	func_proc_sdc,					//	フィルタ処理関数へのポインタ (NULLなら呼ばれません)
	func_init_sdc,						//	開始時に呼ばれる関数へのポインタ (NULLなら呼ばれません)
	func_exit_sdc,						//	終了時に呼ばれる関数へのポインタ (NULLなら呼ばれません)
	func_update_sdc,						//	設定が変更されたときに呼ばれる関数へのポインタ (NULLなら呼ばれません)
	func_WndProc_sdc,						//	設定ウィンドウにウィンドウメッセージが来た時に呼ばれる関数へのポインタ (NULLなら呼ばれません)
	NULL, NULL,					//	システムで使いますので使用しないでください
	NULL,						//  拡張データ領域へのポインタ (FILTER_FLAG_EX_DATAが立っている時に有効)
	NULL,						//  拡張データサイズ (FILTER_FLAG_EX_DATAが立っている時に有効)
	"SigContrast- v1.0 by MT",
	//  フィルタ情報へのポインタ (FILTER_FLAG_EX_INFORMATIONが立っている時に有効)
	NULL,						//	セーブが開始される直前に呼ばれる関数へのポインタ (NULLなら呼ばれません)
	NULL,						//	セーブが終了した直前に呼ばれる関数へのポインタ (NULLなら呼ばれません)
};

FILTER_DLL fSigB = {
	FILTER_FLAG_EX_INFORMATION,	//	フィルタのフラグ
	0, 0,						//	設定ウインドウのサイズ (FILTER_FLAG_WINDOW_SIZEが立っている時に有効)
	"SigBrightness+",			//	フィルタの名前
	TRACK_N_SB,					//	トラックバーの数 (0なら名前初期値等もNULLでよい)
	track_name_sb,					//	トラックバーの名前郡へのポインタ
	track_default_sb,				//	トラックバーの初期値郡へのポインタ
	track_s_sb, track_e_sb,			//	トラックバーの数値の下限上限 (NULLなら全て0～256)
	NULL,					//	チェックボックスの数 (0なら名前初期値等もNULLでよい)
	NULL,					//	チェックボックスの名前郡へのポインタ
	NULL,				//	チェックボックスの初期値郡へのポインタ
	func_proc_sb,					//	フィルタ処理関数へのポインタ (NULLなら呼ばれません)
	func_init_sb,						//	開始時に呼ばれる関数へのポインタ (NULLなら呼ばれません)
	func_exit_sb,						//	終了時に呼ばれる関数へのポインタ (NULLなら呼ばれません)
	NULL,						//	設定が変更されたときに呼ばれる関数へのポインタ (NULLなら呼ばれません)
	func_WndProc_sb,						//	設定ウィンドウにウィンドウメッセージが来た時に呼ばれる関数へのポインタ (NULLなら呼ばれません)
	NULL, NULL,					//	システムで使いますので使用しないでください
	NULL,						//  拡張データ領域へのポインタ (FILTER_FLAG_EX_DATAが立っている時に有効)
	NULL,						//  拡張データサイズ (FILTER_FLAG_EX_DATAが立っている時に有効)
	"SigBrightness+ v1.0 by MT",
	//  フィルタ情報へのポインタ (FILTER_FLAG_EX_INFORMATIONが立っている時に有効)
	NULL,						//	セーブが開始される直前に呼ばれる関数へのポインタ (NULLなら呼ばれません)
	NULL,						//	セーブが終了した直前に呼ばれる関数へのポインタ (NULLなら呼ばれません)
};

FILTER_DLL fSigBM = {
	FILTER_FLAG_EX_INFORMATION,	//	フィルタのフラグ
	0, 0,						//	設定ウインドウのサイズ (FILTER_FLAG_WINDOW_SIZEが立っている時に有効)
	"SigBrightness-",			//	フィルタの名前
	TRACK_N_SBM,					//	トラックバーの数 (0なら名前初期値等もNULLでよい)
	track_name_sbm,					//	トラックバーの名前郡へのポインタ
	track_default_sbm,				//	トラックバーの初期値郡へのポインタ
	track_s_sbm, track_e_sbm,			//	トラックバーの数値の下限上限 (NULLなら全て0～256)
	NULL,					//	チェックボックスの数 (0なら名前初期値等もNULLでよい)
	NULL,					//	チェックボックスの名前郡へのポインタ
	NULL,				//	チェックボックスの初期値郡へのポインタ
	func_proc_sbm,					//	フィルタ処理関数へのポインタ (NULLなら呼ばれません)
	func_init_sbm,						//	開始時に呼ばれる関数へのポインタ (NULLなら呼ばれません)
	func_exit_sbm,						//	終了時に呼ばれる関数へのポインタ (NULLなら呼ばれません)
	NULL,						//	設定が変更されたときに呼ばれる関数へのポインタ (NULLなら呼ばれません)
	func_WndProc_sbm,						//	設定ウィンドウにウィンドウメッセージが来た時に呼ばれる関数へのポインタ (NULLなら呼ばれません)
	NULL, NULL,					//	システムで使いますので使用しないでください
	NULL,						//  拡張データ領域へのポインタ (FILTER_FLAG_EX_DATAが立っている時に有効)
	NULL,						//  拡張データサイズ (FILTER_FLAG_EX_DATAが立っている時に有効)
	"SigBrightness- v1.0 by MT",
	//  フィルタ情報へのポインタ (FILTER_FLAG_EX_INFORMATIONが立っている時に有効)
	NULL,						//	セーブが開始される直前に呼ばれる関数へのポインタ (NULLなら呼ばれません)
	NULL,						//	セーブが終了した直前に呼ばれる関数へのポインタ (NULLなら呼ばれません)
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
//		フィルタ構造体のポインタを渡す関数
//---------------------------------------------------------------------
/*EXTERN_C FILTER_DLL __declspec(dllexport) * __stdcall GetFilterTable(void)
{
	return &filter;
}*/
//	下記のようにすると1つのaufファイルで複数のフィルタ構造体を渡すことが出来ます
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