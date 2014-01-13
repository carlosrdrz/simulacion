
#include "transferencia.h"

using namespace ns3;

TransferenciaHelper::TransferenciaHelper(Ipv4Address address, uint16_t port)
  : BulkSendHelper("ns3::TcpSocketFactory", Address(InetSocketAddress (address,port)))
{
  maxBytes = 0;
  SetAttribute("MaxBytes", UintegerValue(maxBytes));
}

TransferenciaHelper::~TransferenciaHelper()
{

}