// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/shell/platform/linux/fl_settings_plugin.h"

#include <gmodule.h>

#include "flutter/shell/platform/linux/public/flutter_linux/fl_basic_message_channel.h"
#include "flutter/shell/platform/linux/public/flutter_linux/fl_json_message_codec.h"

static constexpr char kChannelName[] = "flutter/settings";
static constexpr char kTextScaleFactorKey[] = "textScaleFactor";
static constexpr char kAlwaysUse24HourFormatKey[] = "alwaysUse24HourFormat";
static constexpr char kPlatformBrightnessKey[] = "platformBrightness";
static constexpr char kPlatformBrightnessLight[] = "light";
static constexpr char kPlatformBrightnessDark[] = "dark";

struct _FlSettingsPlugin {
  GObject parent_instance;

  FlBasicMessageChannel* channel;

  FlSettings* settings;
};

G_DEFINE_TYPE(FlSettingsPlugin, fl_settings_plugin, G_TYPE_OBJECT)

static const gchar* to_platform_brightness(FlColorScheme color_scheme) {
  switch (color_scheme) {
    case FL_COLOR_SCHEME_LIGHT:
      return kPlatformBrightnessLight;
    case FL_COLOR_SCHEME_DARK:
      return kPlatformBrightnessDark;
    default:
      g_return_val_if_reached(nullptr);
  }
}

// Sends the current settings to the Flutter engine.
static void update_settings(FlSettingsPlugin* self) {
  FlClockFormat clock_format = fl_settings_get_clock_format(self->settings);
  FlColorScheme color_scheme = fl_settings_get_color_scheme(self->settings);
  gdouble scaling_factor = fl_settings_get_text_scaling_factor(self->settings);

  g_autoptr(FlValue) message = fl_value_new_map();
  fl_value_set_string_take(message, kTextScaleFactorKey,
                           fl_value_new_float(scaling_factor));
  fl_value_set_string_take(
      message, kAlwaysUse24HourFormatKey,
      fl_value_new_bool(clock_format == FL_CLOCK_FORMAT_24H));
  fl_value_set_string_take(
      message, kPlatformBrightnessKey,
      fl_value_new_string(to_platform_brightness(color_scheme)));
  fl_basic_message_channel_send(self->channel, message, nullptr, nullptr,
                                nullptr);
}

static void fl_settings_plugin_dispose(GObject* object) {
  FlSettingsPlugin* self = FL_SETTINGS_PLUGIN(object);

  g_clear_object(&self->channel);
  g_clear_object(&self->settings);

  G_OBJECT_CLASS(fl_settings_plugin_parent_class)->dispose(object);
}

static void fl_settings_plugin_class_init(FlSettingsPluginClass* klass) {
  G_OBJECT_CLASS(klass)->dispose = fl_settings_plugin_dispose;
}

static void fl_settings_plugin_init(FlSettingsPlugin* self) {}

FlSettingsPlugin* fl_settings_plugin_new(FlBinaryMessenger* messenger) {
  g_return_val_if_fail(FL_IS_BINARY_MESSENGER(messenger), nullptr);

  FlSettingsPlugin* self =
      FL_SETTINGS_PLUGIN(g_object_new(fl_settings_plugin_get_type(), nullptr));

  g_autoptr(FlJsonMessageCodec) codec = fl_json_message_codec_new();
  self->channel = fl_basic_message_channel_new(messenger, kChannelName,
                                               FL_MESSAGE_CODEC(codec));

  return self;
}

void fl_settings_plugin_start(FlSettingsPlugin* self, FlSettings* settings) {
  g_return_if_fail(FL_IS_SETTINGS_PLUGIN(self));
  g_return_if_fail(FL_IS_SETTINGS(settings));

  self->settings = FL_SETTINGS(g_object_ref(settings));
  g_signal_connect_object(settings, "changed", G_CALLBACK(update_settings),
                          self, G_CONNECT_SWAPPED);

  update_settings(self);
}
