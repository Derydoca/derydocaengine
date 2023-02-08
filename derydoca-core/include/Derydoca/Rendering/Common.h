#pragma once

#include "Derydoca/Core.h"
#include <nvrhi/nvrhi.h>

namespace Derydoca::Rendering
{
    typedef void RenderPass;
    typedef void RenderTarget;
    typedef nvrhi::Format ImageFormat;

#define SUBPASS_EXTERNAL               (~0U)

    enum class RenderingAPI {
        Direct3D12,
        Vulkan
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
        union {
            float float32[4];
            int32_t int32[4];
            uint32_t uint32[4];
        };
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
        RectI Area;
        uint32_t ClearValueCount;
        ClearValue* ClearValues;
        RenderTarget** Targets;
        ClearValue* DepthStencil;
    };
}
