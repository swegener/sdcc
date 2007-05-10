# gen_known_bugs.pl - generate knownbugs.html
#
# Copyright (c) 2007 Borut Razem
#
# This file is part of sdcc.
#
#  This software is provided 'as-is', without any express or implied
#  warranty.  In no event will the authors be held liable for any damages
#  arising from the use of this software.
#
#  Permission is granted to anyone to use this software for any purpose,
#  including commercial applications, and to alter it and redistribute it
#  freely, subject to the following restrictions:
#
#  1. The origin of this software must not be misrepresented; you must not
#     claim that you wrote the original software. If you use this software
#     in a product, an acknowledgment in the product documentation would be
#     appreciated but is not required.
#  2. Altered source versions must be plainly marked as such, and must not be
#     misrepresented as being the original software.
#  3. This notice may not be removed or altered from any source distribution.
#
#  Borut Razem
#  borut.razem@siol.net

use strict;
use warnings;

use LWP::Simple;
use HTML::TreeBuilder;


my @headerList = ('Request ID', 'Summary', 'Open Date', 'Priority', 'Status', 'Assigned To', 'Submitted By');
my $version;


# check if the line is a correct header
sub is_header($)
{
  my ($line) = @_;
  
  if (ref($line)) {
    my $i = 0;
    foreach ($line->look_down('_tag', 'td')) {
      if ($_->as_text() ne $headerList[$i++]) {
        return 0;
      }
    }
    return 1;
  }
  else {
    return 0;
  }
}


# check if the line has correct number of fields
sub has_all_fields($)
{
  my ($line) = @_;
 
  my @len = $line->look_down('_tag', 'td');
  return $#len == $#headerList;
}


# process a line
sub process_line($)
{
  my ($line) = @_;

  my $i = 0;
  foreach ($line->look_down('_tag', 'td')) {
    if ($i == 0) {
      # remove nowrap attribute from 'Request ID' field
      $_->attr('nowrap', undef);
    }
    elsif ($i == 1) {
      # convert relative to absolute href in the 'Summary' field
      foreach ($_->look_down('_tag', 'a')) {
        my $attr = $_->attr('href');
        if (defined($attr) && $attr =~ m!^/tracker/index.php?!) {
          $_->attr('href', 'http://sourceforge.net' . $attr);
        }
      }
    }
    elsif ($i == 2) {
      # remove text formatting from 'Open Date' field
      my $text = $_->as_text();
      $text =~ s/^\W*\**\W//;
      $_->delete_content();
      $_->push_content($text);
    }
    elsif ($i == 4) {
      # remove the 'Status' field
      $_->delete();
    }
    elsif ($i == 5 || $i == 6) {
      # remove hrefs in 'Assigned To' and 'Submitted By' fields
      foreach ($_->look_down('_tag', 'a')) {
       $_->replace_with($_->as_text());
      }
    }
    ++$i;
  }
  $line->delete_ignorable_whitespace();
}


# process the HTML page
sub process_page($)
{
  my ($html) = @_;

  # create HTML tree from the page
  my $tree = HTML::TreeBuilder->new();
  $tree->parse($html);

  # find table with the required header
  my $hasTable = 0;
  foreach my $table ($tree->look_down('_tag', 'table')) {
    my @lines = $table->content_list();
    if (is_header($lines[0])) {
      shift(@lines);  #remove the header

      # process the following lines in table
      # if they have required number of fields
      foreach my $line (@lines) {
        if (ref($line) && has_all_fields($line)) {
          # process a line
          process_line($line);
          # and print it
          print($line->as_HTML(undef, '  '));
          $hasTable = 1;
        }
      }
    }
  }

  $tree->delete;
  
  return $hasTable;
}


# print HTML header
sub print_header()
{
  print <<EOF;
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="en">
<!--
This file is generated automagicaly by gen_known_bugs.pl script.
-->
  <head>
    <meta http-equiv="content-type" content="text/html; charset=utf-8" />
    <title>SourceForge.net: Known Bugs</title>
  </head>
  <body>
    <h2>Small Device C Compiler - Release $version Known Bug List</h2>
    <ul>
      <li><a href="http://sdcc.sourceforge.net" >Home&nbsp;Page</a></li>
      <li class="selected"><a href="http://sourceforge.net/tracker/?group_id=599&amp;atid=100599" >Current Bugs</a></li>
    </ul>
    <table width="100%" border="0" cellspacing="2" cellpadding="3">
      <tr bgcolor="#ffffff">
        <td align="center"><font color="#000000"><b>Request ID</b></font></td>
        <td align="center"><font color="#000000"><b>Summary</b></font></td>
        <td align="center"><font color="#000000"><b>Open Date</b></font></td>
        <td align="center"><font color="#000000"><b>Priority</b></font></td>
        <td align="center"><font color="#000000"><b>Assigned To</b></font></td>
        <td align="center"><font color="#000000"><b>Submitted By</b></font></td>
      </tr>
EOF
}


# print HTML footer
sub print_footer()
{
  print <<EOF;
    </table>
    <p><b>Priority Colors:</b></p>
    <br />
    <table border="0">
      <tr>
        <td bgcolor="#dadada">1</td>
        <td bgcolor="#dad0d0">2</td>
        <td bgcolor="#dacaca">3</td>
        <td bgcolor="#dac0c0">4</td>
        <td bgcolor="#dababa">5</td>
        <td bgcolor="#dab0b0">6</td>
        <td bgcolor="#daaaaa">7</td>
        <td bgcolor="#da9090">8</td>
        <td bgcolor="#da8a8a">9</td>
      </tr>
    </table>
  </body>
</html>
EOF
}


# main procedure
{
  my $url = "http://sourceforge.net/tracker/index.php?func=browse&group_id=599&atid=100599&set=custom&_assigned_to=0&_status=1&_category=100&_group=100&order=artifact_id&sort=DESC&offset=";

  if ($#ARGV != 0) {
    printf("Usage: gen_known_bugs.pl <version>\n");
    exit(1);
  }

  # get the SDCC version number from command line
  $version = $ARGV[0];

  # print HTML header
  print_header();

  # get pages from SF bug tracker
  for (my $i = 0; my $html = get($url . $i); $i += 50) {
    # and process them
    if (!process_page($html)) {
      last;
    }
  }

  # print HTML footer
  print_footer();

  exit(0);
}
