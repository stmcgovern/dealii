
// ---------------------------------------------------------------------
// $Id:
//
// Copyright (C) 2013 by the deal.II authors
//
// This file is part of the deal.II library.
//
// The deal.II library is free software; you can use it, redistribute
// it, and/or modify it under the terms of the GNU Lesser General
// Public License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
// The full text of the license can be found in the file LICENSE at
// the top level of the deal.II distribution.
//
// ---------------------------------------------------------------------


// deal.II includes
#include "../tests.h"
#include <deal.II/lac/petsc_vector.h>

#include <fstream>
#include <iostream>
#include <cassert>

#include <complex>

// test dealii::internal::VectorReference::real()
// test dealii::internal::VectorReference::imag()
// on vector

// vector elements
void test_vector (PETScWrappers::Vector &v)
{
  deallog << "Check vector access" << std::endl;

  // fill up a vector with some numbers
  for (unsigned int k=0; k<v.size(); ++k)
    v(k) = std::complex<double> (k,v.size()-k);

  v.compress (VectorOperation::add);

  // check that is what we get by casting PetscScalar to std::real()
  // and std::imag()
  for (unsigned int k=0; k<v.size(); ++k)
    Assert ((static_cast<std::complex<double> > (v(k)).real ()==k) &&
            (static_cast<std::complex<double> > (v(k)).imag ()==v.size()-k),
            ExcInternalError());

  // check that is what we get by
  // dealii::internal::VectorReference::real() and
  // dealii::internal::VectorReference::imag()
  for (unsigned int k=0; k<v.size(); ++k)
    Assert ((v(k).real ()==k) && (v(k).imag ()==v.size()-k),
            ExcInternalError());

  deallog << "OK" << std::endl;
}

int main (int argc, char **argv)
{
  std::ofstream logfile ("output");
  dealii::deallog.attach (logfile);
  dealii::deallog.depth_console (0);
  deallog.threshold_double(1.e-10);

  try
    {
      PetscInitialize (&argc, &argv, (char *) 0, (char *) 0);
      {
        PETScWrappers::Vector v (5);
        test_vector (v);

        deallog << "vector:" << std::endl;
        v.print (logfile);
      }
      PetscFinalize ();
    }


  catch (std::exception &exc)
    {
      std::cerr << std::endl << std::endl
                << "----------------------------------------------------"
                << std::endl;
      std::cerr << "Exception on processing: " << std::endl
                << exc.what() << std::endl
                << "Aborting!" << std::endl
                << "----------------------------------------------------"
                << std::endl;

      return 1;
    }
  catch (...)
    {
      std::cerr << std::endl << std::endl
                << "----------------------------------------------------"
                << std::endl;
      std::cerr << "Unknown exception!" << std::endl
                << "Aborting!" << std::endl
                << "----------------------------------------------------"
                << std::endl;
      return 1;
    }

  logfile << std::endl;

  return 0;
}


