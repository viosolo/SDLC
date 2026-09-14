#include <QApplication>
#include "model.h"
#include "view.h"
#include "controller.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("Калькулятор счастья"));

    HappinessModel model;
    MainWindow view(&model);
    HappinessController controller(&model, &view);

    view.show();
    return app.exec();
}