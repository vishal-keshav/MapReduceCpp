#include <gtest/gtest.h>

#include <HelloWorld.hpp>

TEST(SampleTest1, DemoTest) {
    EXPECT_TRUE(true);
    EXPECT_TRUE(true);
}

TEST(HelloWorldTest, TestParamGetSet) {
    HelloWorld helloWorldObj;
    helloWorldObj.setPrivateVariable(100);
    EXPECT_EQ(helloWorldObj.getPrivateVariable(), 100);
}
