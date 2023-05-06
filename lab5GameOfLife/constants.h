#ifndef LAB5GAMEOFLIFE_CONSTANTS_H
#define LAB5GAMEOFLIFE_CONSTANTS_H

namespace constants
{
    namespace runtime
    {
        constexpr int LOGIC_ERROR = -1;
    }

    namespace game_logic
    {
        constexpr int LINE_ENVIRONMENT = 2;
        constexpr int ITER_LIMIT = 30;
    }

    namespace game_objects
    {
        constexpr int START_NUM_UNIVERSE = 1;
        constexpr int NEW_VECTOR_CAP = 3;
        enum CellType { ALIVE = true, DEAD = false };
    }
}


#endif
