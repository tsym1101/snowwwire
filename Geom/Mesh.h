#ifndef SWW_MESH_H
#define SWW_MESH_H

#include "Core/SwwTypeDef.h"
#include <qglobal.h>
#include "Geom/Shape.h"
#include "Core/Plug.h"

#include "Geom/GL/GeomGLTypeDefFwd.h"

#include <QFlags>

//#include <GL/glew.h>
//#include <GL/gl.h>

struct __GLsync;

namespace SwwGeom {

class SWWGEOMSHARED_EXPORT Mesh : public Shape
{
public:

    friend class Instance;

    struct IndexCache{
        uint32_t uvIndex;
        uint32_t normalIndex;
        uint32_t colorIndex;
        uint32_t newIndex;//新たに追加されたインデックス

        IndexCache():uvIndex(0),normalIndex(0),colorIndex(0),newIndex(0){}

        bool operator==(const IndexCache& cache)const{
            return (uvIndex == cache.uvIndex &&
                    normalIndex == cache.normalIndex &&
                    colorIndex == cache.colorIndex );
        }
        bool operator!=(const IndexCache& cache)const{
            return (*this == cache);
        }
    };

    typedef std::vector<IndexCache> IndexCacheArray;
    typedef std::vector<IndexCacheArray> IndexCacheArrayVec; //2d array

    typedef unsigned int GLenum;

    typedef std::shared_ptr<Mesh> Ptr;
    typedef std::shared_ptr<const Mesh> ConstPtr;

    enum GLStateFlag{
        kNothing               = 0x00,
        kMustUpdateVerts       = 0x01,
        kMustUpdateNormals     = 0x02,
        kMustUpdateUvs         = 0x04,
        kMustUpdateColors      = 0x08,
        kMustUpdateIndices     = 0x10
    };

    Q_DECLARE_FLAGS(GLStateFlags, GLStateFlag)


    Mesh(const std::string& name);

    Mesh::Ptr deepCopy()const;
    virtual  Shape::Ptr shallowCopy()const;

    Mesh::Ptr triangurate(bool needFaceCounts = false)const;
    Mesh::Ptr copyVertices(bool needFaceCounts = false,bool needIndiceArray = false)const;
    void genNormals();
    void genTangents();

    bool mustTriangurate()const;
    bool mustCopyVertices()const;

    bool valid() const;

    void setVertices(const Sww::Core::V3fArrayPtr& ptr);
    void setNormals(const Sww::Core::V3fArrayPtr& ptr);
    void setUvs(const Sww::Core::V2fArrayPtr& ptr);
    void setColors(const Sww::Core::V3fArrayPtr& ptr);
    void setAlphas(const Sww::Core::FloatArrayPtr& ptr);
    void setIndices(const Sww::Core::IntArrayPtr& ptr);

    void setFaceCounts(const Sww::Core::IntArrayPtr& ptr){_faceCounts  = ptr;}
    void setNormalIndices(const Sww::Core::Uint32ArrayPtr& ptr){_meshNormalIndices = ptr;}
    void setUvIndices(const Sww::Core::Uint32ArrayPtr& ptr){_meshUvIndices  = ptr;}
    void setColorIndices(const Sww::Core::Uint32ArrayPtr& ptr){_meshColorIndices  = ptr;}

    const Sww::Core::V3fArrayPtr&  getVertices()const{return _meshVertices;}
    const Sww::Core::V3fArrayPtr& getNormals()const{return _meshNormals;}
    const Sww::Core::V3fArrayPtr& getTangents()const{return _meshTangents;}
    const Sww::Core::V2fArrayPtr& getUvs()const{return _meshUvs;}
    const Sww::Core::V3fArrayPtr& getColors()const{return _meshColors;}
    const Sww::Core::FloatArrayPtr& getAlphas()const{return _meshAlphas;}

    const Sww::Core::IntArrayPtr& getIndices()const{return _meshIndices;}
    const Sww::Core::IntArrayPtr& getFaceCounts()const{return _faceCounts;}
    const Sww::Core::Uint32ArrayPtr& getNormalIndices()const{return _meshNormalIndices;}
    const Sww::Core::Uint32ArrayPtr& getUvIndices()const{return _meshUvIndices;}
    const Sww::Core::Uint32ArrayPtr& getColorIndices()const{return _meshColorIndices;}

    size_t getNumVertices()const;
    size_t getNumIndices()const;
    size_t getNumNormals()const;
    size_t getNumNormalIndices()const;
    size_t getNumUvs()const;
    size_t getNumUvIndices()const;
    size_t getNumColors()const;
    size_t getNumAlphas()const;
    size_t getNumColorIndices()const;

    //void computeBounds();

    void printVertices()const;
    void printIndices()const;

    bool hasNormals()const{return static_cast<bool>(_meshNormals);}
    bool hasUvs()const{return static_cast<bool>(_meshUvs);}
    bool hasColors()const{return static_cast<bool>(_meshColors);}
    bool hasAlphas()const{return static_cast<bool>(_meshAlphas);}
    bool hasTangents()const{return static_cast<bool>(_meshTangents);}

    virtual bool draw()const;
    virtual bool drawLegacy()const;
    virtual GeomType getGeomType()const{return kMesh;}

    void setDrawMode(GLenum mode);

private:

    Sww::Core::V3fArrayPtr _meshVertices;
    Sww::Core::IntArrayPtr _meshIndices;
    Sww::Core::IntArrayPtr _faceCounts;// face array. nullの場合は三角形メッシュとして処理
    Sww::Core::V3fArrayPtr _meshNormals;
    Sww::Core::Uint32ArrayPtr _meshNormalIndices;
    Sww::Core::V3fArrayPtr _meshTangents;
    Sww::Core::V2fArrayPtr _meshUvs;
    Sww::Core::Uint32ArrayPtr _meshUvIndices;
    Sww::Core::V3fArrayPtr _meshColors;
    Sww::Core::FloatArrayPtr _meshAlphas;
    Sww::Core::Uint32ArrayPtr _meshColorIndices;

#ifndef SWW_INSTANCE_TEST
//#define SWW_INSTANCE_TEST
#endif
#ifdef SWW_INSTANCE_TEST
    Sww::Core::V3fArrayPtr _meshInstance;
    bool hasInstance()const{return static_cast<bool>(_meshInstance);}
#endif

    GLMeshPtr _glMesh;

    //for round robin########################
    typedef std::vector<GLMeshPtr> GLMeshPtrVec;
    GLMeshPtrVec _glMeshes;
    int _currentGLMesh;
    int _numGLMesh;
    std::vector<__GLsync*> _fences;
    //#######################################

    bool updateGLResources();
    bool makeValidGLMesh(Mesh::Ptr& mesh)const;
    void putFence();
    void checkFence() const;
    inline void swap(){_currentGLMesh = (_currentGLMesh + 1) % _numGLMesh;}

    GLStateFlags _flag;

    GLenum _drawMode;

//    //このクラスはコピー禁止にしておく
//    Mesh(const  Mesh&);            // コピーコンストラクタ
//    Mesh operator=(const  Mesh&);  // 代入演算子オーバーロード
};

Q_DECLARE_OPERATORS_FOR_FLAGS(Mesh::GLStateFlags)


}//ns

#endif // SWW_MESH_H
