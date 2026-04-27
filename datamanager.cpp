#include "DataManager.h"
#include <QRegularExpression>
//конструктор(по умолчанию вектор)
DataManager::DataManager(QObject *parent) : QObject(parent) {
    m_vector = std::make_unique<VectorStructure>();
    m_currentStruct = StructType::Vector;
    updateElementsProperty();
}
//имя текущей структуры
QString DataManager::currentType() const {
    switch (m_currentStruct) {
    case StructType::Array:  return "Array";
    case StructType::Vector: return "Vector";
    case StructType::Stack:  return "Stack";
    case StructType::Queue:  return "Queue";
    }
    return {};
}
QVariantList DataManager::elements() const {
    return m_elementsCache;
}
//количество элементов
int DataManager::elementCount() const {
    switch (m_currentStruct) {
    case StructType::Array:  return static_cast<int>(m_array->size());
    case StructType::Vector: return static_cast<int>(m_vector->size());
    case StructType::Stack:  return static_cast<int>(m_stack->size());
    case StructType::Queue:  return static_cast<int>(m_queue->size());
    }
    return 0;
}
//приобразование в нужный тип
Element DataManager::parseInput(const QString &input) {
    bool ok;
    int intVal = input.toInt(&ok);
    if (ok) return intVal;

    double dblVal = input.toDouble(&ok);
    if (ok) return dblVal;

    return input.toStdString();
}
//преобразования для QVariantList
QVariant DataManager::elementToVariant(const Element& e) const {
    return std::visit([](const auto& val) -> QVariant {
        using T = std::decay_t<decltype(val)>;
        if constexpr (std::is_same_v<T, int>)    return val;
        if constexpr (std::is_same_v<T, double>) return val;
        if constexpr (std::is_same_v<T, std::string>) return QString::fromStdString(val);
        return QVariant();
    }, e);
}
//обновление кеша для qml
void DataManager::updateElementsProperty() {
    m_elementsCache.clear();
    auto addElements = [&](const auto& container) {
        for (const auto& elem : container->getElements())
            m_elementsCache.append(elementToVariant(elem));
    };

    switch (m_currentStruct) {
    case StructType::Array:  addElements(m_array); break;
    case StructType::Vector: addElements(m_vector); break;
    case StructType::Stack:  addElements(m_stack); break;
    case StructType::Queue:  addElements(m_queue); break;
    }
}

