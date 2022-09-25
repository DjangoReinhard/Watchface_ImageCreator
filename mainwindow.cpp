#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "imagebuilder.h"
#include "persister.h"
#include "watchface.h"
#include <QAction>
#include <QColorDialog>
#include <QComboBox>
#include <QFile>
#include <QFileDialog>
#include <QFontDialog>
#include <QImage>
#include <QLabel>
#include <QModelIndex>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QSettings>
#include <QSpinBox>
#include <QStringListModel>
#include <QDebug>


MainWindow::MainWindow(const QString& symbolBase, QWidget *parent)
 : QMainWindow(parent)
 , ui(new Ui::MainWindow)
 , wf(new WatchFace(192, 490, symbolBase, this))
 , ib(new ImageBuilder(wf, symbolBase, "/dev/null"))
 , pers(new Persister(wf, ib, symbolBase, this))
 , symbolBase(symbolBase)
 , curElem(0) {
  ui->setupUi(this);
  ui->cbElement->setModel(new QStringListModel(wf->elementNames()));
  ui->spWidth->setValue(wf->width());
  ui->spHeight->setValue(wf->height());
  ui->spPower->setValue(95);
  ui->spSteps->setValue(5381);
  ui->spMaxSteps->setValue(7000);

  connect(ui->actionNew, &QAction::triggered, this, &MainWindow::openDir);
  connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openFile);
  connect(ui->actionSave, &QAction::triggered, this, &MainWindow::save);
  connect(ui->actionExit, &QAction::triggered, this, &QWidget::close);

  connect(ui->cbElement, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::selectElement);

  connect(ui->pbFont, &QPushButton::clicked, this, &MainWindow::chooseFont);
  connect(ui->pbColor0, &QPushButton::clicked, this, &MainWindow::chooseColor0);
  connect(ui->pbColor1, &QPushButton::clicked, this, &MainWindow::chooseColor1);
  connect(ui->pbBackground, &QPushButton::clicked, this, &MainWindow::chooseBackground);
  connect(ui->spWidth, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::widthChanged);
  connect(ui->spHeight, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::heightChanged);
  connect(ui->spPower, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::powerChanged);
  connect(ui->spMaxSteps, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::stepsChanged);
  connect(ui->spSteps, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::stepsChanged);
  connect(ui->spX, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::posChanged);
  connect(ui->spY, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::posChanged);
  connect(ui->spAngle, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::angleChanged);
  connect(ui->edSample, &QLineEdit::textChanged, this, &MainWindow::sampleChanged);
  //TODO:
//  connect(ui->actionTest_2, &QAction::triggered, this, &MainWindow::test);
  }


MainWindow::~MainWindow() {
  delete ui;
  }


void MainWindow::angleChanged(int angle) {
//  wf->setRotation(curElem, angle);
//  ib->createElement(curElem);
//  refresh();
  }


void MainWindow::chooseBackground() {
  const QColor oc    = wf->bgColor(curElem);
  const QColor color = QColorDialog::getColor(oc
                                            , this
                                            , tr("Select Background Color")
                                            , QColorDialog::ShowAlphaChannel);

  if (color.isValid()) {
     wf->setBackground(curElem, color);
     ib->createBackground();
     refresh();
     }
  }


void MainWindow::chooseColor0() {
  const QColor oc    = wf->fgColor0(curElem);
  const QColor color = QColorDialog::getColor(oc
                                            , this
                                            , tr("Select first Foreground Color")
                                            , QColorDialog::ShowAlphaChannel);

  if (color.isValid()) {
     wf->setForeground0(curElem, color);
     ib->createElement(curElem);
     refresh();
     }
  }


void MainWindow::chooseColor1() {
  const QColor oc    = wf->fgColor1(curElem);
  const QColor color = QColorDialog::getColor(oc
                                            , this
                                            , tr("Select second Foreground Color")
                                            , QColorDialog::ShowAlphaChannel);

  if (color.isValid()) {
     wf->setForeground1(curElem, color);
     ib->createElement(curElem);
     refresh();
     }
  }


void MainWindow::chooseFont() {
  bool ok;
  QFont oldFont = wf->font(curElem);
  QFont font = QFontDialog::getFont(&ok, oldFont, this, tr("Select Font"), QFontDialog::ScalableFonts);

  if (ok) {
     wf->setFont(curElem, font);
     ib->createElement(curElem);
     refresh();
     }
  }


void MainWindow::heightChanged() {
  wf->setHeight(ui->spHeight->value());
  ib->createBackground();
  refresh();
  }


void MainWindow::openDir() {
  QString faceDir = QFileDialog::getExistingDirectory(this
                                                    , tr("Open Project")
                                                    , "/media/Scratch");
  if (faceDir.isEmpty()) return;
  if (ib) delete ib;
  ib = new ImageBuilder(wf, symbolBase, faceDir);
  ib->createElement(0);
  ui->gFace->setEnabled(true);
  ui->gProperties->setEnabled(true);
//  ui->gSliders->setEnabled(true);
  ui->cbElement->setEnabled(true);
  this->setWindowTitle(QString(tr("FaceBuilder - %1")).arg(faceDir));
  refresh();
  }


void MainWindow::openFile() {
  QString fileName = QFileDialog::getOpenFileName(this
                                                , tr("Open File")
                                                , "/media/Scratch"
                                                , tr("Text files (*.txt)"));

  if (fileName.isEmpty()) return;
  QFileInfo fi(fileName);
  QDir faceDir(fi.absoluteDir());

  qDebug() << "got filename:" << fileName;

  ib->setFaceDir(faceDir.path());
  wf->loadFace(fileName);
  for (int i=0; i < WatchFace::MaxElements; ++i)
      ib->createElement(i);
  ui->gFace->setEnabled(true);
  ui->gProperties->setEnabled(true);
  ui->gSliders->setEnabled(true);  
  ui->cbElement->setEnabled(true);
  this->setWindowTitle(QString(tr("FaceBuilder - %1")).arg(faceDir.path()));
  refresh();
  }


