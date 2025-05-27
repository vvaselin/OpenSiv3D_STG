# include "Step3.hpp"

void Step3Main()
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
		Print << U"Step3: 移動制限"; // ステップ3の説明

		// 自機の移動
		Vec2 move = Vec2{0,0};
		if (KeyLeft.pressed())
		{
			move.x -= PlayerSpeed * Scene::DeltaTime();
		}
		if (KeyRight.pressed())
		{
			move.x += PlayerSpeed * Scene::DeltaTime();
		}
		if (KeyUp.pressed())
		{
			move.y -= PlayerSpeed * Scene::DeltaTime();
		}
		if (KeyDown.pressed())
		{
			move.y += PlayerSpeed * Scene::DeltaTime();
		}
		// 画面外に出ないように制限
		playerPos.moveBy(move).clamp(Scene::Rect());

		// プレイヤーの描画
		playerTexture.drawAt(playerPos);
	}
}
