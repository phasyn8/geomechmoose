//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PrincipalStressDirectionsAux.h"

#include "metaphysicl/raw_type.h"

registerMooseObject("SolidMechanicsApp", PrincipalStressDirectionsAux);
registerMooseObject("SolidMechanicsApp", ADPrincipalStressDirectionsAux);
registerMooseObject("SolidMechanicsApp", MaterialSymmetricRankFourTensorAux);
registerMooseObject("SolidMechanicsApp", ADMaterialSymmetricRankFourTensorAux);

template <typename T, bool is_ad>
InputParameters
PrincipalStressDirectionsAuxTempl<T, is_ad>::validParams()
{
  InputParameters params = MaterialAuxBaseTempl<T, is_ad>::validParams();
  params.addClassDescription(
      "Access a component of a RankTwoTensor for automatic material property output");
  params.addRequiredParam<unsigned int>("i", "The index i of ij for the tensor to output");
  params.addRequiredParam<unsigned int>("j", "The index j of ij for the tensor to output");
  return params;
}

template <typename T, bool is_ad>
PrincipalStressDirectionsAuxTempl<T, is_ad>::PrincipalStressDirectionsAuxTempl(
    const InputParameters & parameters)
  : MaterialAuxBaseTempl<T, is_ad>(parameters),
    _i(this->template getParam<unsigned int>("i")),
    _j(this->template getParam<unsigned int>("j"))
{
  mooseAssert(_i < T::N, "i component out of range.");
  mooseAssert(_j < T::N, "j component out of range.");
}

template <typename T, bool is_ad>
Real
PrincipalStressDirectionsAuxTempl<T, is_ad>::getRealValue()
{
  RankTwoTensor eigvecs=RankTwoTensor();
  std::vector<Real> eigvals;


  this->symmetricEigenvaluesEigenvectors(eigvals,eigvecs)
  return MetaPhysicL::raw_value(eigvecs->_full_value(_i, _j));
}

template class PrincipalStressDirectionsAuxTempl<RankTwoTensor, false>;
template class PrincipalStressDirectionsAuxTempl<RankTwoTensor, true>;
template class PrincipalStressDirectionsAuxTempl<SymmetricRankFourTensor, false>;
template class PrincipalStressDirectionsAuxTempl<SymmetricRankFourTensor, true>;