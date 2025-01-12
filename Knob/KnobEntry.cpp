#include "Knob/KnobEntry.h"

#include "Knob/KnobMaker.h"
#include "Knob/KnobTypeDesc.h"
#include "Knob/KnobFloat.h"
#include "Knob/KnobBool.h"
#include "Knob/KnobInt.h"
#include "Knob/KnobVector2i.h"
#include "Knob/KnobVector3i.h"
#include "Knob/KnobVector2f.h"
#include "Knob/KnobVector3f.h"
#include "Knob/KnobVector4f.h"
#include "Knob/KnobMatrix33f.h"
#include "Knob/KnobMatrix44f.h"
#include "Knob/KnobExecButton.h"
#include "Knob/KnobFile.h"
#include "Knob/KnobEnum.h"
#include "Knob/KnobFloatSlider.h"
#include "Knob/KnobIntSlider.h"
#include "Knob/KnobMultiFile.h"
#include "Knob/KnobMultiLineEdit.h"
#include "Knob/KnobFloatRange.h"
#include "Knob/KnobLineEdit.h"
#include "Knob/KnobEnumString.h"
#include "Knob/KnobTF.h"

#include "Geom/PlugTypeGeom.h"

namespace SwwKnob {

bool SwwKnobEntry(){
    SWW_CHECK_BOOL(KnobMaker::registerKnob(Sww::Core::FLOAT,KnobFloat::creator,KnobTypeDesc::FLOAT::SPIN_BOX,"fKnob"));
    SWW_CHECK_BOOL(KnobMaker::registerKnob(Sww::Core::FLOAT,KnobFloatSlider::creator,KnobTypeDesc::FLOAT::SLIDER,"fSlider"));
    SWW_CHECK_BOOL(KnobMaker::registerKnob(Sww::Core::BOOL,KnobBool::creator,0,"bKnob"));
    SWW_CHECK_BOOL(KnobMaker::registerKnob(Sww::Core::INT,KnobInt::creator,KnobTypeDesc::INTEGER::SPIN_BOX,"iKnob"));
    SWW_CHECK_BOOL(KnobMaker::registerKnob(Sww::Core::INT,KnobEnum::creator,KnobTypeDesc::INTEGER::COMBO_BOX,"iComboBox"));
    SWW_CHECK_BOOL(KnobMaker::registerKnob(Sww::Core::INT,KnobIntSlider::creator,KnobTypeDesc::INTEGER::SLIDER,"iSlider"));
    SWW_CHECK_BOOL(KnobMaker::registerKnob(Sww::Core::V2I,KnobVector2i::creator,0,"v2iKnob"));
    SWW_CHECK_BOOL(KnobMaker::registerKnob(Sww::Core::V3I,KnobVector3i::creator,0,"v3iKnob"));
    SWW_CHECK_BOOL(KnobMaker::registerKnob(Sww::Core::V2F,KnobVector2f::creator,KnobTypeDesc::V2F::SPIN_BOX,"v2fKnob"));
    SWW_CHECK_BOOL(KnobMaker::registerKnob(Sww::Core::V2F,KnobFloatRange::creator,KnobTypeDesc::V2F::RANGE_SLIDER,"fRangeKnob"));
    SWW_CHECK_BOOL(KnobMaker::registerKnob(Sww::Core::V3F,KnobVector3f::creator,0,"v3fKnob"));
    SWW_CHECK_BOOL(KnobMaker::registerKnob(Sww::Core::V4F,KnobVector4f::creator,0,"v4fKnob"));
    SWW_CHECK_BOOL(KnobMaker::registerKnob(Sww::Core::M33F,KnobMatrix33f::creator,0,"m33fKnob"));
    SWW_CHECK_BOOL(KnobMaker::registerKnob(Sww::Core::M44F,KnobMatrix44f::creator,0,"m44fKnob"));
    SWW_CHECK_BOOL(KnobMaker::registerKnob(Sww::Core::_NULL,KnobExecButton::creator,0,"execButton"));
    SWW_CHECK_BOOL(KnobMaker::registerKnob(Sww::Core::STRING,KnobFile::creator,KnobTypeDesc::STRING::INPUT_IMAGE,"fileKnob"));
    SWW_CHECK_BOOL(KnobMaker::registerKnob(Sww::Core::STRING_ARRAY,KnobMultiFile::creator,KnobTypeDesc::STRING_ARRAY::INPUT_FILES,"multiFileKnob"));
    SWW_CHECK_BOOL(KnobMaker::registerKnob(Sww::Core::STRING_ARRAY,KnobMultiLineEdit::creator,KnobTypeDesc::STRING_ARRAY::MULTI_LINE_EDIT,"multiLineEditKnob"));
    SWW_CHECK_BOOL(KnobMaker::registerKnob(Sww::Core::STRING,KnobLineEdit::creator,KnobTypeDesc::STRING::LINE_EDIT,"lineEdit"));
    SWW_CHECK_BOOL(KnobMaker::registerKnob(Sww::Core::STRING,KnobEnumString::creator,KnobTypeDesc::STRING::COMBO_BOX,"stringComboBox"));
    SWW_CHECK_BOOL(KnobMaker::registerKnob(SwwGeom::TRANSFERFUNCTION_DESC,KnobTF::creator,0,"tfKnob"));
    return true;
}

}
