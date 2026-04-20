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
#include <QMessageBox>

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
    m_programTable(nullptr),
    m_headPosition(0),
    m_currentState("q0"),
    m_isHalted(false)

{
    setupUi();
    setControlsEnabledAfterAlphabet(false);

    connect(m_setAlphabetsButton, &QPushButton::clicked,this,
            &MainWindow::onSetAlphabetClicked);
    connect(m_setWordButton, &QPushButton::clicked, this,
            &MainWindow::onSetWordClicked);
    connect(m_stepButton, &QPushButton::clicked, this,
            &MainWindow::onStepClicked);

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
    m_tapeAlphabetEdit->setPlaceholderText("01_");

    QLabel *extraAlphabetLabel = new QLabel("Доп. символы:", alphabetsGroup);
    m_extraAlphabetEdit = new QLineEdit(alphabetsGroup);
    m_extraAlphabetEdit->setPlaceholderText("*#");

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

    m_tapeViewLabel = new QLabel("Сначала задайте алфавит, потом строку", tapeGroup);
    m_tapeViewLabel->setMinimumHeight(120);
    m_tapeViewLabel->setAlignment(Qt::AlignCenter);
    m_tapeViewLabel->setStyleSheet(
        "QLabel {"
        "border: 1px solid gray;"
        "background-color: white;"
        "color: black;"
        "font-size: 16px;"
        "}"
        );

    tapeLayout->addWidget(m_tapeViewLabel);

    QGroupBox *programGroup = new QGroupBox("Программа машины", m_centralWidget);
    QVBoxLayout *programLayout = new QVBoxLayout(programGroup);
/*
    m_programTable = new QTableWidget(programGroup);
    m_programTable->setColumnCount(5);
    m_programTable->setRowCount(5);

    QStringList headers;
    headers << "Состояние" << "0" << "1" << "_" << "Стоп";
    m_programTable->setHorizontalHeaderLabels(headers);
*/
    m_programTable = new QTableWidget(programGroup);
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
QStringList MainWindow::parseAlphabet(const QString &text) const{
    QStringList result;
    const QString trimmedText = text.trimmed();

    for (const QChar &ch : trimmedText){
        if(ch.isSpace()){
            continue;
        }
        const QString symbol(ch);
        if (!result.contains(symbol)){
            result<<symbol;
        }
    }
    return result;
}

void MainWindow::setControlsEnabledAfterAlphabet(bool enabled){
    m_inputWordEdit->setEnabled(enabled);
    m_setWordButton->setEnabled(enabled);
    m_programTable->setEnabled(enabled);
}

bool MainWindow::isWordValidForTapeAlphabet(const QString &word) const{
    if (word.isEmpty()){
        return false;
    }

    for (const QChar &ch :word){
        const QString symbol(ch);
        if (!m_tapeAlphabet.contains(symbol)){
            return false;
        }
    }
    return true;
}

void MainWindow::onSetAlphabetClicked(){
    const QString tapeText = m_tapeAlphabetEdit->text().trimmed();
    const QString extraText = m_extraAlphabetEdit->text().trimmed();

    if (tapeText.isEmpty()) {
        QMessageBox::warning(this,
                             "Ошибка",
                             "Алфавит не должен быть пустым.");
        return;
    }

    if (tapeText.contains(' ')) {
        QMessageBox::warning(this,
                             "Ошибка",
                             "Алфавит нужно вводить без пробелов. Введите заново.");
        return;
    }

    if (!extraText.isEmpty() && extraText.contains(' ')) {
        QMessageBox::warning(this,
                             "Ошибка",
                             "Доп символы нужно вводить без пробелов. Введите заново.");
        return;
    }

    m_tapeAlphabet = parseAlphabet(tapeText);
    m_extraAlphabet = parseAlphabet(extraText);
    m_tableAlphabet = m_tapeAlphabet;

    for(const QString &symbol : m_extraAlphabet){
        if(!m_tableAlphabet.contains(symbol)){
            m_tableAlphabet <<symbol;
        }
    }

    updateProgramTable();
    setControlsEnabledAfterAlphabet(true);

    m_tapeViewLabel->setText("Алфавиты заданы. Теперь введите строку.");
}

void MainWindow::onSetWordClicked()
{
    const QString word = m_inputWordEdit->text().trimmed();

    if (word.isEmpty()) {
        QMessageBox::warning(this,
                             "Ошибка",
                             "Входная строка не должна быть пустой.");
        return;
    }

    if (word.contains(' ')) {
        QMessageBox::warning(this,
                             "Ошибка",
                             "Строку нужно вводить без пробелов.");
        return;
    }

    if (!isWordValidForTapeAlphabet(word)) {
        QMessageBox::warning(this,
                             "Ошибка",
                             "Строка содержит символы, которых нет в алфавите ленты.");
        return;
    }

    m_inputWord = word;
    resetMachineStateFromInput();
    updateTapeView();
}

void MainWindow::updateProgramTable(){
    const int stateCount = 5;
    const int columnCount = 1+m_tableAlphabet.size();

    m_programTable->clear();
    m_programTable->setRowCount(stateCount);
    m_programTable->setColumnCount(columnCount);

    QStringList headers;
    headers << "Состояние";
    headers << m_tableAlphabet;

    m_programTable->setHorizontalHeaderLabels(headers);

    for(int row =0; row < stateCount;++row){
        QTableWidgetItem *stateItem = new QTableWidgetItem(QString("q%1").arg(row));
        m_programTable-> setItem(row, 0, stateItem);
    }
}

void MainWindow::resetMachineStateFromInput(){
    m_tapeCells.clear();
    for (const QChar &ch : m_inputWord){
        m_tapeCells<<QString(ch);
    }
    m_headPosition=0;
    m_currentState="q0";
    m_isHalted=false;
}

void MainWindow::updateTapeView(){
    if (m_tapeCells.empty()){
        m_tapeViewLabel->setText("Лента пуста");
        return;
    }
    QString tapeText;
    QString headText;

    for (int i = 0;i < m_tapeCells.size();++i){
        tapeText+= "[" +m_tapeCells[i] +"] ";

        if (i ==m_headPosition){
            headText+=" ^  ";
        } else{
            headText+="    ";
        }

        const QString fullText =
            QString("Состояние: %1\n%2\n%3")
                .arg(m_currentState)
                .arg(tapeText.trimmed())
                .arg(headText);

        m_tapeViewLabel->setText(fullText);
    }
}

int MainWindow::findColumnForSymbol(const QString &symbol) const
{
    for (int i = 0; i < m_tableAlphabet.size(); ++i) {
        if (m_tableAlphabet[i] == symbol) {
            return i + 1;
        }
    }

    return -1;
}

bool MainWindow::parseCommand(const QString &text,
                              QString &newSymbol,
                              QString &moveDir,
                              QString &newState) const
{
    const QString trimmed = text.trimmed();
    const QStringList parts = trimmed.split(',', Qt::SkipEmptyParts);

    if(parts.size() !=3){
        return false;
    }
    newSymbol = parts[0].trimmed();
    moveDir = parts[1].trimmed().toUpper();
    newState = parts[2].trimmed();

    if (newSymbol.size() != 1) {
        return false;
    }

    if (moveDir != "L" && moveDir != "R" && moveDir != "S") {
        return false;
    }

    if (newState.isEmpty()) {
        return false;
    }

    return true;
}

void MainWindow::onStepClicked()
{
    if (m_inputWord.isEmpty()) {
        QMessageBox::warning(this,
                             "Ошибка",
                             "Сначала задайте входную строку.");
        return;
    }

    if (m_isHalted) {
        QMessageBox::information(this,
                                 "Остановка",
                                 "Машина уже остановлена.");
        return;
    }

    if (m_headPosition < 0 || m_headPosition >= m_tapeCells.size()) {
        QMessageBox::warning(this,
                             "Ошибка",
                             "Головка вышла за границы ленты.");
        return;
    }

    bool ok = false;
    int row = -1;

    if (m_currentState.startsWith('q')) {
        const QString numberPart = m_currentState.mid(1);
        row = numberPart.toInt(&ok);
    }

    if (!ok || row < 0 || row >= m_programTable->rowCount()) {
        QMessageBox::warning(this,
                             "Ошибка",
                             "Текущее состояние не найдено в таблице.");
        return;
    }

    const QString currentSymbol = m_tapeCells[m_headPosition];
    const int column = findColumnForSymbol(currentSymbol);

    if (column == -1) {
        QMessageBox::warning(this,
                             "Ошибка",
                             "Для текущего символа нет столбца в таблице.");
        return;
    }

    QTableWidgetItem *item = m_programTable->item(row, column);

    if (!item || item->text().trimmed().isEmpty()) {
        QMessageBox::information(this,
                                 "Остановка",
                                 "Команда не задана. Машина остановлена.");
        m_isHalted = true;
        return;
    }

    QString newSymbol;
    QString moveDir;
    QString newState;

    if (!parseCommand(item->text(), newSymbol, moveDir, newState)) {
        QMessageBox::warning(this,
                             "Ошибка",
                             "Неверный формат команды.\nИспользуй формат: символ,направление,состояние");
        return;
    }

    m_tapeCells[m_headPosition] = newSymbol;

    if (moveDir == "L") {
        if (m_headPosition > 0) {
            --m_headPosition;
        }
    } else if (moveDir == "R") {
        if (m_headPosition + 1 < m_tapeCells.size()) {
            ++m_headPosition;
        }
    }

    if (newState.toUpper() == "HALT") {
        m_currentState = "HALT";
        m_isHalted = true;
    } else {
        m_currentState = newState;
    }

    updateTapeView();
}

