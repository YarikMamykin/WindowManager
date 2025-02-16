#pragma once
#include "Centered.h"
#include "Grid.h"
#include "ParallelHorizontal.h"
#include "ParallelVertical.h"
#include "StackHorizontalBottom.h"
#include "StackHorizontalDouble.h"
#include "StackHorizontalTop.h"
#include "StackVerticalDouble.h"
#include "StackVerticalLeft.h"
#include "StackVerticalRight.h"

#include <variant>

namespace ymwm::layouts {

  using Parameters = std::variant<Centered,
                                  Grid,
                                  StackVerticalRight,
                                  StackVerticalLeft,
                                  StackVerticalDouble,
                                  StackHorizontalTop,
                                  StackHorizontalBottom,
                                  StackHorizontalDouble,
                                  ParallelVertical,
                                  ParallelHorizontal>;

} // namespace ymwm::layouts
