// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package dev.flutter.scenariosui;

import android.content.Intent;
import androidx.annotation.NonNull;
import androidx.test.filters.LargeTest;
import androidx.test.rule.ActivityTestRule;
import androidx.test.runner.AndroidJUnit4;
import dev.flutter.scenarios.TextPlatformViewActivity;
import org.junit.Before;
import org.junit.Rule;
import org.junit.Test;
import org.junit.runner.RunWith;

@RunWith(AndroidJUnit4.class)
@LargeTest
public class PlatformTextureUiTests {
  Intent intent;

  @Rule @NonNull
  public ActivityTestRule<TextPlatformViewActivity> activityRule =
      new ActivityTestRule<>(
          TextPlatformViewActivity.class, /*initialTouchMode=*/ false, /*launchActivity=*/ false);

  @Before
  public void setUp() {
    intent = new Intent(Intent.ACTION_MAIN);
    // Render a texture.
    intent.putExtra("use_android_view", false);
  }

  @Test
  public void testPlatformView() throws Exception {
    intent.putExtra("scenario_name", "platform_view");
    ScreenshotUtil.capture(
        activityRule.launchActivity(intent), "PlatformTextureUiTests_testPlatformView");
  }

  @Test
  public void testPlatformViewMultiple() throws Exception {
    intent.putExtra("scenario_name", "platform_view_multiple");
    ScreenshotUtil.capture(
        activityRule.launchActivity(intent), "PlatformTextureUiTests_testPlatformViewMultiple");
  }

  @Test
  public void testPlatformViewMultipleBackgroundForeground() throws Exception {
    intent.putExtra("scenario_name", "platform_view_multiple_background_foreground");
    ScreenshotUtil.capture(
        activityRule.launchActivity(intent),
        "PlatformTextureUiTests_testPlatformViewMultipleBackgroundForeground");
  }

  @Test
  public void testPlatformViewCliprect() throws Exception {
    intent.putExtra("scenario_name", "platform_view_cliprect");
    ScreenshotUtil.capture(
        activityRule.launchActivity(intent), "PlatformTextureUiTests_testPlatformViewCliprect");
  }

  @Test
  public void testPlatformViewCliprrect() throws Exception {
    intent.putExtra("scenario_name", "platform_view_cliprrect");
    ScreenshotUtil.capture(
        activityRule.launchActivity(intent), "PlatformTextureUiTests_testPlatformViewCliprrect");
  }

  @Test
  public void testPlatformViewClippath() throws Exception {
    intent.putExtra("scenario_name", "platform_view_clippath");
    ScreenshotUtil.capture(
        activityRule.launchActivity(intent), "PlatformTextureUiTests_testPlatformViewClippath");
  }

  @Test
  public void testPlatformViewTransform() throws Exception {
    intent.putExtra("scenario_name", "platform_view_transform");
    ScreenshotUtil.capture(
        activityRule.launchActivity(intent), "PlatformTextureUiTests_testPlatformViewTransform");
  }

  @Test
  public void testPlatformViewOpacity() throws Exception {
    intent.putExtra("scenario_name", "platform_view_opacity");
    ScreenshotUtil.capture(
        activityRule.launchActivity(intent), "PlatformTextureUiTests_testPlatformViewOpacity");
  }

  @Test
  public void testPlatformViewRotate() throws Exception {
    intent.putExtra("scenario_name", "platform_view_rotate");
    ScreenshotUtil.capture(
        activityRule.launchActivity(intent), "PlatformTextureUiTests_testPlatformViewRotate");
  }

  @Test
  public void testPlatformViewMultipleWithoutOverlays() throws Exception {
    intent.putExtra("scenario_name", "platform_view_multiple_without_overlays");
    ScreenshotUtil.capture(
        activityRule.launchActivity(intent),
        "PlatformTextureUiTests_testPlatformViewMultipleWithoutOverlays");
  }

  @Test
  public void testPlatformViewTwoIntersectingOverlays() throws Exception {
    intent.putExtra("scenario_name", "platform_view_two_intersecting_overlays");
    ScreenshotUtil.capture(
        activityRule.launchActivity(intent),
        "PlatformTextureUiTests_testPlatformViewTwoIntersectingOverlays");
  }

  @Test
  public void testPlatformViewWithoutOverlayIntersection() throws Exception {
    intent.putExtra("scenario_name", "platform_view_no_overlay_intersection");
    ScreenshotUtil.capture(
        activityRule.launchActivity(intent),
        "PlatformTextureUiTests_testPlatformViewWithoutOverlayIntersection");
  }
}
