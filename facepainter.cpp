#include "facepainter.h"
#include "watchface.h"


FacePainter::FacePainter(WatchFace* face, QObject *parent)
 : QObject(parent)
 , face(face) {
  }
