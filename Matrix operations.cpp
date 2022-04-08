#include <fstream>
#include <iostream>

class Matrix;

class MatrixStr { //класс строк матриц
public:
    MatrixStr(Matrix* parentPtr, unsigned int const strNum); //конструктор класса
    int& operator[](unsigned int const col); //взятие по строке
    int& operator()(unsigned int const elem);
private:
    Matrix* m_parentPtr; //указатель на родительскую матрицу
    unsigned int m_strNum; // номер строки
};

class MatrixCol { //класс столбцов матриц
public:
    MatrixCol(Matrix* parentPtr, unsigned int const colNum); //конструктор класса
    int& operator[](unsigned int const str); //взятие по столбцу
    int& operator()(unsigned int const num);
private:
    Matrix* m_parentPtr; //указатель на родительскую матрицу
    unsigned int m_colNum; // номер строки
};

class Matrix { //класс матриц
public:
    Matrix(Matrix const& other); //копирование матрицы
    Matrix(); //конструктор 0 матрицы
    Matrix(unsigned int size); // конструктор едининой матрицы
    Matrix(unsigned int const size, int const* const diagValues); //конструктор диагональной с заданным элементом
    Matrix& operator=(Matrix const& other); //поэлементное присваивание
    Matrix operator+(Matrix const& other) const;//сложение
    Matrix operator-(Matrix const& other) const;//вычитание
    Matrix operator*(Matrix const& other) const;//умножение
    bool operator==(Matrix const& other) const;//сравнение на равенство
    bool operator!=(Matrix const& other) const;//сравнение на неравенство
    Matrix operator!() const; // транспонирование
    void print_to_file(std::ofstream& fout) const;//вывод в файл
    void print_to_screen() const;//вывод на экран
    void read_from_file(std::ifstream& fin);//чтение с файла
    void read_from_screen();// чтение с консоли
    Matrix operator()(unsigned int const str, unsigned int const col) const; // создание минора без i-ой строки и  j-го столбца
    int& at(unsigned int const str, unsigned int const col); //взятие элементов с возможностью их изменять
    int const& at(unsigned int const str, unsigned int const col) const; //взятие элементов без возможности изменений
    ~Matrix();//деструктор
    MatrixStr operator[](unsigned int const str);//взятие по строке
    MatrixCol operator()(unsigned int const col);//взятие по столбцу
private:
    void alloc_body(unsigned int const size);//выделение памяти
    int** m_body; //указатель на матрицу
    unsigned int m_size; //размерность матрицы
};

MatrixStr::MatrixStr(Matrix* parentPtr, unsigned int const strNum) { //конструктор класса строк матриц
    m_parentPtr = parentPtr;//указатель на родительскую матрицу
    m_strNum = strNum; // номер строки
}

int& MatrixStr::operator[](unsigned int const col) { //возврат элемента из строки
    return m_parentPtr->at(m_strNum, col);
}

int& MatrixStr::operator()(unsigned int const elem){
    return m_parentPtr->at(m_strNum, elem);
}

MatrixCol::MatrixCol(Matrix* parentPtr, unsigned int const colNum) { //конструкто стобца
    m_parentPtr = parentPtr;
    m_colNum = colNum;
}

int& MatrixCol::operator[](unsigned int const str) { //возврат элемента из столбца
    return m_parentPtr->at(str, m_colNum);
}

int& MatrixCol::operator()(unsigned int const num) { //возврат элемента из столбца
    return m_parentPtr->at(num, m_colNum);
}


Matrix::Matrix(Matrix const& other) { //копирование
    m_size = other.m_size;
    if (m_size == 0) {
        m_body = nullptr; 
    }
    else {
        alloc_body(m_size);
        for (unsigned int i = 0; i < m_size; ++i)
            for (unsigned int j = 0; j < m_size; ++j)
                at(i, j) = other.at(i, j);
    }
}

Matrix::Matrix() { //констурктор 0 матрицы
    m_size = 0;
    m_body = nullptr;
}

