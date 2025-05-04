#include <iostream>
#include <format>
#include <compare>
#include <fstream>
#include <filesystem>
#include <format>

#include <find-patterns.hpp>

const std::filesystem::path data_dir{DATA_DIR};

constexpr int minPoints = 4;

struct Point { // feel free to modify
public:
    Point(int x = 0, int y = 0) : x_{x}, y_{y} {}

    bool operator==(const Point& p) const = default;

    // Compare by y-coordinates and break ties by x-coordinates
    std::strong_ordering operator<=>(Point p) const {
        if (y_ < p.y_) {
            return std::strong_ordering::less;
        }
        if (y_ > p.y_) {
            return std::strong_ordering::greater;
        }
        if (x_ < p.x_) {
            return std::strong_ordering::less;
        }
        if (x_ > p.x_) {
            return std::strong_ordering::greater;
        }
        return std::strong_ordering::equivalent;
    }

    std::string toString() const { return std::format("({},{})", x_, y_); }

    long long x_;
    long long y_;
};

/* ***************************************************** */

int main() {
    std::cout << "Enter the name of input points file: ";
    std::string points_file;
    std::cin >> points_file;

    analyseData(points_file);
}

/* ***************************************************** */

double calculateSlope(const Point& p, const Point& q) {
    long long dx = q.x_ - p.x_;
    long long dy = q.y_ - p.y_;

    if (dx == 0 && dy == 0) {
        return -std::numeric_limits<double>::infinity();
    }

    if (dx == 0) {
        return std::numeric_limits<double>::infinity();
    }

    if (dy == 0) {
        return 0.0;
    }
                                                             
    return static_cast<double>(dy) / static_cast<double>(dx);
}

void analyseData(const std::filesystem::path& pointsFile, const std::filesystem::path& segmentsFile) {
    /*
     * Add code here
     * Feel free to modify the function signature
     * Break your code into small functions
     */
    std::ifstream pointsInStream(pointsFile);
    if (!pointsInStream) {
        std::cerr << "Cannot open points file: " << pointsFile << "\n";
        return;
    }

    int n;
    pointsInStream >> n;
    std::vector<Point> points;
    points.reserve(n);                  // O(n) -> worst case
                                            
    for (int i = 0; i < n; i++) {       // O(n)
        long long x, y;
        pointsInStream >> x >> y;
        points.push_back(Point{x, y});
    }
}

void analyseData(const std::string& name) {
    std::filesystem::path points_name = name;
    std::filesystem::path segments_name = "segments-" + name;

    analyseData(data_dir / points_name, data_dir / "output" / segments_name);
}
