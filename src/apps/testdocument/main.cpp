#include <QtTest>
#include <QString>
#include <QDebug>
#include <QByteArray>
#include <QFile>
#include <QFont>
#include <QTime>
#include <QColor>
#include <QDomDocument>

#include <QImage>
#include <QMimeDatabase>
#include <QElapsedTimer>
#include <QMimeType>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QTranslator>


#include <docx/document.h>
#include <docx/text.h>
#include <docx/table.h>


using namespace Docx;

class TestDocument : public QObject
{
    Q_OBJECT

public:
    TestDocument();

private Q_SLOTS:
    void testHeaderAndFooter();
    void testStyleDoc();
    void testNewDoc();
    void testMaxDoc();
    void testTable();

    void testImageInfo();

    void testLoadDoc();
    void testLoadTable();
};

const QString imagePath1("://c.png");
const QString imagePath2("://139924.jpg");
const QString imagePath3("://149341.jpg");
const QString imagePath4("://185607.jpg");
const QString imagePath5("Images/1%s.jpg");

QStringList strList;

TestDocument::TestDocument()
{
    // ://testStr.txt
    QFile file("://testStr.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"Can't open the file!"<<endl;
    }

    QTextStream stream(&file);
    QString line;
    do {
        line = stream.readLine();
        strList.append(line);
    } while (!line.isNull());

    //    QTranslator* translator = new QTranslator;
    //    translator->load("Docx_zh_CN.qm");
    //    qApp->installTranslator(translator);

}

void TestDocument::testHeaderAndFooter()
{
    Document doc;
    doc.addParagraph("ABCDEF");
    Section *section = doc.addSection();
    Header *header = section->defaultHeader();
    Paragraph *p = header->addParagraph();
    Run *r = p->addRun();
    r->addTab();
    p->addRun("AAAAA");
    header->showNumber(PageNumberType::NormalNum);
    Footer *footer = section->defaultFooter();
    footer->addParagraph("BBBBB");

    Section *section2 = doc.addSection();
    Header *header2 = section2->defaultHeader();
    header2->addParagraph("CCCC");
    header2->showNumber(PageNumberType::RomanNum);
    section2->restartNum();
    Footer *footer2 = section2->defaultFooter();
    footer2->addParagraph("DDDD");

    //doc.addPicture(imagePath2);
    doc.addPicture("e:/a.png");

    doc.save(QStringLiteral("headerAndFooter.docx"));
}

void TestDocument::testStyleDoc()
{
    Document doc(QStringLiteral("new1.docx"));
    doc.addHeading("Title", 0);
    doc.addHeading("Heading1", 1);
    doc.addHeading("Heading2", 2);
    doc.addHeading("Heading3", 3);

    Table *table = doc.addTable(3, 3);
    QList<Cell *> cells = table->rowCells(0);
    cells.at(0)->addText("Hello");
    cells.at(1)->addText("Word");
    cells.at(2)->addText("!!!");


    doc.addParagraph("Number1", "ListNumber");
    doc.addParagraph("Number2", "ListNumber");

    doc.addParagraph("Bullet1", "ListBullet");
    doc.addParagraph("Bullet2", "ListBullet");

    doc.save(QStringLiteral("new2.docx"));
}

