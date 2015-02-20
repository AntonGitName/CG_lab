static const int MAX_MATRICES = 32;
float4x4 u_view_projection_matrix : VIEWPROJECTION;
float4x4 u_bones_matrix[MAX_MATRICES] : WORLDMATRIXARRAY;

texture ModelTexture;
sampler2D textureSampler = sampler_state {
    Texture = (ModelTexture);
    MinFilter = Linear;
    MagFilter = Linear;
    AddressU = Clamp;
    AddressV = Clamp;
};

struct vs_out_t
{
    float4 pos : POSITION;
    float4 color : COLOR0;
    float2 tex_coord : TEXCOORD0;
};

struct vs_in_t
{
    float3 pos : POSITION;
    float4 color : COLOR0;
    float2 tex_coord : TEXCOORD0;
    float2 blend_coef : TEXCOORD1;
    int2 blend_ind : TEXCOORD2;
};

vs_out_t skinning_VS(vs_in_t vs_in)
{
    vs_out_t vs_out = (vs_out_t)0;
    vs_out.pos = mul(float4(vs_in.pos, 1), mul(
        vs_in.blend_coef.x * u_bones_matrix[vs_in.blend_ind.x] +
        vs_in.blend_coef.y * u_bones_matrix[vs_in.blend_ind.y],
        u_view_projection_matrix));

    vs_out.color = vs_in.color;
    vs_out.tex_coord = vs_in.tex_coord;
    return vs_out;
}


float4 skinning_PS( vs_out_t fs_in ) : COLOR0
{
    float4 textureColor = tex2D(textureSampler, fs_in.tex_coord);
    textureColor.a = 1;
    return textureColor * fs_in.color;
}

technique skinning_technique
{
    pass P0
    {
        Lighting = false;
        VertexShader = compile vs_2_0 skinning_VS();
        PixelShader  = compile ps_2_0 skinning_PS();
    }
}

