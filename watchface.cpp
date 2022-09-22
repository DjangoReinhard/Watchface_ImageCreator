#include "watchface.h"
#include <QDebug>
#include <QSettings>


WatchFace::WatchFace(int width, int height, const QString& symbolBase, QObject *parent)
 : QObject(parent)
 , dows(QStringList() << "Monday"
                      << "Tuesday"
                      << "Wednesday"
                      << "Thursday"
                      << "Friday"
                      << "Saturday"
                      << "Sunday"
                      )
 , lDows(QStringList() << tr("Monday")
                       << tr("Tuesday")
                       << tr("Wednesday")
                       << tr("Thursday")
                       << tr("Friday")
                       << tr("Saturday")
                       << tr("Sunday")
                       )
 , months(QStringList() << "January"
                        << "February"
                        << "March"
                        << "April"
                        << "May"
                        << "June"
                        << "July"
                        << "August"
                        << "September"
                        << "October"
                        << "November"
                        << "December"
                        )
, lMonths(QStringList() << tr("January")
                        << tr("February")
                        << tr("March")
                        << tr("April")
                        << tr("May")
                        << tr("June")
                        << tr("July")
                        << tr("August")
                        << tr("September")
                        << tr("October")
                        << tr("November")
                        << tr("December")
                        )
 , w(width)
 , h(height) {
  setupDefaults(symbolBase);
  }


QColor WatchFace::bgColor(int elem) const {
  if (elem < 0 || elem >= MaxElements) return QColor();
  return cfg[elem].bgColor;
  }


const ElementInfo* WatchFace::elementInfo(int elem) const {
  if (elem < 0 || elem >= MaxElements) return nullptr;
  return &cfg[elem];
  }


ElementInfo* WatchFace::elemInfo(int elem) {
  if (elem < 0 || elem >= MaxElements) return nullptr;
  return &cfg[elem];
  }


QColor WatchFace::fgColor0(int elem) const {
  if (elem < 0 || elem >= MaxElements) return QColor();
  return cfg[elem].fgCol0;
  }


QColor WatchFace::fgColor1(int elem) const {
  if (elem < 0 || elem >= MaxElements) return QColor();
  return cfg[elem].fgCol1;
  }


QFont WatchFace::font(int elem) const {
  if (elem < 0 || elem >= MaxElements) return QFont();
  return cfg[elem].font;
  }


QString WatchFace::id(int elem) const {
  if (elem < 0 || elem >= MaxElements) return QString();
  return cfg[elem].id;
  }


void WatchFace::loadFace(const QString &fileName) {
  QSettings config(fileName, QSettings::IniFormat);

  for (int i=0; i < WatchFace::MaxElements; ++i) {
      ElementInfo* c = elemInfo(i);
      QString      np;

      config.beginGroup(c->id);
      qDebug() << "read settings from group" << config.group();

      c->x = config.value("x").toInt();
      c->y = config.value("y").toInt();
      c->rotation = config.value("rot").toDouble();
      np = config.value("path").toString();
      if (!(np.isNull() || np.isEmpty())) c->path = np;
      c->font = config.value("font").value<QFont>();
      c->fgCol0 = config.value("fg0").value<QColor>();
      c->fgCol1 = config.value("fg1").value<QColor>();
      c->bgColor = config.value("bg").value<QColor>();
      config.endGroup();
      }
  }


int WatchFace::rotation(int elem) const {
  if (elem < 0 || elem >= MaxElements) return 0;
  return cfg[elem].rotation;
  }


void WatchFace::save(const QString &fileName) {
  QSettings config(fileName, QSettings::IniFormat);

  for (int i=0; i < MaxElements; ++i) {
      const ElementInfo* c = elementInfo(i);

      config.beginGroup(c->id);
      config.setValue("x",    c->x);
      config.setValue("y",    c->y);
      config.setValue("rot",  c->rotation);
      config.setValue("path", c->path);
      config.setValue("font", c->font);
      config.setValue("fg0",  c->fgCol0);
      config.setValue("fg1",  c->fgCol1);
      config.setValue("bg",   c->bgColor);
      config.endGroup();
      }
  config.sync();
  }


