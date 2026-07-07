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
    

clean fclean re all

# Resources