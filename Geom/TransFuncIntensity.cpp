#include "Geom/TransFuncIntensity.h"
#include "Geom/TransFuncMappingKey.h"

#include "Core/SwwAssert.h"
#include <QStringList>

namespace SwwGeom {

const std::string TransFuncIntensity::loggerCat_("voreen.TransFuncIntensity");

TransFuncIntensity::TransFuncIntensity(int width)
    : lowerThreshold_(0.f)
    , upperThreshold_(1.f)
    , domain_(0.0, 1.0f)
    , TransFunc(width)
{
    createStdFunc();

    //SWW_DEBUG_VALUE(getNumKeys());
}

//TransFuncIntensity::TransFuncIntensity(const TransFuncIntensity& tf)
//    : TransFunc(tf.dimensions_.x, tf.dimensions_.y, tf.dimensions_.z,
//                tf.format_, tf.dataType_)
//{
//    updateFrom(tf);
//}

TransFuncIntensity::~TransFuncIntensity() {
    for (size_t i = 0; i < keys_.size(); ++i)
        delete keys_[i];
}

bool TransFuncIntensity::operator==(const TransFuncIntensity& tf) const {
    if (lowerThreshold_ != tf.lowerThreshold_)
        return false;
    if (upperThreshold_ != tf.upperThreshold_)
        return false;
    if (domain_ != tf.getDomain(0))
        return false;
    if (dimensions_ != tf.dimensions_)
        return false;

    if (keys_.size() != tf.keys_.size())
        return false;
    for (size_t i = 0; i < keys_.size(); ++i) {
        if (*(keys_[i]) != *(tf.keys_[i]))
            return false;
    }

    return true;
}

bool TransFuncIntensity::operator!=(const TransFuncIntensity& tf) const {
    return !(*this == tf);
}

void TransFuncIntensity::createStdFunc() {
    clearKeys();
    keys_.push_back(new TransFuncMappingKey(0.f,  Sww::Core::V4uc(0, 0, 0, 0)));
    keys_.push_back(new TransFuncMappingKey(1.f,  Sww::Core::V4uc(255)));

    // textureInvalid_ = true;
}

//Imath::V4f TransFuncIntensity::getMeanValue(float segStart, float segEnd) const {
//    ivec4 result(0);
//    float width = static_cast<float>(tex_->getWidth());
//    for (int i = static_cast<int>(segStart*width); i < segEnd*width; ++i)
//        result += ivec4(tex_->texel< Sww::Core::V4uc>(i));

//    return static_cast<Imath::V4f>(result)/(segEnd*width-segStart*width);
//}

Sww::Core::V4uc TransFuncIntensity::getMappingForValue(float value) const {
    // If there are no keys, any further calculation is meaningless
    if (keys_.empty())
        return  Sww::Core::V4uc(0, 0, 0, 0);

    // Restrict value to [0,1]
    value = (value < 0.f) ? 0.f : value;
    value = (value > 1.f) ? 1.f : value;

    // iterate through all keys until we get to the correct position
    std::vector<TransFuncMappingKey*>::const_iterator keyIterator = keys_.begin();

    while ((keyIterator != keys_.end()) && (value > (*keyIterator)->getIntensity()))
        keyIterator++;

    if (keyIterator == keys_.begin())
        return keys_[0]->getColorL();
    else if (keyIterator == keys_.end())
        return (*(keyIterator-1))->getColorR();
    else{
        // calculate the value weighted by the destination to the next left and right key
        TransFuncMappingKey* leftKey = *(keyIterator-1);
        TransFuncMappingKey* rightKey = *keyIterator;
        float fraction = (value - leftKey->getIntensity()) / (rightKey->getIntensity() - leftKey->getIntensity());
        Sww::Core::V4uc leftDest = leftKey->getColorR();
        Sww::Core::V4uc rightDest = rightKey->getColorL();
        Sww::Core::V4uc result = leftDest;
        result.x += static_cast<uint8_t>((rightDest.x - leftDest.x) * fraction);
        result.y += static_cast<uint8_t>((rightDest.y - leftDest.y) * fraction);
        result.z += static_cast<uint8_t>((rightDest.z - leftDest.z) * fraction);
        result.w += static_cast<uint8_t>((rightDest.w - leftDest.w) * fraction);
        return result;
    }
}

//void TransFuncIntensity::updateTexture() {
//    //SWW_DEBUG("update texture");
//    SWW_CHECK_GL_CONTEXT;

////    if (!_tex || (Imath::V3i(_tex->width(),_tex->height(),_tex->depth()) != dimensions_))
////        createTex();
//    if (_mustResetTexture)createTex();

////    SWW_DEBUG_VALUE(_tex->width());
////    SWW_DEBUG_VALUE(_tex->height());

//    Q_ASSERT_X(_tex,"updateTexture()","No texture",);

//    int front_end = static_cast<int>(lowerThreshold_*dimensions_.x);
//    int back_start = static_cast<int>(upperThreshold_*dimensions_.x);

//    if(_tex->type() != GL_UNSIGNED_BYTE)
//    {
//        SWW_ERR("TransFuncIntensity support only uchar.");
//        return;
//    }

//    unsigned char* pixels = (unsigned char*)_tex->getData();

//    if(!pixels)
//    {
//        SWW_ERR("TransFuncIntensity data is null.");
//        return;
//    }

//    for (int x = 0; x < front_end; ++x){
//        pixels[x * 4 + 0] = 0;
//        pixels[x * 4 + 1] = 0;
//        pixels[x * 4 + 2] = 0;
//        pixels[x * 4 + 3] = 0;
//    }

//    for (int x = front_end; x < back_start; ++x){
//        Sww::Core::V4uc tmp =  Sww::Core::V4uc(getMappingForValue(static_cast<float>(x) / dimensions_.x));
//        pixels[x * 4 + 0] = tmp.x;
//        pixels[x * 4 + 1] = tmp.y;
//        pixels[x * 4 + 2] = tmp.z;
//        pixels[x * 4 + 3] = tmp.w;
//    }

//    for (int x = back_start; x < dimensions_.x; ++x){
//        pixels[x * 4 + 0] = 0;
//        pixels[x * 4 + 1] = 0;
//        pixels[x * 4 + 2] = 0;
//        pixels[x * 4 + 3] = 0;
//    }

//    _tex->uploadInternalData();
//    //LGL_ERROR;

//    textureInvalid_ = false;
//    _mustResetTexture = false;

//    //SWW_DEBUG("update tex  tf intensity.");
//    //    for(int i = 0;i < 100;i++){
//    //        SWW_DEBUG_VALUE((int)pixels[i]);
//    //    }
//}

void TransFuncIntensity::setThresholds(float lower, float upper) {
    lowerThreshold_ = lower;
    upperThreshold_ = upper;
    //textureInvalid_ = true;
}

void TransFuncIntensity::setThresholds(const Imath::V2f& thresholds) {
    setThresholds(thresholds.x, thresholds.y);
}

Imath::V2f TransFuncIntensity::getThresholds() const {
    return Imath::V2f(lowerThreshold_, upperThreshold_);
}

int TransFuncIntensity::getNumKeys() const {
    return static_cast<int>(keys_.size());
}

TransFuncMappingKey* TransFuncIntensity::getKey(int i) const {
    return keys_.at(i);
}

const std::vector<TransFuncMappingKey*> TransFuncIntensity::getKeys() const {
    return keys_;
}

void TransFuncIntensity::setKeys(std::vector<TransFuncMappingKey*> keys) {
    keys_ = keys;
    //textureInvalid_ = true;
}

void TransFuncIntensity::addKey(TransFuncMappingKey* key) {
    // insert key at appropriate location
    if (keys_.empty()) {
        keys_.push_back(key);
        return;
    }
    std::vector<TransFuncMappingKey *>::iterator keyIterator = keys_.begin();
    // Fast-forward to the correct position
    while ((keyIterator != keys_.end()) && (key->getIntensity() > (*keyIterator)->getIntensity()))
        keyIterator++;
    keys_.insert(keyIterator, key);

    //textureInvalid_ = true;
}

bool sortFunction(TransFuncMappingKey* a, TransFuncMappingKey* b) {
    return a->getIntensity() < b->getIntensity();
}

void TransFuncIntensity::updateKey(TransFuncMappingKey* /*key*/) {
    std::sort(keys_.begin(), keys_.end(), sortFunction);

    //textureInvalid_ = true;
}

void TransFuncIntensity::removeKey(TransFuncMappingKey* key) {
    std::vector<TransFuncMappingKey *>::iterator keyIterator = find(keys_.begin(), keys_.end(), key);
    if (keyIterator != keys_.end())
        keys_.erase(keyIterator);
    delete key;

    //textureInvalid_ = true;
}

void TransFuncIntensity::clearKeys() {
    std::vector<TransFuncMappingKey *>::iterator keyIterator = keys_.begin();
    // First delete all the referenced objects in the heap
    while (keyIterator != keys_.end()) {
        delete (*keyIterator);
        ++keyIterator;
    }
    // then delete the entries in the vector
    keys_.clear();

    //textureInvalid_ = true;
}

bool TransFuncIntensity::isEmpty() const {
    return keys_.empty();
}

void TransFuncIntensity::updateFrom(const TransFuncIntensity& tf) {
    lowerThreshold_ = tf.lowerThreshold_;
    upperThreshold_ = tf.upperThreshold_;

    domain_ = tf.domain_;

    clearKeys();
    for (size_t i = 0; i < tf.keys_.size(); ++i) {
        TransFuncMappingKey* k = new TransFuncMappingKey(*(tf.keys_.at(i)));
        addKey(k);
    }
}


void TransFuncIntensity::generateKeys(unsigned char* data) {
    // Storage for the old values
    int oldDeltaRed;
    int oldDeltaGreen;
    int oldDeltaBlue;
    int oldDeltaAlpha;

    // Storage for the new values
    int newDeltaRed;
    int newDeltaGreen;
    int newDeltaBlue;
    int newDeltaAlpha;

    // We want at least 2 values in the data array
    if (dimensions_.x < 2)
        return;

    clearKeys();

    addKey(new TransFuncMappingKey(0.f,
                                   Sww::Core::V4uc(data[0], data[1], data[2], data[3])));
    addKey(new TransFuncMappingKey(1.f,
                                   Sww::Core::V4uc(data[4*(dimensions_.x-1)+0], data[4*(dimensions_.x-1)+1], data[4*(dimensions_.x-1)+2], data[4*(dimensions_.x-1)+3])));

    // Calculate the starting point
    newDeltaRed   = data[4*1 + 0] - data[4*0 + 0];
    newDeltaGreen = data[4*1 + 1] - data[4*0 + 1];
    newDeltaBlue  = data[4*1 + 2] - data[4*0 + 2];
    newDeltaAlpha = data[4*1 + 3] - data[4*0 + 3];

    // The main loop. We start at 2 because the value for 1 already has been calculated.
    for (int iter = 2; iter < dimensions_.x; ++iter) {
        // Backup the old values and generate the new ones.
        oldDeltaRed = newDeltaRed;
        oldDeltaGreen = newDeltaGreen;
        oldDeltaBlue = newDeltaBlue;
        oldDeltaAlpha = newDeltaAlpha;

        newDeltaRed   = data[4*iter + 0] - data[4*(iter-1) + 0];
        newDeltaGreen = data[4*iter + 1] - data[4*(iter-1) + 1];
        newDeltaBlue  = data[4*iter + 2] - data[4*(iter-1) + 2];
        newDeltaAlpha = data[4*iter + 3] - data[4*(iter-1) + 3];

        // Has the difference quotient changed in any color channel?
        bool differenceQuotientChanged = (
                    (oldDeltaRed   != newDeltaRed)   ||
                    (oldDeltaGreen != newDeltaGreen) ||
                    (oldDeltaBlue  != newDeltaBlue)  ||
                    (oldDeltaAlpha != newDeltaAlpha));

        // Is the difference quotient different from zero in any channel?
        bool differenceQuotientNotZero = (
                    (newDeltaRed   != 0) ||
                    (newDeltaGreen != 0) ||
                    (newDeltaBlue  != 0) ||
                    (newDeltaAlpha != 0));

        // Has the difference quotient tilted in all channel's?
        // Mind the & instead of |
        bool differenceQuotientTilted = (
                    (oldDeltaRed   == -newDeltaRed)   &&
                    (oldDeltaGreen == -newDeltaGreen) &&
                    (oldDeltaBlue  == -newDeltaBlue)  &&
                    (oldDeltaAlpha == -newDeltaAlpha));

        if (differenceQuotientChanged) {
            if (differenceQuotientNotZero) {
                // We want to put a splitted key here (see ii above)
                TransFuncMappingKey* newkey = new TransFuncMappingKey(iter/static_cast<float>(dimensions_.x-1) ,
                                                                      Sww::Core::V4uc( data[4*(iter-1) + 0], data[4*(iter-1) + 1], data[4*(iter-1) + 2], data[4*(iter-1) + 3] )
                        );
                newkey->setSplit(true);
                newkey->setColorR( Sww::Core::V4uc(data[4*iter + 0], data[4*iter + 1], data[4*iter + 2], data[4*iter + 3]));
                addKey(newkey);
            }
            else if (differenceQuotientTilted) {
                // We want a single key at i-1 here (see iii above)
                addKey(
                            new TransFuncMappingKey((iter - 1)/static_cast<float>(dimensions_.x-1),
                                                    Sww::Core::V4uc(data[4*(iter-1) + 0], data[4*(iter-1) + 1], data[4*(iter-1) + 2], data[4*(iter-1) + 3])
                        ));
            }
            else {
                // Just add a key
                addKey(
                            new TransFuncMappingKey(iter/static_cast<float>(dimensions_.x-1),
                                                    Sww::Core::V4uc(data[4*iter + 0], data[4*iter + 1], data[4*iter + 2], data[4*iter + 3])));
            }
        }
    }
}

// --------------------------------------------------------------------------------
// some stream helper functions


//inline int readInt(std::ifstream& stream) {
//    char* buffer = new char[4];

//    // The bytes are inserted backwards because of a discrepancy of
//    // most significant bit <-> least significant bit between Java and C++
//    for (int i = 3; i >= 0; --i)
//        stream >> buffer[i];

//    int ret = *(reinterpret_cast<int*>(buffer));
//    delete[] buffer;
//    return ret;
//}

///**
// * \sa TransFuncIntensity::readInt(std::ifstream& stream)
// */
//inline short readShort(std::ifstream& stream) {
//    char* buffer = new char[2];

//    for (int i = 1; i >= 0; --i)
//        stream >> buffer[i];

//    short ret = *(reinterpret_cast<short*>(buffer));
//    delete[] buffer;
//    return ret;
//}

///**
// * \sa TransFuncIntensity::readInt(std::ifstream& stream)
// */
//inline double readDouble(std::ifstream& stream) {
//    char* buffer = new char[8];

//    for (int i = 7; i >= 0; --i)
//        stream >> buffer[i];

//    double ret = *(reinterpret_cast<double*>(buffer));
//    delete[] buffer;
//    return ret;
//}

TransFuncIntensity* TransFuncIntensity::clone() const {
    TransFuncIntensity* func = new TransFuncIntensity();

    func->dimensions_.x = dimensions_.x;
    func->dimensions_.y = dimensions_.y;
    func->dimensions_.z = dimensions_.z;
    //    func->format_ = format_;
    //    func->dataType_ = dataType_;
    //func->filter_ = filter_;

    func->lowerThreshold_ = lowerThreshold_;
    func->upperThreshold_ = upperThreshold_;

    func->domain_ = domain_;

    func->keys_.clear();
    std::vector<TransFuncMappingKey*>::const_iterator it;
    for(it = keys_.begin(); it!=keys_.end(); it++) {
        func->keys_.push_back((*it)->clone());
    }

    //func->textureInvalid_ = true;

    return func;
}

Imath::V2f TransFuncIntensity::getDomain(int dimension) const {
    SWW_DEBUG_VALUE(dimension);
    Q_ASSERT_X(dimension == 0,"getDomain","Only one dimension!");

    if(dimension == 0)
        return domain_;
    else
        return Imath::V2f(0.0f);
}

void TransFuncIntensity::setDomain(Imath::V2f domain, int dimension) {
    Q_ASSERT_X(dimension == 0,"getDomain","Only one dimension!");


    if(dimension == 0)
        domain_ = domain;
}

void TransFuncIntensity::printKeyStates() const
{
    std::cout << "mapping key state" << std::endl;
    std::vector<TransFuncMappingKey*>::const_iterator it;
    for(it = keys_.begin(); it!=keys_.end(); ++it) {
        TransFuncMappingKey* key = *it;
        key->printState();
    }
}

QString TransFuncIntensity::valueToString()const{
    QStringList valueList;
    std::vector<TransFuncMappingKey*>::const_iterator it;

    int numKey = 0;
    for(it = keys_.begin(); it!=keys_.end(); ++it) {
        TransFuncMappingKey* key = *it;
        valueList << key->valueToString();
    }
    SWW_DEBUG_VALUE(numKey);


    QString result = valueList.join(",");
    QString header = QString::number(dimensions_.x);
    QString mode;

    //    switch (_texTarget) {
    //    case GL_TEXTURE_1D:
    //        mode = "0";
    //        break;
    //    case GL_TEXTURE_2D:
    //        mode = "1";
    //        break;
    //    default:
    //        SWW_ERR("invalid tex target.");
    //        break;
    //    }

    mode = "0";

    header = header + "," + mode;
    result = header + "," + result;
    return result;
}

//void TransFuncIntensity::setWidth(int width){
//    dimensions_ = Imath::V3f(width,1,1);
//    textureInvalid_ = true;
//    _mustResetTexture = true;
//}

void TransFuncIntensity::setValueString(const QString &str){

    clearKeys();

    QStringList valueList = str.split(",");
    int width = valueList.takeFirst().toInt();
    resize(width);
    int mode = valueList.takeFirst().toInt();

    //    switch (mode) {
    //    case 0:
    //        setTexTarget(GL_TEXTURE_1D);
    //        break;
    //    case 1:
    //        setTexTarget(GL_TEXTURE_2D);
    //        break;
    //    default:
    //        SWW_ERR("invalid tex target.");
    //        break;
    //    }

    const int numTfAttr = 10;
    Q_ASSERT_X(valueList.size() % numTfAttr == 0,"TransFuncIntensity","numTfAttr must be 10.");

    int numForEarch = valueList.size() / numTfAttr;

    for(int i = 0;i<numForEarch;i++){
        QStringList value;
        for(int j = 0;j<numTfAttr;j++){
            value << valueList.takeFirst();
        }
        TransFuncMappingKey* key = new TransFuncMappingKey();
        key->setValueString(value.join(","));
        addKey(key);
    }

    //    Q_FOREACH(QString valueStr , valueList){
    //        QStringList valuePerTf;
    //        for(int )
    //    }

}


bool TransFuncIntensity::genArray(Imath::V4f *&pixels, int resolition){

    const int numChannels = 4;

    Imath::V2f threshold = getThresholds();
    int front_end = static_cast<int>(threshold.x * resolition);
    int back_start = static_cast<int>(threshold.y * resolition);

    //T* pixels = new T[resolition * 4];

    for (int x = 0; x < front_end; ++x){

        //        pixels[x * numChannels + 0] = 0;
        //        pixels[x * numChannels + 1] = 0;
        //        pixels[x * numChannels + 2] = 0;
        //        pixels[x * numChannels + 3] = 0;
        Imath::V4f& pixel = pixels[x];
        pixel.x = 0;
        pixel.y = 0;
        pixel.z = 0;
        pixel.w = 0;
    }


    for (int x = front_end; x < back_start; ++x){
        Sww::Core::V4uc tmp =  Sww::Core::V4uc(getMappingForValue(static_cast<float>(x) / resolition));
        //        pixels[x * numChannels + 0] = static_cast<float>(tmp.x) / 255.f;
        //        pixels[x * numChannels + 1] = static_cast<float>(tmp.y) / 255.f;
        //        pixels[x * numChannels + 2] = static_cast<float>(tmp.z) / 255.f;
        //        pixels[x * numChannels + 3] = static_cast<float>(tmp.w) / 255.f;
        Imath::V4f& pixel = pixels[x];
        pixel.x = static_cast<float>(tmp.x) / 255.f;
        pixel.y = static_cast<float>(tmp.y) / 255.f;
        pixel.z = static_cast<float>(tmp.z) / 255.f;
        pixel.w = static_cast<float>(tmp.w) / 255.f;
    }

    //    for (int x = front_end; x < back_start; ++x){
    //        Sww::Core::V4uc tmp =  Sww::Core::V4uc(getMappingForValue(static_cast<float>(x) / resolition));
    //        pixels[x * numChannels + 0] = tmp.x;
    //        pixels[x * numChannels + 1] = tmp.y;
    //        pixels[x * numChannels + 2] = tmp.z;
    //        pixels[x * numChannels + 3] = tmp.w;
    //    }


    for (int x = back_start; x < resolition; ++x){
        //        pixels[x * numChannels + 0] = 0;
        //        pixels[x * numChannels + 1] = 0;
        //        pixels[x * numChannels + 2] = 0;
        //        pixels[x * numChannels + 3] = 0;
        Imath::V4f& pixel = pixels[x];
        pixel.x = 0;
        pixel.y = 0;
        pixel.z = 0;
        pixel.w = 0;
    }

    return true;
}

} // ns
