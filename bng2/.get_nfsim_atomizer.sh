#!/bin/bash

# First get NFsim

  if [ "${TRAVIS_OS_NAME}" = "linux" ]; then
    # curl -O  http://www.midcapsignals.com/midcap/junk/NFsim-$platform_travis
    curl -O  ftp://15260@www.drivehq.com/d_data/d_travis/NFsim-$platform_travis
  else
    curl -O  ftp://15261@www.drivehq.com/d_data/d_travis/NFsim-$platform_travis
  fi
  mv       NFsim-$platform_travis  $1/NFsim
  chmod +x $1/NFsim


# Next get Atomizer
