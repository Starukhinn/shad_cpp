#include <iostream>
#include <stack>
#include <utility>
#include <vector>
#include <algorithm>

using std::cin;
using std::cout;
using std::pair;
using std::stack;
using std::vector;

vector<int> ReadNumbers(const int& array_size) {
    vector<int> numbers(array_size);
    for (int& element_of_numbers : numbers) {
        cin >> element_of_numbers;
    }
    return numbers;
}

int FindMaxElementBetweenLeftAndRightPointer(
    const vector<int>& numbers, int& position_of_right_pointer, int& position_of_left_pointer,
    stack<pair<int, int>>& head_stack_of_numbers_between_pointers_with_maximum_element,
    stack<pair<int, int>>& tail_stack_of_numbers_between_pointers_with_maximum_element,
    char& name_of_moving_pointer) {
    if (name_of_moving_pointer == 'L') {
        ++position_of_left_pointer;
        if (head_stack_of_numbers_between_pointers_with_maximum_element.empty()) {
            while (!tail_stack_of_numbers_between_pointers_with_maximum_element.empty()) {
                int element_from_top_of_tail_stack =
                    tail_stack_of_numbers_between_pointers_with_maximum_element.top().first;
                tail_stack_of_numbers_between_pointers_with_maximum_element.pop();
                int maximum_element_in_head;
                if (head_stack_of_numbers_between_pointers_with_maximum_element.empty()) {
                    maximum_element_in_head = element_from_top_of_tail_stack;
                } else {
                    maximum_element_in_head = std::max(
                        element_from_top_of_tail_stack,
                        head_stack_of_numbers_between_pointers_with_maximum_element.top().second);
                }
                head_stack_of_numbers_between_pointers_with_maximum_element.push(
                    std::make_pair(element_from_top_of_tail_stack, maximum_element_in_head));
            }
        }
        head_stack_of_numbers_between_pointers_with_maximum_element.pop();
    } else if (name_of_moving_pointer == 'R') {
        ++position_of_right_pointer;
        int maximum_element_in_tail;
        if (tail_stack_of_numbers_between_pointers_with_maximum_element.empty()) {
            maximum_element_in_tail = numbers.at(position_of_right_pointer);
        } else {
            maximum_element_in_tail =
                std::max(numbers.at(position_of_right_pointer),
                         tail_stack_of_numbers_between_pointers_with_maximum_element.top().second);
        }
        tail_stack_of_numbers_between_pointers_with_maximum_element.push(
            std::make_pair(numbers.at(position_of_right_pointer), maximum_element_in_tail));
    }

    if (head_stack_of_numbers_between_pointers_with_maximum_element.empty() ||
        tail_stack_of_numbers_between_pointers_with_maximum_element.empty()) {
        int maximum_element_between_pointers;
        if (tail_stack_of_numbers_between_pointers_with_maximum_element.empty()) {
            maximum_element_between_pointers =
                head_stack_of_numbers_between_pointers_with_maximum_element.top().second;
        } else {
            maximum_element_between_pointers =
                tail_stack_of_numbers_between_pointers_with_maximum_element.top().second;
        }
        return maximum_element_between_pointers;
    }
    int maximum_element_between_pointers =
        std::max(tail_stack_of_numbers_between_pointers_with_maximum_element.top().second,
                 head_stack_of_numbers_between_pointers_with_maximum_element.top().second);
    return maximum_element_between_pointers;
}

void RequestProcessing(const vector<int>& numbers, int& number_movements_of_pointer) {
    int position_of_right_pointer = 0;
    int position_of_left_pointer = 0;
    stack<pair<int, int>> head_stack_of_numbers_between_pointers_with_maximum_element;
    stack<pair<int, int>> tail_stack_of_numbers_between_pointers_with_maximum_element;
    tail_stack_of_numbers_between_pointers_with_maximum_element.push(
        std::make_pair(numbers.at(0), numbers.at(0)));
    while (number_movements_of_pointer > 0) {
        char name_of_moving_pointer;
        cin >> name_of_moving_pointer;
        int maximum_element_between_pointers = FindMaxElementBetweenLeftAndRightPointer(
            numbers, position_of_right_pointer, position_of_left_pointer,
            head_stack_of_numbers_between_pointers_with_maximum_element,
            tail_stack_of_numbers_between_pointers_with_maximum_element, name_of_moving_pointer);
        cout << maximum_element_between_pointers << " ";
        --number_movements_of_pointer;
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int array_size;
    cin >> array_size;
    vector<int> numbers = ReadNumbers(array_size);
    int number_movements_of_pointer;
    cin >> number_movements_of_pointer;
    RequestProcessing(numbers, number_movements_of_pointer);
    return 0;

}
