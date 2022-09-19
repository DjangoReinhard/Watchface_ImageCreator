#ifndef FACEPAINTER_H
#define FACEPAINTER_H
#include <QObject>
#include <QImage>
#include <QPainterPath>
class WatchFace;


typedef struct
{
  int           rotation;
  QPainterPath  pp;
  QString       sample;
  QImage        image;
  } ElementHelper;

class FacePainter : public QObject
{
  Q_OBJECT
public:
  explicit FacePainter(WatchFace* face, QObject *parent = nullptr);

  void setRotation(int elem, int rotation);

private:
  WatchFace*   face;
  };
#endif // FACEPAINTER_H
