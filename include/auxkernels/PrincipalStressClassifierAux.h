#pragma once

#include "AuxKernel.h"
#include "RankTwoTensor.h"

class PrincipalStressClassifierAux : public AuxKernel
{
public:
  static InputParameters validParams();
  PrincipalStressClassifierAux(const InputParameters & parameters);

protected:
  virtual Real computeValue() override;

  const MaterialProperty<RankTwoTensor> & _stress;

  const MooseEnum _quantity;

  const RealVectorValue _vertical;

  // ✅ ADD THIS
  const Real _tol;

  mutable bool _computed;

  // principal stresses
  mutable Real _Sv, _SHmax, _Shmin;

  // eigenvectors
  mutable RealVectorValue _v_Sv, _v_SHmax, _v_Shmin;

  // ✅ ADD THIS
  mutable int _regime;

  void computePrincipalData() const;
};