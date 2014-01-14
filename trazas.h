/*
  Fichero: trazas.h
  Librería de la clase Trazas
*/

#pragma once

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

#define OCTETO 8 
#define POR_CIENTO 100
#define VACIO 0

using namespace std;
using namespace ns3;

class Trazas
{

public:

  ~Trazas ();

  void Monitorize (std::string id, Ptr<NetDevice> r);

  int GetPaquetesEnviados (std::string id)const; //throw(std::out_of_range)

  int GetPaquetesRecibidos (std::string id)const; //throw(std::out_of_range)
  
  int GetPaquetesPerdidos (std::string id)const; //throw(std::out_of_range)

  int GetBytesEnviados (std::string id)const; //throw(std::out_of_range)

  int GetBytesRecibidos (std::string id)const; //throw(std::out_of_range)
  
  int GetBytesPerdidos (std::string id)const; //throw(std::out_of_range)

  void DispositivoRecibe (std::string id, Ptr<const Packet> paquete); //throw(std::out_of_range)

  void DispositivoEnvia (std::string id, Ptr<const Packet> paquete); //throw(std::out_of_range)

  void DispositivoPierde (std::string id, Ptr<const Packet> paquete); //throw(std::out_of_range)
  
  void ImprimeTrazas ();

  double ImprimeTrazas(double tasa, double tiempo_total);

private:

  //double necesarios para obtener el rendimiento
  double uso_enlace;
  double total_bytes_recibidos;
  double total_bytes_perdidos;
  double total_bytes_enviados;

  class Datos; /* FW */

  std::map<std::string, Datos*> valores;

  class Datos{
  public:
    int paquetesEnviados;
    int paquetesRecibidos;
    int paquetesPerdidos;
    int bytesEnviados;
    int bytesRecibidos;
    int bytesPerdidos;

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
    
    //Método llamado en la traza de paquetes perdidos que aumenta un cont.
    void DispositivoPierde(Ptr<const Packet> paquete)
    {
      paquetesPerdidos++;
      bytesPerdidos += paquete->GetSize();
    }
  };

};

