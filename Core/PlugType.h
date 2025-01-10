#ifndef SWW_PLUGTYPE_H
#define SWW_PLUGTYPE_H

#include <limits>
#include "Plug.h"
#include <QStringList>
#include <ImathVec.h>
#include "Core/PlugTypeUtil.h"
#include "Core/SwwTypeDesc.h"
#include "Core/SwwTypeDef.h"
#include "Core/CoreAPI.h"

#include <ImathBox.h>

#include <Knob/KnobTypeDesc.h>　//only header

#include <QObject>

#define DECLARE_PLUG_CREATOR(className) \
    static Sww::Core::PlugBase* creator(const std::string& plugName){return new className(plugName);}

SWW_CORE_NAMESPACE_BEGIN

template<class T>
class NumericPlug : public Plug<T>
{
public :
    NumericPlug(const std::string& name = "noName",
                const T& value = T(),
                T min = std::numeric_limits<T>::min(),
                T max = std::numeric_limits<T>::max())
        :Plug<T>(name,value),_min(min),_max(max)
    {
        setPassthrough(true);
    }
    void setMin(const T& value){_min = value;}
    void setMax(const T& value){_max = value;}
    T getMin()const{return _min;}
    T getMax()const{return _max;}

    virtual bool cloneWhenDisconnect() const{return true;}
private:
    T _min;
    T _max;
};


class BoolPlug : public Plug<bool>
{
public :
    BoolPlug(const std::string& name = "noName",bool data = false):Plug<bool>(name,data){}
    virtual SwwTypeDesc getSwwTypeDesc()const{return Sww::Core::BOOL;}
    virtual std::string getSwwTypeDescString()const{return "bool";}
    virtual PlugColor getPlugColor()const{return PlugColor(180,180,0);}
    virtual bool setValueString(const QString &valueStr){
        bool ok;
        bool value = valueStr.toInt(&ok);
        if(!ok) return false;
        return setInputValue(value);
    }
    virtual QString valueToString()
    {
        bool ok;
        bool value = getConstInputValue(&ok);
        if(!ok)
        {
            return QString();
        }
        return QString::number((int)value);
    }

    DECLARE_PLUG_CREATOR(BoolPlug)
};

class IntPlug : public NumericPlug<int>
{
public :
    IntPlug(const std::string& name = "noName",int data = 0):NumericPlug<int>(name,data){}
    virtual SwwTypeDesc getSwwTypeDesc()const{return Sww::Core::INT;}
    virtual std::string getSwwTypeDescString()const{return "int";}
    virtual PlugColor getPlugColor()const{return PlugColor(0,120,60);}
    virtual bool setValueString(const QString &valueStr){
        bool ok;
        int value = valueStr.toInt(&ok);
        if(!ok) return false;
        return setInputValue(value);
    }
    virtual QString valueToString()
    {
        bool ok;
        int value = getConstInputValue(&ok);
        if(!ok)
        {
            return QString();
        }
        return QString::number(value);
    }
    DECLARE_PLUG_CREATOR(IntPlug)
};

class V2iPlug : public Plug<Imath::V2i>
{
public :
    V2iPlug(const std::string& name = "noName",Imath::V2i data = Imath::V2i(0,0)):
        Plug<Imath::V2i>(name,data){
        setPassthrough(true);
    }
    virtual SwwTypeDesc getSwwTypeDesc()const{return Sww::Core::V2I;}
    virtual std::string getSwwTypeDescString()const{return "v2i";}
    virtual PlugColor getPlugColor()const{return PlugColor(15,200,250);}
    virtual bool setValueString(const QString &valueStr){
        bool ok;
        Imath::V2i value;
        ok = PlugTypeUtil::strToVec2i(valueStr,value);
        if(!ok) return false;
        return setInputValue(value);
    }
    virtual QString valueToString()
    {

        bool ok;
        Imath::V2i value = getConstInputValue(&ok);
        if(!ok)
        {
            SWW_ERR("valueToString failed. "  << getDebugName());
            return QString();
        }

        return  PlugTypeUtil::array2String((int*)(&value),2);
    }
    virtual bool cloneWhenDisconnect() const{return true;}

    DECLARE_PLUG_CREATOR(V2iPlug)
};

