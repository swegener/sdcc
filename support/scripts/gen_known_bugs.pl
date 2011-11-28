# gen_known_bugs.pl - generate knownbugs.html
#
# Copyright (c) 2007-2011 Borut Razem
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


# trim function to remove whitespace from the start and end of the string
sub trim($)
{
  my $string = shift;
  $string =~ s/^\s+//;
  $string =~ s/\s+$//;
  return $string;
}


my @headerList = ('ID', 'Summary', 'Status', 'Opened', 'Assignee', 'Submitter', 'Resolution', 'Priority');

# check if the line is a correct header
sub is_header($)
{
  my ($line) = @_;
  
  if (ref($line)) {
    my @headers = $line->look_down('_tag', 'th');
    foreach my $header (@headerList) {
      my $found = 0;
      foreach (@headers) {
        my $content = trim($_->as_text());
        if ($content eq $header) {
          $found = 1;
          last;
        }
      }
      if (!$found) {
        return 0;
      }
    }
    return 1;
  }
  return 0;
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
    #if ($headerList[$i] == 'ID') {
    #  # remove nowrap attribute from 'Request ID' field
    #  $_->attr('nowrap', undef);
    #}
    if ($headerList[$i] eq 'Summary') {
      # convert relative to absolute href in the 'Summary' field
      foreach ($_->look_down('_tag', 'a')) {
        my $attr = $_->attr('href');
        if (defined($attr) && $attr =~ m!^/tracker/?!) {
          $_->attr('href', 'http://sourceforge.net' . $attr);
        }
      }
    }
    #elsif ($headerList[$i] == 'Opened') {
    #  # remove text formatting from 'Open Date' field
    #  my $text = $_->as_text();
    #  $text =~ s/^\W*\**\W//;
    #  $_->delete_content();
    #  $_->push_content($text);
    #}
    elsif ($headerList[$i] eq 'Status' || $headerList[$i] eq 'Resolution') {
      # don't print Status and Resolution columns
      $_->delete();
    }
    elsif ($headerList[$i] eq 'Assignee' || $headerList[$i] eq 'Submitter') {
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
  my $lines = 0;
  foreach my $table ($tree->look_down('_tag', 'table')) {
    my $thead = $table->look_down('_tag', 'thead');
    if (is_header($thead)) {
      my $tbody = $table->look_down('_tag', 'tbody');
      my @lines = $tbody->content_list();

      # process the lines in table
      # if they have required number of fields
      foreach my $line (@lines) {
        if (ref($line) && has_all_fields($line)) {
          # process a line
          process_line($line);
          # and print it
          print($line->as_HTML(undef, '  '));
          ++$lines;
        }
      }
    }
  }

  $tree->delete;
  
  return $lines;
}


# print HTML header
sub print_header($)
{
  my ($version) = @_;

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
    <style type="text/css">
      .p1 {background-color: #9ff;}
      .p2 {background-color: #cff;}
      .p3 {background-color: #9fc;}
      .p4 {background-color: #cfc;}
      .p5 {background-color: #cf9;}
      .p6 {background-color: #ffc;}
      .p7 {background-color: #ff9;}
      .p8 {background-color: #fc9;}
      .p9 {background-color: #fcc; color: #300;}
    </style>
  </head>
  <body>
    <h2>Small Device C Compiler - Release $version Known Bug List</h2>
    <ul>
      <li><a href="http://sdcc.sourceforge.net" >Home&nbsp;Page</a></li>
      <li class="selected"><a href="http://sourceforge.net/tracker/?group_id=599&amp;atid=100599" >Current Bugs</a></li>
    </ul>
    <table width="100%" border="0" cellspacing="2" cellpadding="3">
      <tr bgcolor="#ffffff">
EOF

  foreach my $header (@headerList) {
    # don't print Status and Resolution columns
    if ($header ne 'Status' && $header ne 'Resolution') {
      print('        <td align="center"><font color="#000000"><b>' . $header . "</b></font></td>\n");
    }
  }
  print("      </tr>\n");
}


# print HTML footer
sub print_footer($)
{
  my ($lines) = @_;

  print <<EOF;
    </table>
    <p><b>Priority Colors:</b></p>
    <table border="0">
      <tr>
        <td class="p1">1</td>
        <td class="p2">2</td>
        <td class="p3">3</td>
        <td class="p4">4</td>
        <td class="p5">5</td>
        <td class="p6">6</td>
        <td class="p7">7</td>
        <td class="p8">8</td>
        <td class="p9">9</td>
      </tr>
    </table>
  </body>
<p><b>Number of open bugs: $lines</b></p>
</html>
EOF
}


# main procedure
{
  my $firstUrl = "http://sourceforge.net/tracker/?func=&group_id=599&atid=100599&assignee=&status=Open&category=&artgroup=&keyword=&submitter=&artifact_id=&assignee=&status=1&category=&artgroup=&submitter=&keyword=&artifact_id=&submit=Filter&limit=%d";
  my $nextUrl = "http://sourceforge.net/tracker/?words=tracker_browse&group_id=599&atid=100599&assignee=&status=1&category=&artgroup=&keyword=&submitter=&artifact_id=&offset=%d";

  if ($#ARGV != 0) {
    printf("Usage: gen_known_bugs.pl <version>\n");
    exit(1);
  }

  my $limit = 100;

  # get the SDCC version number from command line
  my $version = $ARGV[0];

  # print HTML header
  print_header($version);

  # get pages from SF bug tracker
  # and process them
  my $lines = 0;
  while (my $linesRead = process_page(get(($lines == 0) ? sprintf($firstUrl, $limit) : sprintf($nextUrl, $lines)))) {
    $lines += $linesRead;
  }

  # print HTML footer
  print_footer($lines);

  exit(0);
}
