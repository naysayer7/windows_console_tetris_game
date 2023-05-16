#pragma once
#include "Vector2.h"
#include "PaintDevice.h"
#include "SymbolSet.h"
#include <vector>

using std::vector;


class Figure
{
private:
	/// <summary>
	/// Figure moves down every m_TimeForUpdate ms
	/// </summary>
	int m_TimeForUpdate = 500;

	/// <summary>
	/// Stores time in ms past last move down
	/// </summary>
	int m_TimeFromLastUpdate = 0;

	/// <summary>
	/// CHAR_INFO Attributes
	/// </summary>
	WORD m_Attributes;

	/// <summary>
	/// Figures position in absolute coordinates
	/// </summary>
	Vector2 m_Position;
	
	/// <summary>
	/// Index in body array
	/// </summary>
	size_t m_State = 0;

	/// <summary>
	/// Position at the time of the last backup() call
	/// </summary>
	Vector2 m_PositionBackup;
	
	/// <summary>
	/// State at the time of the last backup() call
	/// </summary>
	size_t m_StateBackup;

protected:
	/// <summary>
	/// Array of rotated bodies
	/// </summary>
	vector<vector<Vector2>> m_Bodies;

public:
	Figure(Vector2 position);

	void update(const int dt);
	void render(PaintDevice& paintDevice) const;

	inline const vector<Vector2>& get_body() const { return m_Bodies[m_State]; };
	inline Vector2 get_position() const { return m_Position; };
	inline WORD get_attr() const { return m_Attributes; };
	inline void set_speed(const int time) { m_TimeForUpdate = time; };
	inline void set_position(Vector2 position) { m_Position = position; };

	void move_right();
	void move_left();
	void move_down();
	void rotate();

	/// <summary>
	/// Saves current state
	/// </summary>
	void backup();

	/// <summary>
	/// Restores last saved state
	/// </summary>
	void restore();
};

