

#include "SimpleInc.hlsli"

[maxvertexcount(9)]
void main(
	triangle GeometryShaderInput input[3],
	inout TriangleStream< PixelShaderInput > output
)
{
	for (uint count = 0; count < 3; count++){
		for (uint i = 0; i < 3; i++)
		{
			PixelShaderInput element;
			//頂点の位置を変換
			float4 pos = float4(input[i].pos.xyz, 1.0f);
			float4 Col;
			if (pos.y > 0){
				switch (count){
				case 0:
					Col = float4(1.0f, 0.0f, 0.0f, 0.0f);
					break;
				case 1:
					Col = float4(0.0f, 1.0f, 0.0f, 0.0f);
					break;
				case 2:
					Col = float4(0.0f, 0.0f, 1.0f, 0.0f);
					break;
				default:
					Col = float4(1.0f, 1.0f, 1.0f, 1.0f);
					break;
				}
			}
			else if (pos.y < 0){
				switch (count){
				case 0:
					Col = float4(0.0f, 1.0f, 0.0f, 0.0f);
					break;
				case 1:
					Col = float4(0.0f, 0.0f, 1.0f, 0.0f);
					break;
				case 2:
					Col = float4(1.0f, 0.0f, 0.0f, 0.0f);
					break;
				default:
					Col = float4(1.0f, 1.0f, 1.0f, 1.0f);
					break;
				}
			}
			else{
				Col = float4(1.0f, 1.0f, 1.0f, 1.0f);
			}
			//ワールド変換
			pos = mul(pos, World[count]);
			//ビュー変換
			pos = mul(pos, View);
			//射影変換
			pos = mul(pos, Projection);
			//ピクセルシェーダに渡す変数に設定
			element.pos = pos;
			//ライティング用に法線をワールド変換して設定
			element.norm = mul(input[i].norm, (float3x3)World[count]);
			//頂点色を設定
			element.color = Col;
			//頂点を追加
			output.Append(element);
		}
		//一つの三角形をストリームに送る
		output.RestartStrip();
	}
}
