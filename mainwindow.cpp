#include "mainwindow.h"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    m_centralWidget(nullptr),
    m_tapeViewLabel(nullptr),
    m_tapeAlphabetEdit(nullptr),
    m_extraAlphabetEdit(nullptr),
    m_setAlphabetsButton(nullptr),
    m_inputWordEdit(nullptr),
    m_setWordButton(nullptr),
    m_runButton(nullptr),
    m_stopButton(nullptr),
    m_stepButton(nullptr),
    m_resetButton(nullptr),
    m_fasterButton(nullptr),
    m_slowerButton(nullptr),
    m_programTable(nullptr)
{
    setupUi();
}
MainWindow::~MainWindow()
{
}
void MainWindow::setupUi()
{
    setWindowTitle("Эмулятор машины тьюринга");
    resize(1200, 800);

    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(m_centralWidget);
    QGroupBox *alphabetsGroup = new QGroupBox("Алфавиты", m_centralWidget);
    QHBoxLayout *alphabetsLayout = new QHBoxLayout(alphabetsGroup);

    QLabel *tapeAlphabetLabel = new QLabel("Алфавит ленты:", alphabetsGroup);
    m_tapeAlphabetEdit = new QLineEdit(alphabetsGroup);

    QLabel *extraAlphabetLabel = new QLabel("Доп. символы:", alphabetsGroup);
    m_extraAlphabetEdit = new QLineEdit(alphabetsGroup);

    m_setAlphabetsButton = new QPushButton("Задать алфавиты", alphabetsGroup);

    alphabetsLayout->addWidget(tapeAlphabetLabel);
    alphabetsLayout->addWidget(m_tapeAlphabetEdit, 1);
    alphabetsLayout->addWidget(extraAlphabetLabel);
    alphabetsLayout->addWidget(m_extraAlphabetEdit, 1);
    alphabetsLayout->addWidget(m_setAlphabetsButton);

    QGroupBox *wordGroup = new QGroupBox("Входная строка", m_centralWidget);
    QHBoxLayout *wordLayout = new QHBoxLayout(wordGroup);

    QLabel *inputWordLabel = new QLabel("Строка:", wordGroup);
    m_inputWordEdit = new QLineEdit(wordGroup);
    m_inputWordEdit->setPlaceholderText("Введите входное слово");
    m_setWordButton = new QPushButton("Задать строку", wordGroup);

    wordLayout->addWidget(inputWordLabel);
    wordLayout->addWidget(m_inputWordEdit, 1);
    wordLayout->addWidget(m_setWordButton);

    QGroupBox *controlsGroup = new QGroupBox("Управление", m_centralWidget);
    QHBoxLayout *controlsLayout = new QHBoxLayout(controlsGroup);

    m_runButton = new QPushButton("Запустить", controlsGroup);
    m_stopButton = new QPushButton("Остановить", controlsGroup);
    m_stepButton = new QPushButton("Один шаг", controlsGroup);
    m_resetButton = new QPushButton("Сброс", controlsGroup);
    m_fasterButton = new QPushButton("Быстрее", controlsGroup);
    m_slowerButton = new QPushButton("Медленнее", controlsGroup);

    controlsLayout->addWidget(m_runButton);
    controlsLayout->addWidget(m_stopButton);
    controlsLayout->addWidget(m_stepButton);
    controlsLayout->addWidget(m_resetButton);
    controlsLayout->addWidget(m_fasterButton);
    controlsLayout->addWidget(m_slowerButton);

    QGroupBox *tapeGroup = new QGroupBox("Лента", m_centralWidget);
    QVBoxLayout *tapeLayout = new QVBoxLayout(tapeGroup);

    m_tapeViewLabel = new QLabel("Здесь позже будет визуализация ленты и каретки", tapeGroup);
    m_tapeViewLabel->setMinimumHeight(120);
    m_tapeViewLabel->setAlignment(Qt::AlignCenter);
    m_tapeViewLabel->setStyleSheet(
        "QLabel {"
        "border: 1px solid gray;"
        "background-color: white;"
        "font-size: 16px;"
        "}"
        );

    tapeLayout->addWidget(m_tapeViewLabel);

    QGroupBox *programGroup = new QGroupBox("Программа машины", m_centralWidget);
    QVBoxLayout *programLayout = new QVBoxLayout(programGroup);

    m_programTable = new QTableWidget(programGroup);
    m_programTable->setColumnCount(5);
    m_programTable->setRowCount(5);

    QStringList headers;
    headers << "Состояние" << "0" << "1" << "_" << "Стоп";
    m_programTable->setHorizontalHeaderLabels(headers);

    m_programTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_programTable->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    programLayout->addWidget(m_programTable);

    mainLayout->addWidget(alphabetsGroup);
    mainLayout->addWidget(wordGroup);
    mainLayout->addWidget(controlsGroup);
    mainLayout->addWidget(tapeGroup);
    mainLayout->addWidget(programGroup, 1);
/*
*/

}


