#!/usr/bin/perl
#
# SYNOPSIS:
#   make_dist.pl [OPTS] 
#
# DESCRIPTION:
#   Create a BioNetGen distribution from the reposity. By default, attempts
#   to extract version number and codename from VERSION file in BNG root.
#
# OPTIONS:
#   --bngpath PATH  : path to BioNetGen repository (default: script directory)
#   --outdir  PATH  : output path (default: current directory)
#    --version VERS : version number (overrides VERSION file)
#   --codename NAME : distribution codename (overrides VERSION file)
#   --archive       : create distribution archive file
#   --overwrite     : overwrite any existing distribution
#   --help          : display help

use strict;
use warnings;

# Perl Core Modules
use FindBin;
use File::Spec;

### PARAMETERS ###
# BNG root directory
my $bngpath = $FindBin::RealBin;
# output directory
my $outdir = File::Spec->curdir();
 # if true, creates a tar.gz compressed archive
my $archive = 0;
# if true, overwrites existing distribution
my $overwrite = 0;
# distribution version (default undefined)
my $version;
# distribution codename (default)
my $codename = 'stable';
# regex for excluding files (exclude make_dist.pl itself and all files beginning with "." or "_" or ending in "~")
my $exclude_files = '(^\.|^_|~$|\.old$|^make_dist\.pl$)';
# subdirectories to include in distribution
my @include_subdirectories = qw/ Perl2 Models2 Network3 PhiBPlot Validate /;
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
    elsif ( $arg eq '--codename' )
    {   $codename = shift @ARGV;   }
    elsif ( $arg eq '--archive' )
    {   $archive = 1;   }
    elsif ( $arg eq '--overwrite' )
    {   $overwrite = 1;   }
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



if (defined $version )
{   # check for proper version format
    unless ($version =~ /^\d+\.\d+\.\d+$/)
    {   # invalid format
        print "make_dist.pl error:\ninvalid version '$version'.\n";
        exit -1;
    }
}
else
{   # try to read version file
    print "..attempting to extract version info from VERSION file.\n";
    my $version_file = File::Spec->catfile( $bngpath, "VERSION" );
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

    # extract codename, if any
    if ( $vline =~ s/^(\w+)// )
    {   $codename = $1;   }

    print "version: $version  codename: $codename\n";
}


# define distribution name, directory and archive file
my $dist_name    = "BioNetGen-${version}" . (($codename eq '') ? '' : "-${codename}");
my $dist_dir     = File::Spec->catdir( ${outdir}, ${dist_name} );
my $archive_file = File::Spec->catfile( ${outdir}, "${dist_name}.tar.gz" );



# begin creating distribution
print  "Creating distribution version ${version} codename ${codename}\n";
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
        if ($overwrite)
        {   # warn using about overwrite
            print "make_dist.pl warning:\noverwriting archive file '$archive_file'\n";
        }
        else
        {   # overwrite not allowed! exit with error.
            print "make_dist.pl error:\narchive '$archive_file' already exists.\n";
            exit -1;
        }
    }
}


# check if distribution directory already exists..
if (-d $dist_dir)
{
    if (-e $archive_file)
    {
        if ($overwrite)
        {   # warn using about overwrite
            print "make_dist.pl warning:\noverwriting distribution directory '$dist_dir'\n";
        }
        else
        {   # overwrite not allowed! exit with error.
            print "make_dist.pl error:\ntarget distribution directory '$dist_dir' already exists.\n"; 
            exit -1;
        }
    }
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
my $vstring = "$version" . (($codename) eq "" ? "" : " ${codename}" );
unless( open($vh, ">", File::Spec->catfile($dist_dir, "VERSION")) )
{
    print "make_dist.pl error:\ncould not create VERSION file ($!).\n"; 
    exit -1;
}
print $vh $vstring;
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

    # create directory, unless it exists
    unless ( -d $dest_dir )
    {   
        unless ( mkdir $dest_dir )
        {   # return error
            return "copy_dir: cannot create directory '$dest_dir' ($!)";
        }
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
   make_dist.pl [OPTS] 

DESCRIPTION:
   Create a BioNetGen distribution from the reposity. By default, attempts
   to extract version number and codename from VERSION file in BNG root.

OPTIONS:
   --bngpath PATH  : path to BioNetGen repository (default: script directory)
   --outdir  PATH  : output path (default: current directory)
   --version VERS  : version number (overrides VERSION file)
   --codename NAME : distribution codename (overrides VERSION file)
   --archive       : create distribution archive file
   --overwrite     : overwrite any existing distribution
   --help          : display help

END_HELP

}
