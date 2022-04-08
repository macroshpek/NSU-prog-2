#include <string>
#include <fstream>
#include <utility> // For std::pair
#include <functional> // For std::hash
#include <vector>
#include <stdexcept>

template <class T> class List {
public:
    List() {
        m_firstElem = nullptr;
    }

    List(List const& other) {
        if (other.empty())
            m_firstElem = nullptr;
        m_firstElem = new Elem<T>(other.m_firstElem->value());
        Elem<T>* i = m_firstElem;
        Elem<T>* j = other.m_firstElem;
        while (!j->is_end()) {
            i->set_next(new Elem<T>(j->value()));
            i = i->get_next();
            j = j->get_next();
        }
    }

    bool empty() const {
        return m_firstElem == nullptr;
    }

    size_t size() const {
        size_t counter = 1;
        for (Elem<T>* i = m_firstElem; !i->is_end(); i = i->get_next())
            ++counter;
        return counter;
    }

    T& operator[](size_t const place) {
        if (empty())
            throw std::logic_error("Try to get from empty list");
        Elem<T>* i = m_firstElem;
        for (size_t j = 0; j < place; ++j) {
            if (i->is_end())
                throw std::out_of_range("Try to get element that does not exist");
            i = i->get_next();
        }
        return i->value();
    }

    T const& operator[](size_t const place) const {
        if (empty())
            throw std::logic_error("Try to get from empty list");
        Elem<T>* i = m_firstElem;
        for (size_t j = 0; j < place; ++j) {
            if (i->is_end())
                throw std::out_of_range("Try to get element that does not exist");
            i = i->get_next();
        }
        return i->value();
    }

    void push_front(T const& value) {
        Elem<T>* oldFirstElem = m_firstElem;
        m_firstElem = new Elem<T>(value);
        m_firstElem->set_next(oldFirstElem);
    }

    void push_back(T const& value) {
        Elem<T>* i = m_firstElem;
        while (!i->is_end())
            i = i->get_next();
        i->set_next(new Elem<T>(value));
    }

    void pop_front() {
        if (empty())
            throw std::logic_error("Try to pop from empty list");
        Elem<T>* secondElem = m_firstElem->get_next();
        m_firstElem->~Elem();
        m_firstElem = secondElem;
    }

    void pop_back() {
        if (empty())
            throw std::logic_error("Try to pop from empty list");
        Elem<T>* i = m_firstElem;
        if (i->is_end()) {
            i->~Elem();
            m_firstElem = nullptr;
        }
        while (!i->get_next()->is_end())
            i = i->get_next();
        i->get_next()->~Elem();
        i->set_next(nullptr);
    }

    void remove(size_t const place) {
        if (empty())
            return;
        if (place == 0) {
            pop_front();
            return;
        }
        Elem<T>* i = m_firstElem;
        for (size_t j = 0; j < place - 1; ++j) {
            if (i->is_end())
                return;
            i = i->get_next();
        }
        Elem<T>* copyNext = i->get_next()->get_next();
        i->get_next()->~Elem();
        i->set_next(copyNext);
    }

    ~List() {
        while (!empty())
            pop_front();
    }

    List<T>& operator=(List<T> const& other) {
        if (this == &other)
            return *this;
        while (!empty())
            pop_front();
        if (other.empty())
            m_firstElem = nullptr;
        m_firstElem = new Elem<T>(other.m_firstElem->value());
        Elem<T>* i = m_firstElem;
        Elem<T>* j = other.m_firstElem;
        while (!j->is_end()) {
            i->set_next(new Elem<T>(j->value()));
            i = i->get_next();
            j = j->get_next();
        }
    }

private:
    template <class T> class Elem {
    public:
        explicit Elem(T const& value) {
            m_value = value;
            m_next = nullptr;
        }

        bool is_end() const {
            return m_next == nullptr;
        }

        void set_next(Elem<T>* next) {
            if (!is_end())
                throw std::logic_error("Try to overwrite link in list");
            m_next = next;
        }

        Elem<T>* get_next() { // Return nullptr if no next
            return m_next;
        }

        T& value() {
            return m_value;
        }

        ~Elem() {
            m_value.~T();
        }

    private:
        T m_value;
        Elem<T>* m_next;
    };

    Elem<T>* m_firstElem;

    template <class T> class Iter {
    public:
        explicit Iter(Elem<T>* elem) {
            m_elem = elem;
        }

        bool is_valid() const {
            return m_elem != nullptr;
        }

        Elem<T>*& operator->() {
            if (!is_valid())
                throw std::logic_error("Try to get from invalid iter");
            return m_elem;
        }

        Elem<T>& operator*() {
            if (!is_valid())
                throw std::logic_error("Try to get from invalid iter");
            return *m_elem;
        }

        Iter<T>& operator++() { // Prefix
            if (is_valid())
                m_elem = m_elem->get_next();
            return *this;
        }

        Iter<T> operator++(int) { // Postfix
            Iter<T> copy(m_elem);
            ++(*this);
            return copy;
        }
    private:
        Elem<T>* m_elem;
    };

public:
    Iter<T> front() {
        return Iter<T>(m_firstElem);
    }
};

