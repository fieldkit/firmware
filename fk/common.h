#pragma once

#include <cinttypes>
#include <memory>

namespace fk {

/**
 * This is the theorhetical maximum number of modules that can be physically
 * connected to a single station. This is governed by the largest backplanes and
 * their possible combinations.
 */
constexpr size_t MaximumNumberOfModules = 8;

/**
 * The number of fixed HTTP routes that the firmware registers. These are routes
 * that are always there and never change with configuration.
 */
constexpr size_t HttpFixedRoutes = 2;

/**
 * The maximum number of HTTP routes that can be registered. Trying to register
 * more than this will fail. This is a function of the maximum number of modules
 * and the number of fixed routes.
 */
constexpr size_t HttpMaximumRoutes = MaximumNumberOfModules + HttpFixedRoutes;

}
