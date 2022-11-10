#include <iostream>
#include <stack>
#include <string>
#include <tuple>

std::tuple<int, bool> CheckValidityOfBracketSubsequence(std::string line_of_brackets) {
    std::stack<char> elements_of_bracket_subsequence;
    int number_good_brackets = 0;
    if (line_of_brackets.empty()) {
        bool elements_of_bracket_subsequence_is_good = true;
        return {number_good_brackets, elements_of_bracket_subsequence_is_good};
    }
    for (int number_bracket = 0; number_bracket < int(line_of_brackets.size()); ++number_bracket) {
        bool one_of_the_opening_bracket = (line_of_brackets[number_bracket] == '(') ||
                                          (line_of_brackets[number_bracket] == '[') ||
                                          (line_of_brackets[number_bracket]) == '{';

        if (one_of_the_opening_bracket) {
            elements_of_bracket_subsequence.push(line_of_brackets[number_bracket]);
        } else {
            bool closing_bracket_with_empty_stack = elements_of_bracket_subsequence.empty();
            if (closing_bracket_with_empty_stack) {
                return {number_good_brackets, false};
            }
            bool closing_bracket_that_close_each_type_bracket =
                (line_of_brackets[number_bracket] == ')' &&
                 elements_of_bracket_subsequence.top() == '(') ||
                (line_of_brackets[number_bracket] == ']' &&
                 elements_of_bracket_subsequence.top() == '[') ||
                (line_of_brackets[number_bracket] == '}' &&
                 elements_of_bracket_subsequence.top() == '{');

            if (closing_bracket_that_close_each_type_bracket) {
                number_good_brackets += 2;
                elements_of_bracket_subsequence.pop();
            } else {
                number_good_brackets += elements_of_bracket_subsequence.size();
                bool elements_of_bracket_subsequence_is_good = false;
                return {number_good_brackets, elements_of_bracket_subsequence_is_good};
            }
        }
    }
    number_good_brackets += elements_of_bracket_subsequence.size();
    bool elements_of_bracket_subsequence_is_good = false;
    if (elements_of_bracket_subsequence.empty()) {
        elements_of_bracket_subsequence_is_good = true;
        return {number_good_brackets, elements_of_bracket_subsequence_is_good};
    }
    return {number_good_brackets, elements_of_bracket_subsequence_is_good};
}

int WriteInformationOfLineOfBracket() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::string line_of_bracket;
    std::cin >> line_of_bracket;
    std::tuple information_of_line_of_bracket = CheckValidityOfBracketSubsequence(line_of_bracket);
    if (std::get<1>(information_of_line_of_bracket)) {
        std::cout << "CORRECT";
    } else {
        std::cout << std::get<0>(information_of_line_of_bracket);
    }
    return 0;
}
