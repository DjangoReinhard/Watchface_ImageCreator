#include "persister.h"
#include "imagebuilder.h"
#include "watchface.h"
#include "persister.h"
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include <cmath>


Persister::Persister(WatchFace* face, ImageBuilder* ib, const QString& symbolBase, QObject* parent)
 : QObject(parent)
 , wf(face)
 , ib(ib)
 , symbolBase(symbolBase) {
  this->nameOf[0260] = "degree";
  this->nameOf['%']  = "percent";
  this->nameOf['-']  = "minus";
  }


QString Persister::getSampleText(int n, int l, const QStringList& list) {
  if (n >= list.size()) n = list.size() - 1;
  QString text = list.at(n);

  if (l) text = text.mid(0, l);
  return text;
  }


void Persister::storeImages(const QString& faceDir, const QString& oldFaceDir) {
  for (int i=0; i < WatchFace::MaxElements; ++i) {
      const ElementInfo* cfg = wf->elementInfo(i);

      switch (cfg->type) {
        case Background:
             storeImage(faceDir, cfg);
             break;
        case TextSequence:
             storeTextSequence(faceDir, cfg);
             break;
        case SingleDigit:
             storeNumSequence(faceDir, cfg);
             break;
        case FreeText:
             storeFreeText(faceDir, cfg);
             break;
        case Symbol:
             if (cfg->id == "symbol weather")
                storeWeatherSymbols(faceDir, symbolBase, oldFaceDir, wf->elemInfo(i));
             else
                storeSymbol(faceDir, symbolBase, oldFaceDir, wf->elemInfo(i));
             break;
        case Gauge:
             storeGauge(faceDir, wf->elemInfo(i));
             break;
        default:
             break;
        }
    }
  }


void Persister::storeImage(const QString& baseDir, const ElementInfo* cfg) {
  QFileInfo fi(baseDir + "/" + cfg->path + QString("/%1.png").arg(cfg->sample));
  QImage    img = cfg->image;
  QDir      dir;

  qDebug() << "filename is" << fi.absoluteFilePath();

  dir.mkpath(baseDir + "/" + cfg->path);
  QFile::remove(fi.absoluteFilePath());
  if (!img.save(fi.absoluteFilePath(), "PNG")) {
     qDebug() << "OUPS, failed to save image to file >|" << fi.absoluteFilePath() << "|<";
     }
  }


void Persister::storeTextSequence(const QString& baseDir, const ElementInfo* cfg) {
  if (!cfg) return;
  QStringList parts = cfg->sample.split(";");
  QStringList sl;
  QDir        td;
  QString     lPath = cfg->path + "_de";
  int n = fmax(0, parts.at(0).toInt());
  int l = 3;

  if (parts.size() > 1) l = parts.at(1).toInt();

  // first localized texts
  if (cfg->id == "month")
     sl = wf->lMonths;
  else if (cfg->id == "day of week")
     sl = wf->lDows;

  for (int i=0; i < sl.size(); ++i) {
      QImage  img  = ib->createImage(cfg, getSampleText(i, l, sl));
      QString fn   = baseDir + "/" + lPath + QString("/%1.png").arg(i);

      qDebug() << "filename is" << fn;

      td.mkpath(baseDir + "/" + lPath);
      QFile::remove(fn);
      if (!img.save(fn, "PNG")) {
         qDebug() << "OUPS, failed to save image to file >|" << fn << "|<";
         }
      }

  // now the original texts
  if (cfg->id == "month")
     sl = wf->months;
  else if (cfg->id == "day of week")
     sl = wf->dows;

  for (int i=0; i < sl.size(); ++i) {
      QImage  img  = ib->createImage(cfg, getSampleText(i, l, sl));
      QString fn   = baseDir + "/" + cfg->path + QString("/%1.png").arg(i);

      qDebug() << "filename is" << fn;

      td.mkpath(baseDir + "/" + cfg->path);
      QFile::remove(fn);
      if (!img.save(fn, "PNG")) {
         qDebug() << "OUPS, failed to save image to file >|" << fn << "|<";
         }
      }
  }


void Persister::storeNumSequence(const QString& baseDir, const ElementInfo* cfg) {
  if (!cfg) return;
  QDir dir;

  for (int i=0; i < cfg->max; ++i) {
      QString text = QString("%1").arg(i);
      QImage  img  = ib->createImage(cfg, text);
      QString fn   = baseDir + "/" + cfg->path + QString("/%1.png").arg(i);

      qDebug() << "filename is" << fn;

      dir.mkpath(baseDir + "/" + cfg->path);
      QFile::remove(fn);
      if (!img.save(fn, "PNG")) {
         qDebug() << "OUPS, failed to save image to file >|" << fn << "|<";
         }
      }
  }


