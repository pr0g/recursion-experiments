#include <format>
#include <iostream>
#include <vector>

using image_t = std::vector<std::string>;

void display_image(const image_t& image) {
  for (int row = 0; row < image.size(); row++) {
    for (int col = 0; col < image[0].size(); col++) {
      std::cout << image[row][col];
    }
    std::cout << '\n';
  }
}

void flood_fill_recursive(
  image_t& image, int row, int col, char new_char, char old_char) {
  const int width = image[0].size();
  const int height = image.size();
  if (
    row < 0 || row >= height || col < 0 || col >= width
    || image[row][col] != old_char || image[row][col] == new_char) {
    return;
  } else {
    image[row][col] = new_char;
    // display_image(image); // show step by step
  }
  flood_fill_recursive(image, row + 1, col, new_char, old_char);
  flood_fill_recursive(image, row - 1, col, new_char, old_char);
  flood_fill_recursive(image, row, col + 1, new_char, old_char);
  flood_fill_recursive(image, row, col - 1, new_char, old_char);
}

int main(int argc, char** argv) {

  image_t image = {
    {"..########################..........."},
    {"..#......................#...#####..."},
    {"..#..........########....#####...#..."},
    {"..#..........#......#............#..."},
    {"..#..........########.........####..."},
    {"..######......................#......"},
    {".......#..#####.....###########......"},
    {".......####...#######................"}};

  display_image(image);

  flood_fill_recursive(image, 3, 3, 'o', '.');
  std::cout << '\n';

  display_image(image);

  return 0;
}
