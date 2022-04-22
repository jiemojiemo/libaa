#!/bin/bash

set +e

# Work out the newest common ancestor between the detached HEAD that this CI job
# has checked out, and the upstream target branch.
# `${CI_MERGE_REQUEST_TARGET_BRANCH_NAME}` is only defined if we’re running in
# a merge request pipeline; fall back to `${CI_DEFAULT_BRANCH}` otherwise.

echo "Target branch: ${CI_MERGE_REQUEST_TARGET_BRANCH_NAME:-${CI_DEFAULT_BRANCH}}"
newest_common_ancestor_sha=$(git merge-base origin/${CI_MERGE_REQUEST_TARGET_BRANCH_NAME:-${CI_DEFAULT_BRANCH}} HEAD)

# Apply git clang-format
clang_format_error=$(git-clang-format --commit $newest_common_ancestor_sha --extensions "cpp,hpp,c,h" --quiet --diff --style=file | grep -v --color=never "no modified files to format")

# Check for linter errors
if [ -n "$clang_format_error" ]; then
    echo "Detected formatting issues; please fix" 
    git-clang-format --commit $newest_common_ancestor_sha --extensions "cpp,hpp,c,h" --quiet --diff --style=file
    exit 1
else
    echo "Formatting is correct"
fi