class V3iPlug : public Plug<Imath::V3i>
{
public :
    V3iPlug(const std::string& name = "noName",Imath::V3i data = Imath::V3i(0,0,0)):
        Plug<Imath::V3i>(name,data){
        setPassthrough(true);
    }
    virtual SwwTypeDesc getSwwTypeDesc()const{return Sww::Core::V3I;}
    virtual std::string getSwwTypeDescString()const{return "v3i";}
    virtual PlugColor getPlugColor()const{return PlugColor(170,170,0);}
    virtual bool setValueString(const QString &valueStr){
        bool ok;
        Imath::V3i value;
        ok = PlugTypeUtil::strToVec3i(valueStr,value);
        if(!ok) return false;
        return setInputValue(value);
    }
    virtual QString valueToString()
    {
        bool ok;
        Imath::V3i value = getConstInputValue(&ok);
        if(!ok)
        {
            SWW_ERR("valueToString failed. "  << getDebugName());
            return QString();
        }

        return  PlugTypeUtil::array2String((int*)(&value),3);
    }
    virtual bool cloneWhenDisconnect() const{return true;}
    DECLARE_PLUG_CREATOR(V3iPlug)
};

class FloatPlug : public NumericPlug<float>
{
public :
    FloatPlug(const std::string& name = "noName",float data = 0.f):NumericPlug<float>(name,data){}
    virtual SwwTypeDesc getSwwTypeDesc()const{return Sww::Core::FLOAT;}
    virtual std::string getSwwTypeDescString()const{return "float";}
    virtual PlugColor getPlugColor()const{return PlugColor(90,0,200);}
    virtual bool setValueString(const QString &valueStr){
        bool ok;
        float value = valueStr.toFloat(&ok);
        if(!ok) return false;
        return setInputValue(value);
    }
    virtual QString valueToString()
    {
        bool ok;
        float value = getConstInputValue(&ok);
        if(!ok)
        {
            return QString();
        }
        return QString::number(value);
    }
    DECLARE_PLUG_CREATOR(FloatPlug)
};

class V2fPlug : public Plug<Imath::V2f>
{
public :
    V2fPlug(const std::string& name = "noName",Imath::V2f data = Imath::V2f(0.f,0.f)):
        Plug<Imath::V2f>(name,data){
        setPassthrough(true);
    }
    virtual SwwTypeDesc getSwwTypeDesc()const{return Sww::Core::V2F;}
    virtual std::string getSwwTypeDescString()const{return "v2f";}
    virtual PlugColor getPlugColor()const{return PlugColor(120,10,200);}
    virtual bool setValueString(const QString &valueStr){
        bool ok;
        Imath::V2f value;
        ok = PlugTypeUtil::strToVec2f(valueStr,value);
        if(!ok) return false;
        return setInputValue(value);
    }
    virtual QString valueToString()
    {
        bool ok;
        Imath::V2f value = getConstInputValue(&ok);
        if(!ok)
        {
            SWW_ERR("valueToString failed. "  << getDebugName());
            return QString();
        }

        return  PlugTypeUtil::array2String((float*)(&value),2);
    }
    virtual bool cloneWhenDisconnect() const{return true;}
    DECLARE_PLUG_CREATOR(V2fPlug)
};

class V3fPlug : public Plug<Imath::V3f>
{
public :
    V3fPlug(const std::string& name = "noName",Imath::V3f data = Imath::V3f(0.f,0.f,0.f)):
        Plug<Imath::V3f>(name,data){
        setPassthrough(true);
    }
    virtual SwwTypeDesc getSwwTypeDesc()const{return Sww::Core::V3F;}
    virtual std::string getSwwTypeDescString()const{return "v3f";}
    virtual PlugColor getPlugColor()const{return PlugColor(150,10,15);}
    virtual bool setValueString(const QString &valueStr){
        bool ok;
        Imath::V3f value;
        ok = PlugTypeUtil::strToVec3f(valueStr,value);
        if(!ok) return false;
        return setInputValue(value);
    }
    virtual QString valueToString()
    {
        bool ok;
        Imath::V3f value = getConstInputValue(&ok);
        if(!ok)
        {
            SWW_ERR("valueToString failed. "  << getDebugName());
            return QString();
        }

        return  PlugTypeUtil::array2String((float*)(&value),3);
    }
    virtual bool cloneWhenDisconnect() const{return true;}
    DECLARE_PLUG_CREATOR(V3fPlug)
};

