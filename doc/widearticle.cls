% Simple extension of atricle which wides the margins.
% $Id$
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{widearticle}
\LoadClass{article}

\setlength{\oddsidemargin}{0pt}
\setlength{\evensidemargin}{0pt}

\setlength{\textwidth}{\paperwidth}
\addtolength{\textwidth}{-2in}
\setlength{\textheight}{\paperheight}
\addtolength{\textheight}{-2in}

\setlength{\parindent}{0pt}
