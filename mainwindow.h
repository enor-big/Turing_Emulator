#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>

class QLineEdit;
class QPushButton;
class QTableWidget;
class QLabel;
class QWidget;
class QTimer;
class TapeView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void onSetAlphabetClicked();
    void onSetWordClicked();
    void onStepClicked();
    void onResetClicked();
    void onRunClicked();
    void onStopClicked();
    void onTimerTimeout();
    void onFasterClicked();
    void onSlowerClicked();
    void onAddStateClicked();
    void onRemoveStateClicked();
private:
    void setupUi();
    void updateProgramTable();
    QStringList parseAlphabet(const QString &text) const;
    void setControlsEnabledAfterAlphabet(bool enabled);
    bool isWordValidForTapeAlphabet(const QString &word) const;
    QString findFirstInvalidWordSymbol(const QString &word) const;

    void resetMachineStateFromInput();
    void updateTapeView();
    int findColumnForSymbol(const QString &symbol) const;
    bool parseCommand(const QString &text,
                      QString &newSymbol,
                      QString &moveDir,
                      QString &newState) const;
    void setExecutionControlsRunning(bool running);

    void updateSpeedLabel();
    void updateStateHighLight();
    void refreshStateNames();
private:
    QWidget *m_centralWidget;
    TapeView *m_tapeView;
    QLabel *m_speedLabel;

    QLineEdit *m_tapeAlphabetEdit;
    QLineEdit *m_extraAlphabetEdit;
    QPushButton *m_setAlphabetsButton;
    QLineEdit *m_inputWordEdit;
    QPushButton *m_setWordButton;

    QPushButton *m_runButton;
    QPushButton *m_stopButton;
    QPushButton *m_stepButton;
    QPushButton *m_resetButton;
    QPushButton *m_fasterButton;
    QPushButton *m_slowerButton;

    QTableWidget *m_programTable;
    QPushButton *m_addStateButton;
    QPushButton *m_removeStateButton;

    QStringList m_tapeAlphabet;
    QStringList m_extraAlphabet;
    QStringList m_tableAlphabet;
    const QString m_blankSymbol = "_";

    QString m_inputWord;

    QStringList m_tapeCells;
    int m_headPosition;
    QString m_currentState;
    bool m_isHalted;
    QTimer *m_timer;
    int m_stepIntervalMs;

    bool m_prependedLeft;
    int m_stateCount;


};


#endif // MAINWINDOW_H
