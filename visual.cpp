#include "visual.h"
#include "cellwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

//размер в байтах для QVariant
static int getVariantByteSize(const QVariant &var)
{
    switch (var.typeId()) {
    case QMetaType::Int:       return sizeof(int);
    case QMetaType::Double:    return sizeof(double);
    case QMetaType::QString:   return var.toString().toUtf8().size() + 1;
    default:                   return 0;
    }
}

StructureVisualizer::StructureVisualizer(DataManager *manager, QWidget *parent)
    : QWidget(parent), m_manager(manager), m_isAnimating(false)
{
    m_scrollArea = new QScrollArea(this);
    m_container  = new QWidget;
    m_scrollArea->setWidget(m_container);
    m_scrollArea->setWidgetResizable(true);

    m_layout = new QHBoxLayout(m_container);
    m_layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    m_container->setLayout(m_layout);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_scrollArea);
    setLayout(mainLayout);

}

QString StructureVisualizer::generateAddress(int byteOffset) const
{
    int addr = BASE_ADDRESS + byteOffset;
    return QString("0x%1").arg(addr, 4, 16, QChar('0'));
}

int StructureVisualizer::totalSizeUpTo(int count) const
{
    int total = 0;
    const QVariantList elems = m_manager->elements();
    for (int i = 0; i < count && i < elems.size(); ++i)
        total += getVariantByteSize(elems.at(i));
    return total;
}
void StructureVisualizer::setStructureType(const QString &type)
{

    m_currentType = type;

    QLayout *oldLayout = m_container->layout();
    if (oldLayout) {
        QLayoutItem *item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            if (item->widget())
                delete item->widget();
            delete item;
        }
        delete oldLayout;
    }
    // новый слой
    if (type == "Array" || type == "Vector") {
        m_layout = new QHBoxLayout(m_container);
        m_layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    } else {
        m_layout = new QVBoxLayout(m_container);
        m_layout->setAlignment(Qt::AlignBottom | Qt::AlignHCenter);

        m_layout->addStretch(1);
    }

    rebuildCells();
}

void StructureVisualizer::rebuildCells()
{
    while (m_layout->count() > 0) {
        QLayoutItem *item = m_layout->takeAt(0);
        if (item->widget()) delete item->widget();
        delete item;
    }
    m_cells.clear();

    if (m_currentType == "Stack" || m_currentType == "Queue") {
        m_layout->addStretch(1);

        const QVariantList elems = m_manager->elements();
        int currentOffset = 0;

        for (int i = 0; i < elems.size(); ++i) {
            const QVariant &var = elems.at(i);
            const int byteSize = getVariantByteSize(var);
            CellWidget *cell = new CellWidget(generateAddress(currentOffset),
                                              var.toString(),
                                              byteSize,
                                              m_container);
            m_layout->insertWidget(1, cell);
            m_cells.prepend(cell);
            currentOffset += byteSize;
        }
    } else {
        const QVariantList elems = m_manager->elements();
        int currentOffset = 0;
        for (int i = 0; i < elems.size(); ++i) {
            const QVariant &var = elems.at(i);
            const int byteSize = getVariantByteSize(var);
            CellWidget *cell = new CellWidget(generateAddress(currentOffset),
                                              var.toString(),
                                              byteSize,
                                              m_container);
            m_layout->addWidget(cell);
            m_cells.append(cell);
            currentOffset += byteSize;
        }
    }
}

