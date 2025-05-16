void findCollinearPoints(const Point& p, const std::vector<Point>& other_points,
                         std::set<std::pair<Point, Point>>& uniqueSegmentEndpoints,
                         std::vector<Point>& collinearGroup,
                         const std::filesystem::path& segmentsFileDetailed) {

    double prevSlope = std::numeric_limits<double>::quiet_NaN();
    collinearGroup.clear();

    for (const Point& q : other_points) {
        double currentSlope = calculateSlope(p, q);

        if (collinearGroup.empty() || currentSlope == prevSlope) {
            collinearGroup.push_back(q);
            prevSlope = currentSlope;
            continue;
        }

        // New slope
        if (collinearGroup.size() + 1 >= minPoints - 1) {
            std::cout << p.toString() << "->";
            for (const Point& c : collinearGroup) {
                if (c != collinearGroup.back()) {
                    std::cout << c.toString() << "->";
                    continue;
                }
                std::cout << c.toString() << std::endl;
            }
        }

        collinearGroup = {q};
        prevSlope = currentSlope;
    }

    if (collinearGroup.size() >= minPoints - 1) {
        std::cout << p.toString() << "->";
        for (const Point& c : collinearGroup) {
            if (c != collinearGroup.back()) {
                std::cout << c.toString() << "->";
                continue;
            }
            std::cout << c.toString() << std::endl;
        }
    }
}