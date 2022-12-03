#pragma once

#include "Derydoca/Core.h"
#include "Derydoca/Rendering/RenderTarget.h"

namespace Derydoca::Rendering
{
    typedef void RenderPass;

    enum class RenderingAPI {
        Direct3D12,
        Vulkan
    };

    enum class ImageFormat {
        UNKNOWN = 0,
        R32G32B32A32_TYPELESS = 1,
        R32G32B32A32_FLOAT = 2,
        R32G32B32A32_UINT = 3,
        R32G32B32A32_SINT = 4,
        R32G32B32_TYPELESS = 5,
        R32G32B32_FLOAT = 6,
        R32G32B32_UINT = 7,
        R32G32B32_SINT = 8,
        R16G16B16A16_TYPELESS = 9,
        R16G16B16A16_FLOAT = 10,
        R16G16B16A16_UNORM = 11,
        R16G16B16A16_UINT = 12,
        R16G16B16A16_SNORM = 13,
        R16G16B16A16_SINT = 14,
        R32G32_TYPELESS = 15,
        R32G32_FLOAT = 16,
        R32G32_UINT = 17,
        R32G32_SINT = 18,
        R32G8X24_TYPELESS = 19,
        D32_FLOAT_S8X24_UINT = 20,
        R32_FLOAT_X8X24_TYPELESS = 21,
        X32_TYPELESS_G8X24_UINT = 22,
        R10G10B10A2_TYPELESS = 23,
        R10G10B10A2_UNORM = 24,
        R10G10B10A2_UINT = 25,
        R11G11B10_FLOAT = 26,
        R8G8B8A8_TYPELESS = 27,
        R8G8B8A8_UNORM = 28,
        R8G8B8A8_UNORM_SRGB = 29,
        R8G8B8A8_UINT = 30,
        R8G8B8A8_SNORM = 31,
        R8G8B8A8_SINT = 32,
        R16G16_TYPELESS = 33,
        R16G16_FLOAT = 34,
        R16G16_UNORM = 35,
        R16G16_UINT = 36,
        R16G16_SNORM = 37,
        R16G16_SINT = 38,
        R32_TYPELESS = 39,
        D32_FLOAT = 40,
        R32_FLOAT = 41,
        R32_UINT = 42,
        R32_SINT = 43,
        R24G8_TYPELESS = 44,
        D24_UNORM_S8_UINT = 45,
        R24_UNORM_X8_TYPELESS = 46,
        X24_TYPELESS_G8_UINT = 47,
        R8G8_TYPELESS = 48,
        R8G8_UNORM = 49,
        R8G8_UINT = 50,
        R8G8_SNORM = 51,
        R8G8_SINT = 52,
        R16_TYPELESS = 53,
        R16_FLOAT = 54,
        D16_UNORM = 55,
        R16_UNORM = 56,
        R16_UINT = 57,
        R16_SNORM = 58,
        R16_SINT = 59,
        R8_TYPELESS = 60,
        R8_UNORM = 61,
        R8_UINT = 62,
        R8_SNORM = 63,
        R8_SINT = 64,
        A8_UNORM = 65,
        R1_UNORM = 66,
        R9G9B9E5_SHAREDEXP = 67,
        R8G8_B8G8_UNORM = 68,
        G8R8_G8B8_UNORM = 69,
        BC1_TYPELESS = 70,
        BC1_UNORM = 71,
        BC1_UNORM_SRGB = 72,
        BC2_TYPELESS = 73,
        BC2_UNORM = 74,
        BC2_UNORM_SRGB = 75,
        BC3_TYPELESS = 76,
        BC3_UNORM = 77,
        BC3_UNORM_SRGB = 78,
        BC4_TYPELESS = 79,
        BC4_UNORM = 80,
        BC4_SNORM = 81,
        BC5_TYPELESS = 82,
        BC5_UNORM = 83,
        BC5_SNORM = 84,
        B5G6R5_UNORM = 85,
        B5G5R5A1_UNORM = 86,
        B8G8R8A8_UNORM = 87,
        B8G8R8X8_UNORM = 88,
        R10G10B10_XR_BIAS_A2_UNORM = 89,
        B8G8R8A8_TYPELESS = 90,
        B8G8R8A8_UNORM_SRGB = 91,
        B8G8R8X8_TYPELESS = 92,
        B8G8R8X8_UNORM_SRGB = 93,
        BC6H_TYPELESS = 94,
        BC6H_UF16 = 95,
        BC6H_SF16 = 96,
        BC7_TYPELESS = 97,
        BC7_UNORM = 98,
        BC7_UNORM_SRGB = 99,
        AYUV = 100,
        Y410 = 101,
        Y416 = 102,
        NV12 = 103,
        P010 = 104,
        P016 = 105,
        YUV_420_OPAQUE = 106,
        YUY2 = 107,
        Y210 = 108,
        Y216 = 109,
        NV11 = 110,
        AI44 = 111,
        IA44 = 112,
        P8 = 113,
        A8P8 = 114,
        B4G4R4A4_UNORM = 115,
    };

