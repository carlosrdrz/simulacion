//#if 0
/*
  Fichero para la clase Topologia
*/

#include "topologia.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Topologia");

// Libera la memoria utilizada
Topologia::~Topologia()
{
  NS_LOG_FUNCTION(this);

  // Esto itera por todos los elementos de todos los maps
  // liberando la memoria que habiamos ocupado usando new
  for (std::map<std::string, NodeContainer*>::iterator it = subnets.begin (); it != subnets.end (); ++it) {
    delete it->second;
  }
  for (std::map<std::string, NetDeviceContainer*>::iterator it = netdevices.begin (); it != netdevices.end (); ++it) {
    delete it->second;
  }
  for (std::map<std::string, Ipv4InterfaceContainer*>::iterator it = ips.begin (); it != ips.end (); ++it) {
    delete it->second;
  }
  for (std::map<std::string, PcapHelperForDevice*>::iterator it = pcaps_helpers.begin (); it != pcaps_helpers.end (); ++it) {
    delete it->second;
  }

  // Limpiamos todos los maps
  subnets.clear();
  netdevices.clear();
  ips.clear();
  pcaps_helpers.clear();
}

// Crea un node container con un nombre identificativo y un numero de nodos
void Topologia::AddContainer (std::string nombre, int numNode)
{
  // Creamos un node container con los elementos necesarios
	NodeContainer *toadd = new NodeContainer();
  toadd->Create(numNode);
  // Añadimos esos nodos al container con todos los nodos de la red
	nodes.Add(*toadd);
  // Insertamos el node container creado en el map con su identificador
	subnets.insert(std::pair<std::string, NodeContainer*>(nombre, toadd));
}

// Devuelve el NodeContainer correspondiente a ese nombre
NodeContainer* Topologia::GetNodeContainer (std::string nombre)
{
	return subnets.at(nombre);
}

// Añade un node de un node container a otro node container
void Topologia::AddNodeToContainer (std::string origen, int numNode, std::string destino)
{
  GetNodeContainer (destino)->Add (GetNodeContainer (origen)->Get (numNode));
}

// Obtiene un nodo de un node container a partir de un identificador
// de node container y de la posicion del nodo en dicho container
Ptr<Node> Topologia::GetNode (std::string from, int numNode)
{
  return GetNodeContainer (from)->Get (numNode);
}

// Agrega el stack de internet a todos los nodos
void Topologia::BuildInternetStack ()
{
	InternetStackHelper().Install(nodes);
}

// Crea una red CSMA a partir de un node container.
// Guarda en un map el NetDeviceContainer de esa red y el helper (para luego obtener PCAPs)
void Topologia::AddCsmaNetwork(std::string to, std::string data_rate, std::string delay)
{
	CsmaHelper *csma = new CsmaHelper();
	csma->SetChannelAttribute ("DataRate", DataRateValue (DataRate (data_rate)));
  csma->SetChannelAttribute ("Delay", StringValue (delay));
  Insert(to, csma->Install (*GetNodeContainer(to)));
  pcaps_helpers.insert(std::pair<std::string, PcapHelperForDevice*>(to, csma));
}

// Crea una red Wifi a partir de un node container
void Topologia::AddWifiNetwork (std::string to)
{
  WifiHelper wifi = WifiHelper::Default ();
  NqosWifiMacHelper wifiMac = NqosWifiMacHelper::Default (); 
  wifiMac.SetType ("ns3::AdhocWifiMac");
  YansWifiPhyHelper wifiPhy = YansWifiPhyHelper::Default ();
  YansWifiChannelHelper wifiChannel = YansWifiChannelHelper::Default (); 
  wifiPhy.SetChannel (wifiChannel.Create ());
  Insert(to, wifi.Install (wifiPhy, wifiMac, *GetNodeContainer(to)));
}

