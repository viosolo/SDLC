#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include "model.h"

class QLabel;
class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(HappinessModel *model, QWidget *parent = nullptr);

    // Храним последние введённые данные в главном окне
    HappinessModel::Data lastData()    const { return m_lastData; }
    bool                 hasLastData() const { return m_hasLastData; }

signals:
    void inputRequested();   // кнопка «Ввести данные»

private slots:
    void onModelChanged();   // реакция на активную модель

private:
    void buildUi();
    void applyStyle();

    HappinessModel *m_model = nullptr;

    QLabel      *m_titleLabel         = nullptr;
    QLabel      *m_subtitleLabel      = nullptr;
    QLabel      *m_inputsValue        = nullptr;
    QLabel      *m_indexValue         = nullptr;
    QLabel      *m_recommendationValue= nullptr;
    QPushButton *m_inputButton        = nullptr;

    HappinessModel::Data m_lastData;
    bool                 m_hasLastData = false;
};

#endif // VIEW_H