#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>

using std::vector, std::pair, std::max;
using Lookups = pair<vector<size_t>, vector<size_t>>;

pair<size_t, Lookups> FindLongestSequence(const vector<int>& array) {

    vector<size_t> massive_of_size_seq_for_starting_from_each_position_we_started_with_big_element(
        array.size(), 1);
    vector<size_t>
        massive_of_size_seq_for_starting_from_each_position_we_started_with_small_element(
            array.size(), 1);
    if (array.size() == 1) {
        return {1,
                {massive_of_size_seq_for_starting_from_each_position_we_started_with_small_element,
                 massive_of_size_seq_for_starting_from_each_position_we_started_with_big_element}};
    }

    size_t longest_seq_size = 1;

    for (size_t bounding_pointer = array.size() - 2; bounding_pointer + 1 > 0; --bounding_pointer) {
        for (size_t running_pointer_between_end_and_bounding = array.size() - 1;
             running_pointer_between_end_and_bounding > bounding_pointer;
             --running_pointer_between_end_and_bounding) {
            if (array[bounding_pointer] > array[running_pointer_between_end_and_bounding]) {
                massive_of_size_seq_for_starting_from_each_position_we_started_with_big_element
                    [bounding_pointer] = max(
                        massive_of_size_seq_for_starting_from_each_position_we_started_with_big_element
                            [bounding_pointer],
                        massive_of_size_seq_for_starting_from_each_position_we_started_with_small_element
                                [running_pointer_between_end_and_bounding] +
                            1);
            } else if (array[bounding_pointer] < array[running_pointer_between_end_and_bounding]) {
                massive_of_size_seq_for_starting_from_each_position_we_started_with_small_element
                    [bounding_pointer] = max(
                        massive_of_size_seq_for_starting_from_each_position_we_started_with_small_element
                            [bounding_pointer],
                        massive_of_size_seq_for_starting_from_each_position_we_started_with_big_element
                                [running_pointer_between_end_and_bounding] +
                            1);
            }
        }
        size_t new_max_in_bounding_pointer =
            max(massive_of_size_seq_for_starting_from_each_position_we_started_with_big_element
                    [bounding_pointer],
                massive_of_size_seq_for_starting_from_each_position_we_started_with_small_element
                    [bounding_pointer]);
        longest_seq_size = max(longest_seq_size, new_max_in_bounding_pointer);
    }

    return {longest_seq_size,
            {massive_of_size_seq_for_starting_from_each_position_we_started_with_small_element,
             massive_of_size_seq_for_starting_from_each_position_we_started_with_big_element}};
}

void OutputLongestSubsequence(std::ostream& os, const vector<int>& initial_sequence,
                              const size_t& longest_alternating_sequence, const Lookups& lookups) {
    const auto& [massive_of_size_seq_for_starting_from_each_position_we_started_with_small_element,
                 massive_of_size_seq_for_starting_from_each_position_we_started_with_big_element] =
        lookups;
    size_t number_element_in_initial_sequence = 0;
    bool start_element_is_smaller_than_next = false;
    while (number_element_in_initial_sequence + 1 <= initial_sequence.size()) {
        if (longest_alternating_sequence ==
            massive_of_size_seq_for_starting_from_each_position_we_started_with_small_element
                [number_element_in_initial_sequence]) {
            start_element_is_smaller_than_next = true;
            break;
        } else if (longest_alternating_sequence ==
                   massive_of_size_seq_for_starting_from_each_position_we_started_with_big_element
                       [number_element_in_initial_sequence]) {
            start_element_is_smaller_than_next = false;
            break;
        }
        ++number_element_in_initial_sequence;
    }

    size_t number_items_in_output = 0;
    int last_element_that_was_read_to_output = initial_sequence[number_element_in_initial_sequence];
    os << initial_sequence[number_element_in_initial_sequence] << ' ';
    start_element_is_smaller_than_next = !start_element_is_smaller_than_next;
    ++number_items_in_output;
    ++number_element_in_initial_sequence;

    while (number_items_in_output < longest_alternating_sequence) {
        int number_of_raw_items_in_initial_sequence =
            (initial_sequence.size() - number_element_in_initial_sequence);
        int number_of_elements_required_for_output =
            longest_alternating_sequence - number_items_in_output;
        if (number_of_raw_items_in_initial_sequence > number_of_elements_required_for_output) {
            if (start_element_is_smaller_than_next) {
                if (massive_of_size_seq_for_starting_from_each_position_we_started_with_small_element
                            [number_element_in_initial_sequence] !=
                        size_t(number_of_elements_required_for_output) ||
                    initial_sequence[number_element_in_initial_sequence] >=
                        last_element_that_was_read_to_output) {
                    ++number_element_in_initial_sequence;
                    continue;
                }

            } else {
                if (massive_of_size_seq_for_starting_from_each_position_we_started_with_big_element
                            [number_element_in_initial_sequence] !=
                        size_t(number_of_elements_required_for_output) ||
                    initial_sequence[number_element_in_initial_sequence] <=
                        last_element_that_was_read_to_output) {
                    ++number_element_in_initial_sequence;
                    continue;
                }
            }
        }
        last_element_that_was_read_to_output = initial_sequence[number_element_in_initial_sequence];
        os << last_element_that_was_read_to_output << ' ';
        start_element_is_smaller_than_next = !start_element_is_smaller_than_next;
        ++number_items_in_output;
        ++number_element_in_initial_sequence;
    }
    os << '\n';
}

vector<int> ReadSequence(std::istream& is) {
    int n_elements;
    is >> n_elements;
    vector<int> sequence(n_elements);
    for (auto& elem : sequence) {
        is >> elem;
    }
    return sequence;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    vector<int> sequence = ReadSequence(std::cin);
    auto [longest_seq_size, lookups] = FindLongestSequence(sequence);
    OutputLongestSubsequence(std::cout, sequence, longest_seq_size, lookups);

    return 0;
}
