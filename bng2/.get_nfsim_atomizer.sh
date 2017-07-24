#!/bin/bash

# First get NFsim

  if [ "${TRAVIS_OS_NAME}" = "linux" ]; then
    # curl -O  http://www.midcapsignals.com/midcap/junk/NFsim-$platform_travis
    curl -O  -v -u $FTP_USER_0:$FTP_PASSWORD  ftp://www.drivehq.com/d_data/d_travis/NFsim-${TRAVIS_OS_NAME}
  else
    curl -O  -v -u $FTP_USER_1:$FTP_PASSWORD  ftp://www.drivehq.com/d_data/d_travis/NFsim-${TRAVIS_OS_NAME}
  fi
  mv       NFsim-$platform_travis  $1/NFsim
  chmod +x $1/NFsim


# Next get Atomizer
