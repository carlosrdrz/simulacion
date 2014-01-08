/*
  Fichero: navegador.cc
  Clase que hereda de OnOffApplication
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
#include "navegador.h"
using namespace ns3;

//antiguamente: const char *address
NavegadorHelper::NavegadorHelper(double ton, double toff, Ipv4Address address, uint16_t port)
  :OnOffHelper("ns3::UdpSocketFactory", Address (InetSocketAddress (address,port)))
{
  varon=CreateObject<ConstantRandomVariable>();
  varoff=CreateObject<ConstantRandomVariable>();
  varon->SetAttribute("Constant", DoubleValue(ton));
  varoff->SetAttribute("Constant", DoubleValue(toff));
  //Probar con this->
  SetConstantRate (DataRate ("500kb/s"));
  SetAttribute("OnTime", PointerValue(varon));
  SetAttribute("OffTime", PointerValue(varoff));
  

}
NavegadorHelper::~NavegadorHelper()
{

}