class V4fPlug : public Plug<Imath::V4f>
{
public :
    V4fPlug(const std::string& name = "noName",Imath::V4f data = Imath::V4f(0.f,0.f,0.f,1.0f)):
        Plug<Imath::V4f>(name,data){
        setPassthrough(true);
    }
    virtual SwwTypeDesc getSwwTypeDesc()const{return Sww::Core::V4F;}
    virtual std::string getSwwTypeDescString()const{return "v4f";}
    virtual PlugColor getPlugColor()const{return PlugColor(130,0,0);}
    virtual bool setValueString(const QString &valueStr){
        bool ok;
        Imath::V4f value;
        ok = PlugTypeUtil::strToVec4f(valueStr,value);
        if(!ok) return false;
        return setInputValue(value);
    }
    virtual QString valueToString()
    {
        bool ok;
        Imath::V4f value = getConstInputValue(&ok);
        if(!ok)
        {
            SWW_ERR("valueToString failed. "  << getDebugName());
            return QString();
        }

        return  PlugTypeUtil::array2String((float*)(&value),4);
    }
    virtual bool cloneWhenDisconnect() const{return true;}
    DECLARE_PLUG_CREATOR(V4fPlug)
};

class V3dPlug : public Plug<Imath::V3d>
{
public :
    V3dPlug(const std::string& name = "noName",Imath::V3d data = Imath::V3d(0.0,0.0,0.0)):
        Plug<Imath::V3d>(name,data){
        setPassthrough(true);
    }
    virtual SwwTypeDesc getSwwTypeDesc()const{return Sww::Core::V3D;}
    virtual std::string getSwwTypeDescString()const{return "v3d";}
    virtual PlugColor getPlugColor()const{return PlugColor(0,120,120);}
    virtual bool setValueString(const QString &valueStr){
        bool ok;
        Imath::V3d value;
        ok = PlugTypeUtil::strToVec3d(valueStr,value);
        if(!ok) return false;
        return setInputValue(value);
    }
    virtual QString valueToString()
    {
        bool ok;
        Imath::V3d value = getConstInputValue(&ok);
        if(!ok)
        {
            SWW_ERR("valueToString failed. "  << getDebugName());
            return QString();
        }
        return  PlugTypeUtil::array2String((double*)(&value),3);
    }
    virtual bool cloneWhenDisconnect() const{return true;}
    DECLARE_PLUG_CREATOR(V3dPlug)
};

class M33fPlug : public Plug<Imath::M33f>
{
public :
    M33fPlug(const std::string& name = "noName",
             Imath::M33f data = Imath::M33f(1.f,0.f,0.f,
                                            0.f,1.f,0.f,
                                            0.f,0.f,1.f))
        :Plug<Imath::M33f>(name,data)
    {
        setPassthrough(true);
        //setHidden(true);
    }
    virtual SwwTypeDesc getSwwTypeDesc()const{return Sww::Core::M33F;}
    virtual std::string getSwwTypeDescString()const{return "m33f";}
    virtual PlugColor getPlugColor()const{return PlugColor(120,200,210);}
    virtual bool setValueString(const QString &valueStr){
        bool ok;
        Imath::M33f value;
        ok = PlugTypeUtil::strToM33f(valueStr,value);
        if(!ok) return false;
        return setInputValue(value);
    }

    virtual QString valueToString()
    {
        bool ok;
        Imath::M33f value = getConstInputValue(&ok);
        if(!ok)
        {
            SWW_ERR("valueToString failed. "  << getDebugName());
            return QString();
        }
        return  PlugTypeUtil::array2String((float*)(&value),9);
    }
    virtual bool cloneWhenDisconnect() const{return true;}

    DECLARE_PLUG_CREATOR(M33fPlug)
};

class M44fPlug : public Plug<Imath::M44f>
{
public :
    M44fPlug(const std::string& name = "noName",
             Imath::M44f data = Imath::M44f(1.f,0.f,0.f,0.f,
                                            0.f,1.f,0.f,0.f,
                                            0.f,0.f,1.f,0.f,
                                            0.f,0.f,0.f,1.f))
        :Plug<Imath::M44f>(name,data){
        setPassthrough(true);
    }
    virtual SwwTypeDesc getSwwTypeDesc()const{return Sww::Core::M44F;}
    virtual std::string getSwwTypeDescString()const{return "m44f";}
    virtual PlugColor getPlugColor()const{return PlugColor(200,130,0);}
    virtual bool setValueString(const QString &valueStr){
        bool ok;
        Imath::M44f value;
        ok = PlugTypeUtil::strToM44f(valueStr,value);
        if(!ok) return false;
        return setInputValue(value);
    }

