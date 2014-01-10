#pragma once

#include "ns3/applications-module.h"

using namespace ns3;

class TransferenciaHelper : public BulkSendHelper
{
public:
  TransferenciaHelper (Ipv4Address address, uint16_t port);
  ~TransferenciaHelper ();

private:
  uint32_t maxBytes;

};
