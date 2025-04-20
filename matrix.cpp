#include <iostream>
#include <unordered_map>
#include <map>
#include <utility>
#include <iterator>

template <typename T>
class InfiniteMatrix {
public:
    using key_type = std::pair<int, int>;

private:
    std::map<key_type, T> data_;
    T m_defaultvalue;

public:
    /// \brief class InfiniteMatrix 
    InfiniteMatrix(T defaultvalue) : m_defaultvalue(defaultvalue) {}

    /// \brief Accessing an element by its coordinates
    T getValue(int x, int y) const {
        auto it = data_.find({x, y});
        if (it != data_.end()) {
            return it->second;
        } else {
            return m_defaultvalue;
        }
    }

    /// \brief Setting a value
    void setValue(int x, int y, T value) {
        if (value != m_defaultvalue) {
            data_[{x, y}] = value;
        } else {
            data_.erase({x, y});
        }
    }

    /// \brief Method to count occupied cells
    size_t countOccupiedCells() const {
        return data_.size();
    }

    /// \brief Method to print all occupied cells
    void printOccupiedCells() const {
        for (const auto& entry : data_) {
            std::cout << "matrix["
                      << entry.first.first << "]["
                      << entry.first.second << "] = "
                      << entry.second << std::endl;
        }
    }

    /// \brief Method to print a matrix as Table
    void printAsTable(int Xmin, int Xmax, int Ymin, int Ymax) const {
        for (int y = Ymin; y <= Ymax; ++y) {
            for (int x = Xmin; x <= Xmax; ++x) {
                std::cout << getValue(x, y) << ' ';
            }
            std::cout << std::endl;
        }
    }

    /// \brief class Iterator for the InfiniteMatrix
    class Iterator {
        using map_iterator = typename std::map<key_type, T>::iterator;

    private:
        map_iterator it_;

    public:
        Iterator(map_iterator it) : it_(it) {}

        /// \brief Dereference operator
        std::tuple<int, int, T> operator*() const {
            return {it_->first.first, it_->first.second, it_->second};
        }

        /// \brief  Pre-increment operator
        Iterator& operator++() {
            ++it_;
            return *this;
        }

        /// \brief  Post-increment operator
        Iterator operator++(int) {
            Iterator temp = *this;
            ++it_;
            return temp;
        }

        /// \brief  Equality operator
        bool operator==(const Iterator& other) const {
            return it_ == other.it_;
        }

        /// \brief  Inequality operator
        bool operator!=(const Iterator& other) const {
            return it_ != other.it_;
        }
    };

    /// \brief  Begin iterator
    Iterator begin() {
        return Iterator(data_.begin());
    }

    /// \brief  End iterator
    Iterator end() {
        return Iterator(data_.end());
    }

    /// \brief class ElementProxy
    class ElementProxy {
    private:
        InfiniteMatrix &matrix_;
        int x_, y_;

    public:
        ElementProxy(InfiniteMatrix &matrix, int x, int y) : matrix_(matrix), x_(x), y_(y) {}

        /// \brief  Assignment operator
        ElementProxy& operator=(T new_value) {
            matrix_.setValue(x_, y_, new_value);
            return *this;
        }

        /// \brief  Conversion to T for reading
        operator T() const {
            return matrix_.getValue(x_, y_);
        }
    };
    /// \brief class RowProxy - proxy for [][] enable
    class RowProxy {
    private:
        InfiniteMatrix &matrix_;
        int x_;

    public:
        RowProxy(InfiniteMatrix &matrix, int x) : matrix_(matrix), x_(x) {}

        /// \brief  Operator [] for accessing specific columns
        ElementProxy operator[](int y) {
            return ElementProxy(matrix_, x_, y);
        }
    };

    /// \brief  Access to a row by index
    RowProxy operator[](int x) {
        return RowProxy(*this, x);
    }
};

int main() {
    // 1. При запуске программы необходимо создать матрицу с пустым значением 0
    InfiniteMatrix matrix(0);

    // 1. Заполнить главную диагональ матрицы (от [0,0] до [9,9]) значениями от 0 до 9.
    // 2. Второстепенную диагональ (от [0,9] до [9,0]) значениями от 9 до 0.
    int size = 10;
    for (int i = 0; i < size; ++i) {
        matrix[i][i] = i;
        matrix[i][(size - 1) - i] = (size - 1) - i;
    }

    // 3. Необходимо вывести фрагмент матрицы от [1,1] до [8,8].
    // Между столбцами пробел. Каждая строка матрицы на новой строке консоли.
    matrix.printAsTable(1,8,1,8);

    // 4. Вывести количество занятых ячеек.
    std::cout << "Number of occupied cells: " << matrix.countOccupiedCells() << std::endl;

    // 5. Вывести все занятые ячейки вместе со своими позициями.
    // option 1 - get from map
    std::cout << "option 1 - get from map" << std::endl;
    matrix.printOccupiedCells(); 
    // option 2 - iteration over the matrix
    std::cout << "option 2 - iteration over the matrix" << std::endl;
    for (auto c : matrix) {
        int x, y, data;
        std::tie(x, y, data) = c;
        std::cout << "matrix["
                  << x << "]["
                  << y << "] = "
                  << data << std::endl;
    }

    // Опционально реализовать каноническую форму оператора =, допускающую выражения
    ((matrix[20][20] = 314) = 0) = 217;
    matrix.printAsTable(0,20,0,20);
    std::cout << "Number of occupied cells: " << matrix.countOccupiedCells() << std::endl;

    return 0;
}
