/*
 * FileName:     GameInput.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/11/16
 *
 * ゲームの入力
 */

#include "Pch.h"
#include "GameInput.h"

 // コンストラクタ
GameInput::GameInput(Ref<const Systems::IInput> input)
	: IGameInput{}
	, m_input{ input }
	, m_isActive{ true }
{
}

// プレイヤーの移動入力を取得
Math::Vector3 GameInput::GetPlayerMove() const
{
	if (!m_isActive)
	{
		return Math::Vector3::Zero;
	}

	// 方向
	Math::Vector3 direction{};

	// 上
	if (m_input->GetKey(KeyName::W) || m_input->GetKey(KeyName::Up))
	{
		direction += Math::Vector3::Forward;
	}
	// 左
	if (m_input->GetKey(KeyName::A) || m_input->GetKey(KeyName::Left))
	{
		direction += Math::Vector3::Left;
	}
	// 下
	if (m_input->GetKey(KeyName::S) || m_input->GetKey(KeyName::Down))
	{
		direction += Math::Vector3::Backward;
	}
	// 右
	if (m_input->GetKey(KeyName::D) || m_input->GetKey(KeyName::Right))
	{
		direction += Math::Vector3::Right;
	}

	// 極小値ならゼロに
	if (direction.LengthSquared() < Math::EPSILON)
	{
		return Math::Vector3::Zero;
	}
	// でなければ正規化
	else
	{
		direction.Normalize();
		return direction;
	}
}

// カメラの回転を取得
Math::Vector2 GameInput::GetPlayerCameraRotate() const
{
	if (!m_isActive)
	{
		return Math::Vector2::Zero;
	}

	return m_input->GetMouseMovement() * MOUSE_COEFFICIENT;
}

// プレイヤーのダッシュ入力を取得
bool GameInput::GetPlayerDash() const
{
	if (!m_isActive)
	{
		return false;
	}

	return m_input->GetKey(KeyName::LeftShift) || m_input->GetKey(KeyName::RightShift);
}

// プレイヤーのジャンプ入力を取得
bool GameInput::GetPlayerJump() const
{
	if (!m_isActive)
	{
		return false;
	}

	return m_input->GetKeyDown(KeyName::Space);
}

// プレイヤーの糸伸ばし入力を取得
bool GameInput::GetPlayerExpandWire() const
{
	if (!m_isActive)
	{
		return false;
	}

	return m_input->GetMouseButtonDown(MouseButtonName::Left);
}

// フィールド上のオブジェクトのインタラクト入力を取得
bool GameInput::GetFieldInteract() const
{
	if (!m_isActive)
	{
		return false;
	}

	return m_input->GetKeyDown(KeyName::Z);
}

// テキスト送り入力
bool GameInput::GetTextScroll() const
{
	if (!m_isActive)
	{
		return false;
	}

	return m_input->GetKeyDown(KeyName::Z) || m_input->GetKeyDown(KeyName::X);
}

// ポーズ入力
bool GameInput::GetPause() const
{
	if (!m_isActive)
	{
		return false;
	}

	return m_input->GetKeyDown(KeyName::Escape);
}

// メニュー上入力
bool GameInput::GetMenuAbove() const
{
	if (!m_isActive)
	{
		return false;
	}

	return m_input->GetKeyDown(KeyName::W) || m_input->GetKeyDown(KeyName::Up);
}

// メニュー下入力
bool GameInput::GetMenuBelow() const
{
	if (!m_isActive)
	{
		return false;
	}

	return m_input->GetKeyDown(KeyName::S) || m_input->GetKeyDown(KeyName::Down);
}

// メニュー決定入力
bool GameInput::GetMenuDecide() const
{
	if (!m_isActive)
	{
		return false;
	}

	return m_input->GetKeyDown(KeyName::Z) || m_input->GetKeyDown(KeyName::Space);
}

// メニュー戻る入力
bool GameInput::GetMenuBack() const
{
	if (!m_isActive)
	{
		return false;
	}

	return m_input->GetKeyDown(KeyName::X) || m_input->GetKeyDown(KeyName::Escape);
}
