#include <QApplication>
#include <QMessageBox>

#include "MainWindow.h"
#include "DatabaseManager.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Car Fleet Management");
    app.setApplicationVersion("1.0");
    if (!DatabaseManager::getInstance()->isOpen()) {
        QMessageBox::critical(nullptr, "Error",
            "Could not open the database file.");
        return 1;
    }
    MainWindow window;
    window.show();
    return app.exec();
}
