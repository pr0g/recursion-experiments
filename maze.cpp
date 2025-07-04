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

bool solve(
  maze_t& maze, coord_t coord,
  std::unordered_set<coord_t, coord_hash_t>& visited) {
  const int width = maze[0].size();
  const int height = maze.size();
  const auto [row, col] = coord;
  if (maze[row][col] == 'E') {
    return true;
  }

  visited.insert(coord);
  maze[row][col] = '.';

  const auto up = coord_t{.row = row - 1, .col = col};
  if (
    up.row >= 0 && maze[up.row][up.col] == ' '
    || maze[up.row][up.col] == 'E' && !visited.contains(up)) {
    if (solve(maze, up, visited)) {
      return true;
    }
  }

  const auto down = coord_t{.row = row + 1, .col = col};
  if (
    down.row < height && maze[down.row][down.col] == ' '
    || maze[down.row][down.col] == 'E' && !visited.contains(down)) {
    if (solve(maze, down, visited)) {
      return true;
    }
  }

  const auto right = coord_t{.row = row, .col = col + 1};
  if (
    right.col < width && maze[right.row][right.col] == ' '
    || maze[right.row][right.col] == 'E' && !visited.contains(right)) {
    if (solve(maze, right, visited)) {
      return true;
    }
  }

  const auto left = coord_t{.row = row, .col = col - 1};
  if (
    left.col >= 0 && maze[left.row][left.col] == ' '
    || maze[left.row][left.col] == 'E' && !visited.contains(left)) {
    if (solve(maze, left, visited)) {
      return true;
    }
  }

  return false;
}

// bool solve(
//   maze_t& maze, int row, int col,
//   std::unordered_set<std::pair<int, int>, coord_hash_t>& visited) {
//   if (
//     row < 0 || row >= maze.size() || col < 0 || col >= maze[0].size()
//     || visited.find(std::pair{row, col}) != visited.end()) {
//     return false;
//   }
//   visited.insert(std::pair{row, col});
//   maze[row][col] = '.';
//   if (maze[row][col] == 'E') {
//     return true;
//   }
//   std::pair<int, int> left = {row, col - 1};
//   if (left.second >= 0) {
//     if (maze[left.first][left.second] == ' ') {
//       if (solve(maze, left.first, left.second, visited)) {
//         return true;
//       }
//     }
//   }
//   std::pair<int, int> right = {row, col + 1};
//   if (right.second < maze[0].size()) {
//     if (maze[right.first][right.second] == ' ') {
//       if (solve(maze, right.first, right.second, visited)) {
//         return true;
//       }
//     }
//   }
//   std::pair<int, int> up = {row - 1, col};
//   if (up.first >= 0) {
//     if (maze[up.first][up.second] == ' ') {
//       if (solve(maze, up.first, up.second, visited)) {
//         return true;
//       }
//     }
//   }
//   std::pair<int, int> down = {row + 1, col};
//   if (down.first < maze.size()) {
//     if (maze[down.first][down.second] == ' ') {
//       if (solve(maze, down.first, down.second, visited)) {
//         return true;
//       }
//     }
//   }
//   maze[row][col] = ' ';
//   return false;
// }

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
  std::cout << solve(maze, *start, visited);

  std::cout << '\n';

  for (const auto row : maze) {
    std::cout << std::format("{}\n", row);
  }

  return 0;
}
