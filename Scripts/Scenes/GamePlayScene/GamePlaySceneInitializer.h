/*
 * FileName:     GamePlaySceneInitializer.h
 * Author:       Takao Hayata
 * Last Updated: 2026/09/06
 *
 * ゲームプレイシーンの初期化関数
 */

#pragma once

struct GamePlaySceneInternals;

class GamePlaySceneInitializer
{

public:


	/* 演算子オーバーロード */

	void operator()(Ref<GamePlaySceneInternals> internalsRef) const;


};
