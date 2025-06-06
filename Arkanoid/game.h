#ifndef GAME_H
#define GAME_H

#include "constants.h"
#include "manager.h"

// class that manage the gam itself
// - create the game's window using an sf::RenderWindow
// - keep track of teextures and fonts used in the game
class Game
{
	enum class State { Paused, GameOver, Running, PlayerWins };

	State         _state = State::GameOver;     // state of the game
	int           _lives = Config::Game::lives; // number of remaining lives
	EntityManager _manager;                     // game entity manager

	RenderWindow  _window;	    // SFML game window	
	Texture       _back;		// SFML texture
	Texture       _brick;		// SFML texture
	Texture       _ball;		// SFML texture
	Texture       _paddle;		// SFML texture
	Font             _font;	    // SFML font used for the text
	unique_ptr<Text> _stateTxt; // SFML text to communicate with the player
	unique_ptr<Text> _livesTxt; // SFML text to communicate with the player

	unique_ptr<Text> _dbgTxt;   // SFML debug text

public:
	Game();

	void reset();	// initialize the game
	void run();		// start the game

private:
	unique_ptr<Text> CreateText(V2f offset, uint size);

	void CreateBall();
	void CreateDefaultGrid();
	
	sf::Clock _fpsClock;
    float _fps = 0.0f;
	void DisplayFrameInfos();

	static void SetCenteredText(Text* txt, const string& s);
	static View ComputeLetterboxView(const View& view, float ratio);
};

#endif // GAME_H
