/*
 * FileName:     NPCManager.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/10
 *
 * NPC管理
 */

#pragma once

#include "Scripts/Commons/Components/Component.h"

namespace Components
{
	class RectTransform;
}
namespace Renderings
{
	class ICameraScreen;
	class Canvas;
}
class NPC;
class InteractCursor;
class Player;

// NPC管理
class NPCManager : public Component
{

public:


	/* メンバ関数 */

	// コンストラクタ
	NPCManager(const ComponentDesc& desc);

	// 初期値を設定
	void SetSerializeField(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder) override;
	// 初期化処理
	void Initalize(Ref<IGameObjectFinder> iGameObjectFinder) override;

	// 更新処理
	void Update(float elapsedTime);

	// 向きを設定
	void SetRotation(const Math::Vector3& playerPosition);

	// カーソルを設定
	void SetCursor(Ref<const Renderings::ICameraScreen> iCameraScreenRef);

	// カーソルを隠す
	void HideCursor(bool isHideCursor) { m_isHideCursor = isHideCursor; }

	// プレイヤーとの当たり判定
	void PlayerCollide(Ref<Player> playerRef);

	// NPCの数を取得
	size_t GetNPCCount() const { return m_nPCRefs.size(); }

	// 当たり判定を取得
	std::vector<Math::Box> GetBoxes() const;

	// カーソルが合っているNPCを取得
	const NPC* GetPFocusedNPC() const { return m_pFocusedNPC; }

	// NPC を追加
	void AddNPC(Ref<NPC> nPCRef);


private:


	/* メンバ変数 */

	// カーソルを隠しているか
	bool m_isHideCursor;

	// キャンバスの参照
	Ref<const Renderings::Canvas> m_canvasRef;

	// NPCの参照リスト
	std::vector<Ref<NPC>> m_nPCRefs;
	// カーソルの参照リスト
	std::vector<Ref<InteractCursor>> m_interactCursorRefs;

	// カーソルが合っているMPCのポインタ
	NPC* m_pFocusedNPC;

};
