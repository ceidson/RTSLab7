/**
 * @file
 * @author  Walter Schilling (schilling@msoe.edu)
 * @version 1.0
 *
 * @section LICENSE This is a piece of a template solution to the lab.
 *
 *
 * @section DESCRIPTION
 *      This is the implementation for the periodic tasks.
 */


#include "PeriodicTask.h"
#include <iostream>
#include <chrono>
#include <iomanip>

PeriodicTask::PeriodicTask(std::string threadName) :
		RunnableClass(threadName) {
}

PeriodicTask::~PeriodicTask() {
// Nothing to be done in the destructor.
}

void PeriodicTask::setTaskPeriod(uint32_t period) {
	// Set the task period to the parameter passed in.
	taskPeriod = period;
}

void PeriodicTask::waitForNextExecution(
		std::chrono::microseconds remainingSleepTime) {
	// Sleep for the given amount of time.
	std::this_thread::sleep_for(remainingSleepTime);
}

void PeriodicTask::printInformation() {
	std::cout << myOSThreadID << "\t" << std::setw(18) << myName << "\t Priority: " << myPriority
			<< "\tPeriodic: TP: " << taskPeriod << "ms\t LET "
			<< lastExecutionTime << "us\t WCET " << worstCaseExecutionTime
			<< "us\t LWT " << lastWallTime.count() << "us\t WCWT "
			<< worstCaseWallTime.count() << "us\n";
}

void PeriodicTask::resetThreadDiagnostics() {
	// Reset all diagnostic variables to zero.
	worstCaseExecutionTime = 0;
	lastExecutionTime = 0;
	lastWallTime = std::chrono::microseconds(0);
	worstCaseWallTime = std::chrono::microseconds(0);
}

/**
 * This is a private method that will be used by start to invoke the run method.
 */
void PeriodicTask::invokeRun() {
	// Indicate that the thread has been started.
	this->logThreadStart();
	changePriority();
	keepGoing = true;
	while (keepGoing == true) {
		// Get the start time.
		clockid_t threadTimer;
		struct timespec startTs;
		struct timespec endTs;

		// Obtain the cpu time at the start of this periodic task. This is for CPU time measurement.
		pthread_getcpuclockid(pthread_self(), &threadTimer);
		clock_gettime(threadTimer, &startTs);

		// The following gets the wall time, for determining next execution time.
		std::chrono::high_resolution_clock::time_point start =
				std::chrono::high_resolution_clock::now();

		// Now run the task.
		run();

		// Now get the end CPU time entry.
		clock_gettime(threadTimer, &endTs);
		long deltaInus = (endTs.tv_sec * 1000000 + endTs.tv_nsec / 1000)
				- (startTs.tv_sec * 1000000 + startTs.tv_nsec / 1000);

		if (deltaInus > worstCaseExecutionTime) {
			worstCaseExecutionTime = deltaInus;
		}
		lastExecutionTime = deltaInus;

		// Now determine what time it is.
		std::chrono::high_resolution_clock::time_point end =
				std::chrono::high_resolution_clock::now();

		// Now figure out the difference.
		std::chrono::microseconds executionTime = std::chrono::duration_cast<
				std::chrono::microseconds>(end - start);

		lastWallTime = executionTime;
		if (lastWallTime > worstCaseWallTime) {
			worstCaseWallTime = lastWallTime;
		}

		// Figure out how long to sleep.
		std::chrono::microseconds remainingSleepTime =
				std::chrono::milliseconds(taskPeriod) - executionTime;

		// Sleep until the next execution should occur.
		waitForNextExecution(remainingSleepTime);
	}
}

void PeriodicTask::start() {
	myThread = new std::thread(&PeriodicTask::invokeRun, this);
}

void PeriodicTask::start(int priority) {
	if ((priority >= 0) && (priority <= 99)) {
		myPriority = priority;
	}
	myThread = new std::thread(&PeriodicTask::invokeRun, this);
}


