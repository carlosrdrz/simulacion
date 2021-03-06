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

NS_LOG_COMPONENT_DEFINE("Navegador");

#define MAXONNAV 0.4
#define MINONNAV 0.2
#define MAXOFFNAV 15
#define MINOFFNAV 8

//antiguamente: const char *address
NavegadorHelper::NavegadorHelper(Ipv4Address address, uint16_t port)
  :OnOffHelper("ns3::TcpSocketFactory", Address (InetSocketAddress (address,port)))
{ 
  NS_LOG_FUNCTION(this);
  //Variable para TON
  varon = CreateObject <UniformRandomVariable>();
  varon->SetAttribute("Max", DoubleValue(MAXONNAV));
  varon->SetAttribute("Min", DoubleValue(MINONNAV));
  //Variable para TOFF
  varoff = CreateObject <UniformRandomVariable>();
  varoff->SetAttribute("Max", DoubleValue(MAXOFFNAV));
  varoff->SetAttribute("Min", DoubleValue(MINOFFNAV));
 
  SetConstantRate (DataRate ("100Mbps"));
  SetAttribute("OnTime", PointerValue(varon));
  SetAttribute("OffTime", PointerValue(varoff));
}