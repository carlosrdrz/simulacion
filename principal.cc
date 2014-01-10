
/*
  Fichero: principal.cc
  Descripción: aquí se implementará el main
  y la topología.
*/

#include <iostream>
#include "trazas.h"
#include "topologia.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("CsmaMulticastExample");

int
main ( int argc, char * argv[])
{
  /////// LOG //////////////////////
  LogComponentEnable("Trazas", LOG_LEVEL_INFO);
  LogComponentEnable("CsmaMulticastExample", LOG_LEVEL_INFO);
  // LogComponentEnable("Trazas", LOG_LEVEL_ALL);

  Config::SetDefault ("ns3::CsmaNetDevice::EncapsulationMode", StringValue ("Dix"));

  bool tracing              = true;
  unsigned nodos_acceso_1   = 2;
  unsigned nodos_acceso_2   = 2;
  unsigned nodos_wifi       = 1;
  double distance           = 50.0;
  std::string data_rate_1   = "5Mbps";
  std::string data_rate_2   = "5Mbps";
  std::string data_rate_t   = "5Mbps";
  std::string delay_1       = "0.002";
  std::string delay_2       = "0.002";
  std::string delay_t       = "0.002";

  // Puertos
  uint16_t sink_port = 8421;

  CommandLine cmd;
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

  // Variables del sistema
  Trazas traza;
  Topologia topologia;

  // Añadimos los contenedores de nodos
  NS_LOG_INFO ("Creando Topologia");
  topologia.AddContainer ("troncal", 2);
  topologia.AddContainer ("acceso1", nodos_acceso_1);
  topologia.AddContainer ("acceso2", nodos_acceso_2);
  topologia.AddContainer ("wifi", nodos_wifi);

  // Añadimos los routers para formar las subredes
  topologia.GetNodeContainer ("acceso1")->Add (topologia.GetNodeContainer ("troncal")->Get (0));
  topologia.GetNodeContainer ("acceso2")->Add (topologia.GetNodeContainer ("troncal")->Get (1));
  topologia.GetNodeContainer ("wifi")->Add (topologia.GetNodeContainer ("troncal")->Get (0));

  // Creamos las redes CSMA
  topologia.AddCsmaNetwork("acceso1", data_rate_1, delay_1);
  topologia.AddCsmaNetwork("acceso2", data_rate_2, delay_2);

  // Y las Wifi
  topologia.AddWifiNetwork("wifi");

  // Y el PPP troncal
  topologia.AddPPPNetwork("troncal", data_rate_t, delay_t);

  NS_LOG_INFO ("Añadimos el stack IP");
  topologia.BuildInternetStack ();

  NS_LOG_INFO ("Asignamos direcciones IP.");
  topologia.SetIpToNetwork ("acceso1", "10.1.1.0", "255.255.255.0");
  topologia.SetIpToNetwork ("acceso2", "10.1.2.0", "255.255.255.0");
  topologia.SetIpToNetwork ("troncal", "10.1.3.0", "255.255.255.0");
  topologia.SetIpToNetwork ("wifi", "10.1.4.0", "255.255.255.0");

  // Popular tablas de routing
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  // Añadimos el sistema de mobilidad a la red wifi
  topologia.AddMobility ("wifi", distance);
  
  // Añadimos las trazas a los routers de la troncal
  NS_LOG_INFO("Monitorizamos los dos routers");
  traza.Monitorize (0, topologia.GetNetDeviceContainer("troncal")->Get (0));
  traza.Monitorize (1, topologia.GetNetDeviceContainer("troncal")->Get (1));

  // Añadimos los modelos de errores
  topologia.SetErrorModel("troncal", 0.1);

  // Comenzamos a añadir aplicaciones...
  // Sumidero
  PacketSinkHelper sink ("ns3::UdpSocketFactory", Address (InetSocketAddress (Ipv4Address::GetAny(), sink_port)));
  
  // Se instala la aplicación sink
  ApplicationContainer sinkC = sink.Install (topologia.GetNodeContainer("acceso2")->Get (1));
  sinkC.Start (Seconds (1.0));
  sinkC.Stop (Seconds (10.0));

  // Aplicacion OnOff
  OnOffHelper onoff ("ns3::UdpSocketFactory", Address (InetSocketAddress (topologia.GetInterfaceContainer("acceso2")->GetAddress (1), sink_port)));
  onoff.SetConstantRate (DataRate ("500kb/s"));
  // onoff.SetAttribute("OnTime", PointerValue(varon));
  // onoff.SetAttribute("OffTime", PointerValue(varoff));

  // Se instala la aplicación onoff
  ApplicationContainer app = onoff.Install (topologia.GetNodeContainer("wifi")->Get (0));
  app.Start (Seconds (1.0));
  app.Stop (Seconds (10.0));

  // Habilitar PCAPs
  if(tracing) {
    topologia.EnablePCAPLogging ("acceso1");
    topologia.EnablePCAPLogging ("acceso2");
  }

  NS_LOG_INFO ("Ejecutando simulacion...");
  Simulator::Run();

  // Imprimimos todas las trazas monitorizadas
  traza.ImprimeTrazas();

  Simulator::Destroy ();
  NS_LOG_INFO ("Done");
}