#include <iostream>
#include <format>
#include <compare>
#include <fstream>
#include <filesystem>

#include <find-patterns.hpp>
#include <vector>
#include <set>

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
    std::string toOutputString() const { return std::format("{} {}", x_, y_); }

    long long x_;
    long long y_;
};

#pragma region Function Declerations
std::vector<Point> readPoints(const std::filesystem::path& pointsFilePath);
std::vector<Point>& copyPointsExceptP(std::vector<Point>& fromVector, std::vector<Point>& toVector,
                                      const Point& p);
double calculateSlope(const Point& p, const Point& q);
void sortPointsBySlope(std::vector<Point>& points, const Point& p);
void findCollinearPoints(const Point& p, const std::vector<Point>& pointsExceptP,
                         std::set<std::vector<Point>>& uniqueSegments,
                         std::vector<Point>& collinearGroup);
void analyseData(const std::filesystem::path& pointsFile, const std::filesystem::path& segmentsFile,
                 const std::filesystem::path& segmentsFileDetailed);
void writeSegmentsFiles(const std::filesystem::path& segmentsFile,
                        const std::filesystem::path& segmentsFileDetailed,
                        const std::set<std::vector<Point>>& uniqueSegments);
#pragma endregion

/* ***************************************************** */

int main() {
    std::cout << "Enter the name of input points file: ";
    std::string points_file;
    std::cin >> points_file;

    analyseData(points_file);
}

/* ***************************************************** */

void analyseData(const std::string& name) {
    std::filesystem::path points_name = name;
    std::filesystem::path segments_name = "segments-" + name;
    std::filesystem::path segments_detailed_name = "segments-detailed" + name;

    analyseData(data_dir / points_name, data_dir / "output" / segments_name,
                data_dir / "output" / segments_detailed_name);
}

void analyseData(const std::filesystem::path& pointsFile, const std::filesystem::path& segmentsFile,
                 const std::filesystem::path& segmentsFileDetailed) {
    /*
     * Add code here
     * Feel free to modify the function signature
     * Break your code into small functions
     */

    std::vector<Point> points = readPoints(pointsFile);
    std::sort(points.begin(), points.end());

    // Vector to store all points except P
    std::vector<Point> pointsExceptP;
    pointsExceptP.reserve(points.size() - 1);

    // Store unique segments
    std::set<std::vector<Point>> uniqueSegments;

    std::vector<Point> collinearGroup;
    collinearGroup.reserve(points.size());

    for (const Point& p : points) {
        pointsExceptP = copyPointsExceptP(points, pointsExceptP, p);
        sortPointsBySlope(pointsExceptP, p);
        findCollinearPoints(p, pointsExceptP, uniqueSegments, collinearGroup);
    }

    writeSegmentsFiles(segmentsFile, segmentsFileDetailed, uniqueSegments);
}

void sortPointsBySlope(std::vector<Point>& points, const Point& p) {
    std::stable_sort(points.begin(), points.end(), [&p](const Point& a, const Point& b) {
        return calculateSlope(p, a) < calculateSlope(p, b);
    });
}

void findCollinearPoints(const Point& p, const std::vector<Point>& pointsExceptP,
                         std::set<std::vector<Point>>& uniqueSegments,
                         std::vector<Point>& collinearGroup) {
    collinearGroup = {p};

    Point minPoint = p;
    double prevSlope = std::numeric_limits<double>::quiet_NaN(); // Define it as NaN just to initilize it

    for (const Point& q : pointsExceptP) {
        double currentSlope = calculateSlope(p, q);

        if (std::isnan(prevSlope) || currentSlope == prevSlope) {
            if (q < minPoint) { minPoint = q; }
            collinearGroup.push_back(q);
            prevSlope = currentSlope;
            continue;
        }

        // New slope
        if ((collinearGroup.size() > minPoints - 1) && minPoint == p) {
            uniqueSegments.insert(collinearGroup);
        }

        collinearGroup = {p, q};
        prevSlope = currentSlope;
        minPoint = (q < p) ? q : p;
    }

    // Handle the last set of slopes
    if ((collinearGroup.size() > minPoints - 1) && minPoint == p) {
        uniqueSegments.insert(collinearGroup);
    }
}

#pragma region Calculate Slope
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

#pragma region Copy Points Except P
// Copy points except P, need to pass in toVector in order to have reserve and not reserve memory in the loop
std::vector<Point>& copyPointsExceptP(std::vector<Point>& fromVector, std::vector<Point>& toVector, const Point& p) {
    toVector.clear();
    for (const Point& c : fromVector) {
        if (c == p) {
            continue;
        }
        toVector.push_back(c);
    }

    return toVector;
}
#pragma endregion

#pragma region Read Points Function
std::vector<Point> readPoints(const std::filesystem::path& pointsFilePath) {
    std::ifstream pointsFileStream(pointsFilePath);

    if (!pointsFileStream) {
        std::cout << "Points file path error" << std::endl;
        return {};
    }

    int n_points{0};
    pointsFileStream >> n_points;

    if (n_points < minPoints) {
        std::cout << std::format("Not enough points, {} points are required", minPoints)
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

#pragma region Write Files
std::string detailsLineFormatting(const std::vector<Point>& pts) {
    std::string s;
    for (size_t i = 0; i < pts.size(); ++i) {
        s += pts[i].toString();
        if (i + 1 < pts.size()) s += "->";
    }
    return s;
}

void writeSegmentsFiles(const std::filesystem::path& segmentsFile,
                        const std::filesystem::path& segmentsFileDetailed, 
                        const std::set<std::vector<Point>>& uniqueSegments) {
    std::ofstream outFile(segmentsFile);
    std::ofstream outFileDetailed(segmentsFileDetailed);

    if (!outFile) {
        std::cerr << "Error: Cannot open segments output file: " << segmentsFile << std::endl;
        return;
    }

    if (!outFileDetailed) {
        std::cerr << "Error: Cannot open segments detailed output file: " << segmentsFileDetailed
                  << std::endl;
        return;
    }

    for (const auto& group : uniqueSegments) {
        outFile << group.front().toOutputString() << " " << group.back().toOutputString()
                << std::endl;

        outFileDetailed << detailsLineFormatting(group) << std::endl;
    }

    outFile.close();
    outFileDetailed.close();
    std::cout << "Segments file written to: " << segmentsFile << std::endl;
    std::cout << "Segments detailed file written to: " << segmentsFileDetailed << std::endl;
}
#pragma endregion