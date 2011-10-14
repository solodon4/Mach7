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

void print_cpp(const ipr::Node& n, std::ostream& os = std::cout);
void print_cpp(const ipr::Node& n, std::ostream& os, comments_interface& c);

// FIX: Figure out why for if-then-else with no {} on branches the result of merge
//      is printed before the if-statement
