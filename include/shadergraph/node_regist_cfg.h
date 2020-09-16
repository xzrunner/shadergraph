#ifndef EXE_FILEPATH
#error "You must define EXE_FILEPATH macro before include this file"
#endif

#undef NO_PARM_FILEPATH

//////////////////////////////////////////////////////////////////////////
// artistic
//////////////////////////////////////////////////////////////////////////

// adjustment

#define PARM_NODE_TYPE ChannelMixer
#define PARM_NODE_NAME channel_mixer
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Contrast
#define PARM_NODE_NAME contrast
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Hue
#define PARM_NODE_NAME hue
#include EXE_FILEPATH

#define PARM_NODE_TYPE InvertColors
#define PARM_NODE_NAME invert_colors
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE ReplaceColor
#define PARM_NODE_NAME replace_color
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Saturation
#define PARM_NODE_NAME saturation
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE WhiteBalance
#define PARM_NODE_NAME white_balance
#include EXE_FILEPATH

// blend

#define PARM_NODE_TYPE Blend
#define PARM_NODE_NAME blend
#include EXE_FILEPATH

// filter
#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SelectiveColor
#define PARM_NODE_NAME selective_color
#include EXE_FILEPATH

// mask

#define PARM_NODE_TYPE ChannelMask
#define PARM_NODE_NAME channel_mask
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE ColorMask
#define PARM_NODE_NAME color_mask
#include EXE_FILEPATH

// normal

#define PARM_NODE_TYPE NormalBlend
#define PARM_NODE_NAME normal_blend
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE NormalReconstructZ
#define PARM_NODE_NAME normal_reconstruct_z
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE NormalStrength
#define PARM_NODE_NAME normal_strength
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE NormalUnpack
#define PARM_NODE_NAME normal_unpack
#include EXE_FILEPATH

// utility

#define PARM_NODE_TYPE ColorspaceConversion
#define PARM_NODE_NAME colorspace_conversion
#include EXE_FILEPATH

//////////////////////////////////////////////////////////////////////////
// channel
//////////////////////////////////////////////////////////////////////////

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Combine
#define PARM_NODE_NAME combine
#include EXE_FILEPATH

#define PARM_NODE_TYPE Flip
#define PARM_NODE_NAME flip
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Split
#define PARM_NODE_NAME split
#include EXE_FILEPATH

#define PARM_NODE_TYPE Swizzle
#define PARM_NODE_NAME swizzle
#include EXE_FILEPATH

//////////////////////////////////////////////////////////////////////////
// input
//////////////////////////////////////////////////////////////////////////

// basic

#define PARM_NODE_TYPE Bool
#define PARM_NODE_NAME bool
#include EXE_FILEPATH

#define PARM_NODE_TYPE Constant
#define PARM_NODE_NAME constant
#include EXE_FILEPATH

#define PARM_NODE_TYPE Float
#define PARM_NODE_NAME float1
#include EXE_FILEPATH

#define PARM_NODE_TYPE Float2
#define PARM_NODE_NAME float2
#include EXE_FILEPATH

#define PARM_NODE_TYPE Float3
#define PARM_NODE_NAME float3
#include EXE_FILEPATH

#define PARM_NODE_TYPE Float4
#define PARM_NODE_NAME float4
#include EXE_FILEPATH

#define PARM_NODE_TYPE Int
#define PARM_NODE_NAME int1
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Time
#define PARM_NODE_NAME time
#include EXE_FILEPATH

// geometry

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE TexCoords
#define PARM_NODE_NAME tex_coords
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE UV
#define PARM_NODE_NAME uv
#include EXE_FILEPATH

// gradient

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Blackbody
#define PARM_NODE_NAME blackbody
#include EXE_FILEPATH

// texture

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SampleTexture2D
#define PARM_NODE_NAME sample_texture2d
#include EXE_FILEPATH

#define PARM_NODE_TYPE Texture2DAsset
#define PARM_NODE_NAME texture2d_asset
#include EXE_FILEPATH

//////////////////////////////////////////////////////////////////////////
// master
//////////////////////////////////////////////////////////////////////////

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE PBR
#define PARM_NODE_NAME pbr
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Raymarching
#define PARM_NODE_NAME raymarching
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF
#define PARM_NODE_NAME sdf
#include EXE_FILEPATH

//////////////////////////////////////////////////////////////////////////
// math
//////////////////////////////////////////////////////////////////////////

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Add
#define PARM_NODE_NAME add
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Subtract
#define PARM_NODE_NAME sub
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Multiply
#define PARM_NODE_NAME mul
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Divide
#define PARM_NODE_NAME div
#include EXE_FILEPATH

//////////////////////////////////////////////////////////////////////////
// procedural
//////////////////////////////////////////////////////////////////////////

// noise

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE fBm
#define PARM_NODE_NAME fbm
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE GradientNoise
#define PARM_NODE_NAME gradient_noise
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SimpleNoise
#define PARM_NODE_NAME simple_noise
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Voronoi
#define PARM_NODE_NAME voronoi
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE WorleyNoise
#define PARM_NODE_NAME worley_noise
#include EXE_FILEPATH

