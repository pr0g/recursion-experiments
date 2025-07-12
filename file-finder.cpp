#include <filesystem>
#include <format>
#include <iostream>
#include <stack>

namespace fs = std::filesystem;

bool has_even_byte_size(const fs::path& path) {
  return fs::file_size(path) % 2 == 0;
}

bool has_every_vowel(const fs::path& path) {
  const std::string name = [&path] {
    const auto name = path.stem().filename().string();
    std::string out_name;
    out_name.reserve(name.size());
    std::transform(
      name.begin(), name.end(), std::back_inserter(out_name),
      [](char c) { return std::tolower(c); });
    return name;
  }();
  const std::string vowels = "aeiou";
  return std::all_of(vowels.begin(), vowels.end(), [name](const char c) {
    return name.find(c) != std::string::npos;
  });
}

std::vector<std::string> walk(
  const fs::path& folder, std::function<bool(const std::string&)> matcher) {
  std::vector<std::string> matched_files;
  for (const auto& entry : fs::directory_iterator(folder)) {
    if (entry.is_regular_file()) {
      if (const auto path = entry.path(); matcher(path)) {
        matched_files.push_back(path);
      }
    } else if (entry.is_directory()) {
      const auto sub_matched_files = walk(entry.path(), matcher);
      matched_files.insert(
        matched_files.end(), sub_matched_files.begin(),
        sub_matched_files.end());
    }
  }
  return matched_files;
}

int main(int argc, char** argv) {
  if (argc >= 2) {
    std::cout << std::format("path: {}\n", argv[1]);
    const auto even_byte_size_files =
      walk(fs::path(argv[1]), [](const fs::path& path) {
        return has_even_byte_size(path);
      });
    std::cout << "even byte size files:\n";
    for (const auto& filename : even_byte_size_files) {
      std::cout << std::format("{}\n", filename);
    }
    std::cout << '\n';
    const auto every_vowel_filenames =
      walk(fs::path(argv[1]), [](const fs::path& path) {
        return has_every_vowel(path.filename());
      });
    std::cout << "files with every vowel:\n";
    for (const auto& filename : every_vowel_filenames) {
      std::cout << std::format("{}\n", filename);
    }
  }

  return 0;
}
