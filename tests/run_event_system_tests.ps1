param(
    [string]$BuildDir = "build-test-vs",
    [string]$Configuration = "Debug"
)

$ErrorActionPreference = "Stop"

$Root = Split-Path -Parent $PSScriptRoot
$CenterEvent = Join-Path $Root "arnics-os\dePartment\centerEvent\centerEvent.h"
$EventList = Join-Path $Root "arnics-os\dePartment\centerEvent\eventProcess\eventList.h"
$CenterEventBackup = [System.IO.File]::ReadAllBytes($CenterEvent)
$EventListBackup = [System.IO.File]::ReadAllBytes($EventList)
$Utf8 = New-Object System.Text.UTF8Encoding($false)

try {
    $centerText = [System.IO.File]::ReadAllText($CenterEvent)
    $messageTypes = @"
typedef struct { uint32_t request; uint32_t response; } test_external_employee_msg_t;
typedef struct { uint32_t request; uint32_t response; } test_internal_employee_msg_t;
typedef struct { uint32_t request; uint32_t response; } test_hired_employee_msg_t;

void test_external_employee(void *argv);
void test_internal_employee(void *argv);
void test_hired_employee(void *argv);
"@
    $centerText = [regex]::Replace($centerText, '(?s)(typedef struct\s*\{\s*bool illegal_msg;\s*\}\s*NULL_msg_t;)', '$1' + "`r`n" + $messageTypes)
    $centerText = $centerText.Replace("    NULL_msg_t NULL_msg;", @"
    NULL_msg_t NULL_msg;
    test_external_employee_msg_t test_external_employee_msg;
    test_internal_employee_msg_t test_internal_employee_msg;
    test_hired_employee_msg_t test_hired_employee_msg;
"@)
    [System.IO.File]::WriteAllText($CenterEvent, $centerText, $Utf8)

    $eventListText = @"
#ifndef _EVENLIST_H
#define _EVENLIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Inc/projDefine.h"
#include "Inc/typedef.h"

#define EVENT_INTERNAL_EMPLOY 1
#define EVENT_EXTERNAL_EMPLOY 2
#define EVENT_HIRED_EMPLOY    3

#define X(func, employ_kind, needRsp) \
    {#func, func, employ_kind, needRsp},

#define Y(func, priority, stack_size) \
    {#func, priority, stack_size},

#define REGISTER_ENTRIES \
    X(test_external_employee, EVENT_EXTERNAL_EMPLOY, true) \
    X(test_internal_employee, EVENT_INTERNAL_EMPLOY, true) \
    X(test_hired_employee, EVENT_HIRED_EMPLOY, true)

#define HIRD_EMPLOY_STATIC_REFERENCE \
    Y(test_hired_employee, rtosPriorityNormal, 1024)

#ifdef __cplusplus
}
#endif
#endif
"@
    [System.IO.File]::WriteAllText($EventList, $eventListText, $Utf8)

    cmake -S $Root -B (Join-Path $Root $BuildDir) -G "Visual Studio 17 2022" -A x64 -DARNICS_BUILD_TESTS=ON -DARNICS_BUILD_EVENT_SYSTEM_TESTS=ON
    if ($LASTEXITCODE -ne 0) { throw "CMake 配置失败：$LASTEXITCODE" }

    cmake --build (Join-Path $Root $BuildDir) --config $Configuration --target arnics-event-system-tests --parallel 4 -- /v:minimal
    if ($LASTEXITCODE -ne 0) { throw "事件中心系统测试构建失败：$LASTEXITCODE" }

    ctest --test-dir (Join-Path $Root $BuildDir) -C $Configuration -R "^arnics-event-system-tests$" --output-on-failure --output-junit (Join-Path $Root "$BuildDir\event-system-results.xml")
    if ($LASTEXITCODE -ne 0) { throw "事件中心系统测试失败：$LASTEXITCODE" }
}
finally {
    [System.IO.File]::WriteAllBytes($CenterEvent, $CenterEventBackup)
    [System.IO.File]::WriteAllBytes($EventList, $EventListBackup)
}
