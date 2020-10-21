//---------------------------------------
// Global Vars
//---------------------------------------

float4x4 gWorldViewProj : WorldViewProjection;
float3x3 gWorld : World;
float4x4 gViewInverse : ViewInverse;

Texture2D gDiffuseMap : DiffuseMap;
Texture2D gNormalMap : NormalMap;
Texture2D gSpecularMap : SpecularMap;
Texture2D gGlossMap : GlossMap;

float3 gLightDirection : LightDirection;
float3 gLightBiradiance : LightBiradianceValue;
float3 gCamPos : CameraPosition;
float gShininess : Shininess;
float3 gColor : RawColor;

SamplerState gSampler : Sampler;
BlendState gBlendState : Blendstate;

//---------------------------------------
// Global States
//---------------------------------------

RasterizerState gRasterizerState
{
	FrontCounterClockwise = false;
};

RasterizerState gRS_PC
{
	CullMode = none;
	FrontCounterClockwise = true;
};

BlendState gBS
{
	
};

BlendState gBS_PC
{
	BlendEnable[0] = true;
	SrcBlend = src_alpha;
	DestBlend = inv_src_alpha;
	BlendOp = add;
	SrcBlendAlpha = zero;
	DestBlendAlpha = zero;
	BlendOpAlpha = add;
	RenderTargetWriteMask[0] = 0x0F;
};

DepthStencilState gDSS
{
	
};

DepthStencilState gDSS_PC
{
	DepthEnable = true;
	DepthWriteMask = zero;
	DepthFunc = less;
	StencilEnable = false;

	StencilReadMask = 0x0F;
	StencilWriteMask = 0x0F;

	FrontFaceStencilFunc = always;
	BackFaceStencilFunc = always;

	FrontFaceStencilDepthFail = keep;
	BackFaceStencilDepthFail = keep;

	FrontFaceStencilPass = keep;
	BackFaceStencilPass = keep;

	FrontFaceStencilFail = keep;
	BackFaceStencilFail = keep;
};

//---------------------------------------
// Shading Functions:
//---------------------------------------

float3 LambertDiffuseFunc(float3 normal, float3 lightDir, float3 biradianceValue)
{
	float dotResult = dot(-normal, lightDir);

	return biradianceValue * dotResult;
}

float ObservedArea(float3 normal, float3 lightDir)
{
	return dot(-normal, lightDir);
}

float3 PhongFunc(float specular, float phExp, float3 lightDir, float3 toView, float3 normal)
{
	float3 reflectVector = lightDir + 2 * dot(-normal, lightDir) * normal;

	float3 result = pow(dot(reflectVector, toView), phExp);

	return result;
}

float3 GetNormalFromSample(float3 sampleValue, float3 planeNormal, float3 tangent)
{
	// [0 - 1] --> [-1 - 1]
	float3 finalNormal = { 2 * sampleValue.r - 1, 2 * sampleValue.g - 1, 2 * sampleValue.b - 1 };

	float3 binormal = cross(planeNormal, tangent);
	float3x3 tangentSpaceAxis = 
	{
		tangent.x, tangent.y, tangent.z,
		binormal.x, binormal.y, binormal.z,
		planeNormal.x, planeNormal.y, planeNormal.z
	};

	float3 result = mul(finalNormal, tangentSpaceAxis);

	return result;
}

float GetAverageValueFromSample(float3 sampleValue)
{
	float result = (sampleValue.r + sampleValue.g + sampleValue.b) / 3;

	return result;
}

//-------------------------------------
// I/O Structs
//-------------------------------------

struct VS_INPUT
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float2 UV : TEXCOORD0;
	float3 Color : COLOR;
	float3 WorldPosition : WORLDPOSITION;
};

struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
	float2 UV : TEXCOORD0;
	float3 Color : COLOR;
	float3 WorldPosition : WORLDPOSITION;
};


//---------------------------------------
// Vertex Shader (AKA projection)
//---------------------------------------

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	// These get affected by matrices:
	output.Position = mul(float4(input.Position, 1.f), gWorldViewProj);
	output.Normal = mul(input.Normal, gWorld);
	output.Tangent = mul(input.Tangent, gWorld);
	output.WorldPosition = mul(input.Position, gWorld);

	// these stay the same:
	output.Color = input.Color;
	output.UV = input.UV;
	
	return output;
}

