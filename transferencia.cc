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
#include "transferencia.h"

using namespace ns3;

TransferenciaHelper::TransferenciaHelper(Ipv4Address address, uint16_t port)
  : BulkSendHelper("ns3::TcpSocketFactory", Address(InetSocketAddress (address,port)))
{
  maxBytes = 1000;
  SetAttribute("MaxBytes", UintegerValue(maxBytes));
}

TransferenciaHelper::~TransferenciaHelper()
{

}