Matrix::Matrix(unsigned int const size) { //конструктор единичной мтарицы
    m_size = size;
    if (size == 0) {
        m_body = nullptr;
    }
    else {
        alloc_body(m_size);
        for (unsigned int i = 0; i < m_size; ++i) {
            for (unsigned int j = 0; j < m_size; ++j) {
                if (i == j)
                    at(i, j) = 1;
                else
                    at(i, j) = 0;
            }
        }
    }
}

Matrix::Matrix(unsigned int const size, int const* const diagValues) { //конструктор диагональной
    m_size = size;
    if (size == 0) {
        m_body = nullptr;
    }
    else {
        alloc_body(m_size);
        for (unsigned int i = 0; i < m_size; ++i) {
            for (unsigned int j = 0; j < m_size; ++j) {
                if (i == j)
                    at(i, j) = diagValues[i];
                else
                    at(i, j) = 0;
            }
        }
    }
}

Matrix& Matrix::operator=(Matrix const& other) { //оператор присваивания
    if (m_size != other.m_size) {
        for (unsigned int i = 0; i < m_size; ++i)
            delete[] m_body[i];
        delete[] m_body;
        alloc_body(m_size);
    }
    for (unsigned int i = 0; i < m_size; ++i)
        for (unsigned int j = 0; j < m_size; ++j)
            m_body[i][j] = other.m_body[i][j];
    return *this;
}

Matrix Matrix::operator+(Matrix const& other) const {//сложение
    if (m_size != other.m_size)
        throw std::invalid_argument("Cannot add different size matrix");

    Matrix result(other.m_size);
    for (unsigned int i = 0; i < m_size; ++i)
        for (unsigned int j = 0; j < m_size; ++j)
            result.at(i, j) = at(i, j) + other.at(i, j);
    return result;
}

Matrix Matrix::operator-(Matrix const& other) const {//вычитание
    if (m_size != other.m_size)
        throw std::invalid_argument("Cannot sub different size matrix");

    Matrix result(other.m_size);
    for (unsigned int i = 0; i < m_size; ++i)
        for (unsigned int j = 0; j < m_size; ++j)
            result.at(i, j) = at(i, j) - other.at(i, j);
    return result;
}

Matrix Matrix::operator*(Matrix const& other) const { //умножение
    if (m_size != other.m_size)
        throw std::invalid_argument("Cannot mult different size matrix");

    Matrix result(m_size);
    for (unsigned int i = 0; i < m_size; ++i) {
        for (unsigned int j = 0; j < m_size; ++j) {
            int cellValue = 0;
            for (unsigned int k = 0; k < m_size; ++k)
                cellValue += at(i, k) * other.at(k, j);
            result.at(i, j) = cellValue;
        }
    }
    return result;
}

bool Matrix::operator==(Matrix const& other) const {//равенство
    if (m_size != other.m_size)
        return false;
    for (unsigned int i = 0; i < m_size; ++i)
        for (unsigned int j = 0; j < m_size; ++j)
            if (at(i, j) != other.at(i, j))
                return false;
    return true;
}

bool Matrix::operator!=(Matrix const& other) const {//неравенство
    return !(*this == other);
}

Matrix Matrix::operator!() const { //транспозиция
    Matrix transposed(m_size);
    for (unsigned int i = 0; i < m_size; ++i)
        for (unsigned int j = 0; j < m_size; ++j)
            transposed.at(i, j) = at(j, i);
    return transposed;
}

void Matrix::print_to_file(std::ofstream& fout) const {//вывод в файл
    for (unsigned int i = 0; i < m_size; ++i) {
        for (unsigned int j = 0; j < m_size; ++j)
            fout << at(i, j) << " ";
        fout << std::endl;
    }

}

void Matrix::print_to_screen() const {//вывод на экран
    for (unsigned int i = 0; i < m_size; ++i) {
        for (unsigned int j = 0; j < m_size; ++j)
            std::cout << at(i, j) << " ";
        std::cout << std::endl;
    }
}

