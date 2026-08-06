param(
    [string]$BuildDir = "build-test-vs",
    [string]$Configuration = "Debug"
)

$ErrorActionPreference = "Stop"

cmake -S . -B $BuildDir -G "Visual Studio 17 2022" -A x64 -DARNICS_BUILD_TESTS=ON
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

cmake --build $BuildDir --config $Configuration --parallel 4 -- /v:minimal
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

ctest --test-dir $BuildDir -C $Configuration --output-on-failure --output-junit "$BuildDir/test-results.xml"
exit $LASTEXITCODE
