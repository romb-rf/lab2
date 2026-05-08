#ifndef ARRAYSTRUCTURE_H
#define ARRAYSTRUCTURE_H
#include "Element.h"
#include <array>
#include <vector>
#include <string>
class ArrayStructure {
public:
    static constexpr size_t MAX_SIZE = 100;
    enum class DataType { Unknown, Int, Double, String };
    void add(const Element& value);
    void remove();
    void replace(size_t index, const Element& newValue);
    void shift(int positions);
    void addAt(int index, const Element& value);
    void removeAt(int index);
    std::vector<Element> getElements() const;
    std::string typeName() const;
    size_t size() const;
    void clear();
    DataType currentDataType() const;
private:
    std::array<Element, MAX_SIZE> m_data{};
    size_t m_size = 0;
    DataType m_dataType = DataType::Unknown;
    void checkTypeAndSet(const Element& value);
    static DataType elementType(const Element& e);
};
#endif
