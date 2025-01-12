#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include <GL/glew.h>
#include <QMainWindow>
#include <QProcess>
#include "Gui/GuiAPI.h"
#include <QUndoStack>

class NodeGraphWidget;
class KnobWidget;

namespace Ui {
class MainWindow;
}

namespace Sww::Core{
//class  NodeGraph;
class RootNode;
}

namespace SwwGui{
class  NodeGraphScene;
}

class SWWGUISHARED_EXPORT MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Sww::Core::RootNode *rootNode, QWidget *parent = 0);
    ~MainWindow();
    void initialize();

    void open(const QString& fileName);

//    void pushCommand(QUndoCommand* cmd);
//    void undo();
//    void redo();

protected:
    void closeEvent(QCloseEvent *);
    void keyPressEvent(QKeyEvent *event);

private Q_SLOTS:
    void newFile();
    void open();

    bool save();
    bool saveAs();
    void setCurrentFile(const QString &fileName);
    void openRecentFile();
    void updateRecentFileActions();

//    void openSpool();
    void processError(QProcess::ProcessError error);

    void readDeepLayers();

private:
    Ui::MainWindow *ui;

    Sww::Core::RootNode* _rootNode;
    SwwGui::NodeGraphScene* _rootNgs;

    //void destroyCore();//ngとngs削除

    NodeGraphWidget* _nodeGraphWidget;
    KnobWidget* _knobWidget;

    QString curFile;
    QStringList recentFiles;

    QString _lastOpenXmlPath;
    QString _lastSaveXmlPath;

    void createActions();
    void createDockWindows();
    void createMenus();
    bool okToContinue();
    void saveSettings();
    void loadSettings();

    QMenu *fileMenu;
    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *exitAction;
    QAction* _spoolAction;
    QAction* _readDeepAction;

    enum { MaxRecentFiles = 10 };
    QAction *recentFileActions[MaxRecentFiles];
    QAction *separatorAction;

    QProcess _process;


};

#endif // MAINWINDOW_H
