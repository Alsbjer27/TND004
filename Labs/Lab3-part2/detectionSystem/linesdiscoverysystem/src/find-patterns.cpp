#include <iostream>
#include <format>
#include <compare>
#include <fstream>
#include <filesystem>

#include <find-patterns.hpp>

#include <unordered_set>
#include <sstream>
/* **************** Added includes ********************* */
#include <set>
/* ***************************************************** */

const std::filesystem::path data_dir{DATA_DIR};

constexpr int minPoints = 4;

struct Point {
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

#pragma region Function Declerations
std::vector<Point> readPoints(const std::filesystem::path& pointsFilePath);
double calculateSlope(const Point& p, const Point& q);
void sortPointsBySlope(std::vector<Point>& points, const Point& p);
std::string detailsLineFormatting(const std::vector<Point>& pts);
void copyVectorExceptP(const std::vector<Point>& fromVector, std::vector<Point>& toVector,
                       const Point& p);
void writeSegmentsFileDetailed(std::ofstream& segmentsFileDetailed, const std::vector<Point>& pointsLine);
void findCollinearPoints(const Point& p, const std::vector<Point>& other_points,
                         std::set<std::pair<Point, Point>>& uniqueSegmentEndpoints,
                         std::vector<Point>& collinearGroup, std::ofstream& detailedSegmentsStream);
void writeSegmentsFile(const std::filesystem::path& segmentsFile,
                       const std::set<std::pair<Point, Point>>& segments);
#pragma endregion

int main() {
    std::cout << "Enter the name of input points file: ";
    std::string points_file;
    std::cin >> points_file;

    analyseData(points_file);
}

/* ***************************************************** */
using Segment = std::vector<Point>;  // Ett segment �r en lista med punkter

// Kontrollera om segment a �r en delm�ngd av segment b
bool isSubsegment(const Segment& a, const Segment& b) {
    if (a.size() >= b.size())
        return false;  // a kan inte vara en delm�ngd om det �r lika stort eller st�rre
    return std::includes(b.begin(), b.end(), a.begin(),
                         a.end());  // true om alla punkter i a finns i b i ordning
}

void analyseData(const std::filesystem::path& pointsFile,
                 const std::filesystem::path& segmentsFile) {
    std::ifstream input(pointsFile);  // �ppna filen med punkter
    if (!input) {
        std::cerr << "Could not open file: " << pointsFile << "\n";
        return;
// O(n^2 log n) -> O(n) * O(n log n)
void analyseData(const std::filesystem::path& pointsFile, const std::filesystem::path& segmentsFile, 
                 const std::filesystem::path& segmentsFileDetailed) {
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
    std::set<std::pair<Point, Point>> uniqueSegmentEndpoints; 

    other_points.reserve(points.size() - 1);        // O(n) space
    collinearGroup.reserve(points.size());          // O(n) space

    // Open the detailed segments file for writing progressively
    std::ofstream detailedSegmentsStream(segmentsFileDetailed);
    if (!detailedSegmentsStream) {
        std::cerr << "Error: Cannot open detailed segments output file: " << segmentsFileDetailed
                  << std::endl;
        return;
    }

    // 1. 2. 3. Sort points by slope and find collinear
    for (const Point& p : points) {
        copyVectorExceptP(points, other_points, p);

        sortPointsBySlope(other_points, p);

        findCollinearPoints(p, other_points, uniqueSegmentEndpoints, collinearGroup, detailedSegmentsStream);
    }

    detailedSegmentsStream.close();

    writeSegmentsFile(segmentsFile, uniqueSegmentEndpoints);

    // In order to just write out once
    std::cout << "Segments file detailed written to: " << segmentsFileDetailed << std::endl;
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
                         std::vector<Point>& collinearGroup, std::ofstream& detailedSegmentsStream) {

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

        if (collinearGroup.size() < minPoints) {
            collinearGroup = {p, q};
            prevSlope = currentSlope;
            continue;
        }

        // New slope
        auto [minIt, maxIt] = std::minmax_element(
            collinearGroup.begin(), collinearGroup.end(),
            [](auto const& a, auto const& b) { return (a <=> b) == std::strong_ordering::less; });   

        if ((p <=> *minIt) == 0) {
            std::pair<Point, Point> segment = {*minIt, *maxIt};
            if (uniqueSegmentEndpoints.insert(segment).second) {
                writeSegmentsFileDetailed(detailedSegmentsStream, collinearGroup);
            }
        }

        collinearGroup = {p, q};
        prevSlope = currentSlope;
    }

    if (collinearGroup.size() < minPoints) { return; }

    auto [minIt, maxIt] = std::minmax_element(
        collinearGroup.begin(), collinearGroup.end());

    if ((p <=> *minIt) == 0) {
        std::pair<Point, Point> segment = {*minIt, *maxIt};
        if (uniqueSegmentEndpoints.insert(segment).second) {
            writeSegmentsFileDetailed(detailedSegmentsStream, collinearGroup);
        }
    }
}

#pragma region Copy Vector Exept For Point P
// Copy points except p
void copyVectorExceptP(const std::vector<Point>& fromVector, std::vector<Point>& toVector,
                       const Point& p) {
    toVector.clear();
    for (auto const& c : fromVector) {
        if (c == p) continue;
        toVector.push_back(c);
    }
}
#pragma endregion

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
                       const std::set<std::pair<Point, Point>>& uniqueSegments) {
    std::ofstream outFile(segmentsFile);

    if (!outFile) {
        std::cerr << "Error: Cannot open segments output file: " << segmentsFile << std::endl;
        return;
    }

    for (const auto& segment : uniqueSegments) {
        outFile << segment.first.cordOutputString() << " " << segment.second.cordOutputString();
        outFile << "\n";
    }

    outFile.close();
    std::cout << "Segments file written to: " << segmentsFile << std::endl;
}

void writeSegmentsFileDetailed(std::ofstream& detailedSegmentsStream, const std::vector<Point>& pointsLine) {
    detailedSegmentsStream << detailsLineFormatting(pointsLine) << "\n";
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
        std::cout << std::format("Not enough points, {} points are required", minPoints) << std::endl;
        return {};
    }

