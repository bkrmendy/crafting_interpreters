//
// Created by berci on 2021. 08. 28..
//

#include "gtest/gtest.h"
#include "ApprovalTests.hpp"

TEST(ParserTests, Example) {
    ApprovalTests::Approvals::verify(1);
}

TEST(ParserTests, OtherExample) {
    ApprovalTests::Approvals::verify("hello");
}