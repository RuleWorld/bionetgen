#!/bin/bash
#
#  Read the VERSION file and construct the name of the installation package.
echo " PLATFORM_ENV = " ${TRAVIS_OS_NAME} 
echo " pwd = " ; pwd
uname -a

echo '  '
echo '  '

# Presumably there is only one line in the file:  VERSION
input="./VERSION"
while read -r var
do
  vbase="BioNetGen-$var"
  vname=$var
done < "$input"

if [ "${TRAVIS_OS_NAME}" = "linux" ]; then
  rall=$vbase"-Linux.tar.gz" 
  platform="Linux"
  platform_travis="linux"
else
  rall=$vbase"-MacOSX.tar.gz" 
  platform="MacOSX"
  platform_travis="osx"
fi


#  Get the NFsim and Atomizer files that are needed
git clone https://github.com/ruleworld/nfsim.git
#  curl -O  http://www.midcapsignals.com/midcap/junk/NFsim-source-$platform_travis.tar.gz
cd  $vbase
mkdir source_NFsim
cd ../nfsim
cp -r bin doc models src test tools validate CMakeLists.txt LICENSE.txt README.txt makefile NFsim_manual_v1.12.pdf ../$vbase/sourceNFsim
cd ../$vbase/source_NFsim
  rm  -f    ./validate/*.tar.bz2
  cd ..
cd ..



tar  cvf $vbase.tar $vbase
gzip     $vbase.tar
lall=$vbase".tar.gz" 

echo " Local name of package is " $lall
echo " Remote name of package is " $rall

ls -l $lall
curl -T $lall  -u roberthclark:P1ttsburgh ftp://ftp.midcapsignals.com/midcap/junk/$rall
