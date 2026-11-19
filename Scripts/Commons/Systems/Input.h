/*
 * FileName:     Input.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/19
 *
 * 入力管理
 */

#pragma once

#include "IInput.h"

namespace Systems
{
	// 入力管理
	class Input : public IInput
	{

	public:


		/* メンバ関数 */

		// コンストラクタ
		Input();

		// 初期化処理
		void Initialize(const HWND& hWindow);
		// 更新処理
		void Update();

		// キーの押状態を取得
		bool GetKeyDown(KeyName keyName) const override { return m_keyboardState.IsKeyDown(keyName) && m_keyboardStateOld.IsKeyUp(keyName); }
		// キーの状態を取得
		bool GetKey(KeyName keyName)     const override { return m_keyboardState.IsKeyDown(keyName); }
		// キーの離状態を取得
		bool GetKeyUp(KeyName keyName)   const override { return m_keyboardState.IsKeyUp(keyName) && m_keyboardStateOld.IsKeyDown(keyName); }

		// Shiftキーの状態を取得
		bool GetShiftKey()   const override { return GetKey(KeyName::LeftShift) || GetKey(KeyName::RightShift); }
		// Ctrlキーの状態を取得
		bool GetCtrlKey()    const override { return GetKey(KeyName::LeftControl) || GetKey(KeyName::RightControl); }
		// Altキーの状態を取得
		bool GetAltKey()     const override { return GetKey(KeyName::LeftAlt) || GetKey(KeyName::RightAlt); }
		
		// マウスのボタンの押状態を取得
		bool GetMouseButtonDown(MouseButtonName mouseButtonName) const override;
		// マウスのボタンの状態を取得
		bool GetMouseButton(MouseButtonName mouseButtonName)     const override;
		// マウスのボタンの離状態を取得
		bool GetMouseButtonUp(MouseButtonName mouseButtonName)   const override;

		// マウスの位置を取得
		Math::Vector2Int GetMousePosition() const override { return Math::Vector2Int{ m_mouseState.x, m_mouseState.y }; }
		// マウスの移動量を取得
		Math::Vector2Int GetMouseMovement() const override
		{
			return Math::Vector2Int{ m_mouseState.x - m_mouseStateOld.x, m_mouseState.y - m_mouseStateOld.y };
		}
		// マウスホイールの移動量を取得
		int GetMouseWheelDelta() const override { return m_mouseState.scrollWheelValue - m_mouseStateOld.scrollWheelValue; }

		// マウス座標をループさせるかどうかを設定
		void SetMousePositionLoop(bool mousePositionLoop) override { m_mousePositionLoop = mousePositionLoop; }
		// マウスカーソルを表示
		void DisplayCursor(bool isShow) override;


	private:


		/* メンバ関数 */

		// マウスの座標の計算
		void MousePositionCorrect();


		/* メンバ変数 */

		// ウィンドウへのハンドル
		HWND m_hWindow;

		// キーボード
		DirectX::Keyboard        m_keyboard;
		// 現在のフレームのキーボード状態
		DirectX::Keyboard::State m_keyboardState;
		// 1F前のキーボード状態
		DirectX::Keyboard::State m_keyboardStateOld;

		// マウス
		DirectX::Mouse        m_mouse;
		// 現在のフレームのマウス状態
		DirectX::Mouse::State m_mouseState;
		// 1F前のマウス状態
		DirectX::Mouse::State m_mouseStateOld;

		// マウスの座標をループさせるかどうか
		bool m_mousePositionLoop;
		// マウスが表示されているかどうか
		bool m_isShowMouse;

	};
}
