#include "visualwindow.h"
#include <QVBoxLayout>

VisualizationWindow::VisualizationWindow(DataManager *manager, QWidget *parent)
    : QWidget(parent, Qt::Window)
{
    setWindowTitle("Визуализация");
    resize(800, 500);

    m_visualizer = new StructureVisualizer(manager, this);
    auto *layout = new QVBoxLayout(this);
    layout->addWidget(m_visualizer);
    setLayout(layout);
}

void VisualizationWindow::showWindow()
{
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    show();
    raise();
}
void VisualizationWindow::setStructureType(const QString &type) { m_visualizer->setStructureType(type); }
void VisualizationWindow::insertElement(const QString &value) { m_visualizer->insertElement(value); }
void VisualizationWindow::removeCorrectElement() {
    m_visualizer->removeCorrectElement();
}
void VisualizationWindow::replaceElement(int index, const QString &newValue) { m_visualizer->replaceElement(index, newValue); }
void VisualizationWindow::clearAll() { m_visualizer->clearAll(); }
double VisualizationWindow::getMedian() const { return m_visualizer->getMedian(); }
void VisualizationWindow::cyclicShift(int positions) { m_visualizer->cyclicShift(positions); }
void VisualizationWindow::addElementAt(int index, const QString& value) {
    m_visualizer->addElementAt(index, value);
}
void VisualizationWindow::removeElementAt(int index) {
    m_visualizer->removeElementAt(index);
}
