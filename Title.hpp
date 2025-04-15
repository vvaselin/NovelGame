# pragma once
# include "Common.hpp"


class Title : public App::Scene
{
public:
	Title(const InitData& init);

	void update() override;

	void draw() const override;


private:
	FilePath m_path = U"./Config/config.json";

	const size_t index_max = 4;

	size_t index = index_max -1;

	Font b_font{ 35 };

	
};
