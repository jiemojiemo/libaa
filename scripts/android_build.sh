usage() {
  echo "Usage: ${0} [-a|--abi] [-b|--build_dir]" 1>&2
  exit 1
}

if [ $# -eq 0 ]
  then
    usage
fi

POSITIONAL_ARGS=()

while [[ $# -gt 0 ]]; do
  case $1 in
    -a|--abi)
      ANDROID_ABI="$2"
      shift # past argument
      shift # past value
      ;;
    -b|--build_dir)
      BUILD_DIR="$2"
      shift # past argument
      shift # past value
      ;;
    -*|--*)
      echo "Unknown option $1"
      usage
      exit 1
      ;;
    *)
      POSITIONAL_ARGS+=("$1") # save positional arg
      shift # past argument
      ;;
  esac
done

[ -z "$ANDROID_NDK_HOME" ] && echo "ANDROID_NDK_HOME need to be set" && exit 1
[ -z "$BUILD_DIR" ] && echo "BUILD_DIR need to be set" && exit 1
[ -z "$ANDROID_ABI" ] && echo "ANDROID_ABI need to be set" && exit 1

cmake -S . -B ${BUILD_DIR} \
  -DCMAKE_TOOLCHAIN_FILE="$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake" \
  -DANDROID_ABI=$ANDROID_ABI \
  -DANDROID_ARM_NEON=ON -DANDROID_PLATFORM=android-19 \
  -DCMAKE_INSTALL_PREFIX=${BUILD_DIR}/install \
  -DLIBAA_BUILD_TESTING=OFF \

cmake --build ${BUILD_DIR} --target install