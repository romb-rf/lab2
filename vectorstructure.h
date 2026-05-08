#ifndef VECTORSTRUCTURE_H
#define VECTORSTRUCTURE_H
#include "Element.h"
#include <vector>
#include <string>
class VectorStructure {
public:
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
    std::vector<Element> m_data;
    DataType m_dataType = DataType::Unknown;
    void checkTypeAndSet(const Element& value);
    static DataType elementType(const Element& e);
};
#endif
