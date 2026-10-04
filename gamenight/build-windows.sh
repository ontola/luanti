#!/usr/bin/env bash
set -euo pipefail
source=$(cd -- "$(dirname -- "$0")/.." && pwd)
cache=${1:?Usage: gamenight/build-windows.sh NEW_BUILD_DIRECTORY}
mkdir -p "$cache"
cache=$(cd "$cache" && pwd)
if [[ -e "$cache/build" ]]; then
  echo "Use a fresh build directory to preserve existing artifacts." >&2
  exit 1
fi
c++ -std=c++17 -I"$source/irr/include" "$source/gamenight/test_binding.cpp" -o "$cache/test-binding"
"$cache/test-binding"
c++ -std=c++17 -I"$source/irr/include" "$source/gamenight/test_host_frame.cpp" -o "$cache/test-host-frame"
"$cache/test-host-frame" "$cache/test.frame"
c++ -std=c++17 -I"$source/irr/include" "$source/gamenight/test_navigation.cpp" -o "$cache/test-navigation"
"$cache/test-navigation"
c++ -std=c++17 -I"$source/irr/include" "$source/gamenight/test_layout.cpp" -o "$cache/test-layout"
"$cache/test-layout"
mkdir -p "$cache/toolchain"
bash "$source/util/buildbot/download_toolchain.sh" "$cache/toolchain"
export PATH="$cache/toolchain/bin:$PATH"
export EXISTING_MINETEST_DIR="$source"
bash "$source/util/buildbot/buildwin64.sh" "$cache/build" -DCMAKE_BUILD_TYPE=Release -DENABLE_GETTEXT=OFF
git -C "$source" rev-parse HEAD > "$cache/SOURCE_REVISION.txt"
git -C "$source" diff --quiet
git -C "$source" diff --cached --quiet
cd "$cache/build/build"
sha256sum luanti-*-win64.zip > SHA256SUMS.txt
