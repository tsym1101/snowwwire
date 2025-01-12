
#include "Geom/TransFuncMappingKey.h"
#include <QStringList>

namespace SwwGeom {

TransFuncMappingKey::TransFuncMappingKey(float i, const  Sww::Core::V4uc& color)
    : intensity_(i)
    , colorL_(color)
    , colorR_(color)
//    , texL_(0)
//    , texR_(0)
    , texNameLeft_("")
    , texNameRight_("")
    , split_(false)
{}

TransFuncMappingKey::TransFuncMappingKey()
    : intensity_(0.f)
    , colorL_(0.f)
    , colorR_(0.f)
//    , texL_(0)
//    , texR_(0)
    , texNameLeft_("")
    , texNameRight_("")
    , split_(false)
{}

TransFuncMappingKey::~TransFuncMappingKey() {
}

bool TransFuncMappingKey::operator==(const TransFuncMappingKey& key) {
    return (intensity_ == key.intensity_) && (split_ == key.split_) &&
            (colorR_    == key.colorR_) && (colorL_ == key.colorL_);
}

bool TransFuncMappingKey::operator!=(const TransFuncMappingKey& key) {
    return !(*this == key);
}

void TransFuncMappingKey::setColorL(const  Sww::Core::V4uc& color) {
    colorL_ = color;
    if (!split_)
        colorR_ = color;
}

void TransFuncMappingKey::setColorL(const Imath::V4i& color) {
    colorL_ =  Sww::Core::V4uc(color);
    if (!split_)
        colorR_ =  Sww::Core::V4uc(color);
}

const  Sww::Core::V4uc& TransFuncMappingKey::getColorL() const {
    return colorL_;
}

void TransFuncMappingKey::setColorR(const  Sww::Core::V4uc& color) {
    colorR_ = color;
    if (!split_)
        colorL_ = color;
}

void TransFuncMappingKey::setColorR(const Imath::V4i& color) {
    colorR_ =  Sww::Core::V4uc(color);
    if (!split_)
        colorL_ =  Sww::Core::V4uc(color);
}

const  Sww::Core::V4uc& TransFuncMappingKey::getColorR() const {
    return colorR_;
}

void TransFuncMappingKey::setAlphaR(float a) {
    colorR_.w = static_cast<uint8_t>(a*255.f);
    if (!split_)
        colorL_.w = static_cast<uint8_t>(a*255.f);
}

void TransFuncMappingKey::setAlphaL(float a) {
    colorL_.w = static_cast<uint8_t>(a*255.f);
    if (!split_)
        colorR_.w = static_cast<uint8_t>(a*255.f);
}

float TransFuncMappingKey::getAlphaR() const {
    return colorR_.w / 255.f;
}

float TransFuncMappingKey::getAlphaL() const {
    return colorL_.w / 255.f;
}

bool TransFuncMappingKey::isSplit() const {
    return split_;
}

void TransFuncMappingKey::setSplit(bool split, bool useLeft) {
    if (split_ == split)
        return;
    if (!split) {
        //join colors
        if (useLeft)
            colorR_ = colorL_;
        else
            colorL_ = colorR_;
    }
    split_ = split;
}

float TransFuncMappingKey::getIntensity() const {
    return intensity_;
}

void TransFuncMappingKey::setIntensity(float i) {
    intensity_ = i;
}

const std::string& TransFuncMappingKey::getTexNameLeft() const {
    return texNameLeft_;
}

const std::string& TransFuncMappingKey::getTexNameRight() const {
    return texNameRight_;
}

void TransFuncMappingKey::setTexNameLeft(std::string& name) {
    texNameLeft_ = name;
}

void TransFuncMappingKey::setTexNameRight(std::string& name) {
    texNameRight_ = name;
}


TransFuncMappingKey* TransFuncMappingKey::clone() const {
    TransFuncMappingKey* k = new TransFuncMappingKey();
    k->colorL_ = colorL_;
    k->colorR_ = colorR_;
    k->intensity_ = intensity_;
    k->split_ = split_;
    return k;
}

void TransFuncMappingKey::printState()const
{
    std::cout << "TransFuncMappingKey["
              << "intensity:" << intensity_
              << " colorL:" << colorL_
              << " colorR:" << colorR_
              << "]" << std::endl;
}

QString TransFuncMappingKey::valueToString() const
{
    QStringList valueList;
    valueList << QString::number(intensity_);
    valueList << QString::number((int)split_);
    valueList << QString::number(colorL_.x)
              <<  QString::number(colorL_.y)
               <<  QString::number(colorL_.z)
                <<  QString::number(colorL_.w);
    valueList << QString::number(colorR_.x)
              <<  QString::number(colorR_.y)
               <<  QString::number(colorR_.z)
                << QString::number(colorR_.w);

    return valueList.join(",");
}

void TransFuncMappingKey::setValueString(const QString &str){
    QStringList valueList = str.split(",");
    intensity_ = valueList[0].toFloat();
    split_ = valueList[1].toInt();

    colorL_ = Sww::Core::V4uc(valueList[2].toInt(),valueList[3].toInt(),valueList[4].toInt(),valueList[5].toInt());
    colorR_ = Sww::Core::V4uc(valueList[6].toInt(),valueList[7].toInt(),valueList[8].toInt(),valueList[9].toInt());
}


} // namespace voreen
