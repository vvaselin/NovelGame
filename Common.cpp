# include "Common.hpp"


//関数
//ゲーム初期化関数
void InitGame()
{
	Scene::SetResizeMode(ResizeMode::Keep);
	Window::Resize(1024, 576);
	Scene::Resize(1920, 1080);

	FontAsset::Register(U"TitleFont", 300, Typeface::Heavy);
}
//ルビ振り用関数
void DrawTextWithRuby(const Vec2& basePos, const Font& mainFont, const Font& rubyFont, const String& text, const Array<Ruby>& rubyList, double rubyYOffset)
{
	const Array<double> xAdvances = mainFont(text).getXAdvances();

	Array<Vec2> allPenPos;

	Vec2 penPos{ basePos };

	// 文字単位で描画を制御するためのループ
	for (const auto& glyph : mainFont.getGlyphs(text))
	{
		allPenPos << penPos;

		// 改行文字なら
		if (glyph.codePoint == U'\n')
		{
			// ペンの X 座標をリセット
			penPos.x = basePos.x;

			// ペンの Y 座標をフォントの高さ分進める
			penPos.y += mainFont.height();

			continue;
		}

		// 文字のテクスチャをペンの位置に文字ごとのオフセットを加算して描画
		// FontMethod がビットマップ方式の場合に限り、Math::Round() で整数座標にすると品質が向上
		glyph.texture.draw(Math::Round(penPos + glyph.getOffset()));

		// ペンの X 座標を文字の幅の分進める
		penPos.x += glyph.xAdvance;
	}

	for (const auto& ruby : rubyList)
	{
		if (ruby.indexEnd < text.size())
		{
			const Vec2 beginPenPos = (allPenPos[ruby.indexBegin] + Vec2{ 0, rubyYOffset });

			const Vec2 endPenPos = (allPenPos[ruby.indexEnd] + Vec2{ xAdvances[ruby.indexEnd], rubyYOffset });

			const Vec2 center = ((beginPenPos + endPenPos) / 2);

			rubyFont(ruby.text).draw(Arg::bottomCenter = center);
		}
	}
}

//スクリプト解析用(position)
Vec2 posJudge(const String& pos)
{
	if (pos == U"L")
	{
		return  Scene::Center() - Vec2{ Scene::Center().x / 4, 0} + Vec2{ 0, Scene::Center().y / 5 };
	}
	else if (pos == U"C")
	{
		return Scene::Center() + Vec2{ 0, Scene::Center().y / 5 };
	}
	else if (pos == U"R")
	{
		return Scene::Center() + Vec2{ Scene::Center().x / 4, 0 } + Vec2{ 0, Scene::Center().y / 5 };
	}
	else
	{
		return Vec2{ 0,0 };
	}
}

//スクリプト解析用(scale)
double scaleJudge(const String& scale)
{
	if (scale == U"大")
	{
		return 1.2;
	}
	else if (scale == U"中")
	{
		return 0.6;
	}
	else if (scale == U"小")
	{
		return 0.4;
	}
	else
	{
		return 0.0;
	}
}

//TextureAssetから探す
String SerchAsset(const String& line)
{
	for (auto [name, info] : TextureAsset::Enumerate())
	{
		if (name == line)
		{
			return line;
		}
	}
	return U"NAN";
}

//メッセージボックス端の円
void messageCircle()
{
	const double t = Scene::Time();
	for (auto i : step(12))
	{
		const double theta = (i * 30_deg + t * 120_deg);
		const Vec2 pos = OffsetCircular{ Vec2{1800,1000}, 30, theta };
		Circle{ pos, 5 }.draw(ColorF{ 0.8 });
	}
}

//画面内でクリックされたか(メッセージボックス更新用)
bool winClick()
{
	return ((MouseL.down() || Mouse::Wheel() == 1 || KeyEnter.down() || KeySpace.down()) && (Cursor::OnClientRect()));
}

