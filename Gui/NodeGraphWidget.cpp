#include "NodeGraphWidget.h"

#include "Core/NodeRegister.h"
#include <QList>
#include <QGraphicsTextItem>
#include <QFileDialog>

#include <qmath.h>
#include <QScrollBar>

#include "Gui/NodeItem.h"
#include "Core/Node.h"
#include "Gui/NodeGraphScene.h"
//#include <GL/glew.h>

#include "Gui/KnobWidget.h"

//#include "Util/IOUtility.h"
//#include "OpenGL/GLViewer.h"
#include "OpenGL/GLGlobal.h"
#include "OpenGL/IGLObject.h"

#include "Gui/Connector.h"
#include "Geom/Bounds.h"

#include "Gui/GuiGlobal.h"
#include "Knob/CommandStack.h"

#include "Core/RootNode.h"
#include "Gui/Command.h"

#include "Util/IOUtility.h"

using namespace  SwwGui;

Imath::V2f toV2f(const QPointF& p){
    return Imath::V2f(p.x(),p.y());
}

NodeGraphWidget::NodeGraphWidget(KnobWidget *knobWidget,
                                 QWidget *parent)
    :QGraphicsView(parent),
      _currentScene(0),_readyForMove(false),_readyForScale(false),_knobWidget(knobWidget),_rootNode(0)
{
    //setBackgroundBrush(QBrush(QColor(10,10,10), Qt::SolidPattern));

    QString style =   "QWidget{background-color: rgb(30, 30, 30)}";
    setStyleSheet(style);

    //setCacheMode(CacheBackground);
    //setViewportUpdateMode(FullViewportUpdate);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setTransformationAnchor(AnchorUnderMouse);
    setWindowTitle(tr("NodeGraph"));
    setDragMode(QGraphicsView::RubberBandDrag);
    createContextMenu();    //右クリックメニュー作成
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setMinimumWidth(400);


    setCurrentPathText("/");
    //_metaText["project"] = "hoge";
    //setInteractive(true);
    // setAlignment(Qt::AlignLeft|Qt::AlignTop);

}

NodeGraphWidget::~NodeGraphWidget()
{
}

void NodeGraphWidget::setRootNode(Sww::Core::RootNode *node){
    _rootNode = node;
}

void NodeGraphWidget::paintEvent(QPaintEvent *event){

    QGraphicsView::paintEvent(event);

    //overray viewport ********************************
    QPainter p(viewport());//thisを渡すとだめだった

    p.setPen(Qt::gray);
    int line = 20;
    int lineHeight = 20;
    Q_FOREACH(QString key,_metaText.keys()){
        p.drawText(QPointF(20,line),key + " : " + _metaText[key]);
        line += lineHeight;
    }

}

bool NodeGraphWidget::setScene(const std::string& path , bool withSetup){
    SwwGui::NodeGraphScene* ngs = getNodeGraphSceneByPath(path);
    if(!ngs) return false;

    _knobWidget->clear();

    if(_currentScene && (_currentScene != ngs) ){
        _currentScene->clearNodesAndConnectorsSelection();
        QObject::disconnect(_currentScene,SIGNAL(nodeIsSelected()),this,SLOT(postSelectNodes()));
    }

    _currentScene = ngs;
    QGraphicsView::setScene(ngs);

    if(_currentScene){
        ngs->setItemIndexMethod(QGraphicsScene::NoIndex);
        QObject::connect(_currentScene,SIGNAL(nodeIsSelected()),this,SLOT(postSelectNodes()));
    }

    if(withSetup){
        _currentScene->setup();
    }

    setCurrentPathText(_currentScene->node()->getPath().c_str());

    return true;
}

void NodeGraphWidget::setSceneWithStackCommand(const std::string &path, bool withSetup){
    std::string beforePath = _currentScene->getPath();
    if(setScene(path,withSetup)){
        SwwGui::ChangeSceneCommand* cmd = new SwwGui::ChangeSceneCommand(beforePath,path,this);
        SwwKnob::CommandStack::pushCommand(cmd);
    }
}

