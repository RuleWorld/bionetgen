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
else
  rall=$vbase"-MacOSX.tar.gz" 
  platform="MacOSX"
fi

#  Get the NFsim and Atomizer files that are needed
cd  $vbase
curl -o  http://www.midcapsignals.com/midcap/junk/NFsim.$platform
curl -o  http://www.midcapsignals.com/midcap/junk/NFsim-source-$platform.tar.gz
tar -xvf NFsim-source-$platform.tar.gz
rm  -f   NFsim-source-$platform.tar.gz
cd ..


tar -cvf $vbase.tar $vbase
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
