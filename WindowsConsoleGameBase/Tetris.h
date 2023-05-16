#pragma once
#include "Engine.h"
#include "GameField.h"
#include "Figure.h"
#include "IBlock.h"

#define MAX_SPEED 100

enum GameState
{
	RUNNING,
	GAME_OVER,
	PAUSED
};

class Tetris : public Engine
{
private:
	const size_t m_Width = 14;
	const size_t m_Height = 26;

	GameState m_State = RUNNING;
	unsigned int m_Score = 0;
	int m_FigureFallTime = 500;
	bool m_End = false;

	GameField m_GameField;
	Figure* m_Figure;
	Figure* m_NextFigure;

	/// <returns>Figure with m_FigureFallTime set</returns>
	Figure* get_random_figure(Vector2 position);

	/// <summary>
	/// Draws game over screen
	/// </summary>
	void game_over_screen(PaintDevice& paintDevice) const;
	
	/// <summary>
	/// Draws pause screen
	/// </summary>
	void pause_screen(PaintDevice& paintDevice) const;
	
	/// <summary>
	/// Draws main game screen
	/// </summary>
	void game_screen(PaintDevice& paintDevice) const;

	/// <summary>
	/// Resets score, speed, field and creates new figures
	/// </summary>
	void new_game();

	bool is_paused() const;

	inline bool end() const { return m_End; };
	void on_button_press(const int button);
	void update(const int dt);
	void render(PaintDevice& paintDevice);
	void spawn_figure();
	void add_score(unsigned int lines);
public:
	Tetris();
	~Tetris();
};