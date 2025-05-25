# include "Sample.hpp"

// 敵の位置をランダムに作成する関数
Vec2 GenerateEnemy()
{
	return RandomVec2({ 50, 750 }, -20);
}

void SampleMain()
{
	Scene::SetBackground(ColorF{ 0.1, 0.2, 0.7 });

	const Font font{ FontMethod::MSDF, 48 };

	// 自機テクスチャ
	const Texture playerTexture{ U"🤖"_emoji };
	// 敵テクスチャ
	const Texture enemyTexture{ U"👾"_emoji };

	// 自機
	Vec2 playerPos{ 400, 500 };
	// 敵
	Array<Vec2> enemies = { GenerateEnemy() };

	// 自機ショット
	Array<Vec2> playerBullets;
	// 敵ショット
	Array<Vec2> enemyBullets;

	// 自機のスピード
	constexpr double PlayerSpeed = 550.0;
	// 自機ショットのスピード
	constexpr double PlayerBulletSpeed = 500.0;
	// 敵のスピード
	constexpr double EnemySpeed = 100.0;
	// 敵ショットのスピード
	constexpr double EnemyBulletSpeed = 300.0;

	// 敵の発生間隔の初期値（秒）
	constexpr double InitialEnemySpawnInterval = 2.0;
	// 敵の発生間隔（秒）
	double enemySpawnTime = InitialEnemySpawnInterval;
	// 敵の発生の蓄積時間（秒）
	double enemyAccumulatedTime = 0.0;

	// 自機ショットのクールタイム（秒）
	constexpr double PlayerShotCoolTime = 0.1;
	// 自機ショットのクールタイムタイマー（秒）
	double playerShotTimer = 0.0;

	// 敵ショットのクールタイム（秒）
	constexpr double EnemyShotCoolTime = 0.9;
	// 敵ショットのクールタイムタイマー（秒）
	double enemyShotTimer = 0.0;

	Effect effect;

	// ハイスコア
	int32 highScore = 0;
	// 現在のスコア
	int32 score = 0;

	while (System::Update())
	{
		// ゲームオーバー判定
		bool gameover = false;

		const double deltaTime = Scene::DeltaTime();
		enemyAccumulatedTime += deltaTime;
		playerShotTimer = Min((playerShotTimer + deltaTime), PlayerShotCoolTime);
		enemyShotTimer += deltaTime;

		// 敵を発生させる
		while (enemySpawnTime <= enemyAccumulatedTime)
		{
			enemyAccumulatedTime -= enemySpawnTime;
			enemySpawnTime = Max(enemySpawnTime * 0.95, 0.3);
			enemies << GenerateEnemy();
		}

		// 自機の移動
		const Vec2 move = Vec2{ (KeyRight.pressed() - KeyLeft.pressed()), (KeyDown.pressed() - KeyUp.pressed()) }
		.setLength(deltaTime * PlayerSpeed * (KeyShift.pressed() ? 0.5 : 1.0));
		playerPos.moveBy(move).clamp(Scene::Rect());

		// 自機ショットの発射
		if (PlayerShotCoolTime <= playerShotTimer)
		{
			playerShotTimer -= PlayerShotCoolTime;
			playerBullets << playerPos.movedBy(0, -50);
		}

		// 自機ショットを移動させる
		for (auto& playerBullet : playerBullets)
		{
			playerBullet.y += (deltaTime * -PlayerBulletSpeed);
		}
		// 画面外に出た自機ショットを削除する
		playerBullets.remove_if([](const Vec2& b) { return (b.y < -40); });

		// 敵を移動させる
		for (auto& enemy : enemies)
		{
			enemy.y += (deltaTime * EnemySpeed);
		}
		// 画面外に出た敵を削除する
		enemies.remove_if([&](const Vec2& e)
		{
			if (700 < e.y)
			{
				// 敵が画面外に出たらゲームオーバー
				gameover = true;
				return true;
			}
			else
			{
				return false;
			}
		});

		// 敵ショットの発射
		if (EnemyShotCoolTime <= enemyShotTimer)
		{
			enemyShotTimer -= EnemyShotCoolTime;

			for (const auto& enemy : enemies)
			{
				enemyBullets << enemy;
			}
		}

		// 敵ショットを移動させる
		for (auto& enemyBullet : enemyBullets)
		{
			enemyBullet.y += (deltaTime * EnemyBulletSpeed);
		}
		// 画面外に出た自機ショットを削除する
		enemyBullets.remove_if([](const Vec2& b) {return (700 < b.y); });

		////////////////////////////////
		//
		//	攻撃判定
		//
		////////////////////////////////

		// 敵 vs 自機ショット
		for (auto itEnemy = enemies.begin(); itEnemy != enemies.end();)
		{
			const Circle enemyCircle{ *itEnemy, 40 };
			bool skip = false;

			for (auto itBullet = playerBullets.begin(); itBullet != playerBullets.end();)
			{
				if (enemyCircle.intersects(*itBullet))
				{
					// 爆発エフェクトを追加する
					effect.add([pos = *itEnemy](double t)
					{
						const double t2 = ((0.5 - t) * 2.0);
						Circle{ pos, (10 + t * 280) }.drawFrame((20 * t2), ColorF{ 1.0, (t2 * 0.5) });
						return (t < 0.5);
					});

					itEnemy = enemies.erase(itEnemy);
					playerBullets.erase(itBullet);
					++score;
					skip = true;
					break;
				}

				++itBullet;
			}

			if (skip)
			{
				continue;
			}

			++itEnemy;
		}

		// 敵ショット vs 自機
		for (const auto& enemyBullet : enemyBullets)
		{
			// 敵ショットが playerPos の 20 ピクセル以内に接近したら
			if (enemyBullet.distanceFrom(playerPos) <= 20)
			{
				// ゲームオーバーにする
				gameover = true;
				break;
			}
		}

		// ゲームオーバーならリセットする
		if (gameover)
		{
			playerPos = Vec2{ 400, 500 };
			enemies.clear();
			playerBullets.clear();
			enemyBullets.clear();
			enemySpawnTime = InitialEnemySpawnInterval;
			highScore = Max(highScore, score);
			score = 0;
		}

		////////////////////////////////
		//
		//	描画
		//
		////////////////////////////////

		// 背景のアニメーションを描画する
		for (int32 i = 0; i < 12; ++i)
		{
			const double a = Periodic::Sine0_1(2s, Scene::Time() - (2.0 / 12 * i));
			Rect{ 0, (i * 50), 800, 50 }.draw(ColorF(1.0, a * 0.2));
		}

		// 自機を描画する
		playerTexture.resized(80).flipped().drawAt(playerPos);

		// 自機ショットを描画する
		for (const auto& playerBullet : playerBullets)
		{
			Circle{ playerBullet, 8 }.draw(Palette::Orange);
		}

		// 敵を描画する
		for (const auto& enemy : enemies)
		{
			enemyTexture.resized(60).drawAt(enemy);
		}

		// 敵ショットを描画する
		for (const auto& enemyBullet : enemyBullets)
		{
			Circle{ enemyBullet, 4 }.draw(Palette::White);
		}

		// 爆発エフェクトを描画する
		effect.update();

		// スコアを描画する
		font(U"{} [{}]"_fmt(score, highScore)).draw(30, Arg::bottomRight(780, 580));
	}
}
