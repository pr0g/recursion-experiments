#include <chrono>
#include <format>
#include <iostream>
#include <optional>
#include <random>
#include <ranges>
#include <string>
#include <unordered_set>
#include <vector>

const int g_difficulty = 20;
const int g_board_dim = 4;
const int g_blank = 0;

enum class move_e { up, down, left, right };

std::string move_to_string(const move_e move) {
  switch (move) {
    case move_e::down:
      return "down";
    case move_e::up:
      return "up";
    case move_e::left:
      return "left";
    case move_e::right:
      return "right";
    default:
      return "unknown";
  }
}

struct coord_t {
  int row;
  int col;
};

using board_t = std::vector<int>;

using attempt_move_fn = std::function<bool(
  board_t&, const board_t&, std::vector<move_e>&, int,
  const std::optional<move_e>)>;

int from_rc(int r, int c) {
  return r * g_board_dim + c;
}

coord_t to_rc(int i) {
  return coord_t{.row = i / g_board_dim, .col = i % g_board_dim};
}

void display_board(const board_t& board) {
  for (int row = 0; row < g_board_dim; row++) {
    for (int col = 0; col < g_board_dim; col++) {
      if (board[from_rc(row, col)] == 0) {
        std::cout << std::format("{} ", "__");
      } else {
        std::cout << std::format("{:2} ", board[from_rc(row, col)]);
      }
    }
    std::cout << '\n';
  }
}

int find_blank_space(const board_t& board) {
  // skip checking iterator as we know 0 is there
  return std::distance(board.begin(), std::find(board.begin(), board.end(), 0));
}

int moving_tile(const int blank_tile, const move_e move) {
  const auto [row, col] = to_rc(blank_tile);
  return [row, col, move] {
    switch (move) {
      case move_e::up:
        return from_rc(row + 1, col);
      case move_e::down:
        return from_rc(row - 1, col);
      case move_e::left:
        return from_rc(row, col + 1);
      case move_e::right:
        return from_rc(row, col - 1);
    }
  }();
}

void make_move(
  board_t& board, const int blank_tile, const int tile_to_move,
  const move_e move) {
  std::swap(board[blank_tile], board[tile_to_move]);
}

void make_move(board_t& board, const move_e move) {
  const int blank_tile = find_blank_space(board);
  const int tile_to_move = moving_tile(blank_tile, move);
  return make_move(board, blank_tile, tile_to_move, move);
}

void undo_move(board_t& board, const move_e move) {
  switch (move) {
    case move_e::up:
      make_move(board, move_e::down);
      break;
    case move_e::down:
      make_move(board, move_e::up);
      break;
    case move_e::left:
      make_move(board, move_e::right);
      break;
    case move_e::right:
      make_move(board, move_e::left);
      break;
  }
}

std::vector<move_e> find_valid_moves(
  const board_t& board,
  const std::optional<move_e> previous_move = std::nullopt) {
  const auto [row, col] = to_rc(find_blank_space(board));
  std::vector<move_e> moves;
  if (row != g_board_dim - 1 && previous_move != move_e::down) {
    moves.push_back(move_e::up);
  }
  if (col != g_board_dim - 1 && previous_move != move_e::right) {
    moves.push_back(move_e::left);
  }
  if (row != 0 && previous_move != move_e::up) {
    moves.push_back(move_e::down);
  }
  if (col != 0 && previous_move != move_e::left) {
    moves.push_back(move_e::right);
  }
  return moves;
}

void generate_new_puzzle(board_t& board) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::optional<move_e> previous_move = std::nullopt;
  for (int _ : std::views::iota(0, g_difficulty)) {
    const auto valid_moves = find_valid_moves(board, previous_move);
    std::uniform_int_distribution<> dist(0, valid_moves.size() - 1);
    const auto move = valid_moves[dist(gen)];
    make_move(board, move);
    previous_move = move;
  }
}

bool attempt_move_recursive(
  board_t& board, const board_t& solved_board, std::vector<move_e>& moves_made,
  const int moves_remaining,
  const std::optional<move_e> previous_move = std::nullopt) {
  if (moves_remaining < 0) {
    return false;
  }
  if (board == solved_board) {
    return true;
  }
  for (const auto move : find_valid_moves(board, previous_move)) {
    make_move(board, move);
    moves_made.push_back(move);
    if (attempt_move_recursive(
          board, solved_board, moves_made, moves_remaining - 1, move)) {
      undo_move(board, move);
      return true;
    }
    undo_move(board, move);
    moves_made.pop_back();
  }
  return false;
}

// extract some inline calls to help with porting iterative version
bool attempt_move_recursive_2(
  board_t& board, const board_t& solved_board, std::vector<move_e>& moves_made,
  const int moves_remaining,
  const std::optional<move_e> previous_move = std::nullopt) {
  if (moves_remaining < 0) {
    return false;
  }
  if (board == solved_board) {
    return true;
  }
  const auto valid_moves = find_valid_moves(board, previous_move);
  for (const auto move : valid_moves) {
    make_move(board, move);
    moves_made.push_back(move);
    const auto moved = attempt_move_recursive(
      board, solved_board, moves_made, moves_remaining - 1, move);
    if (moved) {
      undo_move(board, move);
      return true;
    }
    undo_move(board, move);
    moves_made.pop_back();
  }
  return false;
}

