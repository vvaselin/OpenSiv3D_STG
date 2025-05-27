# include "Step1.hpp"

void Step1Main()
{
	// 自機の位置
	Vec2 playerPos{ 400, 500 };

	// 自機のテクスチャ
	const Texture playerTexture{ 0xF1383_icon, 70 };

	while (System::Update())
	{
		ClearPrint(); // 文字のクリア
		Print << U"Step1: 自機の描画"; // ステップ1の説明

		// 自機の移動(後で)

		// プレイヤーの描画
		playerTexture.drawAt(playerPos);
	}
}
