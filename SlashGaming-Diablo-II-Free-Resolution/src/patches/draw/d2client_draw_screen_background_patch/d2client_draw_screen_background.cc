/**
 * SlashGaming Diablo II Free Resolution
 * Copyright (C) 2019  Mir Drualga
 *
 * This file is part of SlashGaming Diablo II Free Resolution.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Additional permissions under GNU Affero General Public License version 3
 *  section 7
 *
 *  If you modify this Program, or any covered work, by linking or combining
 *  it with Diablo II (or a modified version of that game and its
 *  libraries), containing parts covered by the terms of Blizzard End User
 *  License Agreement, the licensors of this Program grant you additional
 *  permission to convey the resulting work. This additional permission is
 *  also extended to any combination of expansions, mods, and remasters of
 *  the game.
 *
 *  If you modify this Program, or any covered work, by linking or combining
 *  it with any Graphics Device Interface (GDI), DirectDraw, Direct3D,
 *  Glide, OpenGL, or Rave wrapper (or modified versions of those
 *  libraries), containing parts not covered by a compatible license, the
 *  licensors of this Program grant you additional permission to convey the
 *  resulting work.
 *
 *  If you modify this Program, or any covered work, by linking or combining
 *  it with any library (or a modified version of that library) that links
 *  to Diablo II (or a modified version of that game and its libraries),
 *  containing parts not covered by a compatible license, the licensors of
 *  this Program grant you additional permission to convey the resulting
 *  work.
 */

#include "d2client_draw_screen_background.hpp"

#include <windows.h>

#include <fmt/format.h>
#include <sgd2mapi.hpp>
#include "../../../config.hpp"
#include "../../../helper/get_resolution_from_id.hpp"

