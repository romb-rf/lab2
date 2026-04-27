#include "stackstructure.h"
#include <stdexcept>
#include <algorithm>
//проверка типа
StackStructure::DataType StackStructure::elementType(const Element& e) {
    if (std::holds_alternative<int>(e))    return DataType::Int;
    if (std::holds_alternative<double>(e)) return DataType::Double;
    if (std::holds_alternative<std::string>(e)) return DataType::String;
    return DataType::Unknown;
}
//проверка и фиксация типа
void StackStructure::checkTypeAndSet(const Element& value) {
    DataType type = elementType(value);
    if (m_dataType == DataType::Unknown) {
        m_dataType = type;
    } else if (m_dataType != type) {
        throw std::runtime_error("Несоответствие типов: не удается добавить элемент другого типа");
    }
}
//добавление элемента
void StackStructure::add(const Element& value) {
    checkTypeAndSet(value);
    m_stack.push(value);
}
//удаление элемента
void StackStructure::remove() {
    if (m_stack.empty())
        throw std::runtime_error("Stack пуст");
    m_stack.pop();
    if (m_stack.empty()) m_dataType = DataType::Unknown;
}
//замена по индексу
void StackStructure::replace(size_t index, const Element& newValue) {
    std::vector<Element> temp = toVector();
    if (index >= temp.size())
        throw std::out_of_range("Индекс выходит за границы");
    checkTypeAndSet(newValue);
    temp[index] = newValue;
    fromVector(temp);
}
//сдвиг
void StackStructure::shift(int positions) {
    std::vector<Element> temp = toVector();
    if (temp.empty()) return;
    positions = positions % static_cast<int>(temp.size());
    if (positions == 0) return;
    std::rotate(temp.rbegin(), temp.rbegin() + positions, temp.rend());
    fromVector(temp);
}
//получение элементов
std::vector<Element> StackStructure::getElements() const {
    return toVector();
}
//имя структуры
std::string StackStructure::typeName() const { return "Stack"; }
//размер
size_t StackStructure::size() const { return m_stack.size(); }
//очистка
void StackStructure::clear() {
    while (!m_stack.empty()) m_stack.pop();
    m_dataType = DataType::Unknown;
}
//тип данных
StackStructure::DataType StackStructure::currentDataType() const {
    return m_dataType;
}
//перевод в вектор
std::vector<Element> StackStructure::toVector() const {
    std::vector<Element> result;
    std::stack<Element> copy = m_stack;
    while (!copy.empty()) {
        result.push_back(copy.top());
        copy.pop();
    }
    std::reverse(result.begin(), result.end());
    return result;
}
//перевод в стек
void StackStructure::fromVector(const std::vector<Element>& vec) {
    while (!m_stack.empty()) m_stack.pop();
    m_dataType = DataType::Unknown;
    for (const Element& val : vec)
        add(val);
}
