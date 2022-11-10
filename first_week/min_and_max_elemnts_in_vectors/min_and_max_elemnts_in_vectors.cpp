#include <iostream>
#include <vector>

using std::vector;

void FillVector(vector<int>& vector_from_storage) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    for (int& element: vector_from_storage){
        std::cin >> element;
    }
}

void MakeStorageOfFixTypeArrays(vector<vector<int>>& array_of_fix_type_arrays) {
    for (vector<int>& one_of_the_arrays: array_of_fix_type_arrays){
        FillVector(one_of_the_arrays);
    }
}

int AlgorithmForFindingElementOfCrossingArrays(const vector<int>& increasing_array,
                                               const vector<int>& decreasing_array,
                                               const int& start_arrays, const int& stop_arrays) {
    int number_middle_element_in_arrays = (stop_arrays + start_arrays) / 2;
    if (number_middle_element_in_arrays == start_arrays or
        number_middle_element_in_arrays == stop_arrays) {
        if (std::max(increasing_array[start_arrays], decreasing_array[start_arrays]) <
            std::max(increasing_array[stop_arrays], decreasing_array[stop_arrays])) {
            return start_arrays;
        }
        return stop_arrays;
    }
    if (increasing_array[number_middle_element_in_arrays] >
        decreasing_array[number_middle_element_in_arrays]) {
        return AlgorithmForFindingElementOfCrossingArrays(
            increasing_array, decreasing_array, start_arrays, number_middle_element_in_arrays);
    } else if (increasing_array[number_middle_element_in_arrays] <
               decreasing_array[number_middle_element_in_arrays]) {
        return AlgorithmForFindingElementOfCrossingArrays(
            increasing_array, decreasing_array, number_middle_element_in_arrays, stop_arrays);
    }
    return number_middle_element_in_arrays;
}

int FindElementOfCrossingArrays(const int& number_increasing_array,
                                const int& number_decreasing_array,
                                const vector<vector<int>>& array_of_increasing_arrays,
                                const vector<vector<int>>& array_of_decreasing_arrays,
                                const int& size_of_array) {
    int first_element_from_increasing_array = array_of_increasing_arrays.at(number_increasing_array)[0];
    int first_element_from_decreasing_array = array_of_decreasing_arrays.at(number_decreasing_array)[0];
    int last_element_from_increasing_array = array_of_increasing_arrays.at(number_increasing_array)[size_of_array - 1];
    int last_element_from_decreasing_array = array_of_decreasing_arrays.at(number_decreasing_array)[size_of_array - 1];
    bool increasing_array_crosses_decreasing_array =
        first_element_from_increasing_array <= first_element_from_decreasing_array and
        last_element_from_increasing_array >= last_element_from_decreasing_array;
    if (increasing_array_crosses_decreasing_array) {
        int number_of_minimum_of_maximum_element = AlgorithmForFindingElementOfCrossingArrays(
            array_of_increasing_arrays.at(number_increasing_array), array_of_decreasing_arrays.at(number_decreasing_array), 0, size_of_array - 1);
            ++number_of_minimum_of_maximum_element;
        return number_of_minimum_of_maximum_element;
    } else if (std::max(array_of_increasing_arrays.at(number_increasing_array)[0], array_of_decreasing_arrays.at(number_decreasing_array)[0]) <
               std::max(array_of_increasing_arrays.at(number_increasing_array)[size_of_array - 1], array_of_decreasing_arrays.at(number_decreasing_array)[size_of_array - 1])) {
        return 1;
    }
    return size_of_array;
}

void ProcessRequests(int& number_request_about_min_max_elements,
                     const vector<vector<int>>& array_of_increasing_arrays,
                     const vector<vector<int>>& array_of_decreasing_arrays,
                     const int& size_of_array) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    while (number_request_about_min_max_elements > 0) {
        --number_request_about_min_max_elements;
        int number_increasing_array;
        int number_decreasing_array;
        std::cin >> number_increasing_array >> number_decreasing_array;
        --number_increasing_array;
        --number_decreasing_array;
        int number_min_max_element = FindElementOfCrossingArrays(
            number_increasing_array, number_decreasing_array, array_of_increasing_arrays,
            array_of_decreasing_arrays, size_of_array);
        std::cout << number_min_max_element << "\n";
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int number_increasing_arrays;
    int number_decreasing_arrays;
    int size_of_arrays;
    std::cin >> number_increasing_arrays >> number_decreasing_arrays >> size_of_arrays;
    vector<vector<int>> array_of_increasing_arrays(number_increasing_arrays, vector<int> (size_of_arrays));
    MakeStorageOfFixTypeArrays(array_of_increasing_arrays);
    vector<vector<int>> array_of_decreasing_arrays(number_decreasing_arrays, vector<int> (size_of_arrays));
    MakeStorageOfFixTypeArrays(array_of_decreasing_arrays);
    int number_request_about_min_max_elements;
    std::cin >> number_request_about_min_max_elements;
    ProcessRequests(number_request_about_min_max_elements, array_of_increasing_arrays,
                    array_of_decreasing_arrays, size_of_arrays);
    return 0;
}
