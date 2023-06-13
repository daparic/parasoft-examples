-------- PROJECT GENERATOR --------
PROJECT NAME :	Sensor
PROJECT DIRECTORY :	C:\WorkSpace\Sensor\Sensor
CPU SERIES :	SH-4
CPU TYPE :	SH7760
TOOLCHAIN NAME :	Renesas SuperH RISC engine Standard Toolchain
TOOLCHAIN VERSION :	9.3.0.0
GENERATION FILES :
    C:\WorkSpace\Sensor\Sensor\dbsct.c
        Setting of B,R Section
    C:\WorkSpace\Sensor\Sensor\typedefine.h
        Aliases of Integer Type
    C:\WorkSpace\Sensor\Sensor\lowlvl.src
        Program of Low level
    C:\WorkSpace\Sensor\Sensor\lowsrc.c
        Program of I/O Stream
    C:\WorkSpace\Sensor\Sensor\sbrk.c
        Program of sbrk
    C:\WorkSpace\Sensor\Sensor\iodefine.h
        Definition of I/O Register
    C:\WorkSpace\Sensor\Sensor\intprg.src
        Interrupt Program
    C:\WorkSpace\Sensor\Sensor\vecttbl.src
        Initialize of Vector Table
    C:\WorkSpace\Sensor\Sensor\vect.inc
        Definition of Vector
    C:\WorkSpace\Sensor\Sensor\env.inc
        Define Interruput Event Register
    C:\WorkSpace\Sensor\Sensor\vhandler.src
        Reset/Interrupt Handler
    C:\WorkSpace\Sensor\Sensor\resetprg.c
        Reset Program
    C:\WorkSpace\Sensor\Sensor\Sensor.c
        Main Program
    C:\WorkSpace\Sensor\Sensor\lowsrc.h
        Header file of I/O Stream file
    C:\WorkSpace\Sensor\Sensor\sbrk.h
        Header file of sbrk file
    C:\WorkSpace\Sensor\Sensor\stacksct.h
        Setting of Stack area
START ADDRESS OF SECTION :
 H'000000000	RSTHandler
 H'000000800	INTHandler,VECTTBL,INTTBL,IntPRG
 H'000001000	PResetPRG
 H'000002000	P,C,C$BSEC,C$DSEC,D
 H'070000000	B,R
 H'073FFFBF0	S

* When the user program is executed,
* the interrupt mask has been masked.
* 
* Program start H'2000.
* RAM start H'70000000.

SELECT TARGET :
    SH-4 Simulator
DATE & TIME : 2010-11-09 13:13:41
