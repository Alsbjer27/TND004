#include <iostream>
#include <format>
#include <compare>
#include <fstream>
#include <filesystem>

#include <find-patterns.hpp>

#include <unordered_set>
#include <sstream>
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

/* ***************************************************** */

int main() {
    std::cout << "Enter the name of input points file: ";
    std::string points_file;
    std::cin >> points_file;

    analyseData(points_file);
}

/* ***************************************************** */
using Segment = std::vector<Point>;  // Ett segment är en lista med punkter

// Kontrollera om segment a är en delmängd av segment b
bool isSubsegment(const Segment& a, const Segment& b) {
    if (a.size() >= b.size())
        return false;  // a kan inte vara en delmängd om det är lika stort eller större
    return std::includes(b.begin(), b.end(), a.begin(),
                         a.end());  // true om alla punkter i a finns i b i ordning
}

void analyseData(const std::filesystem::path& pointsFile,
                 const std::filesystem::path& segmentsFile) {
    std::ifstream input(pointsFile);  // Öppna filen med punkter
    if (!input) {
        std::cerr << "Could not open file: " << pointsFile << "\n";
        return;
    }

    int n;
    input >> n;  // Läs antal punkter
    std::vector<Point> points;
    for (int i = 0; i < n; ++i) {
        int x, y;
        input >> x >> y;
        points.emplace_back(x, y);  // Lägg till punkt i listan
    }

    std::sort(points.begin(), points.end());  // Sortera punkterna

    // Skapa utdatafiler för segment och detaljer
    std::filesystem::create_directory(segmentsFile.parent_path() / "details");
    std::ofstream segmentsOut(segmentsFile);
    std::ofstream detailsOut(segmentsFile.parent_path() / "details" / segmentsFile.filename());

    // Lambda-funktion för att räkna lutning mellan två punkter
    auto slope = [](const Point& a, const Point& b) -> double {
        return (b.x_ == a.x_) ? std::numeric_limits<double>::infinity()
                              : static_cast<double>(b.y_ - a.y_) / (b.x_ - a.x_);
    };

    std::vector<Segment> candidates;            // Möjliga segment
    std::unordered_set<std::string> seen_keys;  // Unika nycklar för att undvika dubbletter

    // Gå igenom varje punkt som startpunkt
    for (size_t i = 0; i < points.size(); ++i) {
        const Point& origin = points[i];
        std::vector<std::pair<double, Point>> slopes;

        // Räkna lutningar till alla andra punkter
        for (size_t j = 0; j < points.size(); ++j) {
            if (i != j) {
                slopes.emplace_back(slope(origin, points[j]), points[j]);
            }
        }

        // Sortera punkter efter lutning
        std::sort(slopes.begin(), slopes.end(),
                  [](const auto& a, const auto& b) { return a.first < b.first; });

        // Sök efter grupper av punkter med samma lutning (kollineära)
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

                // Skapa nyckel för att identifiera segmentet
                std::ostringstream key_stream;
                for (const auto& pt : collinear) key_stream << pt.x_ << "," << pt.y_ << "->";
                std::string key = key_stream.str();

                // Spara bara om segmentet inte redan är registrerat
                if (!seen_keys.count(key)) {
                    seen_keys.insert(key);
                    candidates.push_back(collinear);
                }
            }
        }
    }

    // Filtrera bort segment som är delmängder av andra segment
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