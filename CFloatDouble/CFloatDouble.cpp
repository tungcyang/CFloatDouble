// CFloatDouble.cpp : Defines the entry point for the console application.
//
// This project illustrates the difference in computation speeds for float and double.

#include <stdio.h>
#include <windows.h>

#define ITERATION_COUNT					10000000000	// Both the float and double computations will be repeated so many times.
													// Adjust this constant appropriately.
#define TEST_ITERATION_COUNT			100000000	// A test workload is executed by TEST_ITERATION_COUNT times.  We expect
													// the time elapsed for the formal workload should be roughly
													// (ITERATION_COUNT/TEST_ITERATION_COUNT) times that of the test workload.
													// This works as a guard if the optimizing compiler becomes too smart and
													// aggressive to eliminate the expressions in our workloads.

										// We will make use of the workload in "Optimized C++" by Kurt Guntheroth on page 179.
										// Note that we need "volatile" here or the optimizing compiler ignore the expressions
										// defined in the macro because they do not do useful computations.
#define WORKLOAD(type)					{ \
											volatile type	d; \
											type	t, a = -9.8f, v0 = 0.0f, d0 = 100.0f; \
											for (t = 0.0; t < 3.01f; t += 0.1f) \
												d = a*t*t + v0*t + d0; \
										}


// Function elapsedTime() evaluates the elapsed time between two DWORD timing measurements as returned
// from GetTickCount().  It takes into account the possible wraparound happening between the two
// GetTickCount() calls.
DWORD	elapsedTime(DWORD dStartTime, DWORD dStopTime)
{
	if (dStopTime > dStartTime)
		return (dStopTime - dStartTime);
	else
	{
		// A wraparound occurred between the two GetTickCount() calls.
		return (dStopTime + (ULONG_MAX - dStartTime) + 1);
	}
}


int main()
{
	unsigned long long int	i;		// ULLONG_MAX is 18446744073709551615 (264-1) or greater
	DWORD	dStartTime, dStopTime;
	DWORD	dFloatTestTime, dFloatTime, dDoubleTime;


	// Beginning of test float workload.
	dStartTime = GetTickCount();
	for (i = 0; i < TEST_ITERATION_COUNT; i++)
		WORKLOAD(float)

	dStopTime = GetTickCount();
	dFloatTestTime = elapsedTime(dStartTime, dStopTime);

	// Beginning of float workload.
	dStartTime = GetTickCount();
	for (i = 0; i < ITERATION_COUNT; i++)
		WORKLOAD(float)

	dStopTime = GetTickCount();
	dFloatTime = elapsedTime(dStartTime, dStopTime);

	// Beginning of double workload.
	dStartTime = GetTickCount();
	for (i = 0; i < ITERATION_COUNT; i++)
		WORKLOAD(double)

	dStopTime = GetTickCount();
	dDoubleTime = elapsedTime(dStartTime, dStopTime);

	fprintf(stdout, "The test float workload takes %d milliseconds to run.\n", dFloatTestTime);
	fprintf(stdout, "The float workload takes %d milliseconds to run.\n", dFloatTime);
	fprintf(stdout, "The double workload takes %d milliseconds to run.\n", dDoubleTime);
	fprintf(stdout, "\n");

	fprintf(stdout, "The time the float workload takes is roughly %f times\n", ((float) dFloatTime / dFloatTestTime));
	fprintf(stdout, "that for the test float workload;\n");
	fprintf(stdout, "\tin theory it should be %f.\n", (float) (ITERATION_COUNT/TEST_ITERATION_COUNT));
	fprintf(stdout, "\n");

	fprintf(stdout, "The time the double workload takes is roughly %f times\n", ((float) dDoubleTime / dFloatTime));
	fprintf(stdout, "that for the float workload;\n");

    return 0;
}

