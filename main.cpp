#include "mainwindow.h"
#include <QDebug>
#include <QApplication>
#include <QLocale>
#include <QTranslator>


int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QTranslator translator;
  QStringList uiLanguages = QLocale::system().uiLanguages();
  QString     symbolBase;

  for (int i=1; i < argc; ++i) {
      if (QString(argv[i]) == "-symbols" && argc > (i+1)) {
         symbolBase = QString(argv[++i]);
         }
      }
  for (const QString& locale : uiLanguages) {
      const QString baseName = "FaceBuilder_" + QLocale(locale).name();

      if (translator.load(":/i18n/" + baseName)) {
         a.installTranslator(&translator);
         break;
         }
      }
  MainWindow w(symbolBase);

  w.show();

  return a.exec();
  }