//---------------------------------------
// Pixel Shaders
//---------------------------------------

float4 PS(VS_OUTPUT input) : SV_TARGET
{
	// Sample
	// gDiffuseMap.Sample(gSampler, input.UV);
	return float4(input.Color, 1.f);
}

float4 PS_LambertDiffuse(VS_OUTPUT input) : SV_TARGET
{
	float3 mappedNormal = GetNormalFromSample(gNormalMap.Sample(gSampler, input.UV).rgb, input.Normal, input.Tangent);
	
	float3 lambert = LambertDiffuseFunc(mappedNormal, gLightDirection.xyz, gLightBiradiance.rgb) * gDiffuseMap.Sample(gSampler, input.UV).rgb;

	return float4(lambert, 1.f);
}

float4 PS_Phong(VS_OUTPUT input) : SV_TARGET
{
	// Get Map values:
	float mappedSpecular = GetAverageValueFromSample(gSpecularMap.Sample(gSampler, input.UV).rgb);
	float gloss = gShininess * GetAverageValueFromSample(gGlossMap.Sample(gSampler, input.UV).rgb);
	float3 mappedNormal = GetNormalFromSample(gNormalMap.Sample(gSampler, input.UV).rgb, input.Normal, input.Tangent);

	// Shade:
	float3 lambertDiffuse = LambertDiffuseFunc(mappedNormal, gLightDirection.xyz, gLightBiradiance.xyz) * gDiffuseMap.Sample(gSampler, input.UV).rgb;

	float3 toView = gCamPos - input.WorldPosition;
	toView = normalize(toView);

	float3 phong = PhongFunc(mappedSpecular, gloss, gLightDirection, toView, mappedNormal);
	
	float3 result = phong + lambertDiffuse;

	return float4(result, 1.0f);
}

float4 PS_RawColor(VS_OUTPUT input) : SV_TARGET
{
	return float4(gColor, 1.f);
}

float4 PS_RawTexture(VS_OUTPUT input) : SV_TARGET
{
	return gDiffuseMap.Sample(gSampler, input.UV);
}

float4 PS_PartialCoverage(VS_OUTPUT input) : SV_TARGET
{
	return gDiffuseMap.Sample(gSampler, input.UV);
}

float4 PS_UI() : SV_Target
{
	return float4(1.f, 0.f, 1.f, 1.f);
}


//---------------------------------------
// Techniques
//---------------------------------------
technique11 DefaultTechnique
{
	pass P0
	{
		// These states get set in c++-side:
		//SetRasterizerState(gRasterizerState);
		//SetBlendState(gBS, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);

		SetDepthStencilState(gDSS, 0);
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}

technique11 LambertDiffuse
{
	pass P0
	{
		// These states get set in c++-side:
		//SetRasterizerState(gRasterizerState);
		//SetBlendState(gBS, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);

		SetDepthStencilState(gDSS, 0);
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_LambertDiffuse()));
	}
}

technique11 Phong
{
	pass P0
	{
		// These states get set in c++-side:
		//SetRasterizerState(gRasterizerState);
		//SetBlendState(gBS, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);

		SetDepthStencilState(gDSS, 0);
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_Phong()));
	}
}

technique11 RawColor
{
	pass P0
	{
		// These states get set in c++-side:
		//SetRasterizerState(gRasterizerState);
		//SetBlendState(gBS, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);

		SetDepthStencilState(gDSS, 0);
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_RawColor()));
	}
}

technique11 RawTexture
{
	pass P0
	{
		// These states get set in c++-side:
		//SetRasterizerState(gRasterizerState);
		//SetBlendState(gBS, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);

		SetDepthStencilState(gDSS, 0);
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_RawTexture()));
	}
}

technique11 PartialCoverage
{
	pass P0
	{
		// These states get set in c++-side:
		//SetRasterizerState(gRS_PC);
		//SetBlendState(gBS_PC, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);

		SetDepthStencilState(gDSS_PC, 0);
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_RawTexture()));
	}
}

technique11 UITechnique
{
	pass P0
	{
		SetVertexShader(NULL);
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS_UI()));
	}
};

