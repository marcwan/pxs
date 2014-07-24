#!/bin/bash

txtund=$(tput sgr 0 1)          # Underline
txtbld=$(tput bold)             # Bold
bldred=${txtbld}$(tput setaf 1) #  red
bldylw=${txtbld}$(tput setaf 3) #  yellow
bldgrn=${txtbld}$(tput setaf 2) #  green
txtrst=$(tput sgr0)             # Reset
INFO=${bldgrn}INFO:${txtrst}
ERROR=${bldred}ERROR:${txtrst}
WARN=${bldylw}WARNING:${txtrst}



for x in *.pxsa
do
    ../pxs $x 2> /dev/null > __Output__
    diff __Output__ $x.out
    if [ "$?" != 0 ];
    then
        echo "Test $x ${bldred}FAILED${txtrst}"
    else
        echo "Test $x ${bldgrn}PASSED${txtrst}"
    fi
    rm -f __Output__
done

