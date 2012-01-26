#!/usr/bin/perl
#
# make_dist.pl
#
# SYNOPSIS:
#   make_dist.pl [OPTION] --version VERSION
#
# DESCRIPTION:
#   create a BioNetGen distribution
#
# OPTIONS:
#   --bngpath PATH  : path to BioNetGen repository
#   --outdir  PATH  : output path
#   --archive       : create distribution archive file
#   --help          : display help

use strict;
use warnings;

# Perl Core Modules
use FindBin;
use File::Spec;
# this module isn't copying the executable permission (as advertised)
use File::Copy "cp";

### PARAMETERS ###
# BNG root directory
my $bngpath = $FindBin::RealBin;
# output directory
my $outdir = File::Spec->curdir();
 # if true, delete output files after validation
my $archive = 1;
# size of indent to STDOUT       
my $version = '';
# regex for excluding files (exclude make_dist.pl itself and all files beginning with "." or "_" or ending in "~")
my $exclude_files = '(^\.|^_|~$|^make_dist\.pl$|^Installation_Guide\.html$|^WikiWelcome\.rtf$)';
# subdirectories to include in distribution
my @include_subdirectories = qw/ bin Perl2 Models2 Network2 Network3 PhiBPlot Validate libsource /;
# system copy command
my $syscopy = "cp";


###                                                          ###
###  BEGIN MAIN SCRIPT, no user options beyond this point!!  ###
###                                                          ###

# Greet the User
print "\n>>> BioNetGen Distribution Tool\n";

# Process command line arguments
while ( @ARGV and $ARGV[0] =~ /^--/ )
{
    my $arg = shift @ARGV;
    if ( $arg eq '--bngpath' )
    {   $bngpath = shift @ARGV;   }
    elsif ( $arg eq '--outdir' )
    {   $outdir  = shift @ARGV;    }
    elsif ( $arg eq '--version' )
    {   $version = shift @ARGV;    }
    elsif ( $arg eq '--archive' )
    {   $archive = 1;    }
    elsif ( $arg eq '--help' )
    {
        display_help();
        exit 0;
    }
    else
    {   # unrecognized option!
        print "make_dist.pl syntax error:\nunrecognized command line option '$arg'.\n";
        exit -1;
    }
}


# check for valid version
unless ( $version =~ /^\d+\.\d+\.\d+$/)
{
    print "make_dist.pl error:\ninvalid version '$version'.\n";
    exit -1;
}

my $dist_name = "BioNetGen_${version}";
my $dist_dir     = File::Spec->catdir( ${outdir}, ${dist_name} );
my $archive_file = File::Spec->catfile( ${outdir}, "${dist_name}.tar.gz" );



# begin creating distribution
print  "Creating distribution version $version\n";
printf "  bngpath: %s\n", $bngpath;
printf "  distdir: %s\n", $dist_dir;
if ($archive)
{
    printf "  archive: %s\n", $archive_file;
}
print "\n";




# check if output directory exists..
unless (-d $outdir)
{
    # try to make output directory
    unless ( mkdir $outdir )
    {
        print "make_dist.pl error:\ncannot make output directory ($!).\n";
        exit -1;
    }
}


if ($archive)
{   # check if archive file already exists..
    if (-e $archive_file)
    {
        print "make_dist.pl error:\narchive '$archive_file' already exists.\n";
        exit -1;
    }
}


# check if distribution directory already exists..
if (-d $dist_dir)
{   # don't write over any exist files.. ABORT
    print "make_dist.pl error:\ntarget distribution directory '$dist_dir' already exists.\n"; 
    exit -1;
}



# Include root..
my $recursive = 0;
my $err = copy_dir( $bngpath, $dist_dir, $recursive, $exclude_files);
if ($err)
{
    print "make_dist.pl error:\n$err\n";
    exit -1;
}

# Include subdirectories..
foreach my $dir ( @include_subdirectories )
{   
    my $source_dir = File::Spec->catdir( $bngpath,  $dir );
    my $dest_dir   = File::Spec->catdir( $dist_dir, $dir );

    my $recursive = 1;    
    my $err = copy_dir( $source_dir, $dest_dir, $recursive, $exclude_files );
    if ($err)
    {
        print "make_dist.pl error:\n$err\n";
        exit -1;
    }
}


# Create VERSION file for the distribution
my $vh;
unless( open($vh, ">", File::Spec->catfile($dist_dir, "VERSION")) )
{
    print "make_dist.pl error:\ncould not create VERSION file ($!).\n"; 
    exit -1;
}
print $vh "$version";
close $vh;


if ($archive)
{
    # create tar-archive
    print "\nCreating distribution archive:\n";
    print "tar cvzf ${archive_file} ${dist_dir}\n";
    `tar cvzf ${archive_file} ${dist_dir}`;
}


# all done
print "\nFinished creating distribution.\n";
exit 0;





##-------------##
## Subroutines ##
##-------------##


# copy directories (optionally recursive) with exclude filter
sub copy_dir
{
    my $source_dir = shift @_;
    my $dest_dir   = shift @_;
    my $recursive  = @_ ? shift @_ : 1;
    my $exclude_files = @_ ? shift @_ : '';

    my @files;

    if ( -d $source_dir )
    {   # read files in source_dir
        my $dh;
        unless( opendir $dh, $source_dir )
        {
            return "copy_dir: cannot read directory '$source_dir'.";
        }
        @files = grep {$_ !~ m/$exclude_files/} readdir($dh);
        closedir $dh;
    }
    else
    {   # return error
        return "copy_dir: source '$source_dir' is not a directory.";
    }

    unless ( mkdir $dest_dir )
    {   # return error
        return "copy_dir: cannot create directory ($!)";
    }

    print "including $source_dir . . .\n";
    foreach my $file (@files)
    {
        # TODO: using catfile is questionable, since file may be a subdirectory
        my $source_file = File::Spec->catfile( $source_dir, $file );
        my $dest_file   = File::Spec->catfile( $dest_dir,   $file );

        if ( -d $source_file )
        {   
            if ($recursive)
            {   # copy subdirectory
                my $err = copy_dir( $source_file, $dest_file, $recursive, $exclude_files );
                if (defined $err)
                {
                    return $err;
                }
            }
        }   
        else
        {   # copy file
            print "  $file\n";
            my @args = ( "cp", $source_file, $dest_file );
            unless( system(@args)==0 )
            {
                return "copy_dir: cannot copy file ($?)";
            }
        }
    }

    return undef;
}



# display help menu
sub display_help
{
print <<END_HELP

make_dist.pl

SYNOPSIS:
   make_dist.pl [OPTS] --version VERSION

 DESCRIPTION:
   create a BioNetGen distribution

 OPTIONS:
   --bngpath PATH  : path to BioNetGen repository
   --outdir  PATH  : output path
   --archive       : create distribution archive file
   --help          : display help

END_HELP

}
