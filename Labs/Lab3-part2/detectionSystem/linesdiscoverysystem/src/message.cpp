#include <iostream>
#include <vector>  // Added
#include <string>  // Added
#include <fstream>
#include <filesystem>
#include <algorithm>  // Added for std::sort, std::unique
#include <cmath>      // Added for std::isinf, std::atan2 (alternative slope)
#include <limits>     // Added for infinity
#include <vector>     // Added (duplicate, remove if already present)
#include <set>        // Added for easy duplicate segment removal
#include <iomanip>    // Added for formatting output
#include <fmt/core.h>  // Assuming fmt library is available as per original code
//#include <format>
#include <find-patterns.hpp>

    const std::filesystem::path data_dir{DATA_DIR};

constexpr int minPoints = 4;  // Minimum points on a line segment

struct Point {
public:
    Point(long long x = 0, long long y = 0) : x_{x}, y_{y} {}  // Use long long for coordinates

    bool operator==(const Point& p) const = default;

    // Compare by y-coordinates and break ties by x-coordinates (Lexicographical order)
    // This is crucial for defining canonical start/end points of segments
    std::strong_ordering operator<=>(const Point& p) const {
        if (y_ < p.y_) return std::strong_ordering::less;
        if (y_ > p.y_) return std::strong_ordering::greater;
        if (x_ < p.x_) return std::strong_ordering::less;
        if (x_ > p.x_) return std::strong_ordering::greater;
        return std::strong_ordering::equivalent;
    }

    // Calculate the slope between this point and 'other'.
    // Handles vertical lines and identical points.
    double slopeTo(const Point& other) const {
        if (y_ == other.y_ && x_ == other.x_) {
            // Same point - slope is undefined, return negative infinity for sorting purposes
            return -std::numeric_limits<double>::infinity();
        }
        if (other.x_ == x_) {
            // Vertical line - slope is positive infinity
            return std::numeric_limits<double>::infinity();
        }
        if (other.y_ == y_) {
            // Horizontal line - slope is 0.0 (positive zero)
            // Differentiating +0.0 and -0.0 might be needed in some contexts,
            // but for grouping points with the same slope, 0.0 is usually sufficient.
            return 0.0;
        }
        // Calculate slope using double precision
        return static_cast<double>(other.y_ - y_) / (other.x_ - x_);
    }

    std::string toString() const { return fmt::format("({},{})", x_, y_); }
    std::string toCoordString() const { return fmt::format("{} {}", x_, y_); }  // For output files

    long long x_;
    long long y_;
};

// --- Needed for using std::set<std::vector<Point>> ---
// Define a comparison operator for vectors of points
bool operator<(const std::vector<Point>& a, const std::vector<Point>& b) {
    // Lexicographical comparison based on the Point's <=> operator
    return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
}

// Function to read points from a file
std::vector<Point> readPoints(const std::filesystem::path& filePath) {
    std::ifstream inFile(filePath);
    if (!inFile) {
        std::cerr << "Error: Cannot open points file: " << filePath << std::endl;
        return {};
    }

    int n;
    inFile >> n;
    if (n <= 0) {
        std::cerr << "Error: Invalid number of points (<= 0) in file: " << filePath << std::endl;
        return {};
    }

    std::vector<Point> points;
    points.reserve(n);  // Reserve space for efficiency
    long long x, y;
    for (int i = 0; i < n; ++i) {
        if (!(inFile >> x >> y)) {
            std::cerr << "Error: Reading point " << i + 1 << " from file: " << filePath
                      << std::endl;
            return {};  // Return partially read points or handle error differently
        }
        points.emplace_back(x, y);
    }
    return points;
}

// Function to write the segments file (start/end points)
void writeSegmentsFile(const std::filesystem::path& filePath,
                       const std::set<std::pair<Point, Point>>& segments) {
    std::ofstream outFile(filePath);
    if (!outFile) {
        std::cerr << "Error: Cannot open segments output file: " << filePath << std::endl;
        return;
    }
    for (const auto& seg : segments) {
        // Output format: x1 y1 x2 y2
        outFile << seg.first.toCoordString() << " " << seg.second.toCoordString() << "\n";
    }
    std::cout << "Segments file written to: " << filePath << std::endl;
}

// Function to write the detailed segments file (all points on the segment)
void writeDetailedSegmentsFile(const std::filesystem::path& filePath,
                               const std::set<std::vector<Point>>& detailedSegments) {
    std::ofstream outFile(filePath);
    if (!outFile) {
        std::cerr << "Error: Cannot open detailed segments output file: " << filePath << std::endl;
        return;
    }
    for (const auto& segmentPoints : detailedSegments) {
        // Output format: Start -> End: (x1,y1), (x2,y2), ..., (xn,yn)
        //outFile << segmentPoints.front().toString() << " -> " << segmentPoints.back().toString()
        //        << ": ";
        for (size_t i = 0; i < segmentPoints.size(); ++i) {
            outFile << segmentPoints[i].toString() << (i == segmentPoints.size() - 1 ? "" : "->");
        }
        outFile << "\n";
    }
    std::cout << "Detailed segments file written to: " << filePath << std::endl;
}

/* ********************************************************** */

int main() {
    std::cout << "Enter the name of input points file (e.g., points5.txt): ";
    std::string points_file;
    std::cin >> points_file;

    analyseData(points_file);  // Call the wrapper function

    return 0;  // Indicate successful execution
}

/* ********************************************************** */

