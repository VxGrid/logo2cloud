#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }

class Editor;

class QAction;
class QGraphicsPixmapItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QMdiSubWindow;
class QMenu;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /// Constructor
    MainWindow(QWidget *parent = nullptr);

    /// destructor
    ~MainWindow();

    /// handles mouse interactions with scene
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    /// open dialog to load image
    void open();

    /// close current loaded image
    void close();

private:
    /// loads an image from hard drive
    bool loadFile(const QString &fileName);


    /// Adds the Editor as a dockwidget to the main window
    void addEditorW2MainWin();

    /// Sets the image into the scene
    void addImage2Scene(const QImage &newImage);

    /// Scale the image depending on factor
    void scaleImage(double factor);


    bool setSelectedPixelColorTempToAlpha0(QGraphicsSceneMouseEvent *mouseEvent);

    bool translatesceneItemsByMouseDrag(QGraphicsSceneMouseEvent *mouseEvent);

    /// Mainwindow ui
    Ui::MainWindow *ui_;

    /// the image sub window
    QMdiSubWindow* subWin_;

    /// The scene where the image is displayed
    QGraphicsScene *gscene_;

    /// Editor dock widget where the Editor lives in
    QDockWidget *dockEditor_;

    /// Widget for editing the scene
    Editor *editorW_;

    /// The loaded image
    QImage image_;

    /// the current scale factor of the image
    double scaleFactor_{1.0};

    /// Whether the mouse wheel button is pressed or released
    bool dragActive_{false};
};
#endif // MAINWINDOW_H
