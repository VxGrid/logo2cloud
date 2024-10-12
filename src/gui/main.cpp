#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QTranslator>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  // if (QLocale::system().language() == QLocale::German)
  // {
  //   // set German translation
  //   QTranslator *trans = new QTranslator(&a);
  //   trans->load("logo2Cloud_de_GER.qm");
  //   trans->setObjectName(QString("GermanTranslation"));
  //   a.installTranslator(trans);
  // }

  MainWindow w;

  w.show();
  return a.exec();
}
