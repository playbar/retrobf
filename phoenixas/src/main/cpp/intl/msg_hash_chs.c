﻿/*  RetroArch - A frontend for libretro.
 *  Copyright (C) 2011-2017 - Daniel De Matteis
 *
 *  RetroArch is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  RetroArch is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with RetroArch.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

#include <compat/strl.h>
#include <string/stdstring.h>

#include "../src/msg_hash.h"
#include "../src/configuration.h"
#include "../src/verbosity.h"

#if defined(_MSC_VER) && !defined(_XBOX)
/* https://support.microsoft.com/en-us/kb/980263 */
#pragma execution_character_set("utf-8")
#pragma warning( disable : 4566 )
#endif

int menu_hash_get_help_chs_enum(enum msg_hash_enums msg, char *s, size_t len)
{
   uint32_t driver_hash = 0;
   settings_t      *settings = config_get_ptr();

   if (msg <= MENU_ENUM_LABEL_INPUT_HOTKEY_BIND_END &&
         msg >= MENU_ENUM_LABEL_INPUT_HOTKEY_BIND_BEGIN)
   {
      unsigned idx = msg - MENU_ENUM_LABEL_INPUT_HOTKEY_BIND_BEGIN;

      switch (idx)
      {
         case RARCH_FAST_FORWARD_KEY:
            snprintf(s, len,
                  "在快进和正常速度之间切换。"
                  );
            break;
         case RARCH_FAST_FORWARD_HOLD_KEY:
            snprintf(s, len,
                  "按住来快进。 \n"
                  " \n"
                  "放开按键来取消快进。"
                  );
            break;
         case RARCH_PAUSE_TOGGLE:
            snprintf(s, len,
                  "在暂停和不暂停状态间切换。");
            break;
         case RARCH_FRAMEADVANCE:
            snprintf(s, len,
                  "Frame advance when content is paused.");
            break;
         case RARCH_SHADER_NEXT:
            snprintf(s, len,
                  "应用文件夹中的下一个Shader特效。");
            break;
         case RARCH_SHADER_PREV:
            snprintf(s, len,
                  "应用文件夹中的上一个Shader特效。");
            break;
         case RARCH_CHEAT_INDEX_PLUS:
         case RARCH_CHEAT_INDEX_MINUS:
         case RARCH_CHEAT_TOGGLE:
            snprintf(s, len,
                  "金手指。");
            break;
         case RARCH_RESET:
            snprintf(s, len,
                  "重置游戏内容。");
            break;
         case RARCH_SCREENSHOT:
            snprintf(s, len,
                  "截图。");
            break;
         case RARCH_MUTE:
            snprintf(s, len,
                  "静音/取消静音。");
            break;
         case RARCH_OSK:
            snprintf(s, len,
                  "显示/隐藏屏显键盘。");
            break;
         case RARCH_NETPLAY_FLIP:
            snprintf(s, len,
                  "Netplay flip users.");
            break;
         case RARCH_SLOWMOTION:
            snprintf(s, len,
                  "按住并以慢动作运行。");
            break;
         case RARCH_ENABLE_HOTKEY:
            snprintf(s, len,
                  "启用其他热键. \n"
                  " \n"
                  "If this hotkey is bound to either\n"
                  "a keyboard, joybutton or joyaxis, \n"
                  "all other hotkeys will be enabled only \n"
                  "if this one is held at the same time. \n"
                  " \n"
                  "This is useful for RETRO_KEYBOARD centric \n"
                  "implementations which query a large area of \n"
                  "the keyboard, where it is not desirable that \n"
                  "hotkeys get in the way. \n"
                  " \n"
                  "Alternatively, all hotkeys for keyboard \n"
                  "could be disabled by the user.");
            break;
         case RARCH_VOLUME_UP:
            snprintf(s, len,
                  "提高音量。");
            break;
         case RARCH_VOLUME_DOWN:
            snprintf(s, len,
                  "降低音量。");
            break;
         case RARCH_OVERLAY_NEXT:
            snprintf(s, len,
                  "切换到下一个屏幕覆层。将会循环选择。");
            break;
         case RARCH_DISK_EJECT_TOGGLE:
            snprintf(s, len,
                  "切换弹出光盘。 \n"
                  " \n"
                  "用于多光盘内容。 ");
            break;
         case RARCH_DISK_NEXT:
         case RARCH_DISK_PREV:
            snprintf(s, len,
                  "磁盘镜像周期。弹出后使用。 \n"
                  " \n"
                  "再次开关弹出完成。");
            break;
         case RARCH_GRAB_MOUSE_TOGGLE:
            snprintf(s, len,
                  "鼠标捕获开关 \n"
                  " \n"
                  "当鼠标捕获开启时，RetroArch将会隐藏鼠标指针，并 \n"
                  "使之控制在窗口中。这使得一些依靠鼠标相对位置的程 \n"
                  "序能更好运行。");
            break;
         case RARCH_MENU_TOGGLE:
            snprintf(s, len, "切换菜单显示。");
            break;
         case RARCH_LOAD_STATE_KEY:
            snprintf(s, len,
                  "加载游戏状态。");
            break;
         case RARCH_FULLSCREEN_TOGGLE_KEY:
            snprintf(s, len,
                  "切换到全屏模式。");
            break;
         case RARCH_QUIT_KEY:
            snprintf(s, len,
                  "用于正常退出 RetroArch 的按键 \n"
                  " \n"
                  "使用任何强制手段来退出RetroArch（如发送SIGKILL） \n"
                  "可能导致系统无法正确存储数据。"
#ifdef __unix__
                  "\n在类Unix系统上，SIGINT/SIGTERM信号能够保证正常 \n"
                  "关闭程序。"
#endif
                  "");
            break;
         case RARCH_STATE_SLOT_PLUS:
         case RARCH_STATE_SLOT_MINUS:
            snprintf(s, len,
                  "状态存储槽 \n"
                  " \n"
                  "当槽位选择为0时，状态存档将以*.state命名（或其他 \n"
                  "在命令行中定义的名称）。 \n"
                  " \n"
                  "当状态槽不为0时，路径将会设为<path><d>，其中<d> \n"
                  "是状态槽的索引。");
            break;
         case RARCH_SAVE_STATE_KEY:
            snprintf(s, len,
                  "保存游戏状态。");
            break;
         case RARCH_REWIND:
            snprintf(s, len,
                  "按住按钮来回溯 \n"
                  " \n"
                  "回溯功能必须被启用。");
            break;
         case RARCH_MOVIE_RECORD_TOGGLE:
            snprintf(s, len,
                  "在录制和非录制模式切换。");
            break;
         default:
            if (string_is_empty(s))
               strlcpy(s, msg_hash_to_str(MENU_ENUM_LABEL_VALUE_NO_INFORMATION_AVAILABLE), len);
            break;
      }

      return 0;
   }

   switch (msg)
   {
      case MENU_ENUM_LABEL_ACCOUNTS_RETRO_ACHIEVEMENTS:
         snprintf(s, len, "你的登陆信息 \n"
               "Retro Achievements 账号. \n"
               " \n"
               "访问 retroachievements.org 并注册 \n"
               "以获取一个免费账号. \n"
               " \n"
               "在你注册以后, 你需要 \n"
               "在RetroArch输入你的 \n"
               "账号以及密码.");
         break;
      case MENU_ENUM_LABEL_CHEEVOS_USERNAME:
         snprintf(s, len, "你的Retro Achievements账号的用户名。");
         break;
      case MENU_ENUM_LABEL_CHEEVOS_PASSWORD:
         snprintf(s, len, "你的Retro Achievements账号的密码。");
         break;
      case MENU_ENUM_LABEL_USER_LANGUAGE:
         snprintf(s, len, "依据选择的语言来本地化菜单和其他屏显消息。 \n"
               " \n"
               "需要重新启动来使之生效。 \n"
               " \n"
               "注意：可能不是所有的语言都已完成翻译工作。 \n"
               " \n"
               "若一个语言没有被翻译，则会退回到英语显示。");
         break;
      case MENU_ENUM_LABEL_VIDEO_FONT_PATH:
         snprintf(s, len, "改变屏显文字的字体。");
         break;
      case MENU_ENUM_LABEL_GAME_SPECIFIC_OPTIONS:
         snprintf(s, len, "自动加载游戏内容指定的核心选项.");
         break;
      case MENU_ENUM_LABEL_AUTO_OVERRIDES_ENABLE:
         snprintf(s, len, "自动加载覆盖配置。");
         break;
      case MENU_ENUM_LABEL_AUTO_REMAPS_ENABLE:
         snprintf(s, len, "自动加载输入重映射文件.");
         break;
      case MENU_ENUM_LABEL_SORT_SAVESTATES_ENABLE:
         snprintf(s, len, "Sort save states in folders \n"
               "named after the libretro core used.");
         break;
      case MENU_ENUM_LABEL_SORT_SAVEFILES_ENABLE:
         snprintf(s, len, "Sort save files in folders \n"
               "named after the libretro core used.");
         break;
      case MENU_ENUM_LABEL_RESUME_CONTENT:
         snprintf(s, len, "Exits from the menu and returns back \n"
               "to the content.");
         break;
      case MENU_ENUM_LABEL_RESTART_CONTENT:
         snprintf(s, len, "Restarts the content from the beginning.");
         break;
      case MENU_ENUM_LABEL_CLOSE_CONTENT:
         snprintf(s, len, "关闭内容并从内存中卸载。");
         break;
      case MENU_ENUM_LABEL_UNDO_LOAD_STATE:
         snprintf(s, len, "If a state was loaded, content will \n"
               "go back to the state prior to loading.");
         break;
      case MENU_ENUM_LABEL_UNDO_SAVE_STATE:
         snprintf(s, len, "如果状态被覆盖，它将 \n"
               "它将回滚到上一保存的状态。");
         break;
      case MENU_ENUM_LABEL_TAKE_SCREENSHOT:
         snprintf(s, len, "创建一份截图. \n"
               " \n"
               "截图文件将会存放在 \n"
               "截图目录之中.");
         break;
      case MENU_ENUM_LABEL_RUN:
         snprintf(s, len, "启动内容.");
         break;
      case MENU_ENUM_LABEL_INFORMATION:
         snprintf(s, len, "显示本内容的额外 \n"
               "元数据信息.");
         break;
      case MENU_ENUM_LABEL_FILE_BROWSER_CONFIG:
         snprintf(s, len, "配置文件.");
         break;
      case MENU_ENUM_LABEL_FILE_BROWSER_COMPRESSED_ARCHIVE:
         snprintf(s, len, "压缩归档文件.");
         break;
      case MENU_ENUM_LABEL_FILE_BROWSER_RECORD_CONFIG:
         snprintf(s, len, "记录配置文件.");
         break;
      case MENU_ENUM_LABEL_FILE_BROWSER_CURSOR:
         snprintf(s, len, "数据库指针文件。");
         break;
      case MENU_ENUM_LABEL_FILE_CONFIG:
         snprintf(s, len, "配置文件.");
         break;
      case MENU_ENUM_LABEL_SCAN_THIS_DIRECTORY:
         snprintf(s, len,
               "选择本项以扫描当前 \n"
               "目录中的内容.");
         break;
      case MENU_ENUM_LABEL_USE_THIS_DIRECTORY:
         snprintf(s, len,
               "选择本目录作为指定目录.");
         break;
      case MENU_ENUM_LABEL_CONTENT_DATABASE_DIRECTORY:
         snprintf(s, len,
               "内容数据库目录。 \n"
               " \n"
               "到内容数据库目录的 \n"
               "路径。");
         break;
      case MENU_ENUM_LABEL_THUMBNAILS_DIRECTORY:
         snprintf(s, len,
               "缩略图目录. \n"
               " \n"
               "用以存放缩略图.");
         break;
      case MENU_ENUM_LABEL_LIBRETRO_INFO_PATH:
         snprintf(s, len,
               "核心Core信息目录. \n"
               " \n"
               "用于搜索libretro核心信息 \n"
               "的目录。");
         break;
      case MENU_ENUM_LABEL_PLAYLIST_DIRECTORY:
         snprintf(s, len,
               "运行列表目录. \n"
               " \n"
               "保存所有播放列表到 \n"
               "此目录。");
         break;
      case MENU_ENUM_LABEL_DUMMY_ON_CORE_SHUTDOWN:
         snprintf(s, len,
               "某些libretro核心可能会 \n"
               "支持关机特性. \n"
               " \n"
               "If this option is left disabled, \n"
               "selecting the shutdown procedure \n"
               "would trigger RetroArch being shut \n"
               "down. \n"
               " \n"
               "Enabling this option will load a \n"
               "dummy core instead so that we remain \n"
               "inside the menu and RetroArch won't \n"
               "shutdown.");
         break;
      case MENU_ENUM_LABEL_CHECK_FOR_MISSING_FIRMWARE:
         snprintf(s, len,
               "Some cores might need \n"
               "firmware or bios files. \n"
               " \n"
               "If this option is disabled, \n"
               "it will try to load even if such \n"
               "firmware is missing. \n"
               "down. \n");
         break;
      case MENU_ENUM_LABEL_PARENT_DIRECTORY:
         snprintf(s, len,
               "回到上级目录。");
         break;
      case MENU_ENUM_LABEL_FILE_BROWSER_SHADER_PRESET:
         snprintf(s, len,
               "Shader预设文件。");
         break;
      case MENU_ENUM_LABEL_FILE_BROWSER_SHADER:
         snprintf(s, len,
               "Shader文件。");
         break;
      case MENU_ENUM_LABEL_FILE_BROWSER_REMAP:
         snprintf(s, len,
               "控制重映射文件。");
         break;
      case MENU_ENUM_LABEL_FILE_BROWSER_CHEAT:
         snprintf(s, len,
               "金手指文件。");
         break;
      case MENU_ENUM_LABEL_FILE_BROWSER_OVERLAY:
         snprintf(s, len,
               "覆层文件。");
         break;
      case MENU_ENUM_LABEL_FILE_BROWSER_RDB:
         snprintf(s, len,
               "数据库文件。");
         break;
      case MENU_ENUM_LABEL_FILE_BROWSER_FONT:
         snprintf(s, len,
               "TrueType字体文件。");
         break;
      case MENU_ENUM_LABEL_FILE_BROWSER_PLAIN_FILE:
         snprintf(s, len,
               "普通文件。");
         break;
      case MENU_ENUM_LABEL_FILE_BROWSER_MOVIE_OPEN:
         snprintf(s, len,
               "视频 \n"
               " \n"
               "选择文件并使用视频播放器打开。");
         break;
      case MENU_ENUM_LABEL_FILE_BROWSER_MUSIC_OPEN:
         snprintf(s, len,
               "音乐 \n"
               " \n"
               "选择文件并使用音乐播放器打开。");
         break;
      case MENU_ENUM_LABEL_FILE_BROWSER_IMAGE:
         snprintf(s, len,
               "图片文件。");
         break;
      case MENU_ENUM_LABEL_FILE_BROWSER_IMAGE_OPEN_WITH_VIEWER:
         snprintf(s, len,
               "图片 \n"
               " \n"
               "选择文件并使用图片浏览器打开。");
         break;
      case MENU_ENUM_LABEL_FILE_BROWSER_CORE_SELECT_FROM_COLLECTION:
         snprintf(s, len,
               "Libretro核心 \n"
               " \n"
               "选中核心将会使其关联至游戏。");
         break;
      case MENU_ENUM_LABEL_FILE_BROWSER_CORE:
         snprintf(s, len,
               "Libretro核心 \n"
               " \n"
               "选择该文件使 RetroArch 加载该核心。");
         break;
      case MENU_ENUM_LABEL_FILE_BROWSER_DIRECTORY:
         snprintf(s, len,
               "目录 \n"
               " \n"
               "选择并打开该文件夹。");
         break;
      case MENU_ENUM_LABEL_CACHE_DIRECTORY:
         snprintf(s, len,
               "缓存目录 \n"
               " \n"
               "被RetroArch解压的游戏内容会临时存放到这个文 \n"
               "件夹。");
         break;
      case MENU_ENUM_LABEL_HISTORY_LIST_ENABLE:
         snprintf(s, len,
               "若开启，所有在RetroArch中加载过的文件 \n"
               "都会自动的放入最近使用历史列表中。");
         break;
      case MENU_ENUM_LABEL_RGUI_BROWSER_DIRECTORY:
         snprintf(s, len,
               "文件浏览器目录 \n"
               " \n"
               "设置文件浏览器的初始目录。");
         break;
      case MENU_ENUM_LABEL_INPUT_POLL_TYPE_BEHAVIOR:
         snprintf(s, len,
               "影响输入轮询过程在RetroArch中的执行方式。 \n"
               " \n"
               "稍早 - 输入轮询过程将在帧生成之前执行。 \n"
               "正常 - 输入轮询过程将在被请求时执行。 \n"
               "稍晚 - 输入轮询过程将在每一帧的首次请求时执行。 \n"
               " \n"
               "依据设置的不同，设置为“稍早”或“稍晚”可以获得较低 \n"
               "的时延。 \n"
               "当在进行在线游戏时，不管设置的值如何，都只会启用 \n"
               "正常模式进行输入轮询过程。"
               );
         break;
      case MENU_ENUM_LABEL_INPUT_DESCRIPTOR_HIDE_UNBOUND:
         snprintf(s, len,
               "隐藏不被核心使用的输入描述。");
         break;
      case MENU_ENUM_LABEL_VIDEO_REFRESH_RATE:
         snprintf(s, len,
               "显示器的视频刷新率。 \n"
               "可被用来计算一个合适的音频输入率。");
         break;
      case MENU_ENUM_LABEL_VIDEO_FORCE_SRGB_DISABLE:
         snprintf(s, len,
               "强制关闭sRGB帧缓冲支持。\n"
               "\n"
               "某些在Windows上的Intel的OpenGL驱动会对sRGB帧缓 \n"
               "冲支持产生问题，需要启用以强制关闭程序对其的使用。");
         break;
      case MENU_ENUM_LABEL_AUDIO_ENABLE:
         snprintf(s, len,
               "启用音频输出。");
         break;
      case MENU_ENUM_LABEL_AUDIO_SYNC:
         snprintf(s, len,
               "同步音频（推荐）。");
         break;
      case MENU_ENUM_LABEL_AUDIO_LATENCY:
         snprintf(s, len,
               "Desired audio latency in milliseconds. \n"
               "Might not be honored if the audio driver \n"
               "can't provide given latency.");
         break;
      case MENU_ENUM_LABEL_VIDEO_ALLOW_ROTATE:
         snprintf(s, len,
               "Allow cores to set rotation. If false, \n"
               "rotation requests are honored, but ignored.\n\n"
               "Used for setups where one manually rotates \n"
               "the monitor.");
         break;
      case MENU_ENUM_LABEL_INPUT_DESCRIPTOR_LABEL_SHOW:
         snprintf(s, len,
               "Show the input descriptors set by the core \n"
               "instead of the default ones.");
         break;
      case MENU_ENUM_LABEL_CONTENT_HISTORY_SIZE:
         snprintf(s, len,
               "Number of entries that will be kept in \n"
               "content history playlist.");
         break;
      case MENU_ENUM_LABEL_VIDEO_WINDOWED_FULLSCREEN:
         snprintf(s, len,
               "To use windowed mode or not when going \n"
               "fullscreen.");
         break;
      case MENU_ENUM_LABEL_VIDEO_FONT_SIZE:
         snprintf(s, len,
               "屏显信息的字体大小.");
         break;
      case MENU_ENUM_LABEL_SAVESTATE_AUTO_INDEX:
         snprintf(s, len,
               "Automatically increment slot index on each save, \n"
               "generating multiple savestate files. \n"
               "When the content is loaded, state slot will be \n"
               "set to the highest existing value (last savestate).");
         break;
      case MENU_ENUM_LABEL_FPS_SHOW:
         snprintf(s, len,
               "Enables displaying the current frames \n"
               "per second.");
         break;
      case MENU_ENUM_LABEL_VIDEO_FONT_ENABLE:
         snprintf(s, len,
               "显示/隐藏屏显信息.");
         break;
      case MENU_ENUM_LABEL_VIDEO_MESSAGE_POS_X:
      case MENU_ENUM_LABEL_VIDEO_MESSAGE_POS_Y:
         snprintf(s, len,
               "Offset for where messages will be placed \n"
               "onscreen. Values are in range [0.0, 1.0].");
         break;
      case MENU_ENUM_LABEL_INPUT_OVERLAY_ENABLE:
         snprintf(s, len,
               "Enable or disable the current overlay.");
         break;
      case MENU_ENUM_LABEL_INPUT_OVERLAY_HIDE_IN_MENU:
         snprintf(s, len,
               "Hide the current overlay from appearing \n"
               "inside the menu.");
         break;
      case MENU_ENUM_LABEL_OVERLAY_PRESET:
         snprintf(s, len,
               "Path to input overlay.");
         break;
      case MENU_ENUM_LABEL_OVERLAY_OPACITY:
         snprintf(s, len,
               "Overlay opacity.");
         break;
      case MENU_ENUM_LABEL_INPUT_BIND_TIMEOUT:
         snprintf(s, len,
               "Input bind timer timeout (in seconds). \n"
               "Amount of seconds to wait until proceeding \n"
               "to the next bind.");
         break;
      case MENU_ENUM_LABEL_OVERLAY_SCALE:
         snprintf(s, len,
               "Overlay scale.");
         break;
      case MENU_ENUM_LABEL_AUDIO_OUTPUT_RATE:
         snprintf(s, len,
               "音频输出采样率.");
         break;
      case MENU_ENUM_LABEL_VIDEO_SHARED_CONTEXT:
         snprintf(s, len,
               "Set to true if hardware-rendered cores \n"
               "should get their private context. \n"
               "Avoids having to assume hardware state changes \n"
               "inbetween frames."
               );
         break;
      case MENU_ENUM_LABEL_CORE_LIST:
         snprintf(s, len,
               "加载内核. \n"
               " \n"
               "Browse for a libretro core \n"
               "implementation. Where the browser \n"
               "starts depends on your Core Directory \n"
               "path. If blank, it will start in root. \n"
               " \n"
               "If Core Directory is a directory, the menu \n"
               "will use that as top folder. If Core \n"
               "Directory is a full path, it will start \n"
               "in the folder where the file is.");
         break;
      case MENU_ENUM_LABEL_VALUE_MENU_ENUM_CONTROLS_PROLOG:
         snprintf(s, len,
               "你可以使用下述的方式通过游戏控制器或者键盘来对\n"
               "菜单进行控制：\n"
               " \n"
               );
         break;
      case MENU_ENUM_LABEL_WELCOME_TO_RETROARCH:
         snprintf(s, len,
               "欢迎来到 RetroArch\n"
               );
         break;
      case MENU_ENUM_LABEL_VALUE_HELP_AUDIO_VIDEO_TROUBLESHOOTING_DESC:
         {
            /* Work around C89 limitations */
            char u[501];
            const char * t =
                  "RetroArch relies on an unique form of\n"
                  "audio/video synchronization where it needs to be\n"
                  "calibrated against the refresh rate of your\n"
                  "display for best performance results.\n"
                  " \n"
                  "If you experience any audio crackling or video\n"
                  "tearing, usually it means that you need to\n"
                  "calibrate the settings. Some choices below:\n"
                  " \n";
            snprintf(u, sizeof(u), /* can't inline this due to the printf arguments */
                  "a) Go to '%s' -> '%s', and enable\n"
                  "'Threaded Video'. Refresh rate will not matter\n"
                  "in this mode, framerate will be higher,\n"
                  "but video might be less smooth.\n"
                  "b) Go to '%s' -> '%s', and look at\n"
                  "'%s'. Let it run for\n"
                  "2048 frames, then press 'OK'.",
                  msg_hash_to_str(MENU_ENUM_LABEL_VALUE_SETTINGS),
                  msg_hash_to_str(MENU_ENUM_LABEL_VALUE_VIDEO_SETTINGS),
                  msg_hash_to_str(MENU_ENUM_LABEL_VALUE_SETTINGS),
                  msg_hash_to_str(MENU_ENUM_LABEL_VALUE_VIDEO_SETTINGS),
                  msg_hash_to_str(MENU_ENUM_LABEL_VALUE_VIDEO_REFRESH_RATE_AUTO));
            strlcpy(s, t, len);
            strlcat(s, u, len);
         }
         break;
      case MENU_ENUM_LABEL_VALUE_HELP_SCANNING_CONTENT_DESC:
         snprintf(s, len,
               "若要扫描游戏内容，请访问菜单「%s」 \n"
               "并选择「%s」或者「%s」。\n"
               " \n"
               "文件将会同数据库中的条目进行对比。 \n"
               "若文件匹配某个条目，则它会被加入收藏中。 \n"
               " \n"
               "你可以无需每次都打开文件浏览器，而可以直接 \n"
               "通过菜单项「%s」->「%s」 来访\n"
               "问这些游戏内容。 \n"
               " \n"
               "注意：不是所有核心的游戏内容都支持扫描录入。"
               ,
               msg_hash_to_str(MENU_ENUM_LABEL_VALUE_ADD_CONTENT_LIST),
               msg_hash_to_str(MENU_ENUM_LABEL_VALUE_SCAN_DIRECTORY),
               msg_hash_to_str(MENU_ENUM_LABEL_VALUE_SCAN_FILE),
               msg_hash_to_str(MENU_ENUM_LABEL_VALUE_LOAD_CONTENT_LIST),
               msg_hash_to_str(MENU_ENUM_LABEL_VALUE_CONTENT_COLLECTION_LIST)
               );
         break;
      case MENU_ENUM_LABEL_VALUE_EXTRACTING_PLEASE_WAIT:
         snprintf(s, len,
               "欢迎使用RetroArch\n"
               "\n"
               "正在解压必要文件, 请稍等。\n"
               "这可能需要一点时间……\n"
               );
         break;
      case MENU_ENUM_LABEL_INPUT_DRIVER:
         if (settings)
            driver_hash = msg_hash_calculate(settings->arrays.input_driver);

         switch (driver_hash)
         {
            case MENU_LABEL_INPUT_DRIVER_UDEV:
               snprintf(s, len,
                     "udev Input driver. \n"
                     " \n"
                     "This driver can run without X. \n"
                     " \n"
                     "It uses the recent evdev joypad API \n"
                     "for joystick support. It supports \n"
                     "hotplugging and force feedback (if \n"
                     "supported by device). \n"
                     " \n"
                     "The driver reads evdev events for keyboard \n"
                     "support. It also supports keyboard callback, \n"
                     "mice and touchpads. \n"
                     " \n"
                     "By default in most distros, /dev/input nodes \n"
                     "are root-only (mode 600). You can set up a udev \n"
                     "rule which makes these accessible to non-root."
                     );
               break;
            case MENU_LABEL_INPUT_DRIVER_LINUXRAW:
               snprintf(s, len,
                     "linuxraw Input driver. \n"
                     " \n"
                     "This driver requires an active TTY. Keyboard \n"
                     "events are read directly from the TTY which \n"
                     "makes it simpler, but not as flexible as udev. \n" "Mice, etc, are not supported at all. \n"
                     " \n"
                     "This driver uses the older joystick API \n"
                     "(/dev/input/js*).");
               break;
            default:
               snprintf(s, len,
                     "Input driver.\n"
                     " \n"
                     "Depending on video driver, it might \n"
                     "force a different input driver.");
               break;
         }
         break;
      case MENU_ENUM_LABEL_LOAD_CONTENT_LIST:
         snprintf(s, len,
               "加载游戏内容 \n"
               "通过浏览来加载游戏内容。 \n"
               " \n"
               "你需要同时提供一个“核心”和游戏内容文 \n"
               "件才能启动并加载游戏内容。 \n"
               " \n"
               "设置“文件浏览器目录”可以指定以哪个位 \n"
               "置为文件浏览器的默认目录以方便加载。 \n"
               "若没有设置，默认以根目录为基准。 \n"
               " \n"
               "文件浏览器会以上次加载的核心所支持的 \n"
               "扩展名进行过滤，并使用该核心来加载游 \n"
               "戏内容。"
               );
         break;
      case MENU_ENUM_LABEL_LOAD_CONTENT_HISTORY:
         snprintf(s, len,
               "从历史记录中加载内容. \n"
               " \n"
               "As content is loaded, content and libretro \n"
               "core combinations are saved to history. \n"
               " \n"
               "The history is saved to a file in the same \n"
               "directory as the RetroArch config file. If \n"
               "no config file was loaded in startup, history \n"
               "will not be saved or loaded, and will not exist \n"
               "in the main menu."
               );
         break;
      case MENU_ENUM_LABEL_VIDEO_DRIVER:
         snprintf(s, len,
               "当前视频驱动.");

         if (string_is_equal_fast(settings->arrays.video_driver, "gl", 2))
         {
            snprintf(s, len,
                  "OpenGL视频驱动. \n"
                  " \n"
                  "This driver allows libretro GL cores to  \n"
                  "be used in addition to software-rendered \n"
                  "core implementations.\n"
                  " \n"
                  "Performance for software-rendered and \n"
                  "libretro GL core implementations is \n"
                  "dependent on your graphics card's \n"
                  "underlying GL driver).");
         }
         else if (string_is_equal_fast(settings->arrays.video_driver, "sdl2", 4))
         {
            snprintf(s, len,
                  "SDL 2 视频驱动.\n"
                  " \n"
                  "This is an SDL 2 software-rendered video \n"
                  "driver.\n"
                  " \n"
                  "Performance for software-rendered libretro \n"
                  "core implementations is dependent \n"
                  "on your platform SDL implementation.");
         }
         else if (string_is_equal_fast(settings->arrays.video_driver, "sdl1", 4))
         {
            snprintf(s, len,
                  "SDL 视频驱动.\n"
                  " \n"
                  "This is an SDL 1.2 software-rendered video \n"
                  "driver.\n"
                  " \n"
                  "Performance is considered to be suboptimal. \n"
                  "Consider using it only as a last resort.");
         }
         else if (string_is_equal_fast(settings->arrays.video_driver, "d3d", 3))
         {
            snprintf(s, len,
                  "Direct3D 视频驱动. \n"
                  " \n"
                  "Performance for software-rendered cores \n"
                  "is dependent on your graphic card's \n"
                  "underlying D3D driver).");
         }
         else if (string_is_equal_fast(settings->arrays.video_driver, "exynos", 6))
         {
            snprintf(s, len,
                  "Exynos-G2D 视频驱动. \n"
                  " \n"
                  "This is a low-level Exynos video driver. \n"
                  "Uses the G2D block in Samsung Exynos SoC \n"
                  "for blit operations. \n"
                  " \n"
                  "Performance for software rendered cores \n"
                  "should be optimal.");
         }
         else if (string_is_equal_fast(settings->arrays.video_driver, "drm", 3))
         {
            snprintf(s, len,
                  "Plain DRM 视频驱动. \n"
                  " \n"
                  "This is a low-level video driver using. \n"
                  "libdrm for hardware scaling using \n"
                  "GPU overlays.");
         }
         else if (string_is_equal_fast(settings->arrays.video_driver, "sunxi", 5))
         {
            snprintf(s, len,
                  "Sunxi-G2D 视频驱动. \n"
                  " \n"
                  "This is a low-level Sunxi video driver. \n"
                  "Uses the G2D block in Allwinner SoCs.");
         }
         break;
      case MENU_ENUM_LABEL_AUDIO_DSP_PLUGIN:
         snprintf(s, len,
               "音频DSP插件.\n"
               " Processes audio before it's sent to \n"
               "the driver."
               );
         break;
      case MENU_ENUM_LABEL_AUDIO_RESAMPLER_DRIVER:
         if (settings)
            driver_hash = msg_hash_calculate(settings->arrays.audio_resampler);

         switch (driver_hash)
         {
            case MENU_LABEL_AUDIO_RESAMPLER_DRIVER_SINC:
               snprintf(s, len,
                     "Windowed SINC implementation.");
               break;
            case MENU_LABEL_AUDIO_RESAMPLER_DRIVER_CC:
               snprintf(s, len,
                     "Convoluted Cosine implementation.");
               break;
            default:
               if (string_is_empty(s))
                  strlcpy(s, msg_hash_to_str(MENU_ENUM_LABEL_VALUE_NO_INFORMATION_AVAILABLE), len);
               break;
         }
         break;
      case MENU_ENUM_LABEL_VIDEO_SHADER_PRESET:
         snprintf(s, len,
               "载入预设 Shader. \n"
               " \n"
               " Load a "
#ifdef HAVE_CG
               "Cg"
#endif
#ifdef HAVE_GLSL
#ifdef HAVE_CG
               "/"
#endif
               "GLSL"
#endif
#ifdef HAVE_HLSL
#if defined(HAVE_CG) || defined(HAVE_HLSL)
               "/"
#endif
               "HLSL"
#endif
               " 预设目录. \n"
               "The menu shader menu is updated accordingly. \n"
               " \n"
               "If the CGP uses scaling methods which are not \n"
               "simple, (i.e. source scaling, same scaling \n"
               "factor for X/Y), the scaling factor displayed \n"
               "in the menu might not be correct."
               );
         break;
      case MENU_ENUM_LABEL_VIDEO_SHADER_SCALE_PASS:
         snprintf(s, len,
               "Scale for this pass. \n"
               " \n"
               "The scale factor accumulates, i.e. 2x \n"
               "for first pass and 2x for second pass \n"
               "will give you a 4x total scale. \n"
               " \n"
               "If there is a scale factor for last \n"
               "pass, the result is stretched to \n"
               "screen with the filter specified in \n"
               "'Default Filter'. \n"
               " \n"
               "If 'Don't Care' is set, either 1x \n"
               "scale or stretch to fullscreen will \n"
               "be used depending if it's not the last \n"
               "pass or not."
               );
         break;
      case MENU_ENUM_LABEL_VIDEO_SHADER_NUM_PASSES:
         snprintf(s, len,
               "Shader Passes. \n"
               " \n"
               "RetroArch allows you to mix and match various \n"
               "shaders with arbitrary shader passes, with \n"
               "custom hardware filters and scale factors. \n"
               " \n"
               "This option specifies the number of shader \n"
               "passes to use. If you set this to 0, and use \n"
               "Apply Shader Changes, you use a 'blank' shader. \n"
               " \n"
               "The Default Filter option will affect the \n"
               "stretching filter.");
         break;
      case MENU_ENUM_LABEL_VIDEO_SHADER_PARAMETERS:
         snprintf(s, len,
               "Shader Parameters. \n"
               " \n"
               "Modifies current shader directly. Will not be \n"
               "saved to CGP/GLSLP preset file.");
         break;
      case MENU_ENUM_LABEL_VIDEO_SHADER_PRESET_PARAMETERS:
         snprintf(s, len,
               "Shader Preset Parameters. \n"
               " \n"
               "Modifies shader preset currently in menu."
               );
         break;
      case MENU_ENUM_LABEL_VIDEO_SHADER_PASS:
         snprintf(s, len,
               "Path to shader. \n"
               " \n"
               "All shaders must be of the same \n"
               "type (i.e. CG, GLSL or HLSL). \n"
               " \n"
               "Set Shader Directory to set where \n"
               "the browser starts to look for \n"
               "shaders."
               );
         break;
      case MENU_ENUM_LABEL_CONFIGURATION_SETTINGS:
         snprintf(s, len,
               "Determines how configuration files \n"
               "are loaded and prioritized.");
         break;
      case MENU_ENUM_LABEL_CONFIG_SAVE_ON_EXIT:
         snprintf(s, len,
               "Saves config to disk on exit.\n"
               "Useful for menu as settings can be\n"
               "modified. Overwrites the config.\n"
               " \n"
               "#include's and comments are not \n"
               "preserved. \n"
               " \n"
               "By design, the config file is \n"
               "considered immutable as it is \n"
               "likely maintained by the user, \n"
               "and should not be overwritten \n"
               "behind the user's back."
#if defined(RARCH_CONSOLE) || defined(RARCH_MOBILE)
               "\nThis is not not the case on \n"
               "consoles however, where \n"
               "looking at the config file \n"
               "manually isn't really an option."
#endif
               );
         break;
      case MENU_ENUM_LABEL_SHOW_HIDDEN_FILES:
         snprintf(s, len, "显示隐藏文件和文件夹。");
         break;
      case MENU_ENUM_LABEL_VIDEO_SHADER_FILTER_PASS:
         snprintf(s, len,
               "Hardware filter for this pass. \n"
               " \n"
               "If 'Don't Care' is set, 'Default \n"
               "Filter' will be used."
               );
         break;
      case MENU_ENUM_LABEL_AUTOSAVE_INTERVAL:
         snprintf(s, len,
               "Autosaves the non-volatile SRAM \n"
               "at a regular interval.\n"
               " \n"
               "This is disabled by default unless set \n"
               "otherwise. The interval is measured in \n"
               "seconds. \n"
               " \n"
               "A value of 0 disables autosave.");
         break;
      case MENU_ENUM_LABEL_INPUT_BIND_DEVICE_TYPE:
         snprintf(s, len,
               "输入设备类型. \n"
               " \n"
               "Picks which device type to use. This is \n"
               "relevant for the libretro core itself."
               );
         break;
      case MENU_ENUM_LABEL_LIBRETRO_LOG_LEVEL:
         snprintf(s, len,
               "设置libretro核心的log等级 \n"
               "(GET_LOG_INTERFACE). \n"
               " \n"
               " If a log level issued by a libretro \n"
               " core is below libretro_log level, it \n"
               " is ignored.\n"
               " \n"
               " DEBUG logs are always ignored unless \n"
               " verbose mode is activated (--verbose).\n"
               " \n"
               " DEBUG = 0\n"
               " INFO  = 1\n"
               " WARN  = 2\n"
               " ERROR = 3"
               );
         break;
      case MENU_ENUM_LABEL_STATE_SLOT_INCREASE:
      case MENU_ENUM_LABEL_STATE_SLOT_DECREASE:
         snprintf(s, len,
               "即时存档栏位.\n"
               " \n"
               " With slot set to 0, save state name is *.state \n"
               " (or whatever defined on commandline).\n"
               "When slot is != 0, path will be (path)(d), \n"
               "where (d) is slot number.");
         break;
      case MENU_ENUM_LABEL_SHADER_APPLY_CHANGES:
         snprintf(s, len,
               "应用Shader更改. \n"
               " \n"
               "After changing shader settings, use this to \n"
               "apply changes. \n"
               " \n"
               "Changing shader settings is a somewhat \n"
               "expensive operation so it has to be \n"
               "done explicitly. \n"
               " \n"
               "When you apply shaders, the menu shader \n"
               "settings are saved to a temporary file (either \n"
               "menu.cgp or menu.glslp) and loaded. The file \n"
               "persists after RetroArch exits. The file is \n"
               "saved to Shader Directory."
               );
         break;
      case MENU_ENUM_LABEL_MENU_TOGGLE:
         snprintf(s, len,
               "切换菜单.");
         break;
      case MENU_ENUM_LABEL_GRAB_MOUSE_TOGGLE:
         snprintf(s, len,
               "切换鼠标抓取.\n"
               " \n"
               "When mouse is grabbed, RetroArch hides the \n"
               "mouse, and keeps the mouse pointer inside \n"
               "the window to allow relative mouse input to \n"
               "work better.");
         break;
      case MENU_ENUM_LABEL_DISK_NEXT:
         snprintf(s, len,
               "Cycles through disk images. Use after \n"
               "ejecting. \n"
               " \n"
               " Complete by toggling eject again.");
         break;
      case MENU_ENUM_LABEL_VIDEO_FILTER:
#ifdef HAVE_FILTERS_BUILTIN
         snprintf(s, len,
               "CPU-based video filter.");
#else
         snprintf(s, len,
               "CPU-based video filter.\n"
               " \n"
               "Path to a dynamic library.");
#endif
         break;
      case MENU_ENUM_LABEL_AUDIO_DEVICE:
         snprintf(s, len,
               "Override the default audio device \n"
               "the audio driver uses.\n"
               "This is driver dependent. E.g.\n"
#ifdef HAVE_ALSA
               " \n"
               "ALSA 需要一个PCM设备."
#endif
#ifdef HAVE_OSS
               " \n"
               "OSS 需要一个路径 (例如. /dev/dsp)."
#endif
#ifdef HAVE_JACK
               " \n"
               "JACK wants portnames (e.g. system:playback1\n"
               ",system:playback_2)."
#endif
#ifdef HAVE_RSOUND
               " \n"
               "RSound wants an IP address to an RSound \n"
               "server."
#endif
               );
         break;
      case MENU_ENUM_LABEL_DISK_EJECT_TOGGLE:
         snprintf(s, len,
               "Toggles eject for disks.\n"
               " \n"
               "Used for multiple-disk content.");
         break;
      case MENU_ENUM_LABEL_ENABLE_HOTKEY:
         snprintf(s, len,
               "启用其他热键.\n"
               " \n"
               " If this hotkey is bound to either keyboard, \n"
               "joybutton or joyaxis, all other hotkeys will \n"
               "be disabled unless this hotkey is also held \n"
               "at the same time. \n"
               " \n"
               "This is useful for RETRO_KEYBOARD centric \n"
               "implementations which query a large area of \n"
               "the keyboard, where it is not desirable that \n"
               "hotkeys get in the way.");
         break;
      case MENU_ENUM_LABEL_REWIND_ENABLE:
         snprintf(s, len,
               "启用回溯倒带功能.\n"
               " \n"
               "这可能会严重影响性能, \n"
               "所以缺省设置为关闭.");
         break;
      case MENU_ENUM_LABEL_LIBRETRO_DIR_PATH:
         snprintf(s, len,
               "核心目录. \n"
               " \n"
               "A directory for where to search for \n"
               "libretro core implementations.");
         break;
      case MENU_ENUM_LABEL_VIDEO_REFRESH_RATE_AUTO:
         snprintf(s, len,
               "自动匹配刷新率.\n"
               " \n"
               "The accurate refresh rate of our monitor (Hz).\n"
               "This is used to calculate audio input rate with \n"
               "the formula: \n"
               " \n"
               "audio_input_rate = game input rate * display \n"
               "refresh rate / game refresh rate\n"
               " \n"
               "If the implementation does not report any \n"
               "values, NTSC defaults will be assumed for \n"
               "compatibility.\n"
               " \n"
               "This value should stay close to 60Hz to avoid \n"
               "large pitch changes. If your monitor does \n"
               "not run at 60Hz, or something close to it, \n"
               "disable VSync, and leave this at its default.");
         break;
      case MENU_ENUM_LABEL_VIDEO_ROTATION:
         snprintf(s, len,
               "Forces a certain rotation \n"
               "of the screen.\n"
               " \n"
               "The rotation is added to rotations which\n"
               "the libretro core sets (see Video Allow\n"
               "Rotate).");
         break;
      case MENU_ENUM_LABEL_VIDEO_SCALE:
         snprintf(s, len,
               "全屏分辨率.\n"
               " \n"
               "Resolution of 0 uses the \n"
               "resolution of the environment.\n");
         break;
      case MENU_ENUM_LABEL_FASTFORWARD_RATIO:
         snprintf(s, len,
               "快进比率."
               " \n"
               "The maximum rate at which content will\n"
               "be run when using fast forward.\n"
               " \n"
               " (E.g. 5.0 for 60 fps content => 300 fps \n"
               "cap).\n"
               " \n"
               "RetroArch will go to sleep to ensure that \n"
               "the maximum rate will not be exceeded.\n"
               "Do not rely on this cap to be perfectly \n"
               "accurate.");
         break;
      case MENU_ENUM_LABEL_VIDEO_MONITOR_INDEX:
         snprintf(s, len,
               "指定输出显示器.\n"
               " \n"
               "0 (default) means no particular monitor \n"
               "is preferred, 1 and up (1 being first \n"
               "monitor), suggests RetroArch to use that \n"
               "particular monitor.");
         break;
      case MENU_ENUM_LABEL_VIDEO_CROP_OVERSCAN:
         snprintf(s, len,
               "Forces cropping of overscanned \n"
               "frames.\n"
               " \n"
               "Exact behavior of this option is \n"
               "core-implementation specific.");
         break;
      case MENU_ENUM_LABEL_VIDEO_SCALE_INTEGER:
         snprintf(s, len,
               "Only scales video in integer \n"
               "steps.\n"
               " \n"
               "The base size depends on system-reported \n"
               "geometry and aspect ratio.\n"
               " \n"
               "If Force Aspect is not set, X/Y will be \n"
               "integer scaled independently.");
         break;
      case MENU_ENUM_LABEL_AUDIO_VOLUME:
         snprintf(s, len,
               "Audio volume, expressed in dB.\n"
               " \n"
               " 0 dB is normal volume. No gain will be applied.\n"
               "Gain can be controlled in runtime with Input\n"
               "Volume Up / Input Volume Down.");
         break;
      case MENU_ENUM_LABEL_AUDIO_RATE_CONTROL_DELTA:
         snprintf(s, len,
               "Audio rate control.\n"
               " \n"
               "Setting this to 0 disables rate control.\n"
               "Any other value controls audio rate control \n"
               "delta.\n"
               " \n"
               "Defines how much input rate can be adjusted \n"
               "dynamically.\n"
               " \n"
               " Input rate is defined as: \n"
               " input rate * (1.0 +/- (rate control delta))");
         break;
      case MENU_ENUM_LABEL_AUDIO_MAX_TIMING_SKEW:
         snprintf(s, len,
               "Maximum audio timing skew.\n"
               " \n"
               "Defines the maximum change in input rate.\n"
               "You may want to increase this to enable\n"
               "very large changes in timing, for example\n"
               "running PAL cores on NTSC displays, at the\n"
               "cost of inaccurate audio pitch.\n"
               " \n"
               " Input rate is defined as: \n"
               " input rate * (1.0 +/- (max timing skew))");
         break;
      case MENU_ENUM_LABEL_OVERLAY_NEXT:
         snprintf(s, len,
               "Toggles to next overlay.\n"
               " \n"
               "Wraps around.");
         break;
      case MENU_ENUM_LABEL_LOG_VERBOSITY:
         snprintf(s, len,
               "Enable or disable verbosity level \n"
               "of frontend.");
         break;
      case MENU_ENUM_LABEL_VOLUME_UP:
         snprintf(s, len,
               "调高音量.");
         break;
      case MENU_ENUM_LABEL_VOLUME_DOWN:
         snprintf(s, len,
               "降低音量.");
         break;
      case MENU_ENUM_LABEL_VIDEO_DISABLE_COMPOSITION:
         snprintf(s, len,
               "Forcibly disable composition.\n"
               "Only valid on Windows Vista/7 for now.");
         break;
      case MENU_ENUM_LABEL_PERFCNT_ENABLE:
         snprintf(s, len,
               "启用或关闭前端 \n"
               "性能计数.");
         break;
      case MENU_ENUM_LABEL_SYSTEM_DIRECTORY:
         snprintf(s, len,
               "系统目录. \n"
               " \n"
               "Sets the 'system' directory.\n"
               "Cores can query for this\n"
               "directory to load BIOSes, \n"
               "system-specific configs, etc.");
         break;
      case MENU_ENUM_LABEL_SAVESTATE_AUTO_SAVE:
      case MENU_ENUM_LABEL_SAVESTATE_AUTO_LOAD:
         snprintf(s, len,
               "Automatically saves a savestate at the \n"
               "end of RetroArch's lifetime.\n"
               " \n"
               "RetroArch will automatically load any savestate\n"
               "with this path on startup if 'Auto Load State\n"
               "is enabled.");
         break;
      case MENU_ENUM_LABEL_VIDEO_THREADED:
         snprintf(s, len,
               "Use threaded video driver.\n"
               " \n"
               "Using this might improve performance at the \n"
               "possible cost of latency and more video \n"
               "stuttering.");
         break;
      case MENU_ENUM_LABEL_VIDEO_VSYNC:
         snprintf(s, len,
               "视频垂直同步.\n");
         break;
      case MENU_ENUM_LABEL_VIDEO_HARD_SYNC:
         snprintf(s, len,
               "尝试硬件同步 \n"
               "CPU和GPU.\n"
               " \n"
               "可以降低潜在的性能 \n"
               "开销.");
         break;
      case MENU_ENUM_LABEL_REWIND_GRANULARITY:
         snprintf(s, len,
               "Rewind granularity.\n"
               " \n"
               " When rewinding defined number of \n"
               "frames, you can rewind several frames \n"
               "at a time, increasing the rewinding \n"
               "speed.");
         break;
      case MENU_ENUM_LABEL_SCREENSHOT:
         snprintf(s, len,
               "Take screenshot.");
         break;
      case MENU_ENUM_LABEL_VIDEO_FRAME_DELAY:
         snprintf(s, len,
               "Sets how many milliseconds to delay\n"
               "after VSync before running the core.\n"
               "\n"
               "Can reduce latency at the cost of\n"
               "higher risk of stuttering.\n"
               " \n"
               "Maximum is 15.");
         break;
      case MENU_ENUM_LABEL_VIDEO_HARD_SYNC_FRAMES:
         snprintf(s, len,
               "Sets how many frames CPU can \n"
               "run ahead of GPU when using 'GPU \n"
               "Hard Sync'.\n"
               " \n"
               "Maximum is 3.\n"
               " \n"
               " 0: Syncs to GPU immediately.\n"
               " 1: Syncs to previous frame.\n"
               " 2: Etc ...");
         break;
      case MENU_ENUM_LABEL_VIDEO_BLACK_FRAME_INSERTION:
         snprintf(s, len,
               "Inserts a black frame inbetween \n"
               "frames.\n"
               " \n"
               "Useful for 120 Hz monitors who want to \n"
               "play 60 Hz material with eliminated \n"
               "ghosting.\n"
               " \n"
               "Video refresh rate should still be \n"
               "configured as if it is a 60 Hz monitor \n"
               "(divide refresh rate by 2).");
         break;
      case MENU_ENUM_LABEL_RGUI_SHOW_START_SCREEN:
         snprintf(s, len,
               "Show startup screen in menu.\n"
               "Is automatically set to false when seen\n"
               "for the first time.\n"
               " \n"
               "This is only updated in config if\n"
               "'Save Configuration on Exit' is enabled.\n");
         break;
      case MENU_ENUM_LABEL_VIDEO_FULLSCREEN:
         snprintf(s, len, "Toggles fullscreen.");
         break;
      case MENU_ENUM_LABEL_BLOCK_SRAM_OVERWRITE:
         snprintf(s, len,
               "Block SRAM from being overwritten \n"
               "when loading save states.\n"
               " \n"
               "Might potentially lead to buggy games.");
         break;
      case MENU_ENUM_LABEL_PAUSE_NONACTIVE:
         snprintf(s, len,
               "Pause gameplay when window focus \n"
               "is lost.");
         break;
      case MENU_ENUM_LABEL_VIDEO_GPU_SCREENSHOT:
         snprintf(s, len,
               "Screenshots output of GPU shaded \n"
               "material if available.");
         break;
      case MENU_ENUM_LABEL_SCREENSHOT_DIRECTORY:
         snprintf(s, len,
               "截图目录 \n"
               " \n"
               "用于保存截图的文件夹。"
               );
         break;
      case MENU_ENUM_LABEL_VIDEO_SWAP_INTERVAL:
         snprintf(s, len,
               "VSync Swap Interval.\n"
               " \n"
               "Uses a custom swap interval for VSync. Set this \n"
               "to effectively halve monitor refresh rate.");
         break;
      case MENU_ENUM_LABEL_SAVEFILE_DIRECTORY:
         snprintf(s, len,
               "游戏存盘目录. \n"
               " \n"
               "Save all save files (*.srm) to this \n"
               "directory. This includes related files like \n"
               ".bsv, .rt, .psrm, etc...\n"
               " \n"
               "This will be overridden by explicit command line\n"
               "options.");
         break;
      case MENU_ENUM_LABEL_SAVESTATE_DIRECTORY:
         snprintf(s, len,
               "即时存档目录. \n"
               " \n"
               "Save all save states (*.state) to this \n"
               "directory.\n"
               " \n"
               "This will be overridden by explicit command line\n"
               "options.");
         break;
      case MENU_ENUM_LABEL_ASSETS_DIRECTORY:
         snprintf(s, len,
               "Assets Directory. \n"
               " \n"
               " This location is queried by default when \n"
               "menu interfaces try to look for loadable \n"
               "assets, etc.");
         break;
      case MENU_ENUM_LABEL_DYNAMIC_WALLPAPERS_DIRECTORY:
         snprintf(s, len,
               "动态壁纸目录 \n"
               " \n"
               "保存用于主界面的、依据游戏内容变化的动态壁纸。");
         break;
      case MENU_ENUM_LABEL_SLOWMOTION_RATIO:
         snprintf(s, len,
               "Slowmotion ratio."
               " \n"
               "When slowmotion, content will slow\n"
               "down by factor.");
         break;
      case MENU_ENUM_LABEL_INPUT_AXIS_THRESHOLD:
         snprintf(s, len,
               "Defines axis threshold.\n"
               " \n"
               "How far an axis must be tilted to result\n"
               "in a button press.\n"
               " Possible values are [0.0, 1.0].");
         break;
      case MENU_ENUM_LABEL_INPUT_TURBO_PERIOD:
         snprintf(s, len,
               "Turbo period.\n"
               " \n"
               "Describes the period of which turbo-enabled\n"
               "buttons toggle.\n"
               " \n"
               "Numbers are described in frames."
               );
         break;
      case MENU_ENUM_LABEL_INPUT_DUTY_CYCLE:
         snprintf(s, len,
               "Duty cycle.\n"
               " \n"
               "Describes how long the period of a turbo-enabled\n"
               "should be.\n"
               " \n"
               "Numbers are described in frames."
               );
         break;
      case MENU_ENUM_LABEL_INPUT_TOUCH_ENABLE:
         snprintf(s, len, "Enable touch support.");
         break;
      case MENU_ENUM_LABEL_INPUT_PREFER_FRONT_TOUCH:
         snprintf(s, len, "Use front instead of back touch.");
         break;
      case MENU_ENUM_LABEL_MOUSE_ENABLE:
         snprintf(s, len, "Enable mouse input inside the menu.");
         break;
      case MENU_ENUM_LABEL_POINTER_ENABLE:
         snprintf(s, len, "Enable touch input inside the menu.");
         break;
      case MENU_ENUM_LABEL_MENU_WALLPAPER:
         snprintf(s, len, "Path to an image to set as menu wallpaper.");
         break;
      case MENU_ENUM_LABEL_NAVIGATION_WRAPAROUND:
         snprintf(s, len,
               "Wrap-around to beginning and/or end \n"
               "if boundary of list is reached \n"
               "horizontally and/or vertically.");
         break;
      case MENU_ENUM_LABEL_PAUSE_LIBRETRO:
         snprintf(s, len,
               "If disabled, the libretro core will keep \n"
               "running in the background when we are in the \n"
               "menu.");
         break;
      case MENU_ENUM_LABEL_SUSPEND_SCREENSAVER_ENABLE:
         snprintf(s, len,
               "Suspends the screensaver. Is a hint that \n"
               "does not necessarily have to be \n"
               "honored by the video driver.");
         break;
      case MENU_ENUM_LABEL_NETPLAY_MODE:
         snprintf(s, len,
               "Netplay client mode for the current user. \n"
               "Will be 'Server' mode if disabled.");
         break;
      case MENU_ENUM_LABEL_NETPLAY_DELAY_FRAMES:
         snprintf(s, len,
               "The amount of delay frames to use for netplay. \n"
               " \n"
               "Increasing this value will increase \n"
               "performance, but introduce more latency.");
         break;
      case MENU_ENUM_LABEL_NETPLAY_CHECK_FRAMES:
         snprintf(s, len,
               "The frequency in frames with which netplay \n"
               "will verify that the host and client are in \n"
               "sync. \n"
               " \n"
               "With most cores, this value will have no \n"
               "visible effect and can be ignored. With \n"
               "nondeterminstic cores, this value determines \n"
               "how often the netplay peers will be brought \n"
               "into sync. With buggy cores, setting this \n"
               "to any non-zero value will cause severe \n"
               "performance issues. Set to zero to perform \n"
               "no checks. This value is only used on the \n"
               "netplay host. \n");
         break;
      case MENU_ENUM_LABEL_VIDEO_MAX_SWAPCHAIN_IMAGES:
         snprintf(s, len,
               "Maximum amount of swapchain images. This \n"
               "can tell the video driver to use a specific \n"
               "video buffering mode. \n"
               " \n"
               "Single buffering - 1\n"
               "Double buffering - 2\n"
               "Triple buffering - 3\n"
               " \n"
               "Setting the right buffering mode can have \n"
               "a big impact on latency.");
         break;
      case MENU_ENUM_LABEL_VIDEO_SMOOTH:
         snprintf(s, len,
               "Smoothens picture with bilinear filtering. \n"
               "Should be disabled if using shaders.");
         break;
      case MENU_ENUM_LABEL_TIMEDATE_ENABLE:
         snprintf(s, len,
               "Shows current date and/or time inside menu.");
         break;
      case MENU_ENUM_LABEL_CORE_ENABLE:
         snprintf(s, len,
               "Shows current core inside menu.");
         break;
      case MENU_ENUM_LABEL_NETPLAY_ENABLE_HOST:
         snprintf(s, len,
               "Enables Netplay in host (server) mode.");
         break;
      case MENU_ENUM_LABEL_NETPLAY_ENABLE_CLIENT:
         snprintf(s, len,
               "Enables Netplay in client mode.");
         break;
      case MENU_ENUM_LABEL_NETPLAY_DISCONNECT:
         snprintf(s, len,
               "Disconnects an active Netplay connection.");
         break;
      case MENU_ENUM_LABEL_NETPLAY_SETTINGS:
         snprintf(s, len,
               "Setting related to Netplay.");
         break;
      case MENU_ENUM_LABEL_NETPLAY_LAN_SCAN_SETTINGS:
         snprintf(s, len,
               "Search for and connect to netplay hosts on the local network.");
         break;
      case MENU_ENUM_LABEL_DYNAMIC_WALLPAPER:
         snprintf(s, len,
               "Dynamically load a new wallpaper \n"
               "depending on context.");
         break;
      case MENU_ENUM_LABEL_CORE_UPDATER_BUILDBOT_URL:
         snprintf(s, len,
               "URL to core updater directory on the \n"
               "Libretro buildbot.");
         break;
      case MENU_ENUM_LABEL_BUILDBOT_ASSETS_URL:
         snprintf(s, len,
               "URL to assets updater directory on the \n"
               "Libretro buildbot.");
         break;
      case MENU_ENUM_LABEL_INPUT_REMAP_BINDS_ENABLE:
         snprintf(s, len,
               "if enabled, overrides the input binds \n"
               "with the remapped binds set for the \n"
               "current core.");
         break;
      case MENU_ENUM_LABEL_OVERLAY_DIRECTORY:
         snprintf(s, len,
               "Overlay Directory. \n"
               " \n"
               "Defines a directory where overlays are \n"
               "kept for easy access.");
         break;
      case MENU_ENUM_LABEL_INPUT_MAX_USERS:
         snprintf(s, len,
               "Maximum amount of users supported by \n"
               "RetroArch.");
         break;
      case MENU_ENUM_LABEL_CORE_UPDATER_AUTO_EXTRACT_ARCHIVE:
         snprintf(s, len,
               "After downloading, automatically extract \n"
               "archives that the downloads are contained \n"
               "inside.");
         break;
      case MENU_ENUM_LABEL_NAVIGATION_BROWSER_FILTER_SUPPORTED_EXTENSIONS_ENABLE:
         snprintf(s, len,
               "Filter files being shown by \n"
               "supported extensions.");
         break;
      case MENU_ENUM_LABEL_NETPLAY_NICKNAME:
         snprintf(s, len,
               "The username of the person running RetroArch. \n"
               "This will be used for playing online games.");
         break;
      case MENU_ENUM_LABEL_NETPLAY_CLIENT_SWAP_INPUT:
         snprintf(s, len,
               "When being client over netplay, use \n"
               "keybinds for player 1.");
         break;
      case MENU_ENUM_LABEL_NETPLAY_TCP_UDP_PORT:
         snprintf(s, len,
               "The port of the host IP address. \n"
               "Can be either a TCP or UDP port.");
         break;
      case MENU_ENUM_LABEL_NETPLAY_SPECTATOR_MODE_ENABLE:
         snprintf(s, len,
               "Enable or disable spectator mode for \n"
               "the user during netplay.");
         break;
      case MENU_ENUM_LABEL_NETPLAY_IP_ADDRESS:
         snprintf(s, len,
               "The address of the host to connect to.");
         break;
      case MENU_ENUM_LABEL_STDIN_CMD_ENABLE:
         snprintf(s, len,
               "Enable stdin command interface.");
         break;
      case MENU_ENUM_LABEL_UI_COMPANION_START_ON_BOOT:
         snprintf(s, len,
               "Start User Interface companion driver \n"
               "on boot (if available).");
         break;
      case MENU_ENUM_LABEL_MENU_DRIVER:
         snprintf(s, len, "Menu driver to use.");
         break;
      case MENU_ENUM_LABEL_INPUT_MENU_ENUM_TOGGLE_GAMEPAD_COMBO:
         snprintf(s, len,
               "Gamepad button combination to toggle menu. \n"
               " \n"
               "0 - None \n"
               "1 - Press L + R + Y + D-Pad Down \n"
               "simultaneously. \n"
               "2 - Press L3 + R3 simultaneously. \n"
               "3 - Press Start + Select simultaneously.");
         break;
      case MENU_ENUM_LABEL_INPUT_ALL_USERS_CONTROL_MENU:
         snprintf(s, len, "Allow any RetroPad to control the menu.");
         break;
      case MENU_ENUM_LABEL_INPUT_AUTODETECT_ENABLE:
         snprintf(s, len,
               "Enable input auto-detection.\n"
               " \n"
               "Will attempt to auto-configure \n"
               "joypads, Plug-and-Play style.");
         break;
      case MENU_ENUM_LABEL_CAMERA_ALLOW:
         snprintf(s, len,
               "Allow or disallow camera access by \n"
               "cores.");
         break;
      case MENU_ENUM_LABEL_LOCATION_ALLOW:
         snprintf(s, len,
               "Allow or disallow location services \n"
               "access by cores.");
         break;
      case MENU_ENUM_LABEL_TURBO:
         snprintf(s, len,
               "Turbo enable.\n"
               " \n"
               "Holding the turbo while pressing another \n"
               "button will let the button enter a turbo \n"
               "mode where the button state is modulated \n"
               "with a periodic signal. \n"
               " \n"
               "The modulation stops when the button \n"
               "itself (not turbo button) is released.");
         break;
      case MENU_ENUM_LABEL_OSK_ENABLE:
         snprintf(s, len,
               "Enable/disable on-screen keyboard.");
         break;
      case MENU_ENUM_LABEL_AUDIO_MUTE:
         snprintf(s, len,
               "Mute/unmute audio.");
         break;
      case MENU_ENUM_LABEL_REWIND:
         snprintf(s, len,
               "Hold button down to rewind.\n"
               " \n"
               "Rewind must be enabled.");
         break;
      case MENU_ENUM_LABEL_EXIT_EMULATOR:
         snprintf(s, len,
               "Key to exit RetroArch cleanly."
#if !defined(RARCH_MOBILE) && !defined(RARCH_CONSOLE)
               "\nKilling it in any hard way (SIGKILL, \n"
               "etc) will terminate without saving\n"
               "RAM, etc. On Unix-likes,\n"
               "SIGINT/SIGTERM allows\n"
               "a clean deinitialization."
#endif
               );
         break;
      case MENU_ENUM_LABEL_LOAD_STATE:
         snprintf(s, len,
               "Loads state.");
         break;
      case MENU_ENUM_LABEL_SAVE_STATE:
         snprintf(s, len,
               "Saves state.");
         break;
      case MENU_ENUM_LABEL_NETPLAY_FLIP_PLAYERS:
         snprintf(s, len,
               "Netplay flip users.");
         break;
      case MENU_ENUM_LABEL_CHEAT_INDEX_PLUS:
         snprintf(s, len,
               "Increment cheat index.\n");
         break;
      case MENU_ENUM_LABEL_CHEAT_INDEX_MINUS:
         snprintf(s, len,
               "Decrement cheat index.\n");
         break;
      case MENU_ENUM_LABEL_SHADER_PREV:
         snprintf(s, len,
               "Applies previous shader in directory.");
         break;
      case MENU_ENUM_LABEL_SHADER_NEXT:
         snprintf(s, len,
               "Applies next shader in directory.");
         break;
      case MENU_ENUM_LABEL_RESET:
         snprintf(s, len,
               "Reset the content.\n");
         break;
      case MENU_ENUM_LABEL_PAUSE_TOGGLE:
         snprintf(s, len,
               "Toggle between paused and non-paused state.");
         break;
      case MENU_ENUM_LABEL_CHEAT_TOGGLE:
         snprintf(s, len,
               "打开金手指索引.\n");
         break;
      case MENU_ENUM_LABEL_HOLD_FAST_FORWARD:
         snprintf(s, len,
               "Hold for fast-forward. Releasing button \n"
               "disables fast-forward.");
         break;
      case MENU_ENUM_LABEL_SLOWMOTION:
         snprintf(s, len,
               "Hold for slowmotion.");
         break;
      case MENU_ENUM_LABEL_FRAME_ADVANCE:
         snprintf(s, len,
               "Frame advance when content is paused.");
         break;
      case MENU_ENUM_LABEL_MOVIE_RECORD_TOGGLE:
         snprintf(s, len,
               "Toggle between recording and not.");
         break;
      case MENU_ENUM_LABEL_L_X_PLUS:
      case MENU_ENUM_LABEL_L_X_MINUS:
      case MENU_ENUM_LABEL_L_Y_PLUS:
      case MENU_ENUM_LABEL_L_Y_MINUS:
      case MENU_ENUM_LABEL_R_X_PLUS:
      case MENU_ENUM_LABEL_R_X_MINUS:
      case MENU_ENUM_LABEL_R_Y_PLUS:
      case MENU_ENUM_LABEL_R_Y_MINUS:
         snprintf(s, len,
               "Axis for analog stick (DualShock-esque).\n"
               " \n"
               "Bound as usual, however, if a real analog \n"
               "axis is bound, it can be read as a true analog.\n"
               " \n"
               "Positive X axis is right. \n"
               "Positive Y axis is down.");
         break;
      case MENU_ENUM_LABEL_VALUE_WHAT_IS_A_CORE_DESC:
         snprintf(s, len,
               "RetroArch本身并不能做什么事情。 \n"
               " \n"
               "如果想在上面干点什么，你需要向它加载一个程 \n"
               "序。 \n"
               "\n"
               "我们把这样的程序叫做“Libretro核心”，简单 \n"
               "的称呼其为“核心”。 \n"
               " \n"
               "你可以从“加载核心”菜单中选择一个核心。 \n"
               " \n"
#ifdef HAVE_NETWORKING
               "你可以通过以下几种方法来获取核心: \n"
               "一、通过访问菜单项「%s」 \n"
               " -> 「%s」来下载；\n"
               "二、手动将其移入核心目录中，访问目录设置 \n"
               "找到你的“%s”。",
               msg_hash_to_str(MENU_ENUM_LABEL_VALUE_ONLINE_UPDATER),
               msg_hash_to_str(MENU_ENUM_LABEL_VALUE_CORE_UPDATER_LIST),
               msg_hash_to_str(MENU_ENUM_LABEL_VALUE_LIBRETRO_DIR_PATH)
#else
               "你可以通过手动将核心移入目录中来添加他 \n"
               "们，访问目录设置找到你的“%s”。",
               msg_hash_to_str(MENU_ENUM_LABEL_VALUE_LIBRETRO_DIR_PATH)
#endif
               );
         break;
      case MENU_ENUM_LABEL_VALUE_HELP_CHANGE_VIRTUAL_GAMEPAD_DESC:
         snprintf(s, len,
               "You can change the virtual gamepad overlay\n"
               "by going to '%s' -> '%s'."
               " \n"
               "From there you can change the overlay,\n"
               "change the size and opacity of the buttons, etc.\n"
               " \n"
               "NOTE: By default, virtual gamepad overlays are\n"
               "hidden when in the menu.\n"
               "If you'd like to change this behavior,\n"
               "you can set '%s' to false.",
               msg_hash_to_str(MENU_ENUM_LABEL_VALUE_SETTINGS),
               msg_hash_to_str(MENU_ENUM_LABEL_VALUE_OVERLAY_SETTINGS),
               msg_hash_to_str(MENU_ENUM_LABEL_VALUE_INPUT_OVERLAY_HIDE_IN_MENU)
               );
         break;
      default:
         if (string_is_empty(s))
            strlcpy(s, msg_hash_to_str(MENU_ENUM_LABEL_VALUE_NO_INFORMATION_AVAILABLE), len);
         return -1;
   }

   return 0;
}

