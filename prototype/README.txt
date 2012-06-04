/* Urban Transit Routing Problem.
 * Advanced Artificial Inteligence
 *
 * Name: Nicolás Estrada Irribarra
 * UTFSM Rol: 2773505 - 3
 * mail: nicoestrada.i@gmail.com
 *
 **/

Valgrind checked with following parameters:
    
    -v --tool=memcheck --leak-check=full

Without memory leaks and errors:

| #################################################################### |
==9628== HEAP SUMMARY:
==9628==     in use at exit: 0 bytes in 0 blocks
==9628==   total heap usage: 15 allocs, 15 frees, 30,280 bytes allocated
==9628== 
==9628== All heap blocks were freed -- no leaks are possible
==9628== 
==9628== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 17 from 6)
--9628-- 
--9628-- used_suppression:     17 U1004-ARM-_dl_relocate_object
==9628== 
==9628== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 17 from 6)

| #################################################################### |


COMPILATION:
    make folders, to create the required project folders (only if doesn't exist). 
    make all, compile project.

********************************************************************************
main.cpp:

USE: ./utrp parameter1 parameter2

DESCRIPTION:

	- DESC...
    
OUTPUT:

    The output it's composed by X files:
        -
        -

********************************************************************************
data.hpp:

USE: Included on main.cpp

DESCRIPTION:
	
	- DESC ...


*********************************************************************************
functions.hpp:

USE: Included on data.hpp
DESCRIPTION:
	-DESC ...

*********************************************************************************
iterator.cpp:

USE: ./iterator parameter1 parameter2

DESCRIPTION:

	- DESC ...

*********************************************************************************
