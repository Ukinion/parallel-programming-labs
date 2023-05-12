#ifndef LAB5GAMEOFLIFEPARALLEL_CONSTANTS_H
#define LAB5GAMEOFLIFEPARALLEL_CONSTANTS_H



namespace constants
{
    namespace runtime
    {
        constexpr int LOGIC_ERROR = -1;
        constexpr int SAMPLE = 5;
        constexpr double INFINITY = 1e10;
    }

    namespace game_logic
    {
        constexpr int LINE_ENVIRONMENT = 2;
        constexpr int ITER_LIMIT = 1000;
    }

    namespace game_objects
    {
        constexpr int START_NUM_UNIVERSE = 1;
        constexpr int NEW_VECTOR_CAP = 3;
        enum CellType { ALIVE = true, DEAD = false };
    }
}



#endif
