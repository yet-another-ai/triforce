#include <gtest/gtest.h>

extern "C"
{
#include <triforce.h>
}

TEST(CompileTest, ItHasGitCommitHash)
{
    EXPECT_NE(0, strlen(TRIFORCE_GIT_COMMIT_HASH));
}