bool attempt_move_iterative_recursive(
  board_t& board, const board_t& solved_board, std::vector<move_e>& moves_made,
  const int moves_remaining,
  const std::optional<move_e> previous_move = std::nullopt) {
  enum class return_address_e { before, recursive };
  struct frame_t {
    return_address_e return_address;
    std::vector<move_e> valid_moves;
    std::optional<move_e> previous_move = std::nullopt;
    int moves_remaining;
    bool moved;
    int i;
  };
  std::stack<frame_t> call_stack;
  call_stack.push(
    frame_t{
      .return_address = return_address_e::before,
      .moves_remaining = moves_remaining});
  bool return_value = false;
  while (!call_stack.empty()) {
    auto& top = call_stack.top();
    if (top.return_address == return_address_e::before) {
      if (top.moves_remaining < 0) {
        return_value = false;
        call_stack.pop();
        continue;
      }
      if (board == solved_board) {
        return_value = true;
        call_stack.pop();
        continue;
      }
      const auto valid_moves = find_valid_moves(board, top.previous_move);
      if (!valid_moves.empty()) {
        top.return_address = return_address_e::recursive;
        top.valid_moves = std::move(valid_moves);
        top.i = 0;
        const auto move = top.valid_moves[top.i];
        make_move(board, move);
        moves_made.push_back(move);
        call_stack.push(
          frame_t{
            .return_address = return_address_e::before,
            .moves_remaining = top.moves_remaining - 1,
            .previous_move = move});
        continue;
      }
      call_stack.pop();
    } else if (top.return_address == return_address_e::recursive) {
      top.moved = return_value;
      if (top.moved) {
        undo_move(board, top.valid_moves[top.i]);
        return_value = true;
        call_stack.pop();
        continue;
      }
      undo_move(board, top.valid_moves[top.i]);
      moves_made.pop_back();
      // top of the for loop from recursive version
      top.i++;
      if (top.i < top.valid_moves.size()) {
        const auto move = top.valid_moves[top.i];
        make_move(board, move);
        moves_made.push_back(move);
        // make recursive call again from 'inside' the loop
        call_stack.push(
          frame_t{
            .return_address = return_address_e::before,
            .moves_remaining = top.moves_remaining - 1,
            .previous_move = move});
      } else {
        return_value = false;
        call_stack.pop();
      }
    }
  }
  return return_value;
}

bool solve(
  board_t& board, const board_t& solved_board, int max_moves,
  const attempt_move_fn& attempt_move) {
  std::cout << std::format("attempting to solve in {} moves\n", max_moves);
  std::vector<move_e> solution_moves;
  bool solved = attempt_move_iterative_recursive(
    board, solved_board, solution_moves, max_moves);
  if (solved) {
    std::cout << '\n';
    display_board(board);
    std::cout << '\n';
    for (const auto move : solution_moves) {
      const int blank_tile = find_blank_space(board);
      const int tile_to_move = moving_tile(blank_tile, move);
      std::cout << std::format(
        "move {} {}\n\n", board[tile_to_move], move_to_string(move));
      make_move(board, blank_tile, tile_to_move, move);
      display_board(board);
      std::cout << '\n';
    }
    std::cout << std::format("solved in: {} moves:\n", solution_moves.size());
    for (const auto move :
         solution_moves | std::views::take(solution_moves.size() - 1)) {
      std::cout << std::format("{}, ", move_to_string(move));
    }
    std::cout << std::format("{}\n", move_to_string(solution_moves.back()));
    return true;
  }
  return false;
}

int main(int argc, char** argv) {
  // generate new board
  board_t board(g_board_dim * g_board_dim, 0);
  std::iota(board.begin(), std::prev(board.end()), 1);

  const board_t solved_board = board;

  std::cout << "initial board (solved):\n";
  display_board(board);
  std::cout << '\n';

  using fp_seconds = std::chrono::duration<float, std::chrono::seconds::period>;

  generate_new_puzzle(board);

  {
    auto recursive_board = board;

    std::cout << "shuffled board (unsolved):\n";
    display_board(recursive_board);
    std::cout << '\n';

    std::cout << "tile solver recursive:\n";
    const auto before = std::chrono::steady_clock::now();
    int initial_max_moves = 10;
    while (true) {
      if (solve(
            recursive_board, solved_board, initial_max_moves,
            attempt_move_recursive)) {
        break;
      }
      initial_max_moves++;
    }
    const auto after = std::chrono::steady_clock::now();
    const auto duration = fp_seconds(after - before);
    std::cout << std::format("solved in {}\n", duration);
  }

  std::cout << '\n';

  {
    auto iterative_board = board;

    std::cout << "shuffled board (unsolved):\n";
    display_board(iterative_board);
    std::cout << '\n';

    std::cout << "tile solver iterative:\n";
    const auto before = std::chrono::steady_clock::now();
    int initial_max_moves = 10;
    while (true) {
      if (solve(
            iterative_board, solved_board, initial_max_moves,
            attempt_move_iterative_recursive)) {
        break;
      }
      initial_max_moves++;
    }
    const auto after = std::chrono::steady_clock::now();
    const auto duration = fp_seconds(after - before);
    std::cout << std::format("solved in {}\n", duration);
  }
  return 0;
}