    virtual QString valueToString()
    {
        bool ok;
        Imath::M44f value = getConstInputValue(&ok);
        if(!ok)
        {
            SWW_ERR("valueToString failed. "  << getDebugName());
            return QString();
        }
        return  PlugTypeUtil::array2String((float*)(&value),16);
    }
    virtual bool cloneWhenDisconnect() const{return true;}

    DECLARE_PLUG_CREATOR(M44fPlug)
};

class Box3fPlug : public Plug<Imath::Box3f>
{
public :
    Box3fPlug(const std::string& name = "noName",Imath::Box3f data =
            Imath::Box3f(Imath::V3f(0.f,0.f,0.f),Imath::V3f(1.f,1.f,1.f))):
        Plug<Imath::Box3f>(name,data){
        setPassthrough(true);
    }
    virtual SwwTypeDesc getSwwTypeDesc()const{return Sww::Core::BOX3D;}
    virtual std::string getSwwTypeDescString()const{return "box3f";}
    virtual PlugColor getPlugColor()const{return PlugColor(40,30,20);}
    virtual bool setValueString(const QString &valueStr){
        bool ok;
        Imath::Box3f value;
        ok = PlugTypeUtil::strToBox3f(valueStr,value);
        if(!ok) return false;
        return setInputValue(value);
    }
    virtual QString valueToString()
    {
        bool ok;
        Imath::Box3f value = getConstInputValue(&ok);
        if(!ok)
        {
            SWW_ERR("valueToString failed. "  << getDebugName());
            return QString();
        }

        return  PlugTypeUtil::array2String((float*)(&value),6);
    }
    virtual bool cloneWhenDisconnect() const{return true;}

    DECLARE_PLUG_CREATOR(Box3fPlug)
};

class NullPlug : public Plug<void*>
{
public :
    NullPlug(const std::string& name = "noName"):Plug<void*>(name),_lockedWhenPull(true){
    }
    virtual SwwTypeDesc getSwwTypeDesc()const{return Sww::Core::_NULL;}
    virtual std::string getSwwTypeDescString()const{return "null";}
//    virtual void cloneDataTo(PlugBase *plg){
//        SWW_DEBUG("clone null plug.");
//    }
    void setLockedWhenPull(bool b){_lockedWhenPull = b;}
    bool isLockedWhenPull()const{return _lockedWhenPull;}

    DECLARE_PLUG_CREATOR(NullPlug);

private:
    bool _lockedWhenPull;//execButtonを押したときlockするかどうか
};

class VoidPlug : public Plug<void*>
{
public :
    VoidPlug(const std::string& name = "noName",void* data = 0):Plug<void*>(name,data){
        setHidden(true);
    }
    virtual SwwTypeDesc getSwwTypeDesc()const{return Sww::Core::_VOID;}
    virtual std::string getSwwTypeDescString()const{return std::string();}
    virtual PlugColor getPlugColor()const{return PlugColor(50,50,50);}

    DECLARE_PLUG_CREATOR(VoidPlug)
};

class SWWCORESHARED_EXPORT StringPlug : public Plug<std::string>
{
public :
    StringPlug(const std::string& name = "noName",
               const std::string& data = std::string())
        :Plug<std::string>(name,data){
        setPassthrough(true);
    }
    virtual ~StringPlug();
    virtual SwwTypeDesc getSwwTypeDesc()const{return Sww::Core::STRING;}
    virtual std::string getSwwTypeDescString()const{return "string";}
    virtual PlugColor getPlugColor()const{return PlugColor(220,170,220);}
    virtual bool setValueString(const QString &valueStr){
        std::string value = valueStr.toStdString();
        return setInputValue(value);
    }
    virtual QString valueToString()
    {
        bool ok;
        std::string value = getConstInputValue(&ok);
        if(!ok)
        {
            SWW_ERR("valueToString failed. "  << getDebugName());
            return QString();
        }

        return QString(value.c_str());
    }
    virtual bool cloneWhenDisconnect() const{return true;}

