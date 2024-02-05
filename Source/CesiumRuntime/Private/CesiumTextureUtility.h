// Copyright 2020-2024 CesiumGS, Inc. and Contributors

#pragma once

#include "CesiumGltf/Model.h"
#include "CesiumMetadataValueType.h"
#include "CesiumTextureResource.h"
#include "Engine/Texture.h"
#include "Engine/Texture2D.h"
#include "Engine/TextureDefines.h"
#include "RHI.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Templates/UniquePtr.h"

namespace CesiumGltf {
struct ImageCesium;
struct Texture;
} // namespace CesiumGltf

namespace CesiumTextureUtility {
/**
 * @brief Half-loaded Unreal texture with info on how to finish loading the
 * texture on the game thread and render thread.
 */
struct LoadedTextureResult {
  TextureAddress addressX;
  TextureAddress addressY;
  TextureFilter filter;
  TextureGroup group;
  TWeakObjectPtr<UTexture2D> pTexture;
  bool sRGB{true};
  TUniquePtr<FCesiumTextureResourceBase> pTextureResource;
};

/**
 * Does the asynchronous part of renderer resource preparation for a `Texture`
 * in a glTF. Should be called in a worker thread.
 *
 * The `cesium.pixelData` will be removed from the image associated with the
 * texture so that it can be passed to Unreal's renderer thread without copying
 * it.
 *
 * @param model The glTF Model for which to load this texture.
 * @param texture The glTF Texture to load.
 * @param sRGB True if the texture should be treated as sRGB; false if it should
 * be treated as linear.
 * @param textureResources Unreal RHI texture resources that have already been
 * created for this model. This array must have the same size as `model`'s
 * {@link CesiumGltf::Model::images}, and all pointers must be initialized to
 * nullptr before the first call to `loadTextureFromModelAnyThreadPart` during
 * the glTF load process.
 */
TUniquePtr<LoadedTextureResult> loadTextureFromModelAnyThreadPart(
    CesiumGltf::Model& model,
    const CesiumGltf::Texture& texture,
    bool sRGB,
    std::vector<FCesiumTextureResourceBase*>& textureResources);

/**
 * Does the asynchronous part of renderer resource preparation for a glTF
 * `Image` with the given `Sampler` settings.
 *
 * The `cesium.pixelData` will be removed from the image so that it can be
 * passed to Unreal's renderer thread without copying it.
 *
 * @param image The glTF image for each to create a texture.
 * @param sampler The sampler settings to use with the texture.
 * @param sRGB True if the texture should be treated as sRGB; false if it should
 * be treated as linear.
 * @param pExistingImageResource An existing RHI texture resource that has been
 * created for this image, or nullptr if one hasn't been created yet. When this
 * parameter is not nullptr, the provided image's `pixelData` is not required
 * and can be empty.
 */
TUniquePtr<LoadedTextureResult> loadTextureFromImageAndSamplerAnyThreadPart(
    CesiumGltf::Image& image,
    const CesiumGltf::Sampler& sampler,
    bool sRGB,
    FCesiumTextureResourceBase* pExistingImageResource);

/**
 * @brief Does the asynchronous part of renderer resource preparation for
 * this image. Should be called in a background thread.
 *
 * The `pixelData` will be removed from the image so that it can be
 * passed to Unreal's renderer thread without copying it.
 *
 * @param imageCesium The image.
 * @param addressX The X addressing mode.
 * @param addressY The Y addressing mode.
 * @param filter The sampler filtering to use for this texture.
 * @param useMipMapsIfAvailable true to use this image's mipmaps for sampling,
 * if they exist; false to ignore any mipmaps that might be present.
 * @param group The texture group of this texture.
 * @param sRGB Whether this texture uses a sRGB color space.
 * @param overridePixelFormat The explicit pixel format to use. If std::nullopt,
 * the pixel format is inferred from the image.
 * @param pExistingImageResource An existing RHI texture resource that has been
 * created for this image, or nullptr if one hasn't been created yet. When this
 * parameter is not nullptr, the provided image's `pixelData` is not required
 * and can be empty.
 * @return The loaded texture.
 */
TUniquePtr<LoadedTextureResult> loadTextureAnyThreadPart(
    CesiumGltf::ImageCesium& imageCesium,
    TextureAddress addressX,
    TextureAddress addressY,
    TextureFilter filter,
    bool useMipMapsIfAvailable,
    TextureGroup group,
    bool sRGB,
    std::optional<EPixelFormat> overridePixelFormat,
    FCesiumTextureResourceBase* pExistingImageResource);

/**
 * @brief Does the main-thread part of render resource preparation for this
 * image and queues up any required render-thread tasks to finish preparing the
 * image.
 *
 * @param pHalfLoadedTexture The half-loaded renderer texture.
 * @return The Unreal texture result.
 */
UTexture2D* loadTextureGameThreadPart(LoadedTextureResult* pHalfLoadedTexture);

void destroyHalfLoadedTexture(LoadedTextureResult& halfLoaded);
void destroyTexture(UTexture* pTexture);
} // namespace CesiumTextureUtility
