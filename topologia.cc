//#if 0
/*
  Fichero para la clase Topologia
*/

#include "topologia.h"

using namespace ns3;

Topologia::~Topologia()
{
  std::cout << "Destructor topologia" << std::endl;
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
  subnets.clear();
  netdevices.clear();
  ips.clear();
  pcaps_helpers.clear();
}

void Topologia::AddContainer (std::string nombre, int numNode)
{
	NodeContainer *toadd = new NodeContainer();
  toadd->Create(numNode);
	nodes.Add(*toadd);
	subnets.insert(std::pair<std::string, NodeContainer*>(nombre, toadd));
}

NodeContainer* Topologia::GetNodeContainer (std::string nombre)
{
	return subnets.at(nombre);
}

void Topologia::BuildInternetStack ()
{
	InternetStackHelper().Install(nodes);
}

void Topologia::AddCsmaNetwork(std::string to, std::string data_rate, std::string delay)
{
	CsmaHelper *csma = new CsmaHelper();
	csma->SetChannelAttribute ("DataRate", DataRateValue (DataRate (data_rate)));
  csma->SetChannelAttribute ("Delay", StringValue (delay));
  Insert(to, csma->Install (*GetNodeContainer(to)));
  pcaps_helpers.insert(std::pair<std::string, PcapHelperForDevice*>(to, csma));
}

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

void Topologia::AddPPPNetwork (std::string to, std::string data_rate, std::string delay)
{
  PointToPointHelper *point = new PointToPointHelper();
  point->SetDeviceAttribute ("DataRate", DataRateValue (DataRate (data_rate)));
  point->SetChannelAttribute ("Delay", StringValue (delay));
  Insert(to, point->Install (*GetNodeContainer(to)));
  pcaps_helpers.insert(std::pair<std::string, PcapHelperForDevice*>(to, point));
}

NetDeviceContainer* Topologia::GetNetDeviceContainer (std::string nombre)
{
  return netdevices.at(nombre);
}

void Topologia::AddMobility (std::string to, double distance)
{
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
  positionAlloc->Add (Vector (0.0, 0.0, 0.0));
  positionAlloc->Add (Vector (0.0, distance, 0.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.Install (*GetNodeContainer(to));
}

void Topologia::SetIpToNetwork (std::string to, std::string base, std::string mask)
{
  Ipv4AddressHelper ipv4Addr;
  ipv4Addr.SetBase (base.c_str(), mask.c_str());
  Insert(to, ipv4Addr.Assign (*GetNetDeviceContainer(to)));
}

Ipv4InterfaceContainer* Topologia::GetInterfaceContainer (std::string nombre)
{
  return ips.at(nombre);
}

void Topologia::SetErrorModel (std::string to, float tasa)
{
  // Modelo de errores
  Ptr<RateErrorModel> error = CreateObject<RateErrorModel> ();
  Ptr<UniformRandomVariable> uv = CreateObject<UniformRandomVariable> ();
  error->SetRandomVariable (uv);
  error->SetUnit(RateErrorModel::ERROR_UNIT_PACKET);
  error->SetRate(tasa);

  NetDeviceContainer *container = GetNetDeviceContainer(to); 

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

void Topologia::EnablePCAPLogging (std::string to)
{
  PcapHelperForDevice* helper = pcaps_helpers.at(to);
  helper->EnablePcapAll (to, false);
}

void Topologia::Insert (std::string to, const NetDeviceContainer &nd)
{
  NetDeviceContainer *pnd = new NetDeviceContainer();
  pnd->Add(nd);
  netdevices.insert(std::pair<std::string, NetDeviceContainer*>(to, pnd));
}

void Topologia::Insert (std::string to, const Ipv4InterfaceContainer &nd)
{
  Ipv4InterfaceContainer *pnd = new Ipv4InterfaceContainer();
  pnd->Add(nd);
  ips.insert(std::pair<std::string, Ipv4InterfaceContainer*>(to, pnd));
}
//#endif