// sdf

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_Primitives
#define PARM_NODE_NAME sdf_primitives
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_Sphere
#define PARM_NODE_NAME sdf_sphere
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_Box
#define PARM_NODE_NAME sdf_box
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_RoundBox
#define PARM_NODE_NAME sdf_round_box
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_Torus
#define PARM_NODE_NAME sdf_torus
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_CappedTorus
#define PARM_NODE_NAME sdf_capped_torus
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_Link
#define PARM_NODE_NAME sdf_link
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_InfiniteCylinder
#define PARM_NODE_NAME sdf_infinite_cylinder
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_Cone
#define PARM_NODE_NAME sdf_cone
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_BoundCone
#define PARM_NODE_NAME sdf_bound_cone
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_InfiniteCone
#define PARM_NODE_NAME sdf_infinite_cone
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_Plane
#define PARM_NODE_NAME sdf_plane
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_HexagonalPrism
#define PARM_NODE_NAME sdf_hexagonal_prism
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_TriangularPrism
#define PARM_NODE_NAME sdf_triangular_prism
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_Capsule
#define PARM_NODE_NAME sdf_capsule
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_VerticalCapsule
#define PARM_NODE_NAME sdf_vertical_capsule
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_CappedCylinder
#define PARM_NODE_NAME sdf_capped_cylinder
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_CappedCylinder2
#define PARM_NODE_NAME sdf_capped_cylinder2
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_RoundedCylinder
#define PARM_NODE_NAME sdf_rounded_cylinder
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_CappedCone
#define PARM_NODE_NAME sdf_capped_cone
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_CappedCone2
#define PARM_NODE_NAME sdf_capped_cone2
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_SolidAngle
#define PARM_NODE_NAME sdf_solid_angle
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_RoundCone
#define PARM_NODE_NAME sdf_round_cone
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_RoundCone2
#define PARM_NODE_NAME sdf_round_cone2
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_Ellipsoid
#define PARM_NODE_NAME sdf_ellipsoid
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_Rhombus
#define PARM_NODE_NAME sdf_rhombus
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_Octahedron
#define PARM_NODE_NAME sdf_octahedron
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_Octahedron2
#define PARM_NODE_NAME sdf_octahedron2
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_Pyramid
#define PARM_NODE_NAME sdf_pyramid
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_Triangle
#define PARM_NODE_NAME sdf_triangle
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_Quad
#define PARM_NODE_NAME sdf_quad
#include EXE_FILEPATH

// op

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_Elongation
#define PARM_NODE_NAME sdf_elongation
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_Elongation2
#define PARM_NODE_NAME sdf_elongation2
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_Rounding
#define PARM_NODE_NAME sdf_rounding
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_Onion
#define PARM_NODE_NAME sdf_onion
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_Extrusion
#define PARM_NODE_NAME sdf_extrusion
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_Revolution
#define PARM_NODE_NAME sdf_revolution
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_Union
#define PARM_NODE_NAME sdf_union
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_Subtraction
#define PARM_NODE_NAME sdf_subtraction
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_Intersection
#define PARM_NODE_NAME sdf_intersection
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_SmoothUnion
#define PARM_NODE_NAME sdf_smooth_union
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_SmoothSubtraction
#define PARM_NODE_NAME sdf_smooth_subtraction
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_SmoothIntersection
#define PARM_NODE_NAME sdf_smooth_intersection
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_Scale
#define PARM_NODE_NAME sdf_scale
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_SymmetryX
#define PARM_NODE_NAME sdf_symmetry_x
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_SymmetryXZ
#define PARM_NODE_NAME sdf_symmetry_xz
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_InfiniteRepetition
#define PARM_NODE_NAME sdf_infinite_repetition
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_FiniteRepetition
#define PARM_NODE_NAME sdf_finite_repetition
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_Displacement
#define PARM_NODE_NAME sdf_displacement
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_Twist
#define PARM_NODE_NAME sdf_twist
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SDF_Bend
#define PARM_NODE_NAME sdf_bend
#include EXE_FILEPATH

// shapes

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Checkerboard
#define PARM_NODE_NAME checkerboard
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Ellipse
#define PARM_NODE_NAME ellipse
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Polygon
#define PARM_NODE_NAME polygon
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Rectangle
#define PARM_NODE_NAME rectangle
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE RoundedPolygon
#define PARM_NODE_NAME rounded_polygon
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE RoundedRectangle
#define PARM_NODE_NAME rounded_rectangle
#include EXE_FILEPATH

//////////////////////////////////////////////////////////////////////////
// utility
//////////////////////////////////////////////////////////////////////////

// Illumination

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Phong
#define PARM_NODE_NAME phong
#include EXE_FILEPATH

// shader

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE FragmentShader
#define PARM_NODE_NAME frag_shader
#include EXE_FILEPATH

// tools

#define PARM_NODE_TYPE CustomBlock
#define PARM_NODE_NAME custom_block
#include EXE_FILEPATH

#define PARM_NODE_TYPE Input
#define PARM_NODE_NAME input
#include EXE_FILEPATH

#define PARM_NODE_TYPE Output
#define PARM_NODE_NAME output
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE SubGraph
#define PARM_NODE_NAME sub_graph
#include EXE_FILEPATH

//////////////////////////////////////////////////////////////////////////
// uv
//////////////////////////////////////////////////////////////////////////

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Flipbook
#define PARM_NODE_NAME flipbook
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE PolarCoordinates
#define PARM_NODE_NAME polar_coordinates
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE RadialShear
#define PARM_NODE_NAME radial_shear
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Rotate
#define PARM_NODE_NAME rotate
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Spherize
#define PARM_NODE_NAME spherize
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE TilingAndOffset
#define PARM_NODE_NAME tiling_and_offset
#include EXE_FILEPATH

#define NO_PARM_FILEPATH
#define PARM_NODE_TYPE Twirl
#define PARM_NODE_NAME twirl
#include EXE_FILEPATH
