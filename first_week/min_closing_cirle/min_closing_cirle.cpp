#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <utility>
#include <cmath>

using std::vector;
using std::istream;
using std::pair;
using std::ostream;


struct Point {
    int x_coord = 0;
    int y_coord = 0;
};

istream& operator >> (istream& is, Point& point) {
    is >> point.x_coord >> point.y_coord;
    return is;
}

vector<Point> ReadPoints(istream& is, const int& n_points) {
    vector<Point> points(n_points);
    for (Point& point: points) {
        is >> point;
    }
    return points;
}

void PrintResult(ostream& os, double radius) {
    os << std::fixed << std::setprecision(6) << radius << '\n';
}

double CountSquareDistanceFromCenterOfSurfaceToPoint(const Point& point) {
    return double(point.x_coord * point.x_coord
           + point.y_coord * point.y_coord);
}

int CountNumberPointsInCircle(const vector<Point>& points, const double& radius) {
    vector<pair<double, char>> extreme_positions_of_points_along_ox;
    char the_extreme_position_of_the_point_on_the_left_along_ox = 'l';
    char the_extreme_position_of_the_point_on_the_right_along_ox = 'r';
    for (const Point& point: points) {
        bool point_ma_be_in_circle_with_now_radius = radius >= std::abs(point.y_coord);
        if (point_ma_be_in_circle_with_now_radius) {
            double offset_of_point_in_ox = std::sqrt(radius * radius - point.y_coord * point.y_coord);
            extreme_positions_of_points_along_ox.push_back({point.x_coord - offset_of_point_in_ox,
                 the_extreme_position_of_the_point_on_the_left_along_ox});
            extreme_positions_of_points_along_ox.push_back({point.x_coord + offset_of_point_in_ox,
                 the_extreme_position_of_the_point_on_the_right_along_ox});
        }
    }
    std::sort(begin(extreme_positions_of_points_along_ox), end(extreme_positions_of_points_along_ox));

    int max_intersections_along_ox = 0;
    int intersections_along_ox_in_this_moment = 0;
    for (const auto& [intersection, boundary]: extreme_positions_of_points_along_ox) {
        if (boundary == the_extreme_position_of_the_point_on_the_left_along_ox) {
            ++intersections_along_ox_in_this_moment;
        }
        else if (boundary == the_extreme_position_of_the_point_on_the_right_along_ox) {
            --intersections_along_ox_in_this_moment;
        }
        max_intersections_along_ox = std::max(intersections_along_ox_in_this_moment, max_intersections_along_ox);
    }

    return max_intersections_along_ox;
}

double FindLowerBoundForRadiusToCoverPoints(const vector<Point>& points, double lower_bound_radius, double upper_bound_radius,
                                    const int& number_points_in_circle) {
    double search_precision = 1e-6;
    double radius = lower_bound_radius + (upper_bound_radius -
                     lower_bound_radius) / 2;
    double previous_radius = 0;
    bool radius_of_circle_is_min = std::abs(previous_radius - radius) > search_precision;

    while (radius_of_circle_is_min) {
        int n_intersecting_cuts = CountNumberPointsInCircle(points, radius);
        if (n_intersecting_cuts >= number_points_in_circle) {
            upper_bound_radius = radius;
        } else if (n_intersecting_cuts < number_points_in_circle) {
            lower_bound_radius = radius;
        }
        previous_radius = radius;
        radius = lower_bound_radius + (upper_bound_radius -
                  lower_bound_radius) / 2;
        radius_of_circle_is_min = std::abs(previous_radius - radius) > search_precision;
    }
    return radius;
}

double FindMinimumRadius(const vector<Point>& points, const int& number_points_in_circle) {
    double min_distance_to_x_axis_from_points_in_surface = double(std::abs(
                                              std::min_element(points.begin(), points.end(),
                                                               [](const Point& first_point, const Point& second_point){
                                                                   return std::abs(first_point.y_coord) < std::abs(second_point.y_coord);
                                                                    })->y_coord));
    const Point& the_furthest_point_from_center_of_plane = *std::max_element(
        points.begin(), points.end(),
        [](const Point& first_point, const Point& second_point){
            return CountSquareDistanceFromCenterOfSurfaceToPoint(first_point) <
                   CountSquareDistanceFromCenterOfSurfaceToPoint(second_point);}
    );

    return FindLowerBoundForRadiusToCoverPoints(
        points, min_distance_to_x_axis_from_points_in_surface,
        CountSquareDistanceFromCenterOfSurfaceToPoint(the_furthest_point_from_center_of_plane),
        number_points_in_circle);
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    istream& is = std::cin;
    ostream& os = std::cout;


    int number_points_in_surface, number_points_in_circle;
    is >> number_points_in_surface >> number_points_in_circle;
    vector<Point> points = ReadPoints(is, number_points_in_surface);
    double radius = FindMinimumRadius(points, number_points_in_circle);
    PrintResult(os, radius);
    return 0;
}