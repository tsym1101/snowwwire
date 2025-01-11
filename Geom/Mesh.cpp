#include "Geom/Mesh.h"
#include "Core/SwwAssert.h"

#include "Geom/GL/GLMesh.h"
#include "Core/SwwMacro.h"

#include <QElapsedTimer>

namespace SwwGeom {
Mesh::Mesh(const std::string &name):Shape(name),_flag(kNothing),_drawMode(GL_TRIANGLES),_currentGLMesh(0),_numGLMesh(1)
{
}


bool Mesh::valid()const{    
    return _meshVertices && _meshIndices;
}

void  Mesh::setVertices(const Sww::Core::V3fArrayPtr& ptr){
    _meshVertices = ptr;
    _flag |= kMustUpdateVerts;
}
void  Mesh::setNormals(const Sww::Core::V3fArrayPtr& ptr){
    _meshNormals  = ptr;
    _flag |= kMustUpdateNormals;
}
void  Mesh::setUvs(const Sww::Core::V2fArrayPtr& ptr){
    _meshUvs  = ptr;
    _flag |= kMustUpdateUvs;
}
void  Mesh::setColors(const Sww::Core::V3fArrayPtr& ptr){
    _meshColors  = ptr;
    _flag |= kMustUpdateColors;
}
void  Mesh::setAlphas(const Sww::Core::FloatArrayPtr& ptr){
    _meshAlphas = ptr;
}

void Mesh::setIndices(const Sww::Core::IntArrayPtr &ptr){
    _meshIndices = ptr;
    _flag |= kMustUpdateIndices;
}

size_t Mesh::getNumVertices()const{
    if(!_meshVertices){
        SWW_ERR("invalid mesh.");
        return 0;
    }
    return _meshVertices->size();
}

size_t Mesh::getNumIndices()const{
    if(!_meshIndices){
        SWW_ERR("invalid mesh indices.");
        return 0;
    }
    return _meshIndices->size();
}

size_t Mesh::getNumNormals()const{
    if(!_meshNormals){
        SWW_ERR("invalid normals.");
        return 0;
    }
    return _meshNormals->size();
}

size_t Mesh::getNumNormalIndices()const{
    if(!_meshNormalIndices){
        SWW_ERR("invalid normal indices.");
        return 0;
    }
    return _meshNormalIndices->size();
}

size_t Mesh::getNumUvs()const{
    if(!_meshUvs){
        SWW_ERR("invalid normals.");
        return 0;
    }
    return _meshUvs->size();
}

size_t Mesh::getNumUvIndices()const{
    if(!_meshUvIndices){
        SWW_ERR("invalid normal indices.");
        return 0;
    }
    return _meshUvIndices->size();
}

size_t Mesh::getNumColors()const{
    if(!_meshColors){
        SWW_ERR("invalid colors.");
        return 0;
    }
    return _meshColors->size();
}

size_t Mesh::getNumAlphas()const{
    if(!_meshAlphas){
        SWW_ERR("invalid alphas.");
        return 0;
    }
    return _meshAlphas->size();
}

size_t Mesh::getNumColorIndices()const{
    if(!_meshColorIndices){
        SWW_ERR("invalid color indices.");
        return 0;
    }
    return _meshColorIndices->size();
}

bool Mesh::mustTriangurate()const{
    if(_faceCounts){
        if( _faceCounts->size() == _meshIndices->size() / 3
                && _meshIndices->size() % 3 == 0){
            return false;
        }
    }
    else{
        return false;
    }
    return true;
}

bool Mesh::mustCopyVertices()const{
    return _meshNormalIndices || _meshUvIndices || _meshColorIndices;
}

Mesh::Ptr Mesh::deepCopy() const{

    if(!valid()){
        SWW_ERR("invalid mesh. copy failed.");
        return Mesh::Ptr();
    }

    Sww::Core::V3fArrayPtr oMeshP;
    Sww::Core::IntArrayPtr oMeshIndices;

    Mesh::Ptr oMeshPtr(new Mesh(_name + "_clone"));

    oMeshPtr->setBounds(_bbox);
    oMeshPtr->setTransform(_iTransform);

    oMeshP.reset(new Sww::Core::V3fArray(_meshVertices->size()));
    oMeshIndices.reset(new Sww::Core::IntArray(_meshIndices->size()));

    memcpy(  &((*oMeshP)[0]) ,(void*)&((*_meshVertices)[0]) , sizeof(Imath::V3f) * _meshVertices->size() );
    memcpy(  &((*oMeshIndices)[0]) ,(void*)&((*_meshIndices)[0]) , sizeof(int) * _meshIndices->size() );

    oMeshPtr->setVertices(oMeshP);
    oMeshPtr->setIndices(oMeshIndices);

    if(_faceCounts){
        Sww::Core::IntArrayPtr oFaceCounts;
        oFaceCounts.reset(new Sww::Core::IntArray(_faceCounts->size()));
        memcpy( &((*oFaceCounts)[0]) ,(void*)&((*_faceCounts)[0]) , sizeof(int) * _faceCounts->size() );
        oMeshPtr->setFaceCounts(oFaceCounts);
    }

    if(_meshNormals){
        Sww::Core::V3fArrayPtr oMeshNormals;
        oMeshNormals.reset(new Sww::Core::V3fArray(_meshNormals->size()) );
        memcpy(  &((*oMeshNormals)[0]) ,(void*)&((*_meshNormals)[0]) , sizeof(Imath::V3f) * _meshNormals->size() );
        oMeshPtr->setNormals(oMeshNormals);

        if(_meshNormalIndices){
            Sww::Core::Uint32ArrayPtr oMeshNormalIndices;
            oMeshNormalIndices.reset(new Sww::Core::Uint32Array(_meshNormalIndices->size()) );
            memcpy(  &((*oMeshNormalIndices)[0]) ,(void*)&((*_meshNormalIndices)[0]) , sizeof(uint32_t) * _meshNormalIndices->size() );
            oMeshPtr->setNormalIndices(oMeshNormalIndices);
        }
    }

    if(_meshUvs){
        Sww::Core::V2fArrayPtr oMeshUvs;
        oMeshUvs.reset(new Sww::Core::V2fArray(_meshUvs->size()) );
        memcpy(  &((*oMeshUvs)[0]) ,(void*)&((*_meshUvs)[0]) , sizeof(Imath::V2f) * _meshUvs->size() );
        oMeshPtr->setUvs(oMeshUvs);

        if(_meshUvIndices){
            Sww::Core::Uint32ArrayPtr oMeshUvIndices;
            oMeshUvIndices.reset(new Sww::Core::Uint32Array(_meshUvIndices->size()) );
            memcpy(  &((*oMeshUvIndices)[0]) ,(void*)&((*_meshUvIndices)[0]) , sizeof(uint32_t) * _meshUvIndices->size() );
            oMeshPtr->setUvIndices(oMeshUvIndices);
        }
    }

    if(_meshColors || _meshAlphas){

        if(_meshColors){
            Sww::Core::V3fArrayPtr oMeshColors;
            oMeshColors.reset(new Sww::Core::V3fArray(_meshColors->size()) );
            memcpy(  &((*oMeshColors)[0]) ,(void*)&((*_meshColors)[0]) , sizeof(Imath::V3f) * _meshColors->size() );
            oMeshPtr->setColors(oMeshColors);
        }

        if(_meshAlphas){
            Sww::Core::FloatArrayPtr oMeshAlphas;
            oMeshAlphas.reset(new Sww::Core::FloatArray(_meshAlphas->size()) );
            memcpy(  &((*oMeshAlphas)[0]) ,(void*)&((*_meshAlphas)[0]) , sizeof(float) * _meshAlphas->size() );
            oMeshPtr->setAlphas(oMeshAlphas);
        }

        if(_meshColorIndices){
            Sww::Core::Uint32ArrayPtr oMeshColorIndices;
            oMeshColorIndices.reset(new Sww::Core::Uint32Array(_meshColorIndices->size()) );
            memcpy(  &((*oMeshColorIndices)[0]) ,(void*)&((*_meshColorIndices)[0]) , sizeof(uint32_t) * _meshColorIndices->size() );
            oMeshPtr->setColorIndices(oMeshColorIndices);
        }
    }

    return oMeshPtr;
}

Mesh::Ptr Mesh::triangurate(bool needFaceCounts) const{

    Q_ASSERT_X(_faceCounts,"triangurate failed.","null _faceCounts");

    Sww::Core::IntArrayPtr oMeshIndicesTri;
    oMeshIndicesTri.reset(new  Sww::Core::IntArray());

    bool hasNormalIndices = (bool)_meshNormalIndices;
    bool hasUvIndices = (bool)_meshUvIndices;
    bool hasColorIndices = (bool)_meshColorIndices;

    Sww::Core::Uint32ArrayPtr oNormalIndicesTri;
    Sww::Core::Uint32ArrayPtr oUvIndicesTri;
    Sww::Core::Uint32ArrayPtr oColorIndicesTri;

    if(hasNormalIndices)oNormalIndicesTri.reset(new  Sww::Core::Uint32Array());
    if(hasUvIndices)oUvIndicesTri.reset(new  Sww::Core::Uint32Array());
    if(hasColorIndices)oColorIndicesTri.reset(new  Sww::Core::Uint32Array());

    size_t numFaces = _faceCounts->size();
    size_t numIndices = _meshIndices->size();
    size_t numPoints = _meshVertices->size();

    if ( numFaces < 1 || numIndices < 1 || numPoints < 1 )
    {
        SWW_ERR( "Mesh update quitting because bad arrays"
                << ", numFaces = " << numFaces
                << ", numIndices = " << numIndices
                << ", numPoints = " << numPoints );

        return Mesh::Ptr();
    }

    size_t faceIndexBegin = 0;
    size_t faceIndexEnd = 0;
    for ( size_t face = 0; face < numFaces; ++face )
    {
        faceIndexBegin = faceIndexEnd;
        size_t count = (*_faceCounts)[face];
        faceIndexEnd = faceIndexBegin + count;

        // Check this face is valid
        if ( faceIndexEnd > numIndices ||
             faceIndexEnd < faceIndexBegin )
        {
            SWW_ERR( "Mesh update quitting on face: " << face
                    << " because of wonky numbers"
                    << ", faceIndexBegin = " << faceIndexBegin
                    << ", faceIndexEnd = " << faceIndexEnd
                    << ", numIndices = " << numIndices
                    << ", count = " << count);

            // Just get out, make no more triangles.
            break;
        }

        // Checking indices are valid.
        bool goodFace = true;
        for ( size_t fidx = faceIndexBegin; fidx < faceIndexEnd; ++fidx )
        {
            if ( ( size_t ) ( (*_meshIndices)[fidx] ) >= numPoints )
            {
                std::cout << "Mesh update quitting on face: "
                          << face
                          << " because of bad indices"
                          << ", indexIndex = " << fidx
                          << ", vertexIndex = " << (*_meshIndices)[fidx]
                             << ", numPoints = " << numPoints
                             << std::endl;
                goodFace = false;
                break;
            }
        }

        // Make triangles to fill this face.
        if ( goodFace && count > 2 )
        {
            //counter clockwise for maya Alembic plugin !! **********************************************
            uint32_t tri0 = ( uint32_t )(*_meshIndices)[faceIndexBegin+2];
            uint32_t tri1 = ( uint32_t )(*_meshIndices)[faceIndexBegin+1];
            uint32_t tri2 = ( uint32_t )(*_meshIndices)[faceIndexBegin+0];
            oMeshIndicesTri->push_back( tri0);
            oMeshIndicesTri->push_back( tri1);
            oMeshIndicesTri->push_back( tri2);

            for ( size_t c = 3; c < count; ++c )
            {
                uint32_t tri3 = ( uint32_t )(*_meshIndices)[faceIndexBegin+c];
                uint32_t tri4 = ( uint32_t )(*_meshIndices)[faceIndexBegin+c-1];
                uint32_t tri5 = ( uint32_t )(*_meshIndices)[faceIndexBegin+0];
                oMeshIndicesTri->push_back(tri3);
                oMeshIndicesTri->push_back(tri4);
                oMeshIndicesTri->push_back(tri5);
            }
        }

        if( hasNormalIndices){
            uint32_t nIndice0 = (*_meshNormalIndices)[faceIndexBegin + 2];
            uint32_t nIndice1 = (*_meshNormalIndices)[faceIndexBegin + 1];
            uint32_t nIndice2 = (*_meshNormalIndices)[faceIndexBegin + 0];

            oNormalIndicesTri->push_back(nIndice0);
            oNormalIndicesTri->push_back(nIndice1);
            oNormalIndicesTri->push_back(nIndice2);

            for ( size_t c = 3; c < count; ++c )
            {
                ::uint32_t nIndice0Tmp = (*_meshNormalIndices)[faceIndexBegin + c];
                ::uint32_t nIndice1Tmp = (*_meshNormalIndices)[faceIndexBegin+c-1];
                ::uint32_t nIndice2Tmp = (*_meshNormalIndices)[faceIndexBegin+0];

                oNormalIndicesTri->push_back(nIndice0Tmp);
                oNormalIndicesTri->push_back(nIndice1Tmp);
                oNormalIndicesTri->push_back(nIndice2Tmp);

            }
        }

        if(hasUvIndices){
            uint32_t uvIndice0 = (*_meshUvIndices)[faceIndexBegin + 2];
            uint32_t uvIndice1 = (*_meshUvIndices)[faceIndexBegin + 1];
            uint32_t uvIndice2 = (*_meshUvIndices)[faceIndexBegin + 0];

            oUvIndicesTri->push_back(uvIndice0);
            oUvIndicesTri->push_back(uvIndice1);
            oUvIndicesTri->push_back(uvIndice2);

            for ( size_t c = 3; c < count; ++c )
            {
                uint32_t uvIndice0Tmp = (*_meshUvIndices)[faceIndexBegin + c];
                uint32_t uvIndice1Tmp = (*_meshUvIndices)[faceIndexBegin+c-1];
                uint32_t uvIndice2Tmp = (*_meshUvIndices)[faceIndexBegin+0];

                oUvIndicesTri->push_back(uvIndice0Tmp);
                oUvIndicesTri->push_back(uvIndice1Tmp);
                oUvIndicesTri->push_back(uvIndice2Tmp);

            }
        }

        if(hasColorIndices){
            uint32_t colorIndice0 = (*_meshColorIndices)[faceIndexBegin + 2];
            uint32_t colorIndice1 = (*_meshColorIndices)[faceIndexBegin + 1];
            uint32_t colorIndice2 = (*_meshColorIndices)[faceIndexBegin + 0];

            oColorIndicesTri->push_back(colorIndice0);
            oColorIndicesTri->push_back(colorIndice1);
            oColorIndicesTri->push_back(colorIndice2);

            for ( size_t c = 3; c < count; ++c )
            {
                uint32_t colorIndice0Tmp = (*_meshColorIndices)[faceIndexBegin + c];
                uint32_t colorIndice1Tmp = (*_meshColorIndices)[faceIndexBegin+c-1];
                uint32_t colorIndice2Tmp = (*_meshColorIndices)[faceIndexBegin+0];

                oColorIndicesTri->push_back(colorIndice0Tmp);
                oColorIndicesTri->push_back(colorIndice1Tmp);
                oColorIndicesTri->push_back(colorIndice2Tmp);
            }
        }

    } // for loop faces

    Mesh::Ptr oMesh = deepCopy();

    oMesh->setIndices(oMeshIndicesTri);

    if(hasNormalIndices){
        //SWW_DEBUG("uaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
        oMesh->setNormalIndices(oNormalIndicesTri);
    }
    if(hasUvIndices)oMesh->setUvIndices(oUvIndicesTri);
    if(hasColorIndices)oMesh->setColorIndices(oColorIndicesTri);

    if(!needFaceCounts){
        oMesh->setFaceCounts(Sww::Core::IntArrayPtr());
    }//nullをセット
    else{
        int numTri = oMeshIndicesTri->size() / 3;
        Sww::Core::IntArrayPtr faceCounts(new Sww::Core::IntArray(numTri,3));
        oMesh->setFaceCounts(faceCounts);
    }


    return oMesh;
}

void Mesh::genNormals(){

    if ( !_meshNormals)
    {
        if(mustTriangurate()){
            SWW_ERR("must triangurate before gen normals!");
            return;
        }

        if(_meshNormalIndices) setNormalIndices(Sww::Core::Uint32ArrayPtr() ); // nullをセット

        Sww::Core::V3fArrayPtr oNormals;
        oNormals.reset(new Sww::Core::V3fArray());
        size_t numPoints = _meshVertices->size();

        oNormals->resize( numPoints );
        std::fill( oNormals->begin(), oNormals->end(), Imath::V3f( 0.0f ) );

        size_t numTri = _meshIndices->size() / 3;
        for ( size_t tidx = 0; tidx < numTri; ++tidx )
        {
            uint32_t indexA = (*_meshIndices)[tidx * 3 + 0];
            uint32_t indexB = (*_meshIndices)[tidx * 3 + 1];
            uint32_t indexC = (*_meshIndices)[tidx * 3 + 2];

            const Imath::V3f &A = (*_meshVertices)[indexA];
            const Imath::V3f &B = (*_meshVertices)[indexB];
            const Imath::V3f &C = (*_meshVertices)[indexC];

            Imath::V3f AB = B - A;
            Imath::V3f AC = C - A;

            Imath::V3f wN = AB.cross( AC );

            (*oNormals)[indexA] += wN;
            (*oNormals)[indexB] += wN;
            (*oNormals)[indexC] += wN;
        }
        // Normalize normals.
        for ( size_t nidx = 0; nidx < numPoints; ++nidx )
        {
            (*oNormals)[nidx].normalize();
        }
        setNormals(oNormals);
    }
}

void Mesh::genTangents(){

    if(_meshNormals && _meshUvs && !mustTriangurate()){

        _meshTangents.reset(new Sww::Core::V3fArray());

        size_t numTri = _meshIndices->size() / 3;//_meshIndicesと_meshNormalIndicesは数が同じはず

        std::vector<Imath::V3f> tangents(_meshVertices->size(),Imath::V3f(0,0,0));
        //std::vector<Imath::V3f> bitangents(_meshVertices->size(),Imath::V3f(0,0,0));


        for ( size_t tidx = 0; tidx < numTri; ++tidx )
        {
            uint32_t indexA = (*_meshIndices)[tidx * 3 + 0];
            uint32_t indexB = (*_meshIndices)[tidx * 3 + 1];
            uint32_t indexC = (*_meshIndices)[tidx * 3 + 2];

            const Imath::V3f &A = (*_meshVertices)[indexA];
            const Imath::V3f &B = (*_meshVertices)[indexB];
            const Imath::V3f &C = (*_meshVertices)[indexC];

            uint32_t UvIndexA,UvIndexB,UvIndexC;

            if(_meshUvIndices){
                UvIndexA = (*_meshUvIndices)[tidx * 3 + 0];
                UvIndexB = (*_meshUvIndices)[tidx * 3 + 1];
                UvIndexC = (*_meshUvIndices)[tidx * 3 + 2];
            }
            else{
                UvIndexA = (*_meshIndices)[tidx * 3 + 0];
                UvIndexB = (*_meshIndices)[tidx * 3 + 1];
                UvIndexC = (*_meshIndices)[tidx * 3 + 2];
            }

            const Imath::V2f &UvA = (*_meshUvs)[UvIndexA];
            const Imath::V2f &UvB = (*_meshUvs)[UvIndexB];
            const Imath::V2f &UvC = (*_meshUvs)[UvIndexC];

            Imath::V3f edge1 = B - A;
            Imath::V3f edge2 = C - A;

            Imath::V2f deltaUV1 = UvB - UvA;
            Imath::V2f deltaUV2 = UvC - UvA;

            float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

            Imath::V3f tangent;
            //Imath::V3f bitangent;

            tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
            tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
            tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
            tangent.normalize();

            //            bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
            //            bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
            //            bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
            //            bitangent.normalize();

            //_meshTangents->push_back(tangent);

            //            for (uint32_t j = 0; j < 3; ++j)
            //            {
            //                uint32_t index = (*_meshIndices)[tidx * 3 + j];
            //                tangents[index] += tangent;
            //                //bitangents[index] += bitangent;
            //            }

            tangents[indexA] += tangent;
            tangents[indexB] += tangent;
            tangents[indexC] += tangent;

        }

        for (uint32_t i = 0; i < _meshVertices->size(); ++i)
        {
            _meshTangents->push_back(tangents[i].normalize());
        }

    }
    else{
        SWW_DEBUG("skip gen Tangents. " << getName());
    }

    //    SWW_DEBUG_VALUE(_meshNormals);
    //    SWW_DEBUG_VALUE(_meshUvs);
    //    SWW_DEBUG_VALUE(_meshUvIndices);
    //    SWW_DEBUG_VALUE(mustTriangurate());

}

Mesh::Ptr Mesh::copyVertices(bool needFaceCounts, bool needIndiceArray)const{
    //SWW_DEBUG("copy vertices." << getName());

    size_t numIndices = getNumIndices();

    if(!numIndices){
        return Mesh::Ptr();
    }

    if(mustTriangurate()){
        SWW_ERR("must triangulate before copy vertices. " << getName());
        return Mesh::Ptr();
    }

#ifdef QT_DEBUG
    if(_meshNormalIndices){
        //SWW_DEBUG_VALUE(numIndices);
        //SWW_DEBUG_VALUE(_meshNormalIndices->size());
        Q_ASSERT(numIndices == _meshNormalIndices->size());
    }
    if(_meshUvIndices){
        Q_ASSERT(numIndices == _meshUvIndices->size());
    }
    if(_meshColorIndices){
        Q_ASSERT(numIndices == _meshColorIndices->size());
    }
#endif

    //    _indexCacheArrayVec.clear();
    //    _indexCacheArrayVec.resize(_meshVertices->size());
    //    std::fill(_indexCacheArrayVec.begin(),_indexCacheArrayVec.end(),IndexCacheArray());

    IndexCacheArrayVec indexCacheArrayVec;
    indexCacheArrayVec.resize(_meshVertices->size());
    std::fill(indexCacheArrayVec.begin(),indexCacheArrayVec.end(),IndexCacheArray());

    Sww::Core::V3fArrayPtr oVertices;
    oVertices.reset(new Sww::Core::V3fArray(_meshVertices->begin(),_meshVertices->end()) );

    Sww::Core::IntArrayPtr oMeshIndices;
    oMeshIndices.reset(new Sww::Core::IntArray(_meshIndices->size()) );

    Sww::Core::V3fArrayPtr oMeshNormals;
    Sww::Core::V2fArrayPtr oMeshUvs;
    Sww::Core::V3fArrayPtr oMeshColors;
    Sww::Core::FloatArrayPtr oMeshAlphas;

    bool hasNormals = (bool)_meshNormals;
    bool hasUvs = (bool)_meshUvs;
    bool hasColors = (bool)_meshColors;
    bool hasAlphas = (bool)_meshAlphas;

    if(hasNormals){
        oMeshNormals.reset(new Sww::Core::V3fArray(_meshVertices->size()) );
        //memcpy(  &((*oMeshNormals)[0]) ,(void*)&((*_meshNormals)[0]) , sizeof(Imath::V3f) * _meshNormals->size() );
    }
    if(hasUvs){
        oMeshUvs.reset(new Sww::Core::V2fArray(_meshVertices->size()) );
        //memcpy(  &((*oMeshUvs)[0]) ,(void*)&((*_meshUvs)[0]) , sizeof(Imath::V2f) * _meshUvs->size() );
    }
    if(hasColors){
        oMeshColors.reset(new Sww::Core::V3fArray(_meshVertices->size()) );
        //memcpy(  &((*oMeshColors)[0]) ,(void*)&((*_meshColors)[0]) , sizeof(Imath::V3f) * _meshColors->size() );
    }
    if(hasAlphas){
        oMeshAlphas.reset(new Sww::Core::FloatArray(_meshVertices->size()) );
        //memcpy(  &((*oMeshAlphas)[0]) ,(void*)&((*_meshAlphas)[0]) , sizeof(float) * _meshAlphas->size() );
    }

    for(size_t i = 0;i < numIndices;i++){
        size_t vIndex = (*_meshIndices)[i];
        (*oMeshIndices)[i] = vIndex;

        IndexCache c0;

        if(hasNormals){
            if(_meshNormalIndices)c0.normalIndex = (*_meshNormalIndices)[i];
            else c0.normalIndex = vIndex;
        }
        if(hasUvs){
            if(_meshUvIndices)c0.uvIndex = (*_meshUvIndices)[i];
            else c0.uvIndex = vIndex;
        }
        if(hasColors || hasAlphas){
            if(_meshColorIndices)c0.colorIndex = (*_meshColorIndices)[i];
            else c0.colorIndex = vIndex;
        }

        IndexCacheArray& cacheArray = indexCacheArrayVec[vIndex];

        if(cacheArray.empty()){
            c0.newIndex = vIndex;

            if(hasNormals){
                (*oMeshNormals)[vIndex] = (*_meshNormals)[c0.normalIndex];
            }
            if(hasUvs){
                (*oMeshUvs)[vIndex] = (*_meshUvs)[c0.uvIndex];
            }
            if(hasColors){
                (*oMeshColors)[vIndex] = (*_meshColors)[c0.colorIndex];
            }
            if(hasAlphas){
                (*oMeshAlphas)[vIndex] = (*_meshAlphas)[c0.colorIndex];
            }

            cacheArray.push_back(c0);
        }
        else{
            bool alreadyExist = false;

            //search same vertex.
            for(IndexCacheArray::iterator it =  cacheArray.begin();it != cacheArray.end();++it){
                IndexCache& cUsed = *it;
                if(c0 == cUsed){
                    (*oMeshIndices)[i] = cUsed.newIndex; //set existing index.
                    alreadyExist = true;
                    break;
                }//if
            }//foreach cacheArray

            if(!alreadyExist)
            { //同じ頂点、異なる属性
                size_t newIndex = oVertices->size();
                (*oMeshIndices)[i] = newIndex; //set new index.

                oVertices->push_back((*_meshVertices)[vIndex]);

                if(hasNormals)oMeshNormals->push_back((*_meshNormals)[c0.normalIndex]);
                if(hasUvs)oMeshUvs->push_back((*_meshUvs)[c0.uvIndex]);
                if(hasColors)oMeshColors->push_back((*_meshColors)[c0.colorIndex]);
                if(hasAlphas)oMeshAlphas->push_back((*_meshAlphas)[c0.colorIndex]);

                c0.newIndex = newIndex;
                cacheArray.push_back(c0);
            }// if alreadyExist

        }// if not  cacheArray.empty()

    }//foreach numIndices

    Mesh::Ptr oMesh(new Mesh(_name + "_copied"));
    oMesh->setVertices(oVertices);
    oMesh->setIndices(oMeshIndices);

    oMesh->setBounds(_bbox);
    oMesh->setTransform(_iTransform);

    if(needFaceCounts){
        int numFace = oMeshIndices->size() / 3;
        Sww::Core::IntArrayPtr faceCounts(new Sww::Core::IntArray(numFace,3));
        oMesh->setFaceCounts(faceCounts);
    }

    if(hasNormals){
        oMesh->setNormals(oMeshNormals);
        if(needIndiceArray){
            Sww::Core::Uint32ArrayPtr oMeshNormalIndices(new Sww::Core::Uint32Array(oMeshIndices->begin(),oMeshIndices->end()));
            oMesh->setNormalIndices(oMeshNormalIndices);
        }
    }
    if(hasUvs){
        oMesh->setUvs(oMeshUvs);
        if(needIndiceArray){
            Sww::Core::Uint32ArrayPtr oMeshUvIndices(new Sww::Core::Uint32Array(oMeshIndices->begin(),oMeshIndices->end()));
            oMesh->setUvIndices(oMeshUvIndices);
        }
    }
    if(hasColors){
        oMesh->setColors(oMeshColors);
        if(needIndiceArray){
            Sww::Core::Uint32ArrayPtr oMeshColorIndices(new Sww::Core::Uint32Array(oMeshIndices->begin(),oMeshIndices->end()));
            oMesh->setColorIndices(oMeshColorIndices);
        }
    }
    if(hasAlphas){
        oMesh->setAlphas(oMeshAlphas);
        if(needIndiceArray || !oMesh->getColorIndices()){
            Sww::Core::Uint32ArrayPtr oMeshColorIndices(new Sww::Core::Uint32Array(oMeshIndices->begin(),oMeshIndices->end()));
            oMesh->setColorIndices(oMeshColorIndices);
        }
    }

    return oMesh;
}

//void Mesh::computeBounds(){
//    Q_ASSERT_X(valid(),getName().c_str(),"computeBounds failed.");

//    Shape::computeBounds(_meshVertices);
//}

void Mesh::printVertices()const{
    Q_ASSERT(_meshVertices);
    for(int i = 0;i < _meshVertices->size();i++){
        SWW_DEBUG("pt[" << i << "]=" << (*_meshVertices)[i]);

    }
}

void Mesh::printIndices()const{
    Q_ASSERT(_meshIndices);
    for(int i = 0;i < _meshIndices->size();i++){
        SWW_DEBUG("index[" << i << "]=" << (*_meshIndices)[i]);

    }
}

bool Mesh::makeValidGLMesh(Mesh::Ptr &mesh) const{

    Q_ASSERT(!mesh);

    if(mustTriangurate()){
        if(mesh)mesh = mesh->triangurate();
        else mesh = triangurate();

        if(!mesh)false;
    }

    if(!hasNormals()){
        if(mesh){
            mesh = mesh->deepCopy();
        }
        else{
            mesh = deepCopy();
        }

        mesh->genNormals();
    }

    if(mustCopyVertices()){
        if(mesh)mesh = mesh->copyVertices();
        else mesh = copyVertices();

        if(!mesh)false;
    }

    mesh->genTangents(); //normalとuvがあるときに発動

    return true;
}

bool Mesh::updateGLResources(){

    //    QElapsedTimer timer;
    //    timer.start();
    //#define USE_ROUND_ROBIN

#ifndef USE_ROUND_ROBIN
    if(!_glMesh){

        SWW_DEBUG("updateGLResources");

        _glMesh.reset(new GLMesh());

        SwwGeom::Mesh::Ptr  mesh;

        if(!makeValidGLMesh(mesh)){
            SWW_ERR("invalid.");
            _glMesh.reset();
            return false;
        }

        Mesh* meshPtr;
        if(!mesh)meshPtr = this;
        else meshPtr = mesh.get();

        Imath::V3f *vertices  =  &((*(meshPtr->getVertices()))[0]);
        size_t numVertices = meshPtr->getVertices()->size();
        uint32_t *indices  = (::uint32_t*)&((*(meshPtr->getIndices()))[0]);//uint32_t -> intのキャスト　あやしい
        size_t numIndices =  meshPtr->getIndices()->size();

        GLenum usage = GL_STATIC_DRAW;
        if(isDynamic())usage = GL_DYNAMIC_DRAW;

        _glMesh->setDrawMode(_drawMode);

        _glMesh->generate(vertices,numVertices,indices,numIndices,usage);

        if(meshPtr->hasNormals()){
            Imath::V3f *normals = &((*(meshPtr->getNormals()))[0]);
            size_t numNormals = meshPtr->getNormals()->size();
            _glMesh->generateNormals(normals,numNormals);

            if(meshPtr->hasTangents()){
                Imath::V3f *tangents = &((*(meshPtr->getTangents()))[0]);
                size_t numTangents = meshPtr->getTangents()->size();
                _glMesh->generateTangents(tangents,numTangents);
            }

        }
        if(meshPtr->hasUvs()){
            Imath::V2f *uvs = &((*(meshPtr->getUvs()))[0]);
            size_t numUvs = meshPtr->getUvs()->size();
            _glMesh->generateUvs(uvs,numUvs);
        }
        if(meshPtr->hasColors()){
            Imath::V3f *colors = &((*(meshPtr->getColors()))[0]);
            size_t numColors = meshPtr->getColors()->size();
            _glMesh->generateColors(colors,numColors);
        }

#ifdef SWW_INSTANCE_TEST

        Sww::Core::V3fArray posArray;
        posArray.push_back(Imath::V3f(0,0,0));
        posArray.push_back(Imath::V3f(3,0,0));
        _glMesh->generateInstance(&(posArray[0]),posArray.size());

//        Sww::Core::V3fArray* posArray = new Sww::Core::V3fArray();
//        posArray->push_back(Imath::V3f(0,0,0));
//        posArray->push_back(Imath::V3f(3,0,0));
//        _glMesh->generateInstance(&((*posArray)[0]),posArray->size());

#endif

    }
    else if(isDynamic() || _flag){

        if(_flag == kNothing){
            return true;
        }


        SWW_DEBUG("updateGLResources");

        SwwGeom::Mesh::Ptr  mesh;
        if(!makeValidGLMesh(mesh)){
            SWW_ERR("invalid.");
            _glMesh.reset();
            return false;
        }

        Mesh* meshPtr;
        if(!mesh)meshPtr = this;
        else meshPtr = mesh.get();

        if(_flag & kMustUpdateVerts){
            Imath::V3f *vertices  =  &((*(meshPtr->getVertices()))[0]);
            size_t numVertices = meshPtr->getVertices()->size();
            _glMesh->update(vertices,numVertices);

            //_flag & ~kMustUpdateVerts;//フラグ解除 ビット反転して論理積
        }
        if(_flag & kMustUpdateNormals){
            Imath::V3f *normals = &((*(meshPtr->getNormals()))[0]);
            size_t numNormals = meshPtr->getNormals()->size();
            _glMesh->updateNormals(normals,numNormals);

            if(meshPtr->hasTangents()){
                Imath::V3f *tangents = &((*(meshPtr->getTangents()))[0]);
                size_t numTangents = meshPtr->getTangents()->size();
                _glMesh->updateNormals(tangents,numTangents);
            }

            //_flag & ~kMustUpdateNormals;//フラグ解除
        }
        if(_flag & kMustUpdateUvs){
            Imath::V2f *uvs = &((*(meshPtr->getUvs()))[0]);
            size_t numUvs = meshPtr->getUvs()->size();
            _glMesh->updateUvs(uvs,numUvs);

            //_flag & ~kMustUpdateUvs;//フラグ解除
        }
        if(_flag & kMustUpdateColors){
            Imath::V3f *colors = &((*(meshPtr->getColors()))[0]);
            size_t numColors = meshPtr->getColors()->size();
            _glMesh->updateColors(colors,numColors);

            //_flag & ~kMustUpdateColors;//フラグ解除
        }
        if(_flag & kMustUpdateIndices){
            uint32_t *indices  = (::uint32_t*)&((*(meshPtr->getIndices()))[0]);//uint32_t -> intのキャスト　あやしい
            size_t numIndices = meshPtr->getIndices()->size();
            _glMesh->updateIndices(indices,numIndices);


            //_flag & ~kMustUpdateColors;//フラグ解除
        }
    }

    _flag = kNothing;

#else
    if(_glMeshes.empty()){

        if(!isDynamic()){
            _numGLMesh = 1;
        }else{
            _numGLMesh = 3;
        }

        //_currentGLMesh = 0;

        SwwGeom::Mesh::Ptr  mesh;
        if(!makeValidGLMesh(mesh)){
            SWW_ERR("invalid.");
            _glMeshes.clear();
            return false;
        }

        for(int i = 0;i<_numGLMesh;i++){

            GLMeshPtr glmesh(new GLMesh());

            Mesh* meshPtr;
            if(!mesh)meshPtr = this;
            else meshPtr = mesh.get();

            Imath::V3f *vertices  =  &((*(meshPtr->getVertices()))[0]);
            size_t numVertices = meshPtr->getVertices()->size();
            uint32_t *indices  = (::uint32_t*)&((*(meshPtr->getIndices()))[0]);//uint32_t -> intのキャスト　あやしい
            size_t numIndices =  meshPtr->getIndices()->size();

            GLenum usage = GL_STATIC_DRAW;
            if(isDynamic())usage = GL_DYNAMIC_DRAW;

            glmesh->setDrawMode(_drawMode);

            glmesh->generate(vertices,numVertices,indices,numIndices,usage);

            if(meshPtr->hasNormals()){
                Imath::V3f *normals = &((*(meshPtr->getNormals()))[0]);
                size_t numNormals = meshPtr->getNormals()->size();
                glmesh->generateNormals(normals,numNormals);
            }
            if(meshPtr->hasUvs()){
                Imath::V2f *uvs = &((*(meshPtr->getUvs()))[0]);
                size_t numUvs = meshPtr->getUvs()->size();
                glmesh->generateUvs(uvs,numUvs);
            }
            if(meshPtr->hasColors()){
                Imath::V3f *colors = &((*(meshPtr->getColors()))[0]);
                size_t numColors = meshPtr->getColors()->size();
                glmesh->generateColors(colors,numColors);
            }

            _glMeshes.push_back(glmesh);
            if(isDynamic())_fences.push_back(0);
        }
    }
    else if(isDynamic() || _flag){

        if(_flag == kNothing){
            return true;
        }

        checkFence();

        SwwGeom::Mesh::Ptr  mesh;
        if(!makeValidGLMesh(mesh)){
            SWW_ERR("invalid.");
            _glMeshes.clear();
            return false;
        }

        Mesh* meshPtr;
        if(!mesh)meshPtr = this;
        else meshPtr = mesh.get();

        if(_flag & kMustUpdateVerts){
            Imath::V3f *vertices  =  &((*(meshPtr->getVertices()))[0]);
            size_t numVertices = meshPtr->getVertices()->size();
            _glMeshes[_currentGLMesh]->update(vertices,numVertices);

            //_flag & ~kMustUpdateVerts;//フラグ解除 ビット反転して論理積
        }
        if(_flag & kMustUpdateNormals){
            Imath::V3f *normals = &((*(meshPtr->getNormals()))[0]);
            size_t numNormals = meshPtr->getNormals()->size();
            _glMeshes[_currentGLMesh]->updateNormals(normals,numNormals);

            //_flag & ~kMustUpdateNormals;//フラグ解除
        }
        if(_flag & kMustUpdateUvs){
            Imath::V2f *uvs = &((*(meshPtr->getUvs()))[0]);
            size_t numUvs = meshPtr->getUvs()->size();
            _glMeshes[_currentGLMesh]->updateUvs(uvs,numUvs);

            //_flag & ~kMustUpdateUvs;//フラグ解除
        }
        if(_flag & kMustUpdateColors){
            Imath::V3f *colors = &((*(meshPtr->getColors()))[0]);
            size_t numColors = meshPtr->getColors()->size();
            _glMeshes[_currentGLMesh]->updateColors(colors,numColors);

            //_flag & ~kMustUpdateColors;//フラグ解除
        }
        if(_flag & kMustUpdateIndices){
            uint32_t *indices  = (::uint32_t*)&((*(meshPtr->getIndices()))[0]);//uint32_t -> intのキャスト　あやしい
            size_t numIndices = meshPtr->getIndices()->size();
            _glMeshes[_currentGLMesh]->updateIndices(indices,numIndices);

            //_flag & ~kMustUpdateColors;//フラグ解除
        }
    }

    _flag = kNothing;

#endif

    //    qint64 t = timer.elapsed();
    //    SWW_INF("updateGLResources elapsed:" << t << "ms");

    return true;
}

bool Mesh::draw() const{

#ifndef USE_ROUND_ROBIN
    Mesh* meshTmp = const_cast<Mesh*>(this);
    SWW_CHECK_BOOL(meshTmp->updateGLResources());
    _glMesh->draw();
#else
    Mesh* meshTmp = const_cast<Mesh*>(this);
    if(isDynamic())meshTmp->swap();
    SWW_CHECK_BOOL(meshTmp->updateGLResources());
    _glMeshes[_currentGLMesh]->draw();
    if(isDynamic())meshTmp->putFence();
#endif
    return true;
}

bool Mesh::drawLegacy() const{

#ifndef USE_ROUND_ROBIN
    Mesh* meshTmp = const_cast<Mesh*>(this);
    SWW_CHECK_BOOL(meshTmp->updateGLResources());
    _glMesh->drawLegacy();
#else

    Q_ASSERT(0);
#endif
    return true;

}

void Mesh::setDrawMode(GLenum mode){
    _drawMode = mode;
    if(_glMesh)_glMesh->setDrawMode(mode);
}

void Mesh::checkFence() const{
    if(_fences[_currentGLMesh]){
        GLenum result = glClientWaitSync(_fences[_currentGLMesh],0,500000);
        if(result == GL_TIMEOUT_EXPIRED || result == GL_WAIT_FAILED){
            SWW_ERR("sync error.");
        }
    }
}

void Mesh::putFence(){
    if(_fences[_currentGLMesh])glDeleteSync(_fences[_currentGLMesh]);
    _fences[_currentGLMesh] = glFenceSync( GL_SYNC_GPU_COMMANDS_COMPLETE, 0 );
}

Shape::Ptr Mesh::shallowCopy()const{
    SwwGeom::Mesh::Ptr out(new SwwGeom::Mesh(_name));
    *out = *this;

    SWW_DEBUG_VALUE(this->_glMesh);
    SWW_DEBUG_VALUE(out->_glMesh);
    return out;
}

}//ns
