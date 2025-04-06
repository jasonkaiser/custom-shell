# custom-shell

**Operating Systems – Shell Project**

**Project members:** Alexander Jaison Kaiser, Milan Milunović

**Listing files:** out.txt, test.txt, shell.c, shell.h, README.md, shell






**Question 1.5.1: What does the mmap() function do?** 

\-  The mmap() is a system call function that allows mapping files or devices into memory. This enables users to access the contents of a file as if it were part of the program's memory space, facilitating more efficient input/output operations.




**Question 1.5.2: What happens during a context switch? Do we want the OS to perform many or few context switches? Explain**

\-  A context switch is the process by which an OS saves the state of a currently running process or thread so that it can restore that state later when the process or thread is scheduled to run again. It's like a brief pause in a conversation when one person stops talking and another takes their place.

The processes during context switch:

Saving the Current State: When the OS decides to switch from one task (or process) to another, it first needs to take a "mental snapshot" of the current task – this includes everything it was working on, like where it was in a program and any important information it was holding(CPU registers, program counter, stack pointer, memory management information...)

Updating Records: This snapshot is stored in a special record called a process control block (PCB), which the OS uses to keep track of all tasks. It marks the current task as no longer active.

Choosing the Next Task: The OS then decides which task should run next based on scheduling algorithms. This is like deciding which person in a group should get to speak next, often based on who has been waiting the longest or who has something important to say, let's say like that.

Restoring the Next Task's State: Once the next task is chosen, the OS brings up its saved snapshot, so it can pick up right where it left off.

Execution: Finally, the OS allows the new task to start running on the CPU.



Based on frequency of context switches, it depends on the system's workload and goals what should we perform. In many cases fewer context switches are preferred. We want to avoid too many context swithces because switching takes time and resources. If OS is busy (switching betweeen tasks), it can slow everything. So basically few switches mean the computer can focus on one goal and work efficiently. In situations where systems requiring fair resource distribution, more context switches is desirable. This allows multiple tasks to have their turn and ensures everyone gets a chance to do their work.




**Question 1.5.3: What is the output of the following code snippet? You are given that the exec system call in the child does not succeed.**

Since exec() fails here "exec(some\_binary\_that\_does\_not\_exec);"  the "printf("child\n");" line does execute.

The parent waits for the child to terminate using wait(), then prints "parent\n".

The order is the child prints "child" first then the parent after wait(), prints "parent", so the output will be:

child
parent


**Outline what we did in assingment:**


**Task 1.1 – Shell Interface**
We implemented a basic shell that continuously prompts the user for input and executes commands. It was designed to be stable and handle invalid input gracefully. We also added an advanced prompt format that displays the machine name and username dynamically, replicating the look and feel of a real Linux terminal.



**Task 1.2 – Shell Commands**
We created basic versions of common commands such as rm, cat, clear, and cowsay. At least two of these support command-line arguments or options. Additionally, we added support for output redirection to a text file and introduced a few creative features to make the behavior slightly different from the standard bash implementation.


**Task 1.3 – System Calls**
In C, we demonstrated how to use key system calls including fork(), wait(), and exec() for process creation and management. We also implemented kill() and execv() in separate examples. For the advanced part, we explored and carefully implemented a forkbomb to understand how the system handles extreme process creation.


**Task 1.4 – Shell Styling and Naming**
We styled the shell interface using color to enhance readability and user experience. We also gave our shell a custom name to personalize it and make it stand out.



**Instructions for compiling scripts:** 


This project was developed using the Nano editor and compiled with the GCC compiler on an Ubuntu virtual machine. To get the shell up and running, we simply compiled the C file using GCC, which created an executable version of our shell. After that, we were able to launch it from the terminal. In cases where permission was needed, we made the file executable so it could run smoothly like any other command-line tool.


Sources/tutorials we used to complete task:
We mostly used source from lab work to build the core of the shell, focusing on system calls, shell creation, process management, and topics like limited direct execution, CPU scheduling, and MLFQ. For more advanced aspects of the script, we turned to YouTube tutorials for additional guidance. Tutorials helped us to better understand and implement the more     complex features that went beyond the sources covered in the lectures.   Combining the lab work with yotube tutorials allowed us to create a fully   functional and efficient shell.

**Project Challenges:**

One of the biggest challenges we faced during this project was getting used to how different this Operating Systems course was compared to the other programming classes we’ve taken. Most of our past experience was with higher-level languages and more abstracted environments, but this course really made us dive into how things work at the system level — like process management, system calls, file descriptors, and forking.

A lot of those topics were completely new to us, so we had to rely heavily on online resources, documentation, and community forums just to understand what was going on. It took a lot of time and effort to figure out how things like dup2, execvp, and output redirection actually worked and how to use them properly in our code.

What really helped us overcome these challenges were the labs. They gave us hands-on experience and a chance to apply what we were learning in a more practical way. Even when we made mistakes, we learned from them, and that made it easier to understand the more complex parts of the project.

Overall, while the learning curve was steep, it felt really rewarding to get everything working in the end. We walked away with a much better understanding of how operating systems work under the hood, and we’re proud of how far we came during this project.

