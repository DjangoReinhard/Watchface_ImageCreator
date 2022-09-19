#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <ElementInfo.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class ImageBuilder;
class Persister;
class WatchFace;


class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit MainWindow(const QString& symbolBase, QWidget *parent = nullptr);
  virtual ~MainWindow();

public slots:
  void angleChanged(int angle);
  void chooseFont();
  void chooseColor0();
  void chooseColor1();
  void chooseBackground();
  void heightChanged();
  void openDir();
  void openFile();
  void posChanged();
  void powerChanged();
  void sampleChanged();
  void save();
  void selectElement(int index);
  void stepsChanged();
  void test();
  void widthChanged();

protected:
  void rebuildPath();
  void refresh();

private:
  Ui::MainWindow*  ui;
  WatchFace*       wf;
  ImageBuilder*    ib;
  Persister*       pers;
  const QString&   symbolBase;
  int              curElem;
  };
#endif // MAINWINDOW_H
