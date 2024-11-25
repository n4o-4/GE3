#pragma once
#include "Vectors.h"
#include "ViewProjection.h"
#include <d3d12.h>
#include <memory>
#include <string>
#include <wrl.h>

class PrimitiveDrawer
{
public:
	// 線分の最大数
	static const UINT kMaxLineCount = 4096;
	// 線分の頂点数
	static const UINT kVertexCountLine = 2;
	// 線分のインデックス数
	static const UINT kIndexCountLine = 0;


};