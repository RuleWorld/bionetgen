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


#  Get the NFsim and Atomizer files that are needed
cd  $vbase
curl -O  http://www.midcapsignals.com/midcap/d_appveyor/build/NFsim-$platform.exe
dir      NFsim-$platform.exe
mv       NFsim-$platform.exe  ./bin/NFsim.exe
curl -O  http://www.midcapsignals.com/midcap/d_appveyor/build/$platform-libgcc_s_dw2-1.dll
mv       $platform-libgcc_s_dw2-1.dll ./bin/libgcc_s_dw2-1.dll
cd ..



#  Get the NFsim and Atomizer files that are needed
cd  $vbase
curl -O  http://www.midcapsignals.com/midcap/dist/sbmlTranslator-$platform.exe
mv       sbmlTranslator-$platform.exe                 ./bin/sbmlTranslator.exe
cd ..




zip -r -q  $rall $vbase

echo " Version base name is      " $vbase
echo " Remote name of package is " $rall

ls -l 
# I believe that curl can be installed on Appveyor, but it's more natural to use the
# native Appveyor deployment system, which permits you to use ftp.
#curl -T $rall  -u roberthclark:xxxxxxxxxx ftp://ftp.midcapsignals.com/midcap/junk/$rall

# Move a simple HTML page over to the server, to provide a pointer to the distribution package
perl .make_html.pl  --version $vname  --platform $platform
html_name="BioNetGen-"$platform".html"
#curl -T $html_name -u roberthclark:xxxxxxxxxx ftp://ftp.midcapsignals.com/midcap/junk/
