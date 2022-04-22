# 通过 Git 获取版本信息
find_package(Git)
# 生成版本描述字符串类似 TAG-X-gHASH
execute_process(COMMAND ${GIT_EXECUTABLE} describe --tags --always --dirty=-dev
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE  GIT_REPO_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )
# 获取最新 commit 日期，YYYY-MM-DD-HH-hh:mm:ss
execute_process(COMMAND ${GIT_EXECUTABLE} log -1 --format=%cd --date=format:%Y-%m-%d-%H:%M:%S
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE  GIT_REPO_DATE
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )
# 获取最新 commit Hash
execute_process(COMMAND ${GIT_EXECUTABLE} log -1 --format=%H
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE  GIT_REPO_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )

message("GIT_REPO_VERSION = ${GIT_REPO_VERSION}")
message("GIT_REPO_DATE = ${GIT_REPO_DATE}")
message("GIT_REPO_HASH = ${GIT_REPO_HASH}")
message("CMAKE_PROJECT_VERSION = ${CMAKE_PROJECT_VERSION}")
