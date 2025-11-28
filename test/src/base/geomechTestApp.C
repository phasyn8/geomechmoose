//* This file is part of the MOOSE framework
//* https://mooseframework.inl.gov
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "geomechTestApp.h"
#include "geomechApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"

InputParameters
geomechTestApp::validParams()
{
  InputParameters params = geomechApp::validParams();
  params.set<bool>("use_legacy_material_output") = false;
  params.set<bool>("use_legacy_initial_residual_evaluation_behavior") = false;
  return params;
}

geomechTestApp::geomechTestApp(const InputParameters & parameters) : MooseApp(parameters)
{
  geomechTestApp::registerAll(
      _factory, _action_factory, _syntax, getParam<bool>("allow_test_objects"));
}

geomechTestApp::~geomechTestApp() {}

void
geomechTestApp::registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs)
{
  geomechApp::registerAll(f, af, s);
  if (use_test_objs)
  {
    Registry::registerObjectsTo(f, {"geomechTestApp"});
    Registry::registerActionsTo(af, {"geomechTestApp"});
  }
}

void
geomechTestApp::registerApps()
{
  registerApp(geomechApp);
  registerApp(geomechTestApp);
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
// External entry point for dynamic application loading
extern "C" void
geomechTestApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  geomechTestApp::registerAll(f, af, s);
}
extern "C" void
geomechTestApp__registerApps()
{
  geomechTestApp::registerApps();
}