const char *msg_hash_to_str_chs(enum msg_hash_enums msg)
{
   switch (msg)
   {
      MSG_HASH(
              MSG_COMPILER,
              "编译器"
      )
      MSG_HASH(
              MSG_UNKNOWN_COMPILER,
              "未知的编译器"
      )
      MSG_HASH(
              MSG_DEVICE_DISCONNECTED_FROM_PORT,
              "设备已从端口上断开"
      )
      MSG_HASH(
              MSG_UNKNOWN_NETPLAY_COMMAND_RECEIVED,
              "接收到未知的联机游戏指令"
      )
      MSG_HASH(
              MSG_FILE_ALREADY_EXISTS_SAVING_TO_BACKUP_BUFFER,
              "文件已存在。保存到备份缓冲区"
      )
      MSG_HASH(
              MSG_GOT_CONNECTION_FROM,
              "连接来自： \"%s\""
      )
      MSG_HASH(
              MSG_GOT_CONNECTION_FROM_NAME,
              "连接来自： \"%s (%s)\""
      )
      MSG_HASH(
              MSG_PUBLIC_ADDRESS,
              "公开地址"
      )
      MSG_HASH(
              MSG_NO_ARGUMENTS_SUPPLIED_AND_NO_MENU_BUILTIN,
              "未提供参数也没有内建菜单，显示帮助..."
      )
      MSG_HASH(
              MSG_NETPLAY_USERS_HAS_FLIPPED,
              "联机游戏用户已被踢出"
      )
      MSG_HASH(
              MSG_SETTING_DISK_IN_TRAY,
              "Setting disk in tray"
      )
      MSG_HASH(
              MSG_WAITING_FOR_CLIENT,
              "等待客户端 ..."
      )
      MSG_HASH(
              MSG_NETPLAY_YOU_HAVE_LEFT_THE_GAME,
              "你已离开游戏"
      )
      MSG_HASH(
              MSG_NETPLAY_YOU_HAVE_JOINED_AS_PLAYER_N,
              "你已作为玩家 %d 加入"
      )
      MSG_HASH(
              MSG_NETPLAY_IMPLEMENTATIONS_DIFFER,
              "实现有差异。确保正在使用的RetroArch和核心是同版本的。"
      )
      MSG_HASH(
              MSG_NETPLAY_ENDIAN_DEPENDENT,
              "This core does not support inter-architecture netplay between these systems"
      )
      MSG_HASH(
              MSG_NETPLAY_PLATFORM_DEPENDENT,
              "This core does not support inter-architecture netplay"
      )
      MSG_HASH(
              MSG_NETPLAY_ENTER_PASSWORD,
              "输入联机游戏服务器的密码："
      )
      MSG_HASH(
              MSG_NETPLAY_INCORRECT_PASSWORD,
              "密码错误"
      )
      MSG_HASH(
              MSG_NETPLAY_SERVER_NAMED_HANGUP,
              "\"%s\" 已断开连接"
      )
      MSG_HASH(
              MSG_NETPLAY_SERVER_HANGUP,
              "一个联机游戏客户端已断开"
      )
      MSG_HASH(
              MSG_NETPLAY_CLIENT_HANGUP,
              "联机游戏已断开"
      )
      MSG_HASH(
              MSG_NETPLAY_CANNOT_PLAY_UNPRIVILEGED,
              "你没有游戏权限"
      )
      MSG_HASH(
              MSG_NETPLAY_CANNOT_PLAY_NO_SLOTS,
              "已无空闲插槽" /*FIXME:"There are no free player slots"*/
      )
      MSG_HASH(
              MSG_NETPLAY_CANNOT_PLAY,
              "无法切换到游戏模式"
      )
      MSG_HASH(
              MSG_NETPLAY_PEER_PAUSED,
              "联机游戏对方 \"%s\" 暂停"
      )
      MSG_HASH(
              MSG_NETPLAY_CHANGED_NICK,
              "你的昵称已修改为 \"%s\""
      )
      MSG_HASH(
              MENU_ENUM_SUBLABEL_VIDEO_SHARED_CONTEXT,
              "Give hardware-rendered cores their own private context. Avoids having to assume hardware state changes inbetween frames."
      )
      MSG_HASH(
              MENU_ENUM_SUBLABEL_MENU_SETTINGS,
              "调整菜单屏幕相关的设置。"
      )
      MSG_HASH(
              MENU_ENUM_SUBLABEL_VIDEO_HARD_SYNC,
              "强制同步CPU和GPU，以性能为代价换取低延迟。"
      )
      MSG_HASH(
              MENU_ENUM_SUBLABEL_VIDEO_THREADED,
              "以延迟和视频撕裂为代价换取高性能，当且仅当能\n"
                      "达到全速模拟时使用。"
      )
      MSG_HASH(
              MSG_AUDIO_VOLUME,
              "音频音量"
      )
      MSG_HASH(
              MSG_AUTODETECT,
              "自动检测"
      )
      MSG_HASH(
              MSG_AUTOLOADING_SAVESTATE_FROM,
              "自动加载存档从"
      )
      MSG_HASH(
              MSG_CAPABILITIES,
              "容量"
      )
      MSG_HASH(
              MSG_CONNECTING_TO_NETPLAY_HOST,
              "连接到联机游戏主机"
      )
      MSG_HASH(
              MSG_CONNECTING_TO_PORT,
              "连接到端口"
      )
      MSG_HASH(
              MSG_CONNECTION_SLOT,
              "连接到插槽"
      )
      MSG_HASH(
              MSG_SORRY_UNIMPLEMENTED_CORES_DONT_DEMAND_CONTENT_NETPLAY,
              "对不起，未实现：核心未请求内容，无法加入联机游戏。"
      )
      MSG_HASH(
              MSG_FAILED_TO_SET_DISK,
              "设置磁盘失败")
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_ACCOUNTS_CHEEVOS_PASSWORD,
              "密码"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_ACCOUNTS_CHEEVOS_SETTINGS,
              "Cheevos账户" /*FIXME:"Accounts Cheevos"*/
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_ACCOUNTS_CHEEVOS_USERNAME,
              "用户名"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_ACCOUNTS_LIST,
              "账户"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_ACCOUNTS_LIST_END,
              "账户列表终端"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_ACCOUNTS_RETRO_ACHIEVEMENTS,
              "Retro 成就"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_ACHIEVEMENT_LIST,
              "成就列表"
      )

      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_ACHIEVEMENT_LIST_HARDCORE,
              "成就列表（硬核）" /*FIXME:"Achievement List (Hardcore)"*/
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_ADD_CONTENT_LIST,
              "添加游戏内容"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_CONFIGURATIONS_LIST,
              "配置"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_ADD_TAB,
              "导入游戏内容"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_NETPLAY_TAB,
              "联机游戏房间"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_ASK_ARCHIVE,
              "询问"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_ASSETS_DIRECTORY,
              "资源目录"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_AUDIO_BLOCK_FRAMES,
              "块帧" /*FIXME:"Block Frames"*/
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_AUDIO_DEVICE,
              "音频设备"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_AUDIO_DRIVER,
              "音频驱动"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_AUDIO_DSP_PLUGIN,
              "音频DSP插件"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_AUDIO_ENABLE,
              "启用音频"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_AUDIO_FILTER_DIR,
              "音频过滤器目录"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_TURBO_DEADZONE_LIST,
              "涡轮/盲区"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_AUDIO_LATENCY,
              "音频时延(ms)"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_AUDIO_MAX_TIMING_SKEW,
              "音频最大采样间隔"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_AUDIO_MUTE,
              "音频静音"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_AUDIO_OUTPUT_RATE,
              "音频输出码率(Hz)"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_AUDIO_RATE_CONTROL_DELTA,
              "音频码率控制间隔"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_AUDIO_RESAMPLER_DRIVER,
              "音频重采样驱动"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_AUDIO_SETTINGS,
              "音频"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_AUDIO_SYNC,
              "启用音频同步"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_AUDIO_VOLUME,
              "音频音量级别(dB)"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_AUTOSAVE_INTERVAL,
              "SaveRAM自动保存间隔"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_AUTO_OVERRIDES_ENABLE,
              "自动加载覆写文件"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_AUTO_REMAPS_ENABLE,
              "自动加载重映射文件"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_AUTO_SHADERS_ENABLE,
              "自动加载Shader预设"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_BASIC_MENU_CONTROLS_BACK,
              "返回"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_BASIC_MENU_CONTROLS_CONFIRM,
              "确认"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_BASIC_MENU_CONTROLS_INFO,
              "信息"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_BASIC_MENU_CONTROLS_QUIT,
              "退出"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_BASIC_MENU_CONTROLS_SCROLL_DOWN,
              "向下滚动"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_BASIC_MENU_CONTROLS_SCROLL_UP,
              "向上滚动"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_BASIC_MENU_CONTROLS_START,
              "开始"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_BASIC_MENU_CONTROLS_TOGGLE_KEYBOARD,
              "切换键盘"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_BASIC_MENU_CONTROLS_TOGGLE_MENU,
              "切换菜单"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_BASIC_MENU_ENUM_CONTROLS,
              "基本菜单控制"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_BASIC_MENU_ENUM_CONTROLS_CONFIRM,
              "确认"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_BASIC_MENU_ENUM_CONTROLS_INFO,
              "信息"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_BASIC_MENU_ENUM_CONTROLS_QUIT,
              "退出"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_BASIC_MENU_ENUM_CONTROLS_SCROLL_UP,
              "向上滚动"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_BASIC_MENU_ENUM_CONTROLS_START,
              "默认值"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_BASIC_MENU_ENUM_CONTROLS_TOGGLE_KEYBOARD,
              "切换键盘"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_BASIC_MENU_ENUM_CONTROLS_TOGGLE_MENU,
              "切换菜单"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_BLOCK_SRAM_OVERWRITE,
              "加载保存状态时不覆盖SaveRAM"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_BLUETOOTH_ENABLE,
              "启用蓝牙服务"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_BUILDBOT_ASSETS_URL,
              "构建机器人资源URL"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_CACHE_DIRECTORY,
              "缓存目录"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_CAMERA_ALLOW,
              "允许使用相机"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_CAMERA_DRIVER,
              "相机驱动"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_CHEAT,
              "金手指"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_CHEAT_APPLY_CHANGES,
              "应用金手指修改"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_CHEAT_DATABASE_PATH,
              "金手指文件目录"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_CHEAT_FILE,
              "金手指文件"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_CHEAT_FILE_LOAD,
              "加载金手指文件"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_CHEAT_FILE_SAVE_AS,
              "另存为金手指文件"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_CHEAT_NUM_PASSES,
              "金手指通过" /*FIXME: "Cheat Passes"*/
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_CHEEVOS_DESCRIPTION,
              "描述"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_CHEEVOS_HARDCORE_MODE_ENABLE,
/* FIXME? Translate 'Achievements Hardcore Mode' */
              "专家模式"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_CHEEVOS_LOCKED_ACHIEVEMENTS,
              "已解锁的成就:"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_CHEEVOS_LOCKED_ENTRY,
              "锁定"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_CHEEVOS_SETTINGS,
              "Retro 成就"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_CHEEVOS_TEST_UNOFFICIAL,
/* FIXME? Translate 'Test Unofficial Achievements' */
              "非官方测试"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_CHEEVOS_UNLOCKED_ACHIEVEMENTS,
              "未解锁的成就:"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_CHEEVOS_UNLOCKED_ENTRY,
              "未锁定"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_CLOSE_CONTENT,
              "关闭"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_CONFIG,
              "配置"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_CONFIGURATIONS,
              "加载配置"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_CONFIGURATION_SETTINGS,
              "配置"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_CONFIG_SAVE_ON_EXIT,
              "退出时保存配置"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_CONFIRM_ON_EXIT,
              "退出时进行询问"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_CONTENT_COLLECTION_LIST,
              "收藏"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_CONTENT_DATABASE_DIRECTORY,
              "游戏内容数据库目录"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_CONTENT_DIR,
              "游戏内容目录"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_CONTENT_HISTORY_SIZE,
              "历史记录数量")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_PLAYLIST_ENTRY_REMOVE,
               "允许移除记录")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CONTENT_SETTINGS,
               "快捷菜单")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_ASSETS_DIR,
               "核心资源目录")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_ASSETS_DIRECTORY,
               "下载目录")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_CHEAT_OPTIONS,
               "金手指")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_COUNTERS,
               "核心计数器")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_ENABLE,
               "显示核心名称")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_INFORMATION,
               "核心信息")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_INFO_AUTHORS,
               "作者")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_INFO_CATEGORIES,
               "分类")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_INFO_CORE_LABEL,
               "核心标签")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_INFO_CORE_NAME,
               "核心名称")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_INFO_FIRMWARE,
               "固件")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_INFO_LICENSES,
               "许可证")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_INFO_PERMISSIONS,
               "许可")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_INFO_SUPPORTED_EXTENSIONS,
               "支持的扩展")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_INFO_SYSTEM_MANUFACTURER,
               "系统制造商")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_INFO_SYSTEM_NAME,
               "系统名称")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_INPUT_REMAPPING_OPTIONS,
               "控制")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_LIST,
               "加载核心")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_OPTIONS,
               "选项")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_SETTINGS,
               "核心")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_SET_SUPPORTS_NO_CONTENT_ENABLE,
               "自动启动一个核心")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_UPDATER_AUTO_EXTRACT_ARCHIVE,
               "自动解压下载的档案")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_UPDATER_BUILDBOT_URL,
               "构建机器人核心URL")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_UPDATER_LIST,
               "核心更新程序")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CORE_UPDATER_SETTINGS,
               "更新程序")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CPU_ARCHITECTURE,
               "CPU架构:")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CPU_CORES,
               "CPU核心:")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CURSOR_DIRECTORY,
               "指针目录")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CURSOR_MANAGER,
               "光标管理器")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CUSTOM_RATIO,
               "自定义比率")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DATABASE_MANAGER,
               "数据库管理器")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DATABASE_SELECTION,
               "选择数据库")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DELETE_ENTRY,
               "移除")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_FAVORITES,
               "选择文件并探测核心") /* TODO/FIXME - update */
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DIRECTORY_CONTENT,
               "<游戏内容目录>")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DIRECTORY_DEFAULT,
               "<默认>")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DIRECTORY_NONE,
               "<无>")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DIRECTORY_NOT_FOUND,
               "没有找到文件夹。")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DIRECTORY_SETTINGS,
               "目录")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DISK_CYCLE_TRAY_STATUS,
               "Disk Cycle Tray Status")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DISK_IMAGE_APPEND,
               "追加光盘镜像")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DISK_INDEX,
               "光盘索引")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DISK_OPTIONS,
               "光盘控制")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DONT_CARE,
               "不关心")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DOWNLOADED_FILE_DETECT_CORE_LIST,
               "下载目录")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DOWNLOAD_CORE,
               "下载核心……")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DOWNLOAD_CORE_CONTENT,
               "下载游戏内容")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DPI_OVERRIDE_ENABLE,
               "启用DPI覆盖")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DPI_OVERRIDE_VALUE,
               "DPI覆盖")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DRIVER_SETTINGS,
               "驱动")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DUMMY_ON_CORE_SHUTDOWN,
