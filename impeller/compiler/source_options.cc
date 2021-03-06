// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "impeller/compiler/source_options.h"

namespace impeller {
namespace compiler {

SourceOptions::SourceOptions() = default;

SourceOptions::SourceOptions(const std::string& file_name)
    : type(SourceTypeFromFileName(file_name)), file_name(file_name) {}

SourceOptions::~SourceOptions() = default;

}  // namespace compiler
}  // namespace impeller
