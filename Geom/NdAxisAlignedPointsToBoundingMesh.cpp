#include "NdAxisAlignedPointsToBoundingMesh.h"
#include "Geom//PlugTypeGeom.h"

namespace  SwwGeom{

NdAxisAlignedPointsToBoundingMesh::NdAxisAlignedPointsToBoundingMesh()
{
}

bool NdAxisAlignedPointsToBoundingMesh::initialize()
{
    _iPoints = new SwwGeom::PointsPlug("iPoints");
    _iPoints->setConnectable(true);

    _dims = new Sww::Core::V3iPlug("dims");
    _dims->setConnectable(true);
    _dims->setStorable(true);

    _offsetHalfPixel = new Sww::Core::BoolPlug("offsetHalfPixel",false);
    _offsetHalfPixel->setConnectable(true);
    _offsetHalfPixel->setStorable(true);

    _asLevelPlane = new Sww::Core::BoolPlug("asLevelPlane",true);
    _asLevelPlane->setConnectable(true);
    _asLevelPlane->setStorable(true);

    _oMesh = new SwwGeom::MeshPlug("oMesh");
    _oMesh->setConnectable(true);
    _oMesh->setOutput(true);

    addPlug(_iPoints);
    addPlug(_dims);
    addPlug(_offsetHalfPixel);
    addPlug(_asLevelPlane);
    addPlug(_oMesh);

    _iPoints->affects(_oMesh);
    _dims->affects(_oMesh);
    _offsetHalfPixel->affects(_oMesh);
    _asLevelPlane->affects(_oMesh);

    return true;
}
QString NdAxisAlignedPointsToBoundingMesh::toolTip()const
{
    QString str =
            "格子状に並んだポイントにメッシュをはる\n"
            "\n"
            "iPoints : 入力ポイント　グリッド間隔の計算にバウンディングボックスが必要\n"
            "dims : 各軸ごとのポイント配列のサイズ\n"
            "halfPixelOffset : 点を単位グリッドの中心とみなすかどうか\n"
            "asLevelPlane : +Z向きに平面ポリゴンを生成. (z > 1の場合は　z == １ のポイントを使用)";
    return str;
}

Sww::Core::Node* NdAxisAlignedPointsToBoundingMesh::creator(){
    return new NdAxisAlignedPointsToBoundingMesh();
}

bool NdAxisAlignedPointsToBoundingMesh::execute(const Sww::Core::PlugBase *plug)
{
    SWW_VAL(_iPoints,points);
    SWW_VAL(_dims,dims);
    SWW_VAL(_offsetHalfPixel,offsetHalfPixel);
    SWW_VAL(_asLevelPlane,asLevelPlane);

    if(asLevelPlane){
        Sww::Core::V3fArrayPtr vertices;
        Sww::Core::IntArrayPtr indices;
        Sww::Core::V2fArrayPtr uvs;
        Sww::Core::Uint32ArrayPtr uvIndices;
        Sww::Core::IntArrayPtr faceCounts;

        int numVertices;
        int iDimsX;
        int iDimsY;
        int oDimsX;
        int oDimsY;
        if(offsetHalfPixel){
            iDimsX = dims.x;
            iDimsY = dims.y;
            oDimsX = dims.x + 1;
            oDimsY = dims.y + 1;
        }else{
            iDimsX = dims.x -1;
            iDimsY = dims.y -1;
            oDimsX = dims.x;
            oDimsY = dims.y;
        }

        numVertices = oDimsX * oDimsY;

        int numIndices = iDimsX * iDimsY * (3 * 2);
        int numFace = iDimsX * iDimsY * 2;

        float pixelSizeX = 1.0 / (float)dims.x;
        float pixelSizeY = 1.0 / (float)dims.y;
        float halfPixelX = pixelSizeX / 2.0;
        float halfPixelY = pixelSizeY / 2.0;
        float uvOffsetX = 0.0;
        float uvOffsetY = 0.0;

        vertices.reset(new Sww::Core::V3fArray(numVertices));
        indices.reset(new Sww::Core::IntArray(numIndices));
        uvs.reset(new Sww::Core::V2fArray(numVertices));
        uvIndices.reset(new Sww::Core::Uint32Array(numIndices));
        faceCounts.reset(new Sww::Core::IntArray(numFace,3));

        if(offsetHalfPixel){
            int extendX = dims.x +1;
            int extendY = dims.y +1;
            Imath::Box3f bounds = points->getNonInheritedBounds();
            if(bounds.isEmpty()){
                SWW_ERR("empty bounds.");
                return false;
            }

            Imath::V3f offsetBase(bounds.size().x * halfPixelX,
                                  bounds.size().y * halfPixelY,
                                  0.0);
            for(int y = 0;y < extendY;y++){
                for(int x = 0;x < extendX;x++){
                    int indexX = x;
                    int indexY = y;
                    Imath::V3f offset = -offsetBase;

                    if(x == dims.x){
                        indexX = x - 1;
                        offset.x =  offsetBase.x;
                    }
                    if (y == dims.y){
                        indexY = y - 1;
                        offset.y =  offsetBase.y;
                    }

                    int iIndex = dims.x * indexY + indexX;
                    int oIndex = extendX * y + x;

                    (*vertices)[oIndex] =(*points->getVertices())[iIndex] + offset;
                }
            }
        }else{
            memcpy(&((*vertices)[0]),&((*points->getVertices())[0]),sizeof(Imath::V3f) * numVertices);
            uvOffsetX = halfPixelX;
            uvOffsetY = halfPixelY;
        }

        for(int y = 0;y < iDimsY;y++){
            for(int x = 0;x < iDimsX;x++){
                int i0 = oDimsX * y + x;
                int i1 = oDimsX * y + x + 1;
                int i3 = oDimsX * (y + 1) + x;
                int i2 = oDimsX * (y + 1) + x + 1;
                (*indices)[iDimsX * 6 * y + x * 6 + 0] = i0;
                (*indices)[iDimsX * 6 * y + x * 6 + 1] = i1;
                (*indices)[iDimsX * 6 * y + x * 6 + 2] = i2;
                (*indices)[iDimsX * 6 * y + x * 6 + 3] = i0;
                (*indices)[iDimsX * 6 * y + x * 6 + 4] = i2;
                (*indices)[iDimsX * 6 * y + x * 6 + 5] = i3;
                (*uvIndices)[iDimsX * 6 * y + x * 6 + 0] = i0;
                (*uvIndices)[iDimsX * 6 * y + x * 6 + 1] = i1;
                (*uvIndices)[iDimsX * 6 * y + x * 6 + 2] = i2;
                (*uvIndices)[iDimsX * 6 * y + x * 6 + 3] = i0;
                (*uvIndices)[iDimsX * 6 * y + x * 6 + 4] = i2;
                (*uvIndices)[iDimsX * 6 * y + x * 6 + 5] = i3;
            }
        }

        for(int y = 0;y < oDimsY;y++){
            for(int x = 0;x < oDimsX ;x++){
                float u = pixelSizeX * x + uvOffsetX;
                float v = pixelSizeY * y + uvOffsetY;
                Imath::V2f uv(u,v);
                (*uvs)[oDimsX * y + x] = uv;
            }
        }

        SwwGeom::Mesh::Ptr meshPtr;
        meshPtr.reset(new SwwGeom::Mesh("quad"));
        meshPtr->setVertices(vertices);
        meshPtr->setIndices(indices);
        meshPtr->setFaceCounts(faceCounts);
        meshPtr->setUvs(uvs);
        meshPtr->setUvIndices(uvIndices);
        meshPtr->computeBounds(vertices);

        meshPtr->setTransform(points->getTransform());

        _oMesh->setOutputValue(meshPtr);
    }
    else{
        SWW_ERR("not supported yet.");
        return false;
    }


    return true;
}

}//ns SwwGeom
