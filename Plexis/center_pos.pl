# Perl 5 script for calculating the position of text centered on the screen 
# based on the strings length and the width of the screen resolution.

use warnings;
use strict;
use Getopt::Long;

my $line;
my $gwidth = 0;
my $swidth = 0;

GetOptions("gwidth=i" => \$gwidth, "swidth=i" => \$swidth);

if($gwidth <= 0 || $swidth <= 0) {
    exit(usage());
}

while(1) {
    print 'enter a string (:q to quit):';
    $line = <STDIN>;
    chomp($line);

    if($line eq ":q") {
        exit;
    } 
    else {
        $line = (length($line) * $gwidth) / 2;
        my $pos = ($swidth / 2) - $line;
        print $pos, "\n";
    }
}

sub usage {
print <<USAGE;

Usage: center_pos --gwidth=[character width] --swidth=[screen width]

example: center_pos --gwidth=18 --swidth=1024

USAGE
}