bool Button(const Point& point, const Font& font, const String& text, bool enabled)
{
	RoundRect rect{point, 300, 100, 10};

	double t = 0.0;
	if (rect.mouseOver())
	{
		t = 0.4;
	}
	else
	{
		0.0;
	}

	if (enabled && rect.mouseOver())
	{
		Cursor::RequestStyle(CursorStyle::Hand);
	}

	if (enabled)
	{
		rect.draw( ColorF{ 1.0 }.lerp(Palette::Slateblue,t));
		rect.drawFrame( 0, 1, ColorF{ 0.0 });
		font(text).drawAt(40, (rect.x + rect.w / 2), (rect.y + rect.h / 2),ColorF{0.0});
	}
	else
	{
		rect.draw(ColorF{ 0.5 });
		font(text).drawAt(40, (rect.x + rect.w / 2), (rect.y + rect.h / 2), ColorF{ 0.7 });
	}

	return (enabled && (rect.leftClicked()||(rect.mouseOver()&&KeyEnter.down())));
}

//バッグログ用の文生成
String GeneLog(const String message)
{
	const String charas = U"[r]";
	const String partition{ U"{|}" }; // ルビ区切る用

	String s = message.replaced(charas, U"");
	String tmp;

	int32 i = 0, j = 0; //i:何番目の文字を処理しているか。 j:改行してから何文字目か

	while (s[i])
	{
		if (s[i] == partition[0]) // "{"だったら
		{
			i++;
			while (s[i] != partition[1]) // "|"になるまで
			{
				tmp << s[i];
				i++; j++;
			}
			i++;
			while (s[i] != partition[2]) // "}"になるまで
			{
				i++;
			}
		}
		else
		{
			if ((j % 30 == 0) && j > 1)
			{
				tmp += U"\n";
				j = 0;
			}

			tmp += s[i];
		}
		i++; j++;
	}

	return tmp;
}

//セーブデータでキャラの情報を加える
JSON MakeChara(const String& name, const String& portrait, const String& pos,
	const String& scale, const bool& visible)
{
	JSON json;
	json[U"Name"] = name;
	json[U"Portrait"] = portrait;
	json[U"Pos"] = pos;
	json[U"Scale"] = scale;
	json[U"Visible"] = visible;
	return json;
}


void AssetInit()
{
	//キャラのアセット
	Array<FilePath> file1 = FileSystem::DirectoryContents(U"./柚宇");
	for (auto&& it = file1.begin(); it != file1.end(); ++it)
	{
		if (FileSystem::Extension(*it) == U"png") {
			TextureAsset::Register(U"YUU" + FileSystem::BaseName(*it), *it, TextureDesc::Mipped);
		}
	}
	Array<FilePath> file2 = FileSystem::DirectoryContents(U"./UNI");
	for (auto&& it = file2.begin(); it != file2.end(); ++it)
	{
		if (FileSystem::Extension(*it) == U"png") {
			TextureAsset::Register(U"UNI" + FileSystem::BaseName(*it), *it, TextureDesc::Mipped);
		}
	}

	//背景のアセット
	Array<FilePath> bg = FileSystem::DirectoryContents(U"./Background");
	for (auto&& it = bg.begin(); it != bg.end(); ++it)
	{
		if ((FileSystem::Extension(*it) == U"jpg") || (FileSystem::Extension(*it) == U"png")) {
			TextureAsset::Register(FileSystem::BaseName(*it), *it);
		}
	}
	//BGMのアセット
	Array<FilePath> bgm = FileSystem::DirectoryContents(U"./BGM");
	for (auto&& it = bgm.begin(); it != bgm.end(); ++it)
	{
		if ((FileSystem::Extension(*it) == U"m4a")|| (FileSystem::Extension(*it) == U"mp3")) {
			AudioAsset::Register(FileSystem::BaseName(*it), Audio::Stream, *it);
		}
	}
	//SEのアセット
	AudioAsset::Register(U"Piano", GMInstrument::Piano1, PianoKey::A4, 0.5s);


	//CGのアセット
	Array<FilePath> cg = FileSystem::DirectoryContents(U"./cg");
	for (auto&& it = cg.begin(); it != cg.end(); ++it)
	{
		if ((FileSystem::Extension(*it) == U"jpg")|| (FileSystem::Extension(*it) == U"png")) {
			TextureAsset::Register(FileSystem::BaseName(*it), *it);
		}
	}

}
