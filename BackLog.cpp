# include "BackLog.hpp"

//コンストラクタ
BackLog::BackLog(const SasaGUI::ScrollBar& vbar)
	:vbar{ vbar }
{

}
//バックログのメッセージを追加
void BackLog::addMessage(const String& message)
{
	String s = GeneLog(message);
	const String::value_type dot{ U'.' }; //追加のセリフ



	if (s[0]==dot)
	{
		s.pop_front();
		messages.back() += s;
	}
	else
	{
		messages << s;
	}
}
Array<String> BackLog::getMessages()
{
	return messages;
}
//バックログの領域を更新する
void BackLog::length_update()
{
	//上の長さ+メッセージの数*メッセージ1つの領域
	length = 100.0 + messages.size() * 150;
}
//バックログのカーソルバーが一番下にあるか
bool BackLog::ReadLog()
{
	return (vbar.value() + Scene::Height() + 5 >= vbar.maximum());
}
//バックログの情報更新
void BackLog::update()
{

	vbar.updateLayout({
			rect.w - SasaGUI::ScrollBar::Thickness, 0,
			SasaGUI::ScrollBar::Thickness,
			rect.h - SasaGUI::ScrollBar::Thickness
		});
	vbar.updateConstraints(0.0, length, rect.h);
	vbar.scroll(Mouse::Wheel() * 400);
	vbar.update();
}
//バックログ一番下に移動
void BackLog::ShowNew()
{
	if (length >= rect.h) { vbar.moveTo(vbar.maximum()); }
}
//メッセージ描画
void BackLog::draw()
{
	Title(U"Back Log").draw(Point{100, 50},ColorF{0.0});

	button.update();
	button.draw();

	const ScopedViewport2D viewport{ rect };

	{
		Transformer2D t(Mat3x2::Translate(0, -vbar.value()));
		//siv3dkun.draw(Vec2{ 0, 0 });
		int32 i = 0;
		for (const auto& s : messages)
		{
			font(s).draw(Vec2{ 100,100 + i }, Palette::Black);
			i += 150;
		}
	}
	if (length >= Scene::Height())
	{
		vbar.draw();
	}
}

void BackLog::popback_Log()
{
	messages.pop_back();
}

bool BackLog::buttonClicked()
{
	return button.update();
}
