#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "types.h"

struct Config
{
    // brick size
    //   width  : from 40px to 80px
    //   heigth : from 15px to 30px
    //   usual ratio : w/h in [2:3]
    // ball size
    //   radius : from 4px to 8px
    //   usual ratio: from 1/8 to 1/4 times the height of a brick
    //   (to small: too difficult to be seen. too large: not precise
    // ball Speed
    //   Should feel fast enough to be exciting, but slow enough to react.
    //   Good practice : Start around 1/100 to 1/200 of screen width per frame.
    //   Increase after :
    //      - Time elapsed
    //      - Fewer bricks remaining
    //      - Level - up
    // paddle size
    //   width  : from 80px to 100px
    //   height : from 10px to 20px
    //   usual ratio : 2 to 3 times the width of a brick
    // paddle speed
    //   Should let players reach both sides within ~1 second.
    //   with a 1600u field and 60 fps : 8u/f leads to 3.3s, 20u/f leads to 1.3s
    // speed Ratios
    //   Good balance : ball speed ~ paddle speed x 0.8 to 1.2

    // game field
    struct Game
    {
        static constexpr V2f win{ 1600, 900 };  // width / height (length units : u)
        static constexpr int lives{ 3 };        // How many lives a player has to start with
        static constexpr V2u MaxGrid{ 30, 10 }; // grid of bricks (columns / rows)
        static constexpr int FPS = 60;          // time is controlled through FPS => velocities are displacement per frames !
        static constexpr float Len = Game::win.x / Game::MaxGrid.x; // 53
    };

    struct Brick
    {
        static constexpr V2f Size{ Game::Len, Game::Len / 1.618f }; // size (u) (53,33)
        static constexpr int Strength{ 3 };                         // strength : number of times a brick can be hit
    };

    struct Ball
    {
        static constexpr float Radius{ 7.0f };       // radius (u) 
        static constexpr float Speed{ 8.0f };        // speed  (u/f)
    };

    struct Paddle
    {
        static constexpr V2f   Size{ Game::Len*2, 20.0f }; // size  (u) 60-90-120
        static constexpr float Speed{ 10.0f };             // speed (u/f)
    };

};

#endif // CONSTANTS_H
