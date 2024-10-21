import QtQuick
import QtQuick.Window
import QtCharts
import bk

Window {
    width: 1500
    height: 1000
    visible: true

    ChartView {
        id: myChart
        width: 200
        height: 200
        margins.left: 0
        legend.visible: false

        ValueAxis {
            id: valueAxisX
            min: -1
            max: 14
            tickCount: max - min + 1
            minorTickCount: 1
            labelFormat: "%.0f"
        }

        ValueAxis {
            id: valueAxisY
            min: 0
            max: 14
            tickCount: max - min + 1
            labelsVisible: false
        }

        LineSeries {
            name: "Line"
            XYPoint {
                x: 0
                y: 0
            }
            XYPoint {
                x: 1.1
                y: 2.1
            }
            XYPoint {
                x: 1.9
                y: 3.3
            }
            XYPoint {
                x: 2.1
                y: 2.1
            }
            XYPoint {
                x: 2.9
                y: 4.9
            }
            XYPoint {
                x: 3.4
                y: 3.0
            }
            XYPoint {
                x: 4.1
                y: 3.3
            }
        }
    }
    WidgetView {
        id: qmlwrap
        width: 800
        height: 800
        anchors.top: myChart.bottom
        anchors.left: myChart.right
    }
    /*
    Button {
        id: button
        width: 200
        height: 200
        anchors.top: myChart.bottom
        text: qsTr("Click Me!")
        onClicked: {
            backend.setObject(button);
        }
    }
    */
}
