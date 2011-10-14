\NeedsTeXFormat{LaTeX2e}[1995/12/01]
\ProvidesClass{eptcs}[2010/11/26 v1.5]

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%                             options                            %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\newif\ifadraft
\newif\ifsubmission
\newif\ifpreliminary
\newif\ifcopyright
\newif\ifpublicdomain
\newif\ifcreativecommons
\newif\ifnoderivs
\newif\ifsharealike
\newif\ifnoncommercial
\adraftfalse
\submissionfalse
\preliminaryfalse
\copyrightfalse
\publicdomainfalse
\creativecommonsfalse
\noderivsfalse
\sharealikefalse
\noncommercialfalse
\DeclareOption{adraft}{\adrafttrue}
\DeclareOption{submission}{\submissiontrue}
\DeclareOption{preliminary}{\preliminarytrue}
\DeclareOption{copyright}{\copyrighttrue}
\DeclareOption{publicdomain}{\publicdomaintrue}
\DeclareOption{creativecommons}{\creativecommonstrue}
\DeclareOption{noderivs}{\noderivstrue}
\DeclareOption{noncommercial}{\noncommercialtrue}
\DeclareOption{sharealike}{\sharealiketrue}
\ProcessOptions\relax

\LoadClass[letterpaper,11pt,twoside]{article}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%   On US letter paper the margins (left-top-right-bottom) are       %%
%%              2.795cm - 1.23cm - 2.795cm - 3.46cm                   %%
%%   Note: When \topmargin would be 0, the real top margin would be   %%
%%   (72-25-12=35pt) + 1pt (unused portion of head) = .5in = 1.27cm.  %%
%%   The bottom margin is   11in - 1in + 0.04cm - 623/72in = 3.46cm.  %%
%%   On the first page the bottom margin contains various footers.    %%
%%   When translating from US letter to A4 paper, without scaling, by %%
%%   leaving the centre of the paper invariant (as is possible when   %%
%%   printing the paper with acroread), the resulting A4 margins are  %%
%%                2.5cm - 2.11cm - 2.5cm - 4.34cm                     %%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

\textwidth              16cm             % A4 width is 21cm            %
\textheight             623.01pt         % 46 lines exactly = 21.98cm  %
\oddsidemargin          -0.04cm          % +1 inch = 2.5cm             %
\evensidemargin         -0.04cm          % +1 inch = 2.5cm             %
\topmargin              -0.04cm          % +1 inch = 2.5cm             %
\advance\topmargin-\headheight           % 12pt                        %
\advance\topmargin-\headsep              % 25pt                        %
\marginparwidth          45pt            % leaves 15pt from A4 edge    %
\advance\evensidemargin  .295cm          % centre w.r.t. letter paper  %
\advance\oddsidemargin   .295cm          % centre w.r.t. letter paper  %

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%                  load eptcsdata when available                 %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\IfFileExists{eptcsdata.tex}{\input{eptcsdata}}{}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%			Pagestyle and titlepage      		    %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\pagestyle{myheadings}
\renewcommand\pagestyle[1]{}             % ignore further \pagestyles

