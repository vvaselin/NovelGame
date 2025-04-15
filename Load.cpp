# include "Load.hpp"

Load::Load(const InitData& init)
	: IScene{ init }
{
	int32 i = 0;

	for (size_t x = 0; x < 4; x++)
	{
		for (size_t y = 0; y < 3; y++)
		{
			double t;

			if (IsEven(x)) t = x * 960;
			else t = x * 955;

			FilePath f = U"./saveData/save{}.bin"_fmt(i);

			if (not FileSystem::Exists(f)) BinaryWriter writer{ f };

			SaveDataBox box{ f,font,35, RectF{100 + t, 270 + y * 270, 800, 225},palette_ };
			box.setFile();
			savebox << box;
			i++;
		}
	}	
}

void Load::update()
{
	camera.update();
	{
		const auto t = camera.createTransformer();

		for (auto& box : savebox)
		{
			if (box.update(SaveDataBox::state::Load))
			{
				getData().File = box.getFile();
				getData().ScriptLine = box.getLine();
				changeScene(State::Game);
			}
			box.draw();
		}
	}

	camera.setTargetCenter(Scene::CenterF() + Vec2{ Scene::Width() * page, 0 });

	page = Clamp((page += Mouse::Wheel()), 0, pageMax);

	if (Button(Point{ 900,100 }, b_font, U"Title", true))
	{
		getData().File = getData().NewGameFile;
		getData().ScriptLine = 0;
		changeScene(State::Title);
	}

	if (Button(Point{ 1200,100 }, b_font, U"Save", (getData().ScriptLine != 0)))
	{
		changeScene(State::Save,0s);
	}

	if (Button(Point{ 1500,100 }, b_font, U"Story", (getData().ScriptLine != 0)))
	{
		changeScene(State::Game,0.2s);
	}

}

void Load::draw() const
{
	Title(U"Load").drawAt(250, 100, ColorF{ 0.0 } );
	Scene::SetBackground(ColorF{1.0});
}