void StructureVisualizer::insertElement(const QString &value)
{
    m_manager->addElement(value);
    rebuildCells();

    if (!m_cells.isEmpty()) {
        CellWidget *cell = nullptr;
        if (m_currentType == "Stack" || m_currentType == "Queue") {
            cell = m_cells.first();
        } else {
            cell = m_cells.last();
        }
        cell->setOpacity(0.0);
        QPropertyAnimation *fadeIn = new QPropertyAnimation(cell, "opacity");
        fadeIn->setDuration(300);
        fadeIn->setStartValue(0.0);
        fadeIn->setEndValue(1.0);
        fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void StructureVisualizer::removeCorrectElement() {
    if (m_cells.isEmpty()) {
        m_isAnimating = false;
        m_manager->removeElement();
        rebuildCells();
        return;
    }

    if (m_isAnimating) return;
    m_isAnimating = true;

    int widgetIndex;
    if (m_currentType == "Stack") {
        widgetIndex = 0;
    } else if (m_currentType == "Queue") {
        widgetIndex = m_cells.size() - 1;
    } else {
        widgetIndex = m_cells.size() - 1;
    }

    animateRemove(widgetIndex);
}
void StructureVisualizer::replaceElement(int visualIndex, const QString &newValue)
{
    if (visualIndex < 0 || visualIndex >= m_cells.size()) {
        m_manager->replaceElement(visualIndex, newValue);
        return;
    }

    int modelIndex;
    int total = m_cells.size();
    if (m_currentType == "Stack" || m_currentType == "Queue") {
        modelIndex = total - 1 - visualIndex;
    } else {
        modelIndex = visualIndex;
    }
    CellWidget *cell = m_cells.at(visualIndex);
    auto *group = new QSequentialAnimationGroup(this);

    QPropertyAnimation *blink = new QPropertyAnimation(cell, "opacity");
    blink->setDuration(150);
    blink->setStartValue(1.0);
    blink->setEndValue(0.2);
    blink->setLoopCount(2);

    group->addAnimation(blink);
    group->addPause(100);

    connect(group, &QSequentialAnimationGroup::finished, this, [this, modelIndex, newValue]() {
        m_manager->replaceElement(modelIndex, newValue);
        rebuildCells();
    });

    group->start(QAbstractAnimation::DeleteWhenStopped);
}
void StructureVisualizer::animateRemove(int widgetIndex) {
    if (widgetIndex < 0 || widgetIndex >= m_cells.size()) {
        m_isAnimating = false;
        return;
    }

    CellWidget *cell = m_cells.at(widgetIndex);
    auto *group = new QSequentialAnimationGroup(this);

    QPropertyAnimation *highlight = new QPropertyAnimation(cell, "opacity");
    highlight->setDuration(200);
    highlight->setStartValue(1.0);
    highlight->setEndValue(0.5);
    cell->setStyleSheet("background-color: #ffcdd2; border: 1px solid #e53935;");

    QPropertyAnimation *fadeOut = new QPropertyAnimation(cell, "opacity");
    fadeOut->setDuration(300);
    fadeOut->setStartValue(0.5);
    fadeOut->setEndValue(0.0);

    group->addAnimation(highlight);
    group->addPause(100);
    group->addAnimation(fadeOut);

    QString currentType = m_currentType;

    connect(group, &QSequentialAnimationGroup::finished, this, [this, currentType]() {
        m_manager->removeElement();
        rebuildCells();
        m_isAnimating = false;
    });

    group->start(QAbstractAnimation::DeleteWhenStopped);
}
void StructureVisualizer::clearAll()
{
    m_manager->clear();
    rebuildCells();
}

double StructureVisualizer::getMedian() const
{
    return m_manager->getMedian();

}

void StructureVisualizer::cyclicShift(int positions)
{
    m_manager->shiftElements(positions);
    rebuildCells();
}
void StructureVisualizer::addElementAt(int visualIndex, const QString& value) {
    if (m_currentType == "Stack" || m_currentType == "Queue") {
        return;
    }
    m_manager->addElementAt(visualIndex, value);
    rebuildCells();
    if (visualIndex >= 0 && visualIndex < m_cells.size()) {
        CellWidget* cell = m_cells.at(visualIndex);
        cell->setOpacity(0.0);
        QPropertyAnimation* fadeIn = new QPropertyAnimation(cell, "opacity");
        fadeIn->setDuration(300);
        fadeIn->setStartValue(0.0);
        fadeIn->setEndValue(1.0);
        fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void StructureVisualizer::removeElementAt(int visualIndex) {
    if (m_currentType == "Stack" || m_currentType == "Queue") {

        return;
    }
    if (visualIndex < 0 || visualIndex >= m_cells.size()) {
        m_manager->removeElementAt(visualIndex);
        return;
    }

    if (m_isAnimating) return;
    m_isAnimating = true;
    animateRemoveAtIndex(visualIndex, visualIndex);
}
void StructureVisualizer::animateRemoveAtIndex(int widgetIndex, int modelIndex) {
    if (widgetIndex < 0 || widgetIndex >= m_cells.size()) {
        m_isAnimating = false;
        return;
    }

    CellWidget *cell = m_cells.at(widgetIndex);
    auto *group = new QSequentialAnimationGroup(this);

    QPropertyAnimation *highlight = new QPropertyAnimation(cell, "opacity");
    highlight->setDuration(200);
    highlight->setStartValue(1.0);
    highlight->setEndValue(0.5);
    cell->setStyleSheet("background-color: #ffcdd2; border: 1px solid #e53935;");

    QPropertyAnimation *fadeOut = new QPropertyAnimation(cell, "opacity");
    fadeOut->setDuration(300);
    fadeOut->setStartValue(0.5);
    fadeOut->setEndValue(0.0);

    group->addAnimation(highlight);
    group->addPause(100);
    group->addAnimation(fadeOut);

    QString currentType = m_currentType;

    connect(group, &QSequentialAnimationGroup::finished, this, [this, currentType, modelIndex]() {
        m_manager->removeElementAt(modelIndex);
        rebuildCells();
        m_isAnimating = false;
    });

    group->start(QAbstractAnimation::DeleteWhenStopped);
}