namespace sgd2fr::patches {
namespace {

constexpr std::string_view kOriginalScreenBorderFrameImagePath =
    "data\\global\\UI\\Panel\\800BorderFrame";

d2::CelFile_API& GetScreenBackground() {
  static d2::CelFile_API screen_background;

  return screen_background;
}

d2::CelFile_API& GetOriginalScreenBorderFrame() {
  static d2::CelFile_API screen_border_frame;

  return screen_border_frame;
}

d2::CelFile_API& GetCustomLeftScreenBorderLeft() {
  static d2::CelFile_API screen_border_left;

  return screen_border_left;
}

d2::CelFile_API& GetCustomLeftScreenBorderTop() {
  static d2::CelFile_API screen_border_top;

  return screen_border_top;
}

d2::CelFile_API& GetCustomLeftScreenBorderTopRight() {
  static d2::CelFile_API screen_border_top_right;

  return screen_border_top_right;
}

d2::CelFile_API& GetCustomLeftScreenBorderBottom() {
  static d2::CelFile_API screen_border_bottom;

  return screen_border_bottom;
}

d2::CelFile_API& GetCustomLeftScreenBorderBottomRight() {
  static d2::CelFile_API screen_border_bottom_right;

  return screen_border_bottom_right;
}

d2::CelFile_API& GetCustomRightScreenBorderRight() {
  static d2::CelFile_API screen_border_left;

  return screen_border_left;
}

d2::CelFile_API& GetCustomRightScreenBorderTop() {
  static d2::CelFile_API screen_border_top;

  return screen_border_top;
}

d2::CelFile_API& GetCustomRightScreenBorderTopLeft() {
  static d2::CelFile_API screen_border_top_left;

  return screen_border_top_left;
}

d2::CelFile_API& GetCustomRightScreenBorderBottom() {
  static d2::CelFile_API screen_border_bottom;

  return screen_border_bottom;
}

d2::CelFile_API& GetCustomRightScreenBorderBottomLeft() {
  static d2::CelFile_API screen_border_bottom_left;

  return screen_border_bottom_left;
}

void DrawLeftScreenBackground() {
  d2::CelFile_API& screen_background = GetScreenBackground();
  if (!screen_background.IsOpen() && !config::GetScreenBackgroundImagePath().empty()) {
    screen_background.Open(config::GetScreenBackgroundImagePath(), false);
  }

  std::tuple width_and_height = GetResolutionFromId(d2::d2gfx::GetResolutionMode());

  // If loading fails, draw a black rectangle.
  if (!screen_background.IsOpen()) {
    d2::d2gfx::DrawRectangle(
        0,
        0,
        std::get<0>(width_and_height) / 2,
        std::get<1>(width_and_height),
        0,
        d2::DrawEffect::kNone
    );

    return;
  }

  // Set up the frame options.
  d2::DrawCelFileFrameOptions frame_options;
  frame_options.color = mapi::RGBA32BitColor(255, 255, 255, 255);
  frame_options.draw_effect = d2::DrawEffect::kNone;
  frame_options.position_x_behavior = d2::DrawPositionXBehavior::kRight;
  frame_options.position_y_behavior = d2::DrawPositionYBehavior::kTop;

  // Store an array of Cel_Views to determine Cel Width and Heights.
  std::vector<d2::Cel_View> cels;
  for (unsigned int i = 0; i < screen_background.GetNumFrames(); i += 1) {
    cels.push_back(d2::Cel_View(screen_background.GetCel(0, i)));
  }

  // Draw the background.
  const unsigned int half_num_frames = screen_background.GetNumFrames() / 2;

  int height_covered = 0;
  for (unsigned int row = 0; height_covered < std::get<0>(width_and_height); row += 1) {
    int width_covered = 0;
    unsigned int row_start_frame = (row % 2) * half_num_frames;

    for (unsigned int col = 0; width_covered < ((std::get<0>(width_and_height) / 2) - 1); col += 1) {
      unsigned int current_frame = row_start_frame + ((half_num_frames - 1) - (col % half_num_frames));

      screen_background.DrawFrame(
          (std::get<0>(width_and_height) / 2) - width_covered + cels.at(current_frame).GetOffsetX(),
          height_covered + cels.at(current_frame).GetOffsetY(),
          0,
          current_frame,
          frame_options
      );

      width_covered += cels.at(current_frame).GetWidth();
    }

    height_covered += cels.at(row_start_frame).GetHeight();
  }
}

void DrawRightScreenBackground() {
  d2::CelFile_API& screen_background = GetScreenBackground();
  if (!screen_background.IsOpen() && !config::GetScreenBackgroundImagePath().empty()) {
    screen_background.Open(config::GetScreenBackgroundImagePath(), false);
  }

  std::tuple width_and_height = GetResolutionFromId(d2::d2gfx::GetResolutionMode());

  // If loading fails, draw a black rectangle.
  if (!screen_background.IsOpen()) {
    d2::d2gfx::DrawRectangle(
        std::get<0>(width_and_height) / 2,
        0,
        std::get<0>(width_and_height),
        std::get<1>(width_and_height),
        0,
        d2::DrawEffect::kNone
    );

    return;
  }

  // Set up the frame options.
  d2::DrawCelFileFrameOptions frame_options;
  frame_options.color = mapi::RGBA32BitColor(255, 255, 255, 255);
  frame_options.draw_effect = d2::DrawEffect::kNone;
  frame_options.position_x_behavior = d2::DrawPositionXBehavior::kLeft;
  frame_options.position_y_behavior = d2::DrawPositionYBehavior::kTop;

  // Store an array of Cel_Views to determine Cel Width and Heights.
  std::vector<d2::Cel_View> cels;
  for (unsigned int i = 0; i < screen_background.GetNumFrames(); i += 1) {
    cels.push_back(d2::Cel_View(screen_background.GetCel(0, i)));
  }

  // Draw the background.
  const unsigned int half_num_frames = screen_background.GetNumFrames() / 2;

  int height_covered = 0;
  for (unsigned int row = 0; height_covered < std::get<0>(width_and_height); row += 1) {
    int width_covered = 0;
    unsigned int row_start_frame = (row % 2) * half_num_frames;

    for (unsigned int col = 0; width_covered < ((std::get<0>(width_and_height) / 2) - 1); col += 1) {
      unsigned int current_frame = row_start_frame + (col % half_num_frames);

      screen_background.DrawFrame(
          (std::get<0>(width_and_height) / 2) + width_covered + cels.at(current_frame).GetOffsetX(),
          height_covered + cels.at(current_frame).GetOffsetY(),
          0,
          current_frame,
          frame_options
      );

      width_covered += cels.at(current_frame).GetWidth();
    }

    height_covered += cels.at(row_start_frame).GetHeight();
  }
}

/**
 * Drawing the original screen border frame requires significantly
 * different code due to its abnormal structure.
 */
void DrawOriginalLeftScreenBorderFrame() {
  d2::CelFile_API& screen_border_frame = GetOriginalScreenBorderFrame();
  screen_border_frame.Open(kOriginalScreenBorderFrameImagePath.data(), false);

  std::tuple width_and_height = GetResolutionFromId(d2::d2gfx::GetResolutionMode());

  const int left = (std::get<0>(width_and_height) - 640 - (800 - 640)) / 2;
  const int top = ((std::get<1>(width_and_height) - 480 - (600 - 480)) / 2) - 3;

  std::vector<d2::Cel_View> cels;
  for (unsigned int i = 0; i < screen_border_frame.GetNumFrames(); i += 1) {
    cels.push_back(screen_border_frame.GetCel(0, i));
  }

  d2::DrawCelFileFrameOptions frame_options;
  frame_options.color = mapi::RGBA32BitColor(255, 255, 255, 255);
  frame_options.draw_effect = d2::DrawEffect::kNone;
  frame_options.position_x_behavior = d2::DrawPositionXBehavior::kLeft;
  frame_options.position_y_behavior = d2::DrawPositionYBehavior::kTop;

  screen_border_frame.DrawFrame(left, top, 0, 0, frame_options);

  screen_border_frame.DrawFrame(
      left + cels.at(0).GetWidth(),
      top,
      0,
      1,
      frame_options
  );

  screen_border_frame.DrawFrame(
      left,
      top + cels.at(0).GetHeight(),
      0,
      2,
      frame_options
  );

  screen_border_frame.DrawFrame(
      left,
      top + cels.at(0).GetHeight() + cels.at(2).GetHeight(),
      0,
      3,
      frame_options
  );

  screen_border_frame.DrawFrame(
      left + cels.at(3).GetWidth(),
      top + cels.at(0).GetHeight() + cels.at(2).GetHeight(),
      0,
      4,
      frame_options
  );
}

void DrawOriginalRightScreenBorderFrame() {
  d2::CelFile_API& screen_border_frame = GetOriginalScreenBorderFrame();
  screen_border_frame.Open(kOriginalScreenBorderFrameImagePath.data(), false);

  std::tuple width_and_height = GetResolutionFromId(d2::d2gfx::GetResolutionMode());

  const int right = (std::get<0>(width_and_height) + 640 + (800 - 640)) / 2;
  const int top = ((std::get<1>(width_and_height) - 480 - (600 - 480)) / 2) - 3;

  std::vector<d2::Cel_View> cels;
  for (unsigned int i = 0; i < screen_border_frame.GetNumFrames(); i += 1) {
    cels.push_back(screen_border_frame.GetCel(0, i));
  }

  d2::DrawCelFileFrameOptions frame_options;
  frame_options.color = mapi::RGBA32BitColor(255, 255, 255, 255);
  frame_options.draw_effect = d2::DrawEffect::kNone;
  frame_options.position_x_behavior = d2::DrawPositionXBehavior::kRight;
  frame_options.position_y_behavior = d2::DrawPositionYBehavior::kTop;

  screen_border_frame.DrawFrame(right, top, 0, 6, frame_options);

  screen_border_frame.DrawFrame(
      right - cels.at(6).GetWidth(),
      top,
      0,
      5,
      frame_options
  );

  screen_border_frame.DrawFrame(
      right,
      top + cels.at(6).GetHeight(),
      0,
      7,
      frame_options
  );

  screen_border_frame.DrawFrame(
      right,
      top + cels.at(6).GetHeight() + cels.at(7).GetHeight(),
      0,
      8,
      frame_options
  );

  screen_border_frame.DrawFrame(
      right - cels.at(8).GetWidth(),
      top + cels.at(6).GetHeight() + cels.at(7).GetHeight(),
      0,
      9,
      frame_options
  );
}

void DrawCustomLeftScreenBorderFrame() {
}

void DrawCustomRightScreenBorderFrame() {
}

void DrawLeftScreenBorderFrame() {
  if (!config::IsScreenBorderFrameEnabled()) {
    return;
  }

  if (config::IsUseOriginalScreenBorderFrame()) {
    DrawOriginalLeftScreenBorderFrame();
  } else {
    DrawCustomLeftScreenBorderFrame();
  }
}

void DrawRightScreenBorderFrame() {
  if (!config::IsScreenBorderFrameEnabled()) {
    return;
  }

  if (config::IsUseOriginalScreenBorderFrame()) {
    DrawOriginalRightScreenBorderFrame();
  } else {
    DrawRightScreenBorderFrame();
  }
}

void DrawLeftScreenBackgroundRibbon() {
}

void DrawRightScreenBackgroundRibbon() {
}

} // namespace

void SGD2FR_D2ClientDrawScreenBackground() {
  switch (d2::d2client::GetScreenOpenMode()) {
    case 0: {
      break;
    }

    case 1: {
      DrawRightScreenBackground();
      DrawRightScreenBorderFrame();
      DrawRightScreenBackgroundRibbon();

      break;
    }

    case 2: {
      DrawLeftScreenBackground();
      DrawLeftScreenBorderFrame();
      DrawLeftScreenBackgroundRibbon();

      break;
    }

    case 3: {
      DrawLeftScreenBackground();
      DrawLeftScreenBorderFrame();
      DrawLeftScreenBackgroundRibbon();

      DrawRightScreenBackground();
      DrawRightScreenBorderFrame();
      DrawRightScreenBackgroundRibbon();

      break;
    }

    default: {
      std::wstring_view message = L"Unknown value {} for screen open mode.";

      MessageBoxW(
          nullptr,
          fmt::format(message, d2::d2client::GetScreenOpenMode()).data(),
          L"Unexpected Value",
          MB_OK | MB_ICONERROR
      );

      std::exit(0);
    }
  }
}

} // namespace sgd2fr::patches
