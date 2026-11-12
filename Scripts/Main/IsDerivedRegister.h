/*
 * FileName:     IsDerivedRegister.h
 * Author:       Takao Hayata
 * Last Updated: 2026/11/12
 *
 * 継承関係の登録
 */

#pragma once

#include "../Commons/Systems/IsDerivedRegister.h"

namespace Scenes
{
	template<typename TContext>
	class Scene;
}
struct SceneTransitionData;
class GameContext;
typedef Scenes::Scene<GameContext> Scene;

class SampleScene;
class TitleScene;
class GamePlayScene;
class Action;
class ActionOpenMessageWindow;
class ActionCloseMessageWindow;
class ActionSetMessageWindowText;
class ActionBeginEasingCamera;
class ActionEndEasingCamera;
class ActionWaitUntilEasingCamera;
class ActionManager;
class DebugCamera;
class Player;
class PlayerModel;
class PlayerWire;
class PlayerCamera;
class NPC;
class NPCManager;
class Slime;
class SlimeManager;
class CheckPoint;
class CheckPointManager;
class SelectMenu;
class InteractCursor;
class Z2Talk;
class BlackBelt;
class MessageWindow;
class TargetPoint;

class PlayerState;
class PlayerStateOnGround;
class PlayerStateOnAir;
class PlayerStateFalling;
class PlayerStateExpandWire;
class PlayerStateStick;
class PlayerStateFreeze;
class PlayerStateDefeat;

namespace Renderings
{
	class Particle;
}

class JumpingParticle;
class LandingParticle;
class PoppingParticle;
class SparklingParticle;
class WindCutParticle;
class SlimeDefeatParticle;
class PlayerDefeatParticle;

#define IS_DERIVED_REGISTER(T, U) \
template<> \
struct IsDerivedTrait<T, U> : std::true_type {};

IS_DERIVED_REGISTER(SampleScene, Component)
IS_DERIVED_REGISTER(TitleScene, Component)
IS_DERIVED_REGISTER(GamePlayScene, Component)
IS_DERIVED_REGISTER(Action, Component)
IS_DERIVED_REGISTER(ActionOpenMessageWindow, Component)
IS_DERIVED_REGISTER(ActionCloseMessageWindow, Component)
IS_DERIVED_REGISTER(ActionSetMessageWindowText, Component)
IS_DERIVED_REGISTER(ActionBeginEasingCamera, Component)
IS_DERIVED_REGISTER(ActionEndEasingCamera, Component)
IS_DERIVED_REGISTER(ActionWaitUntilEasingCamera, Component)
IS_DERIVED_REGISTER(ActionManager, Component)
IS_DERIVED_REGISTER(DebugCamera, Component)
IS_DERIVED_REGISTER(Player, Component)
IS_DERIVED_REGISTER(PlayerModel, Component)
IS_DERIVED_REGISTER(PlayerWire, Component)
IS_DERIVED_REGISTER(PlayerCamera, Component)
IS_DERIVED_REGISTER(NPC, Component)
IS_DERIVED_REGISTER(NPCManager, Component)
IS_DERIVED_REGISTER(Slime, Component)
IS_DERIVED_REGISTER(SlimeManager, Component)
IS_DERIVED_REGISTER(CheckPoint, Component)
IS_DERIVED_REGISTER(CheckPointManager, Component)
IS_DERIVED_REGISTER(SelectMenu, Component)
IS_DERIVED_REGISTER(InteractCursor, Component)
IS_DERIVED_REGISTER(Z2Talk, Component)
IS_DERIVED_REGISTER(BlackBelt, Component)
IS_DERIVED_REGISTER(MessageWindow, Component)
IS_DERIVED_REGISTER(TargetPoint, Component)

IS_DERIVED_REGISTER(SampleScene, Scene)
IS_DERIVED_REGISTER(TitleScene, Scene)
IS_DERIVED_REGISTER(GamePlayScene, Scene)

IS_DERIVED_REGISTER(PlayerStateOnGround, PlayerState)
IS_DERIVED_REGISTER(PlayerStateOnAir, PlayerState)
IS_DERIVED_REGISTER(PlayerStateFalling, PlayerState)
IS_DERIVED_REGISTER(PlayerStateExpandWire, PlayerState)
IS_DERIVED_REGISTER(PlayerStateStick, PlayerState)
IS_DERIVED_REGISTER(PlayerStateFreeze, PlayerState)
IS_DERIVED_REGISTER(PlayerStateDefeat, PlayerState)

IS_DERIVED_REGISTER(JumpingParticle, Renderings::Particle)
IS_DERIVED_REGISTER(LandingParticle, Renderings::Particle)
IS_DERIVED_REGISTER(PoppingParticle, Renderings::Particle)
IS_DERIVED_REGISTER(SparklingParticle, Renderings::Particle)
IS_DERIVED_REGISTER(WindCutParticle, Renderings::Particle)
IS_DERIVED_REGISTER(SlimeDefeatParticle, Renderings::Particle)
IS_DERIVED_REGISTER(PlayerDefeatParticle, Renderings::Particle)

#undef IS_DERIVED_REGISTER
