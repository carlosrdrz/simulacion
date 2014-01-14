// servidor.cc

#include "servidor.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Servidor");

Servidor::Servidor(Ptr< Node > disp)
:Application(),m_node(PeekPointer(disp))
{
  DoInitialize();
}

/// Recibe la petición web. Se asume que es válida siempre.
void Servidor::RecibePaquete (Ptr<Socket> socket)
{
	NS_LOG_FUNCTION (this << socket);
	while (socket->GetRxAvailable () > 0)
	{
		Address from;
		Ptr<Packet> p = socket->RecvFrom (0xffffffff, 0, from);
		NS_LOG_DEBUG ("recv " << p->GetSize () << " bytes");
		//NS_ASSERT (InetSocketAddress::IsMatchingType (from));
		//InetSocketAddress realFrom = InetSocketAddress::ConvertFrom (from);
		//NS_ASSERT (realFrom.GetPort ()); // assure port 80 or 443

		RespondePaquete(socket);
	}
}

void Servidor::AceptaConexion (Ptr<Socket> s, const Address& from)
{
	NS_LOG_FUNCTION (this << s << from);
	s->SetRecvCallback (MakeCallback (&Servidor::RecibePaquete, this));
	// m_socketList.push_back (s);
}

/// Método que envía un nuevo paquete.
/// Deberá iniciar el temporizador de retransmisiones
void Servidor::RespondePaquete(Ptr<Socket> s)
{
	NS_LOG_FUNCTION(this << s);
	//const char cadena_prueba[] = "hola";
	//Ptr<Packet> paquetePrueba = new Packet((uint8_t *)cadena_prueba,strlen(cadena_prueba),true);
	Ptr<Packet> enviar = new Packet(120); // TODO Tamaño aleatorio
	
	s->Send(enviar);
}

// Método de inicialización de la aplicación. Instala el Callback.
void Servidor::DoInitialize()
{
 NS_LOG_FUNCTION (this);

 m_socket = Socket::CreateSocket (m_node, TypeId::LookupByName ("ns3::TcpSocketFactory"));
 NS_ASSERT (m_socket != 0);
 // m_socket->SetAttribute ("Protocol", UintegerValue (6)); // tcp

 InetSocketAddress src = InetSocketAddress (Ipv4Address::GetAny (), 8421);
 int status;
 status = m_socket->Bind (src);
 NS_ASSERT (status != -1);
 status = m_socket->Listen();
 
  m_socket->SetAcceptCallback (
    MakeNullCallback<bool, Ptr<Socket>, const Address &> (),
    MakeCallback (&Servidor::AceptaConexion, this));

  // Application::DoInitialize();
}
