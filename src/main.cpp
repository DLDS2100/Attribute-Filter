#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 应用全局样式
    app.setStyleSheet(QStringLiteral(
        "QMainWindow { background-color: #FFFFFF; }"
        "QWidget { font-family: 'Microsoft YaHei', 'SimHei', sans-serif; }"
    ));

    MainWindow window;
    window.show();

    return app.exec();
}
