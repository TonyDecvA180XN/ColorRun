#pragma once

#include "Obstacle.h"
#include "Pawn.h"

template <class TBaseType, class TDerivedType>
BOOL IsType(const TBaseType * const Destination, const TDerivedType * const Source)
{
	return dynamic_cast<const TDerivedType * const>(Destination) != nullptr;
}
