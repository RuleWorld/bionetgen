#!/usr/bin/perl
#
# SYNOPSIS:
#   make_html.pl [OPTS] 
#
# DESCRIPTION:
#   Create an html page that provides a link to the latest generated
#   BioNetGen distribution.
#
# OPTIONS:
#   --platform   PLAT  : Choices are Linux, MacOSX, Windows
#   --version    VERS  : Version number (overrides VERSION file)


use strict;
use warnings;

# Perl Core Modules
use FindBin;
use File::Spec;
use Getopt::Long;
use Cwd ("getcwd");
use Config;
use File::Path qw(remove_tree);


# distribution version (default undefined)
my $version = '';
# platform choices are: (MacOSX or Linux or Windows)
my $platform = '';
# path to version file
my $path_to_version_file = '.';


GetOptions( 'help|h'        => sub { display_help(); exit(0); },
            'platform=s'    => \$platform,
            'version=s'     => \$version);


if ($version ne '')
{   # check for proper version format
    unless ($version =~ /^\d+\.\d+\.\d+$/)
    {   # invalid format
        print "make_dist.pl error:\ninvalid version '$version'.\n";
        exit -1;
    }
}
else
{   # try to read version file
    print "Attempting to extract version info from VERSION file.\n";
    my $version_file = File::Spec->catfile( $path_to_version_file, "VERSION" );
    my $vfh;
    unless (open $vfh, '<', $version_file)
    {   # some problem opening version file
        print "make_dist.pl error:\ncannot open VERSION file'.\n";
        exit -1;
    }
    # get version line
    my $vline = <$vfh>;
    close $vfh;
    # strip leading space
    $vline =~ s/^\s+//;

    # extract version
    if ( $vline =~ s/^(\d+\.\d+\.\d+)\s*// )
    {   $version = $1;   }
    else
    {   # some problem reading version information
        print "make_dist.pl error:\nversion information has unrecognized format.\n";
        exit -1;
    }

}

    print "version: $version     platform: $platform\n";
    &write_html($platform,$version);



exit;

# ########################################################################
#      WRITE HTML
# ########################################################################
sub write_html {
  my $platform   = $_[0];
  my $relevel    = $_[1];

  my $ofile_name = "BioNetGen-".$platform.".html";

  my $zip_type  = '';
  my $travis_os = '';
  if ($platform eq "Linux") {
    $zip_type = ".tar.gz";  $travis_os = "Linux";
  } else {
    if ($platform eq "MacOSX") {
    $zip_type = ".tar.gz";  $travis_os = "MacOSX";
    } else {
      if ($platform eq "Windows") {
        $zip_type = ".zip";  $travis_os = "Windows";
      } else {
        print "Invalid platform: ".$platform."\n";
        exit;
      }
    }
  }


my $date_stamp; my $sec; my $min; my $hour; my $mday; my $mon; my $year; my $wday; my $yday; my $isdst;
open(FNEW,">$ofile_name");
  print FNEW "<html>\n";
  print FNEW "<head>\n";
  print FNEW "<title>".$platform." Beta Site for BioNetGen-".$relevel."</title>\n";
  print FNEW "<META NAME=\"ROBOTS\" CONTENT=\"NOINDEX,NOFOLLOW\">\n";


  print FNEW "<style>\n";
  print FNEW "body {\n";
  print FNEW "   margin-top: 100px;\n";
  print FNEW "   margin-bottom: 100px;\n";
  print FNEW "   margin-right: 150px;\n";
  print FNEW "   margin-left: 80px;\n";
  print FNEW "}\n";
  print FNEW "</style>\n";


  print FNEW "</head>\n";
  print FNEW "<body bgcolor=\"BurlyWood\">\n";

  print FNEW "<font size=\"4\">\n"; 
  print FNEW "<center><h1>".$platform." Beta Site for BioNetGen-".$relevel."</h1></center>\n";
  print FNEW "<br>\n";
  print FNEW "<font color=\"red\">WARNING:</font> This is not the download \n";
  print FNEW "site for BioNetGen.  If you wish to download the latest \n";
  print FNEW "version of BioNetGen, please visit: \n";
  print FNEW "<center><h1><a href=\"http://bionetgen.org\">bionetgen.org</a></h1></center>\n";
  print FNEW "<br>\n";
  print FNEW "<br>\n";
  print FNEW "<br>\n";

  ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst)=localtime(time);
  $date_stamp          = sprintf "%4d-%02d-%02d %02d:%02d\n",$year+1900,$mon+1,$mday,$hour,$min;

  print FNEW "If you are a BioNetGen developer, and you wish to get \n";
  print FNEW "access to beta release ".$relevel." for ".$platform.", please click here: <br>\n";
  print FNEW "<center><h1>\n";
  print FNEW "<a href=\"BioNetGen-".$relevel."-".$travis_os.$zip_type."\">\n";
  print FNEW "BioNetGen-".$relevel."-".$travis_os.$zip_type."</a>\n";
  print FNEW "</center>\n";
  print FNEW "<center>\n";
  print FNEW "<a href=\"BioNetGen-".$relevel."-".$travis_os.$zip_type."\">\n";
  print FNEW "Time Stamp: ".$date_stamp."</a>\n";
  print FNEW "</h1></center>\n";
  print FNEW "<br>\n";
  print FNEW "<br>\n";

  print FNEW "</font></body></html>\n";
  
  close(FNEW);

  return;
}


# ########################################################################
#   HELP 
# ########################################################################


# display help menu
sub display_help
{
print <<END_HELP
make_html.pl
SYNOPSIS:
   make_html.pl [OPTS] 
DESCRIPTION:
   Create an html page that provides a link to the latest generated
   BioNetGen distribution.
OPTIONS:
   --platform   PLAT  : Choices are Linux, MacOSX, Windows
   --version    VERS  : Version number (overrides VERSION file)
END_HELP

}
