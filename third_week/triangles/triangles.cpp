#include <algorithm>
#include <iostream>
#include <map>
#include <optional>
#include <random>
#include <tuple>
#include <vector>

using std::cin;
using std::cout;
using std::gcd;
using std::get;
using std::make_tuple;
using std::map;
using std::optional;
using std::swap;
using std::tuple;
using std::vector;

class Triangle {
public:
    Triangle() {
        a_side_ = 0;
        b_side_ = 0;
        c_side_ = 0;
    }
    Triangle(double& a_side, double& b_side, double& c_side) {
        a_side_ = a_side;
        b_side_ = b_side;
        c_side_ = c_side;
    }

    void operator()(int& a_side, int& b_side, int& c_side) {
        a_side_ = a_side;
        b_side_ = b_side;
        c_side_ = c_side;
    }

    int ReturnA() const {
        return a_side_;
    }

    int ReturnB() const {
        return b_side_;
    }

    int ReturnC() const {
        return c_side_;
    }

private:
    int a_side_;
    int b_side_;
    int c_side_;
};

std::istream& operator>>(std::istream& input, Triangle& triangle) {
    int a_side;
    int b_side;
    int c_side;
    input >> a_side >> b_side >> c_side;
    if (a_side > c_side) {
        swap(c_side, a_side);
    }
    if (a_side > b_side) {
        swap(b_side, a_side);
    }
    if (b_side > c_side) {
        swap(b_side, c_side);
    }
    int greatest_common_divisor = gcd(gcd(a_side, b_side), c_side);
    a_side /= greatest_common_divisor;
    b_side /= greatest_common_divisor;
    c_side /= greatest_common_divisor;
    triangle(a_side, b_side, c_side);
    return input;
}

bool operator==(const Triangle& first_triangle, const Triangle& second_triangle) {
    if (first_triangle.ReturnA() != second_triangle.ReturnA()) {
        return false;
    }
    if (first_triangle.ReturnB() != second_triangle.ReturnB()) {
        return false;
    }
    if (first_triangle.ReturnC() != second_triangle.ReturnC()) {
        return false;
    }
    return true;
}

bool operator!=(const Triangle& first_triangle, const Triangle& second_triangle) {
    if (first_triangle.ReturnA() != second_triangle.ReturnA()) {
        return true;
    }
    if (first_triangle.ReturnB() != second_triangle.ReturnB()) {
        return true;
    }
    if (first_triangle.ReturnC() != second_triangle.ReturnC()) {
        return true;
    }
    return false;
}

int GenerateNumbers(const int& prime_number) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<size_t> distrib(0, prime_number - 1);
    return distrib(generator);
}

struct HashFunctionParameters {
    int a_parameter;
    int b_parameter;
    int c_parameter;
    int d_parameter;
};

HashFunctionParameters GenerateHashFunctionParameters(const int& prime_number) {
    int a_parameter;
    do {
        a_parameter = GenerateNumbers(prime_number);
    } while (a_parameter == 0);
    int b_parameter;
    do {
        b_parameter = GenerateNumbers(prime_number);
    } while (b_parameter == 0);
    int c_parameter;
    do {
        c_parameter = GenerateNumbers(prime_number);
    } while (c_parameter == 0);
    int d_parameter = GenerateNumbers(prime_number);
    return {a_parameter, b_parameter, c_parameter, d_parameter};
}

class HashFunction {
public:
    HashFunction() {
        a_parameter_ = {};
        b_parameter_ = {};
        c_parameter_ = {};
        d_parameter_ = {};
    }

    uint64_t CountHash(const int& a_side, const int& b_side, const int& c_side) const {
        return (a_parameter_ * static_cast<uint64_t>(a_side) +
                b_parameter_ * static_cast<uint64_t>(b_side) +
                c_parameter_ * static_cast<uint64_t>(c_side) + d_parameter_);
    }
    void operator()(HashFunctionParameters& parameters) {
        a_parameter_ = parameters.a_parameter;
        b_parameter_ = parameters.b_parameter;
        c_parameter_ = parameters.c_parameter;
        d_parameter_ = parameters.d_parameter;
    }

private:
    int a_parameter_;
    int b_parameter_;
    int c_parameter_;
    int d_parameter_;
};

class HashTable {
public:
    void Initialize(const int& number_triangles) {
        FillHashTable(number_triangles);
    }
    int CountClasses() {
        return number_classes_;
    }

private:
    map<int, vector<tuple<int, Triangle>>> hash_table_;
    HashFunction hash_function_;
    int prime_ = 1000000011;
    int number_classes_;

    void FillHashTable(const int& number_triangles) {
        number_classes_ = 0;
        HashFunctionParameters hash_function_parameters = GenerateHashFunctionParameters(prime_);
        hash_function_(hash_function_parameters);
        int number_triangle = 0;
        while (number_triangle < number_triangles) {
            Triangle triangle;
            cin >> triangle;
            uint64_t element_hash = hash_function_.CountHash(triangle.ReturnA(), triangle.ReturnB(),
                                                             triangle.ReturnC());

            if (hash_table_[element_hash].empty()) {
                hash_table_.at(element_hash).push_back(make_tuple(1, triangle));
                ++number_classes_;
            } else {
                int number_class = 0;
                Triangle triangle_from_class_similarity;
                do {
                    triangle_from_class_similarity =
                        get<1>(hash_table_.at(element_hash).at(number_class));
                    ++number_class;
                } while (triangle_from_class_similarity != triangle and
                         static_cast<size_t>(number_class) < hash_table_.at(element_hash).size());
                if (triangle_from_class_similarity == triangle) {
                    ++get<0>(hash_table_.at(element_hash).at(number_class - 1));
                } else {
                    hash_table_.at(element_hash).push_back(make_tuple(1, triangle));
                    ++number_classes_;
                }
            }
            ++number_triangle;
        }
    }
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int number_triangles;
    cin >> number_triangles;
    HashTable hash_table_of_triangles;
    hash_table_of_triangles.Initialize(number_triangles);
    int number_classes = hash_table_of_triangles.CountClasses();
    cout << number_classes;
    return 0;
}
