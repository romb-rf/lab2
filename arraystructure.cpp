#include "arraystructure.h"
#include <stdexcept>
#include <algorithm>
//проверка типа
ArrayStructure::DataType ArrayStructure::elementType(const Element& e) {
    if (std::holds_alternative<int>(e))    return DataType::Int;
    if (std::holds_alternative<double>(e)) return DataType::Double;
    if (std::holds_alternative<std::string>(e)) return DataType::String;
    return DataType::Unknown;
}
//проверка и фиксация типа
void ArrayStructure::checkTypeAndSet(const Element& value) {
    DataType type = elementType(value);
    if (m_dataType == DataType::Unknown) {
        m_dataType = type;
    } else if (m_dataType != type) {
        throw std::runtime_error("Несоответствие типов: не удается добавить элемент другого типа");
    }
}
//добавление элемента
void ArrayStructure::add(const Element& value) {
    if (m_size >= MAX_SIZE)
        throw std::runtime_error("Массив переполнен");
    checkTypeAndSet(value);
    m_data[m_size++] = value;
}
//удаление элемента
void ArrayStructure::remove() {
    if (m_size == 0)
        throw std::runtime_error("Массив пуст");
    --m_size;
    if (m_size == 0) m_dataType = DataType::Unknown;
}
//замена по индексу
void ArrayStructure::replace(size_t index, const Element& newValue) {
    if (index >= m_size)
        throw std::out_of_range("Индекс выходит за границы");
    checkTypeAndSet(newValue);
    m_data[index] = newValue;
}
//сдвиг
void ArrayStructure::shift(int positions) {
    if (m_size == 0) return;
    positions = positions % static_cast<int>(m_size);
    if (positions == 0) return;
    std::vector<Element> temp(m_data.begin(), m_data.begin() + m_size);
    std::rotate(temp.rbegin(), temp.rbegin() + positions, temp.rend());
    std::copy(temp.begin(), temp.end(), m_data.begin());
}
//получение элементов
std::vector<Element> ArrayStructure::getElements() const {
    return std::vector<Element>(m_data.begin(), m_data.begin() + m_size);
}
//имя структуры
std::string ArrayStructure::typeName() const { return "Array"; }
//размер массива
size_t ArrayStructure::size() const { return m_size; }
//очистка
void ArrayStructure::clear() {
    m_size = 0;
    m_dataType = DataType::Unknown;
}
//тип данных
ArrayStructure::DataType ArrayStructure::currentDataType() const {
    return m_dataType;
}
