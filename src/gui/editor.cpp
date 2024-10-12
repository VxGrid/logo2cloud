#include "editor.h"
#include "ui_editor.h"

#ifdef WIN32
#define NOMINMAX // disable min max macro of Windows.h
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <algorithm>
#include <cmath>
#include <future>

#include <QDebug>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QShortcut>
#include <QStandardPaths>
#include <QStatusBar>

#include <QColorDialog>

Editor::Editor(std::shared_ptr<QImage> &image, QWidget *parent) : QWidget(parent),
                                                                  ui_(new Ui::Editor),
                                                                  image_{image}
{
  ui_->setupUi(this);
  ui_->w_colDiag->setBackgroundRole(QPalette::Window);
  ui_->w_colDiag->setAutoFillBackground(true);
  ui_->w_colDisPix->setBackgroundRole(QPalette::Window);
  ui_->w_colDisPix->setAutoFillBackground(true);
  ui_->w_colDisPix->setPalette(QPalette(Qt::white));

  connect(ui_->pb_colDisPix, &QPushButton::pressed, this, &Editor::onPropertyChanged);
  connect(ui_->sb_height, &QDoubleSpinBox::editingFinished, this, &Editor::onPropertyChanged);
  connect(ui_->sb_width, &QDoubleSpinBox::editingFinished, this, &Editor::onPropertyChanged);
  connect(ui_->sb_depth, &QDoubleSpinBox::editingFinished, this, &Editor::onPropertyChanged);
  connect(ui_->sb_pointSpacing, &QDoubleSpinBox::editingFinished, this, &Editor::onPropertyChanged);
  connect(ui_->pb_removeColor, &QPushButton::clicked, this, &Editor::onPropertyChanged);
  connect(ui_->pb_export, &QPushButton::clicked, this, &Editor::onPropertyChanged);
  connect(ui_->cb_p2p, &QCheckBox::clicked, this, &Editor::onPropertyChanged);
  QKeySequence ks("F1");
  QShortcut* sh = new QShortcut(ks, this);
  connect(sh, &QShortcut::activated, [this](){ui_->pb_removeColor->animateClick();});

  emit ui_->sb_height->editingFinished(); // so propertyvalues are adapted to ratio
  emit ui_->cb_p2p->clicked();            // so we have values in ui_.sb_pointSpacing
}


Editor::~Editor()
{
  delete ui_;
}


void Editor::setColor(QColor col)
{
  color_ = col;
  ui_->w_colDiag->setPalette(QPalette(col));
}


void Editor::setStatusBar(QStatusBar *statusBar)
{
  statusBar_ = statusBar;
}


void Editor::setBackGroundPixItem(QGraphicsPixmapItem *backgroundPixItem)
{
  backgroundPixItem_ = backgroundPixItem;
}


void Editor::updateImage(QImage &image, QColor colBefore, QColor colAfter)
{
  uchar *imgBits = image.bits();
  const int red{colBefore.red()}, green{colBefore.green()}, blue{colBefore.blue()}, alpha{colBefore.alpha()};
  int index{};
  for (int r = 0; r < image.height(); ++r)
  {
    for (int c = 0; c < image.width(); ++c)
    {
      index = (r * image.width() + c) * 4;
      if (imgBits[index] == blue && imgBits[index + 1] == green && imgBits[index + 2] == red && imgBits[index + 3] == alpha)
      {
        imgBits[index] = colAfter.blue();
        imgBits[index + 1] = colAfter.green();
        imgBits[index + 2] = colAfter.red();
        imgBits[index + 3] = colAfter.alpha();
      }
    }
  }
}


