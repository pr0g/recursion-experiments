#include <algorithm>
#include <chrono>
#include <format>
#include <iostream>
#include <optional>
#include <unordered_map>

// starting pole
// temporary pole (middle)
// end pole

// clang-format off
// 1. solve the n-1 disks puzzle by moving those disks from the start pole to the temporary pole
// 2. move the nth disk from the start pole to the end pole
// 3. solve the n-1 disks puzzle by moving those disks from the temporary pole to the end pole

// what is the base case?
  // solving a tower of one disk
// what argument is passed to the recursive function call?
  // solving a tower of size one less than the current size
// how does the argument become closer to the base case?
  // the size of the tower to solve decreases by one disk for each recursive call, until it is a one disk tower
// clang-format on

const int g_total_disks = 6;

void print_disk(int disk) {
  std::string emptySpace;
  std::generate_n(
    std::back_inserter(emptySpace), g_total_disks - disk, [] { return ' '; });
  if (disk == 0) {
    std::cout << std::format("{}||{}", emptySpace, emptySpace);
  } else {
    std::string diskSpace;
    std::generate_n(std::back_inserter(diskSpace), disk, []() { return '@'; });
    std::string diskNumLabel = std::to_string(disk);
    diskNumLabel.insert(0, "_");
    std::cout << std::format(
      "{}{}{}{}{}", emptySpace, diskSpace, diskNumLabel, diskSpace, emptySpace);
  }
}

using tower_t = std::deque<int>;
using towers_t = std::unordered_map<std::string, tower_t>;
void print_towers(const towers_t& towers) {
  for (const int level : {6, 5, 4, 3, 2, 1, 0}) {
    for (const auto label : {"A", "B", "C"}) {
      const auto tower = towers.find(label);
      if (level >= tower->second.size()) {
        print_disk(0);
      } else {
        print_disk(tower->second[level]);
      }
    }
    std::cout << '\n';
  }
  std::string emptySpace;
  std::generate_n(
    std::back_inserter(emptySpace), g_total_disks, []() { return ' '; });
  std::cout << std::format(
    "{} A{}{} B{}{} C\n", emptySpace, emptySpace, emptySpace, emptySpace,
    emptySpace);
}

void move_disk(
  towers_t& towers, const std::string& start_tower,
  const std::string& end_tower) {
  auto start_tower_it = towers.find(start_tower);
  const auto disk = start_tower_it->second.back();
  start_tower_it->second.pop_back();
  auto end_tower_it = towers.find(end_tower);
  end_tower_it->second.push_back(disk);
}

void solve_towers_of_hanoi_recursive(
  towers_t& towers, int number_of_disks, const std::string& start_tower,
  const std::string& end_tower, const std::string& temp_tower) {
  // move the top number of disks to from start tower to end towers
  if (number_of_disks == 1) {
    move_disk(towers, start_tower, end_tower);
    // print_towers(towers);
  } else {
    solve_towers_of_hanoi_recursive(
      towers, number_of_disks - 1, start_tower, temp_tower, end_tower);
    move_disk(towers, start_tower, end_tower);
    // print_towers(towers);
    solve_towers_of_hanoi_recursive(
      towers, number_of_disks - 1, temp_tower, end_tower, start_tower);
  }
}

void solve_towers_of_hanoi_iterative_recursive(
  towers_t& towers, int number_of_disks, const std::string& start_tower,
  const std::string& end_tower, const std::string& temp_tower) {

  enum class return_address_e { recursive_1, recursive_2 };
  struct frame_t {
    std::optional<return_address_e> return_address;
    int number_of_disks;
    std::string start_tower;
    std::string end_tower;
    std::string temp_tower;
  };

  std::stack<frame_t> call_stack;
  call_stack.push(
    frame_t{

      .number_of_disks = number_of_disks,
      .start_tower = start_tower,
      .end_tower = end_tower,
      .temp_tower = temp_tower});

  while (!call_stack.empty()) {
    auto& top = call_stack.top();
    if (!top.return_address.has_value()) {
      if (top.number_of_disks == 1) {
        move_disk(towers, top.start_tower, top.end_tower);
        call_stack.pop();
        continue;
      }
      top.return_address = return_address_e::recursive_1;
      call_stack.push(
        frame_t{

          .number_of_disks = top.number_of_disks - 1,
          .start_tower = top.start_tower,
          .end_tower = top.temp_tower,
          .temp_tower = top.end_tower});
    } else if (top.return_address == return_address_e::recursive_1) {
      move_disk(towers, top.start_tower, top.end_tower);
      top.return_address = return_address_e::recursive_2;
      call_stack.push(
        frame_t{

          .number_of_disks = top.number_of_disks - 1,
          .start_tower = top.temp_tower,
          .end_tower = top.end_tower,
          .temp_tower = top.start_tower});
    } else if (top.return_address == return_address_e::recursive_2) {
      call_stack.pop();
    }
  }
}

int main(int argc, char** argv) {
  {
    std::cout << "towers of hanoi recursive:\n\n";

    towers_t towers;
    tower_t a_tower;
    for (int i = 6; i > 0; i--) {
      a_tower.push_back(i);
    }

    towers.insert({"A", a_tower});
    towers.insert({"B", {}});
    towers.insert({"C", {}});

    std::cout << "before:\n";
    print_towers(towers);

    const auto before = std::chrono::steady_clock::now();
    solve_towers_of_hanoi_recursive(towers, 6, "A", "C", "B");
    const auto after = std::chrono::steady_clock::now();
    std::cout << "took "
              << std::chrono::duration_cast<std::chrono::nanoseconds>(
                   after - before);

    std::cout << "\nafter:\n";
    print_towers(towers);
  }

  std::cout << "---\n\n";

  {
    std::cout << "towers of hanoi iterative recursive:\n\n";

    towers_t towers;
    tower_t a_tower;
    for (int i = 6; i > 0; i--) {
      a_tower.push_back(i);
    }

    towers.insert({"A", a_tower});
    towers.insert({"B", {}});
    towers.insert({"C", {}});

    std::cout << "before:\n";
    print_towers(towers);

    const auto before = std::chrono::steady_clock::now();
    solve_towers_of_hanoi_iterative_recursive(towers, 6, "A", "C", "B");
    const auto after = std::chrono::steady_clock::now();
    std::cout << "took "
              << std::chrono::duration_cast<std::chrono::nanoseconds>(
                   after - before);

    std::cout << "\nafter:\n";
    print_towers(towers);
  }

  return 0;
}
