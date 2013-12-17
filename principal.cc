/*
  Fichero: principal.cc
  Descripción: aquí se implementará el main
  y la topología.
*/

#include <iostream>
#include <fstream>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("CsmaMulticastExample");

int
main ( int argc, char * argv[])
{
   Config::SetDefault ("ns3::CsmaNetDevice::EncapsulationMode", StringValue ("Dix"));

  // Allow the user to override any of the defaults at
  // run-time, via command-line arguments
  CommandLine cmd;
  cmd.Parse (argc, argv);

  NS_LOG_INFO ("Create nodes.");
  NodeContainer c;
  c.Create (4);
  // We will later want two subcontainers of these nodes, for the two LANs
  //Cada NodeContainer para una subred. En total son 3 subredes.
  NodeContainer cacceso0 = NodeContainer (c.Get (0), c.Get (1));
  NodeContainer ctroncal = NodeContainer (c.Get (1), c.Get (2));
  NodeContainer cacceso1 = NodeContainer (c.Get (2), c.Get(3));


  NS_LOG_INFO ("Build Topology.");
  //CsmaHelper para las redes de acceso
  CsmaHelper csma_acceso;
  csma_acceso.SetChannelAttribute ("DataRate", DataRateValue (DataRate (5000000)));
  csma_acceso.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
  //CsmaHelper para la red troncal
  CsmaHelper csma_troncal;
  csma_troncal.SetChannelAttribute ("DataRate", DataRateValue (DataRate (5000000)));
  csma_troncal.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
  

  // We will use these NetDevice containers later, for IP addressing
  NetDeviceContainer ndacceso0 = csma.Install (cacceso0);  // Primera red de acceso
  NetDeviceContainer ndacceso1 = csma.Install (cacceso1);  // Segunda red de acceso
  NetDeviceContainer ndtroncal = csma.Install (ctroncal);  // Red troncal


  NS_LOG_INFO ("Add IP Stack.");
  InternetStackHelper internet;
  internet.Install (c);

  NS_LOG_INFO ("Assign IP Addresses.");
  Ipv4AddressHelper ipv4Addr;
  ipv4Addr.SetBase ("10.1.1.0", "255.255.255.0");
  ipv4Addr.Assign (ndacceso0);
  ipv4Addr.SetBase ("10.1.2.0", "255.255.255.0");
  ipv4Addr.Assign (ndacceso1);
  ipv4Addr.SetBase ("10.1.3.0", "255.255.255.0");
  ipv4Addr.Assign (ndtroncal);
  


}
