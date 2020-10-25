#pragma once

namespace shadergraph
{

enum class TypeQualifier
{
	Null,

	// Storage qualifiers
	Const,
	Input,
	Output,
	Attribute,

	// Layout qualifiers
	// Precision qualifiers

	// Memory qualifiers
	Volatile,

	// Invariance qualifiers
	// Precise qualifiers

	// Removed qualifiers
	Varying,
};

}