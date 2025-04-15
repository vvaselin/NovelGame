# include "Title.hpp"

Title::Title(const InitData& init)
	: IScene{ init }
{
	AssetInit();

	if (FileSystem::Exists(m_path))
	{
		const JSON json = JSON::Load(m_path);
		if (not json) throw Error{ U"Failed to load `config.json`" };

		//コンフィグ読み込み
		GlobalAudio::SetVolume(json[U"Sound"][U"Master"].get<double>());

		GlobalAudio::BusSetVolume(MixBus0, json[U"Sound"][U"BGM"].get<double>());

		GlobalAudio::BusSetVolume(MixBus1, json[U"Sound"][U"SE"].get<double>());

		getData().ms_add = json[U"Text"][U"Message"].get<double>();

		getData().auto_w = json[U"Text"][U"Auto"].get<double>();
	}
	else
	{
		JSON json;

		json[U"Sound"][U"Master"] = 1.0;
		json[U"Sound"][U"BGM"] = 0.5;
		json[U"Sound"][U"SE"] = 0.5;

		json[U"Text"][U"Message"] = 0.5;
		json[U"Text"][U"Auto"] = 0.5;

		json.save(m_path);
	}

}

void Title::update()
{

	if (KeyDown.down()||KeyUp.down())
	{
		++index %= index_max;

		Cursor::SetPos(Point{350, 550}.movedBy(Point{0,100*index}));
	}

	if (Button(Point{ 200, 500 }, b_font, U"NEW GAME", true))
	{
		// ゲームシーンに遷移
		changeScene(State::Game);
	}
	if (Button(Point{ 200, 600 }, b_font, U"LOAD", true))
	{
		// ロード
		changeScene(State::Load,0.2s);
	}
	if (Button(Point{ 200, 700 }, b_font, U"CONFIG", true))
	{
		// コンフィグ
		changeScene(State::Config,0.2s);
	}
	if (Button(Point{ 200, 800 }, b_font, U"LICENSES", true))
	{
		//ライセンス表示
		LicenseManager::ShowInBrowser();
	}
}

void Title::draw() const
{
	Scene::SetBackground(ColorF{1.0});

	FontAsset(U"TitleFont")(U"Title").drawAt(Scene::Center().movedBy(Point{200,0}), ColorF{0.0});
}
