#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

class HappinessModel;
class MainWindow;

class HappinessController : public QObject
{
    Q_OBJECT
public:
    HappinessController(HappinessModel *model,
                        MainWindow     *view,
                        QObject        *parent = nullptr);

private slots:
    void onInputRequested();   // открыть диалог ввода

private:
    HappinessModel *m_model = nullptr;
    MainWindow     *m_view  = nullptr;
};

#endif // CONTROLLER_H