void Editor::onPropertyChanged()
{
  if (sender() == ui_->cb_p2p)
  {
    ui_->sb_pointSpacing->setEnabled(!ui_->cb_p2p->isChecked());
    calculateSetPointSpacing();
    setPCloudProperties();
    //emit ui_->sb_pointSpacing->editingFinished(); // so propertyvalues are adapted to ratio
  }
  else if (sender() == ui_->sb_height || sender() == ui_->sb_width || sender() == ui_->sb_depth || sender() == ui_->sb_pointSpacing)
  {
    qDebug() << "onPropChanged: " << sender();
    QSignalBlocker sB_sb_height(ui_->sb_height);
    QSignalBlocker sB_sb_width(ui_->sb_width);
    QSignalBlocker sB_sb_depth(ui_->sb_depth);
    QSignalBlocker sB_sb_pointSpacing(ui_->sb_pointSpacing);

    // Set the values so our image width 2 height ratio is still correct
    double ratio = image_->width() / image_->height();
    (sender() == ui_->sb_height) ? ui_->sb_width->setValue(ratio * ui_->sb_height->value()) : ui_->sb_height->setValue(ui_->sb_width->value() / ratio);

    calculateSetPointSpacing();
    setPCloudProperties();

    updateMaxPointGeneratedSBox();
  }
  else if (sender() == ui_->pb_colDisPix)
  {
    QColor color = QColorDialog::getColor();
    color.setAlpha(255); // make sure user can't make background image transparent
    ui_->w_colDisPix->setPalette(QPalette(color));
    auto pixm = backgroundPixItem_->pixmap();
    pixm.fill(color);
    backgroundPixItem_->setPixmap(pixm);
  }
  else if (sender() == ui_->pb_removeColor)
  {
    if (!color_.isValid())
      return;

    qDebug() << "Remove color: " << color_ << Qt::endl;
    QColor colAfter = color_;
    colAfter.setAlpha(0);
    updateImage(*image_, color_, colAfter);
  }
  else if (sender() == ui_->pb_export)
  {
    // Chose path
    QFileDialog dialog(this, tr("Save File as"));
    const auto stdPaths = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
    static bool firstDialog = true;

    if (firstDialog && !stdPaths.empty())
    {
      firstDialog = false;
      dialog.setDirectory(stdPaths.first());
    }

    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::AnyFile);
    //dialog.setOption(QFileDialog::ShowDirsOnly);
    QStringList filters;

    for (const auto &fileFormats : pcloudgen_.fileFormats) filters.push_back(QString::fromStdString(fileFormats));

    dialog.setNameFilters(filters);
    dialog.selectNameFilter(filters.first());
    dialog.setViewMode(QFileDialog::Detail);

    QStringList fileName;
    if (dialog.exec() == QDialog::Accepted)
    {
      // export running, freeze property window
      this->setEnabled(false);

      // Get stuff from dialog
      fileName = dialog.selectedFiles();
      if (!fileName.size())
        return;

      QFileInfo fi(fileName.first());
      QString fPath = fi.absolutePath() + "/" + fi.completeBaseName();
      ui_->le_path->setText(fPath);

      qDebug() << "Export cloud start";
      QImage img(*image_); // make copy
      qDebug() << "image size: " << img.size() << Qt::endl;
      pcloudgen_.setData(img.bits(), img.height(), img.width());
      pcloudgen_.setRandomizeValue(ui_->sb_randomizer->value());

      if (pcloudgen_.exportCloud(fPath.toStdString(), PointCloudGenerator::EXPORTER(filters.indexOf(dialog.selectedNameFilter()))))
      {
        statusBar_->showMessage(tr("Initializing the writer failed!!!"));
        return;
      }

      auto future = std::async(std::launch::async, &PointCloudGenerator::run, &pcloudgen_);
      std::future_status futureStatus;

      do
      {
        if (statusBar_)
          statusBar_->showMessage(tr("Export Status: %1 %").arg(pcloudgen_.getProgressPercent()));

        QCoreApplication::processEvents();
        futureStatus = future.wait_for(std::chrono::milliseconds(25));
      } while (futureStatus != std::future_status::ready);

      qDebug() << "Export cloud finished" << Qt::endl;

      this->setEnabled(true);
      if (statusBar_)
        statusBar_->showMessage(tr("Export finished"));
    }
  }
}


void Editor::updateMaxPointGeneratedSBox()
{
  double pSpacing = ui_->sb_pointSpacing->value();
  double maxPoints = (ui_->sb_width->value() / pSpacing) * (ui_->sb_height->value() / pSpacing) * std::max(1, int(std::ceil(ui_->sb_depth->value() / pSpacing)));
  ui_->le_maxPoints->setText(QString("%1").arg(maxPoints));
}


void Editor::calculateSetPointSpacing()
{
  if (ui_->cb_p2p->isChecked())
    ui_->sb_pointSpacing->setValue(ui_->sb_height->value() / image_->height());
}


void Editor::setPCloudProperties()
{
  double height{ui_->sb_height->value()},
      width{ui_->sb_width->value()},
      depth{ui_->sb_depth->value()},
      pointSpacing{ui_->sb_pointSpacing->value()};

  pcloudgen_.setProperties(width, height, depth, pointSpacing);
}
