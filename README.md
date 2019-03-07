<a href="http://bit.ly/OPMPoster">
  <img src="https://raw.githubusercontent.com/solodon4/Mach7/master/media/posters/OpenPatternMatching-OOPSLA%20(1280x989).jpg" width="100%">
</a>

Mach7: Pattern Matching for C++ [![Build Status: Linux, OSX](https://travis-ci.org/solodon4/Mach7.svg?branch=master)](https://travis-ci.org/solodon4/Mach7) [![Build Status: Windows](https://ci.appveyor.com/api/projects/status/github/solodon4/mach7?branch=master&svg=true)](https://ci.appveyor.com/project/solodon4/mach7)
===============================

by [Yuriy Solodkyy](https://github.com/solodon4), [Gabriel Dos Reis](https://github.com/GabrielDosReis), [Bjarne Stroustrup](https://github.com/BjarneStroustrup)

Abstract
--------

Pattern matching is an abstraction mechanism that can greatly simplify source code.
Commonly, pattern matching is built into a language to provide better syntax, faster 
code, correctness guarantees and improved diagnostics. Mach7 is a library solution 
to pattern matching in C++ that maintains many of these features. All the patterns 
in Mach7 are user-definable, can be stored in variables, passed among functions, 
and allow the use of open class hierarchies.

Mach7 by Example
----------------

Fibonacci numbers demonstrates [the use of patterns with built-in types](https://github.com/solodon4/Mach7/blob/master/code/test/time/numbers.cpp#L202-L216) in Mach7:

```C++
// Fibonacci numbers
int fib(int n)
{
    var<int> m;

    Match(n)
    {
      Case(1)     return 1;
      Case(2)     return 1;
      Case(2*m)   return sqr(fib(m+1)) - sqr(fib(m-1));
      Case(2*m+1) return sqr(fib(m+1)) + sqr(fib(m));
    }
    EndMatch
}
```

Lambda calculator demonstrates [use of pattern matching to decompose objects and nested patterns](https://github.com/solodon4/Mach7/blob/master/code/test/time/lambda.cpp#L101-L118):

```C++
// Declare C++ equivalent of an Algebraic Data Type Term and its 3 variants: 
struct Term       { virtual ~Term() {}     };
struct Var : Term { std::string name;      };
struct Abs : Term { Var&  var;  Term& body;};
struct App : Term { Term& func; Term& arg; };

// Tell Mach7 library which members should be bound in which binding positions
namespace mch
{
    template <> struct bindings<Var> { Members(Var::name); };
    template <> struct bindings<Abs> { Members(Abs::var , Abs::body); };
    template <> struct bindings<App> { Members(App::func, App::arg);  };
}

// Implement fully-functional lambda-calculator
Term* eval(Term* t)
{
    var<const Var&> v; 
    var<const Term&> b,a;

    Match(*t)
    {
      Case(C<Var>())               return &match0;
      Case(C<Abs>())               return &match0;
      Case(C<App>(C<Abs>(v,b),a))  return eval(subs(b,v,a));
      Otherwise() cerr << "error"; return nullptr ;
    } 
    EndMatch
}
```

It can also be used to demonstrate [relational matching on several arguments](https://github.com/solodon4/Mach7/blob/master/code/test/time/lambda.cpp#L127-L144):

```C++
bool operator==(const Term& left, const Term& right)
{
    var<std::string> s;
    var<const Term&> v,t,f;

    Match(left,right)
    {
      Case(C<Var>(s),     C<Var>(+s)     ) return true;
      Case(C<Abs>(&v,&t), C<Abs>(&+v,&+t)) return true;
      Case(C<App>(&f,&t), C<App>(&+f,&+t)) return true;
      Otherwise()                          return false;
    }
    EndMatch

    return false; // To prevent all control path warning
}
```

Next example demonstrates that [the library can deal efficiently and in a type-safe manner with non-polymorphic classes](https://github.com/solodon4/Mach7/blob/master/code/test/unit/xtl.cpp#L354-L383)
like boost::variant as well.

```C++
void print(const boost::variant<double,float,int>& v)
{
    var<double> d; var<float> f; var<int> n;

    Match(v)
    {
      Case(C<double>(d)) cout << "double " << d << endl; break;
      Case(C<float> (f)) cout << "float  " << f << endl; break;
      Case(C<int>   (n)) cout << "int    " << n << endl; break;
    }
    EndMatch
}
```

Breve syntax is not the only thing Mach7 has to offer - the generated code is 
[faster than Visitors](http://bit.ly/OPM13Poster)!

For a more detailed set of examples, have a look at the code that was prepared for 
[CppCon 2014 presentation](http://bit.ly/AcceptNoVisitors),
and implemented using [visitors](https://github.com/solodon4/Mach7/blob/master/code/test/unit/cppcon-visitors.cpp) as well as
[pattern matching](https://github.com/solodon4/Mach7/blob/master/code/test/unit/cppcon-matching.cpp). These are simple enough
to help you get started on your own Mach7 project.

Continuous Integration
----------------------

We use [Travis CI](https://travis-ci.org/solodon4/Mach7) and [AppVeyor](https://ci.appveyor.com/project/solodon4/mach7) for continuous integration 
and currently have all check-ins validated in the following configurations:

| [![Build Status](https://travis-ci.org/solodon4/Mach7.svg?branch=master)](https://travis-ci.org/solodon4/Mach7) | [G++](http://gcc.gnu.org/) | [Clang](http://clang.llvm.org/) |
|-------|-----|-------|
| Linux | 4.9 | 3.4   |
| OSX   | 4.9 | 3.5   |

| [![Build Status: Visual C++](https://ci.appveyor.com/api/projects/status/github/solodon4/mach7?branch=master&svg=true)](https://ci.appveyor.com/project/solodon4/mach7) | 2017 | 2015 | 2013 | 2012 | 2010 | /analyze |
|-------|------|------|------|------|------|------|
| x86   |  OK  |  OK  |  OK  |  OK  |  OK  |  OK  |
| x64   |  OK  |  OK  |  OK  |  OK  |  N/A |  OK  |

Branches
--------

 * master  - main development branch
 * release - cleaned-up branch with non-essential files deleted. FI from but does not RI back to master to avoid deletion of files there. Don't do any actual editing in this branch.

Building sources
----------------

If you haven't done so yet, get a copy of this [Git](https://git-scm.com/book/en/v2/Getting-Started-Installing-Git)
repo locally by executing:

    git clone https://github.com/solodon4/Mach7.git

The library itself is header only and does not require building. To build unit and 
timing tests we've accumulated over time several scripts, which we don't completely
abandon in favor of newer ones as they maintain the flags the original experiments
on the library were built with.

#### Using CMake (3.2 or later)

 [CMake](https://cmake.org/) support is the most recent and is still very experimental at this point. To
 build with cmake, perform the following commands from within Mach7 folder:

    cmake -H. -Bbuild 
    cmake --build build --target install
    
Note: this requires administrative privileges under all systems, if you don't like this, try commands below:

    cmake -H. -Bbuild -DCMAKE_INSTALL_PREFIX=/wherever/doesn't/require/administrative/priviege
    cmake --build build --target install
    
But please make sure msvc/clang/gcc is able to find the path your provide above when
including Mach7's headers in your own project:

- MSVC: https://stackoverflow.com/questions/335408/where-does-visual-studio-look-for-c-header-files
- Gcc/Clang: -I
- CMake: https://stackoverflow.com/questions/13703647/how-to-properly-add-include-directories-with-cmake 

#### Using Makefiles for GCC (4.4 or later) or Clang (3.3 or later)
 
Top-level Makefile synopsis:
 
    make         - build all library tests
    make all     - same as above right now
    make unit    - build all unit tests
    make time    - build all timing tests
    make cmpl    - build all tests for timing the compilation times of the library
    make clean   - clean all built targets and intermediaries
    make test    - run all the built tests
    make check   - run those tests for which there are correct_output/*.out files and check that the output is the same
    make doc     - build Mach7 documentation (requires doxygen)
    make includes.png - build graph representation of header inclusions (requires graphviz dot)

To see a list of more specific targets supported by other makefiles, see comments inside them. 

To build a particular file, say test/unit/example05.cpp, build a target with 
the same filename and extension .exe instead of .cpp (even on Unix family OS).
For example:

 ```
 cd $MACH7_ROOT/code/test/unit
 make example05.exe
 ```

Lower-level makefiles support most of the phony targets of the top-level makefile, 
to which the top-level makefile forwards the corresponding calls. For example:

To build and run just the unit tests:
 
 ```
 cd $MACH7_ROOT/code/test/unit
 make
 make check
 make test
 ```
 
Similarly, to build and run all the timing tests:
 
 ```
 cd $MACH7_ROOT/code/test/time
 make
 make test
 ```

#### Using Visual C++ (2010 or later)

 Mach7 uses its own [build.bat](https://github.com/solodon4/Mach7/blob/master/code/build.bat) 
 script to build all the examples and unit tests that come with it. The script 
 assumes each .cpp file to be a standalone program. You can find 
 [the most up-to-date list of supported commands](https://github.com/solodon4/Mach7/blob/master/code/build.bat#L43-L73) 
 by running:

    build.bat /?

######  Syntax:

    build [ pgo | repro | tmp | <ver> ] [ filemask*.cpp ... ]
    build [ syntax | timing | cmp | doc | clean | test | check ]

######  Commands supported so far:

    build [ pgo | repro | tmp | <ver> | <arch> ] [ filemask*.cpp ... ] - build given C++ files
    build        - Build all examples using the most recent MS Visual C++ compiler installed
    build unit   - Build all unit tests
    build syntax - Build all supported library options combination for syntax variations
    build timing - Build all supported library options combination for timing variations
    build cmp    - Build all executables for comparison with other languages
    build doc    - Build Mach7 documentation
    build clean  - Clean all built examples
    build test   - Run all built examples
    build check  - Run those examples for which there are correct_output/*.out files and 
                   check that output is the same

######  Modifiers:

           pgo   - Perform Profile-Guided Optimization on produced executables
           repro - In case of error, create and compile a pre-processed repro
           tmp   - Keep temporaries
          <ver>  - Use a specific version of Visual Studio to compile the source 
                   code. <ver> can be one of the following:
                    - 2017 - Visual C++ 15.0
                    - 2015 - Visual C++ 14.0
                    - 2013 - Visual C++ 12.0
                    - 2012 - Visual C++ 11.0
                    - 2010 - Visual C++ 10.0
                    - 2008 - Visual C++  9.0
                    - 2005 - Visual C++  8.0
                    - 2003 - Visual C++  7.1
                      0000 - Do not use any VS to set up the environment, I will set it up by myself
          <arch> - Target architecture. Can be one of the following: x86, x64, arm

Talks
-------------

 * "[Accept No Visitors](http://bit.ly/AcceptNoVisitorsVideo)". [CppCon 2014](http://cppcon.org/). September 12, 2014. Bellevue, WA. [[slides](http://bit.ly/AcceptNoVisitors), [video](http://bit.ly/AcceptNoVisitorsVideo)]
 * "[Mach7: The Design and Evolution of a Pattern Matching Library for C++](http://bit.ly/Mach7CppNowVideo)". [C++ Now 2014](http://cppnow.org). May 14, 2014. Aspen, CO. [[slides](http://bit.ly/Mach7CppNow), [video](http://bit.ly/Mach7CppNowVideo)]

Publications
------------

 * Y.Solodkyy, G.Dos Reis, B.Stroustrup. "[Open Pattern Matching for C++: Extended Abstract](http://bit.ly/OPM13EA)" In Proceedings of the 2013 companion publication for conference on Systems, programming, & applications: software for humanity (SPLASH '13). ACM, New York, NY, USA, pp. 97-98. [pdf](http://bit.ly/OPM13EA), [slides](http://bit.ly/OPM13Slides), [notes](http://bit.ly/OPM13Notes), [poster](http://bit.ly/OPM13Poster), [project](http://bit.ly/Mach7)
 * Y.Solodkyy, G.Dos Reis, B.Stroustrup. "[Open Pattern Matching for C++](http://bit.ly/OPM-GPCE13)" In Proceedings of the 12th international conference on Generative programming: concepts & experiences (GPCE '13). ACM, New York, NY, USA, pp. 33-42. [pdf](http://bit.ly/OPM-GPCE13), [slides](http://bit.ly/OPM13Slides), [notes](http://bit.ly/OPM13Notes), [poster](http://bit.ly/OPM13Poster), [project](http://bit.ly/Mach7)
 * Y.Solodkyy. "[Simplifying the Analysis of C++ Programs](http://bit.ly/PhdThesis)" Ph.D. Thesis. Texas A&M University. August 2013. [pdf](http://bit.ly/SolodkyyThesis), [slides](http://bit.ly/SolodkyyThesisSlides)
 * Y.Solodkyy, G.Dos Reis, B.Stroustrup. "[Open and Efficient Type Switch for C++](http://bit.ly/TS-OOPSLA12)" In Proceedings of the ACM international conference on Object Oriented Programming Systems Languages and Applications (OOPSLA '12). ACM, New York, NY, USA, pp. 963-982. [pdf](http://bit.ly/TS-OOPSLA12), [slides](http://bit.ly/TS12Slides), [notes](http://bit.ly/TS12Notes), [poster](http://bit.ly/OPM13Poster), [extras](http://bit.ly/TS12Extras), [project](http://bit.ly/Mach7)]

Others about Mach7
------------------
 * 2016-02-27: [Andrzej's C++ blog](https://akrzemi1.wordpress.com/?s=Mach7): [Another polymorphism](https://akrzemi1.wordpress.com/2016/02/27/another-polymorphism/)

Projects using Mach7
--------------------
 * [Yodl](https://github.com/forflo/yodl): a VHDL frontend for Yosys
 * [Arrow](https://github.com/arrow-lang/arrow): Arrow is a fast (as or faster than C) general-purpose programming language. It does not employ a garbage collector and has minimal runtime overhead.

License
-------

Mach7 is licensed under the [BSD License](LICENSE).

Support
-------

If you have any question about Mach7 or have trouble using it, the best way to get answers is to post an 
[issue](https://github.com/solodon4/Mach7/issues) and label it as 
[Question](https://github.com/solodon4/Mach7/issues?q=label%3Aquestion). This will contribute to our 
[poor man's FAQ](https://github.com/solodon4/Mach7/issues?q=label%3Aquestion) and hopefully help others 
with a similar question. I get notifications about new issues and usually respond within the 
same day. If you prefer not to discuss your question on GitHub, feel free to send me a 
[private email](mailto:yuriy.solodkyy+mach7support@gmail.com) (note there is a 
[+](https://gmail.googleblog.com/2008/03/2-hidden-ways-to-get-more-from-your.html) in the email address).

Call for Help
-------------

We are looking for contributors to the project. If you are a student taking a 
programming languages class or any other class that would require you to write 
a small compiler or interpreter, we would love you try Mach7 for the job. We 
promise to help with any issues you might have with the library.

Known bugs and limitations
--------------------------

Right now, there are several experimental headers that one would need to include to enable one or the other syntax 
to work. This is a work in progress, so before you start working with a particular syntax, check examples with that
syntax and make note of which of headers they include. We will clear this eventually leaving only one header, but at 
the moment it is a mess, and the most intuitive match.hpp is probably not the header you want as it represents older
experiments. The most recent experimentation and the header you are probably looking for is
[mach7/type_switchN-patterns-xtl.hpp](https://github.com/solodon4/Mach7/blob/master/code/mach7/type_switchN-patterns-xtl.hpp).

The library is not yet suitable for multi-threaded environment. Lock-free version of vtbl-map is in the works.

Please refrain from using solution or project files checked in here. They are not in sync with most recent changes
to directory structure and are difficult to maintain. They will ultimately be replaced with a less verbose system 
(likely CMake), and in the meantime please use build.bat to build tests on Windows. 

For the most up-to-date list of known issues see [Mach7 Issues](https://github.com/solodon4/Mach7/issues).
