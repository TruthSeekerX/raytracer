# raytracer
This is a UAS 2nd Year project. A multithreaded raytracing application. The raytracing algorith was written by VAMK professor Johan Dams. I multithreaded the code, and added semaphore/mutex for a global counter
which tracks the number of sub-pixels rendered. 

Objective:
1. Understand how to create threads
2. Thread safty: Race condition
3. Semaphore/Mutex: Keep data intergrity with semaphore/mutex
4. How to understand other people's code and improve on top of it.

User guide:
Set threads number by giving argument in positive number. 
Example:

./raytracer 10

will run the program in threads.