    DECLARE_PLUG_CREATOR(StringPlug)
};

class StringArrayPlug : public Plug<StringArrayPtr>
{
public :
    StringArrayPlug(const std::string& name = "noName",
                    const StringArrayPtr& data = StringArrayPtr(new StringArray()))
        :Plug<StringArrayPtr>(name,data){

    }
    virtual SwwTypeDesc getSwwTypeDesc()const{return Sww::Core::STRING_ARRAY;}
    virtual std::string getSwwTypeDescString()const{return "stringArray";}
    virtual PlugColor getPlugColor()const{return PlugColor(23,125,134);}
    virtual bool setValueString(const QString &valueStr){
        StringArrayPtr value;
        value.reset(new StringArray());
        QStringList sList = valueStr.split(",");
        Q_FOREACH(QString str,sList){
            value->push_back(str.toStdString());
        }
        return setInputValue(value);
    }
    virtual QString valueToString()
    {
        bool ok;
        SWW_GET(StringArrayPtr,array,this,ok);
        QStringList sList;
        SWW_VECTOR_FOREACH(std::string,(*(array.get())) ){
            sList << QString((*i).c_str());
        }
        QString result = sList.join(",");
        return result;
    }

    DECLARE_PLUG_CREATOR(StringArrayPlug)
};

class SWWCORESHARED_EXPORT FilePlugI{
public:
    enum MODE{
        kInputFile,
        kOutputFile,
        kInputDir
    };

    FilePlugI():_mode(kInputFile),_extensions(QString()){
        _repFunc = defaultReplace;
    }
    void setMode(MODE mode){_mode = mode;}
    MODE getMode()const{return _mode;}
    void setExtensionString(const QString& string){_extensions = string;}
    const QString& getExtensionString()const{return _extensions;}

    QString replace(const QString& input){
        return _repFunc(input);
    }

    void setReplaceFunction(QString (*func)(const QString&)){ //関数ポインタが引数
        _repFunc = func;
    }

private:
    MODE _mode;
    QString _extensions;

    QString (*_repFunc)(const QString&);
    //GUIでの入力時に任意の置換をするとき　連番ファイル####など
    static QString defaultReplace(const QString& input);

};

class FilePlug : public StringPlug , public FilePlugI
{
public:
    FilePlug(const std::string& name = "noName",
             const std::string& data = std::string()):StringPlug(name,data)
    {
    }

    virtual std::string getSwwTypeDescString()const{return "file";}
    DECLARE_PLUG_CREATOR(FilePlug)
};

class MultiFilePlug : public StringArrayPlug , public FilePlugI
{

public:
    MultiFilePlug(const std::string& name = "noName",
                  const StringArrayPtr& data = StringArrayPtr(new StringArray())):StringArrayPlug(name,data)
    {
        //setAsSequence(false);
    }

    virtual std::string getSwwTypeDescString()const{return "multiFile";}
    DECLARE_PLUG_CREATOR(MultiFilePlug)
};

class EnumI{
public:
    int enumCount()const{return _enumList.count();}
    const QStringList& getList()const {return _enumList;}
    EnumI& operator<<(const QString& value){
        _enumList << value;
        return *this;
    }

    void clearList(){_enumList.clear();}

protected:
    QStringList _enumList;
};

class EnumPlug : public IntPlug,public EnumI
{
public :
    EnumPlug(const std::string& name = "noName",int data = 0):IntPlug(name,data)
    {
        setKnobType(KnobTypeDesc::INTEGER::COMBO_BOX);
    }

    virtual std::string getSwwTypeDescString()const{return "enum";}
    DECLARE_PLUG_CREATOR(EnumPlug)
};

class SWWCORESHARED_EXPORT EnumStringPlug : public StringPlug,public EnumI,public QObject
{
    Q_OBJECT
public :
    EnumStringPlug(const std::string& name,
                   const std::string& data = std::string())
        :StringPlug(name,data)
    {
        setKnobType(KnobTypeDesc::STRING::COMBO_BOX);
    }

    void updateComboBox(){
        Q_EMIT listChanged();
    }

Q_SIGNALS:
    void listChanged();

public:
    virtual std::string getSwwTypeDescString()const{return "enumString";}
    DECLARE_PLUG_CREATOR(EnumStringPlug)
};