void NodeGraphWidget::updateKnobWidget()
{
    _knobWidget->clear();
    QList<QGraphicsItem*> items = _currentScene->selectedItems();
    Q_FOREACH (QGraphicsItem* item, items){
        NodeItem* n = qgraphicsitem_cast< NodeItem*>(item);
        if(n){
            n->updateKnobs();
            _knobWidget->addKnob(n->getKnob());
        }
    }
}

//void NodeGraphWidget::prepareUpdateSelection()
//{
//    _selectionChanged = true;
//}

void NodeGraphWidget::selectIGLNodes()
{
    SwwOpenGL::GLGlobal::clearSelection();

    QList<QGraphicsItem*> items = _currentScene->selectedItems();
    Q_FOREACH (QGraphicsItem* item, items){
        NodeItem* n = qgraphicsitem_cast< NodeItem*>(item);
        if(n){
            Sww::Core::Node* nodeTmp = n->node();
            SwwGeom::Bounds* bounds = dynamic_cast<SwwGeom::Bounds*>(nodeTmp);
            if(bounds){
                SwwOpenGL::GLGlobal::addBoundsToSelection(bounds);
            }

        }
    }
}

void NodeGraphWidget::createContextMenu()
{
    _myContextMenu = new QMenu(this);

    std::map<std::string,QMenu*> subMenus;


    std::vector<std::string> categorys = Sww::Core::NodeRegister::getCategorys();

    for(std::vector<std::string>::iterator i = categorys.begin(); i != categorys.end() ; ++i)
    {
        std::string value = *i;
        subMenus[value] = _myContextMenu->addMenu(value.c_str());
    }

    std::vector<std::string> nodeList = Sww::Core::NodeRegister::getNodeNames();
    for(std::vector<std::string>::iterator i = nodeList.begin(); i != nodeList.end() ; ++i){
        const std::string& name = *i;
        if(name == "Global") continue;

        QAction *createNodeAction = new QAction(name.c_str(),this);
        const std::string& category = Sww::Core::NodeRegister::getCategory(name);
        subMenus[category]->addAction(createNodeAction);

        //動的にコンテキストメニューを生成するために、NodeMakerクラスに値を渡してSlotに接続。自動で解放されるように、thisを親とする
        NodeMaker *nodeMaker = new NodeMaker(name.c_str(),this);
        connect(createNodeAction,SIGNAL(triggered()),nodeMaker,SLOT(createNode()));
    }




    std::vector<std::string> modList = Sww::Core::NodeRegister::getModNames();
    for(std::vector<std::string>::iterator i = modList.begin(); i != modList.end() ; ++i){
        const std::string& name = *i;

        QString modNameTmp = QString("#") + QString(name.c_str());
        QAction *createModAction = new QAction(modNameTmp,this);
        //QWidgetAction *createModAction = new QWidgetAction(name.c_str(),this);
        const std::string& category = Sww::Core::NodeRegister::getCategory(name);
        subMenus[category]->addAction(createModAction);

        NodeMaker *nodeMaker = new NodeMaker(name.c_str(),this);
        connect(createModAction,SIGNAL(triggered()),nodeMaker,SLOT(createMod()));
    }
}

void NodeGraphWidget::createNode(const QString &nodeType, const QPointF &pos, const QString &nodeName){
    _currentScene->createNode(nodeType,pos,nodeName);
}

void NodeGraphWidget::createMod(const std::string& jsonStr){

    QPointF mousePos = getMousePos();
    _currentScene->createMod(jsonStr,mousePos);

}

void NodeGraphWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Tab:{
        QPointF p = getMousePos();
        NodeMaker::setMousePos(p);
        _myContextMenu->exec(QCursor::pos());
    }
    case Qt::Key_Space:{
        QPointF p = getMousePos();
        NodeMaker::setMousePos(p);
        _myContextMenu->exec(QCursor::pos());

        break;
    }
    case Qt::Key_D:{
        //        QPointF mousePos = getMousePos();
        //        SWW_DEBUG_VALUE(mousePos.x());
        //        SWW_DEBUG_VALUE(mousePos.y());
        SwwKnob::CommandStack::printCommnadStack();
        break;
    }
    case Qt::Key_Plus:{
        scale(1.1,1.1);
        break;
    }
    case Qt::Key_Minus:{
        scale(0.9,0.9);
        break;
    }
    case Qt::Key_Alt:{
        setDragMode(QGraphicsView::ScrollHandDrag);
        setInteractive(false);
        break;
    }
    case Qt::Key_Delete:{
        _currentScene->deleteSelectedNodes();
        break;
    }
    case Qt::Key_F:{
//        QRectF sceneRect = scene()->sceneRect();
        QRectF sceneRect = _currentScene->getSceneRect();
//        QPointF tl = sceneRect.topLeft() + _currentScene->getRootItem()->pos();
//        QPointF tl = sceneRect.topLeft();
//        sceneRect.moveTo(tl);

//        sceneRect = transform().mapRect(sceneRect);

//        sceneRect.translate(-x(),-y());
//        SWW_DEBUG_VALUE(sceneRect.x());
//        SWW_DEBUG_VALUE(sceneRect.y());
        fitInView(sceneRect, Qt::KeepAspectRatio);
        break;
    }
    case Qt::Key_Up:{
        //NodeGraphScene* ngs = _currentScene->parentNgs();
        Sww::Core::Node* parentNode = _currentScene->node()->parentNode();
        if(parentNode){
            setSceneWithStackCommand(parentNode->getPath(),false);
        }

        break;
    }
    case Qt::Key_S:{
        QPointF p = getMousePos();
        _currentScene->createNode("GLShader",p);
        break;
    }
    case Qt::Key_C:{
        if(event->modifiers() & Qt::ControlModifier){
            _currentScene->copyToClipBoard();
        }
        break;
    }
    case Qt::Key_F1:{
        std::string path = _currentScene->node()->getPath();
        Sww::Core::Node* n = _currentScene->node()->getNodeFromPath(path);
        SWW_DEBUG_VALUE(path);
        SWW_DEBUG_VALUE(_currentScene->node());
        SWW_DEBUG_VALUE(n);
        if(n){SWW_DEBUG_VALUE(n->getPath());}
        else{ SWW_ERR("invalid.");}
        break;
    }
    case Qt::Key_V:{
        if(event->modifiers() & Qt::ControlModifier){
            QClipboard *clipboard = QApplication::clipboard();
            std::string jsonStr = clipboard->text().toStdString();
            QPointF mousePos = getMousePos();
            SWW_DEBUG_VALUE(mousePos.x());
            SWW_DEBUG_VALUE(mousePos.y());
            _currentScene->createMod(jsonStr,mousePos);
            SWW_DEBUG("pasted.");
            break;
        }
        else{
            QPointF p = getMousePos();
            _currentScene->createNode("GLViewer",p);
        }
        break;
    }
    case Qt::Key_G:{
        if(event->modifiers() & Qt::ControlModifier){
            QPointF p = getMousePos();
            _currentScene->groupingNodes(p);
            break;
        }
        break;
    }


    }

    QWidget::keyPressEvent(event);
}

void NodeGraphWidget::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Alt:{
        setDragMode(QGraphicsView::RubberBandDrag);
        setInteractive(true);
    }
    }

    QWidget::keyReleaseEvent(event);
}

void NodeGraphWidget::enterEvent(QEvent *)
{
    // setFocus();
}


void NodeGraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{

     QGraphicsView::drawBackground(painter, rect);

    auto drawGrid =
            [&](double gridStep)
    {
        QRect   windowRect = this->rect();
        QPointF tl = mapToScene(windowRect.topLeft());
        QPointF br = mapToScene(windowRect.bottomRight());

        double left   = std::floor(tl.x() / gridStep - 0.5);
        double right  = std::floor(br.x() / gridStep + 1.0);
        double bottom = std::floor(tl.y() / gridStep - 0.5);
        double top    = std::floor (br.y() / gridStep + 1.0);

        // vertical lines
        for (int xi = int(left); xi <= int(right); ++xi)
        {
            QLineF line(xi * gridStep, bottom * gridStep,
                        xi * gridStep, top * gridStep );

            painter->drawLine(line);
        }

        // horizontal lines
        for (int yi = int(bottom); yi <= int(top); ++yi)
        {
            QLineF line(left * gridStep, yi * gridStep,
                        right * gridStep, yi * gridStep );
            painter->drawLine(line);
        }
    };

    QBrush bBrush = backgroundBrush();

    QTransform t = transform();
    if(t.m11() > 0.4){
        QPen pfine(Qt::black, 1.0);
        painter->setPen(pfine);
        drawGrid(100);
    }

    QPen p(Qt::darkGray, 1.0);
    painter->setPen(p);
    drawGrid(500);
}


void NodeGraphWidget::scaleUp(double step){
    double const factor = std::pow(step, 1.0);

    QTransform t = transform();

    if (t.m11() > 2.0)
        return;

    scale(factor, factor);
}


void NodeGraphWidget::scaleDown(double step){
    double const factor = std::pow(step, -1.0);

    scale(factor, factor);
}

void NodeGraphWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton)
    {
        if(event->modifiers() & Qt::AltModifier){
            _readyForScale = true;
            _lastMousePos = event->pos();//scaleの時はスクリーンスペース
        }
        else{
            _readyForMove = true;
            setCursor(Qt::OpenHandCursor);
            _lastMousePos = mapToScene(event->pos()); //moveの時はシーンスペース。スケールを考慮するため。
        }

        return;
    }
    else if ((event->buttons() & Qt::LeftButton) &&
             event->modifiers() & Qt::ShiftModifier)
    {
        _readyForMove = true;
        setCursor(Qt::OpenHandCursor);
        _lastMousePos = mapToScene(event->pos());
        return;
    }

    _currentScene->prepareSelectCommand();

    QGraphicsView::mousePressEvent(event);//選択が変われば、ここで_currentScene->_selectionChangedがtrueになる

    _currentScene->storeSelectedNodesAndConnectors();//_currentScene->_selectionChangedがfalseならスキップ
    _currentScene->prepareMoveCommand();

}

void NodeGraphWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (_readyForMove)
    {

        QPointF difference = _lastMousePos - mapToScene(event->pos());
        setSceneRect(sceneRect().translated(difference.x(), difference.y()));

        return;
    }
    else if(_readyForScale){

        QPointF difference = _lastMousePos - event->pos();

        double d;
        if(std::abs(difference.y()) > std::abs(difference.x()))d = - difference.y();
        else d = - difference.x();

        if (d > 0.0){
            scaleUp();
        }
        else{
            scaleDown();
        }

        _lastMousePos = event->pos();

        return;
    }

    QGraphicsView::mouseMoveEvent(event);
}

void NodeGraphWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (_readyForMove)
    {
        _readyForMove = false;
        setCursor(Qt::ArrowCursor);
        event->ignore();
        return;
    }
    else if(_readyForScale){
        _readyForScale = false;
        event->ignore();
        return;
    }


//    SWW_DEBUG_VALUE(sceneRect().topLeft().x());
//    SWW_DEBUG_VALUE(sceneRect().topLeft().y());

    _currentScene->storeSelectedNodesAndConnectors();
    _currentScene->pushSelectCommand();
    _currentScene->pushMoveCommand();

    QGraphicsView::mouseReleaseEvent(event);
}

void NodeGraphWidget::wheelEvent(QWheelEvent *event)
{


    QPoint delta = event->angleDelta();

    if (delta.y() == 0)
    {
        event->ignore();
        return;
    }

    double const d = delta.y() / std::abs(delta.y());


    if(d > 0.0){
        scaleUp();
    }
    else{
        scaleDown();
    }
}

