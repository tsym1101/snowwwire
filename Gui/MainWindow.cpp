#include "MainWindow.h"
#include "ui_MainWindow.h"


#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#include <QtGui/QDockWidget>
#include <QPlastiqueStyle>
#else
#include <QtWidgets/QDockWidget>
#endif

#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QLabel>

#include "Core/SwwAssert.h"

#include "Gui/NodeGraphWidget.h"
#include "Gui/KnobWidget.h"
#include "Gui/TimeSlider.h"
#include "Gui/NodeGraphScene.h"
//#include <GL/glew.h>

#include "Core/SwwVersion.h"
#include "Core/NodeGraph.h"

#include "OpenGL/GLGlobal.h"

#include "Knob/KnobFile.h"
#include "Knob/KnobMultiFile.h"

#include "Gui/GuiGlobal.h"
//#include "Gui/ConsoleWidget.h"

#include "Log/Log.h"
#include "Log/ConsoleWidget.h"

#include "Core/RootNode.h"

#include "Knob/CommandStack.h"

MainWindow::MainWindow(Sww::Core::RootNode* rootNode, QWidget *parent) :
    QMainWindow(parent),_rootNode(rootNode),_rootNgs(0),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initialize();
}

MainWindow::~MainWindow()
{
    if(_process.state() == QProcess::Running) {
        _process.waitForFinished();
    }

    if(_rootNgs && !_rootNgs->parent()){
        delete _rootNgs;
    }

    delete ui;
}

void MainWindow::initialize()
{

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    setStyle(new QPlastiqueStyle());
#else
#endif

    setCorner( Qt::BottomRightCorner, Qt::RightDockWidgetArea );

    QString ssStr;
    QFile file(":Style_MainWindow.css");
    if (!file.open(QIODevice::ReadOnly)){
        SWW_ERR("main window style sheet load failed.");
        return;
    }

    QTextStream in(&file);
    ssStr = in.readAll();

    // DPI に基づいて値を動的に変更
    QScreen *screen = QGuiApplication::primaryScreen();
    qreal dpiX = screen->logicalDotsPerInchX();
    qreal dpiY = screen->logicalDotsPerInchY();
    qreal scaleFactorX = dpiX / 96.0;
    qreal scaleFactorY = dpiY / 96.0;

    ssStr.replace("min-width: 60px;", QString("min-width: %1px;").arg(60 * scaleFactorX));
    ssStr.replace("max-width: 60px;", QString("max-width: %1px;").arg(60 * scaleFactorX));
    ssStr.replace("min-height: 18px;", QString("min-height: %1px;").arg(18 * scaleFactorY));
    ssStr.replace("max-height: 18px;", QString("max-height: %1px;").arg(18 * scaleFactorY));

    // 正規表現を使ってフォントサイズをスケーリング
    QRegularExpression fontSizeRegex(R"(font-size:\s*(\d+)px;)");
    QRegularExpressionMatchIterator i = fontSizeRegex.globalMatch(ssStr);

    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        int originalSize = match.captured(1).toInt(); // キャプチャされたサイズを取得
        int scaledSize = static_cast<int>(originalSize * scaleFactorY); // スケーリング適用
        ssStr.replace(match.captured(0), QString("font-size: %1px;").arg(scaledSize));
    }

    setStyleSheet(ssStr);

    //setAttribute(Qt::WA_DeleteOnClose);

    //SwwGlobal::setMainWindow(this);
    createDockWindows();
    createActions();
    createMenus();

    setCurrentFile("");
    loadSettings();

    connect(&_process,SIGNAL(error(QProcess::ProcessError))
            ,this,SLOT(processError(QProcess::ProcessError)));

    //SwwGui::GuiGlobal::setMainWindow(this);

}