class FloatArrayPlug : public Plug<FloatArrayConstPtr>
{
public :
    FloatArrayPlug(const std::string& name = "noName")
        :Plug<FloatArrayConstPtr>(name){
        setHidden(true);
    }
    virtual SwwTypeDesc getSwwTypeDesc()const{return Sww::Core::FLOAT_ARRAY;}
    virtual std::string getSwwTypeDescString()const{return "floatArray";}
    virtual PlugColor getPlugColor()const{return PlugColor(0,255,216);}

    DECLARE_PLUG_CREATOR(FloatArrayPlug)
};

class V2fArrayPlug : public Plug<V2fArrayConstPtr>
{
public :
    V2fArrayPlug(const std::string& name = "noName")
        :Plug<V2fArrayConstPtr>(name){
        setHidden(true);
    }
    virtual SwwTypeDesc getSwwTypeDesc()const{return Sww::Core::V2F_ARRAY;}
    virtual std::string getSwwTypeDescString()const{return "v2fArray";}
    virtual PlugColor getPlugColor()const{return PlugColor(192,255,0);}

    DECLARE_PLUG_CREATOR(V2fArrayPlug)
};

class V3fArrayPlug : public Plug<V3fArrayConstPtr>
{
public :
    V3fArrayPlug(const std::string& name = "noName")
        :Plug<V3fArrayConstPtr>(name){
        setHidden(true);
    }
    virtual SwwTypeDesc getSwwTypeDesc()const{return Sww::Core::V3F_ARRAY;}
    virtual std::string getSwwTypeDescString()const{return "v3fArray";}
    virtual PlugColor getPlugColor()const{return PlugColor(255,210,0);}

    DECLARE_PLUG_CREATOR(V3fArrayPlug)
};

class V4fArrayPlug : public Plug<V4fArrayConstPtr>
{
public :
    V4fArrayPlug(const std::string& name = "noName")
        :Plug<V4fArrayConstPtr>(name){
        setHidden(true);
    }
    virtual SwwTypeDesc getSwwTypeDesc()const{return Sww::Core::V4F_ARRAY;}
    virtual std::string getSwwTypeDescString()const{return "v4fArray";}
    virtual PlugColor getPlugColor()const{return PlugColor(47,189,97);}

    DECLARE_PLUG_CREATOR(V4fArrayPlug)
};

class IntArrayPlug : public Plug<IntArrayConstPtr>
{
public :
    IntArrayPlug(const std::string& name = "noName")
        :Plug<IntArrayConstPtr>(name){
        setHidden(true);
    }
    virtual SwwTypeDesc getSwwTypeDesc()const{return Sww::Core::INT_ARRAY;}
    virtual std::string getSwwTypeDescString()const{return "intArray";}
    virtual PlugColor getPlugColor()const{return PlugColor(47,107,189);}

    DECLARE_PLUG_CREATOR(IntArrayPlug)
};

class Uint32ArrayPlug : public Plug<Uint32ArrayConstPtr>
{
public :
    Uint32ArrayPlug(const std::string& name = "noName")
        :Plug<Uint32ArrayConstPtr>(name){
        setHidden(true);
    }
    virtual SwwTypeDesc getSwwTypeDesc()const{return Sww::Core::UINT32_ARRAY;}
    virtual std::string getSwwTypeDescString()const{return "uint32Array";}
    virtual PlugColor getPlugColor()const{return PlugColor(255,0,84);}

    DECLARE_PLUG_CREATOR(Uint32ArrayPlug)
};

class Uint8ArrayPlug : public Plug<Uint8ArrayConstPtr>
{
public :
    Uint8ArrayPlug(const std::string& name = "noName")
        :Plug<Uint8ArrayConstPtr>(name){
        setHidden(true);
    }
    virtual SwwTypeDesc getSwwTypeDesc()const{return Sww::Core::UINT8_ARRAY;}
    virtual std::string getSwwTypeDescString()const{return "uint8Array";}
    virtual PlugColor getPlugColor()const{return PlugColor(0,79,134);}

    DECLARE_PLUG_CREATOR(Uint8ArrayPlug)
};

SWW_CORE_NAMESPACE_END

#endif // SWW_PLUGTYPE_H
