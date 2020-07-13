/**
  itron.h
  
  Copyright (c) 2013-2020 Akihisa ONODA
  
  This software is released under the MIT License.
  http://opensource.org/licenses/mit-license.php
*/

#ifndef __ITRON_H__
#define __ITRON_H__

#include "defines.h"

/*
 * 一般定義
 */
#define NULL			(0)
#define true			(1)
#define false			(0)
#define TRUE			true
#define FALSE			false

/*
 * ITRON仕様共通データ型
 */
typedef int8_t			B;
typedef int16_t			H;
typedef int32_t			W;
typedef int64_t			D;
typedef uint8_t			UB;
typedef uint16_t		UH;
typedef uint32_t		UW;
typedef uint64_t		UD;
typedef uint8_t			VB;
typedef uint16_t		VH;
typedef uint32_t		VW;
typedef uint64_t		VD;
typedef	void			*VP;
typedef	void			(*FP)();
typedef int_t			INT;
typedef uint_t			UINT;
typedef bool_t			BOOL;
typedef int32_t			FN;		/* 符号付き16bit以上 */
typedef int_t			ER;		/* 符号付き 8bit以上 */
typedef int_t			ID;		/* 符号付き16bit以上 */
typedef uint32_t		ATR;		/* 符号無し 8bit以上 */
typedef uint32_t		STAT;		/* 符号無し16bit以上 */
typedef uint32_t		MODE;		/* 符号無し 8bit以上 */
typedef int32_t			PRI;		/* 符号付き16bit以上 */
typedef uintptr_t		SIZE;		/* 符号無しポインタと同じ */
typedef int32_t			TMO;		/* 符号付き16bit以上 */
typedef uint32_t		RELTIM;		/* 符号無し16bit以上 */
typedef uint32_t		SYSTIM;		/* 符号無し16bit以上 */
typedef	intptr_t		VP_INT;
typedef int_t			ER_BOOL;
typedef int_t			ER_ID;
typedef int_t			ER_UINT;

/*
 * メインエラーコード
 */
#define E_OK			(0)
#define	E_SYS			(-5)		/* システムエラー */
#define	E_NOSPT			(-9)		/* 未サポート機能 */
#define	E_RSFN			(-10)		/* 予約機能コード */
#define	E_RSATR			(-11)		/* 予約属性 */
#define	E_PAR			(-17)		/* パラメータエラー */
#define	E_ID			(-18)		/* 不正ID番号 */
#define	E_CTX			(-25)		/* コンテキストエラー */
#define	E_MACV			(-26)		/* メモリアクセス違反 */
#define	E_OACV			(-27)		/* オブジェクトアクセス違反 */
#define	E_ILUSE			(-28)		/* サービスコール不正使用 */
#define	E_NOMEM			(-33)		/* メモリ不足 */
#define	E_NOID			(-34)		/* ID番号不足 */
#define	E_OBJ			(-41)		/* オブジェクト状態エラー */
#define	E_NOEXS			(-42)		/* オブジェクト未生成 */
#define	E_QOVR			(-43)		/* キューイングオーバーフロー */
#define	E_RLWAI			(-49)		/* 待ち状態の強制解除 */
#define	E_TMOUT			(-50)		/* ポーリング失敗またはタイムアウト */
#define	E_DLT			(-51)		/* 待ちオブジェクトの削除 */
#define	E_CLS			(-52)		/* 待ちオブジェクトの状態変化 */
#define	E_WBLK			(-57)		/* ノンブロッキング受付け */
#define	E_BOVR			(-58)		/* バッファオーバーフロー */

/*
 * オブジェクト属性
 */
#define TA_NULL			(0)

/*
 * タイムアウト指定
 */
#define TMO_POL			(0)
#define TMO_FEVR		(-1)
#define TMO_NBLK		(-2)

/*
 * ITRON仕様共通マクロ
 */
#define	ERCD(mercd, sercd)	(((sercd) << 8) | ((mercd) & 0xFF))
#define	MERCD(ercd)		(((ercd) & 0x80) == 0 ? ((ercd) & 0xFF) : ((ercd) | (~(0xFF))))
#define	SERCD(ercd)		((~((~(0)) >> 8)) | ((ercd) >> 8))

#endif /* __ITRON_H__ */
