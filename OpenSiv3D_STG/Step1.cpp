# include "Step1.hpp"

void Step1Main()
{
	// 自機の位置
	Vec2 playerPos{ 400, 500 };

	// 自機のテクスチャ
	const Texture playerTexture{ U"🛸"_emoji };

	while (System::Update())
	{
		ClearPrint(); // 文字のクリア
		Print << U"Step1: 自機の描画"; // ステップ1の説明

		// 自機の移動(後で)

		// プレイヤーの描画
		playerTexture.resized(80).drawAt(playerPos);
	}
}
