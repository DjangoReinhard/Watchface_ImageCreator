#include "imagebuilder.h"
#include "watchface.h"
#include "ElementInfo.h"
#include <QDebug>
#include <QDir>
#include <stdexcept>
#include <QPainter>
#include <QPainterPath>
#include <cmath>


ImageBuilder::ImageBuilder(WatchFace* face, const QString& symbolBase, const QString& faceDir, QObject *parent)
 : QObject(parent)
 , wf(face)
 , base(faceDir)
 , symbolBase(symbolBase) {
  createDirs(faceDir);
  }


QImage ImageBuilder::assembleImage() {
  QImage   scratch(wf->width(), wf->height(), QImage::Format_ARGB32);
  QPainter p;

  if (p.begin(&scratch)) {
     p.setRenderHint(QPainter::Antialiasing);
     p.setCompositionMode(QPainter::CompositionMode_Source);
     p.fillRect(scratch.rect(), Qt::transparent);
     p.setCompositionMode(QPainter::CompositionMode_SourceOver);

     for (int i=0; i < WatchFace::MaxElements; ++i) {
         ElementInfo* cfg = wf->elemInfo(i);

         p.drawImage(cfg->x, cfg->y, cfg->image);
         }
     p.end();
     }
  return scratch;
  }


void ImageBuilder::buildPower(ElementInfo* cfg) {
  QImage scratch(wf->width(), wf->height(), QImage::Format_ARGB32);
  int    scale = cfg->sample.toInt();

  createGaugeI(scratch, cfg, 100, cfg->fgCol0);
  createGaugeI(scratch, cfg, scale, cfg->fgCol1);
  cfg->image = scratch;
  }


void ImageBuilder::buildSteps(ElementInfo* cfg) {
  QImage scratch(wf->width(), wf->height(), QImage::Format_ARGB32);
  int    scale = cfg->sample.toInt();

  createGaugeII(scratch, cfg, 100, cfg->fgCol0);
  createGaugeII(scratch, cfg, scale, cfg->fgCol1);
  cfg->image = scratch;
  }


void ImageBuilder::createBackground() {
  ElementInfo* cfg = wf->elemInfo(0);
  QImage       scratch(wf->width(), wf->height(), QImage::Format_ARGB32);
  int          w = wf->width();
  int          h = wf->height();

  cfg->pp.clear();
  cfg->pp.moveTo(w, w / 2);
  cfg->pp.arcTo(QRectF(0, 0, w, w), 0, 180);
  cfg->pp.lineTo(0, h - w / 2);
  cfg->pp.arcTo(QRectF(0, h - w, w, w), -180, 180);
  cfg->pp.closeSubpath();
  drawImage(scratch, cfg, cfg->pp);
  cfg->image = scratch;
  }


void ImageBuilder::createDirs(const QString &basePath) {
  QDir    subDir(basePath);

  base = subDir.absolutePath();
  for (int i=0; i < WatchFace::MaxElements; ++i) {
      const ElementInfo* cfg = wf->elementInfo(i);

      if (!cfg) continue;
      subDir.mkpath(cfg->path);
      }
  }


void ImageBuilder::createElement(int elem, ElementInfo* cfg) {
  if (!cfg) cfg = wf->elemInfo(elem);
  if (!cfg) return;
  switch (cfg->type) {
    case Background:
         createBackground();
         break;
    case TextSequence: {
         QStringList sl;

         if (cfg->path == "month")
            sl = wf->lMonths;
         else if (cfg->path == "week")
            sl = wf->lDows;
         cfg->image = createImage(cfg, getSampleText(cfg->sample, sl));
         } break;
    case SingleDigit:
    case FreeText:
         cfg->image = createImage(cfg, cfg->sample);
         break;
    case Symbol:
         if (cfg->image.isNull()) {
            QString fileName = QString("%1/%2/%3.png").arg(base
                                                         , cfg->path
                                                         , cfg->sample);

            if (cfg->path.startsWith("/"))
               fileName = QString("%1/%2.png").arg(cfg->path
                                                 , cfg->sample);
            cfg->image = QImage(fileName);
            }
         break;
    case Gauge:
         if (cfg->path.endsWith("top")) {
            buildPower(cfg);
            }
         else if (cfg->path.endsWith("bottom")) {
            buildSteps(cfg);
            }
         break;
    default:
         break;
    }
  }


