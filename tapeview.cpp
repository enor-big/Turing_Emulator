#include "tapeview.h"
#include "headitem.h"

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QPropertyAnimation>
#include <QFont>
#include <QPen>
#include <QBrush>

TapeView::TapeView(QWidget *parent)
    : QGraphicsView(parent),
    m_scene(new QGraphicsScene(this)),
    m_stateText(nullptr),
    m_headItem(nullptr),
    m_headAnimation(nullptr),
    m_headPosition(0),
    m_blankSymbol("_"),
    m_offset(0)
{
    setScene(m_scene);
    setRenderHint(QPainter::Antialiasing, true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setMinimumHeight(200);
    setBackgroundBrush(Qt::white);
    setFrameShape(QFrame::Box);

    buildScene();
}

void TapeView::buildScene()
{
    m_scene->clear();
    m_cellItems.clear();

    m_scene->setSceneRect(0, 0, 900, 180);

    QFont stateFont;
    stateFont.setPointSize(12);
    stateFont.setBold(true);

    m_stateText = m_scene->addText("Состояние: q0", stateFont);
    m_stateText->setDefaultTextColor(Qt::black);
    m_stateText->setPos(20, 10);

    QFont cellFont;
    cellFont.setPointSize(14);

    for (int i = 0; i < kVisibleCellCount; ++i) {
        QGraphicsTextItem *item = m_scene->addText("_", cellFont);
        item->setDefaultTextColor(Qt::black);
        item->setPos(kStartX + i * kCellWidth + 12, kStartY + 6);
        m_cellItems.push_back(item);
    }

    for (int i = 0; i <= kVisibleCellCount; ++i) {
        const int x = kStartX + i * kCellWidth;
        m_scene->addLine(x, kStartY, x, kStartY + kCellHeight, QPen(Qt::black, 1));
    }

    m_scene->addLine(kStartX,
                     kStartY,
                     kStartX + kVisibleCellCount * kCellWidth,
                     kStartY,
                     QPen(Qt::black, 1));
    m_scene->addLine(kStartX,
                     kStartY + kCellHeight,
                     kStartX + kVisibleCellCount * kCellWidth,
                     kStartY + kCellHeight,
                     QPen(Qt::black, 1));

    m_headItem = new HeadItem();
    m_scene->addItem(m_headItem);
    m_headItem->setPos(kStartX + 5, kStartY - 35);

    m_headAnimation = new QPropertyAnimation(m_headItem, "pos", this);
    m_headAnimation->setDuration(250);
}

void TapeView::setTape(const QStringList &cells,
                       int headPosition,
                       const QString &state,
                       const QString &blankSymbol)
{
    const bool headMoved = (headPosition != m_headPosition);

    m_cells = cells;
    m_headPosition = headPosition;
    m_state = state;
    m_blankSymbol = blankSymbol;

    updateSceneContents(headMoved);
}

void TapeView::resetView()
{
    m_cells.clear();
    m_headPosition = 0;
    m_state = "q0";
    m_offset = 0;
    updateSceneContents(false);
}

void TapeView::updateSceneContents(bool animated)
{
    if (!m_stateText) {
        return;
    }

    m_stateText->setPlainText(QString("Состояние: %1").arg(m_state));

    if (m_cells.isEmpty()) {
        for (int i = 0; i < m_cellItems.size(); ++i) {
            m_cellItems[i]->setPlainText(m_blankSymbol);
        }
        m_headItem->setVisible(false);
        return;
    }

    m_headItem->setVisible(true);

    const int centerIndex = kVisibleCellCount / 2;
    m_offset = m_headPosition - centerIndex;
    if (m_offset < 0) {
        m_offset = 0;
    }

    for (int i = 0; i < m_cellItems.size(); ++i) {
        const int logicalPos = m_offset + i;

        if (logicalPos >= 0 && logicalPos < m_cells.size()) {
            m_cellItems[i]->setPlainText(m_cells[logicalPos]);
        } else {
            m_cellItems[i]->setPlainText(m_blankSymbol);
        }
    }

    int visualHead = m_headPosition - m_offset;
    if (visualHead < 0) {
        visualHead = 0;
    }
    if (visualHead >= kVisibleCellCount) {
        visualHead = kVisibleCellCount - 1;
    }

    const QPointF targetPos(kStartX + visualHead * kCellWidth + 5, kStartY - 35);

    if (animated) {
        m_headAnimation->stop();
        m_headAnimation->setStartValue(m_headItem->pos());
        m_headAnimation->setEndValue(targetPos);
        m_headAnimation->start();
    } else {
        m_headItem->setPos(targetPos);
    }
}