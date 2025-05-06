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

std::vector<Point> readPoints(const std::filesystem::path& pointsFilePath);
double calculateSlope(const Point& p, const Point& q);
bool areCollinear(const Point& p1, const Point& p2, const Point& p3);
void sortPointsBySlope(std::vector<Point>& points, const Point& p);
void findCollinearPoints(const Point& p, const std::vector<Point>& points, std::set<std::vector<Point>>& lineSegments);
void writeSegmentsFile(const std::filesystem::path& segmentsFile, const std::filesystem::path& segmentsFileDetailed, const std::set<std::vector<Point>>& segments);

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

    // Create a set to store unique line segments
    std::set<std::vector<Point>> lineSegments;
    std::vector<Point> other_points = points;

    // 1. 2. 3. Sort points by slope and find collinear
    for (const Point& p : points) {
        sortPointsBySlope(other_points, p);

        // Vector sorted by slope (-inf -> inf)
        findCollinearPoints(p, other_points, lineSegments);
    }

    // Write the unique line segments to the output file
    writeSegmentsFile(segmentsFile, segmentsFileDetailed, lineSegments);
}

void analyseData(const std::string& name) {
    std::filesystem::path points_name = name;
    std::filesystem::path segments_name = "segments-" + name;
    std::filesystem::path segments_detailed_name = "segments-detailed" + name;

    analyseData(data_dir / points_name, data_dir / "output" / segments_name, data_dir / "output" / segments_detailed_name);
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
        int x, y;
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
    long long dx = q.x_ - p.x_;
    long long dy = q.y_ - p.y_;

    if (dx == 0 && dy == 0) {
        return -std::numeric_limits<double>::infinity();  // Handle identical points
    }

    if (dx == 0) {
        return std::numeric_limits<double>::infinity();  // Handle vertical line
    }

    if (dy == 0) {
        return 0.0;
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

void findCollinearPoints(const Point& p, const std::vector<Point>& other_points, std::set<std::vector<Point>>& lineSegments) {
    std::vector<Point> collinearGroup;
    double prevSlope = std::numeric_limits<double>::quiet_NaN();
    collinearGroup.reserve(other_points.size());
    // Reserve för collinear -> Skippar att loopen blir O(n^2) -> reserve other_points.size() ifall alla ligger på samma linje?
    // Ta bort / Flytta ut sort
    // Ändra så att man inte kör insert här??

    // O(n^2) -> worst case O(n^3) [push_back]
    for (size_t i = 0; i < other_points.size(); ++i) {
        const Point& q = other_points[i];
        if (p == q) continue;

        double slope = calculateSlope(p, q);

        if (collinearGroup.empty() || slope == prevSlope) {
            collinearGroup.push_back(q);
        } else {
            if (collinearGroup.size() >= minPoints - 1) {
                collinearGroup.push_back(p);
                std::sort(collinearGroup.begin(), collinearGroup.end());

                // Undvik att samma sekvens upptäcks från flera punkter
                if (p == collinearGroup[0]) {
                    lineSegments.insert(collinearGroup);
                }
            }
            collinearGroup = {q};
        }

        prevSlope = slope;
    }

    // Kontrollera sista gruppen också
    if (collinearGroup.size() >= minPoints - 1) {
        collinearGroup.push_back(p);
        std::sort(collinearGroup.begin(), collinearGroup.end());

        if (p == collinearGroup[0]) {
            lineSegments.insert(collinearGroup);
        }
    }
}

void writeSegmentsFile(const std::filesystem::path& segmentsFile, const std::filesystem::path& segmentsFileDetailed, const std::set<std::vector<Point>>& segments) {
    std::ofstream outFile(segmentsFile);
    std::ofstream outFileDetailed(segmentsFileDetailed);
    
    if (!outFile) {
        std::cerr << "Error: Cannot open segments output file: " << segmentsFile << std::endl;
        return;
    }

    if (!outFileDetailed) {
        std::cerr << "Error: Cannot open detailed segments output file: " << segmentsFileDetailed << std::endl;
        return;
    }

    // Segment file writing
    for (const auto& segment : segments) {
        outFile << segment[0].cordOutputString() << " " << segment.back().cordOutputString();
        outFile << "\n";
    }

    outFile.close();

    // Detailed file writing
    for (const auto& segment : segments) {
        for (size_t i = 0; i < segment.size(); ++i) {
            outFileDetailed << segment[i].toString();
            if (i != segment.size() - 1) outFileDetailed << "->";
        }
        outFileDetailed << "\n";
    }

    outFileDetailed.close();

    std::cout << "Segments file written to: " << segmentsFile << std::endl;
    std::cout << "Detailed segments file written to: " << segmentsFileDetailed << std::endl;
}