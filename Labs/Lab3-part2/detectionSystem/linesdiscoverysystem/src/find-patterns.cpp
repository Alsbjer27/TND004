#include <iostream>
#include <format>
#include <compare>
#include <fstream>
#include <filesystem>

#include <find-patterns.hpp>

/* **************** Added includes ********************* */
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

std::vector<Point> readPoints(const std::filesystem::path& pointsFilePath);
double calculateSlope(const Point& p, const Point& q);
void sortPointsBySlope(std::vector<Point>& points, const Point& p);
std::string detailsLineFormatting(const std::vector<Point>& pts);

/* ***** Not sure if this approach is O(N^2 log N) ***** */

void writeSegmentsFileDetailed(const std::filesystem::path& segmentsFileDetailed,
                               const std::vector<Point>& pointsLine);

void findCollinearPoints(const Point& p, const std::vector<Point>& other_points,
                         std::set<std::pair<Point, Point>>& uniqueSegmentEndpoints,
                         std::vector<Point>& collinearGroup,
                         const std::filesystem::path& segmentsFileDetailed);

void writeSegmentsFile(const std::filesystem::path& segmentsFile,
                       const std::set<std::pair<Point, Point>>& segments);

/* ***************************************************** */

int main() {
    std::cout << "Enter the name of input points file: ";
    std::string points_file;
    std::cin >> points_file;

    analyseData(points_file);
}

/* ***************************************************** */

// O(n^2 log n) -> O(n) * O(n log n)
void analyseData(const std::filesystem::path& pointsFile, const std::filesystem::path& segmentsFile, const std::filesystem::path& segmentsFileDetailed) {
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

    // Sort by cordinate
    std::sort(points.begin(), points.end());

    std::vector<Point> other_points;
    std::vector<Point> collinearGroup;
    std::vector<Point> linePoints;
    std::set<std::pair<Point, Point>> uniqueSegmentEndpoints; 

    other_points.reserve(points.size() - 1);        // O(n) space
    collinearGroup.reserve(other_points.size());    // O(n) space

    // 1. 2. 3. Sort points by slope and find collinear
    for (const Point& p : points) {
        other_points.clear();

        // Copy points except p
        for (const Point& c : points) {
            if (c == p) { continue; }

            other_points.push_back(c);
        }

        sortPointsBySlope(other_points, p);
        findCollinearPoints(p, other_points, uniqueSegmentEndpoints, collinearGroup, segmentsFileDetailed);
    }

    //writeSegmentsFile(segmentsFile, uniqueSegmentEndpoints);
}

void analyseData(const std::string& name) {
    std::filesystem::path points_name = name;
    std::filesystem::path segments_name = "segments-" + name;
    std::filesystem::path segments_detailed_name = "segments-detailed" + name;

    analyseData(data_dir / points_name, data_dir / "output" / segments_name, data_dir / "output" / segments_detailed_name);
}

/* ***************************************************** */

#pragma region Sorting points by slope
// Sort by slope: O(n log n)
void sortPointsBySlope(std::vector<Point>& points, const Point& p) {
    std::stable_sort(points.begin(), points.end(), [&p](const Point& a, const Point& b) {
        return calculateSlope(p, a) < calculateSlope(p, b);
    });
}

// Function to calculate the slope between two points: O(1)
double calculateSlope(const Point& p, const Point& q) {
    long long dx = q.x_ - p.x_;
    long long dy = q.y_ - p.y_;

    // Handle identical points
    if (dx == 0 && dy == 0) {
        return -std::numeric_limits<double>::infinity();
    }

    // Handle vertical line
    if (dx == 0) {
        return std::numeric_limits<double>::infinity();
    }

    if (dy == 0) {
        return 0.0;
    }

    return static_cast<double>(q.y_ - p.y_) / static_cast<double>(q.x_ - p.x_);
}
#pragma endregion

