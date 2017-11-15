# SENG265_Compressor
Lossless Data compressor created in SENG 265.

Created in both C and Python. Used to demostrate the efficiency of C at runtime and the power of python tools. 
Phase 1 does not do any compression but sorts characters to create redundancy for the transformation that occurs 
in phase 2. Phase 2 uses Move to Front followed by Run Length encoding to compress the file. 

-phase1.c was the first assignment and was not allowed to use dynamic memeory. It takes on a set block size when
creating redundancy where phase2.c can handle any blocksize.
-phase1.py was the second assignment and was identical to phase1.c with exception of block sizes allowed to be 
specified at the command line
-phase2.py was the third assignment and compressed the file as explained above.
-phase2.c used the linked list struct files and functions with dynamic memory to implement compression.Most linked list functions were given and the arg parser in main was also given.

