#ifndef CONCEPTS_H
#define CONCEPTS_H

// ------ Project wide settings ---------------------
#include "Config.h"

// ------ STL ---------------------------------------
#include <type_traits>

namespace Concepts
{
	template <typename T>
	concept BinarySerializable = std::is_trivially_copyable_v<T>;
}

#endif
