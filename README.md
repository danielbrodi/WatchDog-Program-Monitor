# Program Monitoring Watchdog
A multi-threaded program which monitors another program to see if its out of control or have stopped operation, and resets it if needed.   
Written in C and includes synchronization and concurrency tools, signals and a [custom module of a callbacks scheduler](./scheduler/) which handles the communication between the processes.

# How to invoke and to destroy the Watchdog
By the [API](./watchdog.h), in order to create the watchdog and make it watch and guard client's application, the client needs to put the critical section,
the code that he wants to protect, between `KeepMeAlive()` and `DNR()` calls.

At the beginning of the critical section that needs to be guarded from program termination, the client needs to order the watchdog to start watching the program and basically to terminate itself.
It is done by the `KeepMeAlive(...)` function.

```c
/*	Creates a Watch Dog program that will watch your program and restart it
 *	if it gets stuck or terminates.
 *	signal_intervals is necessary to determine the time that wait time between
 *	signals sending. num_allowed_misses is the amount of signals that the
 *	Watch Dog will not recevie before deciding that the other side is down,
 *	and will restart it.	*/
void KeepMeAlive(int argc, char *argv[], size_t signal_intervals,size_t num_allowed_misses);
```
At the end of the critical section that needs to be guarded from program termination, the client needs to order the watchdog to stop watching the program and basically to terminate itself.
It is done by the `DNR()` function.

```c
/*	DNR - Do not resuscitate.
 *	Used to stop the Watch Dog from watching the program.
 *	Returns '0' if DNR request succeeded and WD won't watch the program anymore.
 *	Returns '1' if failed. 	*/
int DNR(void);
```

An example for a correct use can be found in the following [user app code](./user_app.c).

# Compiling and Running
A [Makefile](./makefile) is available and can completely compile the needed resources together and provide an executable that will be guarded by the watchdog.

- ``make release`` will provide an exectuable file that won't run code which is defined as `DEBUG` by the debug macro which can be found [here](./wd_internal.h).
- ``make debug`` will provide an exectuable file that **will run code which is defined as `DEBUG` and also a code which was not defined as `DEBUG`**.
- ``make clean`` will remove and delete all shared objects files and object file (.o) which were created during the process of building the program. 

#### The resources which will be compiled and linked together are:

* A [dynamic library](./libds.so) which contains the scheduler moudle is provided to the client as well, and can be also created with the [scheduler's makefile](./scheduler/makefile) (which is not provided to the client).
* A [shared object](./wd_internal.o) of the [common operations and functions](./wd_internal.h) which are used **both** by the Watchdog and the User app. These operations handle the creation, destruction and communication of the processes. It handles the creation of the scheduler, along with sending and receiving singals. The shared object **is** provided to the client, but the [header(api) file](./wd_internal.h) and the [source of it](./wd_internal.c) are for internal use and are not provided.
* User's program code which needs to include a call to **KeepMeAlive(...)** and **DNR()** in order call and to stop the watchdog.

### Only limition is to keep `watchdog.o` file in the same directory with the user's executable file.
