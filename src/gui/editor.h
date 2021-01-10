#ifndef EDITOR_H
#define EDITOR_H

#include "../logic/pointcloudgenerator.h"

#include <QImage>
#include <QWidget>

class QGraphicsPixmapItem;

namespace Ui {
class Editor;
}

class Editor : public QWidget
{
    Q_OBJECT

public:
    /// Constructor
    explicit Editor(QImage &image, QWidget *parent = nullptr);

    /// Destructor
    ~Editor();

    /// sets the color into the ui widget w_colDiag
    void setColor(QColor col);

    /// set the background so we can update it from the editor
    void setBackGroundPixItem(QGraphicsPixmapItem *backgroundPixItem);

    /// updates the inserted image with these colors
    static void updateImage(QImage &image, QColor colBefore, QColor colAfter);

public slots:
    void onPropertyChanged();

private:
    /// The ui of the editor
    Ui::Editor *ui_{};

    /// The loaded image reference
    QImage &image_;

    /// image2cloud generator
    pointcloudgenerator pcloudgen_;

    /// Currently selected color
    QColor color_;

    /// Set backgroundPixItem
    QGraphicsPixmapItem *backgroundPixItem_;

};

#endif // EDITOR_H
