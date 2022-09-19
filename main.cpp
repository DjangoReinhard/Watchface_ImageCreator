#include "mainwindow.h"
#include <QDebug>
#include <QApplication>
#include <QLocale>
#include <QTranslator>


int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QTranslator translator;
  QStringList uiLanguages = QLocale::system().uiLanguages();
  const QString symbolBase = "/home/django/misc/mi.watchface/Band7";

//  for (const QString& locale : uiLanguages) {
//      qDebug() << "got ui-language: >>" << QLocale(locale).name() << "<<";
//      }
//  if (!uiLanguages.contains("de") || !uiLanguages.contains("de_DE"))
//     uiLanguages.append("de");
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