template <class T, class N> class HashMap {
    using KeyType = T;
    using ValueType = N;
    using Elem = std::pair<T, N>;

    template <class T, class N> class CHMIterator { // Read only iterator
    public:
        CHMIterator(HashMap<T, N> const& hashMap, size_t const position, size_t const positionList) {
            m_hashMap = &hashMap;
            m_positionInVec = position;
            m_positionInList = positionList;
        }

        bool is_valid() const {
            if (m_positionInVec >= m_hashMap->capacity())
                return false;
            if (m_positionInList >= m_hashMap->m_body->at(m_positionInVec).size())
                return false;
            return true;
        }

        ValueType const& get_value() { // Throws std::logic_error if invalid
            List<Elem>& list = m_hashMap->m_body->at(m_positionInVec);
            return list[m_positionInList].second;
        }

        KeyType const& get_key() { // Throws std::logic_error if invalid
            List<Elem>& list = m_hashMap->m_body->at(m_positionInVec);
            return list[m_positionInList].first;
        }

        std::pair<T, N> operator*() {
            List<Elem>& list = m_hashMap->m_body->at(m_positionInVec);
            return list[m_positionInList];
        }

        CHMIterator<T, N>& operator++() { // Prefix increment
            List<Elem>& list = m_hashMap->m_body->at(m_positionInVec);
            if (m_positionInList + 1 < list.size()) {
                ++m_positionInList;
                return *this;
            }
            m_positionInList = 0;
            do
                ++m_positionInVec;
            while (m_positionInVec < m_hashMap->capacity() && m_hashMap->m_body->at(m_positionInVec).empty());
            return *this;
        }

        CHMIterator<T, N> operator++(int) { // Postfix increment
            HMIterator<T, N> copy(m_hashMap, m_positionInVec, m_positionInList);
            ++(*this);
            return copy;
        }

        bool operator==(CHMIterator<T, N> const& other) const {
            if (m_hashMap != other.m_hashMap)
                return false;
            if (m_positionInVec != other.m_positionInVec)
                return false;
            return m_positionInList == other.m_positionInList;
        }

        bool operator!=(CHMIterator<T, N> const& other) const {
            return !(*this == other);
        }

        CHMIterator<T, N> find(KeyType const& key) const {
            if (m_numberOfElements == 0)
                return end();
            size_t hash = count_hash(key);
            List<Elem>& list = m_body->at(hash);
            auto i = list.front();
            for (size_t j = 0; i.is_valid(); ++i, ++j) {
                if (i.get().value().first == key)
                    return CHMIterator<T, N>(*this, hash, j);
            }
            return end();
        }

    protected:
        HashMap<T, N> const* m_hashMap;
        size_t m_positionInVec;
        size_t m_positionInList;
    };

    template <class T, class N> class HMIterator : public CHMIterator <T, N> { // Writeable iterator
    public:
        HMIterator(HashMap<T, N>& hashMap, size_t const positionVec, size_t const positionList) : CHMIterator<T, N>(hashMap, positionVec, positionList) {
            m_hashMap = &hashMap;
            m_positionInVec = positionVec;
            m_positionInList = positionList;
        }

        ValueType& get_value() {
            List<Elem>& list = m_hashMap->m_body->at(m_positionInVec);
            return list[m_positionInList].second;
        }

        KeyType& get_key() {
            List<Elem>& list = m_hashMap->m_body->at(m_positionInVec);
            return list[m_positionInList].first;
        }

        HMIterator<T, N>& operator++() { // Prefix increment
            List<Elem>& list = m_hashMap->m_body->at(m_positionInVec);
            if (m_positionInList + 1 < list.size()) {
                ++m_positionInList;
                return *this;
            }
            m_positionInList = 0;
            do
                ++m_positionInVec;
            while (m_positionInVec < m_hashMap->capacity() && m_hashMap->m_body->at(m_positionInVec).empty());
            return *this;
        }

        HMIterator<T, N> operator++(int) { // Postfix increment
            HMIterator<T, N> copy(m_hashMap, m_positionInVec, m_positionInList);
            ++(*this);
            return copy;
        }

        bool operator==(HMIterator<T, N> const& other) const {
            if (m_hashMap != other.m_hashMap)
                return false;
            if (m_positionInVec != other.m_positionInVec)
                return false;
            return m_positionInList == other.m_positionInList;
        }

        bool operator!=(HMIterator<T, N> const& other) const {
            return !(*this == other);
        }

    protected:
        HashMap<T, N>* m_hashMap;
        size_t m_positionInVec;
        size_t m_positionInList;
    };

public:
    explicit HashMap(double const filledProportion) {
        m_body = new std::vector<List<Elem>>();
        m_filledProportion = filledProportion;
        m_numberOfElements = 0;
    }

    void insert(KeyType const& key, ValueType const& value) { // Overwrite value if key already here
        if (is_expansion_needed_to_insert())
            expand();
        size_t const hash = count_hash(key);
        List<Elem>& list = m_body->at(hash);
        for (auto i = list.front(); i.is_valid(); ++i)
            if (i->value().first == key) {
                i->value().second = value;
                return;
            }
        list.push_front(std::make_pair(key, value));
        ++m_numberOfElements;
    }

    void remove(KeyType const& key) { // Does nothing if elem doesn't exist
        size_t const hash = count_hash(key);
        List<Elem>& list = m_body->at(hash);
        auto i = list.front();
        for (int j = 0; i.is_valid(); ++i, ++j) {
            if (i->value().first == key) {
                list.remove(j);
                --m_numberOfElements;
            }
        }
    }

    HMIterator<T, N> begin() {
        size_t begin;
        for (begin = 0; begin < capacity() && m_body->at(begin).empty(); ++begin) {}
        return HMIterator<T, N>(*this, begin, 0);
    }

    CHMIterator<T, N> begin() const {
        size_t begin;
        for (begin = 0; begin < capacity() && m_body->at(begin).empty(); ++begin) {}
        return CHMIterator<T, N>(*this, begin, 0);
    }

    HMIterator<T, N> end() { // Points to the place AFTER the last elem (as STL iterators)
        return HMIterator<T, N>(*this, capacity(), 0);
    }

    CHMIterator<T, N> end() const {
        return CHMIterator<T, N>(*this, capacity(), 0);
    }

    ~HashMap() {
        delete m_body;
    }

    size_t get_number_of_elements() const {
        return m_numberOfElements;
    }

private:
    size_t capacity() const {
        return m_body->size();
    }

    size_t count_hash(KeyType const& key) const {
        std::hash<KeyType> hasher;
        return hasher(key) % capacity();
    }

    bool is_expansion_needed_to_insert() const {
        if (capacity() == 0)
            return true;
        return (m_numberOfElements + 1.0) / capacity() >= m_filledProportion;
    }

    void expand() {
        size_t const newCapacity = (m_numberOfElements + 1.0) * 2.0 / m_filledProportion;
        auto* newVec = new std::vector<List<Elem>>(newCapacity);
        auto const endIter = end();
        for (auto i = begin(); i != endIter; ++i) {
            KeyType const key = i.get_key();
            ValueType const value = i.get_value();
            std::hash<KeyType> hasher;
            size_t const newPlace = hasher(key) % newCapacity;
            newVec->at(newPlace).push_front(std::make_pair(key, value));
        }
        delete m_body;
        m_body = newVec;
    }

    std::vector<List<Elem>>* m_body;
    size_t m_numberOfElements;
    double m_filledProportion;
};

