//#include "KnobMaker.h"

//#include<QWidget>
//#include "Core/PlugBase.h"
//#include "Core/SwwAssert.h"
//#include "Knob/KnobObject.h"

//#include "Gui/GuiGlobal.h"

//#include <QMainWindow>

//using namespace Sww::Core;

//QHash<SwwTypeDesc, QHash<SwwKnobTypeDesc,KnobCreator> > KnobMaker::_creatorFunctionContainer;

//KnobMaker::KnobMaker()
//{
//}

//bool KnobMaker::registerKnob(SwwTypeDesc dsTypeDesc, KnobCreatorFunction cFunc , SwwKnobTypeDesc knobTypeDesc, const std::string &knobName)
//{
//    if(_creatorFunctionContainer.contains(dsTypeDesc))
//    {
//        if(_creatorFunctionContainer[dsTypeDesc].contains(knobTypeDesc))
//        {
//            SWW_ERR(knobName << ":knob ID " + std::to_string(knobTypeDesc) + " already exist.");
//            return false;
//        }
//        KnobCreator c(knobName,cFunc);
//        _creatorFunctionContainer[dsTypeDesc].insert(knobTypeDesc,c);
//        return true;
//    }

//    QHash<SwwKnobTypeDesc,KnobCreator> container;
//    KnobCreator c(knobName,cFunc);
//    container.insert(knobTypeDesc,c);
//    _creatorFunctionContainer.insert(dsTypeDesc,container);

//    return true;
//}

//bool KnobMaker::create(Sww::Core::PlugBase *plug, KnobObject *&knob,QWidget *parent)
//{
//    SwwTypeDesc typeDesc = plug->getSwwTypeDesc();
//    SwwKnobTypeDesc knobTypeDesc = plug->getKnobType();

////    SWW_DEBUG_VALUE(typeDesc);
////    SWW_DEBUG_VALUE(knobTypeDesc);
////    Q_ASSERT(knobTypeDesc == 0);

//    KnobCreator c = _creatorFunctionContainer.value(typeDesc).value(knobTypeDesc);
//    KnobCreatorFunction cFunc = c._cFunc;

//    Q_ASSERT_X(cFunc,plug->getDebugName().c_str(),"not found."); //名前がなかったら0を返す

//    knob = (*cFunc)(plug,parent);
//    bool stat = knob->initialize();

//    if(!stat)
//    {
//        SWW_ERR("knob initialize failed in creation." << plug->getDebugName());
//        return false;
//    }

////    QObject::connect(knob,SIGNAL(valueModified(bool))
////            ,SwwGui::GuiGlobal::getMainWindow(),SLOT(setWindowModified(bool)));

//    return true;
//}



