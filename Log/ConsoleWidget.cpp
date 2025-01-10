#include "ConsoleWidget.h"
#include "ui_ConsoleWidget.h"

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QScrollBar>
#include <QMenu>
#else
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QMenu>
#endif

#define SWW_TEST(var) ui->textEdit->append(#var " : ")

QString errFlag = QString("ERR: ").rightJustified(8,' ');
QString warnFlag = QString("WARN: ").rightJustified(8,' ');
QString infoFlag = QString("INFO: ").rightJustified(8,' ');
QString debugFlag = QString("DEBUG: ").rightJustified(8,' ');
QString debugValueFlag = QString("VALUE: ").rightJustified(8,' ');

ConsoleWidget::ConsoleWidget(QWidget *parent) :
    QWidget(parent),_autoScroll(true),
    ui(new Ui::ConsoleWidget)
{
    ui->setupUi(this);

    QScreen *screen = QGuiApplication::primaryScreen();
    qreal dpiX = screen->logicalDotsPerInchX();
    qreal dpiY = screen->logicalDotsPerInchY();
    qreal scaleFactorX = dpiX / 96.0;
    qreal scaleFactorY = dpiY / 96.0;

    ui->textEdit->setFontPointSize(6.0 * scaleFactorX);
    ui->textEdit->setReadOnly(true);
    ui->textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->textEdit->setContextMenuPolicy(Qt::CustomContextMenu);

    this->connect(ui->textEdit, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint &)));

    _clearAction = new QAction("clear",this);
    connect(_clearAction, SIGNAL(triggered()), ui->textEdit, SLOT(clear()));

    _disableAction = new QAction("disable",this);
    _disableAction->setCheckable(true);
    _disableAction->setChecked(false);
}

ConsoleWidget::~ConsoleWidget()
{
    delete ui;
}

void ConsoleWidget::showContextMenu(const QPoint &pt){
    QMenu* menu = ui->textEdit->createStandardContextMenu();
    menu->addSeparator();
    menu->addAction(_clearAction);
    menu->addSeparator();
    menu->addAction(_disableAction);
    menu->exec(ui->textEdit->mapToGlobal(pt));
    delete menu;
}


void ConsoleWidget::logErr(const QString &msg){
    if (_disableAction->isChecked())
        return;

    ui->textEdit->setTextColor(QColor(180,0,0));
    ui->textEdit->append(errFlag  + msg);

    if (_autoScroll && isVisible()) {
        QScrollBar* scrollBar = ui->textEdit->verticalScrollBar();
        if (scrollBar)
            scrollBar->setValue(scrollBar->maximum());
    }
}

void ConsoleWidget::logWarn(const QString &msg){
    if (_disableAction->isChecked())
        return;

    ui->textEdit->setTextColor(QColor(160,100,0));
    ui->textEdit->append(warnFlag  +msg);

    if (_autoScroll && isVisible()) {
        QScrollBar* scrollBar = ui->textEdit->verticalScrollBar();
        if (scrollBar)
            scrollBar->setValue(scrollBar->maximum());
    }
}

void ConsoleWidget::logInfo(const QString &msg){
    if (_disableAction->isChecked())
        return;

    ui->textEdit->setTextColor(QColor(100,210,138));
    ui->textEdit->append(infoFlag  +msg);

    if (_autoScroll && isVisible()) {
        QScrollBar* scrollBar = ui->textEdit->verticalScrollBar();
        if (scrollBar)
            scrollBar->setValue(scrollBar->maximum());
    }
}

void ConsoleWidget::logDebug(const QString &msg){
    if (_disableAction->isChecked())
        return;

    ui->textEdit->setTextColor(QColor(0,100,200));
    ui->textEdit->append(debugFlag  +msg);

    if (_autoScroll && isVisible()) {
        QScrollBar* scrollBar = ui->textEdit->verticalScrollBar();
        if (scrollBar)
            scrollBar->setValue(scrollBar->maximum());
    }
}

void ConsoleWidget::logDebugValue(const QString &msg){
    if (_disableAction->isChecked())
        return;

    ui->textEdit->setTextColor(QColor(190,150,240));
    ui->textEdit->append(debugValueFlag  +msg);

    if (_autoScroll && isVisible()) {
        QScrollBar* scrollBar = ui->textEdit->verticalScrollBar();
        if (scrollBar)
            scrollBar->setValue(scrollBar->maximum());
    }
}
