#!/bin/bash
#
#  Read the VERSION file and construct the name of the installation package.
uname -a
echo " Windows Environment = " $1

echo '  '
echo '  '

# Presumably there is only one line in the file:  VERSION
input="./VERSION"
while read -r var
do
  vbase="BioNetGen-$var"
  vname=$var
done < "$input"

if [ "$1" = "Win32" ]; then
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


#  Get the NFsim and Atomizer files that are needed
cd  $vbase
curl -O  http://www.midcapsignals.com/midcap/d_appveyor/build/NFsim.$platform_travis.exe
mv       NFsim.$platform_travis.exe  ./bin/NFsim.exe
cd ..



#  Get the NFsim and Atomizer files that are needed
cd  $vbase
curl -O  http://www.midcapsignals.com/midcap/dist/sbmlTranslator.exe
mv       sbmlTranslator.exe                 ./bin/sbmlTranslator.exe
cd ..




zip -r -q  $rall $vbase

echo " Version base name is      " $vbase
echo " Remote name of package is " $rall

ls -l 
#curl -T $rall  -u roberthclark:P1ttsburgh ftp://ftp.midcapsignals.com/midcap/junk/$rall

# Move a simple HTML page over to the server, to provide a pointer to the distribution package
perl .make_html.pl  --version $vname  --platform $platform
html_name="BioNetGen-"$platform".html"
#curl -T $html_name -u roberthclark:P1ttsburgh ftp://ftp.midcapsignals.com/midcap/junk/
