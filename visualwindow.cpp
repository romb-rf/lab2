// visualizationwindow.cpp
#include "visualwindow.h"
#include <QVBoxLayout>

VisualizationWindow::VisualizationWindow(DataManager *manager, QWidget *parent)
    : QWidget(parent, Qt::Window)
{
    setWindowTitle("Memory Visualization");
    resize(800, 500);

    m_visualizer = new StructureVisualizer(manager, this);
    auto *layout = new QVBoxLayout(this);
    layout->addWidget(m_visualizer);
    setLayout(layout);
}

void VisualizationWindow::showWindow() { show(); raise(); }
void VisualizationWindow::setStructureType(const QString &type) { m_visualizer->setStructureType(type); }
void VisualizationWindow::insertElement(const QString &value) { m_visualizer->insertElement(value); }
void VisualizationWindow::removeLastElement() { m_visualizer->removeLastElement(); }
void VisualizationWindow::replaceElement(int index, const QString &newValue) { m_visualizer->replaceElement(index, newValue); }
void VisualizationWindow::clearAll() { m_visualizer->clearAll(); }
double VisualizationWindow::getMedian() const { return m_visualizer->getMedian(); }
void VisualizationWindow::cyclicShift(int positions) { m_visualizer->cyclicShift(positions); }