void TestDocument::testNewDoc()
{   
    Document doc;

    doc.addHeading("MyTitle", 0);
    Paragraph *p = doc.addParagraph("helleWord");
    doc.addParagraph("Bullet1", "ListBullet");
    p->insertParagraphBefore("Before", "ListBullet");
    p->insertParagraphBefore("Number1", "ListNumber");
    p->insertParagraphBefore("Number2", "ListNumber");

    Run *addRun = p->addRun("AddRun");
    addRun->setStyle(QStringLiteral("Emphasis"));
    addRun->addText("Hello");
    qDebug() << "run text :" << addRun->text();
    addRun->setText(QStringLiteral("NewText"));
    qDebug() << "run text :" << addRun->text();

    doc.addParagraph("Bullet1", "ListBullet");
    doc.addParagraph("Bullet1", "ListBullet");
    doc.addParagraph();
    doc.addHeading("MyHead1 Paragraph and Run");

    Paragraph *emptyP = doc.addParagraph();
    Run * tabRun = emptyP->addRun();
    tabRun->addTab();
    emptyP->addRun("EmptyParagraph", "IntenseEmphasis");

    doc.addParagraph("West", "IntenseQuoteChar");

    Paragraph *p2 = doc.addParagraph("Two");
    Run *r2 = p2->addRun("Next");
    r2->setBold(true);
    r2->setBold(false);
    r2->setBold(true);

    doc.addHeading("Heading2", 2);
    Paragraph *p3 = doc.addParagraph();
    Run *run = p3->addRun("MyContent");
    run->addText("abc");
    run->setBold(true);
    run->setUnderLine(WD_UNDERLINE::SINGLE);
    run->addTab();

    run = p3->addRun("Main");
    run->setAllcaps(true);
    run->setUnderLine(WD_UNDERLINE::DASH);
    run->addTab();

    run = p3->addRun("Main2");
    run->setDoubleStrike(true);
    run->setUnderLine(WD_UNDERLINE::DASH_HEAVY);
    run->addTab();

    run = p3->addRun("Main3");
    run->setItalic(true);
    run->setUnderLine(WD_UNDERLINE::DOT_DASH);
    run->addTab();

    run = p3->addRun("Main3");
    run->setSmallcaps(true);
    run->setUnderLine(WD_UNDERLINE::None);
    run->setShadow();
    run->addTab();


    qDebug() << p3->text();

    doc.addParagraph();
    p2 = doc.addParagraph("Alignment1");
    p2->setAlignment(WD_PARAGRAPH_ALIGNMENT::CENTER);

    p2 = doc.addParagraph("Alignment2");
    p2->setAlignment(WD_PARAGRAPH_ALIGNMENT::DISTRIBUTE);

    p2 = doc.addParagraph("Alignment3");
    p2->setAlignment(WD_PARAGRAPH_ALIGNMENT::JUSTIFY_HI);

    doc.addHeading("Image", 3);
    doc.addPicture(imagePath2);
    doc.addPicture(imagePath2, Inches::emus(1.25));
    doc.addPicture(imagePath3, Cm::emus(13), Cm::emus(10));
    doc.addPicture(imagePath3, Cm::emus(3), Cm::emus(10));
    QImage img(imagePath4);
    qDebug() << img.size();
    doc.addPicture(img, Cm::emus(5));
    doc.addPicture(img, Cm::emus(2));

    // Table
    Table *table = doc.addTable(3, 3);
    QList<Cell *> cells = table->rowCells(0);
    cells.at(0)->addText("Hello");
    cells.at(1)->addText("Word");
    cells.at(2)->addText("!!!");
    Paragraph *p1 = cells.at(2)->addParagraph();
    Run *r1 = p1->addRun();
    r1->addPicture(imagePath3, Cm::emus(3), Cm::emus(5));

    QList<Cell *> cells2 = table->rowCells(1);
    Cell *cell = cells2.at(0);
    Table *table2 = cell->addTable(5, 5, "MediumShading1");
    cells2 = table2->rowCells(1);
    cells2.at(0)->addText("Table!!!");

    p1 = cells2.at(2)->addParagraph();
    r1 = p1->addRun();

    r1->addPicture(imagePath2, Cm::emus(3), Cm::emus(5));

    table->addColumn();
    table->addRow();

    doc.addPageBreak();
    doc.addHeading("Merge Table Cell", 3);

    // Merge Table Cell
    table = doc.addTable(5, 5);
    cells = table->rowCells(0);
    cells.at(0)->addText("Hello");
    cells.at(1)->addText("Word");
    cells.at(2)->addText("!!!");
    p1 = cells.at(2)->addParagraph();
    r1 = p1->addRun();
    r1->addPicture(imagePath3, Cm::emus(3), Cm::emus(5));

    Cell *cell00 = table->cell(0, 0);
    Cell *cell12 = table->cell(1, 2);

    cell00->merge(cell12);

    doc.addParagraph();


    Cell *cell04 = table->cell(0, 4);
    Cell *cell44 = table->cell(3, 4);
    cell44->merge(cell04);

    cell44 = table->cell(2, 4);
    cell44->addParagraph("new Paragraph");

    cell00 = table->cell(0, 0);
    Cell *cell22 = table->cell(2, 2);
    cell00->merge(cell22);
    table->addColumn();

    // _______________
    table = doc.addTable(3, 3);
    table->setAlignment(WD_TABLE_ALIGNMENT::RIGHT);
    cells = table->rowCells(0);
    cells.at(0)->addText("Hello");
    cells.at(1)->merge(table->cell(2, 2));

    table = doc.addTable(3, 3);
    table->setAlignment(WD_TABLE_ALIGNMENT::CENTER);
    cells = table->rowCells(0);
    cells.at(0)->addText("Hello");

    cells.at(0)->merge(cells.at(2));
    doc.addParagraph("End");

    doc.save("aSave.docx");
}

