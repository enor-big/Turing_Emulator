#include "mainwindow.h"
#include "tapeview.h"

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
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    m_centralWidget(nullptr),
    m_tapeView(nullptr),
    m_speedLabel(nullptr),
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
    m_addStateButton(nullptr),
    m_removeStateButton(nullptr),
    m_headPosition(0),
    m_currentState("q0"),
    m_isHalted(false),
    m_timer(nullptr),
    m_stepIntervalMs(700),
    m_prependedLeft(false),
    m_stateCount(1)

{
    setupUi();
    setControlsEnabledAfterAlphabet(false);

    connect(m_setAlphabetsButton, &QPushButton::clicked,this,
            &MainWindow::onSetAlphabetClicked);
    connect(m_setWordButton, &QPushButton::clicked, this,
            &MainWindow::onSetWordClicked);
    connect(m_stepButton, &QPushButton::clicked, this,
            &MainWindow::onStepClicked);
    connect(m_resetButton, &QPushButton::clicked, this,
            &MainWindow::onResetClicked);

    m_timer = new QTimer(this);
    connect(m_runButton, &QPushButton::clicked, this,
            &MainWindow::onRunClicked);
    connect(m_stopButton, &QPushButton::clicked, this,
            &MainWindow::onStopClicked);
    connect(m_fasterButton, &QPushButton::clicked, this,
            &MainWindow::onFasterClicked);
    connect(m_slowerButton, &QPushButton::clicked, this,
            &MainWindow::onSlowerClicked);
    connect(m_timer, &QTimer::timeout, this,
            &MainWindow::onTimerTimeout);

    connect(m_addStateButton, &QPushButton::clicked, this,
            &MainWindow::onAddStateClicked);
    connect(m_removeStateButton, &QPushButton::clicked, this,
            &MainWindow::onRemoveStateClicked);


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

    m_speedLabel = new QLabel(controlsGroup);
    controlsLayout ->addWidget(m_speedLabel);

    QGroupBox *tapeGroup = new QGroupBox("Лента", m_centralWidget);
    QVBoxLayout *tapeLayout = new QVBoxLayout(tapeGroup);

    m_tapeView = new TapeView(tapeGroup);
    tapeLayout->addWidget(m_tapeView);

    QGroupBox *programGroup = new QGroupBox("Программа машины", m_centralWidget);
    QVBoxLayout *programLayout = new QVBoxLayout(programGroup);

    QHBoxLayout *stateButtonsLayout = new QHBoxLayout();

    m_addStateButton = new QPushButton("Добавить состояние", programGroup);
    m_removeStateButton = new QPushButton("Удалить состояние", programGroup);

    stateButtonsLayout->addWidget(m_addStateButton);
    stateButtonsLayout->addWidget(m_removeStateButton);
    stateButtonsLayout->addStretch();
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
    programLayout->addLayout(stateButtonsLayout);

    mainLayout->addWidget(alphabetsGroup);
    mainLayout->addWidget(wordGroup);
    mainLayout->addWidget(controlsGroup);
    mainLayout->addWidget(tapeGroup);
    mainLayout->addWidget(programGroup, 1);
/*
*/
    updateSpeedLabel();

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
    m_addStateButton->setEnabled(enabled);
    m_removeStateButton->setEnabled(enabled);
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

    if (tapeText.contains(m_blankSymbol)) {
        QMessageBox::warning(this,
                             "Ошибка",
                             "Символ '_' является специальным символом пустоты и не может входить в алфавит ленты.");
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

    if (extraText.contains(m_blankSymbol)) {
        QMessageBox::warning(this,
                             "Ошибка",
                             "Символ '_' является специальным символом пустоты и не может входить в дополнительные символы.");
        return;
    }

    m_tapeAlphabet = parseAlphabet(tapeText);
    m_extraAlphabet = parseAlphabet(extraText);
    m_tableAlphabet = m_tapeAlphabet;

    if (!m_tableAlphabet.contains((m_blankSymbol))){
        m_tableAlphabet <<m_blankSymbol;
    }

    for(const QString &symbol : m_extraAlphabet){
        if(!m_tableAlphabet.contains(symbol)){
            m_tableAlphabet <<symbol;
        }
    }

    updateProgramTable();
    setControlsEnabledAfterAlphabet(true);

    m_tapeCells.clear();
    m_currentState = "q0";
    m_headPosition = 0;
    updateTapeView();
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

QString MainWindow::findFirstInvalidWordSymbol(const QString &word) const
{
    for (const QChar &ch : word) {
        const QString symbol(ch);

        if (symbol == m_blankSymbol) {
            return symbol;
        }

        if (!m_tapeAlphabet.contains(symbol)) {
            return symbol;
        }
    }

    return QString();
}

void MainWindow::updateProgramTable(){
    const int columnCount = 1+m_tableAlphabet.size();

    m_programTable->clear();
    m_programTable->setRowCount(m_stateCount);
    m_programTable->setColumnCount(columnCount);

    QStringList headers;
    headers << "Состояние";
    headers << m_tableAlphabet;

    m_programTable->setHorizontalHeaderLabels(headers);

    refreshStateNames();
    updateStateHighLight();
}

void MainWindow::refreshStateNames()
{
    for (int row = 0; row < m_stateCount; ++row) {
        QTableWidgetItem *stateItem = m_programTable->item(row, 0);
        if (!stateItem) {
            stateItem = new QTableWidgetItem();
            m_programTable->setItem(row, 0, stateItem);
        }

        stateItem->setText(QString("q%1").arg(row));
    }
}

void MainWindow::onAddStateClicked()
{
    ++m_stateCount;
    updateProgramTable();
}

void MainWindow::onRemoveStateClicked()
{
    if (m_stateCount <= 1) {
        QMessageBox::information(this,
                                 "Информация",
                                 "Нельзя удалить последнее состояние.");
        return;
    }

    --m_stateCount;

    bool ok = false;
    int currentStateNumber = -1;

    if (m_currentState.startsWith('q')) {
        currentStateNumber = m_currentState.mid(1).toInt(&ok);
    }

    if (ok && currentStateNumber >= m_stateCount) {
        m_currentState = QString("q%1").arg(m_stateCount - 1);
    }

    updateProgramTable();
    updateTapeView();
}
void MainWindow::resetMachineStateFromInput(){
    m_tapeCells.clear();
    for (const QChar &ch : m_inputWord){
        m_tapeCells<<QString(ch);
    }
    m_headPosition=0;
    m_currentState="q0";
    m_isHalted=false;
    updateStateHighLight();
}

void MainWindow::updateTapeView(){
    if (!m_tapeView) {
        return;
    }

    m_tapeView->setTape(m_tapeCells,
                        m_headPosition,
                        m_currentState,
                        m_blankSymbol,
                        m_prependedLeft);
    qDebug() << "head =" << m_headPosition << "state =" << m_currentState;

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

    if (trimmed.isEmpty()) {
        return false;
    }

    const QStringList parts = trimmed.split(',', Qt::KeepEmptyParts);

    if(parts.size() !=3){
        return false;
    }
    newSymbol = parts[0].trimmed();
    moveDir = parts[1].trimmed().toUpper();
    newState = parts[2].trimmed();

    if (newSymbol.isEmpty() && moveDir.isEmpty() && newState.isEmpty()) {
        return false;
    }

    if (!newSymbol.isEmpty()) {
        if (newSymbol.size() != 1) {
            return false;
        }

        if (!m_tableAlphabet.contains(newSymbol)) {
            return false;
        }
    }

    if (!moveDir.isEmpty()) {
        if (moveDir != "L" && moveDir != "R" && moveDir != "S") {
            return false;
        }
    }

    if (!newState.isEmpty()) {
        if (newState.toUpper() != "HALT") {
            if (!newState.startsWith('q')) {
                return false;
            }

            bool ok = false;
            const int stateNumber = newState.mid(1).toInt(&ok);
            if (!ok || stateNumber < 0 || stateNumber >= m_programTable->rowCount()) {
                return false;
            }
        }
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
        if (m_timer && m_timer->isActive()) {
            m_timer->stop();
            setExecutionControlsRunning(false);
        }
        QMessageBox::warning(this,
                             "Ошибка",
                             "Неверный формат команды.\nИспользуй формат: символ,направление,состояние\nНапример: b,R,q1 или ,L, или ,,HALT");
        return;
    }

    if (!newSymbol.isEmpty()) {
        m_tapeCells[m_headPosition] = newSymbol;
    }
    m_prependedLeft = false;

    if (moveDir == "L") {
        if (m_headPosition > 0) {
            --m_headPosition;
        }else{
            m_tapeCells.prepend(m_blankSymbol);
            m_headPosition=0;
            m_prependedLeft = true;
        }
    } else if (moveDir == "R") {
        ++m_headPosition;
        if (m_headPosition  >= m_tapeCells.size()) {
            m_tapeCells <<m_blankSymbol;
        }
    }
    if (!newState.isEmpty()) {
        if (newState.toUpper() == "HALT") {
            m_currentState = "HALT";
            m_isHalted = true;
        } else {
            m_currentState = newState;
        }
    }

    updateStateHighLight();
    updateTapeView();
}

void MainWindow::onResetClicked()
{
    if (m_timer->isActive()) {
        m_timer->stop();
    }

    setExecutionControlsRunning(false);

    if (m_inputWord.isEmpty()) {
        QMessageBox::warning(this,
                             "Ошибка",
                             "Сначала задайте входную строку.");
        return;
    }

    resetMachineStateFromInput();
    updateTapeView();
}

void MainWindow::setExecutionControlsRunning(bool running)
{
    m_tapeAlphabetEdit->setEnabled(!running);
    m_extraAlphabetEdit->setEnabled(!running);
    m_setAlphabetsButton->setEnabled(!running);

    m_inputWordEdit->setEnabled(!running);
    m_setWordButton->setEnabled(!running);

    m_programTable->setEnabled(!running);

    m_stepButton->setEnabled(!running);
    m_runButton->setEnabled(!running);

    m_stopButton->setEnabled(running);

    m_resetButton->setEnabled(true);
    m_fasterButton->setEnabled(true);
    m_slowerButton->setEnabled(true);
}

void MainWindow::onRunClicked()
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
                                 "Машина уже остановлена. Нажмите Сброс или задайте строку заново.");
        return;
    }

    QString errorText;
    if (!validateProgramTable(errorText)) {
        QMessageBox::warning(this,
                             "Ошибка программы машины",
                             errorText);
        return;
    }

    if (!hasHaltCommand()) {
        QMessageBox::warning(this,
                             "Ошибка программы машины",
                             "В таблице нет ни одной команды с HALT.\n"
                             "Добавьте хотя бы одну команду остановки, иначе запуск запрещён.");
        return;
    }

    if (m_timer->isActive()) {
        return;
    }

    m_timer->start(m_stepIntervalMs);
    setExecutionControlsRunning(true);
}

