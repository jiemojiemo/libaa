# android armv7
cmake -S . -B build_android_armv7 \
  -DCMAKE_TOOLCHAIN_FILE="$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake" \
  -DANDROID_ABI="armeabi-v7a" -DANDROID_ARM_NEON=ON -DANDROID_PLATFORM=android-19 \
  -DBUILD_TESTING=OFF \
  -DAE_BUILD_REGRESSION_DEMO=OFF

cmake --build build_android_armv7 -- -j 8

# android aarch64
cmake -S . -B build_android_aarch64 \
  -DCMAKE_TOOLCHAIN_FILE="$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake" \
  -DANDROID_ABI="arm64-v8a" -DANDROID_ARM_NEON=ON -DANDROID_PLATFORM=android-19 \
  -DBUILD_TESTING=OFF \
  -DAE_BUILD_REGRESSION_DEMO=OFF

cmake --build build_android_aarch64 -- -j 8