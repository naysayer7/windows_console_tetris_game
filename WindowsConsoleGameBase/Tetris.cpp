#include "Tetris.h"
#include "BlockFactory.h"
#include <string>
#include <thread>

Figure* Tetris::get_random_figure(Vector2 position)
{
	Figure* figure = BlockFactory::get_figure(rand() % 7, position);
	figure->set_speed(m_FigureFallTime);
	return figure;
}

void Tetris::game_over_screen(PaintDevice& paintDevice) const
{

	std::wstring label = L"Game over!";
	Vector2 label_pos(m_Width / 2 - label.size() / 2 + 3, 10);

	std::wstring score = std::to_wstring(m_Score);
	Vector2 score_pos(m_Width / 2 - score.size() / 2 + 3, 12);

	// Draw label
	for (size_t i = 0; i < label.size(); i++)
		paintDevice.set_char(label_pos + Vector2(i, 0), label[i]);
	// Draw score
	for (size_t i = 0; i < score.size(); i++)
		paintDevice.set_char(score_pos + Vector2(i, 0), score[i]);
}

void Tetris::pause_screen(PaintDevice& paintDevice) const
{
	std::wstring label = L"Paused";
	Vector2 label_pos(m_Width / 2 - label.size() / 2 + 3, 12);

	for (size_t i = 0; i < label.size(); i++)
		paintDevice.set_char(label_pos + Vector2(i, 0), label[i]);
}

void Tetris::game_screen(PaintDevice& paintDevice) const
{
	m_GameField.render(paintDevice);
	m_Figure->render(paintDevice);

	// Draw score
	std::wstring text = L"Score:";
	Vector2 score_pos(m_Width, 1); // Score text position
	for (size_t i = 0; i < text.size(); i++) // Draw text
		paintDevice.set_char(score_pos + Vector2(i, 0), text[i]);
	std::wstring score = std::to_wstring(m_Score);
	for (size_t i = 0; i < score.size(); i++) // Draw the number
		paintDevice.set_char(score_pos + Vector2(i, 1), score[i]);

	m_NextFigure->render(paintDevice);
}

void Tetris::new_game()
{
	m_GameField.resize(Size(m_Width, m_Height));
	delete m_NextFigure;
	delete m_Figure;
	m_Figure = nullptr;
	m_Score = 0;
	m_State = RUNNING;
	m_FigureFallTime = 500;
	spawn_figure();
}

bool Tetris::is_paused() const
{
	return m_State == PAUSED || m_State == GAME_OVER;
}

void Tetris::on_button_press(const int button)
{
	if (button == VK_ESCAPE) {
		if (m_State == PAUSED)
			m_State = RUNNING;
		else if (m_State == RUNNING)
			m_State = PAUSED;
		else if (m_State == GAME_OVER)
			m_End = true;
	}
	if (m_State == PAUSED)
		return;
	if (m_State == GAME_OVER)
		new_game();

	// Save state of figure
	m_Figure->backup();
	
	switch (button)
	{
	case VK_LEFT:
		m_Figure->move_left();
		break;
	case VK_RIGHT:
		m_Figure->move_right();
		break;
	case VK_DOWN:
		m_Figure->move_down();
		break;
	case VK_UP:
		m_Figure->rotate();
		break;
	default:
		break;
	}

	// Undo the changes if collision
	if (m_GameField.has_collision(*m_Figure))
		m_Figure->restore();
}

void Tetris::update(const int dt)
{
	if (is_paused())
		return;

	m_Figure->backup();
	m_Figure->update(dt);

	// Spawn new figure if figure collides 'dead' blocks or bottom border
	if (m_GameField.has_collision(*m_Figure))
	{
		m_Figure->restore();
		add_score(m_GameField.merge(*m_Figure));

		spawn_figure();
	}
}	

void Tetris::render(PaintDevice& paintDevice)
{
	switch (m_State)
	{
	case GameState::RUNNING:
		game_screen(paintDevice);
		break;
	case GameState::PAUSED:
		pause_screen(paintDevice);
		break;
	case GameState::GAME_OVER:
		game_over_screen(paintDevice);
		break;
	default:
		break;
	}
}

Tetris::Tetris()
{
	// + 6 for score table
	paint_device().resize(Size(m_Width + 6, m_Height));
	m_GameField.resize(Size(m_Width, m_Height));
	
	srand(time(NULL));
	spawn_figure();

	track_key(VK_LEFT);
	track_key(VK_RIGHT);
	track_key(VK_DOWN);
	track_key(VK_UP);
	track_key(VK_ESCAPE);
}

Tetris::~Tetris()
{
	delete m_Figure;
	delete m_NextFigure;
}

void Tetris::spawn_figure()
{
	Vector2 figure_start_pos(5, 1);
	Vector2 next_figure_pos(m_Width + 1, 5);

	if (m_Figure != nullptr)
	{
		delete m_Figure;
		m_Figure = m_NextFigure;
		m_Figure->set_position(figure_start_pos);
		m_NextFigure = get_random_figure(next_figure_pos);
	}
	else
	{
		m_NextFigure = get_random_figure(next_figure_pos);
		m_Figure = get_random_figure(figure_start_pos);
	}

	if(m_FigureFallTime > MAX_SPEED)
		m_FigureFallTime -= 10;

	// If new figure collides 'dead' blocks then the game is over
	if (m_GameField.has_collision(*m_Figure))
		m_State = GAME_OVER;
}

void Tetris::add_score(unsigned int lines)
{
	m_Score += lines * lines;
	if(m_Score >= 5)
		m_FigureFallTime = 250;
}