// free text may be number sequence or number sequence followed by unit characters
void Persister::storeFreeText(const QString& baseDir, const ElementInfo* cfg) {
  if (!cfg) return;
  QString nonDigits;
  QDir    dir;

  for (int i=0; i < cfg->sample.size(); ++i) {
      QChar c = cfg->sample.at(i);

      if (c.isDigit()) continue;
      nonDigits += c;
      }
  // process numbers first
  for (int i=0; i < cfg->max; ++i) {
      QString text = QString("%1").arg(i);
      QImage  img  = ib->createImage(cfg, text);
      QString fn   = baseDir + "/" + cfg->path + QString("/%1.png").arg(i);

      qDebug() << "filename is" << fn;

      dir.mkpath(baseDir + "/" + cfg->path);
      QFile::remove(fn);
      if (!img.save(fn, "PNG")) {
         qDebug() << "OUPS, failed to save image to file >|" << fn << "|<";
         }
      }
  // now the unit characters
  for (int i=0; i < nonDigits.size(); ++i) {
      QChar   text = nonDigits.at(i);
      QImage  img  = ib->createImage(cfg, text);
      QString fn   = baseDir + "/" + cfg->path + QString("/%1.png").arg(nameOf[text]);

      qDebug() << "filename is" << fn;

      QFile::remove(fn);
      if (!img.save(fn, "PNG")) {
         qDebug() << "OUPS, failed to save image to file >|" << fn << "|<";
         }
      }
  }


void Persister::storeSymbol(const QString& faceDir, const QString& symbolBase, const QString& oldFaceDir, ElementInfo* cfg) {
  if (!cfg) return;
  QString relPath = cfg->path;
  QString source;

  if (cfg->path.startsWith("/")) {
     source  = cfg->path + "/" + cfg->sample + ".png";
     relPath = cfg->path.mid(symbolBase.size()).remove("/");
     }
  else
     source = QString("%1/%2/%3.png").arg(oldFaceDir, relPath, cfg->sample);
  QString dest = QString("%1/%2/%3.png").arg(faceDir, relPath, cfg->sample);

  qDebug() << "storeSymbol - relPath:" << relPath << "source:" << source << "dest:" << dest;
  if (dest != source) {
     QDir dir;

     dir.mkpath(faceDir + "/" + relPath);
     cfg->path = relPath;
     QFile::remove(dest);
     if (!QFile::copy(source, dest)) {
        qDebug() << "OUPS, failed to copy" << source << "(symbol) to >|" << dest << "|<";
        }
     }
  }


void Persister::storeWeatherSymbols(const QString& faceDir, const QString& symbolBase, const QString& oldFaceDir, ElementInfo* cfg) {
  if (!cfg) return;
  QString srcDir  = oldFaceDir;
  QString relPath = cfg->path;

  if (cfg->path.startsWith("/")) {
     srcDir  = cfg->path;
     relPath = cfg->path.mid(symbolBase.size()).remove("/");
     }
  else
     srcDir = QString("%1/%2").arg(oldFaceDir, relPath);
  QString dstDir = QString("%1/%2").arg(faceDir, relPath);

  if (dstDir != srcDir) {
     QDir dir;

     dir.mkpath(faceDir + "/" + relPath);
     cfg->path = relPath;
     for (int i=0; i < 29; ++i) {
         QString src = srcDir + QString("/%1.png").arg(i, 2, 10, QChar('0'));
         QString dst = dstDir + QString("/%1.png").arg(i, 2, 10, QChar('0'));

         qDebug() << "copy weather symbol from" << src << "to" << dst;

         QFile::remove(dst);
         if (!QFile::copy(src, dst)) {
            qDebug() << "OUPS, failed to copy" << src << "(weather-symbol) to >|" << dst << "|<";
            }
         }
     }
  }


void Persister::storeGauge(const QString& baseDir, ElementInfo* cfg) {
  if (!cfg) return;
  double stepSize = 100.0 / (double)(cfg->max);
  void (ImageBuilder::*func)(QImage& i, ElementInfo* cfg, int s, const QColor& c);
  QDir dir;
  int  i;

  if (cfg->path.endsWith("top")) func = &ImageBuilder::createGaugeI;
  else                           func = &ImageBuilder::createGaugeII;
  for (i=0; i < cfg->max; ++i) {
      QImage  scratch(wf->width(), wf->height(), QImage::Format_ARGB32);
      QString fn = baseDir + "/" + cfg->path + QString("/%1.png").arg(i);
      int     n  = (int)((double)i * stepSize);

      qDebug() << "filename is" << fn;

      (ib->*(func))(scratch, cfg, 100, cfg->fgCol0);
      (ib->*(func))(scratch, cfg, n, cfg->fgCol1);
      dir.mkpath(baseDir + "/" + cfg->path);
      QFile::remove(fn);
      if (!scratch.save(fn, "PNG")) {
         qDebug() << "OUPS, failed to save image to file >|" << fn << "|<";
         }
      }
  QImage  scratch(wf->width(), wf->height(), QImage::Format_ARGB32);
  QString fn = baseDir + "/" + cfg->path + QString("/%1.png").arg(i);

  qDebug() << "filename is" << fn;

  (ib->*(func))(scratch, cfg, 100, cfg->fgCol1);
  dir.mkpath(baseDir + "/" + cfg->path);
  QFile::remove(fn);
  if (!scratch.save(fn, "PNG")) {
     qDebug() << "OUPS, failed to save image to file >|" << fn << "|<";
     }
  }

