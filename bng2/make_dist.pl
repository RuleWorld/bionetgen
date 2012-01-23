#!/usr/bin/perl


if (@ARGV)
{
    $version= shift;
}
else
{
    $version= `cat VERSION`;
    $version=~ s/[+]//;
    @dat= split('\.', $version);
    # Increment last index
    ++$dat[$#dat];
    $version= join('.',@dat);
    #die "Usage: make_dist.pl version_number";
}


print "Creating release $version:\n\n";
@version_dat= split('\.',$version);

$dist_top="Dist";
$dist_name="BioNetGen";
$dist_dir="${dist_top}/${dist_name}";
$dist_file="${dist_name}_${version}.tgz";


-e $dist_file && die "$dist_file exists: change version number.";
if (-d $dist_dir){
    die "Directory $dist_dir exists.  Remove or rename before running.";
}
mkdir $dist_dir;


# Create VERSION file for the distribution
open(VER,">${dist_dir}/VERSION") || die "Couldn't write to $dist_dir: $!\n";
print VER $version;
close(VER);


# Update development version
open(VER,">VERSION") || die "Couldn't write to $dist_dir: $!\n";
print VER "$version+";
close(VER);


if (defined $ENV{PWD}){
    $top_dir= $ENV{PWD};
} else {
    $top_dir=`pwd`;
}


# update Mathutils archive
#my $mathutils_dir = "Mathutils";
#my $mathutils_archive = "libsource/Mathutils.tar.gz";
#print "updating Mathutils archive:\ntar cvzf ${mathutils_archive} --exclude='.svn' ${mathutils_dir}\n\n";
#`tar cvzf ${mathutils_archive} --exclude='.svn' ${mathutils_dir}`;



# libraries
@library_archives = qw(cvode-2.1.1.tar.gz cvode-2.6.0.tar.gz gsl-1.9.tar.gz muparser_v134.tar.gz Mathutils.tar.gz);
print "Include Network2 source files:\n", join( ', ', @library_archives), "\n\n";    
copy_files("libsource", "$dist_dir", \@library_archives, 1);


# Network2 source code
@network2_source = qw(LICENSE HOW_TO_INSTALL INSTALL Makefile README network.h network.c run_network.c);
print "Include Network2 source files:\n", join( ', ', @network2_source), "\n\n";    
copy_files("Network2", "$dist_dir", \@network2_source, 1);


# Network3 source code
@network3_source = qw(Makefile network.h network.cpp run_network.cpp PLA.cpp PLA.h );
print "Include Network3 source files:\n", join( ', ', @network3_source), "\n\n";    
copy_files("Network3_OLD", "$dist_dir", \@network3_source, 1, "Network3");


# Network executable(s)
#@execs="cygwin1.dll run_network_MSWin32.exe run_network_ppc-darwin run_network_i686-linux";
@execs = qw();
print "Include Executables: ", join( ', ', @execs), "\n\n";    
copy_files("bin", "$dist_dir", \@execs, 1);


# Web site
#@web_source= ("BioNetGen\\ website");
#&copy_files(".", "$dist_dir", \@web_source, 1);


# Files in root directory
@root_source = qw(BNG2.pl bngrc Makefile CREDITS.txt LICENSE.txt README_FIRST.txt VERSION);
print "Include Root files:\n", join( ', ', @root_source), "\n\n";    
copy_files(".", "$dist_dir", \@root_source, 1);



# Version 2 files
if ($version_dat[0]==2)
{
    chdir "Perl2";
    @perl2 = <*.p[lm]>;
    print "Include Perl2 files:\n", join(', ', @perl2), "\n\n";
    chdir "..";
    copy_files("Perl2", "$dist_dir", \@perl2, 1);

    #chdir "GUI";
    #@gui= <*no-source*.jar >;
    #chdir "..";
    #&copy_files("GUI", "$dist_dir", \@gui, 1);


    if (-d "Models2")
    {
        @models2 = qw(run_all.pl blbr.bngl tlbr.bngl fceri_ji.bngl toy-jim.bngl egfr_path.bngl egfr_net.bngl SHP2_base_model.bngl);
        print "Include Model2 files:\n", join( ', ', @models2), "\n\n";    
        copy_files("Models2", "$dist_dir", \@models2, 1);
    }

    #  if (-d "Validate"){
    #      chdir Validate;
    #      @files= <*.pl *.bngl *.net>;
    #      push @files, "DAT_validate";
    #      chdir "..";
    #      copy_files("Validate", "$dist_dir", \@files, 1);
    #  }

    #  if (-d "Tutorial"){
    #      chdir Tutorial;
    #      @files= <tutorial.* example1.*>;
    #      chdir "..";
    #      copy_files("Tutorial", "$dist_dir", \@files, 1);
    #  }

    if (-d "PhiBPlot")
    {
        chdir "PhiBPlot";
        @files= <*>;
        print "Include PhiBPlot files:\n", join(', ', @files), "\n\n";
        chdir "..";
        copy_files("PhiBPlot", "$dist_dir", \@files, 1);
    }

    #  if (-d "RuleBuilder"){
    #      chdir RuleBuilder;
    #      @files= <*>;
    #      chdir "..";
    #      &copy_files("RuleBuilder", "$dist_dir", \@files, 1);
    #  }
}



# create tar-archive
chdir "${dist_top}";
print "creating distribution archive:\ntar cvzf ${dist_file} --exclude='.svn' ${dist_name}\n\n";
`tar cvzf ${dist_file} --exclude='.svn' ${dist_name}`;
`rm -rf ${dist_name}`;


##############
## END HERE ##
##############




# subroutine for copying files
sub copy_files
{
    my $source_dir = shift @_;
    my $dist_dir = shift @_;
    my $file_list = shift @_;
    my $no_split = shift @_;
    my $dest_dir = @_ ? shift @_ : $source_dir;

    my @files;
    if ($no_split){
        @files=@{$file_list};
    } else {
        @files= split(' ',$file_list)
    }
    mkdir "$dist_dir/$dest_dir";
    for my $file (@files){
        if ($file=~/\/[.]svn\//){
            print "Excluding $file\n";
            next;
        }
        my $err=`cp -r $source_dir/$file $dist_dir/$dest_dir`;
    }
}
