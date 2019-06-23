#pragma once
#include "framework.h"

static const std::string NOT_ASSIGNED_STR   = "N/A";
static const int NOT_ASSIGNED_ID            = -1;

enum class ShaderStage : uint
{
    Unknown, Global, VS, PS, GS, CS, VPS,
};

struct GlobalData
{
    Matrix wvp;
    Matrix camera_view;
    Matrix camera_proj;
    Matrix camera_view_proj;
    Matrix camera_view_proj_inverse;
    float camera_near;
    float camera_far;
    Vector2 resolution;
    Vector3 camera_position;
    float padding;
};

struct MaterialData
{
    Color4 albedo_color;
    Vector2 uv_tiling;
    Vector2 uv_offset;
    float normal_multipler;
    float padding[3];
};

struct TransformData
{
    Matrix model_world;
    Matrix wvp_current;
    Matrix wvp_previous;
};

struct AnimationData
{
    Vector2 texture_size;
    Vector2 sprite_offset;
    Vector2 sprite_size;
    float Padding[2];
};