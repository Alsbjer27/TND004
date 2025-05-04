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

// Function to read points from the input file
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

    std::sort(points.begin(), points.end());

    return points;
}

// Function to calculate the slope between two points.
double calculateSlope(const Point& p, const Point& q) {
    long long dx = q.x_ - p.x_;
    long long dy = q.y_ - p.y_;

    if (dx == 0 && dy == 0) {
        return -std::numeric_limits<double>::infinity();  // Handle identical points
    }

    if (dx == 0) {
        return std::numeric_limits<double>::infinity();  // Handle vertical line
    }

    return static_cast<double>(dy) / static_cast<double>(dx);
}

void sortPointsBySlope(std::vector<Point>& points, const Point& p) {
    std::sort(points.begin(), points.end(), [&p](const Point& a, const Point& b) {
        return calculateSlope(p, a) < calculateSlope(p, b);
    });
}

// Function to check collinearity of 3 points (using cross-product, avoids floating-point issues)
bool areCollinear(const Point& p1, const Point& p2, const Point& p3) {
    return (p2.y_ - p1.y_) * (p3.x_ - p1.x_) == (p3.y_ - p1.y_) * (p2.x_ - p1.x_);
}

// Function to find line segments
std::vector<std::pair<Point, Point>> findLineSegments(const std::vector<Point>& points, std::vector<std::vector<Point>>& allCollinearPoints) {
    std::vector<std::pair<Point, Point>> segments;
    allCollinearPoints.clear();  // Clear previous results
    if (points.size() < minPoints) {
        return segments;  // Not enough points to form a line
    }

    for (size_t i = 0; i < points.size(); ++i) {
        std::map<double, std::vector<size_t>> slopeMap;  // Map to group points by slope
        for (size_t j = 0; j < points.size(); ++j) {
            if (i != j) {
                slopeMap[calculateSlope(points[i], points[j])].push_back(j);
            }
        }

        for (const auto& [slope, indices] : slopeMap) {
            if (indices.size() >= minPoints - 1) {  // At least 3 other points (plus the base point)
                std::vector<Point> collinearPoints = {points[i]};
                for (size_t idx : indices) {
                    collinearPoints.push_back(points[idx]);
                }
                std::sort(collinearPoints.begin(),
                          collinearPoints.end());  // Sort collinear points [cite: 57]
                segments.push_back({collinearPoints.front(), collinearPoints.back()});
                allCollinearPoints.push_back(collinearPoints);
            }
        }
    }

    return segments;
}

// Function to remove redundant line segments [cite: 55, 56, 59, 60, 61, 64, 65]
std::vector<std::pair<Point, Point>> filterRedundantSegments(
    const std::vector<std::pair<Point, Point>>& segments) {
    std::vector<std::pair<Point, Point>> filteredSegments;
    std::set<std::pair<Point, Point>> segmentSet;

    auto toCanonical = [](Point p1, Point p2) {  // Ensure consistent ordering within a segment
        return (p1 <= p2) ? std::make_pair(p1, p2) : std::make_pair(p2, p1);
    };

    for (const auto& seg : segments) {
        if (segmentSet.find(toCanonical(seg.first, seg.second)) ==
            segmentSet.end()) {  // Avoid duplicates [cite: 60]
            bool isSubsegment = false;
            for (const auto& existingSeg : segmentSet) {
                if (areCollinear(seg.first, seg.second, existingSeg.first) &&
                    areCollinear(seg.first, seg.second,
                                 existingSeg.second)) {  // Check if subsegment [cite: 61]
                    std::vector<Point> allPoints = {seg.first, seg.second, existingSeg.first,
                                                    existingSeg.second};
                    std::sort(allPoints.begin(), allPoints.end());
                    if ((allPoints.front() == seg.first || allPoints.front() == seg.second) &&
                        (allPoints.back() == seg.first || allPoints.back() == seg.second) &&
                        (seg.first != existingSeg.first || seg.second != existingSeg.second)) {
                        isSubsegment = true;
                        break;
                    }
                }
            }
            if (!isSubsegment) {
                filteredSegments.push_back(seg);
                segmentSet.insert(toCanonical(seg.first, seg.second));
            }
        }
    }
    return filteredSegments;
}

// Function to write line segments to the output file [cite: 40, 41, 42, 43, 44]
void writeLineSegments(const std::filesystem::path& segmentsFilePath,
                       const std::vector<std::pair<Point, Point>>& segments) {
    std::ofstream segmentsFileStream(segmentsFilePath);
    if (!segmentsFileStream) {
        std::cout << std::format("Segments file path error: {}", segmentsFilePath.string())
                  << std::endl;
        return;
    }

    for (const auto& segment : segments) {
        segmentsFileStream << segment.first.x_ << " " << segment.first.y_ << " "
                           << segment.second.x_ << " " << segment.second.y_ << std::endl;
    }
}

void writeCollinearPoints(const std::filesystem::path& collinearFilePath,
                          const std::vector<std::vector<Point>>& allCollinearPoints) {
    std::ofstream collinearFileStream(collinearFilePath);
    if (!collinearFileStream) {
        std::cout << std::format("Collinear points file path error: {}", collinearFilePath.string())
                  << std::endl;
        return;
    }

    for (const auto& points : allCollinearPoints) {
        for (size_t i = 0; i < points.size(); ++i) {
            collinearFileStream << points[i].toString();
            if (i < points.size() - 1) {
                collinearFileStream << "->";
            }
        }
        collinearFileStream << std::endl;
    }
}

void analyseData(const std::filesystem::path& pointsFile, const std::filesystem::path& segmentsFile) {
    /*
     * Add code here
     * Feel free to modify the function signature
     * Break your code into small functions
     */
    std::vector<Point> points = readPoints(pointsFile);
    if (points.empty()) {
        return;  // Exit if there was an error reading points
    }

    std::vector<std::pair<Point, Point>> lineSegment;

    Point p = points[0];

    findCollinearPoints(points, p, lineSegment);

    for (const auto& segment : lineSegment) {
        std::cout << "Line Segment: " << segment.first.toString() << " ->" << segment.second.toString() << std::endl;
    }

    writeLineSegments(segmentsFile, lineSegment);
}

void analyseData(const std::string& name) {
    std::filesystem::path points_name = name;
    std::filesystem::path segments_name = "segments-" + name;

    analyseData(data_dir / points_name, data_dir / "output" / segments_name);
}

void findCollinearPoints(std::vector<Point>& points, const Point& p, std::vector<std::pair<Point, Point>>& lineSegment) {
    sortPointsBySlope(points, p);

    size_t start = 0;
    while (start < points.size()) {
        size_t end = start + 1;

        while (end < points.size() && calculateSlope(p, points[start]) == calculateSlope(p, points[end])) {
            ++end;
        }
        if (end - start >= 3) {
            lineSegment.push_back({p, points[start]});
            lineSegment.push_back({p, points[end - 1]});
        }
        start = end;
    }
}
