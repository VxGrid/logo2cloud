#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "editor.h"

#include <QColorSpace>
#include <QDir>
#include <QDockWidget>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QImageReader>
#include <QLabel>
#include <QMdiSubWindow>
#include <QMessageBox>
#include <QTimer>
#include <QScreen>
#include <QScrollArea>
#include <QScrollBar>
#include <QStandardPaths>
#include <QWheelEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow)
{
    qDebug() << "Setuo this" << Qt::endl;
    ui_->setupUi(this);
    // menu inits
    connect(ui_->actionOpen_File, &QAction::triggered, this, &MainWindow::open);
    connect(ui_->actionClose_image, &QAction::triggered, this, &MainWindow::close);
    setCentralWidget(ui_->mdiArea);
    //resize(QGuiApplication::primaryScreen()->availableSize());
    qDebug() << "size of this" << this->size() << Qt::endl;
    //QTimer::singleShot(250, this, &MainWindow::open);
    qDebug() << "Finished constructor" << Qt::endl;
}


MainWindow::~MainWindow()
{
    delete ui_;
}


bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::GraphicsSceneWheel)
    {
        QGraphicsSceneWheelEvent *wheelEvent = dynamic_cast<QGraphicsSceneWheelEvent *>
            (event);
        (wheelEvent->delta() > 0) ? scaleImage(1.25) : scaleImage(0.8);
        statusBar()->showMessage(tr("Current scale: %1").arg(scaleFactor_));
        event->accept();
        return true;
    }
    else if (event->type() == QEvent::GraphicsSceneMousePress)
    {
        QGraphicsSceneMouseEvent *mouseEvent = dynamic_cast<QGraphicsSceneMouseEvent *>
            (event);

        if (mouseEvent->button() == Qt::LeftButton)
        {
            if (!setSelectedPixelColorTempToAlpha0(mouseEvent))
                return false;
        }
        else if (mouseEvent->button() == Qt::MiddleButton)
        {
            dragActive_ = true;
            statusBar()->showMessage((tr("Activate drag")));
        }

        event->accept();
        return true;
    }
    else if (event->type() == QEvent::GraphicsSceneMouseRelease)
    {
        QGraphicsSceneMouseEvent *mouseEvent = dynamic_cast<QGraphicsSceneMouseEvent *>
            (event);

        if (mouseEvent->button() == Qt::MiddleButton)
        {
            dragActive_ = false;
            statusBar()->showMessage(tr("Deactivate drag"));
        }

        event->accept();
        return true;
    }
    else if (dragActive_ && event->type() == QEvent::GraphicsSceneMouseMove)
    {
        QGraphicsSceneMouseEvent *mouseEvent = dynamic_cast<QGraphicsSceneMouseEvent *>
            (event);

        if (!translatesceneItemsByMouseDrag(mouseEvent))
            return false;

        event->accept();
        return true;
    }

    return QMainWindow::eventFilter(watched, event);
}


void MainWindow::open()
{
    /*
        QFileDialog dialog(this, tr("Open File"));
        static bool firstDialog = true;

        if (firstDialog)
        {
        firstDialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
        }

        QStringList mimeTypeFilters;
        const QByteArrayList supportedMimeTypes = QImageReader::supportedMimeTypes();
        for (const QByteArray &mimeTypeName : supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);

        mimeTypeFilters.sort();
        mimeTypeFilters.prepend("application/octet-stream");
        dialog.setMimeTypeFilters(mimeTypeFilters);

        if (dialog.exec() == QDialog::Accepted && loadFile(dialog.selectedFiles().first()))
        ui_->actionOpen_File->setEnabled(false);
    */
    auto fileContentReady = [this](const QString & fileName, const QByteArray & fileContent)
    {
        if (fileName.isEmpty()) {
            // No file was selected
            qWarning() << "No file selected" << Qt::endl;
        } else {
            qDebug() << "Load the file:" << fileName << Qt::endl;
            loadFile(fileName, fileContent);
        }
    };
    QFileDialog::getOpenFileContent("Images (*.png *.xpm *.jpg)",
        fileContentReady);
}


void MainWindow::close()
{
    if (image_)
    {
        ui_->mdiArea->removeSubWindow(subWin_);
        gscene_->removeEventFilter(this);
        removeDockWidget(dockEditor_);
        image_.reset();
        ui_->actionOpen_File->setEnabled(true);
    }
}


bool MainWindow::loadFile(const QString &fileName)
{
    // Prepare MDI
    gscene_ = new QGraphicsScene(ui_->mdiArea);
    QGraphicsView *view = new QGraphicsView(gscene_, ui_->mdiArea);
    view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    qDebug() << "Add subwindow" << Qt::endl;
    subWin_ = ui_->mdiArea->addSubWindow(view);
    ui_->mdiArea->setViewMode(QMdiArea::TabbedView);
    // load image
    qDebug() << "Start image reader" << Qt::endl;
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();

    if (newImage.isNull())
    {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
            tr("Cannot load %1: %2")
            .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
        qDebug() << "Cannot load image - is null" << Qt::endl;
        return false;
    }

    // Add the image to class
    image_ = std::make_shared<QImage>(newImage);

    if (image_->colorSpace().isValid())
        image_->convertToColorSpace(QColorSpace::SRgb);

    image_->convertTo(QImage::Format_ARGB32);
    addEditorW2MainWin();
    addImage2Scene(newImage);
    setWindowFilePath(fileName);
    QFileInfo fi(fileName);
    subWin_->setWindowTitle(fi.fileName());
    view->show();
    gscene_->installEventFilter(this);
    const QString message = tr("Opened \"%1\", %2x%3, Depth: %4")
        .arg(QDir::toNativeSeparators(fileName))
        .arg(image_->width())
        .arg(image_->height())
        .arg(image_->depth());
    statusBar()->showMessage(message);
    return true;
}


