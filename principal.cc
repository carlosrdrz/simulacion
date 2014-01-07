
/*
  Fichero: principal.cc
  Descripción: aquí se implementará el main
  y la topología.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"

const double distance = 50.0;

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("CsmaMulticastExample");

int
main ( int argc, char * argv[])
{
  Config::SetDefault ("ns3::CsmaNetDevice::EncapsulationMode", StringValue ("Dix"));

  unsigned nodos_acceso_1 = 2;
  unsigned nodos_acceso_2 = 2;
  unsigned nodos_wifi = 1;
  std::string data_rate_1   = "5Mbps";
  std::string data_rate_2   = "5Mbps";
  std::string data_rate_t   = "5Mbps";
  std::string delay_1       = "0.002";
  std::string delay_2       = "0.002";
  std::string delay_t       = "0.002";

  CommandLine cmd; // @FIXME
  cmd.AddValue("NumeroNodosAcceso1", "Número de nodos en la red de acceso 1", nodos_acceso_1);
  cmd.AddValue("NumeroNodosAcceso2", "Número de nodos en la red de acceso 2", nodos_acceso_2);
  cmd.AddValue("NumeroNodosWifi",    "Número de nodos que usan wifi",         nodos_wifi);
  cmd.AddValue("DataRate1",          "Capacidad de la red de acceso 1",       data_rate_1);
  cmd.AddValue("DataRate2",          "Capacidad de la red de acceso 2",       data_rate_2);
  cmd.AddValue("DataRatet",          "Capacidad de la red troncal",           data_rate_2);
  cmd.AddValue("Delay1",             "Retardo de la red de acceso 1",         delay_1);
  cmd.AddValue("Delay2",             "Retardo de la red de acceso 2",         delay_2);
  cmd.AddValue("Delayt",             "Retardo de la red troncal",             delay_t);
  cmd.Parse (argc, argv);
 
  NodeContainer acceso1, acceso2, troncal, wifi;
  troncal.Create(2);
  acceso1.Create(nodos_acceso_1);
  acceso2.Create(nodos_acceso_2);
  wifi.Create(nodos_wifi);

  NodeContainer total(acceso1, acceso2, troncal, wifi);
  // añadimos nodos compartis (routers)
  acceso1.Add(troncal.Get (0));
  acceso2.Add(troncal.Get (1));
  wifi.Add(troncal.Get(0));



  //Ptr<Node> Router1 = troncal.Get(0);  //Con esto podemos acceder a cada router
  //Ptr<Node> Router2 = troncal.Get(1);

  NS_LOG_INFO ("Creando Topologia");
  // Redes de acceso
  CsmaHelper csma_acceso1, csma_acceso2;
  csma_acceso1.SetChannelAttribute ("DataRate", DataRateValue (DataRate (data_rate_1)));
  csma_acceso1.SetChannelAttribute ("Delay", StringValue (delay_1));
  csma_acceso2.SetChannelAttribute ("DataRate", DataRateValue (DataRate (data_rate_2)));
  csma_acceso2.SetChannelAttribute ("Delay", StringValue (delay_2));
  // Wifi
  WifiHelper wifi_acceso1 = WifiHelper::Default ();
  NqosWifiMacHelper wifiMac_acceso1 = NqosWifiMacHelper::Default (); 
  wifiMac_acceso1.SetType ("ns3::AdhocWifiMac");
  YansWifiPhyHelper wifiPhy_acceso1 = YansWifiPhyHelper::Default ();
  YansWifiChannelHelper wifiChannel_acceso1 = YansWifiChannelHelper::Default (); 
  wifiPhy_acceso1.SetChannel (wifiChannel_acceso1.Create ());

  
  // Red troncal
  PointToPointHelper point;
  point.SetDeviceAttribute ("DataRate", DataRateValue (DataRate (data_rate_t)));
  point.SetChannelAttribute ("Delay", StringValue (delay_t)); 
  
  // We will use these NetDevice containers later, for IP addressing
  NetDeviceContainer ndacceso1 = csma_acceso1.Install (acceso1);  // Primera red de acceso
  NetDeviceContainer ndacceso2 = csma_acceso2.Install (acceso2);  // Segunda red de acceso
  NetDeviceContainer ndtroncal = point.Install (troncal);         // Red troncal
  NetDeviceContainer ndwifi = wifi_acceso1.Install (wifiPhy_acceso1, wifiMac_acceso1, wifi);

  NS_LOG_INFO ("Add IP Stack.");
  // InternetStackHelper internet_acceso1;
  // internet_acceso1.Install (acceso1);
  // InternetStackHelper internet_acceso2;
  // internet_acceso2.Install (acceso2);
  // InternetStackHelper internet_wifi;
  // internet_wifi.Install (wifi);
  InternetStackHelper().Install(total);

  NS_LOG_INFO ("Assign IP Addresses.");
  Ipv4AddressHelper ipv4Addr;
  Ipv4InterfaceContainer icacceso1, icacceso2, ictroncal, icwifi;
  ipv4Addr.SetBase ("10.1.1.0", "255.255.255.0");
  icacceso1 = ipv4Addr.Assign (ndacceso1);
  ipv4Addr.SetBase ("10.1.2.0", "255.255.255.0");
  icacceso2 = ipv4Addr.Assign (ndacceso2);
  ipv4Addr.SetBase ("10.1.3.0", "255.255.255.0");
  ictroncal = ipv4Addr.Assign (ndtroncal);
  ipv4Addr.SetBase ("10.1.4.0", "255.255.255.0");
  icwifi = ipv4Addr.Assign (ndwifi);


  // Popular tablas de routing
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  NS_LOG_INFO ("Installing static mobility; distance " << distance << " .");
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc =
    CreateObject<ListPositionAllocator>();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (0.0, distance, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.Install (wifi);
  
  // Sumidero
  uint16_t port = 8421;
  PacketSinkHelper sink ("ns3::UdpSocketFactory", Address (InetSocketAddress (Ipv4Address::GetAny(), port)));
  
  ApplicationContainer sinkC = sink.Install (acceso2.Get (1)); // Node n4
  sinkC.Start (Seconds (1.0));
  sinkC.Stop (Seconds (10.0));

  // Aplicacion OnOff
  OnOffHelper onoff ("ns3::UdpSocketFactory", Address (InetSocketAddress (icacceso2.GetAddress (1), port)));
  onoff.SetConstantRate (DataRate ("500kb/s"));

  ApplicationContainer app = onoff.Install (wifi.Get (0));
  app.Start (Seconds (1.0));
  app.Stop (Seconds (10.0));

  AsciiTraceHelper ascii;
  // csma.EnableAsciiAll (ascii.CreateFileStream ("csma-prueba.tr"));
  //csma_acceso1.EnablePcapAll ("csma-acceso1", false);
  csma_acceso2.EnablePcapAll ("csma-acceso2", false);

  NS_LOG_INFO ("Run Simulation");
  Simulator::Run();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done");
}

