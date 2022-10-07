#include <stdio.h>

#include "echangeContenu.h"
#include "unity/unity.h"


void setUp() {}
void tearDown() {}

void testEchangeContenu() {
 	int a=4,b=14;

    // TODO

    TEST_ASSERT_EQUAL_INT64(a, 14);
    TEST_ASSERT_EQUAL_INT64(b, 4);
}

int main() {
   UNITY_BEGIN();
   RUN_TEST(testEchangeContenu);
   UNITY_END();
}
