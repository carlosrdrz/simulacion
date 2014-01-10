//#if 0
/*
  Fichero: trazas.h
  Librería de la clase Trazas
*/

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
#include "ns3/applications-module.h"

using namespace std;
using namespace ns3;

class Trazas
{

public:

  ~Trazas ();

  void Monitorize (int id, Ptr<NetDevice> r);

  int GetPaquetesEnviados (int id);

  int GetPaquetesRecibidos (int id);

  float GetBytesEnviados (int id);

  float GetBytesRecibidos (int id);

  void DispositivoRecibe (int id, Ptr<const Packet> paquete);

  void DispositivoEnvia (int id, Ptr<const Packet> paquete);
  
  void ImprimeTrazas ();

private:

  class Datos {
  public:
    int paquetesEnviados;
    int paquetesRecibidos;
    float bytesEnviados;
    float bytesRecibidos;

    // Método llamado en la traza de envío que aumenta un contador
    void DispositivoEnvia(Ptr<const Packet> paquete)
    {
      paquetesEnviados++;
      bytesEnviados += paquete->GetSize();
    }

    // Método llamado en la traza de recepción que aumenta un contador
    void DispositivoRecibe(Ptr<const Packet> paquete)
    {
      paquetesRecibidos++;
      bytesRecibidos += paquete->GetSize();
    }
  };

  std::map<int, Datos*> valores;

};

//#endif