/* FIXME? Translate 'Load Dummy on Core Shutdown' */
               "核心关闭时加载虚拟程序")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CHECK_FOR_MISSING_FIRMWARE,
               "加载前检查丢失的固件") /*FIXME: "Check for Missing Firmware Before Loading"*/
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DYNAMIC_WALLPAPER,
               "动态壁纸")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DYNAMIC_WALLPAPERS_DIRECTORY,
               "动态壁纸目录")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_CHEEVOS_ENABLE,
/* FIXME? Translate 'Enable Achievements' */
               "启用")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_ENTRY_HOVER_COLOR,
               "菜单项悬停颜色")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_ENTRY_NORMAL_COLOR,
               "菜单项正常颜色")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_FALSE,
               "假")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_FASTFORWARD_RATIO,
               "最大运行速度")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_FPS_SHOW,
               "显示帧率")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_FRAME_THROTTLE_ENABLE,
               "限制最大运行速度")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_FRAME_THROTTLE_SETTINGS,
               "帧率限制")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_FRONTEND_COUNTERS,
               "前端计数器")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_GAME_SPECIFIC_OPTIONS,
               "自动加载游戏内容特定的核心选项")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_GAME_SPECIFIC_OPTIONS_CREATE,
               "创建游戏选项文件")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_GAME_SPECIFIC_OPTIONS_IN_USE,
               "游戏选项文件")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_HELP,
               "帮助")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_HELP_AUDIO_VIDEO_TROUBLESHOOTING,
               "音频/视频故障排除")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_HELP_CHANGE_VIRTUAL_GAMEPAD,
               "变更虚拟游戏控制器覆层")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_HELP_CONTROLS,
               "基本菜单控制")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_HELP_LIST,
               "帮助")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_HELP_LOADING_CONTENT,
               "加载游戏内容")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_HELP_SCANNING_CONTENT,
               "扫描游戏内容")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_HELP_WHAT_IS_A_CORE,
               "什么是“核心”？")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_HISTORY_LIST_ENABLE,
               "启用历史记录")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_HISTORY_TAB,
               "历史")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_HORIZONTAL_MENU,
               "水平化菜单")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_IMAGES_TAB,
               "图像")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INFORMATION,
               "信息")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INFORMATION_LIST,
               "信息")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_ADC_TYPE,
               "手柄输入转数字选项")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_ALL_USERS_CONTROL_MENU,
               "所有用户都能控制菜单")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_ANALOG_LEFT_X,
               "左摇杆X")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_ANALOG_LEFT_X_MINUS,
               "左摇杆X- (左)")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_ANALOG_LEFT_X_PLUS,
               "左摇杆X+ (右)")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_ANALOG_LEFT_Y,
               "左摇杆Y")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_ANALOG_LEFT_Y_MINUS,
               "左摇杆Y- (上)")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_ANALOG_LEFT_Y_PLUS,
               "左摇杆Y+ (下)")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_ANALOG_RIGHT_X,
               "右摇杆X")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_ANALOG_RIGHT_X_MINUS,
               "右摇杆X- (左)")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_ANALOG_RIGHT_X_PLUS,
               "右摇杆X+ (右)")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_ANALOG_RIGHT_Y,
               "右摇杆Y")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_ANALOG_RIGHT_Y_MINUS,
               "右摇杆Y- (上)")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_ANALOG_RIGHT_Y_PLUS,
               "右摇杆Y+ (下)")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_AUTODETECT_ENABLE,
               "启用自动配置")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_AXIS_THRESHOLD,
               "输入轴阈值")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_MENU_INPUT_SWAP_OK_CANCEL,
               "菜单切换 确定/取消 按钮") /*FIXME:"Menu Swap OK & Cancel Buttons"*/
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_BIND_ALL,
               "绑定全部")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_BIND_DEFAULT_ALL,
               "绑定全部至默认值")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_BIND_TIMEOUT,
               "绑定超时时间")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_DESCRIPTOR_HIDE_UNBOUND,
               "隐藏未绑定的核心输入描述")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_DESCRIPTOR_LABEL_SHOW,
               "显示输入描述标签")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_DEVICE_INDEX,
               "设备索引")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_DEVICE_TYPE,
               "设备类型")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_DRIVER,
               "输入驱动")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_DUTY_CYCLE,
               "Turbo占空比")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_HOTKEY_BINDS,
               "输入热键绑定")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_ICADE_ENABLE,
               "键盘控制器映射启用")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_A,
               "A键(右侧)")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_B,
               "B键(下方)")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_DOWN,
               "下十字键")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_L2,
               "L2键(触发)")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_L3,
               "L3键(拇指)")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_L,
               "L键(手柄肩部)")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_LEFT,
               "左十字键")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_R2,
               "R2键(触发)")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_R3,
               "R3键(拇指)")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_R,
               "R键(手柄肩部)")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_RIGHT,
               "右十字键")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_SELECT,
               "选择键")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_START,
               "开始键")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_UP,
               "上十字键")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_X,
               "X键(上方)")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_JOYPAD_Y,
               "Y键(左侧)")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_KEY,
               "(键: %s)") /*FIXME:"(Key: %s)"*/
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_KEYBOARD_GAMEPAD_MAPPING_TYPE,
               "键盘控制器映射类型")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_MAX_USERS,
               "最大用户数")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_MENU_ENUM_TOGGLE_GAMEPAD_COMBO,
               "游戏控制器菜单切出组合键")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_CHEAT_INDEX_MINUS,
               "金手指索引 -")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_CHEAT_INDEX_PLUS,
               "金手指索引 +")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_CHEAT_TOGGLE,
               "金手指开关")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_DISK_EJECT_TOGGLE,
               "光驱出仓切换")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_DISK_NEXT,
               "下一张光盘")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_DISK_PREV,
               "上一张光盘")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_ENABLE_HOTKEY,
               "启用热键")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_FAST_FORWARD_HOLD_KEY,
               "快进保持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_FAST_FORWARD_KEY,
               "快进切换")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_FRAMEADVANCE,
               "帧提前量")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_FULLSCREEN_TOGGLE_KEY,
               "切换全屏幕")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_GRAB_MOUSE_TOGGLE,
               "鼠标捕获开关")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_GAME_FOCUS_TOGGLE,
               "Game focus toggle")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_LOAD_STATE_KEY,
               "加载状态")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_MENU_TOGGLE,
               "切换菜单")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_MOVIE_RECORD_TOGGLE,
               "视频录制开关")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_MUTE,
               "静音开关")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_NETPLAY_FLIP,
               "联机游戏踢出用户")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_NETPLAY_GAME_WATCH,
               "联机游戏切换 游戏/围观 模式")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_OSK,
               "切换屏幕键盘")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_OVERLAY_NEXT,
               "下一个覆层")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_PAUSE_TOGGLE,
               "切换暂停")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_QUIT_KEY,
               "退出 RetroArch")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_RESET,
               "重置游戏")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_REWIND,
               "回溯")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_SAVE_STATE_KEY,
               "保存状态")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_SCREENSHOT,
               "屏幕截图")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_SHADER_NEXT,
               "下一个Shader")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_SHADER_PREV,
               "上一个Shader")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_SLOWMOTION,
               "慢动作")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_STATE_SLOT_MINUS,
               "存档槽 -")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_STATE_SLOT_PLUS,
               "存档槽 +")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_VOLUME_DOWN,
               "音量 -")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_META_VOLUME_UP,
               "音量 +")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_OSK_OVERLAY_ENABLE,
               "显示键盘覆层")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_OVERLAY_ENABLE,
               "显示覆层")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_OVERLAY_HIDE_IN_MENU,
               "在菜单中隐藏覆层")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_POLL_TYPE_BEHAVIOR,
               "轮询类型行为")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_POLL_TYPE_BEHAVIOR_EARLY,
               "较早")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_POLL_TYPE_BEHAVIOR_LATE,
               "稍晚")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_POLL_TYPE_BEHAVIOR_NORMAL,
               "正常")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_PREFER_FRONT_TOUCH,
               "优先前置触摸")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_REMAPPING_DIRECTORY,
               "输入重映射目录")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_REMAP_BINDS_ENABLE,
               "启用绑定重映射")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_SAVE_AUTOCONFIG,
               "保存自动设置")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_SETTINGS,
               "输入")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_SMALL_KEYBOARD_ENABLE,
               "启用小键盘")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_TOUCH_ENABLE,
               "启用触摸")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_TURBO_ENABLE,
               "TURBO开关")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_TURBO_PERIOD,
               "Turbo区间")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_USER_BINDS,
               "输入用户 %u 的绑定")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INTERNAL_STORAGE_STATUS,
               "内部存储状态")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_JOYPAD_AUTOCONFIG_DIR,
               "输入设备自动配置目录")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_JOYPAD_DRIVER,
               "手柄驱动")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_LAKKA_SERVICES,
               "Lakka 服务")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_CHINESE_SIMPLIFIED,
               "简体中文")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_CHINESE_TRADITIONAL,
               "繁体中文")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_DUTCH,
               "荷兰语")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_ENGLISH,
               "英语")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_ESPERANTO,
               "世界语")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_FRENCH,
               "法语")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_GERMAN,
               "德语")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_ITALIAN,
               "意大利语")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_JAPANESE,
               "日语")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_KOREAN,
               "韩语")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_POLISH,
               "波兰语")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_PORTUGUESE_BRAZIL,
               "葡萄牙语")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_PORTUGUESE_PORTUGAL,
               "葡萄牙语")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_RUSSIAN,
               "俄语")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_SPANISH,
               "西班牙语")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_LANG_VIETNAMESE,
               "越南语")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_LEFT_ANALOG,
               "左侧摇杆")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_LIBRETRO_DIR_PATH,
               "核心目录")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_LIBRETRO_INFO_PATH,
               "核心信息目录")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_LIBRETRO_LOG_LEVEL,
               "核心日志级别")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_LINEAR,
               "线性")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_LOAD_ARCHIVE,
               "使用核心加载压缩包")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_LOAD_CONTENT_HISTORY,
               "加载最近的游戏内容")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_LOAD_CONTENT_LIST,
               "载入游戏内容")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_LOAD_STATE,
               "加载状态")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_LOCATION_ALLOW,
               "允许使用位置")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_LOCATION_DRIVER,
               "定位驱动")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_LOGGING_SETTINGS,
               "日志")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_LOG_VERBOSITY,
               "完整日志记录")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_MAIN_MENU,
               "主菜单")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_MANAGEMENT,
               "数据库设置")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_MATERIALUI_MENU_COLOR_THEME,
               "菜单颜色主题")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_MATERIALUI_MENU_COLOR_THEME_BLUE,
               "蓝色")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_MATERIALUI_MENU_COLOR_THEME_BLUE_GREY,
               "蓝灰色")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_MATERIALUI_MENU_COLOR_THEME_DARK_BLUE,
               "深蓝色")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_MATERIALUI_MENU_COLOR_THEME_GREEN,
               "绿色")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_MATERIALUI_MENU_COLOR_THEME_NVIDIA_SHIELD,
               "NV SHIELD")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_MATERIALUI_MENU_COLOR_THEME_RED,
               "红色")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_MATERIALUI_MENU_COLOR_THEME_YELLOW,
               "黄色")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_MATERIALUI_MENU_FOOTER_OPACITY,
               "底部不透明度")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_MATERIALUI_MENU_HEADER_OPACITY,
               "顶部不透明度")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_MENU_DRIVER,
               "菜单驱动")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_MENU_ENUM_THROTTLE_FRAMERATE,
               "限制菜单帧率")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_MENU_FILE_BROWSER_SETTINGS, /* TODO/FIXME - update */
               "菜单文件浏览器")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_MENU_LINEAR_FILTER,
               "菜单线性过滤")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_MENU_SETTINGS,
               "菜单")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_MENU_WALLPAPER,
               "菜单壁纸")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_MENU_WALLPAPER_OPACITY,
               "壁纸不透明度")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_MISSING,
               "丢失")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_MORE,
               "...")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_MOUSE_ENABLE,
               "鼠标支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_MULTIMEDIA_SETTINGS,
               "多媒体")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_MUSIC_TAB,
               "音乐")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NAVIGATION_BROWSER_FILTER_SUPPORTED_EXTENSIONS_ENABLE,
               "过滤未知扩展名")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NAVIGATION_WRAPAROUND,
               "环绕式导航")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NEAREST,
               "最近")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY,
               "在线游戏")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_CHECK_FRAMES,
               "在线游戏检查帧数")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_CLIENT_SWAP_INPUT,
               "在线玩家P2使用C1")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_DELAY_FRAMES,
               "在线游戏延迟帧数")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_DISCONNECT,
               "断开连接")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_ENABLE,
               "启用在线游戏")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_ENABLE_CLIENT,
               "连接到游戏主机")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_ENABLE_HOST,
               "作为游戏主机")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_DISABLE_HOST,
               "Stop netplay host")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_IP_ADDRESS,
               "服务器地址")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_LAN_SCAN_SETTINGS,
               "扫描本地网络")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_MODE,
               "启用在线游戏客户端")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_NICKNAME,
               "用户名")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_PASSWORD,
               "服务器密码")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_SETTINGS,
               "在线游戏设置")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_STATELESS_MODE,
               "联机无状态模式")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_SPECTATE_PASSWORD,
               "服务器围观的密码")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_SPECTATOR_MODE_ENABLE,
               "启用在线游戏旁观者")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_TCP_UDP_PORT,
               "在线游戏TCP/UDP端口")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_NAT_TRAVERSAL,
               "联机NAT遍历")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NETWORK_CMD_ENABLE,
               "网络命令")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NETWORK_CMD_PORT,
               "网络命令端口")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NETWORK_INFORMATION,
               "网络信息")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NETWORK_REMOTE_ENABLE,
               "网络游戏控制器")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NETWORK_REMOTE_PORT,
               "网络远端基本端口")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NETWORK_SETTINGS,
               "网络")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NO,
               "否")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NONE,
               "无")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NOT_AVAILABLE,
               "N/A")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_ACHIEVEMENTS_TO_DISPLAY,
               "没有可显示的成就。")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_CORE,
               "没有核心")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_CORES_AVAILABLE,
               "没有可用的核心。")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_CORE_INFORMATION_AVAILABLE,
               "没有可用的核心信息。")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_CORE_OPTIONS_AVAILABLE,
               "没有可用的核心选项。")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_ENTRIES_TO_DISPLAY,
               "没有可显示的条目。")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_HISTORY_AVAILABLE,
               "没有可用的历史记录。")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_INFORMATION_AVAILABLE,
               "没有可用的信息。")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_ITEMS,
               "没有条目。")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_NETPLAY_HOSTS_FOUND,
               "未发现联机游戏主机。")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_NETWORKS_FOUND,
               "未发现网络。")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_PERFORMANCE_COUNTERS,
               "没有性能计数器。")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_PLAYLISTS,
               "没有游戏列表。")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_PLAYLIST_ENTRIES_AVAILABLE,
               "没有可用的游戏列表项目。")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_SETTINGS_FOUND,
               "没有找到设置。")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NO_SHADER_PARAMETERS,
               "没有Shader参数.")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_OFF,
               "关")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_ON,
               "开")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_ONLINE_UPDATER,
               "在线更新器")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_ONSCREEN_DISPLAY_SETTINGS,
               "屏幕显示")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_ONSCREEN_OVERLAY_SETTINGS,
               "屏幕覆层")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_OPEN_ARCHIVE,
               "以文件夹形式打开压缩包")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_ONSCREEN_NOTIFICATIONS_SETTINGS,
               "屏幕通知")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_OPTIONAL,
               "任意")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_OSK_OVERLAY_DIRECTORY,
               "OSK覆层目录")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_OVERLAY,
               "覆层")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_OVERLAY_AUTOLOAD_PREFERRED,
               "自动加载最佳的覆层")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_OVERLAY_DIRECTORY,
               "覆层目录")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_OVERLAY_OPACITY,
               "覆层不透明度")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_OVERLAY_PRESET,
               "覆层预设")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_OVERLAY_SCALE,
               "覆层缩放比例")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_OVERLAY_SETTINGS,
               "屏幕覆层")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_PAL60_ENABLE,
               "使用PAL60模式")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_PARENT_DIRECTORY,
               "上一级目录")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_PAUSE_LIBRETRO,
               "当菜单激活时暂停")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_PAUSE_NONACTIVE,
               "禁止后台运行")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_PERFCNT_ENABLE,
               "性能计数器")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_PLAYLISTS_TAB,
               "游戏列表")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_PLAYLIST_DIRECTORY,
               "游戏列表目录")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_PLAYLIST_SETTINGS,
               "游戏列表")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_POINTER_ENABLE,
               "触摸支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_PORT,
               "端口")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_PRESENT,
               "现在")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_PRIVACY_SETTINGS,
               "隐私")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_QUIT_RETROARCH,
               "退出 RetroArch")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_ANALOG,
               "支持摇杆输入")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_BBFC_RATING,
               "BBFC 分级")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_CERO_RATING,
               "CERO 分级")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_COOP,
               "多人游戏支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_CRC32,
               "CRC32")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_DESCRIPTION,
               "描述")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_DEVELOPER,
               "开发者")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_EDGE_MAGAZINE_ISSUE,
               "Edge杂志发行")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_EDGE_MAGAZINE_RATING,
               "Edge杂志评分")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_EDGE_MAGAZINE_REVIEW,
               "Edge杂志评论")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_ELSPA_RATING,
               "ELSPA 分级")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_ENHANCEMENT_HW,
               "增强硬件")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_ESRB_RATING,
               "ESRB 分级")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_FAMITSU_MAGAZINE_RATING,
               "次世代(Famitsu)杂志评分")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_FRANCHISE,
               "经销商")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_GENRE,
               "类型")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_MD5,
               "MD5")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_NAME,
               "名称")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_ORIGIN,
               "起源")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_PEGI_RATING,
               "PEGI 分级")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_PUBLISHER,
               "出版方")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_RELEASE_MONTH,
               "发售月份")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_RELEASE_YEAR,
               "发售年份")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_RUMBLE,
               "震动支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_SERIAL,
               "系列")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_SHA1,
               "SHA1")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_START_CONTENT,
               "启动游戏内容")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RDB_ENTRY_TGDB_RATING,
               "TGDB 评分")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_REBOOT,
               "重启")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RECORDING_CONFIG_DIRECTORY,
               "录像配置目录")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RECORDING_OUTPUT_DIRECTORY,
               "录像输出目录")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RECORDING_SETTINGS,
               "录像")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RECORD_CONFIG,
               "录像配置")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RECORD_DRIVER,
               "录像驱动")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RECORD_ENABLE,
               "启用录像")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RECORD_PATH,
               "输出文件")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RECORD_USE_OUTPUT_DIRECTORY,
               "使用输出目录")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_REMAP_FILE,
               "重映射文件")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_REMAP_FILE_LOAD,
               "加载重映射文件")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_REMAP_FILE_SAVE_CORE,
               "保存核心重映射文件")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_REMAP_FILE_SAVE_GAME,
               "保存游戏重映射文件")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_REQUIRED,
               "必须")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RESTART_CONTENT,
               "重启")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RESTART_RETROARCH,
               "重启 RetroArch")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RESUME,
               "继续")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RESUME_CONTENT,
               "继续")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RETROKEYBOARD,
               "Retro键盘")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RETROPAD,
               "Retro触摸板")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RETROPAD_WITH_ANALOG,
               "RetroPad w/ Analog")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RETRO_ACHIEVEMENTS_SETTINGS,
               "Retro 成就")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_REWIND_ENABLE,
               "启用回溯")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_REWIND_GRANULARITY,
               "回溯粒度")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_REWIND_SETTINGS,
               "回溯")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RGUI_BROWSER_DIRECTORY,
               "文件浏览器目录")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RGUI_CONFIG_DIRECTORY,
               "配置目录")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RGUI_SHOW_START_SCREEN,
               "显示开始屏幕")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RIGHT_ANALOG,
               "右侧摇杆")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_RUN,
               "运行")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SAMBA_ENABLE,
               "启用SAMBA文件共享服务")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SAVEFILE_DIRECTORY,
               "存档文件目录")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SAVESTATE_AUTO_INDEX,
               "自动索引保存状态")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SAVESTATE_AUTO_LOAD,
               "自动加载状态")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SAVESTATE_AUTO_SAVE,
               "自动保存状态")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SAVESTATE_DIRECTORY,
               "状态存储目录")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SAVESTATE_THUMBNAIL_ENABLE,
               "Savestate Thumbnails")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SAVE_CURRENT_CONFIG,
               "保存当前配置")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SAVE_CURRENT_CONFIG_OVERRIDE_CORE,
               "保存核心覆写")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SAVE_CURRENT_CONFIG_OVERRIDE_GAME,
               "保存游戏覆写")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SAVE_NEW_CONFIG,
               "保存新配置")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SAVE_STATE,
               "保存状态")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SAVING_SETTINGS,
               "存档")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SCAN_DIRECTORY,
               "扫描文件夹")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SCAN_FILE,
               "扫描文件")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SCAN_THIS_DIRECTORY,
               "<扫描当前目录>")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SCREENSHOT_DIRECTORY,
               "屏幕截图目录")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SCREEN_RESOLUTION,
               "屏幕分辨率")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SEARCH,
               "搜索：")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SECONDS,
               "秒")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SETTINGS,
               "设置")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SETTINGS_TAB,
               "设置")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SHADER,
               "Shader")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SHADER_APPLY_CHANGES,
               "应用Shader修改")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SHADER_OPTIONS,
               "Shader效果")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SHADER_PIPELINE_RIBBON,
               "Ribbon")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SHADER_PIPELINE_RIBBON_SIMPLIFIED,
               "Ribbon (简化)")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SHADER_PIPELINE_SIMPLE_SNOW,
               "Simple Snow")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SHADER_PIPELINE_SNOW,
               "Snow")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SHOW_ADVANCED_SETTINGS,
               "显示高级设置")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SHOW_HIDDEN_FILES,
               "显示隐藏的文件和文件夹")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SHUTDOWN,
               "关机")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SLOWMOTION_RATIO,
               "慢动作倍率")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SORT_SAVEFILES_ENABLE,
               "排序文件夹中的存档")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SORT_SAVESTATES_ENABLE,
               "排序文件夹中的状态存储")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SSH_ENABLE,
               "启用SSH远程终端服务")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_START_CORE,
               "启动核心")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_START_NET_RETROPAD,
               "启动远程的RetroPad")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_START_VIDEO_PROCESSOR,
               "启动视频处理")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_STATE_SLOT,
               "状态存储槽")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_STATUS,
               "状态")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_STDIN_CMD_ENABLE,
               "标准输入流命令")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SUPPORTED_CORES,
               "支持的核心")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SUSPEND_SCREENSAVER_ENABLE,
               "暂停屏保程序")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_BGM_ENABLE,
               "启用系统背景音乐")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_DIRECTORY,
               "系统/BIOS目录")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFORMATION,
               "系统信息")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_7ZIP_SUPPORT,
               "7zip 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_ALSA_SUPPORT,
               "ALSA 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_BUILD_DATE,
               "编译日期")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_CG_SUPPORT,
               "Cg 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_COCOA_SUPPORT,
               "Cocoa 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_COMMAND_IFACE_SUPPORT,
               "控制台支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_CORETEXT_SUPPORT,
               "CoreText 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_CPU_FEATURES,
               "CPU特性")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_DISPLAY_METRIC_DPI,
               "显示器度量DPI")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_DISPLAY_METRIC_MM_HEIGHT,
               "显示器度量高度(mm)")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_DISPLAY_METRIC_MM_WIDTH,
               "显示器度量宽度(mm)")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_DSOUND_SUPPORT,
               "DirectSound 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_WASAPI_SUPPORT,
               "WASAPI 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_DYLIB_SUPPORT,
               "动态链接库支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_DYNAMIC_SUPPORT,
               "运行时动态加载libretro库")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_EGL_SUPPORT,
               "EGL 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_FBO_SUPPORT,
               "OpenGL/Direct3D 渲染至纹理 (多渲染批次Shader) 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_FFMPEG_SUPPORT,
               "FFmpeg 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_FREETYPE_SUPPORT,
               "FreeType 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_FRONTEND_IDENTIFIER,
               "前端标识")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_FRONTEND_NAME,
               "前端名称")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_FRONTEND_OS,
               "前端操作系统")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_GIT_VERSION,
               "Git版本")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_GLSL_SUPPORT,
               "GLSL 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_HLSL_SUPPORT,
               "HLSL 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_JACK_SUPPORT,
               "JACK 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_KMS_SUPPORT,
               "KMS/EGL 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_LIBRETRODB_SUPPORT,
               "LibretroDB 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_LIBUSB_SUPPORT,
               "Libusb 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_LIBXML2_SUPPORT,
               "libxml2 XML解析支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_NETPLAY_SUPPORT,
               "Netplay (点对点) 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_NETWORK_COMMAND_IFACE_SUPPORT,
               "网络控制台支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_NETWORK_REMOTE_SUPPORT,
               "网络控制器支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_OPENAL_SUPPORT,
               "OpenAL 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_OPENGLES_SUPPORT,
               "OpenGL ES 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_OPENGL_SUPPORT,
               "OpenGL 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_OPENSL_SUPPORT,
               "OpenSL 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_OPENVG_SUPPORT,
               "OpenVG 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_OSS_SUPPORT,
               "OSS 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_OVERLAY_SUPPORT,
               "覆层支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_POWER_SOURCE,
               "电源")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_POWER_SOURCE_CHARGED,
               "已充满电")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_POWER_SOURCE_CHARGING,
               "充电中")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_POWER_SOURCE_DISCHARGING,
               "放电中")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_POWER_SOURCE_NO_SOURCE,
               "没有电源")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_PULSEAUDIO_SUPPORT,
               "PulseAudio 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_PYTHON_SUPPORT,
               "Python (Shader中脚本支持) 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_RBMP_SUPPORT,
               "BMP 支持 (RBMP)")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_RETRORATING_LEVEL,
               "RetroRating 等级")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_RJPEG_SUPPORT,
               "JPEG 支持 (RJPEG)")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_ROARAUDIO_SUPPORT,
               "RoarAudio 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_RPNG_SUPPORT,
               "PNG 支持 (RPNG)")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_RSOUND_SUPPORT,
               "RSound 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_RTGA_SUPPORT,
               "TGA 支持 (RTGA)")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_SDL2_SUPPORT,
               "SDL2 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_SDL_IMAGE_SUPPORT,
               "SDL 图像支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_SDL_SUPPORT,
               "SDL1.2 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_SLANG_SUPPORT,
               "Slang 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_THREADING_SUPPORT,
               "多线程支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_UDEV_SUPPORT,
               "Udev 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_V4L2_SUPPORT,
               "Video4Linux2 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_VIDEO_CONTEXT_DRIVER,
               "视频上下文驱动")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_VULKAN_SUPPORT,
               "Vulkan 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_WAYLAND_SUPPORT,
               "Wayland 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_X11_SUPPORT,
               "X11 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_XAUDIO2_SUPPORT,
               "XAudio2 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_XVIDEO_SUPPORT,
               "XVideo 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SYSTEM_INFO_ZLIB_SUPPORT,
               "Zlib 支持")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_TAKE_SCREENSHOT,
               "截取屏幕")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_THREADED_DATA_RUNLOOP_ENABLE, /* TODO/FIXME - update */
               "启用多线程数据执行循环")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_THUMBNAILS,
               "缩略图")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_THUMBNAILS_DIRECTORY,
               "缩略图目录")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_THUMBNAILS_UPDATER_LIST,
               "缩略图更新程序")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_THUMBNAIL_MODE_BOXARTS,
               "Boxarts")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_THUMBNAIL_MODE_SCREENSHOTS,
               "截屏")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_THUMBNAIL_MODE_TITLE_SCREENS,
               "标题画面")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_TIMEDATE_ENABLE,
               "显示时间日期")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_TITLE_COLOR,
               "菜单标题颜色")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_TRUE,
               "真")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_UI_COMPANION_ENABLE,
               "UI Companion Enable")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_UI_COMPANION_START_ON_BOOT,
               "UI Companion Start On Boot")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_UI_MENUBAR_ENABLE,
               "Menubar")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_UNABLE_TO_READ_COMPRESSED_FILE,
               "无法读取压缩的文件。")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_UNDO_LOAD_STATE,
               "撤销加载状态")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_UNDO_SAVE_STATE,
               "撤销保存状态")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_UNKNOWN,
               "未知")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_UPDATER_SETTINGS,
               "更新程序")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_UPDATE_ASSETS,
               "更新资源")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_UPDATE_AUTOCONFIG_PROFILES,
               "更新自动配置档案")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_UPDATE_CG_SHADERS,
               "更新CG Shader效果文件")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_UPDATE_CHEATS,
               "更新金手指")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_UPDATE_CORE_INFO_FILES,
               "更新核心信息文件")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_UPDATE_DATABASES,
               "更新数据库")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_UPDATE_GLSL_SHADERS,
               "更新GLSL Shader效果文件")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_UPDATE_LAKKA,
               "更新 Lakka")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_UPDATE_OVERLAYS,
               "更新覆层")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_UPDATE_SLANG_SHADERS,
               "更新Slang Shader效果文件")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_USER,
               "用户")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_USER_INTERFACE_SETTINGS,
               "用户界面")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_USER_LANGUAGE,
               "语言")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_USER_SETTINGS,
               "用户")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_USE_BUILTIN_IMAGE_VIEWER,
               "使用内建的图像浏览器")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_USE_BUILTIN_PLAYER,
               "使用内建媒体播放器")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_USE_THIS_DIRECTORY,
               "<使用当前目录>")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_ALLOW_ROTATE,
               "允许旋转")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_ASPECT_RATIO_AUTO,
               "自动选择视口比例")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_ASPECT_RATIO_INDEX,
               "视口比例选项")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_BLACK_FRAME_INSERTION,
               "黑色帧补间")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_CROP_OVERSCAN,
               "Crop Overscan (Reload)")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_DISABLE_COMPOSITION,
               "禁用桌面元素")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_DRIVER,
               "视频驱动")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_FILTER,
               "视频滤镜")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_FILTER_DIR,
               "视频滤镜目录")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_FILTER_FLICKER,
               "闪烁过滤器")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_FONT_ENABLE,
               "显示屏显消息(OSD)")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_FONT_PATH,
               "屏显消息(OSD)字体")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_FONT_SIZE,
               "屏显消息(OSD)大小")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_FORCE_ASPECT,
               "强制视口比例")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_FORCE_SRGB_DISABLE,
               "强制禁止sRGB帧缓冲")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_FRAME_DELAY,
               "帧延时")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_FULLSCREEN,
               "使用全屏模式")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_GAMMA,
               "视频Gamma")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_GPU_RECORD,
               "启用GPU录像")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_GPU_SCREENSHOT,
               "启用GPU截屏")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_HARD_SYNC,
               "强制GPU同步")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_HARD_SYNC_FRAMES,
               "强制GPU同步帧数")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_MAX_SWAPCHAIN_IMAGES,
               "最大交换链图像数")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_MESSAGE_POS_X,
               "屏显消息(OSD)X轴位置")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_MESSAGE_POS_Y,
               "屏显消息(OSD)Y轴位置")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_MONITOR_INDEX,
               "显示器索引")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_POST_FILTER_RECORD,
               "启用录像后期滤镜")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_REFRESH_RATE,
               "刷新率")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_REFRESH_RATE_AUTO,
               "估算的显示器帧率")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_ROTATION,
               "旋转")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_SCALE,
               "窗口缩放量")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_SCALE_INTEGER,
               "整数化缩放量")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_SETTINGS,
               "视频")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_SHADER_DIR,
               "视频Shader目录")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_SHADER_NUM_PASSES,
               "Shader渲染遍数")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_SHADER_PARAMETERS,
               "预览Shader参数")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_SHADER_PRESET,
               "加载Shader预设")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_SHADER_PRESET_PARAMETERS,
               "菜单Shader参数")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_SHADER_PRESET_SAVE_AS,
               "保存Shader预设为")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_SHADER_PRESET_SAVE_CORE,
               "保存核心预设")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_SHADER_PRESET_SAVE_GAME,
               "保存游戏预设")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_SHARED_CONTEXT,
               "启用硬件共享上下文")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_SMOOTH,
               "硬件双线性过滤")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_SOFT_FILTER,
               "启用软件过滤器")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_SWAP_INTERVAL,
               "垂直同步交换间隔")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_TAB,
               "视频")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_THREADED,
               "多线程渲染")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_VFILTER,
               "降低闪烁")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_VIEWPORT_CUSTOM_HEIGHT,
               "自定义视口高度")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_VIEWPORT_CUSTOM_WIDTH,
               "自定义视口宽度")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_VIEWPORT_CUSTOM_X,
               "自定义视口X")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_VIEWPORT_CUSTOM_Y,
               "自定义视口Y")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_VI_WIDTH,
               "设置 VI 屏幕宽度")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_VSYNC,
               "垂直同步")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_WINDOWED_FULLSCREEN,
               "无边框窗口全屏模式")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_WINDOW_WIDTH,
               "窗口宽度")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_VIDEO_WINDOW_HEIGHT,
               "窗口高度")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_WIFI_DRIVER,
               "Wi-Fi驱动")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_WIFI_SETTINGS,
               "Wi-Fi")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_ALPHA_FACTOR,
               "菜单透明度因子")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_FONT,
               "菜单字体")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_ICON_THEME_CUSTOM,
               "自定义")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_ICON_THEME_FLATUI,
               "FlatUI")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_ICON_THEME_MONOCHROME,
               "Monochrome")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_ICON_THEME_SYSTEMATIC,
               "Systematic")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_ICON_THEME_NEOACTIVE,
               "NeoActive")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_ICON_THEME_PIXEL,
               "Pixel")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_ICON_THEME_RETROACTIVE,
               "RetroActive")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_ICON_THEME_DOTART,
               "Dot-Art")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_MENU_COLOR_THEME,
               "菜单颜色主题")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_MENU_COLOR_THEME_APPLE_GREEN,
               "苹果绿")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_MENU_COLOR_THEME_DARK,
               "深色")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_MENU_COLOR_THEME_DARK_PURPLE,
               "深紫色")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_MENU_COLOR_THEME_ELECTRIC_BLUE,
               "铁蓝色")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_MENU_COLOR_THEME_GOLDEN,
               "金色")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_MENU_COLOR_THEME_LEGACY_RED,
               "传统红")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_MENU_COLOR_THEME_MIDNIGHT_BLUE,
               "蓝黑色")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_MENU_COLOR_THEME_PLAIN,
               "朴素")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_MENU_COLOR_THEME_UNDERSEA,
               "海底")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_MENU_COLOR_THEME_VOLCANIC_RED,
               "火山红")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_RIBBON_ENABLE,
               "菜单Shader管线")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_SCALE_FACTOR,
               "菜单缩放因子")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_SHADOWS_ENABLE,
               "启用图标阴影")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_SHOW_HISTORY,
               "显示历史页")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_SHOW_ADD,
               "显示导入内容页")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_SHOW_IMAGES,
               "显示图像页")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_SHOW_MUSIC,
               "显示音乐页")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_SHOW_SETTINGS,
               "显示设置页")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_SHOW_VIDEO,
               "显示视频页")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_XMB_THEME,
               "菜单图标主题")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_YES,
               "是")
      MSG_HASH(MENU_ENUM_LABEL_VIDEO_SHADER_PRESET_TWO,
               "Shader预设")
      MSG_HASH(MENU_ENUM_SUBLABEL_CHEEVOS_ENABLE,
               "打开或关闭成就。更多内容请访问 http://retroachievements.org")
      MSG_HASH(MENU_ENUM_SUBLABEL_CHEEVOS_TEST_UNOFFICIAL,
               "为测试目的而打开或关闭非官方成就和/或测试版特性。")
      MSG_HASH(MENU_ENUM_SUBLABEL_CHEEVOS_HARDCORE_MODE_ENABLE,
               "为所有游戏打开或关闭存档、金手指、回退、快进、暂停和慢动作。")
      MSG_HASH(MENU_ENUM_SUBLABEL_DRIVER_SETTINGS,
               "修改驱动设置。")
      MSG_HASH(MENU_ENUM_SUBLABEL_RETRO_ACHIEVEMENTS_SETTINGS,
               "修改成就设置。")
      MSG_HASH(MENU_ENUM_SUBLABEL_CORE_SETTINGS,
               "修改核心设置。")
      MSG_HASH(MENU_ENUM_SUBLABEL_RECORDING_SETTINGS,
               "修改录制的设置。")
      MSG_HASH(MENU_ENUM_SUBLABEL_ONSCREEN_DISPLAY_SETTINGS,
               "修改显示覆盖、键盘覆盖和屏幕通知的设置。")
      MSG_HASH(MENU_ENUM_SUBLABEL_FRAME_THROTTLE_SETTINGS,
               "修改回滚、快进和慢动作的设置。")
      MSG_HASH(MENU_ENUM_SUBLABEL_SAVING_SETTINGS,
               "修改存档设置。")
      MSG_HASH(MENU_ENUM_SUBLABEL_LOGGING_SETTINGS,
               "修改日志设置。")
      MSG_HASH(MENU_ENUM_SUBLABEL_USER_INTERFACE_SETTINGS,
               "修改用户界面设置。")
      MSG_HASH(MENU_ENUM_SUBLABEL_USER_SETTINGS,
               "修改帐号、用户名和语言。")
      MSG_HASH(MENU_ENUM_SUBLABEL_PRIVACY_SETTINGS,
               "修改你的隐私设置。")
      MSG_HASH(MENU_ENUM_SUBLABEL_DIRECTORY_SETTINGS,
               "修改此系统的默认目录。")
      MSG_HASH(MENU_ENUM_SUBLABEL_PLAYLIST_SETTINGS,
               "修改游戏列表设置。")
      MSG_HASH(MENU_ENUM_SUBLABEL_NETWORK_SETTINGS,
               "修改网络设置。")
      MSG_HASH(MENU_ENUM_SUBLABEL_ADD_CONTENT_LIST,
               "下载且/或者扫描游戏内容，并将其加入你的收藏中。")
      MSG_HASH(MENU_ENUM_SUBLABEL_AUDIO_SETTINGS,
               "调整音频输出的选项。")
      MSG_HASH(MENU_ENUM_SUBLABEL_BLUETOOTH_ENABLE,
               "启用或者禁止蓝牙。")
      MSG_HASH(MENU_ENUM_SUBLABEL_CONFIG_SAVE_ON_EXIT,
               "程序将在退出时保存修改到配置文件。")
      MSG_HASH(MENU_ENUM_SUBLABEL_CONFIGURATION_SETTINGS,
               "修改配置文件的默认设置。")
      MSG_HASH(MENU_ENUM_SUBLABEL_CONFIGURATIONS_LIST,
               "管理和创建配置文件。")
      MSG_HASH(MENU_ENUM_SUBLABEL_CPU_CORES,
               "CPU拥有的核心总数。")
      MSG_HASH(MENU_ENUM_SUBLABEL_FPS_SHOW,
               "在屏幕上显示当前每秒的帧率。")
      MSG_HASH(MENU_ENUM_SUBLABEL_INPUT_HOTKEY_BINDS,
               "配置热键选项。")
      MSG_HASH(MENU_ENUM_SUBLABEL_INPUT_MENU_ENUM_TOGGLE_GAMEPAD_COMBO,
               "控制器用来切出菜单的组合键。")
      MSG_HASH(MENU_ENUM_SUBLABEL_INPUT_SETTINGS,
               "调整游戏控制器、键盘和鼠标的设置。")
      MSG_HASH(MENU_ENUM_SUBLABEL_INPUT_USER_BINDS,
               "配置该用户的控制选项。")
      MSG_HASH(MENU_ENUM_SUBLABEL_LOG_VERBOSITY,
               "启用或禁止向控制台打印日志。")
      MSG_HASH(MENU_ENUM_SUBLABEL_NETPLAY,
               "加入或者开启一个在线多人游戏的会话。")
      MSG_HASH(MENU_ENUM_SUBLABEL_NETPLAY_LAN_SCAN_SETTINGS,
               "在局域网内搜索并连接联网游戏的主机。")
      MSG_HASH(MENU_ENUM_SUBLABEL_INFORMATION_LIST_LIST,
               "显示核心、网络和系统的信息。显示数据库和光标的管理器。")
      MSG_HASH(MENU_ENUM_SUBLABEL_ONLINE_UPDATER,
               "下载并更新RetroArch的附加插件和组件。")
      MSG_HASH(MENU_ENUM_SUBLABEL_SAMBA_ENABLE,
               "启用或者禁止网络文件夹共享(SAMBA)。")
      MSG_HASH(MENU_ENUM_SUBLABEL_SERVICES_SETTINGS,
               "管理操作系统层级的服务。")
      MSG_HASH(MENU_ENUM_SUBLABEL_SHOW_HIDDEN_FILES,
               "在文件浏览器中显示隐藏的文件或文件夹。")
      MSG_HASH(MENU_ENUM_SUBLABEL_SSH_ENABLE,
               "启用或者禁止远程终端访问(SSH)。")
      MSG_HASH(MENU_ENUM_SUBLABEL_SUSPEND_SCREENSAVER_ENABLE,
               "阻止系统激活屏幕保护程序。")
      MSG_HASH(MENU_ENUM_SUBLABEL_USER_LANGUAGE,
               "设置用户界面的语言。")
      MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_BLACK_FRAME_INSERTION,
               "在帧与帧之间插入黑色的中间帧，通常用于消除在\n"
                       "120Hz刷新率的显示器上运行60Hz的游戏内容带来\n"
                       "的鬼影。")
      MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_FRAME_DELAY,
               "以增加画面卡顿的风险换取低延时，在垂直同步后增加\n"
                       "时延(毫秒)。")
      MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_HARD_SYNC_FRAMES,
               "设置当开启“强制GPU同步”时CPU可以预先GPU多少帧。")
      MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_MAX_SWAPCHAIN_IMAGES,
               "强制显示驱动程序使用特定的缓冲模式。")
      MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_MONITOR_INDEX,
               "选择将要使用哪一个显示器。")
      MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_REFRESH_RATE_AUTO,
               "估算的显示器刷新率(Hz)。")
      MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_SETTINGS,
               "调整视频输出的选项。")
      MSG_HASH(MENU_ENUM_SUBLABEL_WIFI_SETTINGS,
               "扫描无线网络并且建立连接。")
      MSG_HASH(MENU_ENUM_SUBLABEL_HELP_LIST,
               "学习更多关于其是如何工作的。")
      MSG_HASH(MSG_APPENDED_DISK,
               "Appended disk")
      MSG_HASH(MSG_APPLICATION_DIR,
               "应用程序目录")
      MSG_HASH(MSG_APPLYING_SHADER,
               "Applying shader")
      MSG_HASH(MSG_AUDIO_MUTED,
               "静音。")
      MSG_HASH(MSG_AUDIO_UNMUTED,
               "取消静音。")
      MSG_HASH(MSG_AUTOCONFIG_FILE_ERROR_SAVING,
               "保存 autoconf 文件错误。")
      MSG_HASH(MSG_AUTOCONFIG_FILE_SAVED_SUCCESSFULLY,
               "自动配置文件保存成功。")
      MSG_HASH(MSG_AUTOSAVE_FAILED,
               "无法初始化自动保存。")
      MSG_HASH(MSG_AUTO_SAVE_STATE_TO,
               "自动保存状态至")
      MSG_HASH(MSG_BLOCKING_SRAM_OVERWRITE,
               "阻止 SRAM 覆盖")
      MSG_HASH(MSG_BRINGING_UP_COMMAND_INTERFACE_ON_PORT,
               "Bringing up command interface on port")
      MSG_HASH(MSG_BYTES,
               "字节")
      MSG_HASH(MSG_CANNOT_INFER_NEW_CONFIG_PATH,
               "无法推断新的配置路径，使用当前时间。")
      MSG_HASH(MSG_CHEEVOS_HARDCORE_MODE_ENABLE,
               "硬核模式开启：及时存档和回放被禁用.")
      MSG_HASH(MSG_COMPARING_WITH_KNOWN_MAGIC_NUMBERS,
               "与已知的magic numbers比较...")
      MSG_HASH(MSG_COMPILED_AGAINST_API,
               "Compiled against API")
      MSG_HASH(MSG_CONFIG_DIRECTORY_NOT_SET,
               "未设置配置目录，无法保存新的配置。")
      MSG_HASH(MSG_CONNECTED_TO,
               "连接至")
      MSG_HASH(MSG_CONTENT_CRC32S_DIFFER,
               "内容的CRC32s不同。无法使用不同的游戏。")
      MSG_HASH(MSG_CONTENT_LOADING_SKIPPED_IMPLEMENTATION_WILL_DO_IT,
               "跳过内容加载。实现将自行加载。")
      MSG_HASH(MSG_CORE_DOES_NOT_SUPPORT_SAVESTATES,
               "核心不支持保存状态。")
      MSG_HASH(MSG_CORE_OPTIONS_FILE_CREATED_SUCCESSFULLY,
               "Core options file created successfully.")
      MSG_HASH(MSG_COULD_NOT_FIND_ANY_NEXT_DRIVER,
               "Could not find any next driver")
      MSG_HASH(MSG_COULD_NOT_FIND_COMPATIBLE_SYSTEM,
               "Could not find compatible system.")
      MSG_HASH(MSG_COULD_NOT_FIND_VALID_DATA_TRACK,
               "无法找到有效的数据轨")
      MSG_HASH(MSG_COULD_NOT_OPEN_DATA_TRACK,
               "无法打开数据轨")
      MSG_HASH(MSG_COULD_NOT_READ_CONTENT_FILE,
               "无法读取内容文件")
      MSG_HASH(MSG_COULD_NOT_READ_MOVIE_HEADER,
               "无法读取视频头部信息.")
      MSG_HASH(MSG_COULD_NOT_READ_STATE_FROM_MOVIE,
               "无法读取视频状态.")
      MSG_HASH(MSG_CRC32_CHECKSUM_MISMATCH,
               "CRC32 checksum mismatch between content file and saved content checksum in replay file header; replay highly likely to desync on playback.")
      MSG_HASH(MSG_CUSTOM_TIMING_GIVEN,
               "Custom timing given")
      MSG_HASH(MSG_DECOMPRESSION_ALREADY_IN_PROGRESS,
               "解压缩已在进行中。")
      MSG_HASH(MSG_DECOMPRESSION_FAILED,
               "解压缩失败。")
      MSG_HASH(MSG_DETECTED_VIEWPORT_OF,
               "Detected viewport of")
      MSG_HASH(MSG_DID_NOT_FIND_A_VALID_CONTENT_PATCH,
               "Did not find a valid content patch.")
      MSG_HASH(MSG_DISCONNECT_DEVICE_FROM_A_VALID_PORT,
               "Disconnect device from a valid port.")
      MSG_HASH(MSG_DISK_CLOSED,
               "已关闭")
      MSG_HASH(MSG_DISK_EJECTED,
               "已弹出")
      MSG_HASH(MSG_DOWNLOADING,
               "正在下载")
      MSG_HASH(MSG_DOWNLOAD_FAILED,
               "下载失败")
      MSG_HASH(MSG_ERROR,
               "错误")
      MSG_HASH(MSG_ERROR_LIBRETRO_CORE_REQUIRES_CONTENT,
               "Libretro core requires content, but nothing was provided.")
      MSG_HASH(MSG_ERROR_LIBRETRO_CORE_REQUIRES_SPECIAL_CONTENT,
               "Libretro core requires special content, but none were provided.")
      MSG_HASH(MSG_ERROR_PARSING_ARGUMENTS,
               "Error parsing arguments.")
      MSG_HASH(MSG_ERROR_SAVING_CORE_OPTIONS_FILE,
               "Error saving core options file.")
      MSG_HASH(MSG_ERROR_SAVING_REMAP_FILE,
               "Error saving remap file.")
      MSG_HASH(MSG_ERROR_SAVING_SHADER_PRESET,
               "Error saving shader preset.")
      MSG_HASH(MSG_EXTERNAL_APPLICATION_DIR,
               "外部应用程序目录")
      MSG_HASH(MSG_EXTRACTING,
               "正在解压")
      MSG_HASH(MSG_EXTRACTING_FILE,
               "解压文件")
      MSG_HASH(MSG_FAILED_SAVING_CONFIG_TO,
               "无法保存配置到")
      MSG_HASH(MSG_FAILED_TO,
               "Failed to")
      MSG_HASH(MSG_FAILED_TO_ACCEPT_INCOMING_SPECTATOR,
               "Failed to accept incoming spectator.")
      MSG_HASH(MSG_FAILED_TO_ALLOCATE_MEMORY_FOR_PATCHED_CONTENT,
               "Failed to allocate memory for patched content...")
      MSG_HASH(MSG_FAILED_TO_APPLY_SHADER,
               "Failed to apply shader.")
      MSG_HASH(MSG_FAILED_TO_BIND_SOCKET,
               "Failed to bind socket.")
      MSG_HASH(MSG_FAILED_TO_CREATE_THE_DIRECTORY,
               "创建目录失败。")
      MSG_HASH(MSG_FAILED_TO_EXTRACT_CONTENT_FROM_COMPRESSED_FILE,
               "从压缩文件中提取内容失败")
      MSG_HASH(MSG_FAILED_TO_GET_NICKNAME_FROM_CLIENT,
               "从客户端获取昵称失败.")
      MSG_HASH(MSG_FAILED_TO_LOAD,
               "无法加载")
      MSG_HASH(MSG_FAILED_TO_LOAD_CONTENT,
               "载入内容失败")
      MSG_HASH(MSG_FAILED_TO_LOAD_MOVIE_FILE,
               "载入视频文件失败")
      MSG_HASH(MSG_FAILED_TO_LOAD_OVERLAY,
               "Failed to load overlay.")
      MSG_HASH(MSG_FAILED_TO_LOAD_STATE,
               "Failed to load state from")
      MSG_HASH(MSG_FAILED_TO_OPEN_LIBRETRO_CORE,
               "打开libretro核心失败")
      MSG_HASH(MSG_FAILED_TO_PATCH,
               "补丁应用失败")
      MSG_HASH(MSG_FAILED_TO_RECEIVE_HEADER_FROM_CLIENT,
               "Failed to receive header from client.")
      MSG_HASH(MSG_FAILED_TO_RECEIVE_NICKNAME,
               "Failed to receive nickname.")
      MSG_HASH(MSG_FAILED_TO_RECEIVE_NICKNAME_FROM_HOST,
               "Failed to receive nickname from host.")
      MSG_HASH(MSG_FAILED_TO_RECEIVE_NICKNAME_SIZE_FROM_HOST,
               "Failed to receive nickname size from host.")
      MSG_HASH(MSG_FAILED_TO_RECEIVE_SRAM_DATA_FROM_HOST,
               "Failed to receive SRAM data from host.")
      MSG_HASH(MSG_FAILED_TO_REMOVE_DISK_FROM_TRAY,
               "Failed to remove disk from tray.")
      MSG_HASH(MSG_FAILED_TO_REMOVE_TEMPORARY_FILE,
               "移除临时文件失败")
      MSG_HASH(MSG_FAILED_TO_SAVE_SRAM,
               "Failed to save SRAM")
      MSG_HASH(MSG_FAILED_TO_SAVE_STATE_TO,
               "Failed to save state to")
      MSG_HASH(MSG_FAILED_TO_SEND_NICKNAME,
               "发送昵称失败.")
      MSG_HASH(MSG_FAILED_TO_SEND_NICKNAME_SIZE,
               "发送昵称尺寸失败.")
      MSG_HASH(MSG_FAILED_TO_SEND_NICKNAME_TO_CLIENT,
               "发送昵称至客户端失败.")
      MSG_HASH(MSG_FAILED_TO_SEND_NICKNAME_TO_HOST,
               "发送昵称至宿主端失败.")
      MSG_HASH(MSG_FAILED_TO_SEND_SRAM_DATA_TO_CLIENT,
               "发送SRAM数据至客户端失败.")
      MSG_HASH(MSG_FAILED_TO_START_AUDIO_DRIVER,
               "音频驱动启动失败，将在无音频模式下继续启动。")
      MSG_HASH(MSG_FAILED_TO_START_MOVIE_RECORD,
               "启动视频录制失败.")
      MSG_HASH(MSG_FAILED_TO_START_RECORDING,
               "Failed to start recording.")
      MSG_HASH(MSG_FAILED_TO_TAKE_SCREENSHOT,
               "Failed to take screenshot.")
      MSG_HASH(MSG_FAILED_TO_UNDO_LOAD_STATE,
               "Failed to undo load state.")
      MSG_HASH(MSG_FAILED_TO_UNDO_SAVE_STATE,
               "Failed to undo save state.")
      MSG_HASH(MSG_FAILED_TO_UNMUTE_AUDIO,
               "Failed to unmute audio.")
      MSG_HASH(MSG_FATAL_ERROR_RECEIVED_IN,
               "Fatal error received in")
      MSG_HASH(MSG_FILE_NOT_FOUND,
               "未找到文件")
      MSG_HASH(MSG_FOUND_AUTO_SAVESTATE_IN,
               "Found auto savestate in")
      MSG_HASH(MSG_FOUND_DISK_LABEL,
               "Found disk label")
      MSG_HASH(MSG_FOUND_FIRST_DATA_TRACK_ON_FILE,
               "Found first data track on file")
      MSG_HASH(MSG_FOUND_LAST_STATE_SLOT,
               "Found last state slot")
      MSG_HASH(MSG_FOUND_SHADER,
               "Found shader")
      MSG_HASH(MSG_FRAMES,
               "帧")
      MSG_HASH(MSG_GAME_SPECIFIC_CORE_OPTIONS_FOUND_AT,
               "Per-Game Options: game-specific core options found at")
      MSG_HASH(MSG_GOT_INVALID_DISK_INDEX,
               "Got invalid disk index.")
      MSG_HASH(MSG_GRAB_MOUSE_STATE,
               "抓取鼠标状态")
      MSG_HASH(MSG_GAME_FOCUS_ON,
               "Game focus on")
      MSG_HASH(MSG_GAME_FOCUS_OFF,
               "Game focus off")
      MSG_HASH(MSG_HW_RENDERED_MUST_USE_POSTSHADED_RECORDING,
               "Libretro core is hardware rendered. Must use post-shaded recording as well.")
      MSG_HASH(MSG_INFLATED_CHECKSUM_DID_NOT_MATCH_CRC32,
               "Inflated checksum did not match CRC32.")
      MSG_HASH(MSG_INPUT_CHEAT,
               "输入金手指")
      MSG_HASH(MSG_INPUT_CHEAT_FILENAME,
               "Cheat Filename")
      MSG_HASH(MSG_INPUT_PRESET_FILENAME,
               "Preset Filename")
      MSG_HASH(MSG_INTERFACE,
               "接口")
      MSG_HASH(MSG_INTERNAL_STORAGE,
               "内部存储")
      MSG_HASH(MSG_REMOVABLE_STORAGE,
               "Removable Storage")
      MSG_HASH(MSG_INVALID_NICKNAME_SIZE,
               "Invalid nickname size.")
      MSG_HASH(MSG_IN_BYTES,
               "(字节)")
      MSG_HASH(MSG_IN_GIGABYTES,
               "(吉字节)")
      MSG_HASH(MSG_IN_MEGABYTES,
               "(兆字节)")
      MSG_HASH(MSG_LIBRETRO_ABI_BREAK,
               "is compiled against a different version of libretro than this libretro implementation.")
      MSG_HASH(MSG_LIBRETRO_FRONTEND,
               "为libretro而设计的前端")
      MSG_HASH(MSG_LOADED_STATE_FROM_SLOT,
               "加载状态从槽 #%d.")
      MSG_HASH(MSG_LOADED_STATE_FROM_SLOT_AUTO,
               "加载状态从槽 #-1 (auto).")
      MSG_HASH(MSG_LOADING,
               "正在加载")
      MSG_HASH(MSG_FIRMWARE,
               "一个或多个固件文件丢失")
      MSG_HASH(MSG_LOADING_CONTENT_FILE,
               "正在加载内容文件")
      MSG_HASH(MSG_LOADING_HISTORY_FILE,
               "正在读取历史文件")
      MSG_HASH(MSG_LOADING_STATE,
               "正在读取存档")
      MSG_HASH(MSG_MEMORY,
               "内存")
      MSG_HASH(MSG_MOVIE_FILE_IS_NOT_A_VALID_BSV1_FILE,
               "视频不是有效的BSV1文件。")
      MSG_HASH(MSG_MOVIE_FORMAT_DIFFERENT_SERIALIZER_VERSION,
               "视频格式看起来使用了不同的序列化版本。很有可能失败。")
      MSG_HASH(MSG_MOVIE_PLAYBACK_ENDED,
               "视频回放结束.")
      MSG_HASH(MSG_MOVIE_RECORD_STOPPED,
               "停止视频录制。")
      MSG_HASH(MSG_NETPLAY_FAILED,
               "初始化联机游戏失败。")
      MSG_HASH(MSG_NO_CONTENT_STARTING_DUMMY_CORE,
               "没有内容，启动虚拟核心。")
      MSG_HASH(MSG_NO_SAVE_STATE_HAS_BEEN_OVERWRITTEN_YET,
               "未覆盖任何存档。")
      MSG_HASH(MSG_NO_STATE_HAS_BEEN_LOADED_YET,
               "没有加载任何存档。")
      MSG_HASH(MSG_OVERRIDES_ERROR_SAVING,
               "保存覆盖错误。")
      MSG_HASH(MSG_OVERRIDES_SAVED_SUCCESSFULLY,
               "覆盖保存成功。")
      MSG_HASH(MSG_PAUSED,
               "暂停。")
      MSG_HASH(MSG_PROGRAM,
               "RetroArch")
      MSG_HASH(MSG_READING_FIRST_DATA_TRACK,
               "读取第一条数据轨...")
      MSG_HASH(MSG_RECEIVED,
               "接收完毕")
      MSG_HASH(MSG_RECORDING_TERMINATED_DUE_TO_RESIZE,
               "录制因改变大小而停止。")
      MSG_HASH(MSG_RECORDING_TO,
               "录制到")
      MSG_HASH(MSG_REDIRECTING_CHEATFILE_TO,
               "重定向金手指文件至")
      MSG_HASH(MSG_REDIRECTING_SAVEFILE_TO,
               "Redirecting save file to")
      MSG_HASH(MSG_REDIRECTING_SAVESTATE_TO,
               "Redirecting savestate to")
      MSG_HASH(MSG_REMAP_FILE_SAVED_SUCCESSFULLY,
               "Remap file saved successfully.")
      MSG_HASH(MSG_REMOVED_DISK_FROM_TRAY,
               "Removed disk from tray.")
      MSG_HASH(MSG_REMOVING_TEMPORARY_CONTENT_FILE,
               "移除临时内容文件")
      MSG_HASH(MSG_RESET,
               "重置")
      MSG_HASH(MSG_RESTARTING_RECORDING_DUE_TO_DRIVER_REINIT,
               "重启录制由于驱动器重新初始化。")
      MSG_HASH(MSG_RESTORED_OLD_SAVE_STATE,
               "重载旧的存档。")
      MSG_HASH(MSG_RESTORING_DEFAULT_SHADER_PRESET_TO,
               "Shaders: restoring default shader preset to")
      MSG_HASH(MSG_REVERTING_SAVEFILE_DIRECTORY_TO,
               "Reverting savefile directory to")
      MSG_HASH(MSG_REVERTING_SAVESTATE_DIRECTORY_TO,
               "Reverting savestate directory to")
      MSG_HASH(MSG_REWINDING,
               "正在回溯。")
      MSG_HASH(MSG_REWIND_INIT,
               "Initializing rewind buffer with size")
      MSG_HASH(MSG_REWIND_INIT_FAILED,
               "初始化回放缓存失败. 回放功能关闭.")
      MSG_HASH(MSG_REWIND_INIT_FAILED_THREADED_AUDIO,
               "Implementation uses threaded audio. Cannot use rewind.")
      MSG_HASH(MSG_REWIND_REACHED_END,
               "到达回放缓存末端.")
      MSG_HASH(MSG_SAVED_NEW_CONFIG_TO,
               "已保存新配置到")
      MSG_HASH(MSG_SAVED_STATE_TO_SLOT,
               "保存状态至槽 #%d.")
      MSG_HASH(MSG_SAVED_STATE_TO_SLOT_AUTO,
               "保存状态至槽 #-1 (auto).")
      MSG_HASH(MSG_SAVED_SUCCESSFULLY_TO,
               "成功保存至")
      MSG_HASH(MSG_SAVING_RAM_TYPE,
               "保存 RAM 类型")
      MSG_HASH(MSG_SAVING_STATE,
               "存档中")
      MSG_HASH(MSG_SCANNING,
               "扫描中")
      MSG_HASH(MSG_SCANNING_OF_DIRECTORY_FINISHED,
               "已完成对文件夹的扫描")
      MSG_HASH(MSG_SENDING_COMMAND,
               "发送指令")
      MSG_HASH(MSG_SEVERAL_PATCHES_ARE_EXPLICITLY_DEFINED,
               "Several patches are explicitly defined, ignoring all...")
      MSG_HASH(MSG_SHADER,
               "Shader")
      MSG_HASH(MSG_SHADER_PRESET_SAVED_SUCCESSFULLY,
               "Shader preset saved successfully.")
      MSG_HASH(MSG_SKIPPING_SRAM_LOAD,
               "跳过 SRAM 加载。")
      MSG_HASH(MSG_SLOW_MOTION,
               "慢动作。")
      MSG_HASH(MSG_SLOW_MOTION_REWIND,
               "慢动作回溯。")
      MSG_HASH(MSG_SRAM_WILL_NOT_BE_SAVED,
               "SRAM will not be saved.")
      MSG_HASH(MSG_STARTING_MOVIE_PLAYBACK,
               "视频回放.")
      MSG_HASH(MSG_STARTING_MOVIE_RECORD_TO,
               "Starting movie record to")
      MSG_HASH(MSG_STATE_SIZE,
               "State size")
      MSG_HASH(MSG_STATE_SLOT,
               "状态存档槽")
      MSG_HASH(MSG_TAKING_SCREENSHOT,
               "截屏。")
      MSG_HASH(MSG_TO,
               "到")
      MSG_HASH(MSG_UNDID_LOAD_STATE,
               "已撤销加载状态。")
      MSG_HASH(MSG_UNDOING_SAVE_STATE,
               "撤销即时存档")
      MSG_HASH(MSG_UNKNOWN,
               "未知")
      MSG_HASH(MSG_UNPAUSED,
               "取消暂停。")
      MSG_HASH(MSG_UNRECOGNIZED_COMMAND,
               "无法识别的指令")
      MSG_HASH(MSG_USING_CORE_NAME_FOR_NEW_CONFIG,
               "Using core name for new config.")
      MSG_HASH(MSG_USING_LIBRETRO_DUMMY_CORE_RECORDING_SKIPPED,
               "使用libretro虚拟核心。跳过录制。")
      MSG_HASH(MSG_VALUE_CONNECT_DEVICE_FROM_A_VALID_PORT,
               "Connect device from a valid port.")
      MSG_HASH(MSG_VALUE_DISCONNECTING_DEVICE_FROM_PORT,
               "从端口断开设备")
      MSG_HASH(MSG_VALUE_REBOOTING,
               "正在重启……")
      MSG_HASH(MSG_VALUE_SHUTTING_DOWN,
               "正在关机……")
      MSG_HASH(MSG_VERSION_OF_LIBRETRO_API,
               "libretro API版本")
      MSG_HASH(MSG_VIEWPORT_SIZE_CALCULATION_FAILED,
               "Viewport size calculation failed! Will continue using raw data. This will probably not work right ...")
      MSG_HASH(MSG_VIRTUAL_DISK_TRAY,
               "virtual disk tray.")
      MSG_HASH(MENU_ENUM_SUBLABEL_AUDIO_LATENCY,
               "Desired audio latency in milliseconds. Might not be honored if the audio driver can't provide given latency.")
      MSG_HASH(MENU_ENUM_SUBLABEL_AUDIO_MUTE,
               "禁音/取消禁音。")
      MSG_HASH(
              MENU_ENUM_SUBLABEL_AUDIO_RATE_CONTROL_DELTA,
              "Helps smooth out imperfections in timing when synchronizing audio and video at the same time. Be aware that if disabled, proper synchronization is nearly impossible to obtain."
      )
      MSG_HASH(
              MENU_ENUM_SUBLABEL_CAMERA_ALLOW,
              "允许或禁止核心使用摄像头。"
      )
      MSG_HASH(
              MENU_ENUM_SUBLABEL_LOCATION_ALLOW,
              "Allow or disallow location services access by cores."
      )
      MSG_HASH(
              MENU_ENUM_SUBLABEL_INPUT_MAX_USERS,
              "Maximum amount of users supported by RetroArch."
      )
      MSG_HASH(
              MENU_ENUM_SUBLABEL_INPUT_POLL_TYPE_BEHAVIOR,
              "Influence how input polling is done inside RetroArch. Setting it to 'Early' or 'Late' can result in less latency, depending on your configuration."
      )
      MSG_HASH(
              MENU_ENUM_SUBLABEL_INPUT_ALL_USERS_CONTROL_MENU,
              "Allows any user to control the menu. If disabled, only User 1 can control the menu."
      )
      MSG_HASH(
              MENU_ENUM_SUBLABEL_AUDIO_VOLUME,
              "Audio volume (in dB). 0 dB is normal volume, no gain applied."
      )
      MSG_HASH(
              MENU_ENUM_SUBLABEL_AUDIO_SYNC,
              "同步音频。推荐。"
      )
      MSG_HASH(
              MENU_ENUM_SUBLABEL_INPUT_AXIS_THRESHOLD,
              "How far an axis must be tilted to result in a button press."
      )
      MSG_HASH(
              MENU_ENUM_SUBLABEL_INPUT_BIND_TIMEOUT,
              "Amount of seconds to wait until proceeding to the next bind."
      )
      MSG_HASH(
              MENU_ENUM_SUBLABEL_INPUT_TURBO_PERIOD,
              "Describes the period of which turbo-enabled buttons toggle. Numbers are described in frames."
      )
      MSG_HASH(
              MENU_ENUM_SUBLABEL_INPUT_DUTY_CYCLE,
              "Describes how long the period of a turbo-enabled button should be. Numbers are described in frames."
      )
      MSG_HASH(
              MENU_ENUM_SUBLABEL_VIDEO_VSYNC,
              "同步显卡的视频输出到屏幕刷新率。推荐。"
      )
      MSG_HASH(
              MENU_ENUM_SUBLABEL_VIDEO_ALLOW_ROTATE,
              "Allow cores to set rotation. When disabled, rotation requests are ignored. Useful for setups where one manually rotates the screen."
      )
      MSG_HASH(
              MENU_ENUM_SUBLABEL_DUMMY_ON_CORE_SHUTDOWN,
              "Some cores might have a shutdown feature. If enabled, it will prevent the core from shutting RetroArch down. Instead, it loads a dummy core."
      )
      MSG_HASH(
              MENU_ENUM_SUBLABEL_CHECK_FOR_MISSING_FIRMWARE,
              "Check if all the required firmware is present before attempting to load content."
      )
      MSG_HASH(
              MENU_ENUM_SUBLABEL_VIDEO_REFRESH_RATE,
              "Vertical refresh rate of your screen. Used to calculate a suitable audio input rate. NOTE: This will be ignored if 'Threaded Video' is enabled."
      )
      MSG_HASH(
              MENU_ENUM_SUBLABEL_AUDIO_ENABLE,
              "启用音频输出。"
      )
      MSG_HASH(
              MENU_ENUM_SUBLABEL_AUDIO_MAX_TIMING_SKEW,
              "The maximum change in audio input rate. You may want to increase this to enable very large changes in timing, for example running PAL cores on NTSC displays, at the cost of inaccurate audio pitch."
      )
      MSG_HASH(
              MSG_FAILED,
              "已失败"
      )
      MSG_HASH(
              MSG_SUCCEEDED,
              "已成功"
      )
      MSG_HASH(
              MSG_DEVICE_NOT_CONFIGURED,
              "未配置"
      )
      MSG_HASH(
              MSG_DEVICE_NOT_CONFIGURED_FALLBACK,
              "not configured, using fallback"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST,
              "数据库 Cursor List"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST_ENTRY_DEVELOPER,
              "数据库 - 过滤器 : 开发者"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST_ENTRY_PUBLISHER,
              "数据库 - 过滤器 : 发布者"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_DISABLED,
              "禁用"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_ENABLED,
              "启用"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_CONTENT_HISTORY_PATH,
              "内容历史路径"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST_ENTRY_ORIGIN,
              "数据库 - 过滤器 : Origin")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST_ENTRY_FRANCHISE,
               "数据库 - 过滤器 : Franchise")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST_ENTRY_ESRB_RATING,
               "数据库 - 过滤器 : ESRB Rating")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST_ENTRY_ELSPA_RATING,
               "数据库 - 过滤器 : ELSPA Rating")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST_ENTRY_PEGI_RATING,
               "数据库 - 过滤器 : PEGI Rating")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST_ENTRY_CERO_RATING,
               "数据库 - 过滤器 : CERO Rating")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST_ENTRY_BBFC_RATING,
               "数据库 - 过滤器 : BBFC Rating")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST_ENTRY_MAX_USERS,
               "数据库 - 过滤器 : 最大用户数")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST_ENTRY_RELEASEDATE_BY_MONTH,
               "数据库 - 过滤器 : 发行月份")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST_ENTRY_RELEASEDATE_BY_YEAR,
               "数据库 - 过滤器 : 发行年代")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST_ENTRY_EDGE_MAGAZINE_ISSUE,
               "数据库 - 过滤器 : Edge Magazine Issue")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST_ENTRY_EDGE_MAGAZINE_RATING,
               "数据库 - 过滤器 : Edge Magazine Rating")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_DATABASE_CURSOR_LIST_ENTRY_DATABASE_INFO,
               "数据库信息")
      MSG_HASH(MSG_WIFI_SCAN_COMPLETE,
               "Wi-Fi 扫描完成。")
      MSG_HASH(MSG_SCANNING_WIRELESS_NETWORKS,
               "扫描无线网络...")
      MSG_HASH(MSG_NETPLAY_LAN_SCAN_COMPLETE,
               "联机游戏主机扫描完成。")
      MSG_HASH(MSG_NETPLAY_LAN_SCANNING,
               "扫描联机游戏主机...")
      MSG_HASH(MENU_ENUM_SUBLABEL_PAUSE_NONACTIVE,
               "当窗口失去焦点时暂停游戏。")
      MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_DISABLE_COMPOSITION,
               "Enable or disable composition (Windows only).")
      MSG_HASH(MENU_ENUM_SUBLABEL_HISTORY_LIST_ENABLE,
               "为游戏、图片、音乐和视频启用/禁用历史记录。")
      MSG_HASH(MENU_ENUM_SUBLABEL_CONTENT_HISTORY_SIZE,
               "游戏、图片、音乐和视频历史记录的数量限制。")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_INPUT_UNIFIED_MENU_CONTROLS,
               "统一菜单控制")
      MSG_HASH(MENU_ENUM_SUBLABEL_INPUT_UNIFIED_MENU_CONTROLS,
               "Use the same controls for both the menu and the game. Applies to the keyboard.")
      MSG_HASH(MENU_ENUM_SUBLABEL_VIDEO_FONT_ENABLE,
               "显示屏幕消息。")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NETWORK_USER_REMOTE_ENABLE,
               "用户 %d 远程允许")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_BATTERY_LEVEL_ENABLE,
               "显示电池电量")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SELECT_FILE,
               "选择文件")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SELECT_FROM_COLLECTION,
               "从收藏中选择")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_FILTER,
               "过滤器")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SCALE,
               "刻度")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_START_WHEN_LOADED,
               "联机游戏将在内容加载后开始。")
      MSG_HASH(MENU_ENUM_LABEL_VALUE_NETPLAY_LOAD_CONTENT_MANUALLY,
               "无法找到合适的核心或内容文件，手动加载。")
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_BROWSE_URL_LIST,
              "浏览URL"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_BROWSE_URL,
              "URL路径"
      )
      MSG_HASH(
              MENU_ENUM_LABEL_VALUE_BROWSE_START,
              "开始"
      )
      MSG_HASH(MENU_ENUM_LABEL_VALUE_SHADER_PIPELINE_BOKEH,
               "Bokeh")
      default:
#if 0
         RARCH_LOG("Unimplemented: [%d]\n", msg);
#endif
         break;
   }

   return "null";
}
