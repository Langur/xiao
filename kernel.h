/**
  kernel.h
  
  Copyright (c) 2013-2021 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#ifndef __KERNEL_H__
#define __KERNEL_H__

#include <defines.h>
#include <itron.h>

/*
 * カーネル共通定数(オブジェクト属性)
 */
#define	TA_HLNG			(0x00)		/* 高級言語用インタフェースで処理単位を起動 */
#define TA_ASM			(0x01)		/* アセンブリ言語用インタフェースで処理単位を起動 */
#define TA_ACT			(0x02)		/* タスクを実行可能状態で生成 */

#define TA_TFIFO		(0x00)		/* タスクの待ち行列をFIFO順にする */
#define TA_TPRI			(0x01)		/* タスクの待ち行列を優先度順にする */

#define TA_MFIFO		(0x00)		/* メッセージキューをFIFO順にする */
#define TA_MPRI			(0x02)		/* メッセージキューを優先度順にする */

/*
 * カーネル共通定数(その他)
 */
#define	TSK_SELF		(0)		/* 自タスク指定 */
#define	TSK_NONE		(0)		/* 該当するタスクが無い */

/*
 * カーネル共通構成定数(優先度)
 */
#define	TMIN_TPRI		(1)		/* タスク優先度の最小値 */
#define	TMAX_TPRI		(4)		/* タスク優先度の最大値 */
#define	TPRI_LOW		TMAX_TPRI
#define	TPRI_MIDDLE		(3)
#define	TPRI_HIGH		(2)
#define	TPRI_MAX		TMIN_TPRI
#define	TMIN_MPRI		(1)		/* メッセージ優先度の最小値 */
#define	TMAX_MPRI		(16)		/* メッセージ優先度の最大値 */

/*
 * カーネル共通構成定数(バージョン情報)
 */
#define	TKERNEL_MAKER		(0x0008)	/* カーネルのメーカーコード(個人) */
#define	TKERNEL_PRID		(0x0001)	/* カーネルの識別番号 */
#define	TKERNEL_SPVER		(0x5403)	/* ITRON仕様のバージョン番号 */
#define	TKERNEL_PRVER		(0x1000)	/* カーネルのバージョン番号 */

/*
 * カーネル構成定数(タスク管理機能)
 */
#define	TMAX_ACTCNT		(1)		/* 起動要求キューイング数の最大値 */

/*
 * 機能コード
 */
#define TFN_CRE_TSK		(-(0x05))
#define TFN_ACRE_TSK		(-(0xC1))
#define TFN_DEL_TSK		(-(0x06))
#define TFN_ACT_TSK		(-(0x07))
#define TFN_IACT_TSK		(-(0x71))
#define TFN_CAN_ACT		(-(0x08))
#define TFN_STA_TSK		(-(0x09))
#define TFN_EXT_TSK		(-(0x0a))
#define TFN_EXD_TSK		(-(0x0b))
#define TFN_TER_TSK		(-(0x0c))
#define TFN_CHG_PRI		(-(0x0d))
#define TFN_GET_PRI		(-(0x0e))
#define TFN_REF_TSK		(-(0x0f))
#define TFN_REF_TST		(-(0x10))

/*
 * パケット定義(タスク管理機能)
 */
typedef struct t_ctsk {
	ATR	tskatr;		/* タスク属性 */
	VP_INT	exinf;		/* タスクの拡張情報 */
	FP	task;		/* タスクの起動番地 */
	PRI	itskpri;	/* タスクの起動時優先度 */
	SIZE	stksz;		/* タスクのスタックサイズ */
	VP	stk;		/* タスクのスタック領域の先頭番地 */
} T_CTSK;

typedef struct t_rtsk {
	STAT	tskstat;	/* タスク状態 */
	PRI	tskpri;		/* タスクの現在優先度 */
	PRI	tskbpri;	/* タスクのベース優先度 */
	STAT	tskwait;	/* 待ち要因 */
	ID	wobjid;		/* 待ち対象のオブジェクトID番号 */
	TMO	lefttmo;	/* タイムアウトするまでの時間 */
	UINT	actcnt;		/* 起動要求キューイング数 */
	UINT	wupcnt;		/* 起床要求キューイング数 */
	UINT	suscnt;		/* 強制待ち要求ネスト数 */
} T_RTSK;

typedef struct t_rtst {
	STAT	tskstat;	/* タスク状態 */
	STAT	tskwait;	/* 待ち要因 */
} T_RTST;

/*
 * パケット定義(割込み管理機能)
 */
typedef struct t_dinh {
	ATR	inhatr;		/* 割込みハンドラ属性 */
	FP	inthdr;		/* 割込みハンドラの起動番地 */
} T_DINH;

/*
 * サービスコール定義
 */
ER act_tsk(ID tskid);
ER iact_tsk(ID tskid);
void ext_tsk(void);

#endif /* __KERNEL_H__ */
