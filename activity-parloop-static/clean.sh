#!/bin/sh

# remove job out and error files
if  ls *.sh.* 1> /dev/null 2>&1; 
then
    rm *.sh.*
fi

# remove *.o files
if ls *.o 1> /dev/null 2>&1;
then
    rm *.o
fi

if ls static_sched 1> /dev/null 2>&1;
then
    rm static_sched
fi

if ls dynamic_sched 1> /dev/null 2>&1;
then
    rm dynamic_sched
fi
