#include "view.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>

MainWindow::MainWindow(HappinessModel *model, QWidget *parent)
    : QMainWindow(parent), m_model(model)
{
    buildUi();
    applyStyle();

    setWindowTitle(QStringLiteral("Калькулятор счастья"));
    resize(540, 620);

    // АКТИВНАЯ МОДЕЛЬ: представление подписывается на изменения модели
    connect(m_model, &HappinessModel::modelChanged,
            this,    &MainWindow::onModelChanged);
}

void MainWindow::buildUi()
{
    auto *central = new QWidget(this);
    setCentralWidget(central);

    auto *root = new QVBoxLayout(central);
    root->setContentsMargins(28, 28, 28, 28);
    root->setSpacing(16);

    // --- Заголовок ---
    m_titleLabel = new QLabel(QStringLiteral("🍑 Калькулятор счастья"), central);
    m_titleLabel->setObjectName("titleLabel");
    m_titleLabel->setAlignment(Qt::AlignCenter);

    m_subtitleLabel = new QLabel(
        QStringLiteral("Оцените свой уровень счастья по пяти факторам:\n"
                       "сон, кофе, общение, работа и выходные."), central);
    m_subtitleLabel->setObjectName("subtitleLabel");
    m_subtitleLabel->setAlignment(Qt::AlignCenter);
    m_subtitleLabel->setWordWrap(true);

    // --- Карточка с введёнными данными ---
    auto *inputsCard = new QFrame(central);
    inputsCard->setObjectName("card");
    auto *inputsLayout = new QVBoxLayout(inputsCard);
    inputsLayout->setContentsMargins(20, 18, 20, 18);
    inputsLayout->setSpacing(6);

    auto *inputsHeader = new QLabel(QStringLiteral("ВВЕДЁННЫЕ ДАННЫЕ"), inputsCard);
    inputsHeader->setObjectName("cardHeader");

    m_inputsValue = new QLabel(QStringLiteral("— данные ещё не введены —"), inputsCard);
    m_inputsValue->setObjectName("cardValue");
    m_inputsValue->setWordWrap(true);

    inputsLayout->addWidget(inputsHeader);
    inputsLayout->addWidget(m_inputsValue);

    // --- Карточка результата ---
    auto *resultCard = new QFrame(central);
    resultCard->setObjectName("card");
    auto *resultLayout = new QVBoxLayout(resultCard);
    resultLayout->setContentsMargins(20, 18, 20, 18);
    resultLayout->setSpacing(6);

    auto *resultHeader = new QLabel(QStringLiteral("ИНДЕКС СЧАСТЬЯ"), resultCard);
    resultHeader->setObjectName("cardHeader");

    m_indexValue = new QLabel(QStringLiteral("—"), resultCard);
    m_indexValue->setObjectName("indexValue");
    m_indexValue->setAlignment(Qt::AlignCenter);

    m_recommendationValue = new QLabel(
        QStringLiteral("Рекомендация появится после расчёта."), resultCard);
    m_recommendationValue->setObjectName("recommendation");
    m_recommendationValue->setAlignment(Qt::AlignCenter);
    m_recommendationValue->setWordWrap(true);

    resultLayout->addWidget(resultHeader);
    resultLayout->addWidget(m_indexValue);
    resultLayout->addWidget(m_recommendationValue);

    // --- Кнопка ---
    m_inputButton = new QPushButton(QStringLiteral("Ввести данные"), central);
    m_inputButton->setObjectName("primaryButton");
    m_inputButton->setCursor(Qt::PointingHandCursor);

    // --- Подсказка ---
    auto *hint = new QLabel(
        QStringLiteral("Последние введённые данные сохраняются "
                       "и подставляются при повторном вводе."), central);
    hint->setObjectName("hintLabel");
    hint->setAlignment(Qt::AlignCenter);
    hint->setWordWrap(true);

    root->addWidget(m_titleLabel);
    root->addWidget(m_subtitleLabel);
    root->addWidget(inputsCard);
    root->addWidget(resultCard);
    root->addSpacing(4);
    root->addWidget(m_inputButton);
    root->addWidget(hint);
    root->addStretch();

    // Событие кнопки — наружу через сигнал (обрабатывает контроллер)
    connect(m_inputButton, &QPushButton::clicked,
            this,           &MainWindow::inputRequested);
}

void MainWindow::applyStyle()
{
    setStyleSheet(R"(
        QMainWindow, QWidget {
            background-color: #FFF3E4;
            color: #4A3424;
            font-family: "Segoe UI", "Tahoma", sans-serif;
            font-size: 14px;
        }
        QLabel#titleLabel {
            font-size: 26px;
            font-weight: 700;
            color: #E0763A;
        }
        QLabel#subtitleLabel {
            color: #7A5A44;
            font-size: 13px;
        }
        QFrame#card {
            background-color: #FFE7CC;
            border: 1px solid #F5C9A0;
            border-radius: 14px;
        }
        QLabel#cardHeader {
            font-size: 11px;
            font-weight: 700;
            color: #B2612A;
            letter-spacing: 1px;
        }
        QLabel#cardValue {
            font-size: 14px;
            color: #4A3424;
        }
        QLabel#indexValue {
            font-size: 34px;
            font-weight: 800;
            color: #E0763A;
        }
        QLabel#recommendation {
            font-size: 16px;
            font-weight: 600;
            color: #8C4A1F;
        }
        QLabel#hintLabel {
            color: #9A7A63;
            font-size: 12px;
        }
        QPushButton#primaryButton {
            background-color: #FFB27A;
            color: #4A2310;
            border: none;
            border-radius: 12px;
            padding: 14px 20px;
            font-size: 16px;
            font-weight: 700;
        }
        QPushButton#primaryButton:hover    { background-color: #FFA15F; }
        QPushButton#primaryButton:pressed  { background-color: #E88B4A; }
    )");
}

void MainWindow::onModelChanged()
{
    const HappinessModel::Data d = m_model->data();

    // Запоминаем последние данные в главном окне
    m_lastData    = d;
    m_hasLastData = true;

    m_inputsValue->setText(QString(
                               "Сон:        %1 ч/сутки\n"
                               "Кофе:       %2 чаш./сутки\n"
                               "Общение:    %3 ч/сутки\n"
                               "Работа:     %4 ч/сутки\n"
                               "Выходные:   %5 дн./неделю")
                               .arg(d.sleepHours,         0, 'f', 1)
                               .arg(d.coffeeCups)
                               .arg(d.communicationHours, 0, 'f', 1)
                               .arg(d.workHours,          0, 'f', 1)
                               .arg(d.weekendsPerWeek));

    m_indexValue->setText(QString("%1 / 100")
                              .arg(m_model->happinessIndex(), 0, 'f', 1));

    m_recommendationValue->setText(QStringLiteral("💡 Рекомендация: ")
                                   + m_model->recommendation());
}