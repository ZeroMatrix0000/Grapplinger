/*
 * FileName:     GamePlaySceneInternals.hpp
 * Author:       Takao Hayata
 * Last Updated: 2026/10/27
 *
 * ゲームプレイシーンの内部データ
 */

#include "Pch.h"
#include "GamePlaySceneInternals.h"

#include "Scripts/Main/IsDerivedRegister.h"
#include "Scripts/Commons/GameObjects/GameObject.h"

GamePlaySceneInternals::GamePlaySceneInternals
(
	Ref<std::vector<std::unique_ptr<GameObject>>> gameObjectsRef,
	Ref<const GameContext> gameContextRef,
	Ref<GameObject> owner
)
	: gameObjectsRef{ gameObjectsRef }
	, gameContextRef{ gameContextRef }
	, playerRef{ owner->GetNullComponent<Player>() }
	, playerCameraRef{ owner->GetNullComponent<PlayerCamera>() }
	, nPCManagerRef{ owner->GetNullComponent<NPCManager>() }
	, groundMeshRef{ owner->GetNullComponent<Colliders::MeshCollider>() }
	, groundModelRef{ owner->GetNullComponent<Renderings::Model3D>() }
	, reticleRef{ owner->GetNullComponent<Renderings::Image>() }
	, z2TalkRef{ owner->GetNullComponent<Z2Talk>() }
	, blackBeltRef{ owner->GetNullComponent<BlackBelt>() }
	, messageWindowRef{ owner->GetNullComponent<MessageWindow>() }
	, targetPointRef{ owner->GetNullComponent<TargetPoint>() }
	, iCameraScreenRef{ owner->GetNullComponent<Renderings::ICameraScreen>() }
	, canvasRef{ owner->GetNullComponent<Renderings::Canvas>() }
	, actionManagerRef{ owner->GetNullComponent<ActionManager>() }
{
}
