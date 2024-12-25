#ifndef EDITOR_H
#define EDITOR_H

#include "../logic/pointcloudgenerator.h"

#include <QImage>
#include <QWidget>

class QGraphicsPixmapItem;
class QStatusBar;

namespace Ui
{
class Editor;
}

class Editor : public QWidget
{
    Q_OBJECT

public:
    /// Constructor
    explicit Editor(std::shared_ptr<QImage> &image, QWidget *parent = nullptr);

    /// Destructor
    ~Editor();

    /// sets the color into the ui widget w_colDiag
    void setColor(QColor col);

    void setStatusBar(QStatusBar *statusBar);

    /// set the background so we can update it from the editor
    void setBackGroundPixItem(QGraphicsPixmapItem *backgroundPixItem);

    /// updates the inserted image with these colors
    static void updateImage(QImage &image, QColor colBefore, QColor colAfter);

public slots:
    void onPropertyChanged();

private:
    void updateMaxPointGeneratedSBox();

    void calculateSetPointSpacing();

    void setPCloudProperties();

    /// The ui of the editor
    Ui::Editor *ui_{};

    /// The loaded image reference
    std::shared_ptr<QImage> image_{};

    /// image2cloud generator
    PointCloudGenerator pcloudgen_;

    /// Currently selected color
    QColor color_;

    /// Set backgroundPixItem
    QGraphicsPixmapItem *backgroundPixItem_{};

    /// statusbar of mainwindow
    QStatusBar *statusBar_{};
};

#endif // EDITOR_H
