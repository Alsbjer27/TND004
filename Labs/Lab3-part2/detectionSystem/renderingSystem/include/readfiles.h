#pragma once

#include <iostream>
#include <vector>
#include <filesystem>

#include <window.h>

#include <fmt/format.h>

const std::filesystem::path data_dir{DATA_DIR};

// using Point = rendering::Point; // I have also a class Point

std::vector<rendering::Point> readLineSegments(const std::filesystem::path& file);

std::vector<rendering::Point> readPoints(const std::filesystem::path& file);
