#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

int number_a;
int number_b;
unsigned int cur = 0;  // беззнаковое 32-битное число

unsigned int NextRand24() {
    cur = cur * number_a + number_b;  // вычисляется с переполнениями
    return cur >> 8;                  // число от 0 до 2**24-1.
}

unsigned int NextRand32() {
    unsigned int a_number = NextRand24(), b_number = NextRand24();
    return (a_number << 8) ^ b_number;  // число от 0 до 2**32-1.
}

void FillArrayOfPositionsHomes(vector<unsigned int>& positions_of_homes) {
    for (auto& position_of_home : positions_of_homes) {
        position_of_home = NextRand32();
    }
}

template <class ElemType, class RandomGenerator>
int64_t FindPivot(vector<ElemType>& massive_for_finding_median,
                  RandomGenerator& generation_number) {
    if (massive_for_finding_median.size() == 1) {
        return massive_for_finding_median.at(0);
    }
    std::uniform_int_distribution<size_t> distrib(0, massive_for_finding_median.size() - 1);
    return massive_for_finding_median.at(distrib(generation_number));
}

template <class ElemType, class RandomGenerator>
ElemType QuickSelect(vector<ElemType>& massive_for_finding_median, size_t position_of_median,
                     RandomGenerator& generation_number) {
    if (massive_for_finding_median.size() == 1) {
        return massive_for_finding_median.at(0);
    }
    unsigned int pivot = FindPivot(massive_for_finding_median, generation_number);
    vector<ElemType> elements_more_pivot;
    vector<ElemType> elements_less_pivot;
    vector<ElemType> elements_equal_pivot;
    for (const auto& element_of_massive : massive_for_finding_median) {
        if (element_of_massive == pivot) {
            elements_equal_pivot.push_back(element_of_massive);
        } else if (element_of_massive < pivot) {
            elements_less_pivot.push_back(element_of_massive);
        } else if (element_of_massive > pivot) {
            elements_more_pivot.push_back(element_of_massive);
        }
    }
    if (position_of_median < static_cast<ElemType>(elements_less_pivot.size())) {
        return QuickSelect(elements_less_pivot, position_of_median, generation_number);
    } else if (position_of_median <
               static_cast<ElemType>(elements_less_pivot.size() + elements_equal_pivot.size())) {
        return elements_equal_pivot.at(0);
    }
    return QuickSelect(
        elements_more_pivot,
        position_of_median - elements_less_pivot.size() - elements_equal_pivot.size(),
        generation_number);
}

template <class ElemType>
ElemType FindMedian(vector<ElemType>& houses_positions) {
    std::random_device rd;
    std::mt19937 generator(rd());
    unsigned int position_home_of_postman;
    if (houses_positions.size() % 2 != 0) {
        position_home_of_postman =
            QuickSelect(houses_positions, houses_positions.size() / 2, generator);
    } else {
        position_home_of_postman =
            QuickSelect(houses_positions, houses_positions.size() / 2 - 1, generator);
    }
    return position_home_of_postman;
}

int64_t FindSumDistanceFromPostmanToOtherPeople(const vector<unsigned int>& positions_of_homes,
                                                const unsigned int& position_home_of_postman) {
    int64_t sum_distance = 0;
    for (const auto& house_position : positions_of_homes) {
        if (house_position > position_home_of_postman) {
            sum_distance +=
                std::abs(static_cast<int64_t>(house_position - position_home_of_postman));
        } else {
            sum_distance +=
                std::abs(static_cast<int64_t>(position_home_of_postman - house_position));
        }
    }
    return sum_distance;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    size_t number_residents;
    cin >> number_residents;
    cin >> number_a >> number_b;
    vector<unsigned int> positions_of_homes(number_residents);
    FillArrayOfPositionsHomes(positions_of_homes);
    unsigned int position_home_of_postman = FindMedian(positions_of_homes);
    int64_t sum_distance_for_postman =
        FindSumDistanceFromPostmanToOtherPeople(positions_of_homes, position_home_of_postman);
    cout << sum_distance_for_postman;
    return 0;
}
