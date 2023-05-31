#pragma once

namespace constants {
    namespace game {
        constexpr int MIN_FIELD_SIZE = 2;
        constexpr int GLIDER_REQ_SIZE = 3;
        constexpr int BASE_HISTORY_CAPACITY = 21;
        constexpr int ITERATION_LIMIT = 4000;
        constexpr int NEW_CAPACITY_ADDITION = 30;
    }

    namespace runtime {
        constexpr int EXCEPTION_THROW = 0x1;
    }
}