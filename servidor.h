
#pragma once

#include "ns3/core-module.h"
#include "ns3/net-device.h"
#include "ns3/application.h"
#include "ns3/internet-module.h"



// @TODO Enviar datos variables. En el constructor se le pasan la VA
class Servidor : public ns3::Application
{
public:
  Servidor(ns3::Ptr<ns3::Node> disp);

  // Comprueba si la secuencia del paquete es la adecuada.
  // Si lo es, actualiza el valor de la ventana de recepción.
  // En cualquier caso envía un nuevo ACK
  void RecibePaquete(ns3::Ptr<ns3::Socket> socket);

  // Método que envía un nuevo paquete.
  void RespondePaquete(ns3::Ptr<ns3::Socket> s);

  void AceptaConexion (ns3::Ptr<ns3::Socket> s, const ns3::Address& from);

  // void StartApplication (void);
  
  // Método de inicialización de la aplicación. Instala el Callback.
  void DoInitialize();
private:
  ns3::Ptr< ns3::Node > m_node;
  ns3::Ptr<ns3::Socket> m_socket;
  std::list<ns3::Ptr<ns3::Socket> > m_socketList; 
};
