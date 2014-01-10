
/*
  Fichero: principal.cc
  Descripción: aquí se implementará el main
  y la topología.
*/

#include <iostream>
#include "trazas.h"
#include "topologia.h"
#include "navegador.h"
#include "transferencia.h"
#include "voip.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Principal");

int
main ( int argc, char * argv[])
{
  /////// LOG //////////////////////
  LogComponentEnable("Trazas", LOG_LEVEL_INFO);
  LogComponentEnable("Topologia", LOG_LEVEL_INFO);
  LogComponentEnable("Principal", LOG_LEVEL_INFO);
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
  traza.Monitorize ("r0", topologia.GetNetDeviceContainer("troncal")->Get (0));
  traza.Monitorize ("r1", topologia.GetNetDeviceContainer("troncal")->Get (1));

  // Añadimos los modelos de errores
  topologia.SetErrorModel("troncal", 0.1);

  // Comenzamos a añadir aplicaciones...
  // Sumideros
  PacketSinkHelper sinkTcp ("ns3::TcpSocketFactory", Address (InetSocketAddress (Ipv4Address::GetAny(), sink_port)));
  PacketSinkHelper sinkUdp ("ns3::UdpSocketFactory", Address (InetSocketAddress (Ipv4Address::GetAny(), sink_port)));
  
  ApplicationContainer sink1 = sinkTcp.Install (topologia.GetNodeContainer("acceso2")->Get (0));
  sink1.Start (Seconds (1.0));
  sink1.Stop (Seconds (10.0));

  ApplicationContainer sink2 = sinkUdp.Install (topologia.GetNodeContainer("acceso2")->Get (0)); 
  sink2.Start (Seconds (1.0));
  sink2.Stop (Seconds (10.0));

  // Navegador
  NavegadorHelper chrome (topologia.GetInterfaceContainer("acceso2")->GetAddress (1), sink_port);
  // Se instala la aplicación navegador
  ApplicationContainer navegador = chrome.Install (topologia.GetNodeContainer("acceso1")->Get (0));
  navegador.Start (Seconds (1.0));
  navegador.Stop (Seconds (10.0));
  
  // Telefono IP
  VoipHelper ciscoPhone (topologia.GetInterfaceContainer("acceso2")->GetAddress (0), sink_port);
  ApplicationContainer app_voip = ciscoPhone.Install (topologia.GetNodeContainer("acceso1")->Get (0));
  // Se instala la aplicacion Voip
  app_voip.Start (Seconds (1.0));
  app_voip.Stop (Seconds (10.0));
   
  // Transferencia fichero
  TransferenciaHelper ftp (topologia.GetInterfaceContainer("acceso2")->GetAddress (1), sink_port);
  // Se instala la aplicación transferencia
  ApplicationContainer transferencia = ftp.Install (topologia.GetNodeContainer("acceso1")->Get (0));
  transferencia.Start (Seconds(1.0));
  transferencia.Stop (Seconds(10.0));

  // Activamos la creacion de archivos PCAPs
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
