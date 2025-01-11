#ifndef SWW_LINES_H
#define SWW_LINES_H

#include "Core/SwwTypeDef.h"
#include <qglobal.h>
#include "Geom/Shape.h"
#include "Core/Plug.h"

#include "Geom/GL/GeomGLTypeDefFwd.h"

#include <QFlags>

//#include <GL/glew.h>
//#include <GL/gl.h>

namespace SwwGeom {

class SWWGEOMSHARED_EXPORT Lines : public Shape
{
public:

    typedef std::shared_ptr<Lines> Ptr;
    typedef std::shared_ptr<const Lines> ConstPtr;

    enum GLStateFlag{
        kNothing               = 0x00,
        kMustUpdateVerts       = 0x01,
        kMustUpdateNormals     = 0x02,
        kMustUpdateUvs         = 0x04,
        kMustUpdateColors      = 0x08,
        kMustUpdateAlphas      = 0x10
    };

    Q_DECLARE_FLAGS(GLStateFlags, GLStateFlag)

    Lines(const std::string& name);

    Lines::Ptr deepCopy()const;

    virtual  Shape::Ptr shallowCopy()const;

    bool valid() const;

    void setVertices(const Sww::Core::V3fArrayPtr& ptr);
    void setNormals(const Sww::Core::V3fArrayPtr& ptr);
    void setUvs(const Sww::Core::V2fArrayPtr& ptr);
    void setColors(const Sww::Core::V3fArrayPtr& ptr);
    void setAlphas(const Sww::Core::FloatArrayPtr& ptr);

    void setCounts(const Sww::Core::IntArrayPtr& ptr){_counts  = ptr;}

    const Sww::Core::V3fArrayPtr&  getVertices()const{return _linesVertices;}
    const Sww::Core::V3fArrayPtr& getNormals()const{return _linesNormals;}
    const Sww::Core::V2fArrayPtr& getUvs()const{return _linesUvs;}
    const Sww::Core::V3fArrayPtr& getColors()const{return _linesColors;}
    const Sww::Core::FloatArrayPtr& getAlphas()const{return _linesAlphas;}

    const Sww::Core::IntArrayPtr& getFaceCounts()const{return _counts;}

    size_t getNumVertices()const;
    size_t getNumNormals()const;
    size_t getNumUvs()const;
    size_t getNumColors()const;
    size_t getNumAlphas()const;

    void printVertices()const;

    bool hasNormals()const{return static_cast<bool>(_linesNormals);}
    bool hasUvs()const{return static_cast<bool>(_linesUvs);}
    bool hasColors()const{return static_cast<bool>(_linesColors);}
    bool hasAlphas()const{return static_cast<bool>(_linesAlphas);}

    virtual bool draw()const;
    virtual bool drawLegacy()const;
    virtual GeomType getGeomType()const{return kLines;}

private:

    Sww::Core::V3fArrayPtr _linesVertices;
    Sww::Core::IntArrayPtr _counts;// face array. nullの場合は三角形メッシュとして処理
    Sww::Core::V3fArrayPtr _linesNormals;
    Sww::Core::V2fArrayPtr _linesUvs;
    Sww::Core::V3fArrayPtr _linesColors;
    Sww::Core::FloatArrayPtr _linesAlphas;

    GLLinesPtr _glLines;

    bool updateGLResources();

    GLStateFlags _flag;


//    //このクラスはコピー禁止にしておく
//    Lines(const  Lines&);            // コピーコンストラクタ
//    Lines operator=(const  Lines&);  // 代入演算子オーバーロード
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Lines::GLStateFlags)


}//ns

#endif // SWW_LINES_H
