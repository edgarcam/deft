// Deft is a density functional package developed by the research
// group of Professor David Roundy
//
// Copyright 2010 The Deft Authors
//
// Deft is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// You should have received a copy of the GNU General Public License
// along with deft; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
//
// Please see the file AUTHORS for a list of authors.

#include "Functionals.h"
#include "utilities.h"

const double R = 2.7;

int main(int, char **) {
  Functional kT(water_prop.kT), x(Identity());
  kT.set_name("kT");
  (kT + x).create_header("tests/generated/sum.h", "Sum", "kT");
  log(x).create_header("tests/generated/log.h", "Log");
  log(1-x).create_header("tests/generated/log-one-minus-x.h", "LogOneMinusX");
  log(1-StepConvolve(R)).create_header("tests/generated/log-one-minus-nbar.h", "LogOneMinusNbar", "R");
  sqr(xShellConvolve(R)).create_header("tests/generated/sqr-xshell.h", "SquareXshell", "R");
}