void NodeGraphWidget::mouseDoubleClickEvent(QMouseEvent *event){

    QGraphicsView::mouseDoubleClickEvent(event);
}

void NodeGraphWidget::readDeepLayers()
{
    QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;

    QString dirStr = QFileDialog::getExistingDirectory(
                this,
                tr("select DeepImage Layer Folder."),
                tr("初期ディレクトリ"), options);

    if ( !dirStr.isEmpty() )
    {
        QDir dir(dirStr);
        if(dir.exists())
        {
            QStringList layerDirlist = dir.entryList(QDir::Dirs,QDir::Name);
            QRegExp reg("^L\\d+$"); //^文頭 Lそのまま \\d数字 +1回以上の繰り返し  $文末
            //QStringList filelist;

            QList<NodeItem*> layerNodes; //あとでコネクトする
            int step = 0;//位置設定用
            Q_FOREACH(QString layerFolder,layerDirlist)
            {
                if(reg.exactMatch(layerFolder))
                {
                    QDir layerDir(dir.absolutePath() +  "/" + layerFolder);
                    QStringList filter;
                    filter += "*.exr";
                    QStringList entryList = layerDir.entryList(filter,QDir::Files);
                    if(entryList.isEmpty()){
                        SWW_ERR("supported file is not exist.");
                        return;
                    }
                    QString oneFile = entryList.first();//連番最初の1ファイル
                    oneFile = layerDir.absolutePath() + "/" + oneFile;

                    std::string oneFileStd = Sww::Util::IOUtility::makeSeaqenceName(oneFile.toStdString());

                    NodeItem* deepImageItem = _currentScene->createNode("GLDeepMesh",QPointF(-500,step * 250));
                    //_currentScene->setValue(deepImageItem->node()->getName(),"fileName",oneFile.toStdString());//ノード名でsetValueはすごく無駄
                    deepImageItem->node()->setValueString("fileName",
                                                          oneFileStd.c_str());//ノード名でsetValueはすごく無駄


                    layerNodes << deepImageItem;
                    step++;
                }
            }

            NodeItem* dpNodeItem = _currentScene->createNode("GLDepthPeeler",QPointF(0,0));

            Q_FOREACH (NodeItem* item, layerNodes) {
                _currentScene->connect(item->node()->getName().c_str(),"drawInit",dpNodeItem->node()->getName().c_str(),"passInit");//ノード名でconnectはすごく無駄
                _currentScene->connect(item->node()->getName().c_str(),"drawPeel",dpNodeItem->node()->getName().c_str(),"passPeel");//ノード名でconnectはすごく無駄
            }

        }
    }
}

QPointF NodeGraphWidget::getMousePos(){
    QPointF posInScene = mapToScene(mapFromGlobal(QCursor::pos()));
    Q_ASSERT(_currentScene);
    QPointF posInRootItem = _currentScene->getRootItem()->mapFromScene(posInScene);
    //QPointF posInRootItem = _currentScene->rootMapFromScene(posInScene);
    return posInRootItem;
}

void NodeGraphWidget::setCurrentPathText(const QString &path){
    _metaText["path"] = path;
}

void NodeGraphWidget::postSelectNodes(){
    updateKnobWidget();
    selectIGLNodes();
}

SwwGui::NodeGraphScene* NodeGraphWidget::getNodeGraphSceneByPath(const std::string& path){
    SwwGui::NodeGraphScene* ngs = _scenes.value(path);
    if(ngs){
        return ngs;
    }
    else{
        Q_ASSERT(_rootNode);
        Sww::Core::Node* n = _rootNode->getNodeFromPath(path);
        if(n){
            ngs = new SwwGui::NodeGraphScene(n,this);
            _scenes[path] = ngs;
            ngs->setup();
            return ngs;
        }else{
            SWW_ERR("get NodeGraphScene failed. " << path);
            return 0;
        }
    }
}