void MainWindow::createDockWindows()
{
    //setFocusPolicy(Qt::NoFocus);

    QDockWidget *dock1 = new QDockWidget(tr("Knob"), this);
    _knobWidget = new KnobWidget(dock1);
    _nodeGraphWidget = new NodeGraphWidget(_knobWidget);
    _nodeGraphWidget->setRootNode(_rootNode);
    _nodeGraphWidget->setFocusPolicy(Qt::StrongFocus);

    _rootNgs = new SwwGui::NodeGraphScene(_rootNode,0);

    _nodeGraphWidget->setScene(_rootNgs->getPath());

    TimeSlider* timeSlider = new TimeSlider(_rootNode);
    QVBoxLayout *verticalLayout1 =  new QVBoxLayout();

    verticalLayout1->addWidget(_nodeGraphWidget);
    verticalLayout1->addWidget(timeSlider);

    QWidget* centralWidget = new QWidget();    //dockWidgetには直接setLayoutできないのでwidgetをかます
    centralWidget->setLayout(verticalLayout1);
    setCentralWidget(centralWidget);

    _knobWidget->setFocusPolicy(Qt::StrongFocus);
    _knobWidget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);


    QRect rec = QApplication::desktop()->screenGeometry();
    _knobWidget->setMinimumSize(rec.width() / 4,240);

    dock1->setWidget(_knobWidget);

    //SwwGlobal::_knobWidget = _knobWidget;

    addDockWidget(Qt::RightDockWidgetArea, dock1);


    QDockWidget *dock2 = new QDockWidget(tr("debug console"),this);
    //dock2->setWidget(new ConsoleWidget());
    dock2->setWidget(Sww::Log::_consoleWidget);

    addDockWidget(Qt::BottomDockWidgetArea,dock2);
    //addDockWidget(Qt::BottomDockWidgetArea,new QDockWidget(tr("debug console"), this));

    //QDockWidget *dock2 = new QDockWidget(tr("viewer"), this);

    //    QWidget* widget = new QWidget();
    //    widget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
    //    widget->setMinimumSize(320,320);
    //    QVBoxLayout* viewerLayout = new QVBoxLayout();

    //    SwwGlobal::_viewerWidget = new ViewerWidget(this);
    //    viewerLayout->addWidget(SwwGlobal::_viewerWidget);

    //    SwwConsoleWidget* consoleWidget = new SwwConsoleWidget(this);
    //    viewerLayout->addWidget(consoleWidget);

    //    widget->setLayout(viewerLayout);
    //    dock2->setWidget(widget);
    //    addDockWidget(Qt::BottomDockWidgetArea, dock2);
}

void MainWindow::createActions()
{
    newAction = new QAction(tr("&New"), this);
    newAction->setIcon(QIcon(":/images/new.png"));
    newAction->setShortcut(tr("Ctrl+N"));
    newAction->setStatusTip(tr("Create a new Scene file"));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

    openAction = new QAction(tr("&Open..."), this);
    openAction->setIcon(QIcon(":/images/open.png"));
    openAction->setShortcut(tr("Ctrl+O"));
    openAction->setStatusTip(tr("Open an existing Scene file"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setIcon(QIcon(":/images/save.png"));
    saveAction->setShortcut(tr("Ctrl+S"));
    saveAction->setStatusTip(tr("Save the Scene to disk"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAction = new QAction(tr("Save &As..."), this);
    saveAsAction->setStatusTip(tr("Save the Scene under a new "
                                  "name"));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i], SIGNAL(triggered()),
                this, SLOT(openRecentFile()));
    }

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    //_2dAction = new QAction("2d",this);
    //_2dAction->setShortcut(tr("F2"));
    //_2dAction->setStatusTip(tr("activate 2d view"));
    //connect(_2dAction, SIGNAL(triggered()), this, SLOT(setViewer2d()));

    _spoolAction = new QAction("spool",this);
    _spoolAction->setShortcut(tr("Ctrl+Shift+P"));
    _spoolAction->setStatusTip(tr("open spool"));
#if 0
    connect(_spoolAction, SIGNAL(triggered()), this, SLOT(openSpool()));
#endif

    _readDeepAction = new QAction("read DeepLayers",this);
    _readDeepAction->setStatusTip(tr("read Deep Layers folder"));
    connect(_readDeepAction, SIGNAL(triggered()), this, SLOT(readDeepLayers()));

}

void MainWindow::createMenus()
{
    //  fileMenu = menuBar()->addMenu(tr("&File"));

    ui->menuFile->addAction(newAction);
    ui->menuFile->addAction(openAction);
    ui->menuFile->addAction(saveAction);
    ui->menuFile->addAction(saveAsAction);
    separatorAction = ui->menuFile->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        ui->menuFile->addAction(recentFileActions[i]);
    }

    ui->menuFile->addSeparator();
    ui->menuFile->addAction(exitAction);

    // ui->menuView->addAction(_2dAction);

    ui->menuTools->addAction(_spoolAction);

    ui->menuMacro->addAction(_readDeepAction);


}

