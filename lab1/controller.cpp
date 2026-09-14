#include "controller.h"
#include "model.h"
#include "view.h"
#include "inputdialog.h"

HappinessController::HappinessController(HappinessModel *model,
                                         MainWindow     *view,
                                         QObject        *parent)
    : QObject(parent), m_model(model), m_view(view)
{
    // Контроллер обрабатывает только пользовательское событие «Ввести данные»
    connect(m_view,  &MainWindow::inputRequested,
            this,    &HappinessController::onInputRequested);
    // Изменения модели напрямую транслируются в представление
    // (модель активная — View подписан на неё в своём конструкторе).
}

void HappinessController::onInputRequested()
{
    // Восстанавливаем последние данные из главного окна
    const HappinessModel::Data initial = m_view->hasLastData()
                                             ? m_view->lastData()
                                             : m_model->data();

    InputDialog dialog(initial, m_view);
    if (dialog.exec() == QDialog::Accepted) {

        m_model->setData(dialog.data());
    }
}