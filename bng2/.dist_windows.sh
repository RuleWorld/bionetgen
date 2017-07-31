#!/bin/bash
#
#  Read the VERSION file and construct the name of the installation package.
uname -a
echo " Windows Environment = " $1
echo "  pwd = " ; pwd

echo '  '
echo '  '

# Presumably there is only one line in the file:  VERSION
input="./VERSION"
while read -r var
do
  vbase="BioNetGen-$var"
  vname=$var
done < "$input"

if [ "$1" = "x86" ]; then
  rall=$vbase"-Win32.zip" 
  platform="Win32"
  platform_travis="win32"
  
  cp /cygdrive/c/cygwin/bin/cygwin*dll  $vbase/bin
  cp /cygdrive/c/cygwin/bin/cygstdc*dll $vbase/bin
  cp /cygdrive/c/cygwin/bin/cygz*dll    $vbase/bin
  cp /cygdrive/c/cygwin/bin/cyggcc*dll  $vbase/bin
else
  rall=$vbase"-Win64.zip" 
  platform="Win64"
  platform_travis="win64"
  
  cp /cygdrive/c/cygwin64/bin/cygwin*dll  $vbase/bin
  cp /cygdrive/c/cygwin64/bin/cygstdc*dll $vbase/bin
  cp /cygdrive/c/cygwin64/bin/cygz*dll    $vbase/bin
  cp /cygdrive/c/cygwin64/bin/cyggcc*dll  $vbase/bin
fi



#  Get the NFsim files that are needed
#  git clone https://github.com/ruleworld/nfsim.git  This clone command had to be moved to an earlier spot in YAML.
#  curl -O  http://www.midcapsignals.com/midcap/junk/NFsim-source-$platform_travis.tar.gz
cd  $vbase
mkdir source_NFsim
cd ../../nfsim
cp -r bin doc models src test tools validate CMakeLists.txt CMakeLists.x86.txt LICENSE.txt README.txt makefile NFsim_manual_v1.12.pdf ../bng2/$vbase/source_NFsim
cd ../bng2/$vbase/source_NFsim
  rm  -f    ./validate/*.tar.bz2
cd ../..


#  Get the Atomizer files that are needed
# git clone https://github.com/ruleworld/atomizer.git
#  curl -O  http://www.midcapsignals.com/midcap/junk/NFsim-source-$platform_travis.tar.gz
cd  $vbase
mkdir source_Atomizer
cd ../../atomizer
cp -r appveyor  Makefile requirements.txt  twistedServer.py config  SBMLparser  utils ../bng2/$vbase/source_Atomizer
cp -r dist reactionDefinitions  stats  gml2sbgn  README.md   ../bng2/$vbase/source_Atomizer
cd ../bng2


zip -r   $rall $vbase
cp $rall ../.

echo " Version base name is      " $vbase
echo " Remote name of package is " $rall

