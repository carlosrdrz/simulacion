//#if 0
/*
  Fichero para la clase Trazas
  A partir de esta clase se obtendrán las trazas 
  del envío de paquetes.
*/

#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "trazas.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Trazas");


Trazas::Trazas()
  :  envia_0(0),
     recibe_0(0),
     envia_1(0),
     recibe_1(0)
{
}

Trazas::~Trazas()
{
}


//Métodos para el nodo dorsal de la izquierda 0
//Método llamado en la traza de recepción que aumenta un contador
void
Trazas::Router0Recibe(Ptr<const Packet> paquete)
{
  NS_LOG_FUNCTION(this);
  recibe_0++;
}
//Método que devuelve el contador de paquetes recibidos
int
Trazas::GetRouter0Recibe()
{
  return recibe_0;
}
//Método llamado en la traza de envio que aumenta un contador
void
Trazas::Router0Envia(Ptr<const Packet> paquete)
{
  envia_0++;
}
//Método que devuelve el contador de paquetes enviados
int
Trazas::GetRouter0Envia()
{
  return envia_0;
}
//Métodos para el nodo dorsal de la derecha 1
//Métodos llamado en la traza de recepción que aumenta un contador
void
Trazas::Router1Recibe(Ptr<const Packet> paquete)
{
  recibe_1++;
}
//Método que devuelve el contador de paquetes recibidos. 
int
Trazas::GetRouter1Recibe()
{
  return recibe_1;
}
//Métodos llamado en la traza de envio que aumenta un contador
void
Trazas::Router1Envia(Ptr<const Packet> paquete)
{
  envia_1++;
}
//Método que devuelve el contador de envío.
int
Trazas::GetRouter1Envia()
{
  return envia_1;
}
void
Trazas::ImprimeTrazas()
{
  std::cout<<"Trazas obtenidas: "<<std::endl;
  std::cout<<"Paquetes enviados por el router 0: "<<envia_0<<std::endl;
  std::cout<<"Paquetes recibidos por el router 0: "<<recibe_0<<std::endl;
  std::cout<<"Paquetes enviados por el router 1: "<<envia_1<<std::endl;
  std::cout<<"Paquetes recibidos por el router 1: "<<recibe_1<<std::endl;
}

//#endif
