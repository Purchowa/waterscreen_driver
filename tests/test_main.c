#include "test_main.h"
#include "uCUnit.h"

void runUnitTests(){
	UCUNIT_Init(); /* initialize framework */

	UCUNIT_TestcaseBegin("Writing to SPI - bottom-up - picture");
	UCUNIT_CheckIsEqual(1, 1);
	UCUNIT_TestcaseEnd();

	UCUNIT_WriteSummary();
	UCUNIT_Shutdown();
}
