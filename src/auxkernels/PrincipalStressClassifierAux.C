#include "PrincipalStressClassifierAux.h"

registerMooseObject("geomechApp", PrincipalStressClassifierAux);

InputParameters
PrincipalStressClassifierAux::validParams()
{
  InputParameters params = AuxKernel::validParams();

  params.addRequiredParam<MaterialPropertyName>("stress", "Stress tensor");

  MooseEnum quantity(
    "Sv SHmax Shmin Sv_x Sv_y Sv_z SHmax_x SHmax_y SHmax_z Shmin_x Shmin_y Shmin_z regime",
    "Sv");

  params.addParam<MooseEnum>("quantity", quantity, "Requested output");

  params.addParam<RealVectorValue>(
      "vertical", RealVectorValue(0, 0, 1), "Vertical direction");

  params.addParam<Real>("tolerance", 1e-8, "Degeneracy tolerance");

  return params;
}

PrincipalStressClassifierAux::PrincipalStressClassifierAux(
    const InputParameters & parameters)
  : AuxKernel(parameters),
    _stress(getMaterialProperty<RankTwoTensor>("stress")),
    _quantity(getParam<MooseEnum>("quantity")),
    _vertical(getParam<RealVectorValue>("vertical")),
    _tol(getParam<Real>("tolerance")),
    _computed(false)
{
}

void
PrincipalStressClassifierAux::computePrincipalData() const
{
  if (_computed)
    return;

  const RankTwoTensor & s = _stress[_qp];

  std::vector<Real> eigvals(3);
  RankTwoTensor eigvecs;

  s.symmetricEigenvaluesEigenvectors(eigvals, eigvecs);

  // Normalize vertical
  RealVectorValue vhat = _vertical / _vertical.norm();

  // Extract eigenvectors (columns)
  std::vector<RealVectorValue> eigvec_list(3);

  for (int i = 0; i < 3; ++i)
  {
    eigvec_list[i] = RealVectorValue(
        eigvecs(0, i),
        eigvecs(1, i),
        eigvecs(2, i));

    eigvec_list[i] /= eigvec_list[i].norm();
  }

  // --- Identify Sv (most aligned with vertical)
  int i_sv = 0;
  Real max_dot = -1.0;

  for (int i = 0; i < 3; ++i)
  {
    Real d = std::abs(eigvec_list[i] * vhat);
    if (d > max_dot)
    {
      max_dot = d;
      i_sv = i;
    }
  }

  _Sv = eigvals[i_sv];
  _v_Sv = eigvec_list[i_sv];

  // Remaining indices
  int i1 = (i_sv + 1) % 3;
  int i2 = (i_sv + 2) % 3;

  // Handle degeneracy
  if (std::abs(eigvals[i1] - eigvals[i2]) < _tol)
  {
    // most compressive = most negative
    _SHmax = std::min(eigvals[i1], eigvals[i2]);
    _Shmin = std::max(eigvals[i1], eigvals[i2]);

    _v_SHmax = eigvec_list[i1];
    _v_Shmin = eigvec_list[i2];
  }
  else
  {
    // Most compressive = most negative = smaller value
  if (eigvals[i1] < eigvals[i2])
  {
    _SHmax = eigvals[i1];      // more negative → more compressive
    _v_SHmax = eigvec_list[i1];

    _Shmin = eigvals[i2];      // less negative
    _v_Shmin = eigvec_list[i2];
  }
  else
  {
    _SHmax = eigvals[i2];
    _v_SHmax = eigvec_list[i2];

    _Shmin = eigvals[i1];
    _v_Shmin = eigvec_list[i1];
  }
  }

  // --- Stress regime classification
  //
  // Convention (Andersonian):
  // σ1 ≥ σ2 ≥ σ3
  //
  // Normal Faulting (NF): Sv = σ1
  // Strike-Slip (SS):     Sv = σ2
  // Thrust Faulting (TF): Sv = σ3

  // Sort eigenvalues
  std::vector<Real> sorted = eigvals;
  std::sort(sorted.begin(), sorted.end());  // ascending

  Real sigma1 = sorted[0]; // most negative → most compressive
  Real sigma2 = sorted[1];
  Real sigma3 = sorted[2]; // least compressive

  if (std::abs(_Sv - sigma1) < _tol)
    _regime = 0; // NF: Sv = σ1 (most compressive)
  else if (std::abs(_Sv - sigma2) < _tol)
    _regime = 1; // SS: Sv = σ2
  else
    _regime = 2; // TF: Sv = σ3 (least compressive)

  _computed = true;
}

Real
PrincipalStressClassifierAux::computeValue()
{
  _computed = false;
  computePrincipalData();

  switch (_quantity)
  {
    case 0: return _Sv;
    case 1: return _SHmax;
    case 2: return _Shmin;

    case 3: return _v_Sv(0);
    case 4: return _v_Sv(1);
    case 5: return _v_Sv(2);

    case 6: return _v_SHmax(0);
    case 7: return _v_SHmax(1);
    case 8: return _v_SHmax(2);

    case 9: return _v_Shmin(0);
    case 10: return _v_Shmin(1);
    case 11: return _v_Shmin(2);

    case 12: return static_cast<Real>(_regime);

    default:
      mooseError("Invalid quantity selection");
  }
}