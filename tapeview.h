#ifndef TAPEVIEW_H
#define TAPEVIEW_H

#include <QGraphicsView>
#include <QStringList>
#include <QVector>

class QGraphicsScene;
class QGraphicsTextItem;
class QPropertyAnimation;
class HeadItem;
class QGraphicsRectItem;
class QVariantAnimation;

class TapeView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit TapeView(QWidget *parent = nullptr);

    void setTape(const QStringList &cells,
                 int headPosition,
                 const QString &state,
                 const QString &blankSymbol,
                 bool prependedLeft);

    void resetView();

private:
    void buildScene();
    void updateSceneContents(bool animated,
                             int previousHeadPosition,
                             int previousOffset,
                             bool prependedLeft);
    void applyWindowTexts();
    void updateHeadPosition(bool animated, int visualHead);
    int clampVisualHead(int headPosition, int offset) const;
    int computeOffsetFromHead(int headPosition, int currentOffset) const;
    void animateTapeShift(int deltaOffset, int finalVisualHead);

private:
    QGraphicsScene *m_scene;
    QVector<QGraphicsRectItem*> m_cellRects;
    QVector<QGraphicsTextItem*> m_cellTexts;
    QGraphicsTextItem *m_stateText;
    HeadItem *m_headItem;
    QPropertyAnimation *m_headAnimation;
    QVariantAnimation *m_slideAnimation;

    QStringList m_cells;
    int m_headPosition;
    QString m_state;
    QString m_blankSymbol;

    int m_offset;

    static const int kVisibleCellCount = 21;
    static const int kCellWidth = 50;
    static const int kCellHeight = 50;
    static const int kStartX = 20;
    static const int kStartY = 70;

    static const int kLeftLimit = 4;
    static const int kRightLimit = 16;
};

#endif // TAPEVIEW_H