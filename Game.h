#pragma once
class Game
{
public:
	Game();
	~Game();
	// Draws a single game frame
	void draw();
	// Checks for all possible collisions
	void checkCollisions();
	// Updates game state
	void update();
};