    enum class RenderPassBeginningAccess
    {
        Discard = 0,
        Preserve,
        Clear,
        NoAccess
    };

    enum class RenderPassEndingAccess
    {
        Discard = 0,
        Preserve,
        Resolve,
        NoAccess
    };

    enum class ImageSampleCount
    {
        _1 = 1 << 0,
        _2 = 1 << 1,
        _4 = 1 << 2,
        _8 = 1 << 3,
        _16 = 1 << 4,
        _32 = 1 << 5,
        _64 = 1 << 6
    };

    enum class ImageLayout
    {
        Undefined = 0,
        General = 1,
        ColorAttachmentOptimal = 2,
        DepthStencilAttachmentOptimal = 3,
        DepthStencilReadOnlyOptimal = 4,
        ShaderReadOnlyOptimal = 5,
        TransferSrcOptimal = 6,
        TransferDstOptimal = 7,
        Preinitialized = 8,
        // Provided by VK_VERSION_1_1
        DepthReadOnlyStencilAttachmentOptimal = 1000117000,
        // Provided by VK_VERSION_1_1
        DepthAttachmentStencilReadOnlyOptimal = 1000117001,
        // Provided by VK_VERSION_1_2
        DepthAttachmentOptimal = 1000241000,
        // Provided by VK_VERSION_1_2
        DepthReadOnlyOptimal = 1000241001,
        // Provided by VK_VERSION_1_2
        StencilAttachmentOptimal = 1000241002,
        // Provided by VK_VERSION_1_2
        StencilReadOnlyOptimal = 1000241003,
        // Provided by VK_VERSION_1_3
        ReadOnlyOptimal = 1000314000,
        // Provided by VK_VERSION_1_3
        AttachmentOptimal = 1000314001,
        // Provided by VK_KHR_swapchain
        PresentSrc = 1000001002,
        // Provided by VK_KHR_shared_presentable_image
        SharedPresent = 1000111000,
        // Provided by VK_EXT_fragment_density_map
        FragmentDensityMapOptimal = 1000218000,
        // Provided by VK_KHR_fragment_shading_rate
        ShadingRateAttachmentOptimal = 1000164003,
        // Provided by VK_EXT_attachment_feedback_loop_layout
        FeedbackLoopOptimal = 1000339000,
    };

    enum class PipelineBindPoint
    {
        Graphics = 0,
        Compute = 1,
        RayTracing = 1000165000,
        SubpassShading = 1000369003
    };

    struct RenderPassRenderTargetDesc
    {
        ImageFormat Format;
        ImageSampleCount Samples;
        ImageLayout InitialLayout;
        ImageLayout FinalLayout;
        RenderPassBeginningAccess BeginningAccess;
        RenderPassEndingAccess EndingAccess;
    };

    struct RenderPassDepthStencilDesc
    {
        ImageFormat Format;
        ImageLayout InitialLayout;
        ImageLayout FinalLayout;
        RenderPassBeginningAccess DepthBeginningAccess;
        RenderPassBeginningAccess StencilBeginningAccess;
        RenderPassEndingAccess DepthEndingAccess;
        RenderPassEndingAccess StencilEndingAccess;
    };

    struct AttachmentReference
    {
        uint32_t Attachment;
        ImageLayout Layout;
    };

    struct SubpassDesc
    {
        PipelineBindPoint BindPoint;
        uint32_t InputAttachmentCount;
        AttachmentReference* InputAttachments;
        uint32_t ColorAttachmentCount;
        AttachmentReference* ColorAttachments; // Color attachments with ending access of `Resolve` are binned to ResolveAttachments
        //AttachmentReference* ResolveAttachments;
        AttachmentReference* DepthStencilAttachment;
        uint32_t PreserveAttachmentCount;
        uint32_t* PreserveAttachments;
    };

