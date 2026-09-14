#include "model.h"
#include <QtMath>

HappinessModel::HappinessModel(QObject *parent) : QObject(parent) {}

bool HappinessModel::validate(const Data &data, QString &error)
{
    if (data.sleepHours < 0.0 || data.sleepHours > 24.0) {
        error = QStringLiteral("Сон должен быть в диапазоне 0–24 ч/сутки.");
        return false;
    }
    if (data.coffeeCups < 0 || data.coffeeCups > 50) {
        error = QStringLiteral("Кофе должен быть в диапазоне 0–50 чашек/сутки.");
        return false;
    }
    if (data.communicationHours < 0.0 || data.communicationHours > 24.0) {
        error = QStringLiteral("Общение должно быть в диапазоне 0–24 ч/сутки.");
        return false;
    }
    if (data.workHours < 0.0 || data.workHours > 24.0) {
        error = QStringLiteral("Работа должна быть в диапазоне 0–24 ч/сутки.");
        return false;
    }
    if (data.weekendsPerWeek < 0 || data.weekendsPerWeek > 7) {
        error = QStringLiteral("Выходных не может быть больше 7 дней в неделю.");
        return false;
    }
    // Суммарное время сна, общения и работы не должно превышать сутки
    if (data.sleepHours + data.communicationHours + data.workHours > 24.0) {
        error = QStringLiteral("Сумма часов сна, общения и работы "
                               "не может превышать 24 часа в сутки.");
        return false;
    }
    return true;
}

void HappinessModel::setData(const Data &data)
{
    m_data    = data;
    m_hasData = true;
    recalculate();
    // Активная модель: уведомляем подписчиков об изменении
    emit modelChanged();
}

void HappinessModel::recalculate()
{
    // ---- Оценка каждой привычки от 0 до 100 ----
    // 1) Сон: идеал 8 ч, штраф 12 баллов за час отклонения
    double sleepScore = 100.0 - qAbs(m_data.sleepHours - 8.0) * 12.0;

    // 2) Кофе: идеал 2 чашки; избыток штрафуется сильнее
    double coffeeDev   = m_data.coffeeCups - 2.0;
    double coffeeScore = 100.0 - (coffeeDev > 0 ? coffeeDev * 15.0
                                                : qAbs(coffeeDev) * 8.0);

    // 3) Общение: чем больше, тем лучше, плато после 4 ч/сутки
    double commScore = qMin(100.0, m_data.communicationHours * 25.0);

    // 4) Работа: идеал 6 ч
    double workScore = 100.0 - qAbs(m_data.workHours - 6.0) * 10.0;

    // 5) Выходные: идеал 2–3 дня
    double weekendScore;
    if (m_data.weekendsPerWeek >= 2 && m_data.weekendsPerWeek <= 3)
        weekendScore = 100.0;
    else if (m_data.weekendsPerWeek < 2)
        weekendScore = 100.0 - (2 - m_data.weekendsPerWeek) * 30.0;
    else
        weekendScore = 100.0 - (m_data.weekendsPerWeek - 3) * 10.0;

    sleepScore   = qBound(0.0, sleepScore,   100.0);
    coffeeScore  = qBound(0.0, coffeeScore,  100.0);
    commScore    = qBound(0.0, commScore,    100.0);
    workScore    = qBound(0.0, workScore,    100.0);
    weekendScore = qBound(0.0, weekendScore, 100.0);

    // ---- Итоговый индекс (взвешенная сумма) ----
    m_index = sleepScore   * 0.30
              + coffeeScore  * 0.10
              + commScore    * 0.20
              + workScore    * 0.25
              + weekendScore * 0.15;

    // ---- Рекомендация по самому слабому фактору ----
    const double scores[5] = { sleepScore, coffeeScore, commScore,
                              workScore,  weekendScore };
    int minIdx = 0;
    for (int i = 1; i < 5; ++i)
        if (scores[i] < scores[minIdx]) minIdx = i;

    switch (minIdx) {
    case 0:  m_recommendation = QStringLiteral("Поспать");        break;
    case 3:  m_recommendation = QStringLiteral("Уволиться");      break;
    case 4:  m_recommendation = QStringLiteral("Уволиться");      break;
    default: m_recommendation = QStringLiteral("Купить пиццу");   break;
    }
}