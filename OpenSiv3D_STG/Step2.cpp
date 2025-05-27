# include "Step2.hpp"

void Step2Main()
{
	// 自機の位置
	Vec2 playerPos{ 400, 500 };

	// 自機のテクスチャ
	const Texture playerTexture{ 0xF1383_icon, 70 };

	// 自機のスピード
	constexpr double PlayerSpeed = 550.0;

	while (System::Update())
	{
		ClearPrint(); // 文字のクリア
		Print << U"Step2: 自機の移動"; // ステップ2の説明

		// 自機の移動
		if (KeyLeft.pressed())
		{
			playerPos.x -= PlayerSpeed * Scene::DeltaTime();
		}
		if (KeyRight.pressed())
		{
			playerPos.x += PlayerSpeed * Scene::DeltaTime();
		}
		if (KeyUp.pressed())
		{
			playerPos.y -= PlayerSpeed * Scene::DeltaTime();
		}
		if (KeyDown.pressed())
		{
			playerPos.y += PlayerSpeed * Scene::DeltaTime();
		}

		// プレイヤーの描画
		playerTexture.drawAt(playerPos);
	}
}
