/*
  Fichero: navegador.h
  Descripción: librería de la clase Navegador
  que hereda de OnOffHelper
*/

#pragma once

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
	
  NavegadorHelper (Ipv4Address address, uint16_t port);
  ~NavegadorHelper ();

private:
  //Declaración de variables uniformes aleatorias
  //para ajustar Ton y Toff
  Ptr<UniformRandomVariable> varon;
  Ptr<UniformRandomVariable> varoff;
};
