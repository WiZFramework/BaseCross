
#include "SimpleInc.hlsli"


GeometryShaderInput main(VertexShaderInput input)
{
	//ジオメトリシェーダーに渡す変数
	//入力をそのまま出力する
	GeometryShaderInput vertexShaderOutput
		= (GeometryShaderInput)input;
	return vertexShaderOutput;
}
