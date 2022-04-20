# android armv7
cmake -S . -B android_build \
  -DCMAKE_TOOLCHAIN_FILE="$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake" \
  -DANDROID_ABI="armeabi-v7a" -DANDROID_ARM_NEON=ON -DANDROID_PLATFORM=android-19 \
  -DBUILD_TESTING=OFF \
  -DAE_BUILD_REGRESSION_DEMO=OFF

cmake --build android_build --verbose -- -j 8

## android aarch64
#cmake -S . -B android_build \
#  -DCMAKE_TOOLCHAIN_FILE="$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake" \
#  -DANDROID_ABI="arm64-v8a" -DANDROID_ARM_NEON=ON -DANDROID_PLATFORM=android-19 \
#  -DAE_BUILD_TESTING=OFF \
#  -DAE_BUILD_REGRESSION_DEMO=OFF
#
#cmake --build android_build -- -j 8