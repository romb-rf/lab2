#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QString>
#include <QVariantList>
#include <memory>

#include "element.h"
#include "ArrayStructure.h"
#include "VectorStructure.h"
#include "StackStructure.h"
#include "QueueStructure.h"

class DataManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentType READ currentType NOTIFY structureChanged)
    Q_PROPERTY(QVariantList elements READ elements NOTIFY elementsChanged)
    Q_PROPERTY(int elementCount READ elementCount NOTIFY elementsChanged)

public:
    enum class StructType { Array, Vector, Stack, Queue };
    Q_ENUM(StructType)

    explicit DataManager(QObject *parent = nullptr);

    QString currentType() const;
    QVariantList elements() const;
    int elementCount() const;

    Q_INVOKABLE void setStructureType(const QString &type);
    Q_INVOKABLE void addElement(const QString &input);
    Q_INVOKABLE void removeElement();
    Q_INVOKABLE void replaceElement(int index, const QString &newValue);
    Q_INVOKABLE void shiftElements(int positions);
    Q_INVOKABLE void clear();

    Q_INVOKABLE double getMedian();
    Q_INVOKABLE void cyclicShift(int positions);

signals:
    void structureChanged();
    void elementsChanged();
    void errorOccurred(const QString &message);
    void operationVisualized(const QString &operation, const QVariantList &params);
private:
    StructType m_currentStruct = StructType::Vector;

    std::unique_ptr<ArrayStructure>   m_array;
    std::unique_ptr<VectorStructure>  m_vector;
    std::unique_ptr<StackStructure>   m_stack;
    std::unique_ptr<QueueStructure>   m_queue;

    QVariantList m_elementsCache;

    void updateElementsProperty();
    Element parseInput(const QString &input);
    QVariant elementToVariant(const Element &e) const;
};

#endif
