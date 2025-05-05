#pragma once

#include <iostream>
#include <string>

/*
* Discoveres line segments of at least 4 points in file name
*/
void analyseData(const std::string& name);

void findCollinearPoints(std::vector<Point>& points, const Point& p, std::vector<Point, Point>& lineSegment);

void sortPointsBySlope(std::vector<Point>& points, const Point& p);