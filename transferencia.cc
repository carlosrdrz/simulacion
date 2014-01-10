
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