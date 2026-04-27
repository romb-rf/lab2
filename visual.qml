import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Window {
    id: visWindow
    width: 700
    height: 300
    title: "Визуализация операций"
    color: "#fafafa"
    visible: false

    property string currentStructureType: dataManager.currentType
    property var elements: []          // текущий список элементов (как в таблице)
    property var oldElements: []      // для анимации замены/сдвига

    // Анимационные таймеры
    Timer {
        id: stepTimer
        interval: 300   // шаг анимации (мс)
        repeat: true
        property int currentStep: 0
        property int totalSteps: 0
        property string operationType: ""
        property var operationParams: []
        onTriggered: {
            if (currentStep >= totalSteps) {
                stop();
                finishAnimation();
                return;
            }
            animateStep(operationType, currentStep);
            currentStep++;
        }
    }

    // Модель визуализируемых элементов
    ListModel {
        id: visualModel
    }

    // Синхронизация элементов из DataManager (без анимации)
    function syncElements() {
        visualModel.clear();
        var elems = dataManager.elements;
        for (var i = 0; i < elems.length; i++) {
            visualModel.append({value: elems[i]});
        }
        elements = elems;
        oldElements = [];
    }

    // Запуск визуализации операции
    function visualize(operation, params) {
        if (!visWindow.visible) return; // не активно

        // Для сложных операций используем пошаговую анимацию
        if (operation === "shift") {
            startShiftAnimation(params[0]);
        } else if (operation === "replace") {
            startReplaceAnimation(params[0], params[1], params[2]);
        } else if (operation === "add") {
            animateAdd();
        } else if (operation === "remove") {
            animateRemove();
        } else if (operation === "clear") {
            syncElements(); // просто мгновенно
        }
    }

    // Анимированное добавление: новый элемент появляется справа
    function animateAdd() {
        syncElements();
        if (visualModel.count === 0) return;
        var lastIndex = visualModel.count - 1;
        // Временно скрываем последний элемент, потом покажем
        var item = listView.itemAtIndex(lastIndex);
        if (item) {
            item.opacity = 0;
            fadeIn.start(item);
        }
    }

    // Анимированное удаление: последний элемент исчезает
    function animateRemove() {
        if (visualModel.count === 0) return;
        var lastIndex = visualModel.count - 1;
        var item = listView.itemAtIndex(lastIndex);
        if (item) {
            item.opacity = 1;
            fadeOut.start(item);
            // после анимации синхронизируем модель
            fadeOut.onFinished = function() {
                syncElements();
                fadeOut.onFinished = null;
            }
        } else {
            syncElements();
        }
    }

    // Замена: старый элемент мигает, затем заменяется
    function startReplaceAnimation(index, oldVal, newVal) {
        syncElements(); // сначала показываем актуальные (уже новые) данные? Нет, мы хотим показать как меняется.
        // Сохраним старые элементы
        oldElements = elements.slice();
        // Временно вернем модель к старому значению
        visualModel.setProperty(index, "value", oldVal);
        // Запустим анимацию: сперва покажем старый (уже есть), потом через паузу заменим на новый
        var item = listView.itemAtIndex(index);
        if (item) {
            item.color = "lightcoral";
            pauseTimer.interval = 500;
            pauseTimer.onTriggered = function() {
                visualModel.setProperty(index, "value", newVal);
                if (item) item.color = "#e8eaf6"; // вернуть обычный цвет
                // восстанавливаем синхронизацию
                syncElements(); // окончательно
                pauseTimer.onTriggered = null;
            }
            pauseTimer.start();
        }
    }

    // Анимация циклического сдвига (пошаговая)
    function startShiftAnimation(positions) {
        var arr = elements.slice();
        if (arr.length === 0) return;
        stepTimer.operationType = "shift";
        stepTimer.operationParams = [arr, positions];
        stepTimer.totalSteps = Math.abs(positions);
        stepTimer.currentStep = 0;
        stepTimer.start();
        // Показываем начальное состояние
        syncElements();
    }

    // Один шаг сдвига (вправо)
    function animateStep(operation, step) {
        if (operation === "shift") {
            var arr = stepTimer.operationParams[0];
            // выполняем один сдвиг вправо
            var last = arr.pop();
            arr.unshift(last);
            stepTimer.operationParams[0] = arr;
            // обновляем модель
            visualModel.clear();
            for (var i = 0; i < arr.length; i++) {
                visualModel.append({value: arr[i]});
            }
        }
    }

    function finishAnimation() {
        syncElements(); // окончательно синхронизировать с DataManager
    }

    // Плавное появление
    PropertyAnimation { id: fadeIn; property: "opacity"; to: 1; duration: 300 }
    // Плавное исчезновение
    PropertyAnimation { id: fadeOut; property: "opacity"; to: 0; duration: 300 }
    // Пауза
    Timer { id: pauseTimer; interval: 500; }

    // Основной вид
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10

        Text {
            text: "Визуализация структуры: " + visWindow.currentStructureType
            font.pixelSize: 16
            font.bold: true
        }

        // Представление в виде горизонтального ряда прямоугольников
        Row {
            id: elementRow
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 5

            Repeater {
                model: visualModel
                delegate: Rectangle {
                    width: 60
                    height: 40
                    border.color: "#333"
                    color: "#e0f7fa"
                    radius: 4
                    Text {
                        anchors.centerIn: parent
                        text: model.value
                    }
                    Behavior on opacity { NumberAnimation { duration: 300 } }
                }
            }
        }
    }

    // Связь с DataManager
    Connections {
        target: dataManager
        function onOperationVisualized(operation, params) {
            visWindow.visualize(operation, params);
        }
    }

    onVisibleChanged: {
        if (visible) syncElements();
    }
}
