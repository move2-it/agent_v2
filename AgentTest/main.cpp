#include <gtest/gtest.h>
#include <QDebug>

int main(int argc, char *argv[])
{
    qSetMessagePattern("%{file}(%{line}): %{message}");

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