void MainWindow::posChanged() {
  wf->setX(curElem, ui->spX->value());
  wf->setY(curElem, ui->spY->value());
  ib->createElement(curElem);
  refresh();
  }


void MainWindow::powerChanged() {
  int powerFactor = ui->spPower->value();

  wf->setSample(10, QString("%1%").arg(powerFactor));
  wf->setSample(21, QString("%1").arg(powerFactor));
  ib->createElement(10);
  ib->createElement(21);
  refresh();
  }


void MainWindow::refresh() {
  QImage scratch = ib->assembleImage();

  ui->label->setPixmap(QPixmap::fromImage(scratch));
  update();
  }


void MainWindow::sampleChanged() {
  wf->setSample(curElem, ui->edSample->text());
  ib->createElement(curElem);
  refresh();
  }


void MainWindow::save() {
  QString faceDir = QFileDialog::getExistingDirectory(this
                                                    , tr("Save Preview")
                                                    , "/media/Scratch");

  pers->storeImages(faceDir, ib->getFaceDir());
  QString fileName = faceDir + "/watchface-pre.txt";

  wf->save(fileName);
  fileName = faceDir + "/preview.png";
  QImage img(ui->label->pixmap(Qt::ReturnByValueConstant()).toImage());

  if (!img.save(fileName, "PNG")) {
     qDebug() << "Oups, failed to store image to file: >|" << fileName << "|<";
     }
  }


void MainWindow::selectElement(int index) {
  curElem = index;
  // avoid event loop during value change
  disconnect(ui->spX, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::posChanged);
  const ElementInfo* cfg = wf->elementInfo(curElem);

  if (cfg) {
     switch (cfg->type) {
       case Background:
            ui->gSliders->setEnabled(false);
            ui->edSample->setVisible(false);
            ui->cGradient->setVisible(false);
            ui->cSize->setVisible(false);
            ui->spAngle->setVisible(false);
            break;
       case SingleDigit:
       case TextSequence:
       case FreeText:
            ui->gSliders->setEnabled(false);
            ui->edSample->setVisible(true);
            ui->cGradient->setVisible(true);
            ui->cSize->setVisible(false);
            ui->spAngle->setVisible(false);
            break;
       case Symbol:
            ui->gSliders->setEnabled(false);
            ui->edSample->setVisible(false);
            ui->cGradient->setVisible(false);
            ui->cSize->setVisible(false);
            ui->spAngle->setVisible(false);
            break;
       case Gauge:
            ui->gSliders->setEnabled(true);
            ui->edSample->setVisible(false);
            ui->cGradient->setVisible(false);
            ui->cSize->setVisible(true);
            ui->spAngle->setVisible(true);
            break;
       default:
            break;
       }
     ui->spX->setValue(cfg->x);
     ui->spY->setValue(cfg->y);
     ui->spAngle->setValue(cfg->rotation);
     ui->edSample->setText(cfg->sample);
     }
  // reenable event for user processing
  connect(ui->spX, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::posChanged);
  refresh();
  }


void MainWindow::stepsChanged() {
  int steps = ui->spSteps->value();
  wf->setSample(11, QString("%1").arg(steps));
  int factor = (int)(((double)steps * 100.0) / (double)ui->spMaxSteps->value());

  wf->setSample(22, QString("%1").arg(factor));
  ib->createElement(11);
  ib->createElement(22);
  refresh();
  }


void MainWindow::test() {
#ifdef REDNOSE
  QFont        font("DejaVu Sans Condensed", 110, 80, true);
  QFontMetrics fm(font);
  QPainter     p;
  static int   i = -1;

//  for (int i=0; i < 10; ++i) {
      if (++i > 9) i = 0;
      QString      text = QString("%1").arg(i);
      QRect        r = fm.boundingRect(text);
      QImage       scratch = QImage(r.width() / 7 * 9, r.height(), QImage::Format_ARGB32);
      QPainterPath pp;

      qDebug() << "bounding rect:" << r.x() << " / " << r.y()
               << "   ==>   " << r.width() << "x" << r.height();

      pp.addText(QPointF(10 - r.x(), -r.y() + 3)
               , font
               , text);

      if (p.begin(&scratch)) {
         p.setRenderHint(QPainter::Antialiasing);
         p.setCompositionMode(QPainter::CompositionMode_Source);
         p.fillRect(scratch.rect(), Qt::red);
         p.setCompositionMode(QPainter::CompositionMode_SourceOver);

//         p.setFont(font);
//         p.setPen(QPen(QColor(255, 170, 0), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
         p.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
//         p.setBrush(Qt::NoBrush);
         p.setBrush(QBrush(Qt::white));
         p.drawPath(pp);
         p.end();

         ui->label->setPixmap(QPixmap::fromImage(scratch));
         }
//      scratch.save(QString("rednose_%1.png").arg(i), "PNG");
//      }
#else
  ui->gFace->setEnabled(true);
  ui->gProperties->setEnabled(true);
//  ui->gSliders->setEnabled(true);
  ui->cbElement->setEnabled(true);
  QImage scratch = ib->testDigit();

  ui->label->setPixmap(QPixmap::fromImage(scratch));
  update();
#endif
  }


void MainWindow::widthChanged() {
  wf->setWidth(ui->spWidth->value());
  ib->createBackground();
  refresh();
  }
