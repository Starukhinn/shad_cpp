#include <algorithm>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::make_heap;
using std::pop_heap;
using std::push_heap;
using std::vector;

struct Point {
    int value;
    int number_vector;
    int64_t number_position_in_vector;
};

bool operator<(const Point& first_point, const Point& second_point) {
    if (first_point.value > second_point.value) {
        return true;
    }
    return false;
}

void FillVector(vector<int>& vector_from_storage) {
    for (int& element : vector_from_storage) {
        cin >> element;
    }
}

void FillStorageOfVectors(vector<vector<int>>& storage_of_vectors) {
    for (vector<int>& one_of_the_vectors : storage_of_vectors) {
        FillVector(one_of_the_vectors);
    }
}

void FillMassiveOfPoints(const vector<vector<int>>& storage_of_vectors,
                         vector<Point>& least_point_from_each_vectors) {
    int number_vector_in_storage = 0;
    for (auto& item : least_point_from_each_vectors) {
        item.value = storage_of_vectors.at(number_vector_in_storage).at(0);
        item.number_vector = number_vector_in_storage;
        item.number_position_in_vector = 0;
        ++number_vector_in_storage;
    }
}

void WriteAscendingSequence(const vector<vector<int>>& storage_of_vectors) {
    vector<Point> least_points_from_each_vectors(storage_of_vectors.size(), {0, 0, 0});
    FillMassiveOfPoints(storage_of_vectors, least_points_from_each_vectors);
    make_heap(least_points_from_each_vectors.begin(), least_points_from_each_vectors.end());
    while (!least_points_from_each_vectors.empty()) {
        Point least_point = least_points_from_each_vectors.front();
        cout << least_point.value << " ";
        pop_heap(least_points_from_each_vectors.begin(), least_points_from_each_vectors.end());
        least_points_from_each_vectors.pop_back();
        int number_of_last_element_in_array =
            storage_of_vectors.at(least_point.number_vector).size() - 1;
        if (least_point.number_position_in_vector != number_of_last_element_in_array) {
            ++least_point.number_position_in_vector;
            least_point.value = storage_of_vectors.at(least_point.number_vector)
                                    .at(least_point.number_position_in_vector);
            least_points_from_each_vectors.push_back(least_point);
            push_heap(least_points_from_each_vectors.begin(), least_points_from_each_vectors.end());
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int number_vectors;
    int number_elements_in_each_vector;
    cin >> number_vectors >> number_elements_in_each_vector;
    vector<vector<int>> storage_of_vectors(number_vectors,
                                           vector<int>(number_elements_in_each_vector));
    FillStorageOfVectors(storage_of_vectors);
    WriteAscendingSequence(storage_of_vectors);
    return 0;
}
