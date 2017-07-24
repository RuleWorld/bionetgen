#!/bin/bash

# First get NFsim

  if [ "${TRAVIS_OS_NAME}" = "linux" ]; then
    # curl -O  http://www.midcapsignals.com/midcap/junk/NFsim-$platform_travis
    curl -v -u $FTP_USER_0:$FTP_PASSWORD  -O ftp://www.drivehq.com/d_data/d_travis/NFsim-${TRAVIS_OS_NAME}
  else
    curl -v -u $FTP_USER_1:$FTP_PASSWORD  -O ftp://www.drivehq.com/d_data/d_travis/NFsim-${TRAVIS_OS_NAME}
  fi
  mv       NFsim-${TRAVIS_OS_NAME}  $1/NFsim
  chmod +x $1/NFsim


# Next get Atomizer
