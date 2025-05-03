#include <readfiles.h>

#include <cassert>
#include <string>
#include <fstream>
#include <algorithm>

std::vector<rendering::Point> readLineSegments(std::ifstream& file) {
    std::vector<rendering::Point> lines;

    rendering::Point start(glm::vec2{}, glm::vec4{1.0f, 1.0f, 0.0f, 1.0f}, 0.002f);
    rendering::Point end(glm::vec2{}, glm::vec4{1.0f, 1.0f, 0.0f, 1.0f}, 0.002f);
    while (file >> start.position.x >> start.position.y &&
           file >> end.position.x >> end.position.y) {
        start.position.x /= 32767.0;
        start.position.y /= 32767.0;
        end.position.x /= 32767.0;
        end.position.y /= 32767.0;
        lines.insert(lines.end(), {start, end});
    }
    return lines;
}

std::vector<rendering::Point> readLineSegments(const std::filesystem::path& file) {
    std::ifstream linesFile(file);
    if (!linesFile) {
        std::cout << "Line segments file error!!\n";
        return {};
    }
    return readLineSegments(linesFile);
}

std::vector<rendering::Point> readPoints(std::istream& is) {
    int n_points{0};
    is >> n_points;  // read number of particles

    std::vector<rendering::Point> points;
    points.reserve(n_points);
    for (int i = 0; i < n_points; ++i) {
        auto& p = points.emplace_back(glm::vec2{}, glm::vec4{1.0f, 1.0f, 0.0f, 1.0f}, 0.002f);
        is >> p.position.x >> p.position.y;
        p.position.x /= 32767.0;
        p.position.y /= 32767.0;
    }
    return points;
}

std::vector<rendering::Point> readPoints(const std::filesystem::path& file) {
    std::ifstream pointsFile(file);
    if (!pointsFile) {
        std::cout << "Points file error!!\n";
        return {};
    }
    return readPoints(pointsFile);
}