    enum class PipelineStageFlags
    {
        TopOfPipe = 0x00000001,
        DrawIndirect = 0x00000002,
        VertexInput = 0x00000004,
        VertexShader = 0x00000008,
        TessellationControlShader = 0x00000010,
        TessellationEvaluationShader = 0x00000020,
        GeometryShader = 0x00000040,
        FragmentShader = 0x00000080,
        EarlyFragmentTests = 0x00000100,
        LateFragmentTests = 0x00000200,
        ColorAttachmentOutput = 0x00000400,
        ComputeShader = 0x00000800,
        Transfer = 0x00001000,
        BottomOfPipe = 0x00002000,
        Host = 0x00004000,
        AllGraphics = 0x00008000,
        AllCommands = 0x00010000,
        None = 0,
        TransformFeedback = 0x01000000,
        ConditionalRendering = 0x00040000,
        AccellerationStructureBuild = 0x02000000,
        RayTracingShader = 0x00200000,
        FragmentDensityProcess = 0x00800000,
        FragmentShadingRateAttachment = 0x00400000,
        CommandPreprocess = 0x00020000,
        TaskShader = 0x00080000,
        MeshShader = 0x00100000
    };

    enum class AccessFlags
    {
        IndirectCommandRead = 0x00000001,
        IndexRead = 0x00000002,
        VertexAttributeRead = 0x00000004,
        UniformRead = 0x00000008,
        InputAttachmentRead = 0x00000010,
        ShaderRead = 0x00000020,
        ShaderWrite = 0x00000040,
        ColorAttachmentRead = 0x00000080,
        ColorAttachmentWrite = 0x00000100,
        DepthStencilAttachmentRead = 0x00000200,
        DepthStencilAttachmnetWrite = 0x00000400,
        TransferRead = 0x00000800,
        TransferWrite = 0x00001000,
        HostRead = 0x00002000,
        HostWrite = 0x00004000,
        MemoryRead = 0x00008000,
        MemoryWrite = 0x00010000,
        None = 0,
        TransformFeedbackWrite = 0x02000000,
        TransformFeedbackCounterRead = 0x04000000,
        TransformFeedbackCounterWrite = 0x08000000,
        ConditionalRenderingRead = 0x00100000,
        ColorAttachmentReadNoncoherent = 0x00080000,
        AccellerationStructureRead = 0x00200000,
        AccellerationStructureWrite = 0x00400000,
        FragmentDensityMapRead = 0x01000000,
        FragmentShadingRateAttachmentRead = 0x00800000,
        CommandPreprocessRead = 0x00020000,
        CommandPreprocessWrite = 0x00040000
    };

    enum class DependencyFlags
    {
        ByRegion = 0x00000001,
        DeviceGroup = 0x00000004,
        ViewLocal = 0x00000002,
        FeedbackLoop = 0x00000008
    };

    struct SubpassDependency
    {
        uint32_t srcSubpass;
        uint32_t dstSubpass;
        PipelineStageFlags srcStageMask;
        PipelineStageFlags dstStageMask;
        AccessFlags srcAccessMask;
        AccessFlags dstAccessMask;
        DependencyFlags dependencyFlags;
    };

    struct RenderPassDesc
    {
        uint32_t AttachmentCount;
        RenderPassRenderTargetDesc* RenderTargets;
        RenderPassDepthStencilDesc* DepthStencil;
        uint32_t SubpassCount;
        SubpassDesc* Subpasses;
        uint32_t DependencyCount;
        SubpassDependency* Dependencies;
    };

    struct ClearColorValue
    {
        float float32[4];
        int32_t int32[4];
        uint32_t uint32[4];
    };

    struct DepthStencilClearValue
    {
        float Depth;
        uint32_t Stencil;
    };

    struct ClearValue
    {
        ClearColorValue Color;
        DepthStencilClearValue DepthStencil;
    };

    struct RenderPassBeginInfo
    {
        RenderTarget* Target;
        RectI Area;
        uint32_t ClearValueCount;
        ClearValue* ClearValues;
    };
}
