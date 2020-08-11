# RxDx DROID
3D printed R2D2 Clone Project

Using a Hitex Infineon Shield Buddy TC275, the multicore processor has three cores which will indepedntely control tasks. Using the three cores to do tasks instead of using them as threads, a RTOS is used to handle mutitasking. Where the tasks are simple, millis() is used for timing to act as a scheduler and breake acting as an interupt. 

