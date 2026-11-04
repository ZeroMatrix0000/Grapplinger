/*
 * FileName:     IGameInput.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/04
 *
 * ゲーム入力のインタフェース
 */

#pragma once

#include "../Commons/Systems/OnlyOne.h"

class IGameInput : public Systems::OnlyOne
{

public:


	/* メンバ関数 */

	// コンストラクタ
	IGameInput()
		: OnlyOne{ typeid(IGameInput) }
	{
	}

	// プレイヤーの移動入力を取得
	virtual Math::Vector3 GetPlayerMove()         const = 0;
	// プレイヤーのカメラの回転入力を取得
	virtual Math::Vector2 GetPlayerCameraRotate() const = 0;
	// プレイヤーのダッシュ入力を取得
	virtual bool          GetPlayerDash()         const = 0;
	// プレイヤーのジャンプ入力を取得
	virtual bool          GetPlayerJump()         const = 0;
	// プレイヤーの糸伸ばし入力を取得
	virtual bool          GetPlayerExpandWire() const = 0;

	// フィールド上のオブジェクトのインタラクト入力を取得
	virtual bool GetFieldInteract() const = 0;

	// テキスト送り入力
	virtual bool GetTextScroll() const = 0;

	// アクティブ化・非アクティブ化
	virtual void SetActive(bool isActive) = 0;
	// アクティブかどうかを取得
	virtual bool GetIsActive() const = 0;

};
