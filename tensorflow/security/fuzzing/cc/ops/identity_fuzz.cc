// Copyright 2022 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <cstdint>
#include <vector>

#include <gtest/gtest.h>
#include "testing/fuzzing/fuzztest.h"
#include "tensorflow/cc/ops/standard_ops.h"
#include "tensorflow/core/framework/types.pb.h"
#include "tensorflow/security/fuzzing/cc/ops/fuzz_session.h"

namespace tensorflow {
namespace fuzzing {

// Creates FuzzIdentity class that wraps a single operation node session.
class FuzzIdentity : public FuzzSession<Tensor> {
  void BuildGraph(const Scope& scope) override {
    auto op_node =
        tensorflow::ops::Placeholder(scope.WithOpName("input"), DT_UINT8);
    tensorflow::ops::Identity(scope.WithOpName("output"), op_node);
  }
  void FuzzImpl(const Tensor& input_tensor) final {
    Status s = RunInputsWithStatus({{"input", input_tensor}});
    if (!s.ok()) {
      LOG(ERROR) << "Execution failed: " << s.error_message();
    }
  }
};

// Setup up fuzzing test.
FUZZ_TEST_F(FuzzIdentity, Fuzz).WithDomains(AnyTensor());

}  // end namespace fuzzing
}  // end namespace tensorflow
