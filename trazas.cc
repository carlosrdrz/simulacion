#if 0
/*
  Fichero para la clase Trazas
  A partir de esta clase se obtendrán las trazas 
  del envío de paquetes.
*/

#include "ns3/network-module.h"
#include "trazas.h"
NS_LOG_COMPONENT_DEFINE ("Trazas");


Trazas::Trazas()
  :  envios_finalizados(0)
{
}

Trazas::~Trazas()
{
}


//Métodos para el nodo dorsal de la izquierda L
//Método llamado en la traza de recepción que aumenta un contador
void
Trazas::NodoLRecibe(Ptr<const Packet> paquete)
{
  recibe_L++;
}
void
//Método que devuelve el contador de paquetes recibidos
int
Trazas::GetNodoLRecibe()
{
  return recibe_L;
}
//Método llamado en la traza de envio que aumenta un contador
void
Trazas::NodoLEnvia(Ptr<const Packet> paquete)
{
  envia_L++;
}
//Método que devuelve el contador de paquetes enviados
int
Trazas::GetNodoLEnvia()
{
  return envia_L;
}
//Métodos para el nodo dorsal de la derecha R
//Métodos llamado en la traza de recepción que aumenta un contador
void
Trazas::NodoRRecibe(Ptr<const Packet> paquete)
{
  recibe_R++;
}
//Método que devuelve el contador de paquetes recibidos. 
int
Trazas::GetNodoRRecibe()
{
  return recibe_R;
}
//Métodos llamado en la traza de envio que aumenta un contador
void
Trazas::NodoREnvia(Ptr<const Packet> paquete)
{
  envia_R++;
}
//Método que devuelve el contador de envío.
int
Trazas::GetNodoREnvia()
{
  return envia_R;
}


#endif