void TestDocument::testMaxDoc()
{
    //        qsrand(QTime::currentTime().msec());
    //        int strListCount = strList.count() - 1;

    //        Document doc;
    //        int ipageCount = 100;
    //        QElapsedTimer timer;

    //        QString strImagePath;
    //        qDebug() << "start " << QTime::currentTime() << "-------***";
    //        timer.start();
    //        while (ipageCount > 0) {
    //            ipageCount--;
    //            int value1 = qrand() % strListCount;

    //            QString str(strList.at(value1));
    //            value1 = qrand() % strListCount;
    //            str.append(strList.at(value1));

    //            doc.addParagraph(str);

    //            //if (ipageCount <  91) {
    //            if (value1 <  91) {
    //                strImagePath = QString("Images/1%1.jpg").arg(value1);
    //                //strImagePath = QString("Images/1%1.jpg").arg(ipageCount);
    //                doc.addPicture(strImagePath, Cm::emus(9));
    //            }
    //        }
    //        qDebug() << "time " << QTime::currentTime() <<  "time count" << timer.elapsed();
    //        doc.save("aMax.docx");
}

void TestDocument::testTable()
{
    //Document doc(QStringLiteral("aSave.docx"));
    Document doc;
    Table *table = doc.addTable(5, 5);
    QList<Cell *> cells = table->rowCells(0);
    cells.at(0)->addText("Hello");
    cells.at(1)->addText("Word");
    cells.at(2)->addText("!!!");
    Paragraph *p1 = cells.at(2)->addParagraph();
    Run *r1 = p1->addRun();
    r1->addPicture(imagePath3, Cm::emus(3), Cm::emus(5));

    Cell *cell00 = table->cell(0, 0);
    Cell *cell12 = table->cell(1, 2);

    cell00->merge(cell12);

    qDebug() << QString::fromLatin1("----Cell index  ") <<  cells.at(1)->cellIndex()
             << QStringLiteral("--Row index  ") << cells.at(1)->rowIndex();

    doc.addParagraph();


    Cell *cell04 = table->cell(0, 4);
    Cell *cell44 = table->cell(3, 4);
    cell44->merge(cell04);

    cell44 = table->cell(2, 4);
    cell44->addParagraph("new Paragraph");

    cell00 = table->cell(0, 0);
    Cell *cell22 = table->cell(2, 2);
    cell00->merge(cell22);
    table->addColumn();

    // _______________
    table = doc.addTable(3, 3);
    table->setAlignment(WD_TABLE_ALIGNMENT::RIGHT);
    cells = table->rowCells(0);
    cells.at(0)->addText("Hello");
    cells.at(1)->merge(table->cell(2, 2));

    table = doc.addTable(3, 3);
    table->setAlignment(WD_TABLE_ALIGNMENT::CENTER);
    cells = table->rowCells(0);
    cells.at(0)->addText("Hello");

    cells.at(0)->merge(cells.at(2));

    doc.save("atable.docx");
}

void TestDocument::testImageInfo()
{

    QMimeDatabase base;
    QMimeType fileInfo = base.mimeTypeForFile(imagePath1);


    QImage image1(imagePath1);
    qDebug() << "Image Info" ;
    qDebug() << "Image name" << fileInfo.name() << "  suffixes " << fileInfo.preferredSuffix();
    qDebug() << fileInfo.preferredSuffix().toStdString().c_str();

    qDebug() << image1.rect() << "  " << image1.size();
    qDebug() << "dpiX" << image1.logicalDpiX() << "  dpiY  " << image1.logicalDpiY();
    //qDebug() << "preX" << image.dotsPerMeterX() << "  perY  " << image.dotsPerMeterY();
    qDebug() << "cacheKey1" << image1.cacheKey();

    QImage image11(imagePath1);
    QImage image2(imagePath2);
    qDebug() << "cacheKey11" << image11.cacheKey();
}


void TestDocument::testLoadDoc()
{
    QFile docfile("aSave.docx");
    if (!docfile.open(QIODevice::ReadOnly)) {
        qDebug() << "open filed..";
        return;
    }
    //Document doc("aSave.docx");
    Document doc(&docfile);
    doc.addParagraph("Load a Document");
    doc.addPicture(imagePath4, Cm::emus(3), Cm::emus(5));
    QList<Paragraph*> ps = doc.paragraphs();
    qDebug() << " Paragraph count " << ps.count();
    Paragraph *p = ps.at(3);
    p->addRun("Append1");
    p->insertParagraphBefore("AppendBefore");
    doc.save("aSaveLoad.docx");
}

void TestDocument::testLoadTable()
{
    Document doc(QStringLiteral("atable.docx"));

    QList<Table*> tables = doc.tables();
    Table *table0 = tables.at(0);
    qDebug() << table0->rows().count();
    Cell *cell12 = table0->cell(1, 2);
    cell12->addParagraph("MyCell12");

    Cell *cell34 = table0->cell(3, 4);
    cell34->addText("Cell34");
    Cell *cell24 = table0->cell(2, 4);
    cell24->addText("AppendText");

    Cell *cell32 = table0->cell(3, 2);
    cell32->addText("Cell22");


    doc.save("atableLoad.docx");
}




QTEST_APPLESS_MAIN(TestDocument)
#include "main.moc"