enum class Type {
    intT,
    doubleT,
    stringT
};

std::pair<Type, Type> read_mode(std::ifstream& fin) {
    std::string in1, in2;
    fin >> in1 >> in2;
    Type type1, type2;
    if (in1 == "I")
        type1 = Type::intT;
    else
        if (in1 == "D")
            type1 = Type::doubleT;
        else
            if (in1 == "S")
                type1 = Type::stringT;
            else
                throw std::invalid_argument("Wrong mode");
    if (in2 == "I")
        type2 = Type::intT;
    else
        if (in2 == "D")
            type2 = Type::doubleT;
        else
            if (in2 == "S")
                type2 = Type::stringT;
            else
                throw std::invalid_argument("Wrong mode");
    return std::make_pair(type1, type2);
}

size_t read_number_of_operations(std::ifstream& fin) {
    size_t number;
    fin >> number;
    return number;
}

template <class T, class N> void read_and_execute_operaton(std::ifstream& fin, HashMap<T, N>& hashmap) {
    char command;
    fin >> command;
    T key;
    fin >> key;
    if (command == 'A') {
        N value;
        fin >> value;
        hashmap.insert(key, value);
        return;
    }
    if (command == 'R') {
        hashmap.remove(key);
        return;
    }
    throw std::invalid_argument("Wrong command");
}

