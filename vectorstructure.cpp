#include "vectorstructure.h"
#include <stdexcept>
#include <algorithm>
//проверка типа
VectorStructure::DataType VectorStructure::elementType(const Element& e) {
    if (std::holds_alternative<int>(e))    return DataType::Int;
    if (std::holds_alternative<double>(e)) return DataType::Double;
    if (std::holds_alternative<std::string>(e)) return DataType::String;
    return DataType::Unknown;
}
//проверка и фиксация типа
void VectorStructure::checkTypeAndSet(const Element& value) {
    DataType type = elementType(value);
    if (m_dataType == DataType::Unknown) {
        m_dataType = type;
    } else if (m_dataType != type) {
        throw std::runtime_error("Несоответствие типов: не удается добавить элемент другого типа");
    }
}
//добавление элемента
void VectorStructure::add(const Element& value) {
    checkTypeAndSet(value);
    m_data.push_back(value);
}
//удаление элемента
void VectorStructure::remove() {
    if (m_data.empty())
        throw std::runtime_error("Vector пуст");
    m_data.pop_back();
    if (m_data.empty()) m_dataType = DataType::Unknown;
}
//замена по индексу
void VectorStructure::replace(size_t index, const Element& newValue) {
    if (index >= m_data.size())
        throw std::out_of_range("Индекс выходит за границы");
    checkTypeAndSet(newValue);
    m_data[index] = newValue;
}
//сдвиг
void VectorStructure::shift(int positions) {
    if (m_data.empty()) return;
    positions = positions % static_cast<int>(m_data.size());
    if (positions == 0) return;
    std::rotate(m_data.rbegin(), m_data.rbegin() + positions, m_data.rend());
}
//получение элементов
std::vector<Element> VectorStructure::getElements() const {
    return m_data;
}
//имя структуры
std::string VectorStructure::typeName() const { return "Vector"; }
//размер массива
size_t VectorStructure::size() const { return m_data.size(); }
//очистка
void VectorStructure::clear() {
    m_data.clear();
    m_dataType = DataType::Unknown;
}
//тип данных
VectorStructure::DataType VectorStructure::currentDataType() const {
    return m_dataType;
}
void VectorStructure::addAt(int index, const Element& value) {
    if (index > m_data.size()) {
        throw std::out_of_range("Индекс вне допустимого диапазона для вставки");
    }
    // Проверка и фиксация типа (может выбросить исключение)
    checkTypeAndSet(value);
    m_data.insert(m_data.begin() + index, value);
}

void VectorStructure::removeAt(int index) {
    if (index >= m_data.size()) {
        throw std::out_of_range("Индекс выходит за границы");
    }
    m_data.erase(m_data.begin() + index);
    // Если после удаления вектор стал пустым, сбросить тип
    if (m_data.empty()) {
        m_dataType = DataType::Unknown;
    }
}
