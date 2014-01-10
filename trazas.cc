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
  std::cout << "Destructor trazas" << std::endl;
  for (std::map<int,Datos*>::iterator it=valores.begin(); it!=valores.end(); ++it) {
    delete it->second;
  }
  valores.clear();
}

//Metodo para comenzar a monitorizar los routers
void Trazas::Monitorize(int id, Ptr<NetDevice> r)
{
  Datos * datos_disp = new Datos();
  Ptr<PointToPointNetDevice> device = r->GetObject<PointToPointNetDevice>();
  device->TraceConnectWithoutContext ("PhyTxEnd", MakeCallback (&Datos::DispositivoEnvia, datos_disp));
  device->TraceConnectWithoutContext ("PhyRxEnd", MakeCallback (&Datos::DispositivoRecibe, datos_disp));
  valores.insert(std::pair<int, Datos*>(id, datos_disp));
}
//Método que devuelve el contador de paquetes enviados
int
Trazas::GetPaquetesEnviados(int id)
{
  return valores.at(id)->paquetesEnviados;
}
//Método que devuelve el contador de paquetes recibidos
int
Trazas::GetPaquetesRecibidos(int id)
{
  return valores.at(id)->paquetesRecibidos;
}
//Método que devuelve el contador de bytes enviados
float
Trazas::GetBytesEnviados(int id)
{
  return valores.at(id)->bytesEnviados;
}
//Método que devuelve el contador de bytes recibidos. 
float
Trazas::GetBytesRecibidos(int id)
{
  return valores.at(id)->bytesRecibidos;
}
void
Trazas::ImprimeTrazas()
{
  std::cout << "Trazas obtenidas: " << std::endl;
  for (std::map<int,Datos*>::iterator it=valores.begin(); it!=valores.end(); ++it) {
    int id = it->first;
    std::cout << "Paquetes enviados por el dispositivo " << id << ": " << GetPaquetesEnviados(id) << std::endl;
    std::cout << "Paquetes recibidos por el dispositivo " << id << ": " << GetPaquetesRecibidos(id) << std::endl;
    std::cout << "Bytes enviados por el dispositivo " << id << ": " << GetBytesEnviados(id) << std::endl;
    std::cout << "Bytes recibidos por el dispositivo " << id << ": " << GetBytesRecibidos(id) << std::endl;
  }
}

//#endif