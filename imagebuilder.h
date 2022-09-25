#ifndef IMAGEBUILDER_H
#define IMAGEBUILDER_H
#include <QObject>
#include <QImage>
#include "ElementInfo.h"
class WatchFace;
class QPainterPath;


class ImageBuilder : public QObject
{
  Q_OBJECT
public:
  explicit ImageBuilder(WatchFace* face, const QString& symbolBase, const QString& faceDir, QObject *parent = nullptr);

  QImage  assembleImage();
  void    buildPower(ElementInfo* cfg);
  void    buildSteps(ElementInfo* cfg);
  void    createBackground();
  void    createDirs(const QString& basePath);
  void    createElement(int elem, ElementInfo* cfg = nullptr);
  void    createGaugeI(QImage& img, ElementInfo* cfg, int scale, const QColor& c);
  void    createGaugeII(QImage& img, ElementInfo* cfg, int scale, const QColor& c);
  void    createImage(ElementInfo* cfg);
  QImage  createImage(const ElementInfo* cfg, const QString& text);
  void    drawGauge(QImage& img, ElementInfo* cfg, QPainterPath& pp, const QColor& c);
  void    drawImage(QImage& img, const ElementInfo* cfg, QPainterPath& pp);
  QString getFaceDir() const;
  QString getSampleText(const QString& sample, QStringList& list);
  QImage  refresh(ElementInfo* cfg);
  void    setFaceDir(const QString& basePath);
  QImage  testDigit();

private:
  WatchFace*  wf;
  QString     base;
  QString     symbolBase;
  };
#endif // IMAGEBUILDER_H