template <class T, class N> void count_elements(HashMap<T, N> const& hashmap, size_t& counter, size_t& unique_counter) {
    double const filledProportion = 0.1;
    counter = hashmap.get_number_of_elements();
    HashMap<N, T> revertedHashmap(filledProportion);
    for (auto i = hashmap.begin(); i != hashmap.end(); ++i) {
        std::pair<T, N> const elem = *i;
        revertedHashmap.insert(elem.second, elem.first);
    }
    unique_counter = revertedHashmap.get_number_of_elements();
}

template <typename T, typename N>
void func(double const filledProportion, size_t numberOperations, std::ifstream& fin, size_t counter, size_t uniqueCounter, std::ofstream& fout) {
    HashMap<T, N> hashmap(filledProportion);
    for (size_t i = 0; i < numberOperations; ++i) {
        read_and_execute_operaton(fin, hashmap);
    }
    count_elements(hashmap, counter, uniqueCounter);
    fout << counter << " " << uniqueCounter << std::endl;
}

int main() {
    double const filledProportion = 0.1;
    std::ifstream fin("input.txt");
    std::ofstream fout("output.txt");
    std::pair<Type, Type> const mode = read_mode(fin);
    size_t const numberOperations = read_number_of_operations(fin);
    size_t counter = 0, uniqueCounter = 0;
    if (mode.first == Type::intT && mode.second == Type::intT) {
        func<int, int>(filledProportion, numberOperations, fin, counter, uniqueCounter, fout);
    }
    if (mode.first == Type::intT && mode.second == Type::doubleT) {
        func<int, double>(filledProportion, numberOperations, fin, counter, uniqueCounter, fout);
    }
    if (mode.first == Type::intT && mode.second == Type::stringT) {
        func<int, std::string>(filledProportion, numberOperations, fin, counter, uniqueCounter, fout);
    }
    if (mode.first == Type::doubleT && mode.second == Type::intT) {
        func<double, int>(filledProportion, numberOperations, fin, counter, uniqueCounter, fout);
    }
    if (mode.first == Type::doubleT && mode.second == Type::doubleT) {
        func<double, double>(filledProportion, numberOperations, fin, counter, uniqueCounter, fout);
    }
    if (mode.first == Type::doubleT && mode.second == Type::stringT) {
        func<double, std::string>(filledProportion, numberOperations, fin, counter, uniqueCounter, fout);
    }
    if (mode.first == Type::stringT && mode.second == Type::intT) {
        func<std::string, int>(filledProportion, numberOperations, fin, counter, uniqueCounter, fout);
    }
    if (mode.first == Type::stringT && mode.second == Type::doubleT) {
        func<std::string, double>(filledProportion, numberOperations, fin, counter, uniqueCounter, fout);
    }
    if (mode.first == Type::stringT && mode.second == Type::stringT) {
        func<std::string, std::string>(filledProportion, numberOperations, fin, counter, uniqueCounter, fout);
    }
    fin.close();
    fout.close();
    return 0;
}