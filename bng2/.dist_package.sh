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
cd  $vbase
curl -O  http://www.midcapsignals.com/midcap/junk/NFsim-$platform_travis
mv       NFsim-$platform_travis  ./bin/NFsim
chmod    +x                      ./bin/NFsim
curl -O  http://www.midcapsignals.com/midcap/junk/NFsim-source-$platform_travis.tar.gz
mkdir source_NFsim
cd    source_NFsim
tar  xvf ../NFsim-source-$platform_travis.tar.gz
rm  -f    ./validate/*.tar.gz   
rm  -f    ./validate/*.tar.bz2
rm  -f   ../NFsim-source-$platform_travis.tar.gz
cd ..
cd ..



#  Get the NFsim and Atomizer files that are needed
cd  $vbase
curl -O  http://www.midcapsignals.com/midcap/junk/sbmlTranslator-$platform_travis
mv       sbmlTranslator-$platform_travis  ./bin/sbmlTranslator
chmod    +x                               ./bin/sbmlTranslator
curl -O  http://www.midcapsignals.com/midcap/junk/Atomizer-source-$platform_travis.tar.gz
mkdir source_Atomizer
cd    source_Atomizer
tar  xvf ../Atomizer-source-$platform_travis.tar.gz
rm  -f   ../Atomizer-source-$platform_travis.tar.gz
cd ..
cd ..




tar  cvf $vbase.tar $vbase
gzip     $vbase.tar
lall=$vbase".tar.gz" 

echo " Local name of package is " $lall
echo " Remote name of package is " $rall

ls -l $lall
curl -T $lall  -u roberthclark:P1ttsburgh ftp://ftp.midcapsignals.com/midcap/junk/$rall

# Move a simple HTML page over to the server, to provide a pointer to the distribution package
perl make_html.pl  --version $vname  --platform $platform
html_name="BioNetGen-"$platform".html"
curl -T $html_name -u roberthclark:P1ttsburgh ftp://ftp.midcapsignals.com/midcap/junk/