void DataManager::setStructureType(const QString &type) {
    StructType newType;
    if (type == "Array")       newType = StructType::Array;
    else if (type == "Vector") newType = StructType::Vector;
    else if (type == "Stack")  newType = StructType::Stack;
    else if (type == "Queue")  newType = StructType::Queue;
    else {
        emit errorOccurred("Неизвестный тип структуры");
        return;
    }

    if (newType == m_currentStruct)
        return;

    switch (m_currentStruct) {
    case StructType::Array:  m_array.reset();  break;
    case StructType::Vector: m_vector.reset(); break;
    case StructType::Stack:  m_stack.reset();  break;
    case StructType::Queue:  m_queue.reset();  break;
    }

    switch (newType) {
    case StructType::Array:  m_array = std::make_unique<ArrayStructure>();  break;
    case StructType::Vector: m_vector = std::make_unique<VectorStructure>(); break;
    case StructType::Stack:  m_stack = std::make_unique<StackStructure>();  break;
    case StructType::Queue:  m_queue = std::make_unique<QueueStructure>();  break;
    }

    m_currentStruct = newType;

    emit structureChanged();
    updateElementsProperty();
    emit elementsChanged();
}
//добавление элемента
void DataManager::addElement(const QString &input) {
    try {
        Element el = parseInput(input);
        switch (m_currentStruct) {
        case StructType::Array:  m_array->add(el); break;
        case StructType::Vector: m_vector->add(el); break;
        case StructType::Stack:  m_stack->add(el); break;
        case StructType::Queue:  m_queue->add(el); break;
        }
    } catch (const std::exception &e) {
        emit errorOccurred(e.what());
        return;
    }
    updateElementsProperty();
    emit elementsChanged();
    emit operationVisualized("add", QVariantList());
}
//удаление элемента
void DataManager::removeElement() {
    try {
        switch (m_currentStruct) {
        case StructType::Array:  m_array->remove(); break;
        case StructType::Vector: m_vector->remove(); break;
        case StructType::Stack:  m_stack->remove(); break;
        case StructType::Queue:  m_queue->remove(); break;
        }
    } catch (const std::exception &e) {
        emit errorOccurred(e.what());
        return;
    }
    updateElementsProperty();
    emit elementsChanged();
    emit operationVisualized("remove", QVariantList());
}
//замена по индексу
void DataManager::replaceElement(int index, const QString &newValue) {
    QVariant oldVal;
    switch (m_currentStruct) {
    case StructType::Array:  if (index < (int)m_array->size()) oldVal = elementToVariant(m_array->getElements()[index]); break;
    case StructType::Vector: if (index < (int)m_vector->size()) oldVal = elementToVariant(m_vector->getElements()[index]); break;
    }
    try {
        Element el = parseInput(newValue);
        switch (m_currentStruct) {
        case StructType::Array:  m_array->replace(index, el); break;
        case StructType::Vector: m_vector->replace(index, el); break;
        case StructType::Stack:  m_stack->replace(index, el); break;
        case StructType::Queue:  m_queue->replace(index, el); break;
        }
    } catch (const std::exception &e) {
        emit errorOccurred(e.what());
        return;
    }
    updateElementsProperty();
    emit elementsChanged();
    emit operationVisualized("replace", QVariantList({index, oldVal, newValue}));
}
//сдвиг
void DataManager::shiftElements(int positions) {
    try {
        switch (m_currentStruct) {
        case StructType::Array:  m_array->shift(positions); break;
        case StructType::Vector: m_vector->shift(positions); break;
        case StructType::Stack:  m_stack->shift(positions); break;
        case StructType::Queue:  m_queue->shift(positions); break;
        }
    } catch (const std::exception &e) {
        emit errorOccurred(e.what());
        return;
    }
    updateElementsProperty();
    emit elementsChanged();
    emit operationVisualized("shift", QVariantList({positions}));
}
//очистка
void DataManager::clear() {
    try {
        switch (m_currentStruct) {
        case StructType::Array:  m_array->clear(); break;
        case StructType::Vector: m_vector->clear(); break;
        case StructType::Stack:  m_stack->clear(); break;
        case StructType::Queue:  m_queue->clear(); break;
        }
    } catch (const std::exception &e) {
        emit errorOccurred(e.what());
        return;
    }
    updateElementsProperty();
    emit elementsChanged();
    emit operationVisualized("clear", QVariantList());
}
//получение медианы
double DataManager::getMedian() {

    std::vector<Element> elements;
    switch (m_currentStruct) {
    case StructType::Array:  elements = m_array->getElements(); break;
    case StructType::Vector: elements = m_vector->getElements(); break;
    case StructType::Stack:  elements = m_stack->getElements(); break;
    case StructType::Queue:  elements = m_queue->getElements(); break;
    }

    if (elements.empty()) {
        emit errorOccurred("Нет данных для вычисления медианы");
        return 0.0;
    }
//проверка что тип данных в структуре числовой
    const Element& first = elements.front();
    if (!isInt(first) && !isDouble(first)) {
        emit errorOccurred("Медиана может быть вычислена только для числовых типов");
        return 0.0;
    }
//сортировка через вектор
    std::vector<double> values;
    values.reserve(elements.size());
    for (const auto& el : elements) {
        if (auto p = std::get_if<int>(&el))
            values.push_back(static_cast<double>(*p));
        else if (auto p = std::get_if<double>(&el))
            values.push_back(*p);
    }
    std::sort(values.begin(), values.end());
//сам поиск медианы
    size_t n = values.size();
    if (n % 2 == 1) {
        return values[n / 2];
    } else {
        return (values[n / 2 - 1] + values[n / 2]) / 2.0;
    }
}
//сдвиг для доп задания
void DataManager::cyclicShift(int positions) {
    shiftElements(positions);
}