void MainWindow::onStopClicked()
{
    if (m_timer->isActive()) {
        m_timer->stop();
    }

    setExecutionControlsRunning(false);
}

void MainWindow::onTimerTimeout()
{
    if (m_isHalted) {
        m_timer->stop();
        setExecutionControlsRunning(false);
        return;
    }

    onStepClicked();

    if (m_isHalted) {
        m_timer->stop();
        setExecutionControlsRunning(false);
    }
}

void MainWindow::onFasterClicked()
{
    if (m_stepIntervalMs > 100) {
        m_stepIntervalMs -= 100;
    }

    updateSpeedLabel();

    if (m_timer->isActive()) {
        m_timer->start(m_stepIntervalMs);
    }
}

void MainWindow::onSlowerClicked()
{
    if (m_stepIntervalMs < 3000) {
        m_stepIntervalMs += 100;
    }
    updateSpeedLabel();

    if (m_timer->isActive()) {
        m_timer->start(m_stepIntervalMs);
    }
}

void MainWindow::updateSpeedLabel(){
    if (!m_speedLabel){
        return;
    }
    m_speedLabel->setText(QString("Скорость %1 мс").arg(m_stepIntervalMs));
}

void MainWindow::updateStateHighLight(){
    for (int row = 0;row<m_programTable->rowCount();++row){
        for (int col= 0;col<m_programTable->columnCount();++col){
            QTableWidgetItem *item =m_programTable->item(row,col);
            if(!item){
                continue;
            }
            item->setBackground(QBrush(QColor(30, 30, 30)));
            item->setForeground(QBrush(QColor(220, 220, 220)));
        }
    }
    bool ok = false;
    int row = -1;

    if(m_currentState.startsWith('q')){
        row = m_currentState.mid(1).toInt(&ok);
    }

    if (!ok ||row<0 || row>=m_programTable->rowCount()){
        return;
    }

    for (int col = 0;col<m_programTable->columnCount();++col){
        QTableWidgetItem *item = m_programTable->item(row, col);
        if (!item) {
            item = new QTableWidgetItem();
            m_programTable->setItem(row, col, item);
        }
        item->setBackground(QBrush(QColor(55, 140, 135)));
        item->setForeground(QBrush(QColor(255, 255, 255)));
    }
}