//void MainWindow::setViewer2d()
//{
//    SwwGlobal::_viewerWidget->show();
//}

void MainWindow::newFile()
{
    if (okToContinue()) {
        SwwOpenGL::GLGlobal::makeCurrent();
        _knobWidget->clear();
        _nodeGraphWidget->setScene(_rootNgs->getPath(),false); //子のNodeGraphSceneにいるときは、いったんルートにもどる。
        _rootNode->clearGraph();
        SwwOpenGL::GLGlobal::doneCurrent();
        _nodeGraphWidget->setScene(_rootNgs->getPath());

        setCurrentFile("");
    }
}

void MainWindow::open()
{
    if (okToContinue()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("Open Scene"), _lastOpenXmlPath,
                                                        tr("ds files (*.json *.xml);;all (*.*)") );
        if (!fileName.isEmpty()){
            open(fileName);
        }
    }
}

void MainWindow::open(const QString& fileName)
{    
    SwwOpenGL::GLGlobal::makeCurrent();
    _knobWidget->clear();
    _nodeGraphWidget->setScene(_rootNgs->getPath(),false); //子のNodeGraphSceneにいるときは、いったんルートにもどる。
    _rootNode->clearGraph();
    SwwOpenGL::GLGlobal::doneCurrent();
    _rootNode->load(fileName.toStdString());
    _nodeGraphWidget->setScene(_rootNgs->getPath());

    setCurrentFile(fileName);
    _lastOpenXmlPath = QFileInfo(fileName).absolutePath();
}

bool MainWindow::save()
{
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        if(!_rootNode->save(curFile.toStdString()))
            return false;
        statusBar()->showMessage(tr("File saved"), 2000);
        setWindowModified(false);
        return true;
    }

    return true;
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Scene"), _lastSaveXmlPath,
                                                    tr("ds files (*.json *.xml);;all (*.*)") );

    if (fileName.isEmpty()) return false;

    if(!_rootNode->save(fileName.toStdString()))
        return false;

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    _lastSaveXmlPath = QFileInfo(fileName).absolutePath();

    return true;
}

//void MainWindow::confirmedClose()
//{
//    if(okToContinue()) close();
//}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowModified(false);

    QString shownName = "Untitled";
    if (!curFile.isEmpty()) {
        shownName =  QFileInfo(curFile).fileName();
        recentFiles.removeAll(curFile);
        recentFiles.prepend(curFile);
        updateRecentFileActions();

    }


    //    QString version = QLocale().toString(SWW_VERSION,'f',2);
    //    QString title = QString("ds v") + version;
    QString title = Sww::Core::SwwVersionStr().c_str();
    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(title));

    //SWW_DEBUG("setcurrent");
}

