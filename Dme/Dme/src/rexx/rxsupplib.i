* === rxsupplib.i ======================================================
*
* Copyright (c) 1986, 1987 by William S. Hawes (All Rights Reserved)
*
* ======================================================================
* 
* Include file for Rexx Support Library

         IFND     REXX_RXSUPPLIB_I
REXX_RXSUPPLIB_I  SET   1

         IFND     REXX_STORAGE_I
         INCLUDE  "rexx/storage.i"
         ENDC

         IFND     REXX_RXSLIB_I
         INCLUDE  "rexx/rxslib.i"
         ENDC

* Structure definition for the Rexx Support Library
         STRUCTURE RexxSupport,LIB_SIZE
         APTR     rs_SysBase           ; EXEC base
         APTR     rs_REXXBase          ; Rexx System Library base
         LONG     rs_SegList           ; library seglist
         LABEL    rs_SIZEOF

RSNAME   MACRO
         dc.b     'rexxsupport.library',0
         ENDM

RSID     MACRO
         dc.b     'rexxsupport 1.0 (22 AUG 87)',13,10,0
         ENDM

RSVERS   EQU   2
RSREV    EQU   1

* Various system constants
MemList     EQU   $142                 ; Memory list
DeviceList  EQU   $15E                 ; Devices list
LibList     EQU   $17A                 ; Libraries list
PortList    EQU   $188                 ; Ports list
ReadyList   EQU   $196                 ; Ready Tasks
WaitList    EQU   $1A4                 ; Waiting Tasks

         ENDC
