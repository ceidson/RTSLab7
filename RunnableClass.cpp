/**
 * @file
 * @author  Walter Schilling (schilling@msoe.edu)
 * @version 1.0
 *
 * @section LICENSE This is a piece of a template solution to the lab.
 *
 *
 * @section DESCRIPTION
 *      This class represents a base class which is virtual in nature.  It allows simple classes to be created which mimic the runnable interface in Java.  This simplifies multithreading and will allow for setting the priority and other things to occur in future projects.
 */
#include "RunnableClass.h"
#include <thread>
#include <string>
#include <iostream>
#include <iomanip>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

/**
 * This is a file scopes variable which holds a list of the threads that are running.
 */
std::list<RunnableClass*> RunnableClass::runningThreads;

RunnableClass::RunnableClass(std::string threadName) {
	// Set the name of the thread accordingly.
	myName = threadName;

	// Add the thread to the list of threads that are executing.
	runningThreads.push_front(this);
}

void RunnableClass::printThreads() {
	std::cout << "===============================================================================================\nThread Diagnostic Information:\n";
	for (std::list<RunnableClass*>::iterator it = runningThreads.begin();
			it != runningThreads.end(); it++) {
		RunnableClass* rc = *it;
		rc->printInformation();
	}
	std::cout << "===============================================================================================\n";

}

void RunnableClass::resetAllThreadInformation()
{
	for (std::list<RunnableClass*>::iterator it = runningThreads.begin();
			it != runningThreads.end(); it++) {
		RunnableClass* rc = *it;
		rc->resetThreadDiagnostics();
	}
}

void RunnableClass::resetThreadDiagnostics()
{
	// For a Runnable class, this method does nothing, as there currently are no diagnostics for runnable threads.
}

void RunnableClass::printInformation()
{
	std::cout << myOSThreadID << "\t" << std::setw(18) << myName << "\t Priority: " << myPriority << "\n";
}

RunnableClass::~RunnableClass() {
	/**
	 * If the thread is not NULL, delete it.
	 */
	if (myThread != NULL) {
		delete myThread;
	}
}

void RunnableClass::changePriority()
{
	// If not zero, change the priority accordingly.
	if (myOSThreadID!=0)
	{
		if (myPriority == 0) {
			// Setup the operating thread to be a real time thread.
			struct sched_param p;
			p.__sched_priority = myPriority;

			if (sched_setscheduler(myOSThreadID, SCHED_OTHER, &p) != 0) {
				// setup failed.
			}
		} else {
			// Setup the operating thread to be a real time thread.
			struct sched_param p;
			p.__sched_priority = myPriority;

			if (sched_setscheduler(myOSThreadID, SCHED_FIFO, &p) != 0) {
				// setup failed.
			}
		}
	}
}

void RunnableClass::logThreadStart() {
	// Obtain the thread id by making a system call.
	myOSThreadID = syscall(SYS_gettid);
	changePriority();
}

void RunnableClass::stop() {
	/**
	 * Stop the thread by changing the value of keepGoing.
	 */
	keepGoing = false;
}

void RunnableClass::start() {
	myThread = new std::thread(&RunnableClass::invokeRun, this);
}

void RunnableClass::start(int priority)
{
	if ((priority >= 0) && (priority <= 99)) {
		myPriority = priority;
	}
	myThread = new std::thread(&RunnableClass::invokeRun, this);
}

/**
 * This is a private method that will be used by start to invoke the run method.
 */
void RunnableClass::invokeRun()
{
	changePriority();
	this->logThreadStart();
	run();
}


bool RunnableClass::isShutdown() {
	return !keepGoing;
}

void RunnableClass::waitForShutdown() {
	if (myThread != NULL) {
		/**
		 * Join the thread and wait for shutdown.
		 */
		myThread->join();
	}
}

/**
 * This method will set the priority for this thread accordingly.
 * @param priority The priority is a value between 0 and 99.  If 0, a non-real time scheduling approach is used.
 * If anything greater than 0, a sched_FIFO policy is used.
 */
void RunnableClass::setPriority(int priority) {
	// Check to make certain the thread is valid and that the priority is valid.
	if ((priority >= 0) && (priority <= 99)) {
		myPriority = priority;
		changePriority();
	}
}

