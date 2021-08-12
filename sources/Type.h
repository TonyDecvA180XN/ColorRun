#pragma once


using INT = int32_t;
using UINT = uint32_t;
using INT64 = int64_t;
using UINT64 = uint64_t;
using FLOAT = float;

using BOOL = INT;
constexpr auto TRUE = 1;
constexpr auto FALSE = 0;

using SIZE = UINT64;
using ID = UINT64;
using INDEX = UINT64;

enum class EMeshType
{
	Static,
	Skinned
};
