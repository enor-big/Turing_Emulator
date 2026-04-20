#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStringList>

class QLineEdit;
class QPushButton;
class QTableWidget;
class QLabel;
class QWidget;

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
private:
    void setupUi();
    void updateProgramTable();
    QStringList parseAlphabet(const QString &text) const;
    void setControlsEnabledAfterAlphabet(bool enabled);
    bool isWordValidForTapeAlphabet(const QString &word) const;

    void resetMachineStateFromInput();
    void updateTapeView();
    int findColumnForSymbol(const QString &symbol) const;
    bool parseCommand(const QString &text,
                      QString &newSymbol,
                      QString &moveDir,
                      QString &newState) const;
private:
    QWidget *m_centralWidget;
    QLabel *m_tapeViewLabel;
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

    QStringList m_tapeAlphabet;
    QStringList m_extraAlphabet;
    QStringList m_tableAlphabet;
    const QString m_blankSymbol = "_";

    QString m_inputWord;

    QStringList m_tapeCells;
    int m_headPosition;
    QString m_currentState;
    bool m_isHalted;
};


#endif // MAINWINDOW_H