bool MainWindow::okToContinue()
{
    if (isWindowModified()) {
        int r = QMessageBox::warning(this, tr("sww"),
                                     tr("The scene has been modified.\n"
                                        "Do you want to save your changes?"),
                                     QMessageBox::Yes | QMessageBox::Default,
                                     QMessageBox::No,
                                     QMessageBox::Cancel | QMessageBox::Escape);
        if (r == QMessageBox::Yes) {
            return save();
        } else if (r == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

void MainWindow::openRecentFile()
{
    if (okToContinue()) {
        QAction *action = qobject_cast<QAction *>(sender());
        if (action)
        {
            //            _knobWidget->clear();
            //            _nodeGraphWidget->clearNodeGraphScene();
            //            _nodeGraphWidget->load(action->data().toString());
            //            setCurrentFile(action->data().toString());
            open(action->data().toString());

        }
    }

}

void MainWindow::updateRecentFileActions()
{
    QMutableStringListIterator i(recentFiles);
    while (i.hasNext()) {
        if (!QFile::exists(i.next()))
            i.remove();
    }

    for (int j = 0; j < MaxRecentFiles; ++j) {
        if (j < recentFiles.count()) {
            QString text = tr("&%1 %2")
                    .arg(j + 1)
                    .arg(QFileInfo(recentFiles[j]).absoluteFilePath());
            recentFileActions[j]->setText(text);
            recentFileActions[j]->setData(recentFiles[j]);
            recentFileActions[j]->setVisible(true);
        } else {
            recentFileActions[j]->setVisible(false);
        }
    }
    separatorAction->setVisible(!recentFiles.isEmpty());
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    if (okToContinue()) {
        saveSettings();
        event->accept();

        //destroyCore();

    } else {
        event->ignore();
    }
}

void MainWindow::loadSettings()
{
    QSettings settings("tsym", "sww");
    recentFiles = settings.value("recentFiles").toStringList();
    _lastOpenXmlPath = settings.value("lastOpenXmlPath").toString();
    _lastSaveXmlPath = settings.value("lastSaveXmlPath").toString();

    //SWW_DEBUG_VALUE(_lastOpenXmlPath.toStdString());

    QStringList keys = settings.allKeys();
    QStringList keysOpenDialogPath = keys.filter("IO_open_");
    QStringList keysSaveDialogPath = keys.filter("IO_save_");
    //QStringList keysOpenDialogPathMulti = keys.filter("IO_open_multi_");

    Q_FOREACH(QString str,keysOpenDialogPath){
        QString key = str;
        key.remove("IO_open_");
        KnobFile::getLastOpenDialogPath()[key] = settings.value(str).toString();
    }

    Q_FOREACH(QString str,keysSaveDialogPath){
        QString key = str;
        key.remove("IO_save_");
        KnobFile::getLastSaveDialogPath()[key] = settings.value(str).toString();
    }

    //    Q_FOREACH(QString str,keysOpenDialogPathMulti){
    //        QString key = str;
    //        key.remove("IO_open_multi_");
    //        KnobMultiFile::getLastOpenDialogPath()[key] = settings.value(str).toString();
    //    }

    updateRecentFileActions();
}

void MainWindow::saveSettings()
{
    QSettings settings("tsym", "sww");
    settings.setValue("recentFiles", recentFiles);
    settings.setValue("lastOpenXmlPath",_lastOpenXmlPath);
    settings.setValue("lastSaveXmlPath",_lastSaveXmlPath);

    QHash<QString, QString>::iterator i = KnobFile::getLastOpenDialogPath().begin();
    while (i != KnobFile::getLastOpenDialogPath().end()) {
        settings.setValue("IO_open_" + i.key(),i.value());
        ++i;
    }

    i = KnobFile::getLastSaveDialogPath().begin();
    while (i != KnobFile::getLastSaveDialogPath().end()) {
        settings.setValue("IO_save_" + i.key(),i.value());
        ++i;
    }

    //    i = KnobMultiFile::getLastOpenDialogPath().begin();
    //    while (i != KnobMultiFile::getLastOpenDialogPath().end()) {
    //        settings.setValue("IO_open_multi_" + i.key(),i.value());
    //        ++i;
    //    }

    //SWW_DEBUG("saved settings*****");
    //qDebug() << settings.allKeys();

    //settings.clear();

}

# if 0
void MainWindow::openSpool()
{
    QString execPath = SWW_DISTRIBUTE_EXECUTABLE;

    execPath = execPath.replace("__SWW_VERSION__",Sww::Core::SwwVersionStr().c_str());

    SWW_INF(execPath.toStdString());

    QString cmd;

    QStringList arg;

    QString startStr = QString::number(_rootNode->startFrame());
    QString endStr = QString::number(_rootNode->endFrame());

    QString arg1 = "-script";
    QString arg2;
    arg2 = "{type:custom,exec:" + execPath +  ",s:"
            + startStr  + ",e:" + endStr +  ",sv:elite6|elite7" + ",op:-nogui%s1,file:"
            + curFile
            + "}";

    arg << arg1 << arg2;

#ifdef SWW_ENV_WIN
    //spool.confファイル内にtractorのバージョンを記述し、replace
    cmd = "C:/Program Files/spool-build-desktop/release/spool.exe";
#endif
#ifdef SWW_ENV_MAC
    cmd = "";
#endif

    _process.start(cmd,arg);
}
#endif

void MainWindow::processError(QProcess::ProcessError error)
{
    SWW_ERR("process error!");
}

void MainWindow::readDeepLayers()
{
    _nodeGraphWidget->readDeepLayers();
}



void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Z:{
        if(event->modifiers() & Qt::ControlModifier){
            SwwKnob::CommandStack::undo();
        }
        break;
    }
    case Qt::Key_Y:{
        if(event->modifiers() & Qt::ControlModifier){
            SwwKnob::CommandStack::redo();
        }
        break;
    }

    }//switch

    QWidget::keyPressEvent(event);
}

