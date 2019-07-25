#include "board.h"

namespace fk {

static Board board;

Board *get_board() {
    return &board;
}

}