    int n;
    input >> n;  // L�s antal punkter
    std::vector<Point> points;
    for (int i = 0; i < n; ++i) {
        int x, y;
        input >> x >> y;
        points.emplace_back(x, y);  // L�gg till punkt i listan
    }

    std::sort(points.begin(), points.end());  // Sortera punkterna

    // Skapa utdatafiler f�r segment och detaljer
    std::filesystem::create_directory(segmentsFile.parent_path() / "details");
    std::ofstream segmentsOut(segmentsFile);
    std::ofstream detailsOut(segmentsFile.parent_path() / "details" / segmentsFile.filename());

    // Lambda-funktion f�r att r�kna lutning mellan tv� punkter
    auto slope = [](const Point& a, const Point& b) -> double {
        return (b.x_ == a.x_) ? std::numeric_limits<double>::infinity()
                              : static_cast<double>(b.y_ - a.y_) / (b.x_ - a.x_);
    };

    std::vector<Segment> candidates;            // M�jliga segment
    std::unordered_set<std::string> seen_keys;  // Unika nycklar f�r att undvika dubbletter

    // G� igenom varje punkt som startpunkt
    for (size_t i = 0; i < points.size(); ++i) {
        const Point& origin = points[i];
        std::vector<std::pair<double, Point>> slopes;

        // R�kna lutningar till alla andra punkter
        for (size_t j = 0; j < points.size(); ++j) {
            if (i != j) {
                slopes.emplace_back(slope(origin, points[j]), points[j]);
            }
        }

        // Sortera punkter efter lutning
        std::sort(slopes.begin(), slopes.end(),
                  [](const auto& a, const auto& b) { return a.first < b.first; });

        // S�k efter grupper av punkter med samma lutning (kolline�ra)
        size_t j = 0;
        while (j < slopes.size()) {
            Segment collinear{origin};
            double current_slope = slopes[j].first;

            while (j < slopes.size() && slopes[j].first == current_slope) {
                collinear.push_back(slopes[j].second);
                ++j;
            }

            if (collinear.size() >= minPoints) {
                std::sort(collinear.begin(), collinear.end());

                // Skapa nyckel f�r att identifiera segmentet
                std::ostringstream key_stream;
                for (const auto& pt : collinear) key_stream << pt.x_ << "," << pt.y_ << "->";
                std::string key = key_stream.str();

                // Spara bara om segmentet inte redan �r registrerat
                if (!seen_keys.count(key)) {
                    seen_keys.insert(key);
                    candidates.push_back(collinear);
                }
            }
        }
    }

    // Filtrera bort segment som �r delm�ngder av andra segment
    std::vector<Segment> final_segments;
    for (const auto& seg : candidates) {
        bool isSub = false;
        for (const auto& other : candidates) {
            if (&seg != &other && isSubsegment(seg, other)) {
                isSub = true;
                break;
            }
        }
        if (!isSub) {
            final_segments.push_back(seg);
        }
    }

    // Skriv ut de slutgiltiga segmenten
    for (const auto& seg : final_segments) {
        const Point& p1 = seg.front();
        const Point& p2 = seg.back();
        segmentsOut << p1.x_ << ' ' << p1.y_ << ' ' << p2.x_ << ' ' << p2.y_ << '\n';
        for (const auto& pt : seg) detailsOut << pt.toString() << " ";
        detailsOut << '\n';
    }
}

void analyseData(const std::string& name) {
    std::filesystem::path points_name = name;
    std::filesystem::path segments_name = "segments-" + name;
    analyseData(data_dir / points_name, data_dir / "output" / segments_name);
}
    return points;
}
#pragma endregion
