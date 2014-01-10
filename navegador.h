/*
  Fichero: navegador.h
  Descripción: librería de la clase Navegador
  que hereda de OnOffHelper
*/
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <cassert>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/callback.h"
#include "ns3/internet-module.h"


using namespace ns3;

class NavegadorHelper : public OnOffHelper
{
public:
  NavegadorHelper (double ton, double toff,Ipv4Address address, uint16_t port);
  ~NavegadorHelper ();

private:
  Ptr<ConstantRandomVariable> varon;
  Ptr<ConstantRandomVariable> varoff;

};
