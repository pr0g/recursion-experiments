#include <algorithm>
#include <format>
#include <iostream>
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

void printDisk(int disk) {
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

using towers_t = std::unordered_map<std::string, std::vector<int>>;
void printTowers(const towers_t& towers) {
  for (const int level : {6, 5, 4, 3, 2, 1, 0}) {
    for (const auto label : {"A", "B", "C"}) {
      const auto tower = towers.find(label);
      if (level >= tower->second.size()) {
        printDisk(0);
      } else {
        printDisk(tower->second[level]);
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

int main(int argc, char** argv) {
  towers_t towers;

  towers.insert({"A", {6, 5, 4, 3, 2, 1}});
  towers.insert({"B", {}});
  towers.insert({"C", {}});

  printTowers(towers);

  return 0;
}
