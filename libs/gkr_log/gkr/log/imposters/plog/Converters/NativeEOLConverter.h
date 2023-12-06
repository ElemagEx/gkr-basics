#pragma once
#include <plog/Converters/UTF8Converter.h>
#include <plog/Util.h>

namespace plog
{
template<class NextConverter = UTF8Converter>
class NativeEOLConverter : public NextConverter {};
}
