#include <optional>
#include <random>
#include <vector>

using std::optional;
using std::vector;

int GenerateNumbers(const int& prime_number) {
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<size_t> distrib(0, prime_number - 1);
    return distrib(generator);
}

struct HashFunctionParameters {
    int a_parameter;
    int b_parameter;
};

HashFunctionParameters GenerateHashFunctionParameters(const int& prime_number) {
    int a_parameter = GenerateNumbers(prime_number);
    while (a_parameter == 0) {
        a_parameter = GenerateNumbers(prime_number);
    }
    int b_parameter = GenerateNumbers(prime_number);
    return {a_parameter, b_parameter};
}

class HashFunction {
public:
    HashFunction() {
        a_parameter_ = {};
        b_parameter_ = {};
    }
    HashFunction(HashFunctionParameters& parameters) {
        a_parameter_ = parameters.a_parameter;
        b_parameter_ = parameters.b_parameter;
    }

    uint64_t CountHash(const int& key) const {
        return (a_parameter_ * (key + 2000000011) + b_parameter_);
    }
    void operator()(HashFunctionParameters& parameters) {
        HashFunction hash_function(parameters);
        a_parameter_ = hash_function.a_parameter_;
        b_parameter_ = hash_function.b_parameter_;
    }

    void operator=(const HashFunction& hash_function) {
        a_parameter_ = hash_function.a_parameter_;
        b_parameter_ = hash_function.b_parameter_;
    }

private:
    int a_parameter_;
    int b_parameter_;
};

class LinearHashTable {
public:
    LinearHashTable() {
        bucket_.resize(0);
    }

    LinearHashTable(const int& number_elements_in_bucket) {
        size_of_hash_table_ = number_elements_in_bucket * number_elements_in_bucket;
        number_elements_in_bucket_ = number_elements_in_bucket;
        bucket_.resize(size_of_hash_table_);
    }

    void Initialize(const int& number) {
        FillLinearHashTable(number);
    }

    bool Contains(const int& number) const {
        return CheckNumber(number);
    }

    void operator=(const LinearHashTable& linear_hash_table) {
        size_of_hash_table_ = linear_hash_table.size_of_hash_table_;
        number_elements_in_bucket_ = linear_hash_table.number_elements_in_bucket_;
        bucket_.resize(size_of_hash_table_);
    }

private:
    vector<optional<int>> bucket_;
    vector<int> array_numbers_;
    int size_of_hash_table_;
    size_t number_elements_in_bucket_;
    HashFunction hash_function_;

    void AddElementToArrayNumbers(const int& number) {
        array_numbers_.push_back(number);
    }

    void AddElementsFromArrayToBucket() {
        int prime = 2000000011;
        HashFunctionParameters hash_function_parameters = GenerateHashFunctionParameters(prime);
        hash_function_(hash_function_parameters);
        size_t number_element = 0;
        do {
            int element = array_numbers_.at(number_element);
            uint64_t element_hash = hash_function_.CountHash(element) % prime % size_of_hash_table_;
            if (bucket_.at(element_hash).has_value()) {
                number_element = 0;
                bucket_.clear();
                bucket_.resize(size_of_hash_table_);
                HashFunctionParameters parameters = GenerateHashFunctionParameters(prime);
                hash_function_(parameters);
            } else {
                bucket_.at(element_hash) = element;
                ++number_element;
            }
        } while (number_element < number_elements_in_bucket_);
    }

    void FillLinearHashTable(const int& number) {
        AddElementToArrayNumbers(number);
        if (array_numbers_.size() == number_elements_in_bucket_) {
            AddElementsFromArrayToBucket();
        }
    }
    bool CheckNumber(const int& element_for_checking) const {
        if (bucket_.empty()) {
            return false;
        }
        int prime = 2000000011;
        uint64_t hash_of_element =
            hash_function_.CountHash(element_for_checking) % prime % size_of_hash_table_;
        return bucket_.at(hash_of_element) == element_for_checking;
    }
};

class FixedSet {
public:
    FixedSet(){};

    void Initialize(const vector<int>& numbers) {
        FillHashTable(numbers);
    }
    bool Contains(int number) const {
        return CheckNumber(number);
    }

private:
    vector<LinearHashTable> hash_table_;
    HashFunction hash_function_;
    size_t size_of_hash_table_;

    void ClearHashTable() {
        hash_table_.clear();
    }

    vector<int> CountSizeBuckets(const vector<int>& numbers) {
        vector<int> number_collisions_of_each_hash(size_of_hash_table_, 0);
        for (const int& number : numbers) {
            int prime = 2000000011;
            uint64_t hash = hash_function_.CountHash(number) % prime % size_of_hash_table_;
            number_collisions_of_each_hash.at(hash) += 1;
        }
        return number_collisions_of_each_hash;
    }

    size_t CountTotalSquareSizeBuckets(const vector<int>& number_collisions_of_each_hash) {
        size_t total_square_size = 0;
        for (const auto& number_collisions : number_collisions_of_each_hash) {
            total_square_size += number_collisions * number_collisions;
        }
        return total_square_size;
    }
    bool CheckValidityHashFunction(const size_t& total_square_size_of_buckets) {
        return total_square_size_of_buckets > 4 * size_of_hash_table_;
    }

    void GenerateHashFunction(const vector<int>& numbers) {
        int prime = 2000000011;
        HashFunctionParameters hash_function_parameters = GenerateHashFunctionParameters(prime);
        hash_function_(hash_function_parameters);
        vector<int> number_collisions_of_each_hash = CountSizeBuckets(numbers);
        int total_square_size = CountTotalSquareSizeBuckets(number_collisions_of_each_hash);
        bool hash_function_is_not_validity = CheckValidityHashFunction(total_square_size);
        while (hash_function_is_not_validity) {
            hash_function_parameters = GenerateHashFunctionParameters(prime);
            hash_function_(hash_function_parameters);
            number_collisions_of_each_hash = CountSizeBuckets(numbers);
            total_square_size = CountTotalSquareSizeBuckets(number_collisions_of_each_hash);
            hash_function_is_not_validity = CheckValidityHashFunction(total_square_size);
        }
    }

    void FillHashTable(const vector<int>& numbers) {
        ClearHashTable();
        size_of_hash_table_ = numbers.size();
        GenerateHashFunction(numbers);
        vector<int> number_elements_in_each_bucket = CountSizeBuckets(numbers);
        hash_table_.resize(size_of_hash_table_, LinearHashTable(0));
        for (size_t bucket_number = 0; bucket_number < size_of_hash_table_; ++bucket_number) {
            int number_elements_in_bucket = number_elements_in_each_bucket.at(bucket_number);
            hash_table_.at(bucket_number) = LinearHashTable(number_elements_in_bucket);
        }
        for (const auto& number : numbers) {
            int prime = 2000000011;
            uint64_t number_hash = hash_function_.CountHash(number) % prime % size_of_hash_table_;
            hash_table_.at(number_hash).Initialize(number);
        }
    }

    bool CheckNumber(const int& number) const {
        if (hash_table_.empty()) {
            return false;
        }
        int prime = 2000000011;
        uint64_t hash_of_element = hash_function_.CountHash(number) % prime % size_of_hash_table_;
        return hash_table_.at(hash_of_element).Contains(number);
    }
};
