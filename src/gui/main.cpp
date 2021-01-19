#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QTranslator>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  //if (QLocale::system().language() == QLocale::German)
  {
    // set German translation
    QTranslator *trans = new QTranslator(&a);
    bool loadedTransSuccessful = trans->load("logo2Cloud_de_GER.qm");
    trans->setObjectName(QString("GermanTranslation"));
    qDebug() << "Loaded trans succes: " << loadedTransSuccessful << Qt::endl;
    a.installTranslator(trans);
    //QCoreApplication::installTranslator(trans);
  }

  MainWindow w;

  w.show();
  return a.exec();
}
