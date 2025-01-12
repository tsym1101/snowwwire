//#ifndef KNOBMAKER_H
//#define KNOBMAKER_H

//#include "Core/SwwTypeDesc.h"
//#include "Knob/KnobTypeDesc.h"
//#include "Gui/GuiAPI.h"
//#include <QHash>

//class QWidget;

//namespace Sww::Core{
//class PlugBase;
//}

//class KnobObject;

//typedef KnobObject* (*KnobCreatorFunction)(Sww::Core::PlugBase *plug,QWidget* parent);

//struct  KnobCreator{
//    KnobCreator():_knobName("none"),_cFunc(0){}
//    KnobCreator(const std::string& name,KnobCreatorFunction cFunc):_knobName(name),_cFunc(cFunc){}
//    std::string _knobName;
//    KnobCreatorFunction _cFunc;
//};

//class SWWGUISHARED_EXPORT KnobMaker
//{
//public:
//    KnobMaker();
//    //static bool registerKnob(int dsTypeDesc, KnobCreatorFunction cFunc ,int knobTypeDesc = 0);
//    static bool registerKnob(SwwTypeDesc dsTypeDesc, KnobCreatorFunction cFunc ,SwwKnobTypeDesc knobTypeDesc,const std::string& knobName);
//    static bool create(Sww::Core::PlugBase *plug,KnobObject*& knob,QWidget *parent = 0);
//private:
//    //型情報→knobタイプの順に格納
//    //static QHash<int, QHash<int,KnobCreatorFunction> > _creatorFunctionContainer;
//    static QHash<SwwTypeDesc,QHash<SwwKnobTypeDesc,KnobCreator> > _creatorFunctionContainer;
//};

//#endif // KNOBMAKER_H
