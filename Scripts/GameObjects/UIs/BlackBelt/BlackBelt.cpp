/*
 * FileName:     BlackBelt.cpp
 * Author:       Takao Hayata
 * Last Updated: 2026/10/28
 *
 * 上下の黒帯
 */

#include "Pch.h"
#include "BlackBelt.h"

#include "Scripts/Commons/Components/RectTransform.h"
#include "Scripts/Commons/Renderings/Image/Image.h"
#include "Scripts/Commons/Systems/JsonSerializer.h"
#include "Scripts/Commons/Systems/IsDerivedRegister.h"

// コンストラクタ
BlackBelt::BlackBelt(const ComponentDesc& desc)
	: Component{ desc }
	, m_moveTime{}
	, m_height{}
	, m_moveRate{}
	, m_topImageTransformRef{ GetOwner()->GetNullComponent<RectTransform>() }
	, m_topImageRef{ GetOwner()->GetNullComponent<Renderings::Image>() }
	, m_bottomImageTransformRef{ GetOwner()->GetNullComponent<RectTransform>() }
	, m_bottomImageRef{ GetOwner()->GetNullComponent<Renderings::Image>() }
{
}

// 初期化処理
void BlackBelt::Initalize(const nlohmann::ordered_json& json, Ref<IGameObjectFinder> iGameObjectFinder)
{
	m_moveRate.SetMovement(0.0f, 0.0f);

	Ref canvasRef{ GetOwner()->GetNullComponent<Renderings::Canvas>().Const() };

	Systems::JsonSerializer serializer{ iGameObjectFinder };
	serializer.AddParameter(Ref{ m_moveTime }, "MoveTime");
	serializer.AddParameter(canvasRef.Nest(), "Canvas");
	serializer.Load(json);

	// 画像を生成
	if (m_topImageTransformRef == GetOwner()->GetNullComponent<RectTransform>())
	{
		Ref<GameObject> refObj = Instantiate("Prefab_BlackBeltImage");
		m_topImageTransformRef = refObj->GetComponent<RectTransform>();
		m_topImageRef = refObj->GetComponent<Renderings::Image>();
		m_topImageTransformRef->SetAnchor(Utility::AlignmentPoint::TopCenter);
		refObj = Instantiate("Prefab_BlackBeltImage");
		m_bottomImageTransformRef = refObj->GetComponent<RectTransform>();
		m_bottomImageRef = refObj->GetComponent<Renderings::Image>();
		m_bottomImageTransformRef->SetAnchor(Utility::AlignmentPoint::BottomCenter);

		// 高さを取得
		m_height = m_topImageTransformRef->GetRect().size.y;
	}

	m_topImageRef->SetCanvasRef(canvasRef);
	m_bottomImageRef->SetCanvasRef(canvasRef);
}

// 更新処理
void BlackBelt::Update(float elapsedTime)
{
	// 移動率を更新
	m_moveRate.Tick(elapsedTime);
	// 移動率
	float moveRate = m_moveRate.GetMovement();

	// 透明度を変更
	Math::Color color = m_topImageRef->GetColor();
	color.w = moveRate;
	m_topImageRef->SetColor(color);
	m_bottomImageRef->SetColor(color);

	// 移動率が 0 なら以降何もしない
	if (m_moveRate.GetMovement() == 0.0f)
	{
		return;
	}

	// サイズを変更
	Math::Vector2 size = m_topImageTransformRef->GetRect().size;
	size.y = m_height * moveRate;
	m_topImageTransformRef->SetSize(size);
	m_bottomImageTransformRef->SetSize(size);
}

// 出現
void BlackBelt::Appear(bool isAppear)
{
	m_moveRate.SetMovement(isAppear ? 1.0f : 0.0f, m_moveTime, Easing::Type::Quart, Easing::InOut::Out);
}

// 幅を設定
void BlackBelt::SetWidth(float width)
{
	Math::Vector2 size = m_topImageTransformRef->GetRect().size;
	size.x = width;
	m_topImageTransformRef->SetSize(size);
	m_bottomImageTransformRef->SetSize(size);
}
