/**
 * @file
 * @author  Walter Schilling (schilling@msoe.edu)
 * @version 1.0
 *
 * @section LICENSE This is a piece of a template solution to the lab.
 *
 *
 * @section DESCRIPTION
 *      This file defines the behavior for a periodic task.  A periodic task is one in which the run method is called periodically by the task manager.
 */

#ifndef PERIODICTASK_H_
#define PERIODICTASK_H_

#include "RunnableClass.h"

#include <chrono>

class PeriodicTask: public RunnableClass {
public:
	/**
	 * This is the default constructor for the class.
	 * @param threadName This is the name of the thread in a human readable format.
	 */
	PeriodicTask(std::string threadName);

	/**
	 * This method will clean up from the periodic task and its execution.
	 */
	virtual ~PeriodicTask();

	/**
	 * This will set the period for the given task.
	 * @param period This is the period, given in ms, between executions of this task.
	 */
	void setTaskPeriod(uint32_t period);

	/**
	 * This method will suspend execution until the next period has been reached.  It will do this by blocking.
	 */
	void waitForNextExecution(std::chrono::microseconds remainingSleepTime);

	virtual void start();

	/**
	 * This method will print out information about the given thread.  The info will be dependent upon the given thread.
	 */
	virtual void printInformation();

	/**
	 * This method will reset thread diagnostics back to their default values.  The wall times and CPU times will be set to 0.
	 */
	virtual void resetThreadDiagnostics();

	/**
	 * This method will start the given class at a given priority.
	 * @param priority
	 */
	virtual void start(int priority);

private:
	/**
	 * This variable sets the period for the task.  The period defines the length of time from one invocation until the next invocation.  The task period is given in ms.
	 */
	uint32_t taskPeriod = 0;

	/**
	 * This variable stores the worst case execution time from a CPU time standpoint.
	 */
	long worstCaseExecutionTime = 0;
	/**
	 * This variable holds the last execution value for the CPU, showing the time it took to execute.
	 */
	long lastExecutionTime = 0;
	/**
	 * This variable holds the last wall time measurement for the given periodic task.  The wall time is the time from the start to the end of the task running.
	 */
	std::chrono::microseconds lastWallTime = std::chrono::microseconds(0);
	/**
	 * This variable holds the worst case wall time.  This is the worst case from start to finish of the given periodic task.
	 */
	std::chrono::microseconds worstCaseWallTime = std::chrono::microseconds(0);

	/**
	 * This is a private method that will be used by start to invoke the run method.
	 */
	void invokeRun();

};

#endif /* PERIODICTASK_H_ */
