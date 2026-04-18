#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
private:
    void setupUi();
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
};


#endif // MAINWINDOW_H
