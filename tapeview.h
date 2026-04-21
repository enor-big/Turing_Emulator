#ifndef TAPEVIEW_H
#define TAPEVIEW_H

#include <QGraphicsView>
#include <QStringList>
#include <QVector>

class QGraphicsScene;
class QGraphicsTextItem;
class QPropertyAnimation;
class HeadItem;

class TapeView : public QGraphicsView
{
    Q_OBJECT

public:
    explicit TapeView(QWidget *parent = nullptr);

    void setTape(const QStringList &cells,
                 int headPosition,
                 const QString &state,
                 const QString &blankSymbol);

    void resetView();

private:
    void buildScene();
    void updateSceneContents(bool animated);

private:
    QGraphicsScene *m_scene;
    QVector<QGraphicsTextItem*> m_cellItems;
    QGraphicsTextItem *m_stateText;
    HeadItem *m_headItem;
    QPropertyAnimation *m_headAnimation;

    QStringList m_cells;
    int m_headPosition;
    QString m_state;
    QString m_blankSymbol;

    int m_offset;

    static const int kVisibleCellCount = 21;
    static const int kCellWidth = 40;
    static const int kCellHeight = 40;
    static const int kStartX = 20;
    static const int kStartY = 70;
};

#endif // TAPEVIEW_H