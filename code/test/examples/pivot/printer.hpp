#pragma once

#include <iostream>
#include <string>
#include <ipr/interface>  // Pivot interfaces

/// An interface a user can override providing his own object that will
/// provide comments to the printed text.
/// FIX: Not the best interface since methods are always called even when
///      none comment was provided and a whole empty string is created.
struct comments_interface
{
    virtual std::string before(const ipr::Stmt&) { return std::string(); }
    virtual std::string  after(const ipr::Stmt&) { return std::string(); }
};

/// Pattern-matching version of C++ printer
namespace cxxm
{
    void print_cpp(const ipr::Node& n, std::ostream& os = std::cout);
    void print_cpp(const ipr::Node& n, std::ostream& os, comments_interface& c);
}

/// Visitors version of C++ printer
namespace cxxv
{
    void print_cpp(const ipr::Node& n, std::ostream& os = std::cout);
    void print_cpp(const ipr::Node& n, std::ostream& os, comments_interface& c);
}

namespace cxx
{
    using cxxm::print_cpp;
}

// FIX: Figure out why for if-then-else with no {} on branches the result of merge
//      is printed before the if-statement
