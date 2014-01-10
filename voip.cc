/*
  Fichero: voip.cc
  Clase que hereda de OnOffApplication
  Emula llamadas sobre VoIP
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
#include "voip.h"
using namespace ns3;
#define ONTIMEVOIP 0.6
#define OFFTIMEVOIP 0.2
/*
  Constructor de la clase Voip.
  Hereda el constructor de OnOffHelper
  Configura la aplicación estableciendo
  una tasa y OnTime, OffTime
  SOBRE UDP
*/
VoipHelper::VoipHelper(Ipv4Address address, uint16_t port)
  :OnOffHelper("ns3::UdpSocketFactory", Address (InetSocketAddress (address,port)))
{
  //Creamos las variables aleatorias para fijar On/Off Time
  varon=CreateObject<ConstantRandomVariable>();
  varoff=CreateObject<ConstantRandomVariable>();
  varon->SetAttribute("Constant", DoubleValue(ONTIMEVOIP));
  varoff->SetAttribute("Constant", DoubleValue(OFFTIMEVOIP));
  //Configuramos la aplicación OnOff
  SetConstantRate (DataRate ("500kb/s"));
  SetAttribute("OnTime", PointerValue(varon));
  SetAttribute("OffTime", PointerValue(varoff));
  

}
VoipHelper::~VoipHelper()
{

}
