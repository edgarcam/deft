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

class ChemicalPotentialType : public FieldFunctionalInterface {
public:
  ChemicalPotentialType(double chemical_potential)
    : mu(chemical_potential) {}

  VectorXd transform(const GridDescription &, const VectorXd &data) const {
    return mu*data;
  }
  double transform(double n) const {
    return mu*n;
  }
  double grad(double) const {
    return mu;
  }

  void grad(const GridDescription &, const VectorXd &, const VectorXd &ingrad,
            VectorXd *outgrad, VectorXd *outpgrad) const {
    *outgrad += mu*ingrad;
    if (outpgrad) *outpgrad += mu*ingrad;
  }
private:
  double mu; // the chemical potential
};

FieldFunctional ChemicalPotential(double chemical_potential) {
  return FieldFunctional(new ChemicalPotentialType(chemical_potential), "chemical potential");
}
