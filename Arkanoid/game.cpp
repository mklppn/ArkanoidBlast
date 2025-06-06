#include <algorithm>
#include <variant>
#include "game.h"
#include "interactions.h"
#include <iostream>

using namespace sf::Keyboard;

Game::Game()
	: _window(VideoMode(V2u((uint)Config::Game::win.x, (uint)Config::Game::win.y)), "Arkanoid")
	, _lives(Config::Game::lives)
	, _state(State::Paused)
{
	// configure text objects
	_font.openFromFile("verdana.ttf");	
	_stateTxt = CreateText(Config::Game::win / 2.0f - V2f{125, 100}, 35);
	_livesTxt = CreateText({ 10,20 }, 10);
	_dbgTxt   = CreateText({ 10, Config::Game::win.y - 50 }, 10);

	_window.setFramerateLimit(Config::Game::FPS); // max rate is 60 frames per second
}

// initialize the game
void Game::reset()
{	
	_lives = Config::Game::lives;    // reset the number of lives
    _state = State::Running;         // reset the game state
	_manager.Clear();			     // destroy all the entities and re-create them

	_back  .loadFromFile("background.jpg");
	_brick .loadFromFile("brick01.png");
	_ball  .loadFromFile("ball.png");
	_paddle.loadFromFile("paddle.png");

	_manager.create<Background>(_back  , V2f{ 0.0f, 0.0f });
	_manager.create<Paddle>    (_paddle, V2f{ Config::Game::win.x / 2.0f, Config::Game::win.y - Config::Paddle::Size.y });
	CreateBall();
	CreateDefaultGrid();
}

// start the game
void Game::run()
{
	// Was the pause key pressed in the last frame?
	bool pauseKeyActive{false};

	while (_window.isOpen())
	{
		_window.clear(Color::Black); // clear the screen

        // check events since the last loop iteration
		// if "ESC", or window closed : close the window and terminate the program
		for (optional<Event> e = _window.pollEvent(); e != nullopt; e = _window.pollEvent())
		{
			if (e->is<Event::Closed>()) _window.close();
			if (e->is<Event::Resized>())
			{
				const auto* r = e->getIf<Event::Resized>();

				View baseView(FloatRect({ 0.f, 0.f }, Config::Game::win));
				_window.setView(ComputeLetterboxView(baseView, (float)r->size.x/(float)r->size.y));
			}
		}

        // check if the user pressed any key
		bool pauseKey = false;
        if      (isKeyPressed(Key::Escape)) break;   // if "ESC" is pressed : terminate the program
		else if (isKeyPressed(Key::R     )) reset(); // reset the game
		else if (isKeyPressed(Key::Space ))
		{
			if (_state == State::Paused)
                _state = State::Running; // if the game is paused, start it
		}
		else if (isKeyPressed(Key::P     ))
		{
			// if not pressed on the last iteration, toggle the status (prevent repeate use)
			pauseKey = true;
			if (!pauseKeyActive)
				_state = _state == State::Paused ? State::Running : State::Paused;
		}	

		pauseKeyActive = pauseKey;

		if (_state == State::Running) // check game evolution and update the entities
		{
			if (_manager.GetAll<Brick>().empty())
			{
				_state = State::PlayerWins;					// no remaining bricks, the player has won!
			}
			if (_manager.GetAll<Ball >().empty())		    // remaining balls on the screen
			{
				--_lives;
				if  (_lives <= 0) _state = State::GameOver; // the player has used up all their lives, the game is over!
				else              CreateBall();             // spawn a new one and reduce the p layer's remaining lives
			}

			_manager.Update();		 // move, weaken, detect collisions  and so on
			_manager.Prune();		 // remove destroyed elements
			_manager.Draw(_window);  // Display the updated graphics
		}
		else
		{
			if (_state == State::Paused) _manager.Draw(_window); // display entities

			switch (_state) {
			case State::Paused    : SetCenteredText(_stateTxt.get(), "Paused"      ); break;
			case State::GameOver  : SetCenteredText(_stateTxt.get(), "Game Over!"  ); break;
			case State::PlayerWins: SetCenteredText(_stateTxt.get(), "Player Wins!"); break;
			default: break;
			}

			_window.draw(*_stateTxt);
		}

		DisplayFrameInfos();

		_livesTxt->setString("Lives: " + to_string(_lives)); // update the text for the number of remaining lives
		_window.draw(*_livesTxt);
		_window.display();
	}
}

void Game::DisplayFrameInfos()
{
	float dt = _fpsClock.restart().asSeconds();
	if (dt == 0) return;               // should never happen since RenderWindow.display() controls the fps
	
    constexpr float exp = 0.1f;        // exponential smoothing factor in[0:1]. 0.1 smooth but slow reaction, [0.3:0.5] decent balance.
    _fps = _fps * (1- exp) + exp / dt; // exponential smoothing for FPS

	string dbg = "";
	dbg += "FPS  : " + to_string(_fps) + "\n";
	dbg += "State: " + to_string((int)_state) + "\n";

	Entity* e;
	e = _manager.GetFirst<Paddle>(); if (e != nullptr) dbg += "Paddle: " + e->ToString() + "\n";
	e = _manager.GetFirst<Ball  >(); if (e != nullptr) dbg += "Ball  : " + e->ToString() + "\n";
	_dbgTxt->setString(dbg);
	_window.draw(*_dbgTxt);
}

void Game::CreateDefaultGrid()
{
	float x = Config::Brick::Size.x / 2;
	float y = Config::Brick::Size.y;

	for (uint i = 0; i < Config::Game::MaxGrid.x; ++i, x += Config::Brick::Size.x)
	{ 
		y = Config::Brick::Size.y*1.5;
		for (uint j = 0; j < Config::Game::MaxGrid.y; ++j, y += Config::Brick::Size.y)
		{
			_manager.create<Brick>(_brick, V2f{ x, y });
		}
	}
}

void Game::CreateBall()
{
	const Entities& paddles = _manager.GetAll<Paddle>();
	if (paddles.empty())
	{
		cerr << "No paddle found to create a ball!" << endl;
		return;
    }

    V2f paddleCenter = paddles[0]->GetCenter();
	V2f ballCenter   = paddleCenter - V2f{ 0, Config::Paddle::Size.y * 0.5f + Config::Ball::Radius };
	_manager.create<Ball>(_ball, ballCenter, V2f{ 0.0f, 0.0f });
}

unique_ptr<Text> Game::CreateText(V2f pos, uint size)
{
	unique_ptr<Text> txt = std::make_unique<Text>(_font);
	txt->setFont(_font);
	txt->setCharacterSize(size);
	txt->setFillColor(Color::White);
	txt->setPosition(pos);
	return txt;
}

void Game::SetCenteredText(Text* txt, const string& s)
{
	txt->setString(s);

	FloatRect b = txt->getLocalBounds();
	txt->setOrigin(b.position + b.size / 2.0f); // center the origin
	txt->setPosition(Config::Game::win / 2.0f); // origin position
}

View Game::ComputeLetterboxView(const View& view, float ratio)
{
	float viewRatio = view.getSize().x / view.getSize().y;
	float scaleX = 1.f, scaleY = 1.f;

	if (ratio > viewRatio)  scaleX = viewRatio / ratio;
	else                    scaleY = ratio / viewRatio;

	View newView = view;
	newView.setViewport(FloatRect(
		{ (1.f - scaleX) / 2.f, (1.f - scaleY) / 2.f },  // left, top
		{ scaleX, scaleY }
	));

	return newView;
}