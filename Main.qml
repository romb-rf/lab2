import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material

ApplicationWindow {
    id: root
    visible: true
    width: 1300
    height: 900
    title: "Структуры"
    color: "#f5f5f5"
    Material.theme: Material.Light
    Material.accent: Material.Indigo

    property var visWin: null

    // Фон
    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#e8eaf6" }
            GradientStop { position: 1.0; color: "#c5cae9" }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        // Заголовок и выбор структуры
        RowLayout {
            Layout.fillWidth: true
            spacing: 15

            Text {
                text: "Тип Структуры:"
                font.pixelSize: 18
                font.bold: true
                color: "#1a237e"
            }

            ComboBox {
                id: structureCombo
                model: ["Array", "Vector", "Stack", "Queue"]
                currentIndex: 1

                background: Rectangle {
                    radius: 8
                    color: "white"
                    border.color: "#3f51b5"
                    border.width: 1
                }
                contentItem: Text {
                    leftPadding: 12
                    rightPadding: 12
                    verticalAlignment: Text.AlignVCenter
                    text: structureCombo.currentText
                    font.pixelSize: 16
                    font.bold: true
                    color: "#1a237e"
                }
                indicator: Canvas {
                    width: 12
                    height: 8
                    contextType: "2d"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 12
                    onPaint: {
                        context.reset();
                        context.moveTo(0, 0);
                        context.lineTo(width, 0);
                        context.lineTo(width / 2, height);
                        context.closePath();
                        context.fillStyle = "#3f51b5";
                        context.fill();
                    }
                }
                onCurrentTextChanged: {
                    dataManager.setStructureType(currentText);
                    visualizer.setStructureType(currentText);
                }
            }

            Item { Layout.fillWidth: true }

            // текущий тип
            Rectangle {
                radius: 20
                color: "#e8eaf6"
                border.color: "#5c6bc0"
                width: currentTypeLabel.implicitWidth + 40
                height: 40
                Text {
                    id: currentTypeLabel
                    anchors.centerIn: parent
                    text: "Активный тип: " + dataManager.currentType
                    font.pixelSize: 16
                    font.bold: true
                    color: "#283593"
                }
            }
        }

        // основное поле
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 20

            // таблица
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredWidth: parent.width * 0.6
                radius: 12
                color: "white"
                border.color: "#c5cae9"
                border.width: 1
                layer.enabled: true
                layer.smooth: true
                layer.samples: 8

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 10
                    spacing: 0

                    Text {
                        text: "Элементы"
                        font.pixelSize: 18
                        font.bold: true
                        color: "#1a237e"
                        bottomPadding: 5
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        height: 30
                        color: "#e0e0e0"
                        radius: 4

                        Row {
                            anchors.fill: parent
                            Rectangle {
                                width: parent.width / 2
                                height: parent.height
                                color: "transparent"
                                Text {
                                    anchors.centerIn: parent
                                    text: "Индекс"
                                    font.bold: true
                                    font.pixelSize: 14
                                    color: "#1a237e"
                                }
                            }
                            Rectangle {
                                width: parent.width / 2
                                height: parent.height
                                color: "transparent"
                                Text {
                                    anchors.centerIn: parent
                                    text: "Значение"
                                    font.bold: true
                                    font.pixelSize: 14
                                    color: "#1a237e"
                                }
                            }
                        }
                    }

                    ListView {
                        id: listView
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        clip: true
                        spacing: 0
                        model: dataManager.elements
                        boundsBehavior: Flickable.StopAtBounds

                        delegate: Rectangle {
                            width: listView.width
                            height: 40
                            color: index % 2 ? "#f3e5f5" : "#e8eaf6"
                            border.color: "#e0e0e0"

                            Row {
                                anchors.fill: parent
                                Rectangle {
                                    width: parent.width / 2
                                    height: parent.height
                                    color: "transparent"
                                    Text {
                                        anchors.centerIn: parent
                                        text: "[" + index + "]"
                                        font.pixelSize: 16
                                        font.bold: true
                                        color: "#283593"
                                    }
                                }
                                Rectangle {
                                    id: valueCell
                                    width: parent.width / 2
                                    height: parent.height
                                    color: "transparent"
                                    Text {
                                        id: valueText
                                        anchors.centerIn: parent
                                        text: modelData
                                        font.pixelSize: 16
                                        color: "#1a1a1a"
                                        visible: !editBackground.visible
                                    }
                                    Rectangle {
                                        id: editBackground
                                        anchors.centerIn: parent
                                        width: editInput.implicitWidth + 12
                                        height: editInput.implicitHeight + 6
                                        color: editInput.activeFocus ? "#e3f2fd" : "transparent"
                                        border.color: editInput.activeFocus ? "#2196f3" : "transparent"
                                        radius: 4
                                        visible: false
                                        z: 9

                                        TextInput {
                                            id: editInput
                                            anchors.centerIn: parent
                                            font.pixelSize: 16
                                            color: "black"
                                            verticalAlignment: TextInput.AlignVCenter
                                            horizontalAlignment: TextInput.AlignHCenter
                                            maximumLength: 50
                                            clip: false
                                            onAccepted: {
                                                var newValue = text.trim();
                                                if (newValue !== modelData) {
                                                    visualizer.replaceElement(index, newValue);
                                                }
                                                editBackground.visible = false;
                                            }
                                        }
                                    }

                                    // красный слой
                                    Rectangle {
                                        id: errorOverlay
                                        anchors.fill: parent
                                        color: "#ffcdd2"
                                        border.color: "#b71c1c"
                                        border.width: 2
                                        visible: false
                                        opacity: 0.4
                                    }

                                    Timer {
                                        id: localErrorTimer
                                        interval: 3000
                                        onTriggered: {
                                            errorOverlay.visible = false;
                                        }
                                    }

                                    Connections {
                                        target: dataManager
                                        function onErrorOccurred(message) {
                                            if (editBackground.visible) {
                                                errorOverlay.visible = true;
                                                localErrorTimer.restart();
                                            }
                                        }
                                    }

                                    MouseArea {
                                        anchors.fill: parent
                                        onDoubleClicked: {
                                            editInput.text = modelData;
                                            editBackground.visible = true;
                                            editInput.focus = true;
                                            errorOverlay.visible = false;
                                        }
                                    }
                                }
                            }
                        }

                        //для пустой структуры
                        Rectangle {
                            anchors.fill: parent
                            color: "transparent"
                            visible: listView.count === 0
                            Text {
                                anchors.centerIn: parent
                                text: "Пока нет элементов:(\nДобавьте что-нибудь."
                                font.pixelSize: 16
                                color: "#9e9e9e"
                            }
                        }

                        ScrollBar.vertical: ScrollBar {
                            policy: ScrollBar.AsNeeded
                        }
                    }

                    Text {
                        text: "Количество элементов: " + dataManager.elementCount
                        font.pixelSize: 14
                        color: "#5c6bc0"
                        topPadding: 5
                    }
                }
            }

            // панель управления
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredWidth: parent.width * 0.4
                radius: 12
                color: "white"
                border.color: "#c5cae9"
                border.width: 1
                layer.enabled: true
                layer.smooth: true
                layer.samples: 8

                ScrollView {
                    anchors.fill: parent
                    anchors.margins: 15
                    clip: true

                    ColumnLayout {
                        width: parent.width
                        spacing: 20

                        GroupBox {
                            title: "Операции"
                            Layout.fillWidth: true
                            font.bold: true
                            background: Rectangle {
                                y: parent.topPadding - 10
                                height: parent.height - parent.topPadding
                                color: "transparent"
                                border.color: "#9fa8da"
                                radius: 8
                            }

                            ColumnLayout {
                                anchors.fill: parent
                                spacing: 10

                                RowLayout {
                                    TextField {
                                        id: insertField
                                        Layout.fillWidth: true
                                        placeholderText: "Значение"
                                        font.pixelSize: 14
                                    }
                                    Button {
                                        text: "Вставить"
                                        highlighted: true
                                        onClicked: visualizer.insertElement(insertField.text)
                                        Material.background: Material.Indigo
                                        contentItem: Text {
                                            text: parent.text
                                            font: parent.font
                                            color: "black"
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                            elide: Text.ElideRight
                                        }
                                    }
                                }
                                RowLayout {
                                            visible: dataManager.currentType === "Array" || dataManager.currentType === "Vector"
                                            TextField {
                                                id: insertAtIndexField
                                                Layout.fillWidth: true
                                                placeholderText: "Индекс"
                                                validator: IntValidator {}
                                            }
                                            TextField {
                                                id: insertAtIndexValue
                                                Layout.fillWidth: true
                                                placeholderText: "Значение"
                                            }
                                            Button {
                                                text: "Вставить по индексу"
                                                onClicked: visualizer.addElementAt(
                                                    parseInt(insertAtIndexField.text),
                                                    insertAtIndexValue.text
                                                )
                                                Material.background: Material.Indigo
                                                contentItem: Text {
                                                    text: parent.text
                                                    font: parent.font
                                                    color: "black"
                                                    horizontalAlignment: Text.AlignHCenter
                                                    verticalAlignment: Text.AlignVCenter
                                                    elide: Text.ElideRight
                                                }
                                            }
                                        }
                                RowLayout {
                                    TextField {
                                        id: indexField
                                        Layout.fillWidth: true
                                        placeholderText: "Индекс"
                                        validator: IntValidator {}
                                    }
                                    TextField {
                                        id: valueField
                                        Layout.fillWidth: true
                                        placeholderText: "Новое значение"
                                    }
                                    Button {
                                        text: "Заменить"
                                        onClicked: visualizer.replaceElement(
                                            parseInt(indexField.text),
                                            valueField.text
                                        )
                                        Material.background: Material.Amber
                                        contentItem: Text {
                                            text: parent.text
                                            font: parent.font
                                            color: "black"
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                            elide: Text.ElideRight
                                        }
                                    }
                                }
                                RowLayout {
                                    visible: dataManager.currentType === "Array" || dataManager.currentType === "Vector"
                                    TextField {
                                        id: removeAtIndexField
                                        Layout.fillWidth: true
                                        placeholderText: "Индекс"
                                        validator: IntValidator {}
                                    }
                                    Button {
                                        text: "Удалить по индексу"
                                        onClicked: visualizer.removeElementAt(parseInt(removeAtIndexField.text))
                                        Material.background: Material.Material.Red
                                        contentItem: Text {
                                            text: parent.text
                                            font: parent.font
                                            color: "black"
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                            elide: Text.ElideRight
                                        }
                                    }
                                }

                                Button {
                                    text: "Удалить последний элемент"
                                    Layout.fillWidth: true
                                    onClicked: visualizer.removeCorrectElement()
                                    Material.background: Material.Red
                                    contentItem: Text {
                                        text: parent.text
                                        font: parent.font
                                        color: "black"
                                        horizontalAlignment: Text.AlignHCenter
                                        verticalAlignment: Text.AlignVCenter
                                        elide: Text.ElideRight
                                    }
                                }

                            }
                        }

                        GroupBox {
                            title: "Дополнительные задания"
                            Layout.fillWidth: true
                            font.bold: true
                            background: Rectangle {
                                y: parent.topPadding - 10
                                height: parent.height - parent.topPadding
                                color: "transparent"
                                border.color: "#9fa8da"
                                radius: 8
                            }

                            ColumnLayout {
                                anchors.fill: parent
                                spacing: 10

                                RowLayout {
                                    Button {
                                        text: "Найти медиану"
                                        Layout.fillWidth: true
                                        highlighted: true
                                        onClicked: {
                                            var med = visualizer.getMedian();
                                            medianResult.text = med.toFixed(2);
                                        }
                                        Material.background: Material.Teal
                                        contentItem: Text {
                                            text: parent.text
                                            font: parent.font
                                            color: "black"
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                            elide: Text.ElideRight
                                        }
                                    }
                                    Rectangle {
                                        Layout.preferredWidth: 120
                                        Layout.fillHeight: true
                                        height: 40
                                        radius: 8
                                        color: "#e0f2f1"
                                        border.color: "#009688"
                                        Text {
                                            id: medianResult
                                            anchors.centerIn: parent
                                            text: "—"
                                            font.pixelSize: 16
                                            font.bold: true
                                            color: "#00695c"
                                            elide: Text.ElideRight
                                            maximumLineCount: 1
                                        }
                                    }
                                }


                                RowLayout {
                                    TextField {
                                        id: shiftField
                                        Layout.fillWidth: true
                                        placeholderText: "Сдвиг на:"
                                        validator: IntValidator {}
                                    }
                                    Button {
                                        text: "Циклический сдвиг"
                                        Layout.fillWidth: true
                                        onClicked: visualizer.cyclicShift(parseInt(shiftField.text))
                                        Material.background: Material.Purple
                                        contentItem: Text {
                                            text: parent.text
                                            font: parent.font
                                            color: "black"
                                            horizontalAlignment: Text.AlignHCenter
                                            verticalAlignment: Text.AlignVCenter
                                            elide: Text.ElideRight
                                        }
                                    }
                                }
                            }
                        }

                        Button {
                            text: "Очистить всё"
                            Layout.fillWidth: true
                            onClicked: visualizer.clearAll()
                            Material.background: Material.Grey
                            contentItem: Text {
                                text: parent.text
                                font: parent.font
                                color: "black"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                            }
                        }

                        Button {
                            text: "Визуализация"
                            Layout.fillWidth: true
                            highlighted: true
                            onClicked: visualizer.showWindow()
                            Material.background: Material.DeepPurple
                            contentItem: Text {
                                text: parent.text
                                font: parent.font
                                color: "white"
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                                elide: Text.ElideRight
                            }
                        }
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            height: 40
            radius: 8
            color: "#ffebee"
            border.color: "#ef9a9a"
            visible: errorText.text !== ""

            Text {
                id: errorText
                anchors.centerIn: parent
                font.pixelSize: 14
                color: "#c62828"
            }
        }
    }

    // обработчик ошибок
    Connections {
        target: dataManager
        function onErrorOccurred(message) {
            errorText.text = message;
            errorTimer.restart();
        }
    }

    Timer {
        id: errorTimer
        interval: 3000
        onTriggered: errorText.text = ""
    }
}