void findCollinearPoints(const Point& p, const std::vector<Point>& other_points,
                         std::set<std::pair<Point, Point>>& uniqueSegmentEndpoints,
                         std::vector<Point>& collinearGroup,
                         const std::filesystem::path& segmentsFileDetailed) {

    double prevSlope = std::numeric_limits<double>::quiet_NaN();
    collinearGroup.clear();

    collinearGroup = {p};
    for (const Point& q : other_points) {
        double currentSlope = calculateSlope(p, q);

        if (std::isnan(prevSlope) || currentSlope == prevSlope) {
            collinearGroup.push_back(q);
            prevSlope = currentSlope;
            continue;
        }

        if (collinearGroup.size() + 1 < minPoints) {
            collinearGroup = {p, q};
            prevSlope = currentSlope;
            continue;
        }

        // New slope
        Point minPt = *std::min_element(collinearGroup.begin(), collinearGroup.end());      // <- Chat GPT O(n)

        if ((p <=> minPt) == 0) {
            std::pair<Point, Point> pair = {collinearGroup.front(), collinearGroup.back()};
            if (uniqueSegmentEndpoints.insert(pair).second) {
                writeSegmentsFileDetailed(segmentsFileDetailed, collinearGroup);
            }
        }

        collinearGroup = {p, q};
        prevSlope = currentSlope;
    }

    if (collinearGroup.size() + 1 < minPoints) { return; }

    Point minPt = *std::min_element(collinearGroup.begin(), collinearGroup.end());          // <- Chat GPT O(n)

    if ((p <=> minPt) == 0) {
        std::pair<Point, Point> pair = {collinearGroup.front(), collinearGroup.back()};

        if (uniqueSegmentEndpoints.insert(pair).second) {
            writeSegmentsFileDetailed(segmentsFileDetailed, collinearGroup);
        }
    }
}

#pragma region Write File Functions
std::string detailsLineFormatting(const std::vector<Point>& pts) {
    std::string s;
    for (size_t i = 0; i < pts.size(); ++i) {
        s += pts[i].toString();
        if (i + 1 < pts.size()) s += "->";
    }
    return s;
}

// Modified writeSegmentsFile to output endpoint pairs
void writeSegmentsFile(const std::filesystem::path& segmentsFile,
                       const std::set<std::pair<Point, Point>>& segments) {
    std::ofstream outFile(segmentsFile);

    if (!outFile) {
        std::cerr << "Error: Cannot open segments output file: " << segmentsFile << std::endl;
        return;
    }

    for (const auto& segment : segments) {
        outFile << segment.first.cordOutputString() << " " << segment.second.cordOutputString();
        outFile << "\n";
    }

    outFile.close();
    std::cout << "Segments file written to: " << segmentsFile << std::endl;
}

void writeSegmentsFileDetailed(const std::filesystem::path& segmentsFileDetailed,
                               const std::vector<Point>& pointsLine) {
    // Open the detailed segments file for writing progressively
    std::ofstream detailedSegmentsStream(segmentsFileDetailed);
    if (!detailedSegmentsStream) {
        std::cerr << "Error: Cannot open detailed segments output file: " << segmentsFileDetailed
                  << std::endl;
        // Handle error appropriately
        return;
    }

    detailedSegmentsStream << detailsLineFormatting(pointsLine) << "\n";

    detailedSegmentsStream.close();

    std::cout << "Segments file written to: " << segmentsFileDetailed << std::endl;
}
#pragma endregion

#pragma region Read Points Function
// Function to read points from the input file: O(n)
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
                         "Not enough points, in order to find patterns, a minimum of {} points are "
                         "required",
                         minPoints)
                  << std::endl;
        return {};
    }

    std::vector<Point> points;
    points.reserve(n_points);

    for (int i = 0; i < n_points; ++i) {
        int x, y;
        pointsFileStream >> x >> y;
        points.push_back(Point{x, y});
    }

    pointsFileStream.close();

    return points;
}
#pragma endregion