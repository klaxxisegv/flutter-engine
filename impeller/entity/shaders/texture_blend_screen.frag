// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

uniform sampler2D texture_sampler_dst;
uniform sampler2D texture_sampler_src;

in vec2 v_dst_texture_coords;
in vec2 v_src_texture_coords;

out vec4 frag_color;

// Emulate SamplerAddressMode::ClampToBorder.
vec4 SampleWithBorder(sampler2D tex, vec2 uv) {
  if (uv.x > 0 && uv.y > 0 && uv.x < 1 && uv.y < 1) {
    return texture(tex, uv);
  }
  return vec4(0);
}

void main() {
  vec4 dst = SampleWithBorder(texture_sampler_dst, v_dst_texture_coords);
  vec4 src = SampleWithBorder(texture_sampler_src, v_src_texture_coords);
  frag_color = src + dst - src * dst;
}