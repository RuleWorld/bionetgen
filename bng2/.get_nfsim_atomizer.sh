#!/bin/bash



#  This script reflects the fact that as of  July 30, 2017, none of the Atomizer executables are working.  So for each
#  platform, the code that captures the latest NFsim executable, is followed by code for capturing the sbmlTranslator
#  that has been commented out.  The comment is then followed by temporary code that captures a version of sbmlTranslator
#  that is knownn to work and that has been squirreled away on the ftp server for the particular platform.


  if [ "${TRAVIS_OS_NAME}" = "linux" ]; then
    # curl -O  http://www.midcapsignals.com/midcap/junk/NFsim-$platform_travis
    curl -v -u $FTP_USER_4:$FTP_PASSWORD  -O ftp://ftp.drivehq.com/d_data/d_travis/NFsim-${TRAVIS_OS_NAME}
    mv       NFsim-${TRAVIS_OS_NAME}  $1/NFsim
    chmod +x $1/NFsim

#    curl -v -u $FTP_USER_4:$FTP_PASSWORD  -O ftp://ftp.drivehq.com/d_data/d_travis/sbmlTranslator-${TRAVIS_OS_NAME}
#    mv       sbmlTranslator-${TRAVIS_OS_NAME}  $1/sbmlTranslator
#    chmod +x $1/sbmlTranslator

    curl -v -u $FTP_USER_4:$FTP_PASSWORD  -O ftp://ftp.drivehq.com/Linux/sbmlTranslator
    mv       sbmlTranslator               $1/sbmlTranslator
    chmod +x $1/sbmlTranslator
  fi
  if [ "${TRAVIS_OS_NAME}" = "osx" ]; then
    curl -v -u $FTP_USER_5:$FTP_PASSWORD  -O ftp://ftp.drivehq.com/d_data/d_travis/NFsim-${TRAVIS_OS_NAME}
    mv       NFsim-${TRAVIS_OS_NAME}  $1/NFsim
    chmod +x $1/NFsim
    
#    curl -v -u $FTP_USER_5:$FTP_PASSWORD  -O ftp://ftp.drivehq.com/d_data/d_travis/sbmlTranslator-${TRAVIS_OS_NAME}
#    mv       sbmlTranslator-${TRAVIS_OS_NAME}  $1/sbmlTranslator
#    chmod +x $1/sbmlTranslator

    curl -v -u $FTP_USER_5:$FTP_PASSWORD  -O ftp://ftp.drivehq.com/MacOSX/sbmlTranslator
    mv       sbmlTranslator               $1/sbmlTranslator
    chmod +x $1/sbmlTranslator
  fi
  
  
  if [ "$2" = "x86" ]; then
    curl -v -u 15262:$3  -O ftp://ftp.drivehq.com/d_data/d_appveyor/build/NFsim-Win32.exe
    mv       NFsim-Win32.exe           $1/NFsim.exe

#    curl -v -u 15262:$3  -O ftp://ftp.drivehq.com/d_data/d_appveyor/dist/sbmlTranslator-Win32.exe
#    mv       sbmlTranslator-Win32.exe  $1/sbmlTranslator.exe

     curl -v -u 15262:$3  -O ftp://ftp.drivehq.com/Win32/sbmlTranslator_64.exe
     mv ./sbmlTranslator_64.exe  $1/sbmlTranslator_64.exe
  fi

  if [ "$2" = "x64" ]; then
    curl -v -u 15263:$3  -O ftp://ftp.drivehq.com/d_data/d_appveyor/build/NFsim-Win64.exe
    mv       NFsim-Win64.exe           $1/NFsim.exe

#    curl -v -u 15263:$3  -O ftp://ftp.drivehq.com/d_data/d_appveyor/dist/sbmlTranslator-Win64.exe
#    mv       sbmlTranslator-Win64.exe  $1/sbmlTranslator.exe

    curl -v -u 15263:$3  -O ftp://ftp.drivehq.com/Win64/sbmlTranslator.exe
    mv ./sbmlTranslator.exe  $1/sbmlTranslator.exe
  fi

