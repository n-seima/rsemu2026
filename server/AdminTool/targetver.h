#pragma once

// 以下宏定要求的最低平台。要求的最低平台
// 是具有行用程序所需功能的 Windows、Internet Explorer 等品的
// 最早版本。通在指定版本及更低版本的平台上用所有可用的功能，宏可以
// 正常工作。

// 如果必要低于以下指定版本的平台，修改下列定。
// 有不同平台的最新信息，考 MSDN。
#ifndef WINVER                          // 指定要求的最低平台是 Windows Vista。
#define WINVER 0x0600           // 此更改相的，以适用于 Windows 的其他版本。
#endif

#ifndef _WIN32_WINNT            // 指定要求的最低平台是 Windows Vista。
#define _WIN32_WINNT 0x0600     // 此更改相的，以适用于 Windows 的其他版本。
#endif

#ifndef _WIN32_WINDOWS          // 指定要求的最低平台是 Windows 98。
#define _WIN32_WINDOWS 0x0410 // 此更改适的，以适用于 Windows Me 或更高版本。
#endif

#ifndef _WIN32_IE                       // 指定要求的最低平台是 Internet Explorer 7.0。
#define _WIN32_IE 0x0700        // 此更改相的，以适用于 IE 的其他版本。
#endif