// Main analysis function
void analyseData(const std::filesystem::path& pointsFile,
                 const std::filesystem::path& segmentsFile) {
    // 1. Read Points
    std::vector<Point> points = readPoints(pointsFile);
    if (points.empty()) {
        return;  // Error handled in readPoints
    }
    int n = points.size();
    std::cout << "Read " << n << " points from " << pointsFile << std::endl;

    // It's good practice but not strictly required by the core algorithm's logic *during* slope
    // sorting. However, sorting here helps ensure consistent segment representation (min point
    // first) when adding to the set later.
    std::sort(points.begin(), points.end());
    std::cout << "Initially sorted points by y, then x." << std::endl;

    // Use sets to automatically handle duplicate segments
    // Store pairs of <min_point, max_point> for the simple output
    std::set<std::pair<Point, Point>> found_segments_simple;
    // Store sorted vectors of points for the detailed output
    std::set<std::vector<Point>> found_segments_detailed;

    // Auxiliary vector to hold points for sorting relative to p
    std::vector<Point> other_points;
    other_points.reserve(n);  // Reserve space

    // 2. Iterate through each point 'p' as the original
    for (int i = 0; i < n; ++i) {
        Point p = points[i];
        other_points.clear();  // Clear for the next iteration

        // Create a copy of all points *except* p
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            other_points.push_back(points[j]);
        }

        // 3. Sort the other points based on the slope they make with p
        std::sort(other_points.begin(), other_points.end(), [&p](const Point& q1, const Point& q2) {
            // Compare slopes relative to p
            return p.slopeTo(q1) < p.slopeTo(q2);
            // Alternatively, use angles for potentially better robustness:
            // return p.angleTo(q1) < p.angleTo(q2);
        });

        // 4. Find sequences of points with the same slope
        if (other_points.size() < minPoints - 1) {
            // Not enough other points to form a line of minPoints
            continue;
        }

        int count = 0;  // Counts consecutive points with the same slope
        for (size_t j = 0; j < other_points.size() - 1; ++j) {
            double slope1 = p.slopeTo(other_points[j]);
            double slope2 = p.slopeTo(other_points[j + 1]);

            // Check for equal slopes. Be mindful of floating point comparison issues.
            // Direct comparison often works here because we sorted by these values.
            // If issues arise, use an epsilon comparison: std::abs(slope1 - slope2) < 1e-9
            // Also handle infinities correctly.
            bool slopes_equal =
                (slope1 == slope2) || (std::isinf(slope1) && std::isinf(slope2));  // Both vertical

            if (slopes_equal) {
                if (count == 0) {  // Start of a potential sequence
                    count = 2;     // The first pair
                } else {
                    count++;  // Extend the sequence
                }
            }

            // If slopes are not equal OR we are at the end of a sequence
            // Check if the *previous* sequence was long enough
            if (!slopes_equal || j == other_points.size() - 2) {
                int current_seq_end_index = slopes_equal ? (j + 1) : j;
                int current_seq_start_index = current_seq_end_index - count + 1;

                // Check if the count (number of points *excluding p*) meets the threshold
                if (count >= minPoints - 1) {
                    // We found a collinear set of size count + 1 (including p)
                    std::vector<Point> segmentPoints;
                    segmentPoints.push_back(p);  // Add the origin point
                    // Add points from the collinear sequence
                    for (int k = current_seq_start_index; k <= current_seq_end_index; ++k) {
                        segmentPoints.push_back(other_points[k]);
                    }

                    // Sort the points on the segment to get a canonical representation
                    // Uses the Point's <=> operator (sorts by y, then x)
                    std::sort(segmentPoints.begin(), segmentPoints.end());

                    // Add to sets (duplicates are handled automatically)
                    // Simple segment: pair of min and max points
                    found_segments_simple.insert({segmentPoints.front(), segmentPoints.back()});
                    // Detailed segment: vector of all sorted points
                    found_segments_detailed.insert(segmentPoints);
                }

                // Reset count for the next potential sequence
                count = 0;
            }
        }
    }  // End of outer loop (iterating through p)

    std::cout << "Found " << found_segments_simple.size() << " unique line segments." << std::endl;

    // 5. Write Output Files
    // Ensure output directory exists
    std::filesystem::path outputDir = segmentsFile.parent_path();
    std::filesystem::path detailedOutputDir =
        outputDir / "detailed_output";  // Subfolder for detailed lists
    try {
        std::filesystem::create_directories(outputDir);
        std::filesystem::create_directories(detailedOutputDir);
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error creating output directories: " << e.what() << std::endl;
        return;
    }

    // Construct detailed file path
    std::string detailedFileName = "detailed-" + segmentsFile.filename().string();
    std::filesystem::path detailedSegmentsFile = detailedOutputDir / detailedFileName;

    writeSegmentsFile(segmentsFile, found_segments_simple);
    writeDetailedSegmentsFile(detailedSegmentsFile, found_segments_detailed);

    std::cout << "Analysis complete." << std::endl;
}

// Wrapper function to construct file paths
void analyseData(const std::string& name) {
    std::filesystem::path points_name = name;
    // Ensure the input name is just the file name, not a path
    if (points_name.has_parent_path() && !points_name.parent_path().empty()) {
        points_name = points_name.filename();
        std::cout << "Warning: Input name contained path separators. Using filename: "
                  << points_name << std::endl;
    }

    std::filesystem::path segments_name = "segments-" + points_name.string();  // Use string()

    // Construct full paths
    std::filesystem::path inputFilePath = data_dir / points_name;
    std::filesystem::path outputSegmentsFilePath = data_dir / "output" / segments_name;

    // Call the main analysis function
    analyseData(inputFilePath, outputSegmentsFilePath);
}
