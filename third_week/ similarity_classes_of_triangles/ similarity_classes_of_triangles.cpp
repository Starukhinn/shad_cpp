#include <algorithm>
#include <iostream>
#include <vector>
#include <list>

using std::cin;
using std::cout;
using std::swap;
using std::vector;

struct Triangle {
    double a_side;
    double b_side;
    double c_side;
};

struct SimilarityClass {
    Triangle class_representative;
};

Triangle InputTriangle() {
    double side_a;
    double side_b;
    double side_c;
    cin >> side_a >> side_b >> side_c;
    if (side_a > side_c) {
        swap(side_c, side_a);
    }
    if (side_a > side_b) {
        swap(side_b, side_a);
    }
    if (side_b > side_c) {
        swap(side_b, side_c);
    }
    return {side_a, side_b, side_c};
}

bool CheckSimilarityTriangles(const Triangle& first_triangle, const Triangle& second_triangle) {
    if ((first_triangle.a_side / second_triangle.a_side ==
         first_triangle.b_side / second_triangle.b_side) and
        (first_triangle.c_side / second_triangle.c_side ==
         first_triangle.c_side / second_triangle.c_side)) {
        return true;
    }
    return false;
}

bool CheckingExistenceOfSuitableClassForTriangle(vector<SimilarityClass>& classes_of_similarity,
                                                 const Triangle& checking_triangle) {
    if (classes_of_similarity.empty()) {
        return false;
    }
    for (const auto& class_of_similarity : classes_of_similarity) {
        if (CheckSimilarityTriangles(class_of_similarity.class_representative, checking_triangle)) {
            return true;
        }
    }
    return false;
}

void AddClassOfSimilarity(vector<SimilarityClass>& classes_of_similarity,
                          const Triangle& potential_representative_of_new_class) {

    if (classes_of_similarity.empty()) {
        classes_of_similarity.push_back({potential_representative_of_new_class});
    }

    else if (CheckingExistenceOfSuitableClassForTriangle(classes_of_similarity,
                                                    potential_representative_of_new_class)) {
        return;
    }

    else {
        classes_of_similarity.push_back(
            {potential_representative_of_new_class});
    }
}

int CountNumberClasses(vector<SimilarityClass>& classes_of_similarity, int& number_triangles) {
    while (number_triangles > 0) {
        Triangle one_of_the_input_triangles = InputTriangle();
        AddClassOfSimilarity(classes_of_similarity, one_of_the_input_triangles);
        --number_triangles;
    }
    return classes_of_similarity.size();
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int number_triangles;
    cin >> number_triangles;
    vector<SimilarityClass> classes_of_similarity;
    cout << CountNumberClasses(classes_of_similarity, number_triangles);
    return 0;
}