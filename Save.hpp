# pragma once
# include "Common.hpp"
# include "SaveDataBox.hpp"
# include "SasaGUI.hpp"

class Save : public App::Scene
{
public:
	Save(const InitData& init);

	void update() override;

	void draw() const override;
private:
	Array<FilePath> savefiles;
	Array<SaveDataBox> savebox;

	const Font font{ FontMethod::MSDF, 35 };
	const double FontSize = 35;


	const SaveDataBox::palette_ palette_{
		.tileColor1 = ColorF{1.0} ,
		.tileColor2 = ColorF{ 0.0 },
		.borderColor1 = ColorF{ 0.0 }
	};

	const Font Title{200};
	const Font b_font{ 35 };

	int32 page = 0;
	int32 pageMax = 1;
	Camera2D camera{ Scene::CenterF(), 1.0, CameraControl::None_};
};
