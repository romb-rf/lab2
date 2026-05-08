#ifndef STACKSTRUCTURE_H
#define STACKSTRUCTURE_H
#include "Element.h"
#include <stack>
#include <vector>
#include <string>
class StackStructure {
public:
    enum class DataType { Unknown, Int, Double, String };
    void add(const Element& value);
    void remove();
    void replace(size_t index, const Element& newValue);
    void shift(int positions);
    std::vector<Element> getElements() const;
    std::string typeName() const;
    size_t size() const;
    void clear();
    DataType currentDataType() const;
private:
    std::stack<Element> m_stack;
    DataType m_dataType = DataType::Unknown;
    std::vector<Element> toVector() const;
    void fromVector(const std::vector<Element>& vec);
    void checkTypeAndSet(const Element& value);
    static DataType elementType(const Element& e);
};
#endif
