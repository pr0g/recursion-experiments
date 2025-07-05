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

  const auto start = find_start(maze);
  std::unordered_set<coord_t, coord_hash_t> visited;
  std::cout << solve_recursive(maze, *start, visited);

  std::cout << '\n';

  for (const auto row : maze) {
    std::cout << std::format("{}\n", row);
  }

  return 0;
}
