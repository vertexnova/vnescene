/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   February 2026
 *
 * Autodoc:   yes
 * ----------------------------------------------------------------------
 */

#include "vertexnova/scene/camera/camera_factory.h"
#include <vertexnova/logging/logging.h>

namespace vne::scene {

namespace {
CREATE_VNE_LOGGER_CATEGORY("vnescene.camera")
}  // namespace

std::shared_ptr<PerspectiveCamera> CameraFactory::createPerspective(
    const PerspectiveCameraParameters& params) noexcept {
    std::shared_ptr<PerspectiveCamera> camera;
    if (params.viewport_width > 0.0f && params.viewport_height > 0.0f) {
        camera = std::make_shared<PerspectiveCamera>(params.fov,
                                                     params.viewport_width,
                                                     params.viewport_height,
                                                     params.near_plane,
                                                     params.far_plane,
                                                     params.name);
    } else {
        camera = std::make_shared<PerspectiveCamera>(params.fov,
                                                     params.aspect_ratio,
                                                     params.near_plane,
                                                     params.far_plane,
                                                     params.name);
    }
    camera->setPosition(params.position);
    camera->setTarget(params.target);
    camera->setUp(params.up);
    camera->updateMatrices();
    VNE_LOG_INFO << "CameraFactory: PerspectiveCamera \"" << params.name << "\" created";
    return camera;
}

std::shared_ptr<OrthographicCamera> CameraFactory::createOrthographic(
    const OrthographicCameraParameters& params) noexcept {
    auto camera = std::make_shared<OrthographicCamera>(params.left,
                                                       params.right,
                                                       params.bottom,
                                                       params.top,
                                                       params.near_plane,
                                                       params.far_plane,
                                                       params.name);
    camera->setPosition(params.position);
    camera->setTarget(params.target);
    camera->setUp(params.up);
    camera->updateMatrices();
    VNE_LOG_INFO << "CameraFactory: OrthographicCamera \"" << params.name << "\" created";
    return camera;
}

std::shared_ptr<ICamera> CameraFactory::create(const CameraParameters& params) noexcept {
    switch (params.type) {
        case CameraType::ePerspective: {
            const auto* p = dynamic_cast<const PerspectiveCameraParameters*>(&params);
            if (p) {
                return createPerspective(*p);
            }
            VNE_LOG_WARN << "CameraFactory::create: dynamic_cast to PerspectiveCameraParameters failed, using defaults";
            return createPerspective(PerspectiveCameraParameters{});
        }
        case CameraType::eOrthographic: {
            const auto* o = dynamic_cast<const OrthographicCameraParameters*>(&params);
            if (o) {
                return createOrthographic(*o);
            }
            VNE_LOG_WARN << "CameraFactory::create: dynamic_cast to OrthographicCameraParameters failed, using defaults";
            return createOrthographic(OrthographicCameraParameters{});
        }
        default:
            return createPerspective(PerspectiveCameraParameters{});
    }
}

}  // namespace vne::scene