void WatchFace::setBackground(int elem, const QColor& color) {
  if (elem < 0 || elem >= MaxElements) return;
  cfg[elem].bgColor = color;
  }


void WatchFace::setForeground0(int elem, const QColor& color) {
  if (elem < 0 || elem >= MaxElements) return;
  cfg[elem].fgCol0 = color;
  }


void WatchFace::setForeground1(int elem, const QColor& color) {
  if (elem < 0 || elem >= MaxElements) return;
  cfg[elem].fgCol1 = color;
  }


void WatchFace::setFont(int elem, const QFont& font) {
  if (elem < 0 || elem >= MaxElements) return;
  cfg[elem].font = font;
  }


void WatchFace::setHeight(int height) {
  h = height;
  }


void WatchFace::setPath(int elem, const QString& p) {
  if (elem < 0 || elem >= MaxElements) return;
  if (p.isEmpty()) return;
  cfg[elem].path = p;
  }


void WatchFace::setRotation(int elem, int angle) {
  if (elem < 0 || elem >= MaxElements) return;
  cfg[elem].rotation = angle;
  }


void WatchFace::setSample(int elem, const QString &text) {
  if (elem < 0 || elem >= MaxElements) return;
  cfg[elem].sample = text;
  }


void WatchFace::setupDefaults(const QString& symbolBase) {
  cfg[ 0].id = "background";
  cfg[ 0].name = tr("background");
  cfg[ 0].sample = "background";
  cfg[ 0].path = "background";
  cfg[ 0].x = 0;
  cfg[ 0].y = 0;
  cfg[ 0].max = 0;
  cfg[ 0].rotation = 0;
  cfg[ 0].type = Background;
  cfg[ 0].font = QFont("DejaVu Sans Condensed", 20);
  cfg[ 0].bgColor = Qt::black;
  cfg[ 0].fgCol0  = Qt::black;
  cfg[ 0].fgCol1  = Qt::black;

  cfg[ 1].id = "left hour";
  cfg[ 1].name = tr("left hour");
  cfg[ 1].sample = "0";
  cfg[ 1].path = "hour/left";
  cfg[ 1].x = 7;
  cfg[ 1].y = 220;
  cfg[ 1].max = 3;
  cfg[ 1].rotation = 10;
  cfg[ 1].type = SingleDigit;
  cfg[ 1].font = QFont("DejaVu Sans Condensed", 120, 80, true);
  cfg[ 1].bgColor = QColor(0, 0, 0, 0);
  cfg[ 1].fgCol0  = Qt::white;
  cfg[ 1].fgCol1  = Qt::red;

  cfg[ 2].id = "right hour";
  cfg[ 2].name = tr("right hour");
  cfg[ 2].sample = "8";
  cfg[ 2].path = "hour/right";
  cfg[ 2].x = 0;
  cfg[ 2].y = 0;
  cfg[ 2].max = 10;
  cfg[ 2].rotation = 0;
  cfg[ 2].type = SingleDigit;
  cfg[ 2].font = QFont("DejaVu Sans Condensed", 20);
  cfg[ 2].bgColor = QColor(0, 0, 0, 0);
  cfg[ 2].fgCol0  = Qt::white;
  cfg[ 2].fgCol1  = Qt::white;

  cfg[ 3].id = "left minute";
  cfg[ 3].name = tr("left minute");
  cfg[ 3].sample = "5";
  cfg[ 3].path = "minute/left";
  cfg[ 3].x = 0;
  cfg[ 3].y = 0;
  cfg[ 3].max = 6;
  cfg[ 3].rotation = 0;
  cfg[ 3].type = SingleDigit;
  cfg[ 3].font = QFont("DejaVu Sans Condensed", 20);
  cfg[ 3].bgColor = QColor(0, 0, 0, 0);
  cfg[ 3].fgCol0  = Qt::white;
  cfg[ 3].fgCol1  = Qt::white;

  cfg[ 4].id = "right minute";
  cfg[ 4].name = tr("right minute");
  cfg[ 4].sample = "9";
  cfg[ 4].path = "minute/right";
  cfg[ 4].x = 0;
  cfg[ 4].y = 0;
  cfg[ 4].max = 10;
  cfg[ 4].rotation = 0;
  cfg[ 4].type = SingleDigit;
  cfg[ 4].font = QFont("DejaVu Sans Condensed", 20);
  cfg[ 4].bgColor = QColor(0, 0, 0, 0);
  cfg[ 4].fgCol0  = Qt::white;
  cfg[ 4].fgCol1  = Qt::white;

  cfg[ 5].id = "left day of month";
  cfg[ 5].name = tr("left day of month");
  cfg[ 5].sample = "2";
  cfg[ 5].path = "dom/left";
  cfg[ 5].x = 0;
  cfg[ 5].y = 0;
  cfg[ 5].max = 4;
  cfg[ 5].rotation = 0;
  cfg[ 5].type = SingleDigit;
  cfg[ 5].font = QFont("DejaVu Sans Condensed", 20);
  cfg[ 5].bgColor = QColor(0, 0, 0, 0);
  cfg[ 5].fgCol0  = Qt::white;
  cfg[ 5].fgCol1  = Qt::white;

  cfg[ 6].id = "right day of month";
  cfg[ 6].name = tr("right day of month");
  cfg[ 6].sample = "9";
  cfg[ 6].path = "dom/right";
  cfg[ 6].x = 0;
  cfg[ 6].y = 0;
  cfg[ 6].max = 10;
  cfg[ 6].rotation = 0;
  cfg[ 6].type = SingleDigit;
  cfg[ 6].font = QFont("DejaVu Sans Condensed", 20);
  cfg[ 6].bgColor = QColor(0, 0, 0, 0);
  cfg[ 6].fgCol0  = Qt::white;
  cfg[ 6].fgCol1  = Qt::white;

  cfg[ 7].id = "month";
  cfg[ 7].name = tr("month");
  cfg[ 7].sample = "7;4";
  cfg[ 7].path = "month";
  cfg[ 7].x = 0;
  cfg[ 7].y = 0;
  cfg[ 7].max = 12;
  cfg[ 7].rotation = 0;
  cfg[ 7].type = TextSequence;
  cfg[ 7].font = QFont("DejaVu Sans Condensed", 20);
  cfg[ 7].bgColor = QColor(0, 0, 0, 0);
  cfg[ 7].fgCol0  = Qt::white;
  cfg[ 7].fgCol1  = Qt::white;

  cfg[ 8].id = "day of week";
  cfg[ 8].name = tr("day of week");
  cfg[ 8].sample = "4;4";
  cfg[ 8].path = "week";
  cfg[ 8].x = 0;
  cfg[ 8].y = 0;
  cfg[ 8].max = 7;
  cfg[ 8].rotation = 0;
  cfg[ 8].type = TextSequence;
  cfg[ 8].font = QFont("DejaVu Sans Condensed", 20);
  cfg[ 8].bgColor = QColor(0, 0, 0, 0);
  cfg[ 8].fgCol0  = Qt::white;
  cfg[ 8].fgCol1  = Qt::white;

  cfg[ 9].id = "temperature";
  cfg[ 9].name = tr("temperature");
  cfg[ 9].sample = "-23°";
  cfg[ 9].path = "temperature";
  cfg[ 9].x = 0;
  cfg[ 9].y = 0;
  cfg[ 9].max = 10;
  cfg[ 9].rotation = 0;
  cfg[ 9].type = FreeText;
  cfg[ 9].font = QFont("DejaVu Sans Condensed", 20);
  cfg[ 9].bgColor = QColor(0, 0, 0, 0);
  cfg[ 9].fgCol0  = Qt::white;
  cfg[ 9].fgCol1  = Qt::white;

  cfg[10].id = "power";
  cfg[10].name = tr("power");
  cfg[10].sample = "95%";
  cfg[10].path = "power";
  cfg[10].x = 0;
  cfg[10].y = 0;
  cfg[10].max = 10;
  cfg[10].rotation = 0;
  cfg[10].type = FreeText;
  cfg[10].font = QFont("DejaVu Sans Condensed", 20);
  cfg[10].bgColor = QColor(0, 0, 0, 0);
  cfg[10].fgCol0  = QColor(0, 255, 0);
  cfg[10].fgCol1  = QColor(0, 255, 0, 110);

  cfg[11].id = "steps";
  cfg[11].name = tr("steps");
  cfg[11].sample = "5381";
  cfg[11].path = "steps";
  cfg[11].x = 0;
  cfg[11].y = 0;
  cfg[11].max = 10;
  cfg[11].rotation = 0;
  cfg[11].type = FreeText;
  cfg[11].font = QFont("DejaVu Sans Condensed", 20);
  cfg[11].bgColor = QColor(0, 0, 0, 0);
  cfg[11].fgCol0  = QColor(255, 170, 0);
  cfg[11].fgCol1  = QColor(255, 170, 110, 110);

  cfg[12].id = "pulse";
  cfg[12].name = tr("pulse");
  cfg[12].sample = "107";
  cfg[12].path = "pulse";
  cfg[12].x = 0;
  cfg[12].y = 0;
  cfg[12].max = 10;
  cfg[12].rotation = 0;
  cfg[12].type = FreeText;
  cfg[12].font = QFont("DejaVu Sans Condensed", 20, 20);
  cfg[12].bgColor = QColor(0, 0, 0, 0);
  cfg[12].fgCol0  = Qt::white;
  cfg[12].fgCol1  = Qt::white;

  cfg[13].id = "calories";
  cfg[13].name = tr("calories");
  cfg[13].sample = "213";
  cfg[13].path = "calories";
  cfg[13].x = 0;
  cfg[13].y = 0;
  cfg[13].max = 10;
  cfg[13].rotation = 0;
  cfg[13].type = FreeText;
  cfg[13].font = QFont("DejaVu Sans Condensed", 20, 20);
  cfg[13].bgColor = QColor(0, 0, 0, 0);
  cfg[13].fgCol0  = Qt::white;
  cfg[13].fgCol1  = Qt::white;

  cfg[14].id = "rain";
  cfg[14].name = tr("rain");
  cfg[14].sample = "55";
  cfg[14].path = "rain";
  cfg[14].x = 0;
  cfg[14].y = 0;
  cfg[14].max = 10;
  cfg[14].rotation = 0;
  cfg[14].type = FreeText;
  cfg[14].font = QFont("DejaVu Sans Condensed", 20, 20);
  cfg[14].bgColor = QColor(0, 0, 0, 0);
  cfg[14].fgCol0  = Qt::white;
  cfg[14].fgCol1  = Qt::white;

  cfg[15].id = "symbol power";
  cfg[15].name = tr("symbol power");
  cfg[15].sample = "power";
  cfg[15].path = symbolBase + "/other_symbols/";
  cfg[15].x = 0;
  cfg[15].y = 0;
  cfg[15].max = 0;
  cfg[15].rotation = 0;
  cfg[15].type = Symbol;
  cfg[15].font = QFont("DejaVu Sans Condensed", 20, 20);
  cfg[15].bgColor = Qt::blue;
  cfg[15].fgCol0  = Qt::white;
  cfg[15].fgCol1  = Qt::black;

  cfg[16].id = "symbol steps";
  cfg[16].name = tr("symbol steps");
  cfg[16].sample = "steps";
  cfg[16].path = symbolBase + "/other_symbols/";
  cfg[16].x = 0;
  cfg[16].y = 0;
  cfg[16].max = 0;
  cfg[16].rotation = 0;
  cfg[16].type = Symbol;
  cfg[16].font = QFont("DejaVu Sans Condensed", 20, 20);
  cfg[16].bgColor = Qt::blue;
  cfg[16].fgCol0  = Qt::white;
  cfg[16].fgCol1  = Qt::black;

  cfg[17].id = "symbol pulse";
  cfg[17].name = tr("symbol pulse");
  cfg[17].sample = "heart";
  cfg[17].path = symbolBase + "/other_symbols/";
  cfg[17].x = 0;
  cfg[17].y = 0;
  cfg[17].max = 0;
  cfg[17].rotation = 0;
  cfg[17].type = Symbol;
  cfg[17].font = QFont("DejaVu Sans Condensed", 20, 20);
  cfg[17].bgColor = Qt::blue;
  cfg[17].fgCol0  = Qt::white;
  cfg[17].fgCol1  = Qt::white;

  cfg[18].id = "symbol calories";
  cfg[18].name = tr("symbol calories");
  cfg[18].sample = "fire";
  cfg[18].path = symbolBase + "/other_symbols/";
  cfg[18].x = 0;
  cfg[18].y = 0;
  cfg[18].max = 10;
  cfg[18].rotation = 0;
  cfg[18].type = Symbol;
  cfg[18].font = QFont("DejaVu Sans Condensed", 20, 20);
  cfg[18].bgColor = Qt::blue;
  cfg[18].fgCol0  = Qt::white;
  cfg[18].fgCol1  = Qt::white;

  cfg[19].id = "symbol rain";
  cfg[19].name = tr("symbol rain");
  cfg[19].sample = "rain";
  cfg[19].path = symbolBase + "/other_symbols/";
  cfg[19].x = 0;
  cfg[19].y = 0;
  cfg[19].max = 0;
  cfg[19].rotation = 0;
  cfg[19].type = Symbol;
  cfg[19].font = QFont("DejaVu Sans Condensed", 20, 20);
  cfg[19].bgColor = Qt::blue;
  cfg[19].fgCol0  = Qt::white;
  cfg[19].fgCol1  = Qt::white;

  cfg[20].id = "symbol weather";
  cfg[20].name = tr("symbol weather");
  cfg[20].sample = "07";
  cfg[20].path = symbolBase + "/weather_symbols/";
  cfg[20].x = 0;
  cfg[20].y = 0;
  cfg[20].max = 29;
  cfg[20].rotation = 0;
  cfg[20].type = Symbol;
  cfg[20].font = QFont("DejaVu Sans Condensed", 20, 20);
  cfg[20].bgColor = Qt::blue;
  cfg[20].fgCol0  = Qt::white;
  cfg[20].fgCol1  = Qt::black;

  cfg[21].id = "gauge power";
  cfg[21].name = tr("gauge power");
  cfg[21].sample = "95";
  cfg[21].path = "gauge/top";
  cfg[21].x = 0;
  cfg[21].y = 0;
  cfg[21].max = 10;
  cfg[21].rotation = 0;
  cfg[21].type = Gauge;
  cfg[21].font = QFont("DejaVu Sans Condensed", 20, 20);
  cfg[21].bgColor = Qt::black;
  cfg[21].fgCol0  = QColor(0, 255, 0,  90);
  cfg[21].fgCol1  = QColor(0, 255, 0, 255);

  cfg[22].id = "gauge steps";
  cfg[22].name = tr("gauge steps");
  cfg[22].sample = "77";
  cfg[22].path = "gauge/bottom";
  cfg[22].x = 0;
  cfg[22].y = 0;
  cfg[22].max = 10;
  cfg[22].rotation = 0;
  cfg[22].type = Gauge;
  cfg[22].font = QFont("DejaVu Sans Condensed", 20, 20);
  cfg[22].bgColor = Qt::black;
  cfg[22].fgCol0  = QColor(255, 170, 0,  90);
  cfg[22].fgCol1  = QColor(255, 170, 0, 255);

  for (int i=0; i < MaxElements; ++i) {
      en << cfg[i].name;
      }
  }


void WatchFace::setWidth(int width) {
  w = width;
  }


void WatchFace::setX(int elem, int x) {
  if (elem < 0 || elem >= MaxElements) return;
  cfg[elem].x = x;
  }


void WatchFace::setY(int elem, int y) {
  if (elem < 0 || elem >= MaxElements) return;
  cfg[elem].y = y;
  }


