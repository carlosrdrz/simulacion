
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
#include "servidor.h"

#define TSTUDENT 1.8331               //10 Simulaciones con 90% de intervalo de confianza
#define NUM_SIMULACIONES 10
#define TASA 5000000
#define T_INICIO 1
#define T_FINAL 20
#define HTTP_PORT 80
#define FTP_PORT 20
#define VOIP_PORT 4569

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Principal");

int
main ( int argc, char * argv[])
{
  LogComponentEnable("Trazas", LOG_LEVEL_INFO);
  LogComponentEnable("Topologia", LOG_LEVEL_INFO);
  LogComponentEnable("Principal", LOG_LEVEL_INFO);
  LogComponentEnable("Servidor", LOG_LEVEL_INFO);

  Config::SetDefault ("ns3::CsmaNetDevice::EncapsulationMode", StringValue ("Dix"));

  bool tracing              = true;
  unsigned nodos_acceso     = 5;
  unsigned nodos_empresa    = 3;
  unsigned nodos_wifi       = 1;
  double distance           = 50.0;
  std::string data_rate_1   = "100Mbps";
  std::string data_rate_2   = "100Mbps";
  std::string data_rate_t   = "1Gbps";
  std::string delay_1       = "0.002";
  std::string delay_2       = "0.002";
  std::string delay_t       = "0.002";
  double tasa_errores        = 0.02;        
  int indice                = 0;   
  double tasa               = TASA;
  double tiempo             = T_FINAL-T_INICIO;
  double uso_enlace         = 0;      //Porcentaje de uso del enlace que devolverá el método ImprimeTrazas
  Average<double> acumulador_uso;  
  double intervalo          = 0;
  //Nombre BASE del fichero de datos de salida
  int numero_fichero        = 1;
  std::string nombre_archivo_datos = "trabajo_final";
  
  CommandLine cmd;
  cmd.AddValue("NumeroNodosAcceso",  "Número de nodos en la red de acceso 1",   nodos_acceso);
  cmd.AddValue("NumeroNodosEmpresa", "Número de nodos en la red de la empresa", nodos_empresa);
  cmd.AddValue("NumeroNodosWifi",    "Número de nodos que usan wifi",           nodos_wifi);
  cmd.AddValue("DataRate1",          "Capacidad de la red de acceso 1",         data_rate_1);
  cmd.AddValue("DataRate2",          "Capacidad de la red de acceso 2",         data_rate_2);
  cmd.AddValue("DataRatet",          "Capacidad de la red troncal",             data_rate_t);
  cmd.AddValue("Delay1",             "Retardo de la red de acceso 1",           delay_1);
  cmd.AddValue("Delay2",             "Retardo de la red de acceso 2",           delay_2);
  cmd.AddValue("Delayt",             "Retardo de la red troncal",               delay_t);
  cmd.Parse (argc, argv);

for(nodos_acceso = 3; nodos_acceso <= 15; nodos_acceso+=3)
 {
  nodos_wifi = nodos_acceso;
  NS_LOG_INFO("El número de usuarios que acceden es: " << nodos_acceso << " por CSMA, y " << nodos_wifi << " por wifi");
  for(tasa_errores = 0.1; tasa_errores <= 0.1; tasa_errores += 0.2)
  {
    NS_LOG_INFO("LA TASA DE ERRORES ES: " << tasa_errores);
    //Nombre variable del fichero de datos
    std::stringstream stream;
    std::string num;
    stream << numero_fichero++;
    num = stream.str();
    //Nombre completo del fichero de salida
    std::string nombreFichero = nombre_archivo_datos + num + ".dat";
    std::ofstream fichero (nombreFichero.c_str(), std::ios_base::app);
    acumulador_uso.Reset ();
    for (indice = 0; indice <= 0; indice++)
    {
      // Variables del sistema
      Trazas traza;
      Topologia topologia;

      // Añadimos los contenedores de nodos
      NS_LOG_INFO ("Creando Topologia");
      topologia.AddContainer ("troncal", 2);
      topologia.AddContainer ("acceso", nodos_acceso);
      topologia.AddContainer ("empresa", nodos_empresa);
      topologia.AddContainer ("wifi", nodos_wifi);

      // Añadimos los routers para formar las subredes
      topologia.AddNodeToContainer("troncal", 0, "acceso");
      topologia.AddNodeToContainer("troncal", 1, "empresa");
      topologia.AddNodeToContainer("troncal", 0, "wifi");

      // Creamos las redes PointToPoint, CSMA y Wifi
      topologia.AddPPPNetwork("troncal", data_rate_t, delay_t);
      topologia.AddCsmaNetwork("acceso", data_rate_1, delay_1);
      topologia.AddCsmaNetwork("empresa", data_rate_2, delay_2);
      topologia.AddWifiNetwork("wifi");

      NS_LOG_INFO ("Añadimos el stack IP");
      topologia.BuildInternetStack ();

      NS_LOG_INFO ("Asignamos direcciones IP.");
      topologia.SetIpToNetwork ("acceso", "10.1.1.0", "255.255.255.0");
      topologia.SetIpToNetwork ("empresa", "10.1.2.0", "255.255.255.0");
      topologia.SetIpToNetwork ("troncal", "10.1.3.0", "255.255.255.0");
      topologia.SetIpToNetwork ("wifi", "10.1.4.0", "255.255.255.0");

      // Popular tablas de routing
      Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

      // Añadimos el sistema de mobilidad a la red wifi
      topologia.AddMobility ("wifi", distance);
  
      // Añadimos las trazas a los routers de la troncal
      NS_LOG_INFO("Monitorizamos los dos routers");
      traza.Monitorize ("r0", topologia.GetNetDevice("troncal", 0));
      traza.Monitorize ("r1", topologia.GetNetDevice("troncal", 1));

      // Añadimos los modelos de errores
      topologia.SetErrorModel("troncal", tasa_errores);

      // Comenzamos a añadir aplicaciones...
      // Sumideros
      PacketSinkHelper sinkUdp ("ns3::UdpSocketFactory", Address (InetSocketAddress (Ipv4Address::GetAny(), VOIP_PORT)));
      PacketSinkHelper sinkFtp ("ns3::TcpSocketFactory", Address (InetSocketAddress (Ipv4Address::GetAny(), FTP_PORT)));

      Servidor servidor (topologia.GetNode("empresa", 2));  //Aplicación servidor HTTP

      ApplicationContainer sink2 = sinkUdp.Install (topologia.GetNode("empresa", 1)); 
      sink2.Start (Seconds (T_INICIO));
      sink2.Stop (Seconds (T_FINAL));

      ApplicationContainer sink3 = sinkFtp.Install (topologia.GetNode("empresa", 0));
      sink3.Start (Seconds (T_INICIO));
      sink3.Stop (Seconds (T_FINAL));

      // Navegador
      NavegadorHelper chrome (topologia.GetIPv4Address("empresa", 2), HTTP_PORT);
      // Se instala la aplicación navegador
      for (unsigned acceso_indice = 0; acceso_indice < nodos_acceso; acceso_indice++) {
        ApplicationContainer navegador = chrome.Install (topologia.GetNode("acceso", acceso_indice));
        navegador.Start (Seconds (T_INICIO));
        navegador.Stop (Seconds (T_FINAL));
      }

      // Navegador wifi
      NavegadorHelper safari (topologia.GetIPv4Address("empresa", 2), HTTP_PORT);
      // Se instala la aplicación navegador en un dispositivo con wifi
      for (unsigned wifi_indice = 0; wifi_indice < nodos_wifi; wifi_indice++) {
        ApplicationContainer navegador_wifi = safari.Install (topologia.GetNode("wifi", wifi_indice));
        navegador_wifi.Start (Seconds (T_INICIO));
        navegador_wifi.Stop (Seconds (T_FINAL));
      }
        
      // Telefono IP
      VoipHelper ciscoPhone (topologia.GetIPv4Address("empresa", 1), VOIP_PORT);
      ApplicationContainer app_voip = ciscoPhone.Install (*(topologia.GetNodeContainer ("acceso")));
      // Se instala la aplicacion Voip
      app_voip.Start (Seconds (T_INICIO));
      app_voip.Stop (Seconds (T_FINAL));
   
      // //Transferencia fichero
      TransferenciaHelper ftp (topologia.GetIPv4Address("empresa", 0), FTP_PORT);
      // Se instala la aplicación transferencia
      ApplicationContainer transferencia = ftp.Install (*(topologia.GetNodeContainer ("acceso")));
      transferencia.Start (Seconds(T_INICIO));
      transferencia.Stop (Seconds(T_FINAL));

      // Activamos la creacion de archivos PCAPs
      if(tracing) {
        topologia.EnablePCAPLogging ("acceso");
        topologia.EnablePCAPLogging ("empresa");
        topologia.EnablePCAPLogging ("troncal");
      }

      NS_LOG_INFO ("Ejecutando simulacion...");
      Simulator::Stop(Seconds(T_FINAL));
      Simulator::Run();
      Simulator::Destroy ();
      NS_LOG_INFO ("Done");

      // Imprimimos todas las trazas monitorizadas
      uso_enlace = traza.ImprimeTrazas(tasa,tiempo);
      acumulador_uso.Update(uso_enlace);
      NS_LOG_INFO("El uso del enlace es: " << uso_enlace << "%");
    }
      intervalo = sqrt(acumulador_uso.Var() / NUM_SIMULACIONES) * TSTUDENT;
      NS_LOG_INFO ("El intervalo de confianza es: " << intervalo);
      fichero << tasa_errores << " " << acumulador_uso.Mean() << " " << intervalo << std::endl;
  }
 }
 return 0;
}
