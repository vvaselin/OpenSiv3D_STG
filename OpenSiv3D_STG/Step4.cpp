# include "Step4.hpp"

void Step4Main()
{
	// 自機の位置
	Vec2 playerPos{ 400, 500 };

	// 自機のテクスチャ
	const Texture playerTexture{ 0xF1383_icon, 70 };

	// 自機のスピード
	constexpr double PlayerSpeed = 550.0;

	// 自機ショット
	Array<Vec2> playerBullets;

	// 自機ショットのスピード
	constexpr double PlayerBulletSpeed = 500.0;

	while (System::Update())
	{
		ClearPrint(); // 文字のクリア
		Print << U"Step4: 自機ショット"; // ステップ4の説明

		// 自機の移動
		Vec2 move = Vec2{ 0,0 };
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

		// 自機ショットの発射
		if (KeySpace.pressed())
		{
			playerBullets << playerPos.movedBy(0, -0); // 自機の位置から上方向にショットを発射
		}

		// 自機ショットを移動させる
		for (auto& playerBullet : playerBullets)
		{
			playerBullet.y += (Scene::DeltaTime() * -PlayerBulletSpeed);
		}
		// 画面外に出た自機ショットを削除する
		playerBullets.remove_if([](const Vec2& b) { return (b.y < -20); });

		// プレイヤーの描画
		playerTexture.drawAt(playerPos);

		// 自機ショットを描画する
		for (const auto& playerBullet : playerBullets)
		{
			Circle{ playerBullet, 8 }.draw(Palette::Orange);
		}
	}
}
