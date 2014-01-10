//#if 0
/*
  Fichero para la clase Trazas
  A partir de esta clase se obtendrán las trazas 
  del envío de paquetes.
*/

#include "trazas.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Trazas");

Trazas::~Trazas()
{
  NS_LOG_FUNCTION(this);
  for (std::map<std::string,Datos*>::iterator it=valores.begin(); it!=valores.end(); ++it) {
    delete it->second;
  }
  valores.clear();
}

//Metodo para comenzar a monitorizar los routers
void Trazas::Monitorize(std::string id, Ptr<NetDevice> r)
{
  Datos * datos_disp = new Datos();
  Ptr<PointToPointNetDevice> device = r->GetObject<PointToPointNetDevice>();
  device->TraceConnectWithoutContext ("PhyTxEnd", MakeCallback (&Datos::DispositivoEnvia, datos_disp));
  device->TraceConnectWithoutContext ("PhyRxEnd", MakeCallback (&Datos::DispositivoRecibe, datos_disp));
  device->TraceConnectWithoutContext ("PhyRxDrop", MakeCallback (&Datos::DispositivoPierde, datos_disp));
  valores.insert(std::pair<std::string, Datos*>(id, datos_disp));
}
//Método que devuelve el contador de paquetes enviados
int
Trazas::GetPaquetesEnviados(std::string id)const
{
  return valores.at(id)->paquetesEnviados;
}
//Método que devuelve el contador de paquetes recibidos
int
Trazas::GetPaquetesRecibidos(std::string id)const
{
  return valores.at(id)->paquetesRecibidos;
}
//Método que devuelve los paquetes perdidos por el dispositivo.
int
Trazas::GetPaquetesPerdidos(std::string id)const
{
  return valores.at(id)->paquetesPerdidos;
}
//Método que devuelve el contador de bytes enviados
int
Trazas::GetBytesEnviados(std::string id)const
{
  return valores.at(id)->bytesEnviados;
}
//Método que devuelve el contador de bytes recibidos. 
int
Trazas::GetBytesRecibidos(std::string id)const
{
  return valores.at(id)->bytesRecibidos;
}
//Método que devuelve el contador de bytes perdidos.
int
Trazas::GetBytesPerdidos(std::string id)const
{
  return valores.at(id)->bytesPerdidos;
}
//Método que muestra los valores de los contadores
void
Trazas::ImprimeTrazas()
{
 std::cout << "#################" << std::endl;
 std::cout << "Trazas obtenidas: " << std::endl;
 std::cout << "#################" << std::endl;
  for (std::map<std::string,Datos*>::const_iterator it=valores.begin(); it!=valores.end(); ++it) {
    const std::string id = it->first;
    std::cout << "Paquetes enviados por el dispositivo " << id << ": " << GetPaquetesEnviados(id) << std::endl;
    std::cout << "Paquetes recibidos por el dispositivo " << id << ": " << GetPaquetesRecibidos(id) << std::endl;
    std::cout << "Paquetes perdidos por el dispositivo " << id << ": " << GetPaquetesPerdidos(id) << std::endl;
    
    std::cout << "Bytes enviados por el dispositivo " << id << ": " << GetBytesEnviados(id) << std::endl;
    std::cout << "Bytes recibidos por el dispositivo " << id << ": " << GetBytesRecibidos(id) << std::endl;
    std::cout << "Bytes perdidos por el dispositivo " << id << ": " << GetBytesPerdidos(id) << std::endl;
  }
}

//#endif