void ImageBuilder::createGaugeI(QImage& img, ElementInfo* cfg, int scale, const QColor& c) {
  QRect  r  = img.rect();
  int    w  = r.width();
  int    h0 = r.height() - r.width() / 2 - 4;
  int    h1 = r.width() / 2;
  int    a1 = scale > 50 ? 3.6 * (scale - 50) : 0;

  cfg->pp.clear();
  cfg->pp.moveTo(w - 3, h0);
  if (scale < 50) {
     cfg->pp.lineTo(w - 3, h0 - scale * 6);
     }
  else {
     cfg->pp.lineTo(w - 3, h1);
     cfg->pp.arcTo(QRectF(3
                        , 3
                        , w - 6
                        , w - 6)
                 , 0     // start angle (degree)
                 , a1);  // arc length  (degree)

     // same way back to avoid filling!
     cfg->pp.arcTo(QRectF(3
                        , 3
                        , w - 6
                        , w - 6)
                 , a1
                 , -a1);
     cfg->pp.lineTo(w - 3, h0);
     }
  drawGauge(img, cfg, cfg->pp, c);
  }


void ImageBuilder::createGaugeII(QImage& img, ElementInfo* cfg, int scale, const QColor& c) {
  QRect  r  = img.rect();
  int    w  = r.width();
  int    h  = r.height();
  int    h0 = w / 2 + 4;
  int    h1 = h - w / 2;
  int    a1 = scale > 50 ? 3.6 * (scale - 50) : 0;

  cfg->pp.clear();
  cfg->pp.moveTo(3, h0);
  if (scale < 50) {
     cfg->pp.lineTo(3, h0 + scale * 6);
     }
  else {
     cfg->pp.lineTo(3, h1);
     cfg->pp.arcTo(QRectF(3
                        , h - w + 3
                        , w - 6
                        , w - 6)
                 , -180
                 , a1);
     // here we avoid filling without going same way back?!?
     }
  drawGauge(img, cfg, cfg->pp, c);
  }

void ImageBuilder::createImage(ElementInfo* cfg) {
  QFontMetrics fm(cfg->font);
  QRectF       r = fm.boundingRect(cfg->sample);
  QImage       scratch(r.width(), r.height(), QImage::Format_ARGB32);

  drawImage(scratch, cfg, cfg->pp);
  cfg->image = scratch;
  }


QImage ImageBuilder::createImage(const ElementInfo* cfg, const QString& text) {
  if (!cfg) throw new std::invalid_argument("missing cfg parameter");
  QFont        font = cfg->font;
  QFontMetrics fm(font);
  QRect        r = fm.boundingRect(text);
  QImage       scratch(10 + (int)((double)r.width() * 1.3)
                     , 10 + r.height()
                     , QImage::Format_ARGB32);
  if (cfg) {
     QPainterPath pp;

     pp.addText(QPointF(10 - r.x(), -r.y() + 3), font, text);
     drawImage(scratch, cfg, pp);
     }
  return scratch;
  }


void ImageBuilder::drawGauge(QImage& img, ElementInfo* cfg, QPainterPath& pp, const QColor& c) {
  QPainter     p;

  if (p.begin(&img)) {
     p.setRenderHint(QPainter::Antialiasing);
     if (c == cfg->fgCol0) {
        p.setCompositionMode(QPainter::CompositionMode_Source);
        p.fillRect(img.rect(), Qt::transparent);
        }
     p.setCompositionMode(QPainter::CompositionMode_SourceOver);
     p.setPen(QPen(c, 8, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
     p.drawPath(pp);
     p.end();
     }
  }


void ImageBuilder::drawImage(QImage& img, const ElementInfo* cfg, QPainterPath& pp) {
  QPainter     p;

  if (p.begin(&img)) {
     p.setRenderHint(QPainter::Antialiasing);
     p.setCompositionMode(QPainter::CompositionMode_Source);
     p.fillRect(img.rect(), Qt::transparent);
     p.setCompositionMode(QPainter::CompositionMode_SourceOver);
     QLinearGradient gradient(0, 0, 0, img.height() * 1.3);

     gradient.setColorAt(0, cfg->fgCol1);
     gradient.setColorAt(1, cfg->fgCol0);
     p.setPen(QPen(cfg->fgCol1, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
     p.setBrush(gradient);
     p.drawPath(pp);
     p.end();
     }
  }


QString ImageBuilder::getSampleText(const QString& sample, QStringList& list) {
  QStringList sl = sample.split(";");
  int n = fmax(0, sl.at(0).toInt());
  int l = 3;

  if (sl.size() > 1) l = sl.at(1).toInt();
  if (n >= list.size()) n = list.size() - 1;
  QString text = list.at(n);

  if (l) text = text.mid(0, l);
  return text;
  }


void ImageBuilder::setFaceDir(const QString &basePath) {
  base = basePath;
  }
