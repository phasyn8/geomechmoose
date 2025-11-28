//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "MaterialAuxBase.h"
#include "RankTwoTensor.h"
#include "SymmetricRankFourTensor.h"

/**
 * PrincipalStressDirectionsAux is designed to take the data in the RankTwoTensor material
 * property, for example stress or strain, and output the value for the
 * supplied indices.
 */

template <typename T, bool is_ad>
class PrincipalStressDirectionsAuxTempl : public MaterialAuxBaseTempl<T, is_ad>
{
public:
  static InputParameters validParams();

  PrincipalStressDirectionsAuxTempl(const InputParameters & parameters);

protected:
  virtual Real getRealValue() override;

  ///@{ tensor indices
  const unsigned int _i;
  const unsigned int _j;
  ///@}
};

typedef PrincipalStressDirectionsAuxTempl<RankTwoTensor, false> PrincipalStressDirectionsAux;
typedef PrincipalStressDirectionsAuxTempl<RankTwoTensor, true> ADPrincipalStressDirectionsAux;
typedef PrincipalStressDirectionsAuxTempl<SymmetricRankFourTensor, false>
    MaterialSymmetricRankFourTensorAux;
typedef PrincipalStressDirectionsAuxTempl<SymmetricRankFourTensor, true>
    ADMaterialSymmetricRankFourTensorAux;
