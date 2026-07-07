*This activity has been created as part of the 42 curriculum by mschyns*

# Description

The goal of the project is to learn multithreading using POSIX threads, mutexes, and intelligent scheduling, and to master resource synchronization before resources are exhausted.

There are *n* coders competing for resources located to their immediate right and left. The coders must compile, debug, and refactor within a time limit; if they fail to do so, they burn out.

The resources have a cooldown.

The program terminates if all the code segments have compiled at least x times, or if one of them experiences burnout.

Coders do not communicate with each other.

Coders do not know if another coder is about to burn out.
# Instructions
To launch the project, use the provided Makefile. The commands are as follows:

     make

or

     make all

It will create the Codexion executable.

     make clean
    
To remove all the file .o

      make fclean

To remove everything

      make re

To fclean all -> Restart


So, when you have make, you have the executable **codexion**.

You can run the project by:

      ./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler

The project takes:

- **number_of_coders**: The number of coders and also the number of dongles

- **time_to_burnout**: (in milliseconds) if a coder did not start compiling within time_to_burnout milliseconds since the beginning of their last compile or the beginning of the simula-tion, they burn out.

- **time_to_compile**: (in milliseconds) The time it takes for a coder to compile. During that time, they must hold two dongles.

- **time_to_debug**: (in milliseconds) The time a coder will spend debugging.

- **time_to_refactor**: (in milliseconds) The time a coder will spend refactoring. After completing the refactoring phase, the coder will immediately attempt to acquire dongles and start compiling again.

- **number_of_compiles_required**: If all coders have compiled at least this many times, the simulation stops. Otherwise, it stops when a coder burns out

- **dongle_cooldown**: (in milliseconds) After being released, a dongle is unavailable until its cooldown has passed

- **scheduler**: The arbitration policy used by dongles to decide who gets them when multiple coders request them. The value must be exactly one of: fifo or edf.
               
               fifo means First In, First Out: the dongle is granted to the coder whose request arrived first.
               
               edf means Earliest Deadline First with deadline = last_compile_start + time_to_burnout.

# Resources

Multithreading with POSIX : https://franckh.developpez.com/tutoriels/posix/pthreads/

More informations : https://www.codequoi.com/threads-mutex-et-programmation-concurrente-en-c/

AI USE: Help to review the basics of c

# Additionnal Section

## Blocking cases handled

I ran into quite a few problems during the project.

1) Race Condition

Issue with two threads using a shared variable

I had to learn how to use mutexes correctly—specifically, locking and unlocking shared variables.

2) Starvation and Deadlock

During development, I encountered issues with starvation and contention. One coder would pick up the dongle to their left, while another picked up the one to their right, causing a deadlock for both. Two resources and the two coders ended up blocked.

To solve this problem, I ensured that, depending on the coder's rank, they start by picking up the dongle either to their left or to their right. This allowed the resources to be acquired without deadlocks.

3) Burnout detection

The problem was that my monitoring system perfectly detected burnout, but I needed a way to make the coders aware that they were experiencing burnout and to halt all processes.

To achieve this, a shared variable protected by a mutex is used. Before performing any action, the coder checks whether or not they are in a state of burnout.

## Thread synchronization mechanisms

To avoid race conditions in this project and ensure that resources have reliable, up-to-date data, the following were used:

	pthread_mutex_t
and
	pthread_cond_t,

1) pthread_mutex_t
Used to protect data via `pthread_mutex_lock` and `pthread_mutex_unlock`.

Any sensitive data—meaning data that could be accessed by multiple threads simultaneously—was protected by mutexes in this project to prevent race conditions.

`pthread_mutex_lock` was called whenever the data was accessed, ensuring safe usage.

`pthread_mutex_lock` -> "If the data isn't currently in use by someone else, I will use it. I am starting to use it."

`pthread_mutex_unlock` -> "From this point on, I no longer need the data. If another thread wants to use it, they can. I am done with it."

For Log serialization too

Every message printed by the program is protected by mutex_printf. This guarantees that only one thread writes to stdout at a time, preventing interleaved or corrupted out

Example:

	To check if a thread has "burned out" (terminated), I created a variable called `is_dead` that is accessible to all threads.

	This variable is associated with a mutex (`mutex_is_dead`).

	Every time `is_dead` is accessed, I lock the mutex, use the variable, and finally unlock it.

2) pthread_cond_t

Used to synchronize the dongles.

Each dongle has its own `pthread_cond_t`.

`pthread_cond_t` allows a thread to sleep while waiting for a specific event to occur.

It is commonly used with: `pthread_cond_wait`

And `pthread_cond_broadcast`.

`pthread_cond_wait`

	-> Waits until a signal is received on the condition variable; the program sleeps.

`pthread_cond_broadcast`

	-> Signals the condition variable.

When `cond_wait` receives the signal, the thread wakes up and program execution resumes.

I used this mechanism for my dongles. As long as it is not the encoder's turn to take the dongle, it waits for the dongle to be released.

As soon as the dongle is released, the process releasing it broadcasts a signal. All encoders waiting for the resource wake up and check if it is their turn.

If so, they take it.

If not, they go back to waiting.

Example:
------------------
Wait

while (!my_turn())

pthread_cond_wait(&cond, &mutex)

----------------
Release

take(dongle)

compile()

pthread_cond_broadcast(&cond)

## Resume
So each shared ressources (dongles, logging, monitor state) are protected by a mutex.

And synchronization is managed by pthread_cond_t, pthread_cond_broadcast and pthread_cond_wait or pthread_cond_timedwait