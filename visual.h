#ifndef STRUCTUREVISUALIZER_H
#define STRUCTUREVISUALIZER_H

#include <QWidget>
#include <QScrollArea>
#include <QBoxLayout>
#include <QVector>
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>
#include "datamanager.h"

class CellWidget;

class StructureVisualizer : public QWidget
{
    Q_OBJECT
public:
    explicit StructureVisualizer(DataManager *manager, QWidget *parent = nullptr);

public slots:
    void setStructureType(const QString &type);
    void insertElement(const QString &value);
    void replaceElement(int index, const QString &newValue);
    void clearAll();
    double getMedian() const;
    void cyclicShift(int positions);
    void removeCorrectElement();

    void addElementAt(int visualIndex, const QString& value);
    void removeElementAt(int visualIndex);
private:
    void rebuildCells();
    void animateRemoveLast();
    void animateReplace(int index, const QString &newValue);
    void rebuildCellsInverted();
    void animateRemove(int widgetIndex);
    void animateRemoveAtIndex(int widgetIndex, int modelIndex);
    QString generateAddress(int byteOffset) const;
    int totalSizeUpTo(int count) const;

    DataManager *m_manager;
    QScrollArea *m_scrollArea;
    QWidget *m_container;
    QBoxLayout *m_layout;
    QVector<CellWidget *> m_cells;
    QString m_currentType;
    bool m_isAnimating;
    static const int BASE_ADDRESS = 0x1000;
};

#endif
