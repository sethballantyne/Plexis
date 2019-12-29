#quick n dirty script to archive the game

use strict;
use warnings;
use Archive::Zip;

my $zip = Archive::Zip->new();

$zip->addTree('data/', 'data/');
$zip->addFile('game_debug.exe');
$zip->addFile('game_release.exe');
$zip->addFile('PLeD.exe');
$zip->addFile('options.xml');
$zip->addFile('PLeD_options.xml');
$zip->addFile('paths.xml');

$zip->writeToFileNamed("plexis.0.8.zip");
