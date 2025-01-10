#ifndef SWW_TRANSFUNCINTENSITY_H
#define SWW_TRANSFUNCINTENSITY_H

#include <ImathVec.h>
#include "Geom/TransFunc.h"
#include <vector>
#include "Core/SwwTypeDef.h"
#include <QString>

namespace SwwGeom {
class TransFuncMappingKey;

class SWWGEOMSHARED_EXPORT TransFuncIntensity : public TransFunc {
public:
    TransFuncIntensity(int width = 256);
    virtual ~TransFuncIntensity();
    bool operator==(const TransFuncIntensity& tf) const;
    bool operator!=(const TransFuncIntensity& tf) const;
    //void updateTexture();
    void createStdFunc();
    //Imath::V4f getMeanValue(float segStart, float segEnd) const;
    Sww::Core::V4uc getMappingForValue(float value) const;
    int getNumKeys() const;
    TransFuncMappingKey* getKey(int i) const;
    const std::vector<TransFuncMappingKey*> getKeys() const;
    void setKeys(std::vector<TransFuncMappingKey*> keys);
    void addKey(TransFuncMappingKey* key);
    void updateKey(TransFuncMappingKey* key);
    void removeKey(TransFuncMappingKey* key);
    void clearKeys();
    bool isEmpty() const;
    void setThresholds(float lower, float upper);
    void setThresholds(const Imath::V2f& thresholds);
    Imath::V2f getThresholds() const;
    void updateFrom(const TransFuncIntensity& tf);

    virtual TransFuncIntensity* clone() const;
    //virtual int getNumDimensions() const { return 1; }
    virtual Imath::V2f getDomain(int dimension = 0) const;
    virtual void setDomain(Imath::V2f domain, int dimension = 0);
    virtual void setDomain(float lower, float upper, int dimension) { setDomain(Imath::V2f(lower, upper), dimension); }

    void printKeyStates()const;
    //void setWidth(int width);
    QString valueToString()const;
    void setValueString(const QString& str);

    //template <class T>
    bool genArray(Imath::V4f*& pixels,int resolition); //ピクセル配列を生成。pixelsには、4チャンネルでallocateされたポインタを渡す。



protected:
    void generateKeys(unsigned char* data);
    std::vector<TransFuncMappingKey*> keys_; ///< internal representation of the transfer function as a set of keys
    float lowerThreshold_; ///< lower threshold
    float upperThreshold_; ///< upper threshold
    Imath::V2f domain_;
    static const std::string loggerCat_; ///< logger category
};


} // ns

#endif // SWW_TRANSFUNCINTENSITY_H
