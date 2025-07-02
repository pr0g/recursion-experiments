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

void flood_fill_iterative_recursive(
  image_t& image, int row, int col, char new_char, char old_char) {
  enum class return_address_e {
    before,
    recursive_1,
    recursive_2,
    recursive_3,
    recursive_4
  };
  struct frame_t {
    return_address_e return_address;
    int row;
    int col;
    char new_char;
    char old_char;
  };
  std::stack<frame_t> call_stack;
  call_stack.push(
    frame_t{
      .return_address = return_address_e::before,
      .row = row,
      .col = col,
      .new_char = new_char,
      .old_char = old_char});
  while (!call_stack.empty()) {
    auto& top = call_stack.top();
    if (top.return_address == return_address_e::before) {
      if (
        top.row < 0 || top.row >= image.size() || top.col < 0
        || top.col >= image[0].size() || image[top.row][top.col] != top.old_char
        || image[top.row][top.col] == top.new_char) {
        call_stack.pop();
        continue;
      } else {
        image[top.row][top.col] = top.new_char;
        top.return_address = return_address_e::recursive_1;
        call_stack.push(
          {.return_address = return_address_e::before,
           .row = top.row + 1,
           .col = top.col,
           .new_char = top.new_char,
           .old_char = top.old_char});
      }
    } else if (top.return_address == return_address_e::recursive_1) {
      top.return_address = return_address_e::recursive_2;
      call_stack.push(
        {.return_address = return_address_e::before,
         .row = top.row - 1,
         .col = top.col,
         .new_char = top.new_char,
         .old_char = top.old_char});
    } else if (top.return_address == return_address_e::recursive_2) {
      top.return_address = return_address_e::recursive_3;
      call_stack.push(
        {.return_address = return_address_e::before,
         .row = top.row,
         .col = top.col + 1,
         .new_char = top.new_char,
         .old_char = top.old_char});
    } else if (top.return_address == return_address_e::recursive_3) {
      top.return_address = return_address_e::recursive_4;
      call_stack.push(
        {.return_address = return_address_e::before,
         .row = top.row,
         .col = top.col - 1,
         .new_char = top.new_char,
         .old_char = top.old_char});
    } else if (top.return_address == return_address_e::recursive_4) {
      call_stack.pop();
    }
  }
}

int main(int argc, char** argv) {
  {
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
  }

  std::cout << '\n';

  {
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

    flood_fill_iterative_recursive(image, 3, 3, 'o', '.');
    std::cout << '\n';

    display_image(image);
  }

  return 0;
}
