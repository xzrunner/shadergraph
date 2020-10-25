#pragma once

namespace shadergraph
{

class VarNames
{
public:
	struct PositionUniforms
	{
		static const char* cam_pos;
	};

	struct FragInputs
	{
		static const char* frag_pos;
	};

}; // VarNames

}