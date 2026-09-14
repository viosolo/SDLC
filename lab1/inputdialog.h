#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QDialog>
#include "model.h"

class QDoubleSpinBox;
class QSpinBox;

class InputDialog : public QDialog
{
    Q_OBJECT
public:
    explicit InputDialog(const HappinessModel::Data &initial,
                         QWidget *parent = nullptr);

    HappinessModel::Data data() const;

private slots:
    void onAccept();   // проверка данных перед закрытием

private:
    void buildUi();

    QDoubleSpinBox *m_sleep         = nullptr;
    QSpinBox       *m_coffee        = nullptr;
    QDoubleSpinBox *m_communication = nullptr;
    QDoubleSpinBox *m_work          = nullptr;
    QSpinBox       *m_weekends      = nullptr;
};

#endif // INPUTDIALOG_H