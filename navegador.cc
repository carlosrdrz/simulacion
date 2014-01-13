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

#define MAXONNAV 0.4
#define MINONNAV 0.2
#define MAXOFFNAV 0.8
#define MINOFFNAV 0.6

//antiguamente: const char *address
NavegadorHelper::NavegadorHelper(Ipv4Address address, uint16_t port)
  :OnOffHelper("ns3::TcpSocketFactory", Address (InetSocketAddress (address,port)))
{ 
  //Variable para TON
  varon = CreateObject <UniformRandomVariable>();
  varon->SetAttribute("Max", DoubleValue(MAXONNAV));
  varon->SetAttribute("Min", DoubleValue(MINONNAV));
  //Variable para TOFF
  varoff = CreateObject <UniformRandomVariable>();
  varoff->SetAttribute("Max", DoubleValue(MAXOFFNAV));
  varoff->SetAttribute("Min", DoubleValue(MINOFFNAV));
 
  SetConstantRate (DataRate ("500kb/s"));
  SetAttribute("OnTime", PointerValue(varon));
  SetAttribute("OffTime", PointerValue(varoff));

  //std::cout<<"Valor de OnTime: "<<varon->GetValue()<<std::endl;
  //std::cout<<"Valor de OffTime: "<<varoff->GetValue()<<std::endl;
}
NavegadorHelper::~NavegadorHelper()
{

}
