#include "Poly2triHelper.h"
#include "poly2tri.h"

namespace SwwOpenGL {
using namespace p2t;

Poly2triHelper::Poly2triHelper(Sww::Core::V2fArray& verts) : _org(verts), _numVertex(0), _numTriangles(0) {
    for (int i = 0; i < verts.size(); i++) {
        Imath::V2f& v = verts[i];
        PointPtr p = std::make_shared<p2t::Point>(v.x, v.y);
        _pointPtrVec.push_back(p);
        _polyLine.push_back(p.get());
    }

    _cdtPtr = std::make_unique<p2t::CDT>(_polyLine);
    _numVertex = verts.size();
}

bool Poly2triHelper::getTriangles(Sww::Core::V2fArray& oPosArray,
                                  Sww::Core::Uint32Array& oIndiceArray) {
    bool valid = true;
    for (int i = 0; i < _polyLine.size() - 2; i++) {
        for (int j = i + 1; j < _polyLine.size() - 1; j++) {
            if (_polyLine[i] == _polyLine[j]) {
                valid = false;
                break;
            }
        }
        if (!valid) break;
    }

    if (!valid) {
        oPosArray = _org;
        for (int i = 0; i < _org.size(); i++) {
            oIndiceArray.push_back(i);
        }
        return false;
    }

    _cdtPtr->Triangulate();

    std::vector<p2t::Triangle*> triangles = _cdtPtr->GetTriangles();
    _numTriangles = triangles.size();

    std::vector<UsedPoint> used;

    for (int l = 0; l < triangles.size(); l++) {
        p2t::Triangle* t = triangles[l];

        for (int n = 0; n < 3; n++) {
            p2t::Point& p = *(t->GetPoint(n));

            bool isUsed = false;
            for (int m = 0; m < used.size(); m++) {
                if (p == *(used[m].p)) {
                    oIndiceArray.push_back(used[m].index);
                    isUsed = true;
                    break;
                }
            }
            if (isUsed) continue;

            Imath::V2f pos;
            pos.x = p.x;
            pos.y = p.y;

            UsedPoint usedP;

            oPosArray.push_back(pos);
            uint32_t vIndex = oPosArray.size() - 1;
            oIndiceArray.push_back(vIndex);

            usedP.p = &p;
            usedP.index = vIndex;
            used.push_back(usedP);
        }
    }

    if (used.size() != _org.size()) {
        oPosArray = _org;
        oIndiceArray.clear();

        for (int i = 0; i < _org.size(); i++) {
            oIndiceArray.push_back(i);
        }
        return false;
    }

    return true;
}

} // namespace SwwOpenGL
