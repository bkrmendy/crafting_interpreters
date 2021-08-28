//
// Created by berci on 2021. 08. 28..
//

#define APPROVALS_GOOGLETEST_EXISTING_MAIN
#include "ApprovalTests.hpp"
#include "gtest/gtest.h"
int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest();

    ApprovalTests::initializeApprovalTestsForGoogleTests();

    return RUN_ALL_TESTS();
}