void Matrix::read_from_file(std::ifstream& fin) {//чтение из файла
    for (unsigned int i = 0; i < m_size; ++i)
        for (unsigned int j = 0; j < m_size; ++j)
            fin >> at(i, j);
}

void Matrix::read_from_screen() {//чтение с экрана
    for (unsigned int i = 0; i < m_size; ++i)
        for (unsigned int j = 0; j < m_size; ++j)
            std::cin >> at(i, j);
}

Matrix Matrix::operator()(unsigned int const str, unsigned int const col) const { //минор
    if (str >= m_size || col >= m_size)
        throw std::out_of_range("Try to delete unexisting part");

    Matrix minor(m_size - 1);
    for (unsigned int i = 0; i < m_size; ++i) {
        if (i == str)
            continue;
        unsigned int const minorStrNum = i > str ? i - 1 : i;
        for (unsigned int j = 0; j < m_size; ++j) {
            if (j == col)
                continue;
            unsigned int const minorColNum = j > col ? j - 1 : j;
            minor.at(minorStrNum, minorColNum) = at(i, j);
        }
    }
    return minor;
}

int& Matrix::at(unsigned int const str, unsigned int const col) {//взятие с изменением
    if (str >= m_size || col >= m_size)
        throw std::out_of_range("Try to get unexisting part");

    return m_body[str][col];
}

int const& Matrix::at(unsigned int const str, unsigned int const col) const {//без изменения
    if (str >= m_size || col >= m_size)
        throw std::out_of_range("Try to get unexisting part");

    return m_body[str][col];
}

Matrix::~Matrix() {//дестркутор
    if (m_body != nullptr) {
        for (unsigned int i = 0; i < m_size; ++i)
            delete[] m_body[i];
        delete[] m_body;
    }
}

MatrixStr Matrix::operator[](unsigned int const str) {//взятие по строке
    return MatrixStr(this, str);
}

MatrixCol Matrix::operator()(unsigned int const col) {//взятие по столбцу
    return MatrixCol(this, col);
}

void Matrix::alloc_body(unsigned int const size) {//выделение памяти
    m_size = size;
    m_body = new int* [m_size];
    for (unsigned int i = 0; i < size; ++i)
        m_body[i] = new int[m_size];
}

Matrix test_compute(Matrix const& a, Matrix const& b, Matrix const& c, Matrix const& d, Matrix const& k) {
    return (a + b * (!c) + k) * (!d);
}

int main() {
    std::ifstream fin("input.txt");
    unsigned int matrixSize;
    fin >> matrixSize;
    int diagElem;
    fin >> diagElem;
    Matrix a(matrixSize);
    a.read_from_file(fin);
    Matrix b(matrixSize);
    b.read_from_file(fin);
    Matrix c(matrixSize);
    c.read_from_file(fin);
    Matrix d(matrixSize);
    d.read_from_file(fin);
    fin.close();

    int* diagArray = new int[matrixSize];
    for (unsigned int i = 0; i < matrixSize; ++i)
        diagArray[i] = diagElem;
    Matrix k(matrixSize, diagArray);
    delete[] diagArray;

    Matrix result = test_compute(a, b, c, d, k);

    std::ofstream fout("output.txt");
    result.print_to_file(fout);
    fout.close();

    Matrix f(10);

    f(3)[5] = 6;
    std::cout << f[5][3] << std::endl;
    std::cout << f(3)[5] << std::endl;

    if (f[5][3] == f(3)[5]) {
        std::cout << "!" << std::endl;
    }

    f[5][5] = 10;
    if (f(5)[5] == 10) {
        std::cout << "!!" << std::endl;
    }

    f[5](3) = 100;
    std::cout << f[5](3) << std::endl;
    if (f(3)[5] == f[5](3)) {
        std::cout << "!!!" << std::endl;
    }

    if (f(3)(5) == f(3)[5] && f[5][3] == f[5](3) && f(3)(5) == f[5][3] && f(3)[5] == f[5](3)) {
        std::cout << "!!!!" << std::endl;
    }

    return 0;
}