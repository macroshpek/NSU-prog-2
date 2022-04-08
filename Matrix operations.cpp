#include <fstream>
#include <iostream>

class Matrix;

class MatrixStr { //����� ����� ������
public:
    MatrixStr(Matrix* parentPtr, unsigned int const strNum); //����������� ������
    int& operator[](unsigned int const col); //������ �� ������
    int& operator()(unsigned int const elem);
private:
    Matrix* m_parentPtr; //��������� �� ������������ �������
    unsigned int m_strNum; // ����� ������
};

class MatrixCol { //����� �������� ������
public:
    MatrixCol(Matrix* parentPtr, unsigned int const colNum); //����������� ������
    int& operator[](unsigned int const str); //������ �� �������
    int& operator()(unsigned int const num);
private:
    Matrix* m_parentPtr; //��������� �� ������������ �������
    unsigned int m_colNum; // ����� ������
};

class Matrix { //����� ������
public:
    Matrix(Matrix const& other); //����������� �������
    Matrix(); //����������� 0 �������
    Matrix(unsigned int size); // ����������� �������� �������
    Matrix(unsigned int const size, int const* const diagValues); //����������� ������������ � �������� ���������
    Matrix& operator=(Matrix const& other); //������������ ������������
    Matrix operator+(Matrix const& other) const;//��������
    Matrix operator-(Matrix const& other) const;//���������
    Matrix operator*(Matrix const& other) const;//���������
    bool operator==(Matrix const& other) const;//��������� �� ���������
    bool operator!=(Matrix const& other) const;//��������� �� �����������
    Matrix operator!() const; // ����������������
    void print_to_file(std::ofstream& fout) const;//����� � ����
    void print_to_screen() const;//����� �� �����
    void read_from_file(std::ifstream& fin);//������ � �����
    void read_from_screen();// ������ � �������
    Matrix operator()(unsigned int const str, unsigned int const col) const; // �������� ������ ��� i-�� ������ �  j-�� �������
    int& at(unsigned int const str, unsigned int const col); //������ ��������� � ������������ �� ��������
    int const& at(unsigned int const str, unsigned int const col) const; //������ ��������� ��� ����������� ���������
    ~Matrix();//����������
    MatrixStr operator[](unsigned int const str);//������ �� ������
    MatrixCol operator()(unsigned int const col);//������ �� �������
private:
    void alloc_body(unsigned int const size);//��������� ������
    int** m_body; //��������� �� �������
    unsigned int m_size; //����������� �������
};

MatrixStr::MatrixStr(Matrix* parentPtr, unsigned int const strNum) { //����������� ������ ����� ������
    m_parentPtr = parentPtr;//��������� �� ������������ �������
    m_strNum = strNum; // ����� ������
}

int& MatrixStr::operator[](unsigned int const col) { //������� �������� �� ������
    return m_parentPtr->at(m_strNum, col);
}

int& MatrixStr::operator()(unsigned int const elem){
    return m_parentPtr->at(m_strNum, elem);
}

MatrixCol::MatrixCol(Matrix* parentPtr, unsigned int const colNum) { //���������� ������
    m_parentPtr = parentPtr;
    m_colNum = colNum;
}

int& MatrixCol::operator[](unsigned int const str) { //������� �������� �� �������
    return m_parentPtr->at(str, m_colNum);
}

int& MatrixCol::operator()(unsigned int const num) { //������� �������� �� �������
    return m_parentPtr->at(num, m_colNum);
}


Matrix::Matrix(Matrix const& other) { //�����������
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

Matrix::Matrix() { //����������� 0 �������
    m_size = 0;
    m_body = nullptr;
}

Matrix::Matrix(unsigned int const size) { //����������� ��������� �������
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

Matrix::Matrix(unsigned int const size, int const* const diagValues) { //����������� ������������
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

Matrix& Matrix::operator=(Matrix const& other) { //�������� ������������
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

Matrix Matrix::operator+(Matrix const& other) const {//��������
    if (m_size != other.m_size)
        throw std::invalid_argument("Cannot add different size matrix");

    Matrix result(other.m_size);
    for (unsigned int i = 0; i < m_size; ++i)
        for (unsigned int j = 0; j < m_size; ++j)
            result.at(i, j) = at(i, j) + other.at(i, j);
    return result;
}

Matrix Matrix::operator-(Matrix const& other) const {//���������
    if (m_size != other.m_size)
        throw std::invalid_argument("Cannot sub different size matrix");

    Matrix result(other.m_size);
    for (unsigned int i = 0; i < m_size; ++i)
        for (unsigned int j = 0; j < m_size; ++j)
            result.at(i, j) = at(i, j) - other.at(i, j);
    return result;
}

Matrix Matrix::operator*(Matrix const& other) const { //���������
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

bool Matrix::operator==(Matrix const& other) const {//���������
    if (m_size != other.m_size)
        return false;
    for (unsigned int i = 0; i < m_size; ++i)
        for (unsigned int j = 0; j < m_size; ++j)
            if (at(i, j) != other.at(i, j))
                return false;
    return true;
}

bool Matrix::operator!=(Matrix const& other) const {//�����������
    return !(*this == other);
}

Matrix Matrix::operator!() const { //������������
    Matrix transposed(m_size);
    for (unsigned int i = 0; i < m_size; ++i)
        for (unsigned int j = 0; j < m_size; ++j)
            transposed.at(i, j) = at(j, i);
    return transposed;
}

void Matrix::print_to_file(std::ofstream& fout) const {//����� � ����
    for (unsigned int i = 0; i < m_size; ++i) {
        for (unsigned int j = 0; j < m_size; ++j)
            fout << at(i, j) << " ";
        fout << std::endl;
    }

}

void Matrix::print_to_screen() const {//����� �� �����
    for (unsigned int i = 0; i < m_size; ++i) {
        for (unsigned int j = 0; j < m_size; ++j)
            std::cout << at(i, j) << " ";
        std::cout << std::endl;
    }
}

void Matrix::read_from_file(std::ifstream& fin) {//������ �� �����
    for (unsigned int i = 0; i < m_size; ++i)
        for (unsigned int j = 0; j < m_size; ++j)
            fin >> at(i, j);
}

void Matrix::read_from_screen() {//������ � ������
    for (unsigned int i = 0; i < m_size; ++i)
        for (unsigned int j = 0; j < m_size; ++j)
            std::cin >> at(i, j);
}

Matrix Matrix::operator()(unsigned int const str, unsigned int const col) const { //�����
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

int& Matrix::at(unsigned int const str, unsigned int const col) {//������ � ����������
    if (str >= m_size || col >= m_size)
        throw std::out_of_range("Try to get unexisting part");

    return m_body[str][col];
}

int const& Matrix::at(unsigned int const str, unsigned int const col) const {//��� ���������
    if (str >= m_size || col >= m_size)
        throw std::out_of_range("Try to get unexisting part");

    return m_body[str][col];
}

Matrix::~Matrix() {//����������
    if (m_body != nullptr) {
        for (unsigned int i = 0; i < m_size; ++i)
            delete[] m_body[i];
        delete[] m_body;
    }
}

MatrixStr Matrix::operator[](unsigned int const str) {//������ �� ������
    return MatrixStr(this, str);
}

MatrixCol Matrix::operator()(unsigned int const col) {//������ �� �������
    return MatrixCol(this, col);
}

void Matrix::alloc_body(unsigned int const size) {//��������� ������
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