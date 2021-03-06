// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "flutter/shell/platform/linux/testing/mock_binary_messenger.h"
#include "flutter/shell/platform/linux/testing/mock_binary_messenger_response_handle.h"

using namespace flutter::testing;

G_DECLARE_FINAL_TYPE(FlMockBinaryMessenger,
                     fl_mock_binary_messenger,
                     FL,
                     MOCK_BINARY_MESSENGER,
                     GObject)

struct _FlMockBinaryMessenger {
  GObject parent_instance;
  MockBinaryMessenger* mock;
};

bool MockBinaryMessenger::HasMessageHandler(const gchar* channel) const {
  return message_handlers.at(channel) != nullptr;
}

void MockBinaryMessenger::SetMessageHandler(
    const gchar* channel,
    FlBinaryMessengerMessageHandler handler,
    gpointer user_data) {
  message_handlers[channel] = handler;
  user_datas[channel] = user_data;
}

void MockBinaryMessenger::ReceiveMessage(FlBinaryMessenger* messenger,
                                         const gchar* channel,
                                         GBytes* message) {
  FlBinaryMessengerMessageHandler handler = message_handlers[channel];
  if (response_handles[channel] == nullptr) {
    response_handles[channel] = FL_BINARY_MESSENGER_RESPONSE_HANDLE(
        fl_mock_binary_messenger_response_handle_new());
  }
  handler(messenger, channel, message, response_handles[channel],
          user_datas[channel]);
}

static void fl_mock_binary_messenger_iface_init(
    FlBinaryMessengerInterface* iface);

G_DEFINE_TYPE_WITH_CODE(
    FlMockBinaryMessenger,
    fl_mock_binary_messenger,
    G_TYPE_OBJECT,
    G_IMPLEMENT_INTERFACE(fl_binary_messenger_get_type(),
                          fl_mock_binary_messenger_iface_init))

static void fl_mock_binary_messenger_class_init(
    FlMockBinaryMessengerClass* klass) {}

static void fl_mock_binary_messenger_set_message_handler_on_channel(
    FlBinaryMessenger* messenger,
    const gchar* channel,
    FlBinaryMessengerMessageHandler handler,
    gpointer user_data,
    GDestroyNotify destroy_notify) {
  g_return_if_fail(FL_IS_MOCK_BINARY_MESSENGER(messenger));
  FlMockBinaryMessenger* self = FL_MOCK_BINARY_MESSENGER(messenger);
  self->mock->SetMessageHandler(channel, handler, user_data);
  self->mock->fl_binary_messenger_set_message_handler_on_channel(
      messenger, channel, handler, user_data, destroy_notify);
}

static gboolean fl_mock_binary_messenger_send_response(
    FlBinaryMessenger* messenger,
    FlBinaryMessengerResponseHandle* response_handle,
    GBytes* response,
    GError** error) {
  g_return_val_if_fail(FL_IS_MOCK_BINARY_MESSENGER(messenger), false);
  FlMockBinaryMessenger* self = FL_MOCK_BINARY_MESSENGER(messenger);
  return self->mock->fl_binary_messenger_send_response(
      messenger, response_handle, response, error);
}

static void fl_mock_binary_messenger_send_on_channel(
    FlBinaryMessenger* messenger,
    const gchar* channel,
    GBytes* message,
    GCancellable* cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data) {
  g_return_if_fail(FL_IS_MOCK_BINARY_MESSENGER(messenger));
  FlMockBinaryMessenger* self = FL_MOCK_BINARY_MESSENGER(messenger);
  self->mock->fl_binary_messenger_send_on_channel(
      messenger, channel, message, cancellable, callback, user_data);
}

static GBytes* fl_mock_binary_messenger_send_on_channel_finish(
    FlBinaryMessenger* messenger,
    GAsyncResult* result,
    GError** error) {
  g_return_val_if_fail(FL_IS_MOCK_BINARY_MESSENGER(messenger), nullptr);
  FlMockBinaryMessenger* self = FL_MOCK_BINARY_MESSENGER(messenger);
  return self->mock->fl_binary_messenger_send_on_channel_finish(messenger,
                                                                result, error);
}

static void fl_mock_binary_messenger_iface_init(
    FlBinaryMessengerInterface* iface) {
  iface->set_message_handler_on_channel =
      fl_mock_binary_messenger_set_message_handler_on_channel;
  iface->send_response = fl_mock_binary_messenger_send_response;
  iface->send_on_channel = fl_mock_binary_messenger_send_on_channel;
  iface->send_on_channel_finish =
      fl_mock_binary_messenger_send_on_channel_finish;
}

static void fl_mock_binary_messenger_init(FlMockBinaryMessenger* self) {}

FlBinaryMessenger* fl_binary_messenger_new_mock(MockBinaryMessenger* mock) {
  FlMockBinaryMessenger* self = FL_MOCK_BINARY_MESSENGER(
      g_object_new(fl_mock_binary_messenger_get_type(), NULL));
  self->mock = mock;
  return FL_BINARY_MESSENGER(self);
}
