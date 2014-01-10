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

using namespace std;

class Trazas
{

public:

  ~Trazas ();

  void Monitorize (std::string id, Ptr<NetDevice> r);

  int GetPaquetesEnviados (std::string id)const; //throw(std::out_of_range)

  int GetPaquetesRecibidos (std::string id)const; //throw(std::out_of_range)

  int GetBytesEnviados (std::string id)const; //throw(std::out_of_range)

  int GetBytesRecibidos (std::string id)const; //throw(std::out_of_range)

  void DispositivoRecibe (std::string id, Ptr<const Packet> paquete);

  void DispositivoEnvia (std::string id, Ptr<const Packet> paquete);
  
  void ImprimeTrazas ();

private:

  class Datos; /* FW */

  std::map<std::string, Datos*> valores;

  class Datos{
  public:
    int paquetesEnviados;
    int paquetesRecibidos;
    int bytesEnviados;
    int bytesRecibidos;

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

};

//#endif
