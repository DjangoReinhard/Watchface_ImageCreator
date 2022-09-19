#ifndef ELEMENTINFO_H
#define ELEMENTINFO_H
#include <QString>
#include <QFont>
#include <QColor>
#include <QPainterPath>
#include <QImage>


enum ElemType {
  Unknown
, Background
, SingleDigit
, TextSequence
, FreeText
, Symbol
, Gauge
};


typedef struct
{
  QString       id;
  QString       name;
  QString       path;
  int           x;
  int           y;
  int           rotation;
  int           max;
  ElemType      type;
  QString       sample;
  QFont         font;
  QColor        fgCol0;
  QColor        fgCol1;
  QColor        bgColor;
  QPainterPath  pp;
  QImage        image;
  } ElementInfo;
#endif // ELEMENTINFO_H
