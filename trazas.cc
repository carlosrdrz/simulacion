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


/*
Evento llamado al iniciar la traza
*/
void
Trazas::EventoEnviarFin(Ptr<const Packet> paquete)
{
  envios_finalizados++;
}

/*
Método que devuelve el contador de paquetes cuyo envío ha sido finalizado.
*/
int
Trazas::GetEnviosFin()
{
  return envios_finalizados;
}
