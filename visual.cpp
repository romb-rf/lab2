#include "visual.h"
#include "cellwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

// Вспомогательная функция: размер в байтах для QVariant
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
    : QWidget(parent), m_manager(manager)
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

    connect(m_manager, &DataManager::structureChanged,
            this, &StructureVisualizer::rebuildCells);
    connect(m_manager, &DataManager::elementsChanged,
            this, &StructureVisualizer::rebuildCells);
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
    qDeleteAll(m_cells);
    m_cells.clear();

    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != nullptr)
        delete child;

    if (type == "Array" || type == "Vector") {
        delete m_container->layout();
        m_layout = new QHBoxLayout(m_container);
        m_layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    } else {
        delete m_container->layout();
        m_layout = new QVBoxLayout(m_container);
        m_layout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    }
    m_container->setLayout(m_layout);
    rebuildCells();
}

void StructureVisualizer::rebuildCells()
{
    qDeleteAll(m_cells);
    m_cells.clear();

    QLayoutItem *child;
    while ((child = m_layout->takeAt(0)) != nullptr)
        delete child;

    const QVariantList elems = m_manager->elements();
    int currentOffset = 0;

    for (const QVariant &var : elems) {
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

void StructureVisualizer::insertElement(const QString &value)
{
    const int byteSize = m_manager->getElementSize(value);
    animateInsert(value, byteSize);
}

void StructureVisualizer::animateInsert(const QString &value, int byteSize)
{
    const int currentEnd = totalSizeUpTo(m_cells.size());

    CellWidget *cell = new CellWidget(generateAddress(currentEnd),
                                      value,
                                      byteSize,
                                      m_container);
    cell->setOpacity(0.0);
    m_layout->addWidget(cell);
    m_cells.append(cell);

    auto *group = new QSequentialAnimationGroup(this);

    QPropertyAnimation *allocAnim = new QPropertyAnimation(cell, "opacity");
    allocAnim->setDuration(300);
    allocAnim->setStartValue(0.0);
    allocAnim->setEndValue(0.8);
    group->addAnimation(allocAnim);
    group->addPause(200);

    QPropertyAnimation *writeAnim = new QPropertyAnimation(cell, "opacity");
    writeAnim->setDuration(300);
    writeAnim->setStartValue(0.8);
    writeAnim->setEndValue(1.0);
    group->addAnimation(writeAnim);

    connect(group, &QSequentialAnimationGroup::finished, this, [this, value]() {
        m_manager->addElement(value);
        rebuildCells();
    });

    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void StructureVisualizer::removeLastElement()
{
    if (m_cells.isEmpty()) return;

    CellWidget *cell = m_cells.last();
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

    connect(group, &QSequentialAnimationGroup::finished, this, [this]() {
        m_manager->removeElement();
        rebuildCells();
    });

    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void StructureVisualizer::replaceElement(int index, const QString &newValue)
{
    if (index < 0 || index >= m_cells.size()) return;

    CellWidget *cell = m_cells.at(index);
    auto *group = new QSequentialAnimationGroup(this);

    QPropertyAnimation *blink = new QPropertyAnimation(cell, "opacity");
    blink->setDuration(150);
    blink->setStartValue(1.0);
    blink->setEndValue(0.2);
    blink->setLoopCount(2);

    group->addAnimation(blink);
    group->addPause(100);

    connect(group, &QSequentialAnimationGroup::finished, this, [this, index, newValue]() {
        m_manager->replaceElement(index, newValue);
        rebuildCells();
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
