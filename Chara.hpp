# pragma once
# include "Common.hpp"

//キャラの情報
class Chara
{
public:
	//キャラを登録
	Chara(const String& s_name, const String& portrait, const Vec2& pos, const double& scale, const bool& s_visible);
	Chara();

	//立ち絵や位置や大きさの変更
	void change(const String& portrait, const double& s_scale, const Vec2& pos);

	//描画するかどうかを変更
	void setVisible(const bool& s_visible);

	//TextureAssetから描画
	void draw();

	void changeFlag();

	//スクリプトの名前が登録キャラと一致するか
	bool judgeName(const String& s_name);




	//セーブデータ用
	String getName() const;

private:
	String name;//キャラの名前
	double scale = 0.6, scale_before = 0.6;//キャラの大きさ
	String asset, asset_before;//どの立ち絵を使うか
	bool visible, visible_before = false;
	Vec2 position, position_before;
	bool changed = false, fadeIN = true, fadeOUT = false;
	Stopwatch m_stopwatch; //change用

	bool mooving = false;

	Transition transition{ 0.5s, 0.8s };
};
