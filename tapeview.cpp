#include "tapeview.h"
#include "headitem.h"

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QPropertyAnimation>
#include <QVariantAnimation>
#include <QFont>
#include <QPen>
#include <QBrush>

TapeView::TapeView(QWidget *parent)
    : QGraphicsView(parent),
    m_scene(new QGraphicsScene(this)),
    m_stateText(nullptr),
    m_headItem(nullptr),
    m_headAnimation(nullptr),
    m_slideAnimation(nullptr),
    m_headPosition(0),
    m_blankSymbol("_"),
    m_offset(0)
{
    setScene(m_scene);
    setRenderHint(QPainter::Antialiasing, true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setMinimumHeight(220);
    setBackgroundBrush(Qt::white);
    setFrameShape(QFrame::Box);

    buildScene();
}

void TapeView::buildScene()
{
    m_scene->clear();
    m_cellRects.clear();
    m_cellTexts.clear();

    m_scene->setSceneRect(0, 0, 1200, 220);

    QFont stateFont;
    stateFont.setPointSize(12);
    stateFont.setBold(true);

    m_stateText = m_scene->addText("Состояние: q0", stateFont);
    m_stateText->setDefaultTextColor(Qt::black);
    m_stateText->setPos(20, 10);

    QFont cellFont;
    cellFont.setPointSize(14);

    for (int i = 0; i < kVisibleCellCount; ++i) {
        const int x = kStartX + i * kCellWidth;
        QGraphicsRectItem *rect =
            m_scene->addRect(x, kStartY, kCellWidth, kCellHeight,
                             QPen(QColor(120, 120, 120), 1),
                             QBrush(QColor(220, 220, 220)));
        m_cellRects.push_back(rect);

        QGraphicsTextItem *text = m_scene->addText("_", cellFont);
        text->setDefaultTextColor(Qt::black);
        text->setPos(kStartX + i * kCellWidth + 12, kStartY + 6);
        m_cellTexts.push_back(text);
    }

    m_headItem = new HeadItem();
    m_scene->addItem(m_headItem);
    m_headItem->setPos(kStartX + 10, kStartY - 35);

    m_headAnimation = new QPropertyAnimation(m_headItem, "pos", this);
    m_headAnimation->setDuration(180);
    m_headAnimation->setEasingCurve(QEasingCurve::OutCubic);

    m_slideAnimation = new QVariantAnimation(this);
    m_slideAnimation->setDuration(180);
    m_slideAnimation->setEasingCurve(QEasingCurve::OutCubic);
}

void TapeView::setTape(const QStringList &cells,
                       int headPosition,
                       const QString &state,
                       const QString &blankSymbol,
                       bool prependedLeft)
{
    const int previousHeadPosition = m_headPosition;
    const int previousOffset = m_offset;

    m_cells = cells;
    m_headPosition = headPosition;
    m_state = state;
    m_blankSymbol = blankSymbol;

    updateSceneContents(headPosition != previousHeadPosition || prependedLeft,
                        previousHeadPosition,
                        previousOffset,
                        prependedLeft);
}

void TapeView::resetView()
{
    m_cells.clear();
    m_headPosition = 0;
    m_state = "q0";
    m_offset = 0;
    updateSceneContents(false, 0, 0, false);
}

int TapeView::clampVisualHead(int headPosition, int offset) const
{
    int visualHead = headPosition - offset;

    if (visualHead < 0) {
        visualHead = 0;
    }
    if (visualHead >= kVisibleCellCount) {
        visualHead = kVisibleCellCount - 1;
    }

    return visualHead;
}

int TapeView::computeOffsetFromHead(int headPosition, int currentOffset) const
{
    int visualHead = headPosition - currentOffset;
    int newOffset = currentOffset;

    if (visualHead > kRightLimit) {
        newOffset = headPosition - kRightLimit;
    } else if (visualHead < kLeftLimit) {
        newOffset = headPosition - kLeftLimit;
        if (newOffset < 0) {
            newOffset = 0;
        }
    }

    return newOffset;
}

void TapeView::applyWindowTexts()
{
    if (m_cells.isEmpty()) {
        for (int i = 0; i < m_cellTexts.size(); ++i) {
            m_cellTexts[i]->setPlainText(m_blankSymbol);
            m_cellRects[i]->setBrush(QBrush(QColor(220, 220, 220)));
        }
        return;
    }

    const int visualHead = clampVisualHead(m_headPosition, m_offset);

    for (int i = 0; i < m_cellTexts.size(); ++i) {
        const int logicalPos = m_offset + i;

        if (logicalPos >= 0 && logicalPos < m_cells.size()) {
            m_cellTexts[i]->setPlainText(m_cells[logicalPos]);
        } else {
            m_cellTexts[i]->setPlainText(m_blankSymbol);
        }

        if (i == visualHead) {
            m_cellRects[i]->setBrush(QBrush(QColor(55, 140, 135)));
        } else {
            m_cellRects[i]->setBrush(QBrush(QColor(220, 220, 220)));
        }
    }
}

void TapeView::updateHeadPosition(bool animated, int visualHead)
{
    const QPointF targetPos(kStartX + visualHead * kCellWidth + 10, kStartY - 35);

    if (animated) {
        m_headAnimation->stop();
        m_headAnimation->setStartValue(m_headItem->pos());
        m_headAnimation->setEndValue(targetPos);
        m_headAnimation->start();
    } else {
        m_headItem->setPos(targetPos);
    }
}

void TapeView::animateTapeShift(int deltaOffset, int finalVisualHead)
{
    if (deltaOffset == 0) {
        applyWindowTexts();
        updateHeadPosition(false, finalVisualHead);
        return;
    }

    const qreal pixelShift = -deltaOffset * kCellWidth;

    m_slideAnimation->stop();
    disconnect(m_slideAnimation, nullptr, nullptr, nullptr);

    connect(m_slideAnimation, &QVariantAnimation::valueChanged, this,
            [this](const QVariant &value) {
                const qreal shift = value.toReal();

                for (int i = 0; i < m_cellRects.size(); ++i) {
                    const int baseX = kStartX + i * kCellWidth;
                    m_cellRects[i]->setRect(baseX + shift, kStartY, kCellWidth, kCellHeight);
                    m_cellTexts[i]->setPos(baseX + shift + 16, kStartY + 8);
                }
            });

    connect(m_slideAnimation, &QVariantAnimation::finished, this,
            [this, finalVisualHead]() {
                for (int i = 0; i < m_cellRects.size(); ++i) {
                    const int baseX = kStartX + i * kCellWidth;
                    m_cellRects[i]->setRect(baseX, kStartY, kCellWidth, kCellHeight);
                    m_cellTexts[i]->setPos(baseX + 16, kStartY + 8);
                }

                applyWindowTexts();
                updateHeadPosition(false, finalVisualHead);
            });

    m_slideAnimation->setStartValue(0.0);
    m_slideAnimation->setEndValue(pixelShift);
    m_slideAnimation->start();
}

void TapeView::updateSceneContents(bool animated,
                                   int previousHeadPosition,
                                   int previousOffset,
                                   bool prependedLeft)
{
    if (!m_stateText) {
        return;
    }

    m_stateText->setPlainText(QString("Состояние: %1").arg(m_state));

    if (m_cells.isEmpty()) {
        for (int i = 0; i < m_cellTexts.size(); ++i) {
            m_cellTexts[i]->setPlainText(m_blankSymbol);
        }
        m_headItem->setVisible(false);
        m_offset = 0;
        return;
    }

    m_headItem->setVisible(true);

    const int oldOffset = previousOffset;
    const int newOffset = computeOffsetFromHead(m_headPosition, oldOffset);
    const int oldVisualHead = clampVisualHead(previousHeadPosition, oldOffset);
    const int newVisualHead = clampVisualHead(m_headPosition, newOffset);

    m_offset = newOffset;
    if (prependedLeft) {
        animateTapeShift(-1, newVisualHead);

    } else if (animated && newOffset != oldOffset) {
        animateTapeShift(newOffset - oldOffset, newVisualHead);
    } else {
        applyWindowTexts();

        if (animated && newVisualHead != oldVisualHead) {
            updateHeadPosition(true, newVisualHead);
        } else {
            updateHeadPosition(false, newVisualHead);
        }
    }
}