\renewcommand\maketitle{\par
  \begingroup
    \providecommand{\event}{}
    \ifadraft
      \providecommand{\publicationstatus}{\Large DRAFT\quad\today}
    \else\ifsubmission
      \providecommand{\publicationstatus}{Submitted to:\\
      \event}
    \else\ifpreliminary
      \providecommand{\publicationstatus}{Preliminary Report. Final version to appear in:\\
      \event}
    \else
      \providecommand{\publicationstatus}{To appear in EPTCS.}
    \fi\fi\fi
    \providecommand{\titlerunning}{Please define {\tt $\backslash$titlerunning}}
    \providecommand{\authorrunning}{Please define {\tt $\backslash$authorrunning}}
    \providecommand{\copyrightholders}{\authorrunning}
    \renewcommand\thefootnote{\@fnsymbol\c@footnote}%
    \def\@makefnmark{\rlap{\@textsuperscript{\normalfont\@thefnmark}}}%
    \long\def\@makefntext##1{\parindent 1em\noindent
            \hb@xt@1.8em{%
                \hss\@textsuperscript{\normalfont\@thefnmark}}##1}%
    \newpage
    \global\@topnum\z@   % Prevents figures from going at top of page.
    \@maketitle
    \thispagestyle{empty}\@thanks
  \endgroup
  \setcounter{footnote}{0}%
  \label{FirstPage}
  \global\let\thanks\relax
  \global\let\maketitle\relax
  \global\let\@maketitle\relax
  \global\let\@thanks\@empty
  \global\let\@author\@empty
  \global\let\@date\@empty
  \global\let\@title\@empty
  \global\let\title\relax
  \global\let\author\relax
  \global\let\date\relax
  \global\let\and\relax
}
\def\@maketitle{%                       adapted from article.cls
  \newpage
\noindent
\raisebox{-22.8cm}[0pt][0pt]{\footnotesize
\begin{tabular}{@{}l}
\publicationstatus
\end{tabular}}
\hfill\vspace{-1em}
\raisebox{-22.8cm}[0pt][0pt]{\footnotesize
\makebox[0pt][r]{
\begin{tabular}{l@{}}
\ifpublicdomain
  This work is \href{http://creativecommons.org/licenses/publicdomain/}
  {dedicated to the public domain}.
\else
  \ifcopyright
    \copyright~\copyrightholders\\
  \fi
  \ifcreativecommons
    This work is licensed under the
    \ifnoncommercial
      \href{http://creativecommons.org}{Creative Commons}\\
      \ifnoderivs
	\href{http://creativecommons.org/licenses/by-nc-nd/3.0/}
	{Attribution-Noncommercial-No Derivative Works} License.
      \else\ifsharealike
	\href{http://creativecommons.org/licenses/by-nc-sa/3.0/}
	{Attribution-Noncommercial-Share Alike} License.
      \else
	\href{http://creativecommons.org/licenses/by-nc/3.0/}
	{Attribution-Noncommercial} License.
      \fi\fi
    \else
      \ifnoderivs
        \href{http://creativecommons.org}{Creative Commons}\\
	\href{http://creativecommons.org/licenses/by-nd/3.0/}
	{Attribution-No Derivative Works} License.
      \else\ifsharealike
        \href{http://creativecommons.org}{Creative Commons}\\
	\href{http://creativecommons.org/licenses/by-sa/3.0/}
	{Attribution-Share Alike} License.
      \else
        \\\href{http://creativecommons.org}{Creative Commons}
	\href{http://creativecommons.org/licenses/by/3.0/}
	{Attribution} License.
      \fi\fi
    \fi
  \fi
\fi
\end{tabular}}}
  \null
  %\vskip 2em%				a bit of space removed (< 2em)
  \begin{center}%
  \let \footnote \thanks
    {\LARGE\bf \@title \par}%		\bf added
    \vskip 2em%				was: 1.5em
    {\large
      \lineskip .5em%
      \begin{tabular}[t]{c}%
        \@author
      \end{tabular}\par}%
    \vskip 1em%			       \date and extra space removed
  \end{center}%
  \par
  \markboth{\hfill\titlerunning}{\authorrunning\hfill}
  \vskip .5em}

\AtBeginDocument{
  \providecommand{\firstpage}{1}
  \setcounter{firstpage}{\firstpage}
  \setcounter{page}{\firstpage}
  \@ifpackageloaded{array}% Contributed by Wolfgang Jeltsch
    {\newcommand{\IfArrayPackageLoaded}[2]{#1}}
    {\newcommand{\IfArrayPackageLoaded}[2]{#2}}}
\newcommand{\institute}[1]{\IfArrayPackageLoaded
    {\\{\scriptsize\begin{tabular}[t]{@{}>{\footnotesize}c@{}}#1\end{tabular}}}
    {\\{\scriptsize\begin{tabular}[t]{@{\footnotesize}c@{}}#1\end{tabular}}}}
\newcommand{\email}[1]{\\{\footnotesize\tt #1}}

\renewenvironment{abstract}{\begin{list}{}%   header removed and noindent
			{\rightmargin\leftmargin
			\listparindent 1.5em
			\parsep 0pt plus 1pt}
			\small\item}{\end{list}
}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\RequirePackage{hyperref} % add hyperlinks
\RequirePackage{mathptmx} % Pick Times Roman as a base font

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%			Remember page numbers      		    %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\newcounter{firstpage}
\setcounter{firstpage}{1}
\AtEndDocument{\clearpage
   \addtocounter{page}{-1}
   \immediate\write\@auxout{\string
   \newlabel{LastPage}{{}{\thepage}{}{page.\thepage}{}}}%
   \newwrite\pages
   \immediate\openout\pages=\jobname.pag
   \immediate\write\pages{\arabic{firstpage}-\arabic{page}}
   \addtocounter{page}{1}}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%			Less space in lists      		    %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\def\@listi{\leftmargin\leftmargini
            \parsep 2.5\p@ \@plus1.5\p@ \@minus\p@
            \topsep 5\p@   \@plus2\p@ \@minus5\p@
            \itemsep2.5\p@ \@plus1.5\p@ \@minus\p@}
\let\@listI\@listi
\@listi
\def\@listii {\leftmargin\leftmarginii
              \labelwidth\leftmarginii
              \advance\labelwidth-\labelsep
              \topsep    1\p@ \@plus\p@ \@minus\p@
              \parsep    1\p@   \@plus\p@  \@minus\p@
              \itemsep   \parsep}
\def\@listiii{\leftmargin\leftmarginiii
              \labelwidth\leftmarginiii
              \advance\labelwidth-\labelsep
              \topsep    \z@
              \parsep    \z@
              \itemsep   \topsep}

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%       References small and with less space between items       %%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
\renewenvironment{thebibliography}[1]
     {\section*{\refname}\small%              small added
      \list{\@biblabel{\@arabic\c@enumiv}}%
           {\settowidth\labelwidth{\@biblabel{#1}}%
            \leftmargin\labelwidth
            \advance\leftmargin\labelsep
            \@openbib@code
            \usecounter{enumiv}%
            \let\p@enumiv\@empty
            \renewcommand\theenumiv{\@arabic\c@enumiv}}%
      \sloppy
      \clubpenalty4000
      \@clubpenalty \clubpenalty
      \widowpenalty4000%
      \sfcode`\.\@m
		\setlength{\parskip}{0pt}%
		\setlength{\itemsep}{3pt plus 2pt}% less space between items
     }
     {\def\@noitemerr
       {\@latex@warning{Empty `thebibliography' environment}}%
      \endlist}
