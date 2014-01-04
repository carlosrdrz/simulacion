//#if 0
/*
  Fichero: trazas.h
  Librería de la clase Trazas
*/
using namespace ns3;
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <cassert>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/callback.h"
#include "ns3/internet-module.h"


class Trazas
{
public:

  Trazas ();

  virtual ~Trazas ();

  void Router0Recibe (Ptr<const Packet> paquete);

  int GetRouter0Recibe ();

  void Router0Envia (Ptr<const Packet> paquete);

  int GetRouter0Envia ();

  void Router1Recibe (Ptr<const Packet> paquete);

  int GetRouter1Recibe ();

  void Router1Envia (Ptr<const Packet> paquete);

  int GetRouter1Envia ();
  
  void ImprimeTrazas ();


private:
  //Variable que cuenta los paquetes cuyo envío ha finalizado.
  int envia_0;
  int recibe_0;
  int envia_1;
  int recibe_1;

};

//#endif
