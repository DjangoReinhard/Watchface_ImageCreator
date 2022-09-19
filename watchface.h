#ifndef WATCHFACE_H
#define WATCHFACE_H
#include <QObject>
#include <ElementInfo.h>


class WatchFace : public QObject
{
  Q_OBJECT
public:
  explicit WatchFace(int width, int height, const QString& symbolBase, QObject *parent = nullptr);

  QStringList elementNames() const { return en; };
  const ElementInfo* elementInfo(int elem) const;
  int     width()  const { return w; };
  int     height() const { return h; };
  QColor  bgColor(int elem) const;
  QColor  fgColor0(int elem) const;
  QColor  fgColor1(int elem) const;
  QFont   font(int elem) const;
  QString id(int elem) const;
  int     rotation(int elem) const;
  void    save(const QString& fileName);
  void    setBackground(int elem, const QColor& color);
  void    setForeground0(int elem, const QColor& color);
  void    setForeground1(int elem, const QColor& color);
  void    setFont(int elem, const QFont& font);
  void    setPath(int elem, const QString& path);
  void    setRotation(int elem, int angle);
  void    setSample(int elem, const QString& text);
  void    setWidth(int width);
  void    setHeight(int height);
  void    setX(int elem, int x);
  void    setY(int elem, int y);

  const  QStringList dows;
  const  QStringList lDows;
  const  QStringList months;
  const  QStringList lMonths;
  static const int   MaxElements = 23;

protected:
  ElementInfo* elemInfo(int elem);
  void rebuildPath(int elem);
  void setupDefaults(const QString& symbolBase);

private:
  ElementInfo  cfg[MaxElements];
  int          w;
  int          h;
  QStringList  en;
  friend class ImageBuilder;
  friend class Persister;
  };
#endif // WATCHFACE_H
