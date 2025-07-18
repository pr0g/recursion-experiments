#include <format>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_set>
#include <vector>

template<class T>
inline void hash_combine(std::size_t& seed, const T& v) {
  std::hash<T> hasher;
  seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

using maze_t = std::vector<std::string>;

struct coord_t {
  int row;
  int col;
};

bool operator==(const coord_t& lhs, const coord_t& rhs) {
  return lhs.row == rhs.row && lhs.col == rhs.col;
}

std::optional<coord_t> find_start(const maze_t& maze) {
  for (int r = 0; r < maze.size(); r++) {
    for (int c = 0; c < maze[0].size(); c++) {
      if (maze[r][c] == 'S') {
        return coord_t{r, c};
      }
    }
  }
  return std::nullopt;
}

struct coord_hash_t {
  std::size_t operator()(const coord_t& coord) const noexcept {
    std::size_t seed;
    hash_combine(seed, coord.row);
    hash_combine(seed, coord.col);
    return seed;
  }
};

bool solve_recursive(
  maze_t& maze, const coord_t& coord,
  std::unordered_set<coord_t, coord_hash_t>& visited) {
  const int width = maze[0].size();
  const int height = maze.size();
  const auto [row, col] = coord;
  if (maze[row][col] == 'E') {
    return true;
  }

  visited.insert(coord);
  maze[row][col] = '.';

  bool solved = false;
  const auto up = coord_t{.row = row - 1, .col = col};
  if (
    up.row >= 0 && maze[up.row][up.col] == ' '
    || maze[up.row][up.col] == 'E' && !visited.contains(up)) {
    solved = solved || solve_recursive(maze, up, visited);
  }

  const auto down = coord_t{.row = row + 1, .col = col};
  if (
    down.row < height && maze[down.row][down.col] == ' '
    || maze[down.row][down.col] == 'E' && !visited.contains(down)) {
    solved = solved || solve_recursive(maze, down, visited);
  }

  const auto left = coord_t{.row = row, .col = col - 1};
  if (
    left.col >= 0 && maze[left.row][left.col] == ' '
    || maze[left.row][left.col] == 'E' && !visited.contains(left)) {
    solved = solved || solve_recursive(maze, left, visited);
  }

  const auto right = coord_t{.row = row, .col = col + 1};
  if (
    right.col < width && maze[right.row][right.col] == ' '
    || maze[right.row][right.col] == 'E' && !visited.contains(right)) {
    solved = solved || solve_recursive(maze, right, visited);
  }

  if (!solved) {
    // reset space when backtracking and not solved
    maze[row][col] = ' ';
  }

  return solved;
}

bool solve_iterative_recursive(
  maze_t& maze, const coord_t& coord,
  std::unordered_set<coord_t, coord_hash_t>& visited) {
  enum class return_address_e {
    before,
    recursive_1,
    recursive_2,
    recursive_3,
    recursive_4
  };
  struct frame_t {
    std::optional<return_address_e> return_address;
    coord_t coord;
    bool solved = false;
  };

  const int width = maze[0].size();
  const int height = maze.size();
  std::stack<frame_t> call_stack;
  call_stack.push(frame_t{.coord = coord});

  auto check = [width, height, &maze, &visited](coord_t coord, coord_t delta) {
    const auto next =
      coord_t{.row = coord.row + delta.row, .col = coord.col + delta.col};
    if (
      next.row >= 0 && next.row < height && next.col >= 0 && next.col < width
        && maze[next.row][next.col] == ' '
      || maze[next.row][next.col] == 'E' && !visited.contains(next)) {
      return true;
    }
    return false;
  };

  bool return_value = false;
  while (!call_stack.empty()) {
    auto& top = call_stack.top();
    if (!top.return_address.has_value()) {
      if (maze[top.coord.row][top.coord.col] == 'E') {
        return_value = true;
        call_stack.pop();
        continue;
      }

      visited.insert(top.coord);
      maze[top.coord.row][top.coord.col] = '.';

      if (check(top.coord, coord_t{-1, 0})) {
        top.return_address = return_address_e::recursive_1;
        call_stack.push(
          frame_t{.coord = {.row = top.coord.row - 1, .col = top.coord.col}});
        continue;
      }
      if (check(top.coord, coord_t{1, 0})) {
        top.return_address = return_address_e::recursive_2;
        call_stack.push(
          frame_t{.coord = {.row = top.coord.row + 1, .col = top.coord.col}});
        continue;
      }
      if (check(top.coord, coord_t{0, -1})) {
        top.return_address = return_address_e::recursive_3;
        call_stack.push(
          frame_t{.coord = {.row = top.coord.row, .col = top.coord.col - 1}});
        continue;
      }
      if (check(top.coord, coord_t{0, 1})) {
        top.return_address = return_address_e::recursive_4;
        call_stack.push(
          frame_t{.coord = {.row = top.coord.row, .col = top.coord.col + 1}});
        continue;
      }
      if (!return_value) {
        maze[top.coord.row][top.coord.col] = ' ';
      }
      call_stack.pop();
    } else if (top.return_address == return_address_e::recursive_1) {
      return_value = return_value || top.solved;
      if (check(top.coord, coord_t{1, 0})) {
        top.return_address = return_address_e::recursive_2;
        call_stack.push(
          frame_t{.coord = {.row = top.coord.row + 1, .col = top.coord.col}});
        continue;
      }
      if (check(top.coord, coord_t{0, -1})) {
        top.return_address = return_address_e::recursive_3;
        call_stack.push(
          frame_t{.coord = {.row = top.coord.row, .col = top.coord.col - 1}});
        continue;
      }
      if (check(top.coord, coord_t{0, 1})) {
        top.return_address = return_address_e::recursive_4;
        call_stack.push(
          frame_t{.coord = {.row = top.coord.row, .col = top.coord.col + 1}});
        continue;
      }
      if (!return_value) {
        maze[top.coord.row][top.coord.col] = ' ';
      }
      call_stack.pop();
    } else if (top.return_address == return_address_e::recursive_2) {
      return_value = return_value || top.solved;
      if (check(top.coord, coord_t{0, -1})) {
        top.return_address = return_address_e::recursive_3;
        call_stack.push(
          frame_t{.coord = {.row = top.coord.row, .col = top.coord.col - 1}});
        continue;
      }
      if (check(top.coord, coord_t{0, 1})) {
        top.return_address = return_address_e::recursive_4;
        call_stack.push(
          frame_t{.coord = {.row = top.coord.row, .col = top.coord.col + 1}});
        continue;
      }
      if (!return_value) {
        maze[top.coord.row][top.coord.col] = ' ';
      }
      call_stack.pop();
    } else if (top.return_address == return_address_e::recursive_3) {
      if (check(top.coord, coord_t{0, 1})) {
        return_value = return_value || top.solved;
        top.return_address = return_address_e::recursive_4;
        call_stack.push(
          frame_t{.coord = {.row = top.coord.row, .col = top.coord.col + 1}});
        continue;
      }
      if (!return_value) {
        maze[top.coord.row][top.coord.col] = ' ';
      }
      call_stack.pop();
    } else if (top.return_address == return_address_e::recursive_4) {
      if (!return_value) {
        maze[top.coord.row][top.coord.col] = ' ';
      }
      call_stack.pop();
    }
  }
  return return_value;
}

int main(int argc, char** argv) {
  maze_t maze = {
    "#######################################################################",
    "#S#                 #       # #   #     #         #     #   #         #",
    "# ##### ######### # ### ### # # # # ### # # ##### # ### # # ##### # ###",
    "# #   #     #     #     #   # # #   # #   # #       # # # #     # #   #",
    "# # # ##### # ########### ### # ##### ##### ######### # # ##### ### # #",
    "#   #     # # #     #   #   #   #         #       #   #   #   #   # # #",
    "######### # # # ##### # ### # ########### ####### # # ##### ##### ### #",
    "#       # # # #     # #     # #   #   #   #     # # #   #         #   #",
    "# # ##### # # ### # # ####### # # # # # # # ##### ### ### ######### # #",
    "# # #   # # #   # # #     #     #   #   #   #   #   #     #         # #",
    "### # # # # ### # # ##### ####### ########### # ### # ##### ##### ### #",
    "#   # #   # #   # #     #   #     #       #   #     # #     #     #   #",
    "# ### ####### ##### ### ### ####### ##### # ######### ### ### ##### ###",
    "#   #         #     #     #       #   # #   # #     #   # #   # #   # #",
    "### ########### # ####### ####### ### # ##### # # ##### # # ### # ### #",
    "#   #   #       # #     #   #   #     #       # # #     # # #   # #   #",
    "# ### # # ####### # ### ##### # ####### ### ### # # ####### # # # ### #",
    "#     #         #     #       #           #     #           # #      E#",
    "#######################################################################"};

  {
    auto maze_recursive = maze;
    const auto start = find_start(maze_recursive);
    std::unordered_set<coord_t, coord_hash_t> visited;
    std::cout << "recursive:\n";
    std::cout << solve_recursive(maze_recursive, *start, visited) << '\n';
    for (const auto row : maze_recursive) {
      std::cout << std::format("{}\n", row);
    }
  }

  {
    auto maze_iterative = maze;
    const auto start = find_start(maze_iterative);
    std::unordered_set<coord_t, coord_hash_t> visited;
    std::cout << "iterative:\n";
    std::cout << solve_iterative_recursive(maze_iterative, *start, visited)
              << '\n';
    for (const auto row : maze_iterative) {
      std::cout << std::format("{}\n", row);
    }
  }

  return 0;
}
