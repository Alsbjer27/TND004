#include <iostream>
#include <format>
#include <compare>
#include <fstream>
#include <filesystem>

#include <find-patterns.hpp>

/* **************** Added includes ********************* */
#include <map>
#include <set>
/* ***************************************************** */

const std::filesystem::path data_dir{DATA_DIR};

constexpr int minPoints = 4;

struct Point { // feel free to modify
public:
    Point(int x = 0, int y = 0) : x_{x}, y_{y} {}

    bool operator==(const Point& p) const = default;

    // Compare by y-coordinates and break ties by x-coordinates
    std::strong_ordering operator<=>(Point p) const {
        if (y_ < p.y_) { return std::strong_ordering::less; }
        if (y_ > p.y_) { return std::strong_ordering::greater; }
        if (x_ < p.x_) { return std::strong_ordering::less; }
        if (x_ > p.x_) { return std::strong_ordering::greater; }
        return std::strong_ordering::equivalent;
    }

    std::string toString() const { return std::format("({},{})", x_, y_); }
    std::string cordOutputString() const { return std::format("{} {}", x_, y_); }

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

// Function to read points from the input file: O(n * log n) -> O(n) + O(n log n)
std::vector<Point> readPoints(const std::filesystem::path& pointsFilePath) {
    std::ifstream pointsFileStream(pointsFilePath);

    if (!pointsFileStream) {
        std::cout << "Points file path error" << std::endl;
        return {};
    }

    int n_points{0};
    pointsFileStream >> n_points;

    if (n_points < minPoints) {
        std::cout << std::format(
            "Not enough points, in order to find patterns, a minimum of {} points are required",
            minPoints) << std::endl;
        return {};
    }

    std::vector<Point> points;
    points.reserve(n_points);
    for (int i = 0; i < n_points; ++i) {
        long long x, y;
        pointsFileStream >> x >> y;
        points.push_back(Point{x, y});
    }

    pointsFileStream.close();

    // Sorting using the spaceship operator (sorted by y cord, then x if y is equal)
    std::sort(points.begin(), points.end());

    return points;
}

// Function to calculate the slope between two points: O(1)
double calculateSlope(const Point& p, const Point& q) {
    if (p.x_ == q.x_ && p.y_ == q.y_) {
        return -std::numeric_limits<double>::infinity();  // Handle identical points
    }

    if (p.x_ == q.x_) {
        return std::numeric_limits<double>::infinity();  // Handle vertical line
    }

    return static_cast<double>(q.y_ - p.y_) / static_cast<double>(q.x_ - p.x_);
}

// Calculate if 3 points are collinear: O(1)
bool areCollinear(const Point& p1, const Point& p2, const Point& p3) {
    return ((p2.y_ - p1.y_) * (p3.x_ - p1.x_)) == ((p3.y_ - p1.y_) * (p2.x_ - p1.x_));
}

// Sort by slope: O(n log n)
void sortPointsBySlope(std::vector<Point>& points, const Point& p) {
    std::sort(points.begin(), points.end(), [&p](const Point& a, const Point& b) {
        return calculateSlope(p, a) < calculateSlope(p, b);
    });
}

// O(n^2 log n) -> O(n) * O(n log n)
void analyseData(const std::filesystem::path& pointsFile, const std::filesystem::path& segmentsFile) {
    /*
     * Add code here
     * Feel free to modify the function signature
     * Break your code into small functions
     */
    
    // Read points from the input file
    std::vector<Point> points = readPoints(pointsFile);
    if (points.empty()) {
        return;  // Exit if there was an error reading points
    }

    // 1. and 2. Sort points by slope
    for (const Point& p : points) {
        sortPointsBySlope(points, p);
    }
}

void analyseData(const std::string& name) {
    std::filesystem::path points_name = name;
    std::filesystem::path segments_name = "segments-" + name;

    analyseData(data_dir / points_name, data_dir / "output" / segments_name);
}
