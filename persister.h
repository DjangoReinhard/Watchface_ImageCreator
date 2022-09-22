#ifndef PERSISTER_H
#define PERSISTER_H
#include <QObject>
#include <QMap>
#include <ElementInfo.h>
class ImageBuilder;
class WatchFace;


class Persister : public QObject
{
  Q_OBJECT
public:
  Persister(WatchFace* face, ImageBuilder* ib, const QString& symbolBase, QObject* parent = nullptr);

  void    storeImages(const QString& faceDir, const QString& oldFaceDir);

protected:
  QString getSampleText(int n, int l, const QStringList& list);
  void    storeImage(const QString& baseDir, const ElementInfo* cfg);
  void    storeTextSequence(const QString& baseDir, const ElementInfo* cfg);
  void    storeNumSequence(const QString& baseDir, const ElementInfo* cfg);
  void    storeFreeText(const QString& baseDir, const ElementInfo* cfg);
  void    storeSymbol(const QString& baseDir, const QString& symbolBase, const QString& oldFaceDir, ElementInfo* cfg);
  void    storeWeatherSymbols(const QString& baseDir, const QString& symbolBase, const QString& oldFaceDir, ElementInfo* cfg);
  void    storeGauge(const QString& baseDir, ElementInfo* cfg);

private:
  WatchFace*           wf;
  ImageBuilder*        ib;
  QString              symbolBase;
  QMap<QChar, QString> nameOf;
};

#endif // PERSISTER_H
