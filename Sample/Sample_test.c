
#include "../../utilities/ARM_Header_Files/LPC214x.h"
#include "../../utilities/Unity-master/src/unity.h"
#include "Temp.h"
#include "../../utilities/CMock/lib/mocks/Mock_ms_delay.h"
#include <stdio.h>

void test_buzzer_on(void)
{
	BUZZER_ON();	
	TEST_ASSERT_EQUAL_HEX32 (1<<25, IO0SET);
}

void test_buzzer_off(void)
{
	BUZZER_OFF();
	TEST_ASSERT_EQUAL_HEX32 (1<<25, IO0CLR);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_buzzer_on);
	RUN_TEST(test_buzzer_off);
	return UNITY_END();
}
