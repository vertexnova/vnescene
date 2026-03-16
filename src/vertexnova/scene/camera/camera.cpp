/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Author:    Ajeet Singh Yadav
 * Created:   March 2026
 *
 * Autodoc:   yes
 * ----------------------------------------------------------------------
 */

#include "vertexnova/scene/camera/camera.h"
#include <vertexnova/math/projection_utils.h>

namespace vne::scene {

vne::math::Mat4f ICamera::getClipToScreenMatrix(float width, float height) const noexcept {
    return vne::math::clipToScreenMatrix(width, height, getGraphicsApi());
}

}  // namespace vne::scene
