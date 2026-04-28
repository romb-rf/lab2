#include "queuestructure.h"
#include <stdexcept>
#include <algorithm>
//проверка типа
QueueStructure::DataType QueueStructure::elementType(const Element& e) {
    if (std::holds_alternative<int>(e))    return DataType::Int;
    if (std::holds_alternative<double>(e)) return DataType::Double;
    if (std::holds_alternative<std::string>(e)) return DataType::String;
    return DataType::Unknown;
}
//проверка и фиксация типа
void QueueStructure::checkTypeAndSet(const Element& value) {
    DataType type = elementType(value);
    if (m_dataType == DataType::Unknown) {
        m_dataType = type;
    } else if (m_dataType != type) {
        throw std::runtime_error("Несоответствие типов: не удается добавить элемент другого типа");
    }
}
//добавление элемента
void QueueStructure::add(const Element& value) {
    checkTypeAndSet(value);
    m_queue.push(value);
}
//удаление элемента
void QueueStructure::remove() {
    if (m_queue.empty())
        throw std::out_of_range("Queue пуста");
    m_queue.pop();
    if (m_queue.empty()) m_dataType = DataType::Unknown;
}
//замена по индексу
void QueueStructure::replace(size_t index, const Element& newValue) {
    std::vector<Element> temp = toVector();
    if (index >= temp.size())
        throw std::out_of_range("Индекс выходит за границы");
    checkTypeAndSet(newValue);
    temp[index] = newValue;
    fromVector(temp);
}
//сдвиг
void QueueStructure::shift(int positions) {
    std::vector<Element> temp = toVector();
    if (temp.empty()) return;
    positions = positions % static_cast<int>(temp.size());
    if (positions == 0) return;
    std::rotate(temp.rbegin(), temp.rbegin() + positions, temp.rend());
    fromVector(temp);
}
//получение элементов
std::vector<Element> QueueStructure::getElements() const {
    return toVector();
}
//имя структуры
std::string QueueStructure::typeName() const { return "Queue"; }
//размер
size_t QueueStructure::size() const { return m_queue.size(); }
//очистка
void QueueStructure::clear() {
    while (!m_queue.empty()) m_queue.pop();
    m_dataType = DataType::Unknown;
}
//тип данных
QueueStructure::DataType QueueStructure::currentDataType() const {
    return m_dataType;
}
//перевод в вектор
std::vector<Element> QueueStructure::toVector() const {
    std::vector<Element> result;
    std::queue<Element> copy = m_queue;
    while (!copy.empty()) {
        result.push_back(copy.front());
        copy.pop();
    }
    return result;
}
//перевод в очередь
void QueueStructure::fromVector(const std::vector<Element>& vec) {
    clear();
    for (const Element& val : vec)
        add(val);
}
