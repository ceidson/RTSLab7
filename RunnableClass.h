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
#ifndef RUNNABLECLASS_H_
#define RUNNABLECLASS_H_

#include <thread>
#include <string>
#include <list>
#include <sys/types.h>

/**
 * This is the runnable class, which mimics the runnable interface from Java.  It is a virtual class which should not directly be instantiated.
 */
class RunnableClass {
protected:
	/**
	 * This is the instance of the thread that is to be executed by this class.  At instantiation it is NULL.  It will be instantiated when the start method of the class is invoked.
	 */
	std::thread *myThread=NULL;
	/**
	 * This method will determine whether or not the given thread is to continue executing.
	 */
	bool keepGoing=true;

	std::string myName;

	static std::list <RunnableClass*> runningThreads;

	pid_t myOSThreadID=0;

	/**
	 * This is the priority for the given task.  By default, zero uses the standard scheduler.
	 * Otherwise, 1-99 will use the real time scheduler.
	 */
	int myPriority=0;

	/**
	 * This private method will actually cause the priority of the given task to be changed.
	 */
	void changePriority();

private:
	/**
	 * This is a private method that will be used by start to invoke the run method.
	 */
	void invokeRun();

public:
	/**
	 * This method will print out to the console each of the running threads and their thread ID's.
	 */
	static void printThreads();


	/**
	 * This method will reset the thread information which is dynamic in nature and changes as the robot runs.
	 * This predominantely impacts threads which are not part of the Runnable class.
	 */
	static void resetAllThreadInformation();

	/**
	 * This method will print out information about the given thread.  The info will be dependent upon the given thread.
	 */
	virtual void printInformation();

	/**
	 * This is the default constructor for the class.
	 * @param threadName This is the name of the thread in a human readable format.
	 */
	RunnableClass(std::string threadName);
	/**
	 * This is the default destructor for the class.  It must properly clean up the instantiated thread.
	 */
	virtual ~RunnableClass();

	/**
	 * This virtual method is the start method.  It must be implemented in child classes.  The purpose of this method is to instantiate a new thread and invoke the run method.
	 */
	virtual void start();

	/**
	 * This method will start the given class at a given priority.
	 * @param priority
	 */
	virtual void start(int priority);
	/**
	 * This is the virtual run method.  It will execute the given code that is to be executed by this class.
	 */



	virtual void run()=0;
	/**
	 * This method will stop the execution of the given class.
	 */
	virtual void stop();
	/**
	 * This method will determine if the given thread is shutdown.
	 * @return The return will be true if the thread is shutdown or false otherwise.
	 */
	bool isShutdown();
	/**
	 * This method will block waiting for the given thread to terminate before continuing.
	 */
	void waitForShutdown();

	/**
	 * This method will be used to log that a given thread has started, obtaining the thread ID for the running thread.
	 */
	void logThreadStart();

	/**
	 * This method will set the priority for this thread accordingly.
	 * @param priority The priority is a value between 0 and 99.  If 0, a non-real time scheduling approach is used.
	 * If anything greater than 0, a sched_FIFO policy is used.
	 */
	void setPriority(int priority);

	/**
	 * This method will reset thread diagnostics back to their default values.
	 */
	virtual void resetThreadDiagnostics();

};

#endif /* RUNNABLECLASS_H_ */
