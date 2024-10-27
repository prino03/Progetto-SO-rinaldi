# Pseudo Malloc 

This is a project done for the Sapienza course "Sistemi Operativi" with Giorgio Grisetti  

The Pseudo Malloc allocates memory in two different ways: <br />
     -if the memory requested is less than the page size we request the memory to a buddy allocator
     which is implemented with a bitmap <br />
    -otherwise we use a mmap to return the requested memory

To test the code you must compile it with the "make" command in the terminal <br />
Once this is done you can execute the program by running "./main" 

