#include "inputdialog.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QMessageBox>

InputDialog::InputDialog(const HappinessModel::Data &initial, QWidget *parent)
    : QDialog(parent)
{
    buildUi();

    m_sleep->setValue(initial.sleepHours);
    m_coffee->setValue(initial.coffeeCups);
    m_communication->setValue(initial.communicationHours);
    m_work->setValue(initial.workHours);
    m_weekends->setValue(initial.weekendsPerWeek);

    setWindowTitle(QStringLiteral("Ввод данных"));
    setModal(true);
    setMinimumWidth(430);
}

void InputDialog::buildUi()
{
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(24, 24, 24, 24);
    root->setSpacing(14);

    auto *title = new QLabel(QStringLiteral("Ваши привычки"), this);
    title->setObjectName("dialogTitle");

    auto *subtitle = new QLabel(
        QStringLiteral("Заполните поля и нажмите «Рассчитать»."), this);
    subtitle->setObjectName("dialogSubtitle");
    subtitle->setWordWrap(true);

    root->addWidget(title);
    root->addWidget(subtitle);

    auto *form = new QFormLayout();
    form->setLabelAlignment(Qt::AlignLeft);
    form->setFormAlignment(Qt::AlignLeft | Qt::AlignTop);
    form->setHorizontalSpacing(18);
    form->setVerticalSpacing(12);

    m_sleep = new QDoubleSpinBox(this);
    m_sleep->setRange(0.0, 24.0);
    m_sleep->setDecimals(1);
    m_sleep->setSingleStep(0.5);
    m_sleep->setSuffix(QStringLiteral(" ч/сутки"));

    m_coffee = new QSpinBox(this);
    m_coffee->setRange(0, 50);
    m_coffee->setSuffix(QStringLiteral(" чаш./сутки"));

    m_communication = new QDoubleSpinBox(this);
    m_communication->setRange(0.0, 24.0);
    m_communication->setDecimals(1);
    m_communication->setSingleStep(0.5);
    m_communication->setSuffix(QStringLiteral(" ч/сутки"));

    m_work = new QDoubleSpinBox(this);
    m_work->setRange(0.0, 24.0);
    m_work->setDecimals(1);
    m_work->setSingleStep(0.5);
    m_work->setSuffix(QStringLiteral(" ч/сутки"));

    m_weekends = new QSpinBox(this);
    m_weekends->setRange(0, 7);
    m_weekends->setSuffix(QStringLiteral(" дн./неделю"));

    form->addRow(QStringLiteral("Сон:"),      m_sleep);
    form->addRow(QStringLiteral("Кофе:"),     m_coffee);
    form->addRow(QStringLiteral("Общение:"),  m_communication);
    form->addRow(QStringLiteral("Работа:"),   m_work);
    form->addRow(QStringLiteral("Выходные:"), m_weekends);

    root->addLayout(form);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok
                                             | QDialogButtonBox::Cancel, this);
    buttons->button(QDialogButtonBox::Ok)->setText(QStringLiteral("Рассчитать"));
    buttons->button(QDialogButtonBox::Cancel)->setText(QStringLiteral("Отмена"));
    buttons->button(QDialogButtonBox::Ok)->setObjectName("okButton");
    buttons->button(QDialogButtonBox::Cancel)->setObjectName("cancelButton");

    connect(buttons, &QDialogButtonBox::accepted, this, &InputDialog::onAccept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    root->addWidget(buttons);

    setStyleSheet(R"(
        QDialog {
            background-color: #FFF3E4;
            color: #4A3424;
            font-family: "Segoe UI", "Tahoma", sans-serif;
            font-size: 14px;
        }
        QLabel { color: #4A3424; }
        QLabel#dialogTitle {
            font-size: 20px;
            font-weight: 700;
            color: #E0763A;
        }
        QLabel#dialogSubtitle {
            color: #7A5A44;
            font-size: 13px;
        }
        QDoubleSpinBox, QSpinBox {
            background-color: #FFE7CC;
            border: 1px solid #F5C9A0;
            border-radius: 8px;
            padding: 6px 10px;
            min-width: 170px;
            color: #4A3424;
        }
        QDoubleSpinBox:focus, QSpinBox:focus {
            border: 2px solid #FFB27A;
        }
        QPushButton#okButton {
            background-color: #FFB27A;
            color: #4A2310;
            border: none;
            border-radius: 10px;
            padding: 10px 20px;
            font-weight: 700;
            min-width: 120px;
        }
        QPushButton#okButton:hover    { background-color: #FFA15F; }
        QPushButton#cancelButton {
            background-color: #FFE0C2;
            color: #8C4A1F;
            border: 1px solid #F5C9A0;
            border-radius: 10px;
            padding: 10px 20px;
            font-weight: 600;
            min-width: 90px;
        }
        QPushButton#cancelButton:hover { background-color: #FFD2A8; }
    )");
}

HappinessModel::Data InputDialog::data() const
{
    HappinessModel::Data d;
    d.sleepHours         = m_sleep->value();
    d.coffeeCups         = m_coffee->value();
    d.communicationHours = m_communication->value();
    d.workHours          = m_work->value();
    d.weekendsPerWeek    = m_weekends->value();
    return d;
}

void InputDialog::onAccept()
{
    HappinessModel::Data d = data();
    QString error;
    if (!HappinessModel::validate(d, error)) {
        QMessageBox::warning(this,
                             QStringLiteral("Некорректные данные"),
                             error);
        return;
    }
    accept();
}