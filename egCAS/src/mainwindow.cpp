#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPrinter>
#include <QPrintDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new EgCasScene(this);
    QString dummy("<math  display=\"block\"><mrow><mfrac><msqrt><mn>1</mn><mo>+</mo><mroot><mrow><mn>2</mn><mo>+</mo><mroot><mrow><mn>3</mn><mo>+</mo><mroot><mrow><mn>4</mn><mo>+</mo><mroot><mrow><mn>5</mn><mo>+</mo><mroot><mrow><mn>6</mn><mo>+</mo><mroot><mrow><mn>7</mn><mo>+</mo><mroot><mi>A</mi><mn>19</mn></mroot></mrow><mn>17</mn></mroot></mrow><mn>13</mn></mroot></mrow><mn>11</mn></mroot></mrow><mn>7</mn></mroot></mrow><mn>5</mn></mroot></mrow><mn>3</mn></mroot></msqrt><msup><mi>&exponentiale;</mi><mi>&pi;</mi></msup></mfrac><mo>=</mo><msup><mi>x</mi><mo style=\"font-size: larger;\">&tprime;</mo></msup></mrow></math>");
    QString dummy2("<math><mfrac><mrow><mn> 1 </mn><mo> + </mo><msqrt><mn> 5 </mn></msqrt></mrow><mn> 2 </mn></mfrac></math>");
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    ui->graphicsView->setScene(scene);
    FormulaItem::setBaseFontSize(30);
    FormulaItem *formula = new FormulaItem(dummy, QPointF(120.0, 200.0));
    formula->setFontSize(40);
    scene->addItem(formula);
    scene->addItem(new FormulaItem(dummy2, QPointF(120.0, 300.0)));
    QFont font_text(QString("Century Schoolbook L"));
    font_text.setPixelSize(60);
    scene->addText(QString("This is a test text"), font_text);
    QRectF rect(0,0,2100,2900);
    scene->setSceneRect(rect);


    QPrinter printer(QPrinter::HighResolution);
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("/home/jo/tmp/nonwritable.pdf");

    QPainter painter(&printer);
    scene->render(&painter);

}

MainWindow::~MainWindow()
{
    delete ui;
}