bool MainWindow::hasHaltCommand() const
{
    for (int row = 0; row < m_programTable->rowCount(); ++row) {
        for (int col = 1; col < m_programTable->columnCount(); ++col) {
            QTableWidgetItem *item = m_programTable->item(row, col);
            if (!item) {
                continue;
            }

            const QString text = item->text().trimmed();
            if (text.isEmpty()) {
                continue;
            }

            QString newSymbol;
            QString moveDir;
            QString newState;

            if (!parseCommand(text, newSymbol, moveDir, newState)) {
                continue;
            }

            if (newState.toUpper() == "HALT") {
                return true;
            }
        }
    }

    return false;
}

bool MainWindow::validateProgramTable(QString &errorText) const
{
    for (int row = 0; row < m_programTable->rowCount(); ++row) {
        for (int col = 1; col < m_programTable->columnCount(); ++col) {
            QTableWidgetItem *item = m_programTable->item(row, col);
            if (!item) {
                continue;
            }

            const QString text = item->text().trimmed();
            if (text.isEmpty()) {
                continue;
            }

            QString newSymbol;
            QString moveDir;
            QString newState;

            if (!parseCommand(text, newSymbol, moveDir, newState)) {
                errorText = QString("Ошибка в строке q%1, столбце '%2'.\n"
                                    "Команда содержит неверный формат или несуществующее состояние.")
                                .arg(row)
                                .arg(m_programTable->horizontalHeaderItem(col)->text());
                return false;
            }
        }
    }

    return true;
}