//#if 0
/*
  Fichero: topologia.h
  Librería de la clase Topologia
*/

#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/wifi-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/mobility-module.h"

class Topologia
{
public:
	~Topologia();

	void AddContainer (std::string nombre, int numNode);
	ns3::NodeContainer* GetNodeContainer (std::string nombre);

	void BuildInternetStack ();

	void AddCsmaNetwork (std::string to, std::string data_rate, std::string delay);
	void AddWifiNetwork (std::string to);
	void AddPPPNetwork (std::string to, std::string data_rate, std::string delay);
	ns3::NetDeviceContainer* GetNetDeviceContainer (std::string nombre);

	void AddMobility (std::string to, double distance);

	void SetIpToNetwork (std::string to, std::string base, std::string mask);
	ns3::Ipv4InterfaceContainer* GetInterfaceContainer (std::string nombre);

	void SetErrorModel (std::string to, float tasa);

	void EnablePCAPLogging (std::string to);

private:
	void Insert (std::string to, const ns3::NetDeviceContainer &nd);
	void Insert (std::string to, const ns3::Ipv4InterfaceContainer &nd);

  ns3::NodeContainer nodes;
  std::map<std::string, ns3::NodeContainer*> subnets;
  std::map<std::string, ns3::NetDeviceContainer*> netdevices;
  std::map<std::string, ns3::Ipv4InterfaceContainer*> ips;
  std::map<std::string, ns3::PcapHelperForDevice*> pcaps_helpers;
};

//#endif