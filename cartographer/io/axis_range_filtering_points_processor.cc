/*
 * Copyright 2016 The Cartographer Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "cartographer/io/axis_range_filtering_points_processor.h"

#include "absl/memory/memory.h"
#include "cartographer/common/lua_parameter_dictionary.h"
#include "cartographer/io/points_batch.h"

namespace cartographer {
namespace io {

std::unique_ptr<AxisRangeFilteringPointsProcessor>
AxisRangeFilteringPointsProcessor::FromDictionary(
    common::LuaParameterDictionary* const dictionary,
    PointsProcessor* const next) {
  return absl::make_unique<AxisRangeFilteringPointsProcessor>(
      dictionary->GetDouble("min"), dictionary->GetDouble("max"),
      dictionary->GetString("axis"), next);
}

AxisRangeFilteringPointsProcessor::AxisRangeFilteringPointsProcessor(
    const double min, const double max,
    const std::string axis, PointsProcessor* next)
    : min_(min), max_(max),
      axis_(axis), next_(next) {}

void AxisRangeFilteringPointsProcessor::Process(
    std::unique_ptr<PointsBatch> batch) {
  absl::flat_hash_set<int> to_remove;
  for (size_t i = 0; i < batch->points.size(); ++i) {
    if (axis_ == "x")
    {
      const float distance =
          batch->points[i].position.x() - batch->points[i].origin.x();
      if (!(min_ <= distance && distance <= max_) ) {
        to_remove.insert(i);
      }
    }
    else if (axis_ == "y")
    {
      const float distance =
          batch->points[i].position.y() - batch->points[i].origin.y();
      if (!(min_ <= distance && distance <= max_) ) {
        to_remove.insert(i);
      }
    }
    else if (axis_ == "z")
    {
      const float distance =
          batch->points[i].position.z() - batch->points[i].origin.z();
      if (!(min_ <= distance && distance <= max_) ) {
        to_remove.insert(i);
      }
    }
  }
  RemovePoints(to_remove, batch.get());
  next_->Process(std::move(batch));
}

PointsProcessor::FlushResult AxisRangeFilteringPointsProcessor::Flush() {
  return next_->Flush();
}

}  // namespace io
}  // namespace cartographer
