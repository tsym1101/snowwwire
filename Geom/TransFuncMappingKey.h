#ifndef SWW_TRANSFUNCMAPPINGKEY_H
#define SWW_TRANSFUNCMAPPINGKEY_H

#include <ImathVec.h>

#include "Core/SwwTypeDef.h"
#include <QString>
#include "Geom/GeomAPI.h"

namespace SwwGeom {

class  SWWGEOMSHARED_EXPORT TransFuncMappingKey {
public:
    TransFuncMappingKey(float i, const  Sww::Core::V4uc& color);
    TransFuncMappingKey();
    ~TransFuncMappingKey();
    bool operator==(const TransFuncMappingKey& key);
    bool operator!=(const TransFuncMappingKey& key);
    void setColorL(const  Sww::Core::V4uc& color);
    void setColorL(const Imath::V4i& color);
    const  Sww::Core::V4uc& getColorL() const;
    void setColorR(const  Sww::Core::V4uc& color);
    void setColorR(const Imath::V4i& color);
    const  Sww::Core::V4uc& getColorR() const;
    void setAlphaR(float a);
    void setAlphaL(float a);
    float getAlphaR() const;
    float getAlphaL() const;
    bool isSplit() const;
    void setSplit(bool split, bool useLeft=true);
    float getIntensity() const;
    void setIntensity(float i);
    const std::string& getTexNameLeft() const;
    const std::string& getTexNameRight() const;
    void setTexNameLeft(std::string& name);
    void setTexNameRight(std::string& name);
    TransFuncMappingKey* clone() const;

    void printState() const;

    QString valueToString() const;
    void setValueString(const QString& str);

private:

    float intensity_;  ///< intensity at which the key is located
    Sww::Core::V4uc colorL_; ///< color of the left part of the key
    Sww::Core::V4uc colorR_; ///< color of the right part of the key
    std::string texNameLeft_;
    std::string texNameRight_;
    bool split_;       ///< is the key split?
};

} // ns

#endif // SWW_TRANSFUNCMAPPINGKEY_H