// Crea una red point to point a partir de un NodeContainer
// Guarda en un map el NetDeviceContainer de esa red y el helper (para luego obtener PCAPs)
void Topologia::AddPPPNetwork (std::string to, std::string data_rate, std::string delay)
{
  PointToPointHelper *point = new PointToPointHelper();
  point->SetDeviceAttribute ("DataRate", DataRateValue (DataRate (data_rate)));
  point->SetChannelAttribute ("Delay", StringValue (delay));
  Insert(to, point->Install (*GetNodeContainer(to)));
  pcaps_helpers.insert(std::pair<std::string, PcapHelperForDevice*>(to, point));
}

// Obtiene un NetDeviceContainer a partir de su identificador, que es el
// mismo que el del NodeContainer desde el que fue creado
NetDeviceContainer* Topologia::GetNetDeviceContainer (std::string nombre)
{
  return netdevices.at(nombre);
}

// Obtiene un NetDevice a partir del identificador de NetDeviceContainer
// y su posicion dentro de ese contenedor
Ptr<NetDevice> Topologia::GetNetDevice (std::string from, int numNode)
{
  return GetNetDeviceContainer(from)->Get (numNode);
}

// Añade movilidad a una red wifi ya existente
void Topologia::AddMobility (std::string to, double distance)
{
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (0.0, distance, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.Install (*GetNodeContainer(to));
}

// Añade las direcciones IP a una red ya existente
void Topologia::SetIpToNetwork (std::string to, std::string base, std::string mask)
{
  Ipv4AddressHelper ipv4Addr;
  ipv4Addr.SetBase (base.c_str(), mask.c_str());
  Insert(to, ipv4Addr.Assign (*GetNetDeviceContainer(to)));
}

// Obtiene un contenedor de IPs a partir de su id
Ipv4InterfaceContainer* Topologia::GetInterfaceContainer (std::string nombre)
{
  return ips.at(nombre);
}

// Obtiene una direccion IP a partir del id de su contenedor
// y de su posicion en ese contenedor
Ipv4Address Topologia::GetIPv4Address (std::string from, int num)
{
  return GetInterfaceContainer(from)->GetAddress (num);
}

// Añade un modelo de errores uniforme a una red
void Topologia::SetErrorModel (std::string to, float tasa)
{
  // Modelo de errores
  Ptr<RateErrorModel> error = CreateObject<RateErrorModel> ();
  Ptr<UniformRandomVariable> uv = CreateObject<UniformRandomVariable> ();
  error->SetRandomVariable (uv);
  error->SetUnit(RateErrorModel::ERROR_UNIT_PACKET);
  error->SetRate(tasa);

  // Obtenemos el contenedor de red
  NetDeviceContainer *container = GetNetDeviceContainer(to); 

  // Aplicamos en todos los nodos
  for (NetDeviceContainer::Iterator it = container->Begin (); it != container->End (); ++it) {
    Ptr<PointToPointNetDevice> pppnd = (*it)->GetObject<PointToPointNetDevice>();
    Ptr<CsmaNetDevice> csmand = (*it)->GetObject<CsmaNetDevice>();
    if (pppnd) {
      pppnd->SetReceiveErrorModel(error);
    } else if (csmand) {
      csmand->SetReceiveErrorModel(error);
    }
  }
}

// Habilitamos el registro mediante PCAP
void Topologia::EnablePCAPLogging (std::string to)
{
  PcapHelperForDevice* helper = pcaps_helpers.at(to);
  helper->EnablePcapAll (to, false);
}

// Insertamos un NetDeviceContainer en el map correspondiente
void Topologia::Insert (std::string to, const NetDeviceContainer &nd)
{
  NetDeviceContainer *pnd = new NetDeviceContainer();
  pnd->Add(nd);
  netdevices.insert(std::pair<std::string, NetDeviceContainer*>(to, pnd));
}

// Insertamos un Ipv4InterfaceContainer en el map correspondiente
void Topologia::Insert (std::string to, const Ipv4InterfaceContainer &nd)
{
  Ipv4InterfaceContainer *pnd = new Ipv4InterfaceContainer();
  pnd->Add(nd);
  ips.insert(std::pair<std::string, Ipv4InterfaceContainer*>(to, pnd));
}
//#endif