bool MainWindow::loadFile(const QString &fileName, const QByteArray &fileContent)
{
    // Prepare MDI
    gscene_ = new QGraphicsScene(ui_->mdiArea);
    QGraphicsView *view = new QGraphicsView(gscene_, ui_->mdiArea);
    view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    qDebug() << "Add subwindow" << Qt::endl;
    subWin_ = ui_->mdiArea->addSubWindow(view);
    ui_->mdiArea->setViewMode(QMdiArea::TabbedView);
    // load image
    qDebug() << "Start image reader" << fileContent.size() << Qt::endl;
    //QImage newImage(fileContent.data());
    // Add the image to class
    image_ = std::make_shared<QImage>(QImage::fromData(fileContent));
    qDebug() << "new image size: " << image_->size() << Qt::endl;
    // QLabel myLabel;
    // myLabel.setPixmap(QPixmap::fromImage(*image_));
    // myLabel.show();

    if (image_->colorSpace().isValid())
        image_->convertToColorSpace(QColorSpace::SRgb);

    image_->convertTo(QImage::Format_ARGB32);
    addEditorW2MainWin();
    addImage2Scene(*image_);
    setWindowFilePath(fileName);
    QFileInfo fi(fileName);
    subWin_->setWindowTitle(fi.fileName());
    view->show();
    gscene_->installEventFilter(this);
    const QString message = tr("Opened \"%1\", %2x%3, Depth: %4")
        .arg(QDir::toNativeSeparators(fileName))
        .arg(image_->width())
        .arg(image_->height())
        .arg(image_->depth());
    statusBar()->showMessage(message);
    qDebug() << "Finalized loadFile" << Qt::endl;
    return true;
}


void MainWindow::addEditorW2MainWin()
{
    // Add dockwidget to mainwindow for edit options
    dockEditor_ = new QDockWidget(this);
    editorW_ = new Editor(image_, dockEditor_);
    editorW_->setStatusBar(statusBar());
    dockEditor_->setWidget(editorW_);
    dockEditor_->setAllowedAreas(Qt::TopDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, dockEditor_);
}


void MainWindow::addImage2Scene(const QImage &newImage)
{
    QPixmap backGroundpix(newImage.width() + 200, newImage.height() + 200);
    backGroundpix.fill();
    QGraphicsPixmapItem *backgroundPixItem = new QGraphicsPixmapItem(backGroundpix);
    backgroundPixItem->setOffset(QPoint(-100, -100));
    gscene_->addItem(backgroundPixItem);
    editorW_->setBackGroundPixItem(backgroundPixItem);
    QGraphicsPixmapItem *graphPixItem = new QGraphicsPixmapItem(QPixmap::fromImage(
            *image_));
    graphPixItem->setZValue(
        1); // for identification and that the loaded image is on top, so alpha works
    gscene_->addItem(graphPixItem);
    scaleFactor_ = 1.0; // reset the scaleFactor_ when loading new image
}


void MainWindow::scaleImage(double factor)
{
    scaleFactor_ *= factor;
    gscene_->views().first()->scale(factor,
        factor); // TODO: Add support for multiple images
}


bool MainWindow::setSelectedPixelColorTempToAlpha0(QGraphicsSceneMouseEvent
    *mouseEvent)
{
    auto sceneItems = gscene_->items();

    for (const auto &graphItem : sceneItems)
    {
        QPointF point = graphItem->mapFromParent(mouseEvent->scenePos());
        statusBar()->showMessage(tr("Clicked pixel [x|column ; y|row]: %1 ; %2").arg(
                point.x()).arg(point.y()));
        QGraphicsPixmapItem *graphPixItem = dynamic_cast<QGraphicsPixmapItem *>
            (graphItem);

        if (!graphPixItem)
            return false;

        if (!image_->valid(point.toPoint()))
        {
            graphPixItem->setPixmap(QPixmap::fromImage(*image_));
            return true;
        }

        QColor col = image_->pixelColor(point.x(), point.y());
        editorW_->setColor(col);

        if (graphPixItem->zValue() == 1)
        {
            QImage tmpImg = *image_;
            QColor colAfter = col;
            colAfter.setAlpha(0);
            Editor::updateImage(tmpImg, col, colAfter);
            graphPixItem->setPixmap(QPixmap::fromImage(tmpImg));
            const auto views = gscene_->views();

            for (const auto &view : views)
                view->update();
        }
    }

    return true;
}


bool MainWindow::translatesceneItemsByMouseDrag(QGraphicsSceneMouseEvent
    *mouseEvent)
{
    QPointF trans = mouseEvent->scenePos() - mouseEvent->lastScenePos();

    if (trans.isNull())
        return false;

    auto sceneItems = gscene_->items();

    for (auto item : sceneItems)
        item->setPos(item->x() + trans.x(), item->y() + trans.y());

    statusBar()->showMessage(tr("Translate: %1 %2").arg(trans.x()).arg(trans.y()));
    return true;
}
