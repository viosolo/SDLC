#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QString>

// =====================================================================
//  Активная модель: хранит состояние и САМА уведомляет подписчиков
//  (Представление) об изменениях через сигнал modelChanged().
// =====================================================================
class HappinessModel : public QObject
{
    Q_OBJECT
public:
    struct Data {
        double sleepHours         = 8.0;  // часов в сутки
        int    coffeeCups         = 2;    // чашек в сутки
        double communicationHours = 3.0;  // часов в сутки
        double workHours          = 8.0;  // часов в сутки
        int    weekendsPerWeek    = 2;    // дней в неделю
    };

    explicit HappinessModel(QObject *parent = nullptr);

    void setData(const Data &data);
    const Data &data() const { return m_data; }

    bool    hasData()        const { return m_hasData; }
    double  happinessIndex() const { return m_index; }
    QString recommendation() const { return m_recommendation; }

    // Статическая валидация входных данных (используется диалогом ввода)
    static bool validate(const Data &data, QString &error);

signals:
    // Ключевой признак активной модели: сигнал об изменении состояния
    void modelChanged();

private:
    void recalculate();

    Data    m_data;
    bool    m_hasData = false;
    double  m_index = 0.0;
    QString m_recommendation;
};

#endif // MODEL_H