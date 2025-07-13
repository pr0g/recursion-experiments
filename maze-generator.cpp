#include <format>
#include <iostream>
#include <optional>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

const std::string g_empty = " ";
const std::string g_mark = "@";
const std::string g_wall = "\u2588";

template<class T>
inline void hash_combine(std::size_t& seed, const T& v) {
  std::hash<T> hasher;
  seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

struct coord_t {
  int row;
  int col;
};

bool operator==(const coord_t& lhs, const coord_t& rhs) {
  return lhs.row == rhs.row && lhs.col == rhs.col;
}

struct coord_hash_t {
  std::size_t operator()(const coord_t& coord) const noexcept {
    std::size_t seed;
    hash_combine(seed, coord.row);
    hash_combine(seed, coord.col);
    return seed;
  }
};

using maze_t = std::vector<std::string>;
using visited_t = std::unordered_set<coord_t, coord_hash_t>;

int rc(int row, int col, int stride) {
  return row * stride + col;
}

void visit_recursive(
  maze_t& maze, coord_t coord, int width, int height, visited_t& visited) {
  const auto [row, col] = coord;
  // where we've just moved to
  maze[rc(row, col, width)] = g_empty;
  while (true) {
    struct move_t {
      coord_t next;
      coord_t connecting;
    };
    std::array<move_t, 4> moves = {
      move_t{
        .next = coord_t{row - 2, col}, .connecting = coord_t{row - 1, col}},
      move_t{
        .next = coord_t{row + 2, col}, .connecting = coord_t{row + 1, col}},
      move_t{
        .next = coord_t{row, col - 2}, .connecting = coord_t{row, col - 1}},
      move_t{
        .next = coord_t{row, col + 2}, .connecting = coord_t{row, col + 1}}};

    const auto in_range = [width, height](coord_t coord) {
      return coord.row <= height - 2 && coord.row >= 1 && coord.col <= width - 2
          && coord.col >= 1;
    };

    std::vector<move_t> unvisited_neighbours;
    for (const auto move : moves) {
      if (in_range(move.next) && visited.find(move.next) == visited.end()) {
        unvisited_neighbours.push_back(move);
      }
    }

    if (unvisited_neighbours.empty()) {
      return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> random_unvisited_node(
      0, unvisited_neighbours.size() - 1);

    int next_index = random_unvisited_node(gen);
    const auto next = unvisited_neighbours[next_index];
    // in between cell (as we move 2 steps at a time, fill in the gap)
    maze[rc(next.connecting.row, next.connecting.col, width)] = g_empty;

    visited.insert(next.next);
    visit_recursive(maze, next.next, width, height, visited);
  }
}

void visit_iterative_recursive(
  maze_t& maze, coord_t coord, int width, int height, visited_t& visited) {
  struct frame_t {
    coord_t coord;
  };
  std::stack<frame_t> call_stack;
  call_stack.push(frame_t{.coord = coord});
  while (!call_stack.empty()) {
    auto& top = call_stack.top();
    const auto [row, col] = top.coord;
    // where we've just moved to
    maze[rc(row, col, width)] = g_empty;
    while (true) {
      struct move_t {
        coord_t next;
        coord_t connecting;
      };
      std::array<move_t, 4> moves = {
        move_t{
          .next = coord_t{row - 2, col}, .connecting = coord_t{row - 1, col}},
        move_t{
          .next = coord_t{row + 2, col}, .connecting = coord_t{row + 1, col}},
        move_t{
          .next = coord_t{row, col - 2}, .connecting = coord_t{row, col - 1}},
        move_t{
          .next = coord_t{row, col + 2}, .connecting = coord_t{row, col + 1}}};

      const auto in_range = [width, height](coord_t coord) {
        return coord.row <= height - 2 && coord.row >= 1
            && coord.col <= width - 2 && coord.col >= 1;
      };

      std::vector<move_t> unvisited_neighbours;
      for (const auto move : moves) {
        if (in_range(move.next) && visited.find(move.next) == visited.end()) {
          unvisited_neighbours.push_back(move);
        }
      }

      if (unvisited_neighbours.empty()) {
        call_stack.pop();
        break;
      }

      std::random_device rd;
      std::mt19937 gen(rd());
      std::uniform_int_distribution<> random_unvisited_node(
        0, unvisited_neighbours.size() - 1);

      int next_index = random_unvisited_node(gen);
      const auto next = unvisited_neighbours[next_index];
      // in between cell (as we move 2 steps at a time, fill in the gap)
      maze[rc(next.connecting.row, next.connecting.col, width)] = g_empty;

      visited.insert(next.next);
      call_stack.push(frame_t{.coord = next.next});
      // break out of while loop
      break;
    }
  }
}

int main(int argc, char** argv) {
  const int width = 49;
  const int height = 21;

  {
    // initialize maze
    maze_t maze;
    for (int row = 0; row < height; row++) {
      for (int col = 0; col < width; col++) {
        maze.push_back(g_wall);
      }
    }

    visited_t visited;
    visited.insert(coord_t{1, 1});
    maze[rc(1, 1, width)] = g_empty;

    visit_recursive(maze, coord_t{1, 1}, width, height, visited);

    std::cout << "generate maze recursive:\n";
    for (int row = 0; row < height; row++) {
      for (int col = 0; col < width; col++) {
        std::cout << std::format("{}", maze[rc(row, col, width)]);
      }
      std::cout << '\n';
    }
  }

  {
    // initialize maze
    maze_t maze;
    for (int row = 0; row < height; row++) {
      for (int col = 0; col < width; col++) {
        maze.push_back(g_wall);
      }
    }

    visited_t visited;
    visited.insert(coord_t{1, 1});
    maze[rc(1, 1, width)] = g_empty;

    visit_iterative_recursive(maze, coord_t{1, 1}, width, height, visited);

    std::cout << "generate maze iterative recursive:\n";
    for (int row = 0; row < height; row++) {
      for (int col = 0; col < width; col++) {
        std::cout << std::format("{}", maze[rc(row, col, width)]);
      }
      std::cout << '\n';
    }
  }

  return 0;
}
